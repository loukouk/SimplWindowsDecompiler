#ifndef __CRESTRON_SMW_SYMBOL_LIB_H__
#define __CRESTRON_SMW_SYMBOL_LIB_H__


#ifndef STR_SIZE
	#define STR_SIZE 256
#endif

struct SmwSymbolDefinition
{
	char name[STR_SIZE]; 
	int SmC;
};

void DeallocateLibArray(struct SmwSymbolDefinition *arr, int *arr_size);
struct SmwSymbolDefinition *ReadSmwSymbolLib(int *lib_size);


#endif