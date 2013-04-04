#include <stdio.h>

#include <string>

#include "numbers.hpp"

using namespace std;

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("usage: readnum <number>\n");
		return -1;
	}

	string n (argv[1]);

	Number num;
	int value = num.toInt(n);
	int size;
	unsigned char *bytes = num.toByteArray(n,&size);

	printf("%d\n",value);

	int i;
	for(i=0 ; i<size ; i++)
	{
		printf("%x ",(unsigned int)bytes[i]);
	}
	printf("\n");

	return 0;

}
