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

#ifndef REGISTERS_HPP
#define REGISTERS_HPP

#include <string>
#include <map>

using namespace std;

typedef struct s_register
{
	string name;
}t_register;

class	Registers
{

	public:

	Registers();

	/**
	*	carrega os registradores que estao definidas na string config
	*/
	void load(string config);

	/**
	*	retorna o numero do registrador caso ele exista,
	* -1 caso nao exista
	*/
	int number(string regName);

	private:

	map<string,t_register> regs;

};

#endif // REGISTERS_HPP
