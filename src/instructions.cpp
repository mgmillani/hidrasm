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

#include "instructions.hpp"
#include "addressings.hpp"
#include "registers.hpp"
#include "labels.hpp"
#include "numbers.hpp"
#include "stringer.hpp"
#include "defs.hpp"

using namespace std;

Instructions::Instructions()
{

}

/**
*	carrega as instrucoes que estao definidas na string config
*/
void Instructions::load(string config)
{
	list<string> words = stringReadWords(config,"'\"",'\\','#');

	list<string>::iterator it=words.begin();

	t_instruction inst;
	Number n;
	inst.size = n.toInt(*(it++));

	if(it==words.end()) throw (eInvalidFormat);
	inst.mnemonic = *(it++);

	if(it==words.end()) throw (eInvalidFormat);
	inst.operandExpression = *(it++);

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
	if(this->insts.find(mnemonic)!=this->insts.end())
		return true;
	else
		return false;
}

/**
*	gera o codigo binario de uma instrucao usando notacao little-endian
* retorna o array com esse codigo
* escreve o numero de bytes em size
*/
unsigned char* assemble(string mnemonic, string operands,int *size,Addressings addressings,Labels labels, Registers registers)
{

	//busca todas as instrucoes com o dado mnemonico
	map<string,t_instruction>::iterator it = this->insts.find(mnemonic);
	//se nao encontrou
	if(it==this->insts.end())
		throw (eUnknownInstruction);

	list<t_instruction> matches = it->second;
	list<t_instruction>::iterator jt;
	bool inOk = false;	//se foi encontrada a instrucao certa
	t_instruction i;

	list<t_operand> operands;

	for(jt=matches.begin() ; jt!=matches.end() && !inOk; jt++)
	{
		i = *jt;
		Expression e(i.operandExpression);

		list<string>::iterator addr;
		//cria uma lista com todas as expressoes dos modos de enderecamento
		list<string> expr;
		for(addr=i.addressingNames.begin() ; addr!=i.addressingNames.end() ; addr++)
		{
			t_addressing a = addressings.getAddressing(*addr);
			expr.push_back(a.exp);
		}

		list<t_match > matches = e.findAllSub(operands,expr);

		//verifica se as variaveis sao do tipo adequado
		list<t_match>::iterator imatch;

		//limpa as listas de operandos
		operands = list<t_operand>();

		Number number;
		bool opOk = true;
		for(imatch=matches.begin() ; imatch!=matches.end() && opOk ; imatch++)
		{
			opOk = false;
			t_match m = *imatch;
			//determina o tipo do operando
			if(m.subtype[VAR_REGISTER] || m.subtype[VAR_ANYTHING])
			{
				if(registers.exists(m.element))
				{
					t_operand op;
					op.name = m.element;
					op.type = 'r';
					op.relative = m.relative[VAR_REGISTER];
					op.addressingCode = m.subCode[VAR_REGISTER];
					operands.push_back(op);
					opOk=true;
					continue;
				}
			}
			if(m.subtype[VAR_NUMBER] || m.subtype[VAR_ANYTHING] || m.subtype[VAR_ADDRESS])
			{
				if(number.exists(m.element))
				{
					t_operand op;
					op.name = m.element;
					op.type = 'n';
					op.relative = m.relative[VAR_NUMBER];
					op.addressingCode = m.subCode[VAR_NUMBER];
					operands.push_back(op);
					opOk=true;
					continue;
				}
			}
			if(m.subtype[VAR_LABEL] || m.subtype[VAR_ANYTHING] || m.subtype[VAR_ADDRESS])
			{
				if(labels.exists(m.element))
				{
					t_operand op;
					op.name = m.element;
					op.type = 'l';
					op.relative = m.relative[VAR_LABEL];
					op.addressingCode = m.subCode[VAR_LABEL];
					operands.push_back(op);
					opOk=true;
					continue;
				}
			}
		}//end for match
		//se todos os operandos estao corretos, essa eh a instrucao certa
		if(opOk)
			inOk = true;
	}//end for instruction

	//se nenhuma instrucao satisfez, a linha esta incorreta
	if(!inOk)
		throw(eInvalidFormat);
	else
	{
		//i contem a ultima instrucao avaliada
		string code = replaceOperands(i.binFormat,operands,registers,labels,addressings);
		Number n;
		unsigned char *code = n.toByteArray(code,size);
		return code;
	}
}

