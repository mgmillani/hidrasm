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
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <string>

#include "sha1.hpp"

#include "assembler.hpp"
#include "instructions.hpp"
#include "addressings.hpp"
#include "registers.hpp"
#include "machine.hpp"
#include "stringer.hpp"
#include "memory.hpp"

#include "defs.hpp"

using namespace std;

/**
*	cria um montador, especificando quais sao as propriedades da maquina/arquiterua para qual os codigos serao montados
*/
//Assemlber::Assembler(Instructions *inst, Registers *reg, Machine *machine, Adressing *adr);

//dumy
Assembler::Assembler()
{

}

/**
*	le as caracteristicas da arquitetura que estao no arquivo dado
*/
Assembler::Assembler(const char *filename)
{
	FILE *fl = fopen(filename,"rb");
	if(fl == NULL)
	{
		throw eFileNotFound;
	}
	else
	{

		int size;
		fseek(fl,0,SEEK_END);
		size = ftell(fl);
		char *data = (char *)malloc(size+1);
		fseek(fl,0,SEEK_SET);
		fread(data,1,size,fl);
		fclose(fl);
		data[size] = '\0';

		string text (data);

		list<string> lines = stringSplitChar(text,"\n\r");
		list<string>::iterator it;

		e_category category = CAT_NONE;
		for(it=lines.begin() ; it!=lines.end() ; it++)
		{
			string line = *it;
			for(unsigned int i=0 ; i<line.size() ; i++)
			{
				char c = line[i];

				//ignora caracteres em branco
				if(c==' ' || c=='\t')
					continue;

				//comentario
				if(c=='#')
					break;

				//se for um '[', eh a definicao de uma categoria
				if(c=='[')
				{
					//busca o ']'
					i++;
					int start = i;
					while(i<line.size() && line[i]!=']')
						i++;
					if(line[i]!=']')
						throw eInvalidFormat;
					string catName = stringTrim(string(line,start,i-start)," \t");
					if(stringCaselessCompare(catName,"instructions")==0)
						category = CAT_INST;
					else if(stringCaselessCompare(catName,"addressings")==0)
						category = CAT_ADDR;
					else if(stringCaselessCompare(catName,"machine")==0)
						category = CAT_MACH;
					else if(stringCaselessCompare(catName,"registers")==0)
						category = CAT_REGI;
					else
						category = CAT_NONE;
				}
				else if(category == CAT_INST)
				{
					this->inst.load(line);
					break;
				}
				else if(category == CAT_ADDR)
				{
					this->addr.load(line);
					break;
				}
				else if(category == CAT_REGI)
				{
					this->regs.load(line);
					break;
				}
				else if(category == CAT_MACH)
				{
					this->mach.load(line);
					break;
				}
			}
		}

		free(data);

	}
}

/**
*	monta o codigo assembly passado
* retorna a memoria gerada
*/
Memory Assembler::assembleCode(string code)
{
	//quebra as linhas
	list<string> lines = stringSplitChar(code,"\n\r");

	//aloca espaco suficiente para a memoria
	int size = pow(2,this->mach.getPCSize());
	Memory memory(size);

	unsigned int pos = 0;

	list<string>::iterator it;
	unsigned int line=1;
	//monta cada linha
	for(it=lines.begin() ; it!=lines.end() ; it++,line++)
	{
		pos = this->assembleLine(*it,&memory,pos,line);
	}

	//resolve as pendencias
	while(!this->pendecies.empty())
	{
		t_pendency pend = this->pendecies.top();
		this->pendecies.pop();

		//valor da label
		unsigned int value = this->labels.value(pend.label);
		//se for relativo ao PC, calcula a distancia
		if(pend.relative)
		{
			int distance = value-pend.byte;
			this->mach.writeValue(distance,pend.size,&memory,pend.byte);
		}
		else
		{
			this->mach.writeValue(value,pend.size,&memory,pend.byte);
		}
	}

	return memory;
}

/**
*	cria o arquivo binario para a memoria
* o arquivo tera o seguinte formato:
* primeiro byte: versao (0, no caso)
* nome da maquina, terminado por um '\0'
* dump da memoria (size bytes)
* SHA1 do resto do arquivo (20 bytes)
*/
void Assembler::createBinaryV0(string filename,string machineName,Memory *memory)
{
	FILE *fl = fopen(filename.c_str(),"wb");
	//escreve a versão
	char version = 0;
	fwrite(&version,1,1,fl);
	//nome da maquina
	fwrite(machineName.c_str(),1,machineName.size(),fl);
	//termina com '\0'
	char zero = '\0';
	fwrite(&zero,1,1,fl);
	//calcula o SHA1 do arquivo
	//comeca concatenando o arquivo
	unsigned int size;
	unsigned char *memPack = memory->pack(&size);
	char *cat = (char *)malloc(1+machineName.size()+1+size);
	unsigned int pos=0;
	cat[pos++] = 0;
	memcpy(cat+pos,machineName.c_str(),machineName.size());
	pos+=machineName.size();
	cat[pos++]='\0';

	//copia a memoria compactada

	memcpy(cat+pos,memPack,size);
	pos+=size;

	//calcula o SHA1
	SHA1 *shaCalc = new SHA1();
	shaCalc->Input(cat,pos);
	unsigned int *sha = (unsigned int *)malloc(20);	//SHA1 = 160 bits = 20 bytes
	shaCalc->Result(sha);

	//faz um dump da memoria
	fwrite(memPack,1,size,fl);

	//escreve o SHA1
	fwrite(sha,1,20,fl);

	fclose(fl);
	delete shaCalc;
	free(cat);
	free(sha);


}

