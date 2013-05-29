#include <stdio.h>

#include <list>

#include "operands.hpp"

#include "debug.hpp"
#include "defs.hpp"

using namespace std;

Operands::Operands(list<t_operand> ops)
{
	this->operands = ops;
	this->itMode = this->operands.begin();
	this->itReg = this->operands.begin();
	this->itAddr = this->operands.begin();
}

/**
	* retorna o proximo operando do tipo dado
	*/
t_operand Operands::getNextOperand(char type)
{
	switch(type)
	{
		case ADDRESSING:
			while(this->itMode != this->operands.end())
			{
				if(this->itMode->type == type)
				{
					return *this->itMode;
				}
				this->itMode++;
			}
			break;
		case ADDRESS:
			while(this->itAddr != this->operands.end())
			{
				if(this->itAddr->type == type)
					return *this->itAddr;
				this->itAddr++;
			}
			break;
		case REGISTER:

			while(this->itReg != this->operands.end())
			{
				if(this->itAddr->type == type)
					return *this->itReg;
				this->itReg++;
			}
			break;
	}

	return this->operands.front();

}

/**
	* retorna o n-esimo operando do tipo dado
	*/
t_operand Operands::getOperandIndex(char type, unsigned int index)
{
	return this->operands.front();
}
