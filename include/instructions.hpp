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

#ifndef INSTRUCTIONS_HPP
#define INSTRUCTIONS_HPP

#include <string>
#include <list>
#include <map>

#include "addressings.hpp"

using namespace std;

typedef struct s_instruction
{
	unsigned int size;
	string mnemonic;
	string operandExpression;
	list<string> addressingNames;
	list<string> regs;
	string binFormat;

}t_instruction;

typedef struct s_operand
{
	string name;
	string addressingCode;	//codigo binario do modo de enderecamento
	string value;
	char type;	//r,l ou n
	bool relative;
}t_operand;

class Instructions
{

	public:

	Instructions();

	/**
	*	carrega as instrucoes que estao definidas na string config
	*/
	void load(string config);

	/**
	*	determina se o dado mnemonico corresponde a uma instrucao ou nao
	*/
	bool isInstruction(string mnemonic);

	/**
	*	gera o codigo binario de uma instrucao usando notacao little-endian
	* retorna o array com esse codigo
	* escreve o numero de bytes em size
	*/
	unsigned char* assemble(string mnemonic, string operands,int *size,Addressings addressings);

	private:

	map<string,list<t_instruction> > insts;	//relaciona o mnemonico com a estrutura
};

/**
* substitui os operandos, escrevendo seu valor binario na string
* a string retornanda contera somente 0s e 1s e sera terminada por um 'b'
* em format:
* r[n] indica o n-esimo registrador. Se n for omitido, segue a ordem em que aparecem
* e[n] indica o n-esimo endereco. Se n for omitido, segue a ordem em que aparecem
* m[n] indica o n-esimo modo de enderecamento. Se n for omitido, segue a ordem em que aparecem
* 1 e 0 indicam os proprios algarismos
* qualquer outro caractere sera ignorado
*/
string replaceOperands(string format,list<t_operand> operands,Registers registers,Labels labels,Addressings addressings);

#endif // INSTRUCTIONS_HPP
