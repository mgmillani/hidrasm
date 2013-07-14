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
		case TYPE_ADDRESSING:
			while(this->itMode != this->operands.end())
			{
				if(this->itMode->type == type)
					return *this->itMode;

				this->itMode++;
			}
			break;
		case TYPE_ADDRESS:
			while(this->itAddr != this->operands.end())
			{
				if(this->itAddr->type == type)
					return *this->itAddr;
				this->itAddr++;
			}

			for(this->itAddr=this->operands.begin() ; this->itAddr!=this->operands.end() ; this->itAddr++)
			{
				e_type typeFound = this->itAddr->type;
				if(isSubtype(typeFound,type))
					return *this->itAddr;
			}
			break;
		case TYPE_REGISTER:
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
		case TYPE_ADDRESSING:
			this->itMode = this->operands.begin();

			//se nao existirem operandos o suficiente
			if(this->operands.size() <= index)
				throw(eOperandNotFound);

			for(i=0 ; i<index && this->itMode!=this->operands.end() ; this->itMode++,i++)
				;
			ERR("Found: %s\n",this->itMode->name.c_str());
			return *this->itMode;
			break;
		case TYPE_ADDRESS:
			this->itAddr = this->operands.begin();

			for(i=0 ; i<=index && this->itAddr!=this->operands.end() ; this->itAddr++)
			{
				if(this->itAddr->type == type)
					i++;
			}
			if(i==index)
				throw(eOperandNotFound);
			return *this->itAddr;
			break;
		case TYPE_REGISTER:
			this->itReg = this->operands.begin();

			for(i=0 ; i<=index && this->itReg!=this->operands.end() ; this->itReg++)
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
