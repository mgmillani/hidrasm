#ifndef OPERANDS_HPP
#define OPERANDS_HPP

#include <string>

#include "types.hpp"

using namespace std;

typedef struct s_operand
{
	string name;
	string addressingCode;	//codigo binario do modo de enderecamento
	string value;
	e_type type;	//r,l ou n
	bool relative;
}t_operand;

class Operands
{
	public:

	Operands(list<t_operand>);

	/**
	  * retorna o proximo operando do tipo dado
	  */
	t_operand getNextOperand(e_type type);

	/**
	  * retorna o n-esimo operando do tipo dado
	  */
	t_operand getOperandIndex(e_type type, unsigned int index);

	private:
	list<t_operand> operands;
	list<t_operand>::iterator itMode;
	list<t_operand>::iterator itReg;
	list<t_operand>::iterator itAddr;
};

#endif // OPERANDS_HPP

