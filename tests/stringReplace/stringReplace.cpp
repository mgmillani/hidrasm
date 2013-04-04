#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <list>

#include "stringer.hpp"
#include "file.hpp"

using namespace std;

/**
*	cada linha corresponde a uma substituicao
*/
list<pair<string,string> > readIn(const char *fname)
{
	list<string> *lines = fileReadLines(fname);

	list<string>::iterator it;

	list<pair<string,string> > replace;

	for(it=lines->begin() ; it!=lines->end() ; it++)
	{
		list<string> *el = stringSplitChar(*it," \t");
		replace.push_back(pair<string,string>(el->front(),el->back()));
		delete el;
	}

	delete lines;

	return replace;
}

int main(int argc, char *argv[])
{
	if(argc!=3)
	{
		printf("usage: replace <source file> <replacements file>\n");
		return 1;
	}

	int size;
	char *data = fileRead(argv[1],&size,1);
	data[size] = '\0';
	string text(data);

	list<pair<string,string> > replace = readIn(argv[2]);

	string replaced = stringReplaceAll(text,replace);

	printf("%s\n",replaced.c_str());

	free(data);

	return 0;
}
