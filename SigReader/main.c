#include <stdio.h>

#include "SigReader.h"


int main(int argc, char **argv)
{
	struct SignalInfo *signals, *signals_indexed;
	int num_signals, idx_size;
	char *file_header = NULL;
	int dup_sig = 0;

	if (argc != 2)
	{
		printf("No filepath entered. Press the Enter key to close...");
		getc(stdin);
		exit(EXIT_SUCCESS);
	}

	signals = ReadSigFile(argv[1], &num_signals, &file_header);

	dup_sig = FindDuplicateSignal(signals, num_signals);
	if (dup_sig)
	{
		printf("Warning: duplicate signal number (%d)\n\n", dup_sig);
	}

	signals_indexed = CreateIndexedSigArray(signals, num_signals, &idx_size);

	printf("- HEADER -\n%s\n\n", file_header);

	//PrintSigArray(signals, num_signals);
	//printf("\nTotal Signals: %d\n", num_signals);
	PrintSigArray(signals_indexed, idx_size);
	printf("Max Indexed Number: %d\n", idx_size-1);

	DeallocateSigArray(signals, &num_signals);
	DeallocateSigArray(signals_indexed, &idx_size);

	getc(stdin);

	return EXIT_SUCCESS;
}