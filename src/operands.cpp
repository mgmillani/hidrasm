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
t_operand Operands::getNextOperand(e_type type)
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
				{
					return *this->itAddr;
				}
				this->itAddr++;
			}

			for(this->itAddr=this->operands.begin() ; this->itAddr!=this->operands.end() ; this->itAddr++)
			{
				char typeFound = this->itAddr->type;
				if(ISADDRESS(typeFound))
				{
					return *this->itAddr;
				}
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
		default:
			break;
	}

	return this->operands.front();

}

/**
	* retorna o n-esimo operando do tipo dado
	*/
t_operand Operands::getOperandIndex(e_type type, unsigned int index)
{
	unsigned int i;
	switch(type)
	{
		case ADDRESSING:
			this->itMode = this->operands.begin();

			for(i=0 ; i<index && this->itMode!=this->operands.end() ; this->itMode++)
			{
				if(this->itMode->type == type)
					i++;
			}
			if(i==index)
				throw(eOperandNotFound);
			return *this->itMode;
			break;
		case ADDRESS:
			this->itAddr = this->operands.begin();

			for(i=0 ; i<index && this->itAddr!=this->operands.end() ; this->itAddr++)
			{
				if(this->itAddr->type == type)
					i++;
			}
			if(i==index)
				throw(eOperandNotFound);
			return *this->itAddr;
			break;
		case REGISTER:
			this->itReg = this->operands.begin();

			for(i=0 ; i<index && this->itReg!=this->operands.end() ; this->itReg++)
			{
				if(this->itReg->type == type)
					i++;
			}
			if(i==index)
				throw(eOperandNotFound);
			return *this->itReg;
			break;
		default:
			break;
	}
	return this->operands.front();
}
