#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "assembler.hpp"
#include "memory.hpp"

using namespace std;
/**
	*	le um arquivo binario, escrevendo seu tamanho em size
	* retorna um ponteiro para o arquivo
	*/
char *readBinFile(const char *filename, int *size)
{

	FILE *fl = fopen(filename,"rb");
	if(fl == NULL)
		return NULL;

	fseek(fl,0,SEEK_END);
	*size = ftell(fl);
	fseek(fl,0,SEEK_SET);

	char *text = (char *)malloc(*size);
	fread(text,1,*size,fl);
	fclose(fl);

	return text;
}

int main(int argc, char *argv[])
{
	if(argc != 4)
	{
		printf("usage: binv0 <memoryFile> <machine name> <destination>\n");
		return 1;
	}

	char *memFile = argv[1];
	char *machName = argv[2];
	char *dest = argv[3];

	int size;
	char *mem = readBinFile(memFile,&size);
	Memory m (size);
	m.writeArray((unsigned char *)mem,size,0);

	Assembler mont;

	mont.createBinaryV0(string(dest),string(machName),&m);
	free(mem);

	return 0;

}
