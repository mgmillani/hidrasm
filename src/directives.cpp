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
#include <stack>

#include "directives.hpp"
#include "stringer.hpp"
#include "numbers.hpp"
#include "labels.hpp"
#include "memory.hpp"
#include "defs.hpp"

#include "debug.hpp"

using namespace std;

/**
* executa a diretiva, retornando em qual byte a montagem deve continuar
*/
unsigned int Directives::execute(string directive, string operands, Labels labels, stack<t_pendency> *pendencies, Memory *memory,unsigned int currentByte)
{
	Number n;
	unsigned int size;
	list<string> ops = stringSplitCharProtected(operands," ,\t","'\"",'\\');
	//muda o proximo byte par amontagem
	if(stringCaselessCompare(directive,"org")==0)
	{
		//o operando deve ser um numero
		return n.toInt(operands);
	}
	else if(stringCaselessCompare(directive,"db")==0)
	{
		size = 1;
	}
	else if(stringCaselessCompare(directive,"dw")==0)
	{
		size = 2;
	}
	else if(stringCaselessCompare(directive,"dab")==0)
	{
		size = 1;
	}
	else if(stringCaselessCompare(directive,"daw")==0)
	{
		size = 2;
	}
	//array de valores
	list<string>::iterator ot;

	for(ot=ops.begin() ; ot!=ops.end() ; ot++)
	{
		//se existir uma label com esse nome, usa seu valor
		if(labels.exists(*ot))
			memory->writeNumber(Number::toBin(labels.value(*ot)),currentByte,size);
		else if(Number::exists(*ot))
			memory->writeNumber(Number::toBin(*ot),currentByte,size);
		//se nao for nada conhecido, adiciona uma pendencia
		else
		{
			t_pendency p;
			p.byte = currentByte;
			t_operand op;
			op.name = *ot;
			op.type = TYPE_LABEL;
			op.relative = false;
			p.operands.push_back(op);
			p.binFormat = "a";
			p.size = size*8;
			pendencies->push(p);
		}
		currentByte += size;
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
