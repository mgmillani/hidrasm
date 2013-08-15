/**
* Copyright 2013 Marcelo Millani
*	This file is part of hidrasm.
*
* hidrasm is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* hidrasm is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with hidrasm.  If not, see <http://www.gnu.org/licenses/>
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <string>

#include "numbers.hpp"
#include "defs.hpp"
#include "debug.hpp"

using namespace std;

Number::Number()
{
	this->numDigits = 0;
	this->digits = NULL;
}

/**
  * inicializa o numero a partir de uma string binaria, decimal ou hexadecimal
  */
Number::Number(string n)
{

	e_numType type = numberType(n);
	if(type == INVALID)
		throw(eInvalidFormat);
	if(type == DECIMAL)
	{
		int value = toInt(n);
		this->digits = (unsigned char *)malloc(sizeof(value));
		unsigned int i;
		for(i=0 ; i<sizeof(value) ; i++)
		{
			this->digits[i] = value;
			value >>= 8;
		}
		this->numDigits = sizeof(value);
	}
	else
	{
		this->digits = toByteArray(n,&this->numDigits);
	}

}

Number::~Number()
{
	if(this->digits != NULL)
		free(this->digits);
}

/**
  * aplica a operacao dada usando n como o segundo operando
  * escreve o resultado neste objeto
  */
void Number::operate(char operation,Number n)
{

	switch(operation)
	{
		//a - b = a + (-b)
		case '-':
			n.baseComplement();
		case '+':
			{
				unsigned int i;
				int carry = 0;
				for(i=0 ; i<n.numDigits && i<this->numDigits ; i++)
				{
					unsigned int a = this->digits[i];
					unsigned int b = n.digits[i];
					this->digits[i] = a+b+carry;
					carry = (a+b+carry) >> 8;
				}
				//se o outro numero for menor, apenas propaga o carry
				if(i<this->numDigits)
				{
					while(carry > 0 && i<this->numDigits)
					{
						unsigned int a = this->digits[i];
						this->digits[i] = a+carry;
						carry = (a+carry)>>8;
						i++;
					}
					//se existir carry, sera necessario aumentar o numero
					if(carry>0)
					{
						this->numDigits++;
						this->digits = (unsigned char *)realloc(this->digits,this->numDigits);
						this->digits[this->numDigits-1] = carry;
					}
				}
				//se o outro numero for maior, sera necessario aumentar o atual
				else if(i<n.numDigits)
				{
					this->numDigits = n.numDigits+1;
					this->digits = (unsigned char *)realloc(this->digits,this->numDigits);

					//adiciona os digitos que faltaram
					for(; i<n.numDigits ; i++)
					{
						unsigned int b = n.digits[i];
						this->digits[i] = b+carry;
						carry = (b+carry)>>8;
					}
					this->digits[this->numDigits-1] = carry;
				}
			}
			break;
		default:
			throw(eInvalidOperation);
	}

}

/**
  * determina o tipo do numero (decimal, binario ou hexadecimal),retornando-o
  * retorna INVALID se o numero nao estiver no formato adequado
  */
e_numType Number::numberType(string n)
{
	if(n.size()==0)
		return INVALID;
	unsigned int end = n.size()-1;
	char d = n[end];
	switch(d)
	{
		default:
			end++;
		case 'd':
		case 'D':
			end--;
			int i;
			for(i=end; i >0 ; i--)
			{
				if(n[i]<'0' || n[i]>'9')
					return INVALID;
			}
			if(n[0]=='-' || (n[0]>='0' && n[0]<='9'))
				return DECIMAL;
			else
				return INVALID;
			break;
		case 'b':
		case 'B':
			end--;
			for(i=end; i >0 ; i--)
			{
				if(n[i]<'0' || n[i]>'1')
					return INVALID;
			}
			if(n[0]=='-' || (n[0]>='0' && n[0]<='1'))
				return BINARY;
			else
				return INVALID;
			break;
		case 'h':
		case 'H':
			end--;
			for(i=end; i >0 ; i--)
			{
				if(n[i]>='0' && n[i]<='9')
					continue;
				else if(n[i]>='A' && n[i]<='F')
					continue;
				else if(n[i]>='a' && n[i]<='f')
					continue;
				else
					return INVALID;
			}
			if(n[0]=='-' || (n[0]>='0' && n[0]<='9') || (n[i]>='A' && n[i]<='F') || (n[i]>='a' && n[i]<='f'))
				return HEXADECIMAL;
			else
				return INVALID;
			break;
	}
}


