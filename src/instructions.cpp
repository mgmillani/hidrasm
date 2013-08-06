/**
* Copyright 2013 Marcelo Millani
*	This file is part of hidrasm.
*
* hidrasm is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* hidrasm is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with hidrasm.  If not, see <http://www.gnu.org/licenses/>
*/

#include <stdio.h>

#include <string>

#include <boost/algorithm/string.hpp>

#include "instructions.hpp"
#include "addressings.hpp"
#include "registers.hpp"
#include "expression.hpp"
#include "operands.hpp"
#include "multiExpression.hpp"
#include "memory.hpp"
#include "labels.hpp"
#include "numbers.hpp"
#include "stringer.hpp"
#include "types.hpp"

#include "defs.hpp"
#include "debug.hpp"

using namespace std;

Instructions::Instructions()
{

}

/**
*	carrega as instrucoes que estao definidas na string config
*/
void Instructions::load(string config)
{
	list<string> words = stringReadWords(config,"'\"",'\\',SYMB_COMMENT);

	list<string>::iterator it=words.begin();

	t_instruction inst;
	Number n;
	inst.size = n.toInt(*(it++));

	if(it==words.end()) throw (eInvalidFormat);
	inst.mnemonic = boost::to_upper_copy(*(it++));

	if(it==words.end()) throw (eInvalidFormat);
	if(Expression::getExpressionType(*(it)) != TYPE_NONE)
		inst.operandExpression = *(it++);
	else
		inst.operandExpression = "";

	if(it==words.end()) throw (eInvalidFormat);
	inst.addressingNames = stringSplitChar(*(it++),",");

	if(it==words.end()) throw (eInvalidFormat);
	inst.regs = stringSplitChar(*(it++),",");

	if(it==words.end()) throw (eInvalidFormat);
	inst.binFormat = *it;

	this->insts[inst.mnemonic].push_back(inst);
}

/**
*	determina se o dado mnemonico corresponde a uma instrucao ou nao
*/
bool Instructions::isInstruction(string mnemonic)
{
	//boost::to_upper(mnemonic);
	if(this->insts.find(mnemonic)!=this->insts.end())
		return true;
	else
		return false;
}

/**
  *	gera o codigo binario de uma instrucao, escrevendo-o na memoria
  * retorna o numero de bytes escritos na memoria
  */
