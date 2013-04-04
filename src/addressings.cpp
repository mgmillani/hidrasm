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

#include "addressings.hpp"
#include "defs.hpp"

using namespace std;

Addressings::Addressings()
{

}

/**
*	carrega os modos de enderecamento que estao definidos na string config
*/
void Addressings::load(string config)
{
	typedef enum {STATE_INI,STATE_NAME,STATE_RELATIVE,STATE_SKIP,STATE_CODE,STATE_EXP,STATE_END} e_state;

	e_state state = STATE_INI;
	e_state nextState = STATE_INI;
	unsigned int i;
	unsigned int b;
	t_addressing addr;
	string name;
	bool comment = false;
	for(i=0 ; i<config.size() && !comment ; i++)
	{
		char c = config[i];
		switch(state)
		{
			//le espacos a esquerda
			case STATE_INI:
				//comentario
				if(c=='#')
					comment = true;
				else if(!ISWHITESPACE(c))
				{
					b = i;
					state = STATE_NAME;
				}
				break;
			//le o nome interno do modo de enderecamento
			case STATE_NAME:

				if(ISWHITESPACE(c))
				{
					name = config.substr(b,i-b);
					state = STATE_RELATIVE;
				}
				break;
			//determina se eh relativo ao pc ou nao
			case STATE_RELATIVE:
				if(c == '=')
				{
					addr.relative = false;
					state = STATE_SKIP;
					nextState = STATE_CODE;
				}
				else if(c=='-')
				{
					addr.relative = true;
					state = STATE_SKIP;
					nextState = STATE_CODE;
				}
				else if(!ISWHITESPACE(c))
				{
					addr.relative = false;
					b = i;
					state = STATE_CODE;
				}
				break;
			//pula caracteres em branco
			case STATE_SKIP:
				if(!ISWHITESPACE(c))
				{
					state = nextState;
					b = i;
				}
				break;
			//le o codigo binario do modo de enderecamento
			case STATE_CODE:
				if(ISWHITESPACE(c))
				{
					addr.code = config.substr(b,i-b);
					state = STATE_SKIP;
					nextState = STATE_EXP;
				}
				break;
			//le a expressao que identifica o modo de enderecamento
			case STATE_EXP:
				if(ISWHITESPACE(c))
				{
					addr.exp = config.substr(b,i-b);
					state = STATE_END;
				}
				break;
			case STATE_END:
				if(c=='#')
					comment = true;
				else if(!ISWHITESPACE(c))
					throw(eInvalidFormat);
				break;
		}//end switch
	}//end for

	if(state != STATE_END && !comment)
	{
		addr.exp = config.substr(b,i-b);
	}

	this->addrs[name] = addr;
}

/**
*	retorna a estrutura do modo de enderecamento com o nome dado
*/
t_addressing getAddressing(string name)
{
	return this->addrs.find(name)->second;
}