/**
* substitui os operandos, escrevendo seu valor binario na string
* em format:
* r[n] indica o n-esimo registrador. Se n for omitido, segue a ordem em que aparecem
* e[n](m) indica o n-esimo endereco. Se n for omitido, segue a ordem em que aparecem. m indica o tamanho, em bits
* m[n] indica o n-esimo modo de enderecamento. Se n for omitido, segue a ordem em que aparecem
* 1 e 0 indicam os proprios algarismos
* qualquer outro caractere sera ignorado
* size indica quantos bits o resultado deve ter
* a string retornanda contera somente 0s e 1s e sera terminada por um 'b'
*/
string replaceOperands(string format,list<t_operand> operands,Registers registers,Labels labels,Addressings addressings,unsigned int size)
{

	typedef enum {STATE_COPY,STATE_NUM,STATE_REGISTER,STATE_MODE,STATE_ADDRESS,STATE_I_OPERAND,STATE_W_OPERAND} e_state;

	Number n;
	e_state state = STATE_COPY;
	e_state nextState = STATE_COPY;
	unsigned int r,w;
	unsigned int b;
	string number;
	int value;

	string result(size+1,'0');
	list<string> addresses;
	result[size] = 'b';

	t_operand op;
	unsigned char type;

	unsigned int defSize = 0;	//tamanho padrao dos enderecos

	for(r=w=0 ; r<format.size() && w<size ; r++)
	{
		unsigned char c = string[r];
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
					b = i;
					state = STATE_NUM;
					nextState = STATE_I_OPERAND;
					type = REGISTER;
				}
				else
				{
					op = getNextOperand(operands,REGISTER);
					number = op.value;
					//copia o valor
					unsigned int i;
					for(i=0 ; i<number.size() -1 ; i++)
						result[w++] = number[i];

					switch(tolower(c))
					{
						case REGISTER: state = STATE_REGISTER; break;
						case ADDRESSING: state = STATE_MODE; break;
						case ADDRESS: state = STATE_ADDRESS; break;
						default:
							i--;
							state=STATE_COPY;
					}
				}

				break;
			case STATE_MODE:
				//se for um [, o indice esta sendo informado
				if(c=='[')
				{
					b = i;
					state = STATE_NUM;
					nextState = STATE_I_OPERAND;
					type = ADDRESSING;
				}
				else
				{
					op = getNextOperand(operands,ADDRESSING);
					number = op.value;
					//copia o valor
					unsigned int i;
					for(i=0 ; i<number.size() -1 ; i++)
						result[w++] = number[i];

					switch(tolower(c))
					{
						case REGISTER: state = STATE_REGISTER; break;
						case ADDRESSING: state = STATE_MODE; break;
						case ADDRESS: state = STATE_ADDRESS; break;
						default:
							i--;
							state=STATE_COPY;
					}
				}

				break;
			//escreve o codigo do endereco
			case STATE_ADDRESS:
				//se for um [, o indice esta sendo informado
				if(c=='[')
				{
					b = i;
					state = STATE_NUM;
					nextState = STATE_I_OPERAND;
					type = ADDRESS;
				}
				else
				{
					op = getNextOperand(operands,ADDRESS);
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
							i--;
							state=STATE_COPY;
					}
				}

				break;

			//foi informado o indice do operando
			case STATE_I_OPERAND:
				op = getOperand(operands,type,value);
				number = op.value;
				//foi informado o tamanho do operando
				if(c=='(')
				{
					state = STATE_NUM;
					nextState = STATE_S_OPERAND;
					b = i;
				}
				else
				{
					//se for um endereco, apenas escreve um 'a'
					//e adiciona o endereco a lista de enderecos
					if(type==ADDRESS)
					{
						result[w++] = ADDRESS;
						addresses.push_back(op.value);
					}
					//caso contrario, escreve o valor
					else
						for(int k=0 ; k<op.value.size()-1 ; k++)
							result[w++] = value[k];
					//determina o proximo estado
					switch(tolower(c))
					{
						case REGISTER: state = STATE_REGISTER; break;
						case ADDRESSING: state = STATE_MODE; break;
						case ADDRESS: state = STATE_ADDRESS; break;
						default:
							i--;
							state=STATE_COPY;
					}
				}

				break;
			//escreve o operando (esta em number), usando exatamente value bits
			case STATE_S_OPERAND:
				{
					//propaga o sinal, se necessario
					int k;
					for(k=0 ; k<(value-(number.size()-1)) ; k++)
						result[w++] = op.value[0];
					//trunca o numero, escrevendo somente os bits menos significativos
					for(k = value-k ; k<(number.size()-1)) ; k++)
						result[w++] = op.value[k];
				}
				//determina o proximo estado
				switch(tolower(c))
				{
					case REGISTER: state = STATE_REGISTER; break;
					case ADDRESSING: state = STATE_MODE; break;
					case ADDRESS: state = STATE_ADDRESS; break;
					default:
						i--;
						state=STATE_COPY;
				}

				break;
			//le um numero decimal, converte-o para inteiro e depois vai para nextState
			case STATE_NUM:

				//o numero terminou
				if(c==']' || c==')')
				{
					value = n.toInt(format.substr(b,i-b));
					state = nextState;
				}

				break;
		}//end switch
	}
}
