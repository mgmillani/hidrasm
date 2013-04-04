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

#include <string>
#include <list>

#include "registers.hpp"
#include "stringer.hpp"
#include "defs.hpp"

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
	if(words.size() != 1)
		throw(eInvalidFormat);

	t_register r;
	r.name = *(words.begin());

	this->regs[r.name] = r;

}

/**
*	retorna o numero do registrador caso ele exista,
* -1 caso nao exista
*/
int Registers::number(string regName)
{
	return 0;
}
