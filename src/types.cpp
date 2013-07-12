#include <stdio.h>

#include "types.hpp"
#include "debug.hpp"
/**
  * determina se A eh um subtipo de B
  */
bool isSubtype(e_type A, e_type B)
{
	//ERR("Comparing %u and %u\n",A,B);
	if(B == TYPE_ANYTHING)
		return true;
	if(B == TYPE_ADDRESS)
		return A==TYPE_LABEL || A==TYPE_NUMBER;
	return A==B;

}
