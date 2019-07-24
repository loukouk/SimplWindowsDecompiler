#include <stdio.h>

#include "SymbolLib/SymbolLib.h"
#include "SigReader/SigReader.h"
//#include "SmwBuilder.c"


int main(int argc, char **argv)
{
	if (argc != 3)
		return EXIT_FAILURE;

	//decompile LPZ
	//decompile SIG
	//get SMW symbol library
	//build smw

	struct SmwSymbolDefinition *lib;
	struct BinSymbol *symbols;
	struct SignalInfo *signals;
	int lib_size, num_symbols, num_signals, dup_sig;

	lib = ReadSmwSymbolLib(&lib_size);
	symbols = ParseBinFile(argv[1], &num_symbols, NULL)
	signals = ParseSigFile(argv[2], &num_signals, NULL);

	dup_sig = FindDuplicateSignal(signals, num_signals);
	if (dup_sig)
	{
		printf("Error: duplicate signal number (%d)\n\n", dup_sig);
		getc(stdin);
		exit(EXIT_FAILURE);
	}

	signals_indexed = CreateIndexedSigArray(signals, num_signals, &idx_size);
	DeallocateSigArray(signals, &num_signals);

	//PrintSigArray(signals, num_signals);
	//printf("\nTotal Signals: %d\n", num_signals);
	PrintSigArray(signals_indexed, idx_size);
	printf("Max Indexed Number: %d\n", idx_size-1);


	DeallocateSigArray(signals_indexed, &idx_size);


	getc(stdin);

	return EXIT_SUCCESS;
}