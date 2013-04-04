#include <stdio.h>

#include <string>

#include "stringer.hpp"

using namespace std;

int main(int argc, char*argv[])
{
	if(argc != 3)
	{
		printf("usage: <str a> <str b>\n");
		return 1;
	}

	string a (argv[1]);
	string b (argv[2]);

	printf("%s - %s = %d\n",a.c_str(),b.c_str(),stringCaselessCompare(a,b));

	return 0;
}
