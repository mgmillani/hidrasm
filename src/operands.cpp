
#include <list>

#include "operands.hpp"

using namespace std;

Operands::Operands(list<t_operand> ops)
{
	this->operands = ops;
	this->itMode = ops.begin();
	this->itReg = ops.begin();
	this->itAddr = ops.begin();
}

/**
	* retorna o proximo operando do tipo dado
	*/
t_operand Operands::getNextOperand(char type)
{

	return this->operands.front();

}

/**
	* retorna o n-esimo operando do tipo dado
	*/
t_operand Operands::getOperandIndex(char type, unsigned int index)
{
	return this->operands.front();
}