unsigned int Instructions::assemble(string mnemonic, string operandsStr,Memory *mem,unsigned int pos,stack<t_pendency> *pendencies,Addressings addressings,Labels labels, Registers registers)
{

	//busca todas as instrucoes com o dado mnemonico
	boost::to_upper(mnemonic);
	map<string,list<t_instruction> >::iterator it = this->insts.find(mnemonic);

	//se nao encontrou
	if(it==this->insts.end())
	{
		throw (eUnknownMnemonic);
	}

	list<t_instruction> matches = it->second;
	list<t_instruction>::iterator jt;
	bool inOk = false;	//se foi encontrada a instrucao certa
	t_instruction i;

	list<t_operand> operands;
	bool hasPendency = false;
	bool wrongOperands = false;

	for(jt=matches.begin() ; jt!=matches.end() && !inOk; jt++)
	{
		i = *jt;
		bool potentialLabel = false;

		//se nao houver operando e a instrucao nao requer operandos
		if(operandsStr.size() == 0 && i.addressingNames.front()=="-")
		{
			inOk = true;
			break;
		}

		//cria uma lista com todas as expressoes dos modos de enderecamento
		list<Expression> expr;
		list<t_addressing> addrs;
		//se o modo de enderecamento for *, pega todos os modos de enderecamento existentes
		if(i.addressingNames.front()=="*")
		{
			addrs = addressings.getAllAddressings();
			list<t_addressing>::iterator at;
			for(at=addrs.begin() ; at!=addrs.end() ; at++)
				expr.push_back(at->expression);
		}
		else if(i.addressingNames.front() != "-")
		{
			list<string>::iterator addr;
			for(addr=i.addressingNames.begin() ; addr!=i.addressingNames.end() ; addr++)
			{
				t_addressing a = addressings.getAddressing(*addr);
				addrs.push_back(a);
				expr.push_back(a.expression);
			}
		}

		MultiExpression mulEx(expr,i.operandExpression);
		list<t_match > matches;

		try
		{
			matches = mulEx.findAllSub(operandsStr);
		}
		catch (e_exception e)
		{
			wrongOperands = true;
			continue;
		}

		//verifica se as variaveis sao do tipo adequado
		list<t_match>::iterator imatch;

		//limpa as listas de operandos
		operands = list<t_operand>();

		Number number;
		bool opOk = true;

		for(imatch=matches.begin() ; imatch!=matches.end() && opOk ; imatch++)
		{

			opOk = false;
			potentialLabel = false;
			t_match m = *imatch;
			t_operand op;
			op.name = m.element;

			//determina o tipo do operando
			if(m.subtype[TYPE_REGISTER] || m.subtype[TYPE_ANYTHING])
			{
				if(registers.exists(m.element))
				{
					op.type = TYPE_REGISTER;
					op.value = Number::toBin(registers.number(m.element));
					opOk=true;
				}
			}
			if(m.subtype[TYPE_NUMBER] || m.subtype[TYPE_ANYTHING] || m.subtype[TYPE_ADDRESS])
			{
				if(number.exists(m.element))
				{
					op.type = TYPE_NUMBER;
					op.value = Number::toBin(m.element);
					opOk=true;
				}
				else if(Number::isString(m.element))
				{

					op.type = TYPE_NUMBER;
					op.value = Number::stringToBin(m.element);
					opOk=true;
				}
			}
			if(m.subtype[TYPE_LABEL] || m.subtype[TYPE_ANYTHING] || m.subtype[TYPE_ADDRESS])
			{
				if(labels.exists(m.element))
				{
					op.type = TYPE_LABEL;
					op.value = Number::toBin(labels.value(op.name));
					opOk=true;
				}
				else
					potentialLabel = true;
			}
			if(opOk || potentialLabel)
			{
				//determina o codigo do modo de enderecamento
				//filtra os modos de enderecamento nos quais nao houve match
				list<t_addressing> matchedAddrs;
				list<t_addressing>::iterator at;
				at = addrs.begin();
				unsigned int atpos = 0;
				list<unsigned int>::iterator it;
				for(it=m.indexes.begin() ; it!=m.indexes.end() ; it++)
				{
					unsigned int pos = *it;
					if(pos < atpos)
					{
						atpos = 0;
						at = addrs.begin();
					}
					while(atpos<pos)
					{
						atpos++;
						at++;
					}
					//adiciona o modo de enderecamento com indice encontrado
					matchedAddrs.push_back(*at);
				}

				//se for uma label ainda nao definida
				if(!opOk)
				{
					op.type = TYPE_LABEL;
					hasPendency = true;
				}

				t_addressing a = findBestAddressing(matchedAddrs,op.type);
				op.addressingCode = a.code;
				op.relative = a.relative;
				operands.push_back(op);
			}
		}//end for match
		//se todos os operandos estao corretos, essa eh a instrucao certa
		if(opOk)
			inOk = true;
	}//end for instruction

	if(!inOk && wrongOperands && !hasPendency)
	{
		throw(eIncorrectOperands);
	}

	//se nenhuma instrucao satisfez, a linha esta incorreta
	if(!inOk && !hasPendency)
		throw(eInvalidFormat);
	//se houver alguma pendencia, monta a linha depois
	else if(hasPendency)
	{
		t_pendency p;
		p.byte = pos;
		p.operands = operands;
		p.binFormat = i.binFormat;
		p.size = i.size;

		pendencies->push(p);
		return i.size/8;
	}
	else
	{
		//i contem a ultima instrucao avaliada
		string code = replaceOperands(i.binFormat,operands,i.size);

		return mem->writeNumber(code,pos,-1);
	}
	// esse ponto nunca deve ser executado
	return 0;
}

/**
  * substitui os operandos, escrevendo seu valor binario na string
  * em format:
  * r[n] indica o n-esimo registrador. Se n for omitido, segue a ordem em que aparecem
  * a[n](m) indica o n-esimo endereco. Se n for omitido, segue a ordem em que aparecem. m indica o tamanho, em bits
  * m[n] indica o n-esimo modo de enderecamento. Se n for omitido, segue a ordem em que aparecem
  * 1 e 0 indicam os proprios algarismos
  * qualquer outro caractere sera ignorado
  * size indica quantos bits o resultado deve ter
  * a string retornanda contera somente 0s e 1s e sera terminada por um 'b'
  */
