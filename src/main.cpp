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
#include <string.h>

#include "messenger.hpp"
#include "assembler.hpp"
#include "file.hpp"

#include "debug.hpp"

using namespace std;

int main(int argc,char *argv[])
{
	bool help = false;

	FILE *source = stdin;
	FILE *output = stdout;
	FILE *errors = stderr;
	FILE *warnings = stderr;
	FILE *symbols = NULL;
	FILE *machine = NULL;
	FILE *messages = NULL;
	unsigned int version = 0;

	if(argc <=3)
		help = true;
	else
	{
		int i;
		//determina o que foi passado pela linha de comando
		for(i=1;i<argc;i++)
		{
			char *opt = argv[i];
			char *left = strtok(opt,"=");
			char *right = strtok(NULL,"");
			bool opened = true;
			FILE *file = NULL;

			if(strcmp(left,"source") == 0)
				file = source = fopen(right,"rb");
			else if(strcmp(left,"output") == 0)
				file = output = fopen(right,"wt");
			else if(strcmp(left,"warnings") == 0)
				file = warnings = fopen(right,"wt");
			else if(strcmp(left,"errors") == 0)
				file = errors = fopen(right,"wt");
			else if(strcmp(left,"symbols") == 0)
				file = symbols = fopen(right,"wt");
			else if(strcmp(left,"machine") == 0)
				file = machine = fopen(right,"rb");
			else if(strcmp(left,"messages") == 0)
				file = messages = fopen(right,"rb");
			else if(strcmp(left,"version") == 0)
			{
				opened = false;
				sscanf(right,"%u",&version);
				if(version != 0 && version != 3)
				{
					help = true;
					ERR("Invalid version: %u\n",version);
				}
			}
			else
			{
				ERR("Invalid option: %s\n",left);
				help = true;
				opened = false;
			}

			if(file == NULL && opened)
			{
				ERR("Failed to open:(%s)\n",right);
				help = true;
			}
		}
	}

	//mostra mensagem de ajuda
	if(help)
	{
		ERR("\n\nusage: hidrassembler [OPTIONS...] machine=<machine> messages=<messages>\n");
		ERR("where:\n");
		ERR("machine=<machine>\tloads the specifications for the assembler from the file <machine>\n");
		ERR("messages=<messages>\tloads errors and warnings from <messages>\n");
		ERR("\navailable options:\n");
		ERR("source=<filename>\t source file <filename> will be used instead of stdin\n");
		ERR("output=<filename>\t generated binary will be written to <filename> instead of stdout\n");
		ERR("warnings=<filename>\t generated warnings will be written to <filename> instead of stderr\n");
		ERR("errors=<filename>\t generated errors will be written to <filename> instead of stderr\n");
		ERR("symbols=<filename>\t defined symbols will be written to <filename>\n");
		ERR("version=<number>\t creates a binary file for the <number> version. 3 is for Daedalus. Hidra uses version 0 (default:0)\n");
	}
	else if(machine != NULL && messages != NULL)
	{
		Messenger messenger(messages,warnings,errors);
		Assembler assembler(machine,messenger);

		int size;
		char *codeP = fileRead(source,&size,1);
		codeP[size] = '\0';
		string code(codeP);

		Memory mem = assembler.assembleCode(code);

		if(!assembler.hasErrors())
		{
			if(version == 3)
				assembler.createBinaryV3(output,&mem);
			else if(version == 0)
				assembler.createBinaryV0(output,&mem);
		}

		free(codeP);
	}
	else
	{
		if(machine == NULL)
			ERR("Error: the machine was not specified\n");
		if(messages == NULL)
			ERR("Error: message file was not specified\n");
	}

	if(source != NULL)
		fclose(source);
	if(output != NULL)
		fclose(output);
	if(errors != NULL)
		fclose(errors);
	if(warnings != NULL)
		fclose(warnings);
	if(symbols != NULL)
		fclose(symbols);
	if(machine != NULL)
		fclose(machine);
	if(messages != NULL)
		fclose(messages);

	return 0;

}
