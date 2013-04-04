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

#ifndef DIRECTIVES_HPP
#define DIRECTIVES_HPP

#include <string>

#include "memory.hpp"

using namespace std;

class Directives
{

	public:

	/**
	* executa a diretiva, retornando em qual byte a montagem deve continuar
	*/
	unsigned int execute(string directive,string operands,Memory *memory,unsigned int currentByte);

	/**
	* verifica se a string passada corresponde a uma diretiva
	*/
	bool isDirective(string name);

	private:

};

#endif // DIRECTIVES_HPP