string replaceOperands(string format,list<t_operand> operands,unsigned int size)
{

	typedef enum {STATE_COPY,STATE_NUM,STATE_REGISTER,STATE_MODE,STATE_ADDRESS,STATE_I_OPERAND,STATE_W_OPERAND} e_state;

	list<t_operand>::iterator ops;

	Operands operand(operands);
	Number n;
	e_state state = STATE_COPY;
	e_state nextState = STATE_COPY;
	unsigned int r,w;
	unsigned int b=0;
	string number;
	int value = 0;

	string result(size+1,'0');
	list<string> addresses;
	result[size] = 'b';

	t_operand op;
	e_type type = TYPE_NONE;

	unsigned int defSize = 0;	//tamanho padrao dos enderecos
	const char *formatStr = format.c_str();
	for(r=w=0 ; r<=format.size() && w<size ; r++)
	{
		unsigned char c = formatStr[r];

		switch (state)
		{
			case STATE_COPY:
				if(c=='1' || c=='0')
					result[w++] = c;
				else if(c==REGISTER)
					state = STATE_REGISTER;
				else if(c==ADDRESSING)
					state = STATE_MODE;
				else if(c==ADDRESS)
					state = STATE_ADDRESS;
				break;
			//escreve o codigo do registrador
			case STATE_REGISTER:
				//se for um [, o indice esta sendo informado
				if(c=='[')
				{
					b = r+1;
					state = STATE_NUM;
					nextState = STATE_I_OPERAND;
					type = TYPE_REGISTER;
				}
				else
				{
					op = operand.getNextOperand(TYPE_REGISTER);
					number = op.value;
					//copia o valor
					unsigned int i;
					for(i=0 ; (i+1)<number.size() ; i++)
					{
						result[w++] = number[i];
					}

					switch(tolower(c))
					{
						case REGISTER: state = STATE_REGISTER; break;
						case ADDRESSING: state = STATE_MODE; break;
						case ADDRESS: state = STATE_ADDRESS; break;
						default:
							r--;
							state=STATE_COPY;
					}
				}

				break;
			case STATE_MODE:
				//se for um [, o indice esta sendo informado
				if(c=='[')
				{
					b = r+1;
					state = STATE_NUM;
					nextState = STATE_I_OPERAND;
					type = TYPE_ADDRESSING;
				}
				else
				{
					op = operand.getNextOperand(TYPE_ADDRESSING);
					number = op.addressingCode;
					//copia o valor
					unsigned int i;
					for(i=0 ; (i+1)<number.size() ; i++)
						result[w++] = number[i];

					switch(tolower(c))
					{
						case REGISTER: state = STATE_REGISTER; break;
						case ADDRESSING: state = STATE_MODE; break;
						case ADDRESS: state = STATE_ADDRESS; break;
						default:
							r--;
							state=STATE_COPY;
					}
				}

				break;
			//escreve o codigo do endereco
			case STATE_ADDRESS:
				//se for um [, o indice esta sendo informado
				if(c=='[')
				{
					b = r+1;
					state = STATE_NUM;
					nextState = STATE_I_OPERAND;
					type = TYPE_ADDRESS;
				}
				else
				{
					op = operand.getNextOperand(TYPE_ADDRESS);
					number = op.value;
					//adiciona a lista de enderecos
					addresses.push_back(number);
					//escreve o simbolo do endereco
					result[w++] = ADDRESS;

					switch(tolower(c))
					{
						case REGISTER: state = STATE_REGISTER; break;
						case ADDRESSING: state = STATE_MODE; break;
						case ADDRESS: state = STATE_ADDRESS; break;
						default:
							r--;
							state=STATE_COPY;
					}
				}

				break;

			//foi informado o indice do operando
			case STATE_I_OPERAND:
				op = operand.getOperandIndex(type,value);
				if(type == TYPE_ADDRESS || type == TYPE_REGISTER)
					number = op.value;
				else //if(type == TYPE_ADDRESSING)
					number = op.addressingCode;

				//foi informado o tamanho do operando
				if(c=='(')
				{
					state = STATE_NUM;
					nextState = STATE_W_OPERAND;
					b = r+1;
				}
				else
				{
					//se for um endereco, apenas escreve um 'a'
					//e adiciona o endereco a lista de enderecos
					if(type==TYPE_ADDRESS)
					{
						result[w++] = ADDRESS;
						addresses.push_back(op.value);
					}
					//caso contrario, escreve o valor
					else
						for(unsigned int k=0 ; k<number.size()-1 ; k++)
							result[w++] = number[k];
					//determina o proximo estado
					switch(tolower(c))
					{
						case REGISTER: state = STATE_REGISTER; break;
						case ADDRESSING: state = STATE_MODE; break;
						case ADDRESS: state = STATE_ADDRESS; break;
						default:
							r--;
							state=STATE_COPY;
					}
				}

				break;
			//escreve o operando (esta em number), usando exatamente value bits
			case STATE_W_OPERAND:
				{
					//propaga o sinal, se necessario
					int k;
					for(k=0 ; k<(int)(value-number.size()+1) ; k++)
						result[w++] = op.value[0];
					//trunca o numero, escrevendo somente os bits menos significativos
					for(k = value-k ; k<(int)(number.size()-1) ; k++)
					{
						result[w++] = op.value[k];
					}
				}
				//determina o proximo estado
				switch(tolower(c))
				{
					case REGISTER: state = STATE_REGISTER; break;
					case ADDRESSING: state = STATE_MODE; break;
					case ADDRESS: state = STATE_ADDRESS; break;
					default:
						r--;
						state=STATE_COPY;
				}

				break;
			//le um numero decimal, converte-o para inteiro e depois vai para nextState
			case STATE_NUM:

				//o numero terminou
				if(c==']' || c==')')
				{
					value = n.toInt(format.substr(b,r-b));
					state = nextState;
				}

				break;
		}//end switch
	}

	//substitui todas as ocorrencias de um ADDRESS pelo respectivo valor
	if(addresses.size()>0)
	{

		defSize = 1+ (size-w)/addresses.size();
		r=w=0;
		list<string>::iterator ad;
		for(ad=addresses.begin() ; ad!=addresses.end() ; ad++)
		{
			//busca o proximo ADDRESS
			while(result[r]!=ADDRESS)
			{
				r++;
			}
			number = Number::toBin(*ad);
			//escreve o valor binario, ajustando para o tamanho adequado
			w = r;
			unsigned int i;
			for(i=0 ; i < defSize - (number.size()-1) ; i++)
				result[w++] = '0';
			unsigned int k=0;
			while(i<defSize)
			{
				result[w++] = number[k++];
				i++;
			}
		}
	}
	else
		defSize = 0;

	return result;
}

