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

#ifndef NUMBERS_HPP
#define NUMBERS_HPP

#include <string>

using namespace std;

typedef enum {BINARY,DECIMAL,HEXADECIMAL,INVALID} e_numType;

class Number
{
	public:
	Number();

	/**
	  *	converte o numero para um inteiro
	  *	caso seja maior que um, trunca-o, retornando somente os bits menos significativos
	  * o ultimo caractere determina o tipo do numero:
	  * b/B - binario
	  * d/D - decimal
	  * h/H - hexadecimal
	  * nada/algarismo - decimal
	  */
	static int toInt(string n);

	/**
	  * converte a string do numero dado para uma string binaria (terminada com 'b')
	  */
	static string toBin(string n);
	static string toBin(unsigned int n);

	/**
	  * escreve o valor binario de um digito hexadecimal em dest
	  */
	static void writeHexaDigitAsBin(char digit, char *dest);

	/**
	  * retorna um string contendo apenas os digitos do numero, sem 0s a esquerda
	  * e sem indicativos de sua base
	  */
	static string getMinDigits(string n);

	/**
	  *	converte o numero para um array de bytes com notacao little-endian
	  *	o ultimo caractere determina o tipo do numero:
	  *	b/B - binario
	  *	h/H - hexadecimal
	  *	nada/algarismo - binario
	  *	escreve o numero de bytes do numero em size
	  * nao suporta numeros decimais
	  */
	static unsigned char *toByteArray(string n, int *size);

	/**
	  * determina o tipo do numero (decimal, binario ou hexadecimal),retornando-o
	  * retorna INVALID se o numero nao estiver no formato adequado
	  */
	static e_numType numberType(string n);

	/**
	  * converte os caracteres do numero para seus respectivos valores
	  * o vetor values deve ser grande o suficiente
	  * o numero n deve ser valido
	  */
	static void convertDigits(string n, unsigned char *values,e_numType type);

	/**
	  * verifica se o dado numero eh valido
	  */
	static bool exists(string number);
};

#endif // NUMBERS_HPP

