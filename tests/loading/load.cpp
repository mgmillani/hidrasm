#include <stdio.h>

#include <string>

#include "assembler.hpp"

#include "defs.hpp"
using namespace std;

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("usage: load <config file>\n");
		return 1;
	}

	try
	{
		Assembler ass (argv[1]);
	}
	catch (e_exception e)
	{
		printf("EX:%d\n",e);
	}

	return 0;

}