list<t_instruction> Instructions::getInstructions(string mnemonic)
{
	map<string, list<t_instruction> >::iterator it = this->insts.find(mnemonic);
	if(it != this->insts.end())
		return it->second;
	else
		return list<t_instruction>();
}

/**
	* escreve as caracteristicas do conjunto de instrucoes em stream
	*/
void Instructions::print(FILE *stream)
{

	map<string, list<t_instruction> >::iterator it;
	for(it=this->insts.begin() ; it!=this->insts.end() ; it++)
	{
		list<t_instruction>::iterator jt;
		for(jt=it->second.begin() ; jt!=it->second.end() ; jt++)
		{
			t_instruction i = *jt;
			this->printInstruction(&i,stream);
		}
	}

}

/**
	* escreve os atributos da estrutura em stream
	*/
void Instructions::printInstruction(t_instruction *inst,FILE *stream)
{

	//mnemonico
	fprintf(stream,"\n# %s #\n",inst->mnemonic.c_str());
	//tamanho
	fprintf(stream,"Size = %u\n",inst->size);
	//expressao do operando
	fprintf(stream,"Operand Expression: %s\n",inst->operandExpression.c_str());
	//modos de enderecamento que podem ser usados
	fprintf(stream,"Available addressing modes: \n");
	list<string>::iterator it;
	for(it=inst->addressingNames.begin() ; it!=inst->addressingNames.end() ; it++)
		fprintf(stream," %s",it->c_str());
	fprintf(stream,"\n");

	//registradores que podem ser usados
	fprintf(stream,"Available registers: \n");
	for(it=inst->regs.begin() ; it!=inst->regs.end() ; it++)
		fprintf(stream," %s",it->c_str());
	fprintf(stream,"\n");

	//formato binario da instrucao
	fprintf(stream,"Format: %s\n",inst->binFormat.c_str());

}
