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
	int toInt(string n);

	/**
	*	converte o numero para um array de bytes com notacao little-endian
	*	o ultimo caractere determina o tipo do numero:
	*	b/B - binario
	*	h/H - hexadecimal
	*	nada/algarismo - binario
	*	escreve o numero de bytes do numero em size
	* nao suporta numeros decimais
	*/
	unsigned char *toByteArray(string n, int *size);

	/**
	* determina o tipo do numero (decimal, binario ou hexadecimal),retornando-o
	* retorna INVALID se o numero nao estiver no formato adequado
	*/
	e_numType numberType(string n);

	/**
	* converte os caracteres do numero para seus respectivos valores
	* o vetor values deve ser grande o suficiente
	* o numero n deve ser valido
	*/
	void convertDigits(string n, unsigned char *values,e_numType type);
};

#endif // NUMBERS_HPP

