#include <stdio.h>

#include <string>

#include "expression.hpp"
#include "defs.hpp"
#include "file.hpp"

using namespace std;

int main(int argc, char*argv[])
{

	if(argc!=4)
	{
		printf("usage: mex <expression definition file> <multi expression> <phrase>\n");
		return 1;
	}

	list<string> fl = fileReadLines(argv[1]);

	list<Expression> exp;

	//cria as expressoes
	list<string>::iterator lines;
	for(lines=fl.begin() ; lines!=fl.end() ; lines++)=
	{
		exp.push_back(Expression(*lines));
	}

	list<pair<string,char> > vars;
	list<Expression>::iterator it;

	string multiex(argv[2]);


	string phrase(argv[3]);
/*
	//verifica se a frase passda bate com alguma expressao
	for(it=exp.begin() ; it!=exp.end() ; it++)
	{
		try
		{
			vars = it->findAll(phrase);
			list<pair<string,char> >::iterator v;
			printf("Matched: %s\n",it->expression().c_str());
			for(v=vars.begin() ; v!=vars.end() ; v++)
			{
				printf("%s:%c\n",v->first.c_str(),v->second);
			}
		}
		catch(e_exception e)
		{
			printf("Does not match %s\n",it->expression().c_str());
		}
	}
*/
	return 0;

}
