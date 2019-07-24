#include <stdio.h>

#include "SymbolLib/SymbolLib.h"
#include "BinReader/BinReader.h"
#include "SigReader/SigReader.h"
//#include "SmwBuilder.c"

void PrintBinArrayWithSignalLib(struct BinSymbol *syms, int sym_size, struct SignalInfo *sigs, int sig_size, struct SmwSymbolDefinition *lib, int lib_size)
{
	unsigned int temp;

	for (int i = 0 ; i < sym_size; i++)
	{
		printf("%s - ", syms[i].number);

		temp = syms[i].id;
		if (temp < lib_size && lib[temp].name != NULL)
			printf("%s", lib[temp].name);
		else
			printf("id:%u", temp);

		if (syms[i].expand != 0)
			printf(" exp:%d", syms[i].expand);
		printf("\n");

		if (syms[i].num_ins != 0 && syms[i].sig_inputs != NULL)
		{
			printf("\tinput signals\n");
			for (int j = 0; j < syms[i].num_ins; j++)
			{
				temp = syms[i].sig_inputs[j];

				if (temp < sig_size && sigs[temp].name != NULL)
					printf("\t\t%s\n", sigs[temp].name);
				else
					printf("\t\t%u\n", temp);
			}
		}

		if (syms[i].num_outs != 0 && syms[i].sig_outputs != NULL)
		{
			printf("\toutput signals\n");
			for (int j = 0; j < syms[i].num_outs; j++)
			{
				temp = syms[i].sig_outputs[j];

				if (temp < sig_size && sigs[temp].name != NULL)
					printf("\t\t%s\n", sigs[temp].name);
				else
					printf("\t\t%u\n", temp);
			}
		}

		if (syms[i].num_params != 0 && syms[i].params != NULL)
		{
			printf("\tparameters\n");
			for (int j = 0; j < syms[i].num_params; j++)
			{
				if (syms[i].params[j].type == PARAM_TYPE_USHORT)
				{
					printf("\t\tushort: %u\n", *((int*)(syms[i].params[j].value)));
				}
				else if (syms[i].params[j].type == PARAM_TYPE_STRING)
				{
					printf("\t\tstring: \"");
					PrintStringCrestronStyle((char*)syms[i].params[j].value, syms[i].params[j].len);
					printf("\"\n");
				}
				else
				{
					printf("\t\tunknown parameter type (0x%04x): ", (unsigned int)syms[i].params[j].type);
					PrintStringCrestronStyle((char*)syms[i].params[j].value, strlen((char*)syms[i].params[j].value));
					printf("\n");
				}
			}
		}

		printf("\n");
	}
}

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
	struct SignalInfo *signals, *signals_indexed;
	int lib_size, num_symbols, num_signals, dup_sig, idx_size;

	lib = ReadSmwSymbolLib(&lib_size);
	symbols = ReadBinFile(argv[1], &num_symbols, NULL);
	signals = ReadSigFile(argv[2], &num_signals, NULL);

	dup_sig = FindDuplicateSignal(signals, num_signals);
	if (dup_sig)
	{
		printf("Error: duplicate signal number (%d)\n\n", dup_sig);
		getc(stdin);
		exit(EXIT_FAILURE);
	}

	signals_indexed = CreateIndexedSigArray(signals, num_signals, &idx_size);
	DeallocateSigArray(signals, &num_signals);

	PrintBinArrayWithSignalLib(symbols, num_symbols, signals_indexed, idx_size, lib, lib_size);

	DeallocateBinArray(symbols, &num_symbols);
	DeallocateSigArray(signals_indexed, &idx_size);
	getc(stdin);

	return EXIT_SUCCESS;
}