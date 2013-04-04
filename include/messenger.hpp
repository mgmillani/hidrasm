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

#ifndef MESSENGER_HPP
#define MESSENGER_HPP

#include <string>
#include <list>
#include <map>

#include "instructions.hpp"
#include "addressings.hpp"
#include "registers.hpp"
#include "machine.hpp"
#include "labels.hpp"

using namespace std;

//OBS: A ordem dos elementos eh MUITO importante e deve corresponder a do arquivo que possui as mensagens
typedef enum
{
	mBaseError,
	mBaseWarning,
	mUnknownInstruction,
	mIncorrectOperands,
	mLargeRelativeAddress,
	mUnkownAddressing,
	mRedefinedLabel,
	mUndefinedLabel,
	mOverwritenRegion,
	mUnusedLabel

}e_messages;

typedef struct s_status
{
	unsigned int line;
	unsigned int lastOrgLine;	//a linha do codigo fonte em que ocorreu o ultimo ORG
	unsigned int position;	//a posicao do proximo byte a ser escrito
	unsigned int foundOperands;
	unsigned int expectedOperands;
	unsigned int operandSize;	//numero de bits do operando
	unsigned int firstDefinition; //numero da linha da primeira definicao da label
	int value;	//valor do operando, sem truncar
	string operand;	//o operando junto com seu modo de enderecamento
	string label;	//a ultima label lida (referencia ou definicao)
	string mnemonic;	//mnemonico da ultima instrucao ou diretiva lida
}t_status;

typedef struct s_message
{
	string message;
	bool error;
}t_message;

class Messenger
{

	public:

	/**
	*	inicializa sem nenhuma mensagem
	*/
	Messenger(FILE *warningStream = stderr, FILE *errorStream = stderr);

	/**
	*	inicializa e carrega as mensagens do arquivo
	*/
	Messenger(const char *filename,FILE *warningStream = stderr, FILE *errorStream = stderr);

	~Messenger();

	/**
	*	carrega as mensagens de erro e avisos de um arquivo
	*/
	void load(const char *filename);

	/**
	*	gera a mensagem com o codigo dado, escrevendo-a na stream adequada
	*	usa as informacoes de status para gerar a mensagem
	*/
	void generateMessage(unsigned int code,t_status *status);

	/**
	*	retorna o numero de erros que ocorreram
	*/
	unsigned int numberErrors();

	private:

	/**
	*	atualiza o valor de todas as variaveis utilizadas que estao em this->variables
	*/
	void updateVariables(t_status *status);

	map<unsigned int, t_message> msgs;

	map<string,string> variables;	//associa cada variavel a seu valor

	unsigned int errors;
	unsigned int warnings;

	FILE *errorStream;
	FILE *warningStream;

};

#endif // MESSENGER_HPP
