

#include <boost/regex.hpp>

#include "stringer.hpp"
#include "expression.hpp"
#include "multiExpression.hpp"

#include "defs.hpp"

#include "debug.hpp"

	/**
	  * todas as variaveis da expressao inicial correspondem a qualquer uma das subexpressoes
	  */
	MultiExpression::MultiExpression(list<Expression> subexpressions,string expression)
	{
		//determina o numero de variaveis da expressao
		unsigned int amount=0;
		bool escape = false;
		unsigned int i;
		for(i=0 ; i<expression.size() ; i++)
		{
			if(escape)
			{
				escape = false;
				continue;
			}
			else if(expression[i]=='\\')
				escape = true;
			else if(isReserved(expression[i]))
				amount++;
		}

		//determina quais sao as variaveis da expressao principal
		this->vars = (char *)malloc(amount);

		//cria a expressao regular
		string regexStr = "[[:blank:]]*";
		escape = false;
		amount = 0;
		for(i=0 ; i<expression.size() ; i++)
		{
			char c= expression[i];
			if(c=='\\')
			{
				escape = true;
				continue;
			}
			else if(stringIn(c,".[{}()\\*+?|^$"))
			{
				regexStr += '\\';
				regexStr += c;
			}
			//as variaveis sao qualquer sequencia de a-zA-Z0-9 ou _
			else if(isReserved(c) && !escape)
			{
				//pode ser qualquer uma das subexpressoes
				regexStr += "(";
				//para cada subexpressao, adiciona a sua expressao regular
				unsigned int max = subexpressions.size();
				unsigned int count = 0;
				list<Expression>::iterator  it;
				for(it=subexpressions.begin() ; count<max-1; it++,count++)
				{
					regexStr += "(?:" + it->regexpression() + ")|";
				}
				regexStr += "(?:" + it->regexpression() + ")";
				regexStr += ")";
				//guarda o tipo da variavel
				this->vars[amount++] = c;
			}
			else
			{
				regexStr += c;
			}
			//qualquer sequencia (0 ou mais) de brancos
			regexStr += "[[:blank:]]*";
			escape = false;
		}

		this->regexStr = regexStr;
		this->regexp = boost::regex(regexStr);
		ERR("MultiExpression regex: %s\n",regexStr.c_str());

	}

	/**
    * se a frase satisfizer a expressao, faz o match entre as variaveis da frase com as da expressao
    * retorna uma lista dos matches feitos
    * se a frase nao satisfizer, throws eUnmatchedExpression
    */
	list<t_match > MultiExpression::findAllSub(string phrase)
	{
		//usa a regex
		boost::match_results<const char*> what;
		boost::regex_match(phrase.c_str(),what,this->regexp);
		if(what[0].matched==0)
			throw(eUnmatchedExpression);

		unsigned int i;
		list<t_match> vars;
		for(i=1 ; i<what.size() ; i++)
		{
			string whatStr = what[i];
			//vars.push_back(pair<string,char>(what[i],this->vars[i-1]));
			ERR("Match %d: %s\n",i,whatStr.c_str());
		}

		return vars;
	}

	string MultiExpression::expression()
	{
		return this->exp;
	}
	string MultiExpression::regexpression()
	{
		return this->regexStr;
	}

