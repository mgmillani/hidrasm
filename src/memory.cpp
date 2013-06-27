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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "numbers.hpp"
#include "memory.hpp"

#include "debug.hpp"

//using namespace std;

/**
  *	inicializa uma memoria com suporte de, pelo menos, size bytes
  */
Memory::Memory(unsigned int size,bool bigEndian)
{
	this->bigEndian = bigEndian;
	this->area = (unsigned char*)calloc(size,1);
	this->size = size;
}

Memory::~Memory()
{
	if(this->size>0)
		free(this->area);
}

/**
  *	escreve um valor numa determinada posicao da memoria
  */
void Memory::writeValue(unsigned char value,unsigned int pos)
{
	this->area[pos] = value;
}

/**
  *	escreve todos os valores do array a partir da posicao startPos na memoria
  */
void Memory::writeArray(unsigned char *array, unsigned int arraySize, unsigned int startPos)
{
	memcpy(this->area+startPos,array,arraySize);
}

/**
  * escreve um numero a partir de uma determinada posicao
  * usa a notacao adequada
  * retorna o numero de bytes escritos
  */
unsigned int Memory::writeNumber(string number,unsigned int startPos)
{
	unsigned int size;
	unsigned char *array = Number::toByteArray(number,&size);
	if(this->bigEndian)
	{
		unsigned int i;
		for(i=0 ; i<size ; i++)
		{
			this->area[startPos+i] = array[size-i-1];
		}
	}
	else
		memcpy(this->area+startPos,array,size);
	free(array);
	return size;
}

/**
  *	le um o valor que esta na posicao dada da memoria
  */
unsigned char Memory::readValue(unsigned int pos)
{
	return this->area[pos];
}

/**
  *	le amount posicoes da memoria a partir da posicao dada, escrevendo os valores em array
  */
void Memory::readArray(unsigned char *array, unsigned int amount, unsigned int pos)
{
	memcpy(array,this->area+pos,amount);
}

/**
  *	retorna uma forma compactada da memoria
  *	o vetor retornado nao deve ser desalocado
  * (por enquanto, eh apenas o dump da memoria, sem nenhuma compactacao)
  */
unsigned char *Memory::pack(unsigned int *size)
{
	*size = this->size;
	return this->area;
}

/**
	* escreve todos os valores da memoria na stream dada
	*/
void Memory::print(FILE *stream)
{

	unsigned int i;
	for(i=0 ; i<this->size ; i++)
	{
		fprintf(stream,"%u:\t%d\t%s\n",i,(int)this->area[i],Number::toBin(this->area[i]).c_str());
	}

}
