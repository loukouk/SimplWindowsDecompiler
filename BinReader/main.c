#include <stdio.h>

#include "BinReader.h"

int main(int argc, char **argv)
{
	int num_symbols;
	struct BinSymbol *symbols = NULL;
	char* symbol_header = NULL;

	if (argc != 2)
	{
		printf("No filepath entered. Press the Enter key to close...");
		getc(stdin);
		exit(EXIT_SUCCESS);
	}

	symbols = ReadBinFile(argv[1], &num_symbols, &symbol_header);

	printf("- HEADER -\n%s\n\n", symbol_header);
	PrintBinArray(symbols, num_symbols);
	printf("Done. Total Symbols: %d\n", num_symbols);

	DeallocateBinArray(symbols, &num_symbols);
	if (symbol_header != NULL)
		free(symbol_header);

	getc(stdin);

	return EXIT_SUCCESS;
}