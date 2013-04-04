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

#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

#include <string>
#include <stack>
#include <list>

#include "instructions.hpp"
#include "addressings.hpp"
#include "directives.hpp"
#include "registers.hpp"
#include "messenger.hpp"
#include "machine.hpp"
#include "labels.hpp"

using namespace std;

typedef enum {CAT_NONE,CAT_INST,CAT_ADDR,CAT_REGI,CAT_MACH} e_category;

typedef struct s_pendency
{
	unsigned int byte;
	string label;
	bool relative;	//se a label eh relativa ao PC ou nao
	unsigned int size;	//numero de bytes que devem ser usados para o valor
}t_pendency;

class Assembler
{

	public:
	/**
	*	cria um montador, especificando quais sao as propriedades da maquina/arquiterua para qual os codigos serao montados
	*/
	Assembler(Instructions inst, Registers reg, Machine machine, Addressings adr);

	/**
	*	le as caracteristicas da arquitetura que estao no arquivo dado
	*/
	Assembler(const char *filename);

	//dummy
	Assembler();

	/**
	*	monta o codigo assembly passado
	* retorna a memoria gerada
	*/
	Memory assembleCode(string code);

	/**
	*	cria o arquivo binario para a memoria
	* o arquivo tera o seguinte formato:
	* primeiro byte: versao (0, no caso)
	* nome da maquina, terminado por um '\0'
	* md5 do resto do arquivo (16 bytes)
	* dump da memoria (size bytes)
	*/
	void createBinaryV0(string filename,string machineName,Memory *memory);

	private:

	stack<t_pendency> pendecies; //bytes em que ha labels pendentes
	Labels labels; //labels definidas
	Instructions inst;
	Registers regs;
	Addressings addr;
	Machine mach;
	Messenger messenger;
	Directives directives;

	/**
	*	monta uma linha, escrevendo seu codigo binario a partir de memory[byte]
	* line eh a linha a ser montada
	* se houver alguma label que ainda nao foi definida, reserva espaco e adiciona a pendencia na pilha
	* se for encontrada a definicao de uma label, acrescenta-a as Labels conhecidas
	* retorna a posicao da memoria em que a proxima linha deve comecar
	*/
	unsigned int assembleLine(string line, Memory *memory,unsigned int byte,unsigned int lineNumber);

};

#endif // ASSEMBLER_HPP

