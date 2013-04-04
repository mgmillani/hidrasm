#include <stdio.h>

#include "messenger.hpp"

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("usage: messenger <filename>\n");
		return 1;
	}

	Messenger msg (argv[1]);

	t_status status;

	status.position = 10;
	status.lastOrgLine = 5;
	status.value = 200;
	status.expectedOperands = 1;
	status.foundOperands = 2;
	status.operand = "ASD,i";
	status.label = "ASD";
	status.mnemonic = "ADD";
	status.line = 25;
	status.operandSize = 8;

	int i;
	for(i=2 ; i<=8 ; i++)
		msg.generateMessage(i,&status);

	return 0;
}
