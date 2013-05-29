#include <stdio.h>

#include "assembler.hpp"
#include "file.hpp"

const char gDelimiter[] = "############\n";
const char gConfFile[] = "ramses";
const char gCodeFile[] = "code.asm";
const char gMsgFile[] = "messages";

void assemblerTest()
{

	Messenger messenger(gMsgFile,stdout,stdout);

	printf("%sLoading configuration file:%s",gDelimiter,gDelimiter);
	Assembler ass(gConfFile,messenger);
	ass.print(stdout);

	printf("%sAssembling code\n%s",gDelimiter,gDelimiter);

	int size=0;
	char *codeP = fileRead(gCodeFile,&size,1);
	if(codeP == NULL)
	{
		printf("%s%sError reading: %s\n%s%s",gDelimiter,gDelimiter,gCodeFile,gDelimiter,gDelimiter);
		return;
	}
	codeP[size] = '\0';
	string code(codeP);
	Memory mem = ass.assembleCode(code);

	//printf("%sGenerated memory\n%s",gDelimiter,gDelimiter);
	//mem.print(stdout);

	free(codeP);
}

int main()
{

	printf(gDelimiter);
	printf("Testing assembler module:\n\n");
	printf(gDelimiter);

	assemblerTest();

	return 0;
}
