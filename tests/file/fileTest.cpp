#include <stdio.h>

#include <list>
#include <string>

#include "file.hpp"

using namespace std;

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("usage: readfile <filename>\n");
		return 1;
	}

	list<string> *lines = fileReadLines(argv[1]);
	list<string>::iterator it;

	int i =0;
	for(it=lines->begin() ; it!=lines->end() ; it++,i++)
	{
		printf("%d:\t%s\n",i,(*it).c_str());
	}

	delete lines;

	return 0;

}
