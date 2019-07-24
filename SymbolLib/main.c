#include<stdio.h>

#include "SymbolLib.h"


int main()
{
	int num_defs = 0, defs_size;
	struct SmwSymbolDefinition *defs = ReadSmwSymbolLib(&defs_size);

	for (int i = 0; i < defs_size; i++)
	{
		if (defs[i].SmC)
		{
			printf("id: %d    \tSmC: %d  \t%s\n", i, defs[i].SmC, defs[i].name);
			num_defs++;
		}
	}
	printf("\nDone. Actual number of symbol definitions: %d\n", num_defs);
	getc(stdin);
}