#include <stdio.h>

#include "assembler.hpp"

const char gDelimiter[] = "############\n";
const char gConfFile[] = "config";

void assemblerTest()
{

	Assembler ass(gConfFile);
	ass.print(stdout);
}

int main()
{

	printf(gDelimiter);
	printf("Testing assembler module:\n\n");
	printf(gDelimiter);

	assemblerTest();

	return 0;
}
