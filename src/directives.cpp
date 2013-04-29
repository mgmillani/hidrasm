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

#include <string>

#include "directives.hpp"
#include "stringer.hpp"
#include "numbers.hpp"
#include "memory.hpp"
#include "defs.hpp"

using namespace std;

/**
* executa a diretiva, retornando em qual byte a montagem deve continuar
*/
unsigned int Directives::execute(string directive,string operands,Memory *memory,unsigned int currentByte)
{
	Number n;
	//muda o proximo byte par amontagem
	if(stringCaselessCompare(directive,"org")==0)
	{
		//o operando deve ser um numero
		return n.toInt(operands);
	}
	else if(stringCaselessCompare(directive,"db")==0)
	{
		int value = n.toInt(operands);
		memory->writeValue((unsigned char)value,currentByte);
		currentByte++;
	}
	else if(stringCaselessCompare(directive,"dw")==0)
	{

	}
	else if(stringCaselessCompare(directive,"dab")==0)
	{

	}
	else if(stringCaselessCompare(directive,"daw")==0)
	{

	}

	return currentByte;
}

/**
* verifica se a string passada corresponde a uma diretiva
*/
bool Directives::isDirective(string directive)
{

	if(stringCaselessCompare(directive,"org")==0)
		return true;
	else if(stringCaselessCompare(directive,"db")==0)
		return true;
	else if(stringCaselessCompare(directive,"dw")==0)
		return true;
	else if(stringCaselessCompare(directive,"dab")==0)
		return true;
	else if(stringCaselessCompare(directive,"daw")==0)
		return true;
	return false;
}
