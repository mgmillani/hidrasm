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

#ifndef ADDRESSINGS_HPP
#define ADDRESSINGS_HPP

#include <stdio.h>

#include <string>
#include <map>

#include "expression.hpp"

using namespace std;

typedef struct s_addressing
{
	string expStr;
	Expression expression;
	string code;
	bool relative;
}t_addressing;

class Addressings
{

	public:

	Addressings();

	/**
	*	carrega os modos de enderecamento que estao definidos na string config
	*/
	void load(string config);

	/**
	*	retorna a estrutura do modo de enderecamento com o nome dado
	*/
	t_addressing getAddressing(string name);

	/**
	  * escreve os dados da estrutura em stream
	  */
	void print(FILE *stream);

	void printAddressing(t_addressing *a, FILE *stream);

	private:

	map<string,t_addressing> addrs;	//associa o nome do modo de enderecamento com seus atributos

};

#endif // ADDRESSINGS_HPP