/**
  * converte os caracteres do numero para seus respectivos valores
  * o vetor values deve ser grande o suficiente
  * o numero n deve ser valido
  */
void Number::convertDigits(string n, unsigned char *values,e_numType type)
{
	int min = 0;
	int i;
	int max = n.size();
	switch(type)
	{
		case DECIMAL:
			if(n[max-1] == 'd' || n[max-1] == 'D')
				max--;
			if(n[0] == '-')
				min=1;

			for(i=min ; i<max ; i++)
				values[i] = n[i]-'0';
			break;
		case BINARY:
			if(n[0] == '-')
				min=1;

			int i;
			for(i=min ; i<max ; i++)
				values[i] = n[i]-'0';
			break;
		case HEXADECIMAL:
			if(n[0] == '-')
				min=1;

			for(i=min ; i<max ; i++)
			{
				if(n[i]>='0' && n[i]<='9')
					values[i] = n[i] - '0';
				else if(n[i]>='A' && n[i]<='F')
					values[i] = n[i] - 'A' + 10;
				else
					values[i] = n[i] - 'a' + 10;
			}
			break;
		default:
			break;
	}
}

/**
  *	converte o numero para um inteiro
  * caso seja maior que um, trunca-o, retornando somente os bits menos significativos
  * o ultimo caractere determina o tipo do numero:
  * b/B - binario
  * d/D - decimal
  * h/H - hexadecimal
  * nada/algarismo - decimal
  * se for encontrado um numero desconhecido, retorna 0 (REVER ISSO)
  */
int Number::toInt(string n)
{
	int base = 2;
	int power = 1;
	int end = n.size()-1;
	e_numType type = numberType(n);
	char last = n[end];
	switch(type)
	{
		case DECIMAL:
			if(last != 'd' && last!='D')
				end++;
			base = 10;
			break;
		case BINARY:
			base = 2;
			break;
		case HEXADECIMAL:
			base = 16;
			break;
		case INVALID:
			throw (eInvalidFormat);
	}
	int begin = 0;
	bool negative = false;
	if(n[0] == '-')
	{
		negative = true;
		begin = 1;
	}

	unsigned char values[n.size()];
	convertDigits(n,values,type);

	//ignora o digito que indica a base
	end--;
	int val = 0;
	//calcula o valor do numero
	int i;
	for(i=end; i>=begin ; i--)
	{
		val += values[i]*power;
		power *= base;
	}

	if(negative)
		val = -val;

	return val;
}

/**
  * converte a string do numero dado para uma string binaria (terminada com 'b')
  */
string Number::toBin(string n)
{

	switch(Number::numberType(n))
	{
		case DECIMAL:
			{
				int num = toInt(n);
				string val = toBin((unsigned int)num);
				return val;
			}
			break;
		case HEXADECIMAL:
			{
				string number = getMinDigits(n);
				unsigned int size = number.size()*4;
				//cada caractere representa 4 bits
				char *result = (char *)malloc(sizeof(*result)*(size+2));
				result[size] = 'b';
				result[size+1] = '\0';
				unsigned int i,j;
				for(i=0,j=0 ; i<number.size() ; i++,j+=4)
				{
					writeHexaDigitAsBin(number[i],result+j);
				}
				string end(result);
				free(result);
				return end;
			}
			break;
		default:
			return n;
	}
}

string Number::toBin(unsigned int n)
{
	unsigned int size = LOG2(n)+1;

	char *result = (char *)malloc(size+2);
	result[size] = 'b';
	result[size+1] = '\0';

	int d;
	for(d = size-1; d>=0 ; d--)
	{
		//1 se n impar, 0 se par
		result[d] = '0' + (n&1);
		n>>=1;
	}

	result[size] = 'b';
	result[size+1] = '\0';
	string end(result);
	free(result);
	return end;
}

/**
  * converte uma string de caracteres para um numero binario (terminado por 'b')
  * cada carcatere sera convertido para 8 bits na ordem em que aparecer
  */
