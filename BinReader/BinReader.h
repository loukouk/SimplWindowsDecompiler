#ifndef __CRESTRON_BIN_READER_H__
#define __CRESTRON_BIN_READER_H__


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

#ifndef BUF_SIZE
	#define BUF_SIZE 1024
#endif

#ifndef STR_SIZE
	#define STR_SIZE 256
#endif

#ifndef SYM_SIZE_DEFAULT
	#define SYM_SIZE_DEFAULT 16
#endif

#ifndef DEBUG
	#define DEBUG 0
#endif

enum
{
	BIN_STATE_FILE_HEADER_SIZE = 0,
	BIN_STATE_FILE_HEADER,
	BIN_STATE_SYMBOL_HEADER_LOW_WORD,
	BIN_STATE_SYMBOL_HEADER_HIGH_WORD,
	BIN_STATE_SYMBOL_ID_LOW_WORD,
	BIN_STATE_SYMBOL_ID_HIGH_WORD,
	BIN_STATE_SYMBOL_SIG_INPUTS_LOW_WORD,
	BIN_STATE_SYMBOL_SIG_INPUTS_HIGH_WORD,
	BIN_STATE_SYMBOL_SIG_OUTPUTS_LOW_WORD,
	BIN_STATE_SYMBOL_SIG_OUTPUTS_HIGH_WORD,
	BIN_STATE_SYMBOL_NUMBER_LENGTH,
	BIN_STATE_SYMBOL_NUMBER_UNKNOWN,
	BIN_STATE_SYMBOL_NUMBER_VALUE,
	BIN_STATE_SYMBOL_PARAMETER_LENGTH,
	BIN_STATE_SYMBOL_PARAMETER_TYPE,
	BIN_STATE_SYMBOL_PARAMETER_NUM_LOW_WORD,
	BIN_STATE_SYMBOL_PARAMETER_NUM_HIGH_WORD,
	BIN_STATE_SYMBOL_PARAMETER_USHORT_LOW_WORD,
	BIN_STATE_SYMBOL_PARAMETER_USHORT_HIGH_WORD,
	BIN_STATE_SYMBOL_PARAMETER_EXPAND_LOW_WORD,
	BIN_STATE_SYMBOL_PARAMETER_EXPAND_HIGH_WORD,
	BIN_STATE_SYMBOL_PARAMETER_STRING,
	BIN_STATE_SYMBOL_PARAMETER_SKIP,
	BIN_STATE_SYMBOL_SIG_OUTPUT_VALUE_LOW_WORD,
	BIN_STATE_SYMBOL_SIG_OUTPUT_VALUE_HIGH_WORD,
	BIN_STATE_SYMBOL_SIG_INPUT_VALUE_LOW_WORD,
	BIN_STATE_SYMBOL_SIG_INPUT_VALUE_HIGH_WORD,
	BIN_STATE_DEVICE_DEFINITION
};

enum
{
	PARAM_TYPE_USHORT = 0x0002,
	PARAM_TYPE_STRING = 0x0003,
	PARAM_TYPE_NUM_PARAMS = 0x000f,
	PARAM_TYPE_EXPAND	= 0x0005
};

struct Parameter
{
	int type;
	unsigned int len;
	void *value;
};

struct BinSymbol
{
	unsigned int id;
	char *number;
	unsigned int num_params;
	unsigned int num_ins;
	unsigned int num_outs;
	struct Parameter *params;
	unsigned int *sig_outputs;
	unsigned int *sig_inputs;
	unsigned int expand;
};

void ResizeString(char **str, int *size);
void PrintStringCrestronStyle(char* str, int len);
void PrintBinArray(struct BinSymbol *syms, int size);
void DeallocateBinArray(struct BinSymbol *arr, int *size);
void InitBinArray(struct BinSymbol *arr, int size);
void ResizeBinArray(struct BinSymbol **arr, int *size);
struct BinSymbol *ReadBinFile(char *filepath, int *num_symbols, char **file_header);


#endif