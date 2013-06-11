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
#include <math.h>

#include <string>
#include <list>

#include "registers.hpp"
#include "stringer.hpp"
#include "numbers.hpp"
#include "defs.hpp"

#include "debug.hpp"

using namespace std;

Registers::Registers()
{

}

/**
*	carrega os registradores que estao definidas na string config
*/
void Registers::load(string config)
{

	list<string> words = stringReadWords(config,"",'\0','#');
	if(words.size() != 2)
		throw(eInvalidFormat);

	list<string>::iterator it = words.begin();

	t_register r;
	r.name = *(it++);
	//o indice deve ser em binario
	r.index = Number::toBin(*(it++));

	this->regs[r.name] = r;

	this->adjustRegistersIndexes();

}

/**
  * garante que todos os registradores possuam o numero certo de digitos
  */
void Registers::adjustRegistersIndexes()
{
	map<string,t_register>::iterator it;
	unsigned int numDigits = floor(log10((double)this->regs.size())/LOG102)+1;
	for(it=this->regs.begin() ; it!=this->regs.end() ; it++)
	{
		//garante que o indice do registrador possui o numero correto de digitos
		t_register r = it->second;

		if(numDigits - (r.index.size()-1) > 0)
		{
			string leftZeroes(numDigits-r.index.size()+1,'0');
			r.index = leftZeroes.append(r.index);
		}

		string name = it->first;
		this->regs[name] = r;
	}
}

/**
*	retorna o numero do registrador caso ele exista,
* string vazia caso nao exista
*/
string Registers::number(string regName)
{
	map<string,t_register>::iterator it = this->regs.find(regName);
	if(it == this->regs.end())
		return string();
	else
		return it->second.index;
}

/**
	* verifica se um registrador existe ou nao
	*/
bool Registers::exists(string regName)
{
	if(this->regs.find(regName) == this->regs.end())
		return false;
	else
		return true;
}

/**
	* escreve os dados do objeto em stream
	*/
void Registers::print(FILE *stream)
{

	map<string,t_register>::iterator it;

	for(it=this->regs.begin() ; it!=this->regs.end() ; it++)
	{
		t_register r = it->second;
		fprintf(stream,"%s\n",r.name.c_str());
	}

}