/**
* faz o parsing de uma linha, escrevendo a label definida em defLabel,
* o mnemonico da linha em mnemonic e os operandos em operands
*/
void Assembler::parseLine(string line,string *defLabel, string *mnemonic, string *operands)
{
	typedef enum {STATE_INI,STATE_FIRST_WORD,STATE_FIRST_END,STATE_LABEL,STATE_INST,STATE_INST_END,STATE_OPERAND} e_states;

	e_states state = STATE_INI;

	unsigned int i;
	unsigned int b;
	bool read = false;	//indica se uma palavra esta sendo lida ou nao
	for(i=0 ; i<line.size() ; i++)
	{
		char c = line[i];
		if(c == ';')
			break;
		switch(state)
		{
			case STATE_INI:
				if(!ISWHITESPACE(c))
				{
					state = STATE_FIRST_WORD;
					b = i;
					read = true;
				}
				break;

			case STATE_FIRST_WORD:
				//a primeira palavra eh uma instrucao ou diretiva
				if(ISWHITESPACE(c))
				{
					state = STATE_FIRST_END;
					*mnemonic = line.substr(b,i-b);
					read = false;
				}
				//eh uma label
				else if(c==':')
				{
					state = STATE_LABEL;
					*defLabel = line.substr(b,i-b);
					read = false;
				}
				break;
			//le os espacos em branco ate encontrar outro caractere
			case STATE_FIRST_END:
				if(!ISWHITESPACE(c))
				{
					state = STATE_OPERAND;
					b = i;
					read = true;
				}
				break;
			//a definicao de uma label foi lida
			//a proxima palavra eh um instrucao ou diretiva
			//le os espacos em branco
			case STATE_LABEL:
				if(!ISWHITESPACE(c))
				{
					state = STATE_INST;
					b = i;
					read = true;
				}
				break;
			//inicio de uma instrucao ou diretiva
			case STATE_INST:
				if(ISWHITESPACE(c))
				{
					state = STATE_INST_END;
					*mnemonic = line.substr(b,i-b);
					read = false;
				}
				break;
			//le os espacos em branco apos uma instrucao
			case STATE_INST_END:
				//a proxima palavra eh um operando
				if(!ISWHITESPACE(c))
				{
					state = STATE_OPERAND;
					b = i;
					read = true;
				}
				break;
			case STATE_OPERAND:

				break;
		}
	}

	//se uma palavra ainda deve ser lida
	if(read)
	{
		if(*mnemonic == "")
			*mnemonic = line.substr(b,i-b);
		else
			*operands= line.substr(b,i-b);
	}
}

/**
*	monta uma linha, escrevendo seu codigo binario a partir de memory[byte]
* line eh a linha a ser montada
* se houver alguma label que ainda nao foi definida, reserva espaco e adiciona a pendencia na pilha
* se for encontrada a definicao de uma label, acrescenta-a as Labels conhecidas
* retorna a posicao da memoria em que a proxima linha deve comecar
*/
unsigned int Assembler::assembleLine(string line, Memory *memory,unsigned int byte,unsigned int lineNumber)
{

	t_status status;
	status.line = lineNumber;
	status.pos = byte;
	string defLabel;
	string mnemonic;
	string operands;
	this->parseLine(line,&defLabel,&mnemonic,&operands);
	status.label = defLabel;
	status.mnemonic = mnemonic;
	status.operand = operands;
	//define a label
	if(defLabel!="")
	{
		try
			this->labels.define(defLabel);
		catch(e_exception e)
		{
			if(e == eRedefinedLabel)
			{
				status.firstDefinition = this->labels.line(defLabel);
				this->messenger.generateMessage(mRedefinedLabel,&status);
			}
		}
	}
	//se for uma instrucao, monta-a
	if(this->inst.isInstruction(mnemonic))
	{
		this->inst.assemble(mnemonic,operands);
	}
	//executa a diretiva
	else
		try
			byte = this->directives.execute(mnemonic,operands,memory,byte);
		catch(e_exception e)
		{
			switch(e)
			{
				case eUnkownMnemonic:
					this->messenger.generateMessage(mUnknownInstruction,&status);
					break;
				case eIncorrectOperands:
					this->messenger.generateMessage(mIncorrectOperands,&status);
					break;
			}
		}

	printf("Line:%s\n",line.c_str());
	printf("Defined Label: %s\n",defLabel.c_str());
	printf("Instruction: %s\n",mnemonic.c_str());
	printf("Operands: %s\n",operands.c_str());
	printf("\n\n*********\n");
	getchar();

	return byte;
}







