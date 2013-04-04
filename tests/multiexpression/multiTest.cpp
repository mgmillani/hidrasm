#include <stdio.h>

#include <string>

#include "expression.hpp"
#include "multiExpression.hpp"
#include "defs.hpp"
#include "file.hpp"

using namespace std;

int main(int argc, char *argv[])
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
	for(lines=fl.begin() ; lines!=fl.end() ; lines++)
	{
		exp.push_back(Expression(*lines));
	}

	list<t_match > vars;
	list<Expression>::iterator it;

	string multiex(argv[2]);

	//cria a multiexpressao
	MultiExpression mex(exp,multiex);

	string phrase(argv[3]);

	//verifica se a frase passada bate com alguma expressao

	try
	{
		vars = mex.findAllSub(phrase);
		list<t_match >::iterator v;
		printf("Matched: %s\n",mex.expression().c_str());
		/*for(v=vars.begin() ; v!=vars.end() ; v++)
		{
			printf("%s:%c\n",v->first.c_str(),v->second);
		}*/
	}
	catch(e_exception e)
	{
		printf("Does not match %s\n",mex.expression().c_str());
	}


	return 0;

}
