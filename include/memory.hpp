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

#ifndef MEMORY_HPP
#define MEMORY_HPP

class Memory
{

	public:

	/**
	*	inicializa uma memoria com suporte de, pelo menos, size bytes
	*/
	Memory(unsigned int size);

	/**
	*	escreve um valor numa determinada posicao da memoria
	*/
	void writeValue(unsigned char value,unsigned int pos);

	/**
	*	escreve todos os valores do array a partir da posicao startPos na memoria
	*/
	void writeArray(unsigned char *array, unsigned int arraySize, unsigned int startPos);

	/**
	*	le um o valor que esta na posicao dada da memoria
	*/
	unsigned char readValue(unsigned int pos);

	/**
	*	le amount posicoes da memoria a partir da posicao dada, escrevendo os valores em array
	*/
	void readArray(unsigned char *array, unsigned int amount, unsigned int pos);

	/**
	*	retorna uma forma compactada da memoria
	*	o vetor retornado nao deve ser desalocado
	* (por enquanto, eh apenas o dump da memoria, sem nenhuma compactacao)
	*/
	unsigned char *pack(unsigned int *size);

	private:

	unsigned char *area;
	unsigned int size;

};

#endif // MEMORY_HPP

