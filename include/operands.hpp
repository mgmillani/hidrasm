#ifndef OPERANDS_HPP
#define OPERANDS_HPP

#include <string>

using namespace std;

typedef struct s_operand
{
	string name;
	string addressingCode;	//codigo binario do modo de enderecamento
	string value;
	char type;	//r,l ou n
	bool relative;
}t_operand;

class Operands
{
	public:

	Operands(list<t_operand>);

	/**
	  * retorna o proximo operando do tipo dado
	  */
	t_operand getNextOperand(char type);

	/**
	  * retorna o n-esimo operando do tipo dado
	  */
	t_operand getOperandIndex(char type, unsigned int index);

	private:
	list<t_operand> operands;
	list<t_operand>::iterator itMode;
	list<t_operand>::iterator itReg;
	list<t_operand>::iterator itAddr;
};

#endif // OPERANDS_HPP

