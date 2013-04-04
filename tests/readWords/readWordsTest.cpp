#include <stdio.h>

#include <string>
#include <list>

#include "stringer.hpp"
#include "defs.hpp"
#include "file.hpp"

using namespace std;

int main(int argc, char *argv[])
{
	if(argc!=2)
	{
		printf("usage: readwords <filename>\n");
		return 1;
	}

	int size;
	char *text = fileRead(argv[1],&size,1);
	text[size] = '\0';
	string phrase(text);
	try
	{
		list<string> words = stringReadWords(phrase,"\"'",'\\','#');
		list<string>::iterator it;

		printf("Words:%lu\n",words.size());
		int i=0;
		for(it=words.begin() ; it!=words.end() ; it++,i++)
		{
			printf("%d:\t%s\n",i,it->c_str());
		}
	}
	catch(e_exception e)
	{
		printf("Open string\n");
	}



	return 0;
}