string Number::stringToBin(string str)
{

	char bits[str.size()*8];

	unsigned int i;
	unsigned int w=0;
	bool escape = false;
	for(i=1 ; i+1<str.size() ; i++)
	{
		unsigned char c = str[i];

		if(c == '\\' && !escape)
			escape = true;
		else
		{
			escape = false;
			unsigned int j;
			for(j=0 ; j<8 ; j++)
			{
				if(c & 128)
					bits[w++] = '1';
				else
					bits[w++] = '0';
				c <<= 1;
			}
		}
	}

	bits[w] = 'b';
	bits[w+1] = '\0';
	return string(bits);

}

/**
  * escreve o valor binario de um digito hexadecimal em dest
  */
void Number::writeHexaDigitAsBin(char digit, char *dest)
{
	unsigned int v;
	//determina o valor do caractere
	if(digit>='0' && digit<='9')
		v = digit - '0';
	else if(digit>='A' && digit<='F')
		v = digit - 'A' + 10;
	else
		v = digit - 'a' + 10;

	//converte para binario
	int d;
	for(d=3; d>=0 ; d--)
	{
		//1 se v impar, 0 se v par
		dest[d] = '0' + (v&1);
		v = v>>1;
	}
}

/**
  * retorna um string contendo apenas os digitos do numero, sem 0s a esquerda
  * e sem indicativos de sua base
  */
string Number::getMinDigits(string n)
{
	unsigned int i = 0;
	bool negative = false;
	if(n[0] == '-')
	{
		negative = true;
		i++;
	}
	for(; i<n.size() ; i++)
	{
		if(n[i] != '0')
			break;
	}
	string result;
	char last = toupper(n[n.size()-1]);
	//se o ultimo caractere indicar a base
	if(last == 'D' || last == 'B' || last == 'H')
		result = n.substr(i,n.size()-i-1);
	else
		result = n.substr(i,n.size()-i);

	if(negative)
		return "-" + result;
	else
		return result;

}

/**
  * faz o complemento de 2 do numero, ou seja, troca o sinal
  */
void Number::baseComplement()
{

	unsigned int i;
	unsigned int carry = 1;
	for(i=0 ; i<this->numDigits ; i++)
	{
		int a = ~this->digits[i];
		this->digits[i] = a+carry;
		carry = (a+carry) >> 8;
	}

}

/**
  *	converte o numero para um array de bytes com notacao little-endian
  *	o ultimo caractere determina o tipo do numero:
  *	b/B - binario
  *	h/H - hexadecimal
  *	escreve o numero de bytes do numero em size
  * nao suporta numeros decimais
  */
unsigned char *Number::toByteArray(string n, unsigned int *size)
{

	unsigned char values[n.size()];
	unsigned char *number = NULL;
	e_numType type = numberType(n);

	if(type == INVALID || type==DECIMAL)
	{
		*size = 0;
		return NULL;
	}

	convertDigits(n,values,type);

	int first=0;
	int last = n.size()-1;
	if(n[0] == '-')
		first=1;
	if(n[last] <'0' || n[last]>'9')
		last--;

	int len = last-first+1;
	int i;
	int max=0;
	int byte;
	int pos = n.size()-2;
	switch(type)
	{
		case BINARY:
			//cada digito representa um bit
			//numero de bytes = digitos/8
			max = ceil(len/8.0);
			number = (unsigned char *)malloc(max);
			for(byte=0 ; byte<max ; byte++)
			{
				number[byte] = 0;
				for(i=0 ; i<=7 && pos<len && pos>=0 ; i++,pos--)
					number[byte] |= (values[pos]<<i);
			}

			break;
		case HEXADECIMAL:
			//numero de bytes = digitos/2
			max = ceil(len/2.0);
			number = (unsigned char *)calloc(max,1);

			for(byte=0 ; byte<max ; byte++)
			{
				char shift;
				for(shift=0, i=n.size()-2 -byte*2; shift<=4 && i>=0 ; i--,shift+=4)
					number[byte] |= values[i]<<shift;
			}

			break;
	}

	*size = max;

	return number;

}

/**
  * verifica se o dado numero eh valido
  */
bool Number::exists(string number)
{

	e_numType num = numberType(number);
	if(num == INVALID)
	{

		return false;
	}
	else
		return true;

}

/**
  * verifica se o elemento passado eh uma string valida
  */
bool Number::isString(string element)
{

	char first = element[0];
	if(first != '\'' && first != '"')
		return false;

	size_t max = element.size();
	if(element[max-1] != first)
		return false;
	if(element[max-2] == '\\')
		return false;
	return true;
}




