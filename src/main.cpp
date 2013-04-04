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

using namespace std;

int main(int argc,char *argv[])
{
	bool help = false;

	if(argc <=3)
		help = true;
	else
	{
		int i;
		for(i=1;i<argc;i++)
		{

		}
	}

	//mostra mensagem de ajuda
	if(help)
	{
		printf("usage: hidrassembler [OPTIONS...] machine=<machine> src=<source file>\n\n");
		printf("available options:\n");
		printf("output=<filename>\t generated binary will be written to <filename> instead of stdout\n");
		printf("warnings=<filename>\t generated warnings will be written to <filename> instead of stderr\n");
		printf("errors=<filename>\t generated errors will be written to <filename> instead of stderr\n");
		printf("symbols=<filename>\t defined symbols will be written to <filename>\n");
	}

}
