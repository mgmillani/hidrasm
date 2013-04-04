#include <stdio.h>

#include "assembler.hpp"
#include "file.hpp"

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("usage: parse <filename>\n");
		return 1;
	}

	Assembler a;
	int size;

	char *code = fileRead(argv[1],&size,1);
	code[size] = '\0';

	a.assembleCode(string(code));

	return 0;
}
