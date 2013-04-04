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
#include <map>
#include <set>

#include "labels.hpp"
#include "defs.hpp"

using namespace std;

/**
*	adiciona a definicao de uma label
*/
void Labels::define(string name,unsigned int pos,unsigned int line)
{

	if(this->defs.find(name) == this->defs.end())
	{
		this->defs[name] = pos;
		this->lines[name] = line;
		//marca como nao referenciada
		this->unrefs.insert(name);
	}
}

/**
*	retorna a posicao em que a label foi definida
*	conta como uma referencia a label
* se a label ainda nao foi definida, throws eUndefinedLabel
*/
unsigned int Labels::value(string name)
{
	map<string,unsigned int>::iterator i = this->defs.find(name);
	if(i == this->defs.end())
	{
		//label referenciada mas nao definida
		throw(eUndefinedLabel);
	}
	else
	{
		//marca a label como referenciada
		this->unrefs.erase(name);
		return i->second;
	}
}

/**
* retorna a linha em que a label foi definida
* nao conta como uma referencia
* se a label ainda nao foi definida, throws eUndefinedLabel
*/
unsigned int Labels::line(string name)
{
	map<string,unsigned int>::iterator i = this->lines.find(name);
	if(i == this->defs.end())
	{
		//label referenciada mas nao definida
		throw(eUndefinedLabel);
	}
	else
	{
		return i->second;
	}
}
