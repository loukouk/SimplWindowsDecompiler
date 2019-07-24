#ifndef __CRESTRON_SIG_READER_H__
#define __CRESTRON_SIG_READER_H__


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

#ifndef SIG_SIZE_DEFAULT
	#define SIG_SIZE_DEFAULT 16
#endif

#ifndef DEBUG
	#define DEBUG 0
#endif

struct SignalInfo
{
	char *name;
	int type;
	unsigned int number;
	int footer[2];
};

enum
{
	SIG_STATE_FILE_HEADER = 0,
	SIG_STATE_SIGNAL_HEADER,
	SIG_STATE_SIGNAL_NAME_EMPTY,
	SIG_STATE_SIGNAL_NAME_CHAR,
	SIG_STATE_SIGNAL_NUMBER_EMPTY,
	SIG_STATE_SIGNAL_NUMBER_BYTE1,
	SIG_STATE_SIGNAL_NUMBER_BYTE2,
	SIG_STATE_SIGNAL_NUMBER_BYTE3,
	SIG_STATE_SIGNAL_NUMBER_BYTE4,
	SIG_STATE_SIGNAL_FOOTER_BYTE1,
	SIG_STATE_SIGNAL_FOOTER_BYTE2
};

enum
{
	SIG_TYPE_DIGITAL = 0,
	SIG_TYPE_ANALOG,
	SIG_TYPE_SERIAL
};

void DeallocateSigArray(struct SignalInfo *signals, int *size);
struct SignalInfo *CreateIndexedSigArray(struct SignalInfo *arr, int size, int *new_size);
void PrintSigArray(struct SignalInfo *signals, int sig_index);
int FindDuplicateSignal(struct SignalInfo *signals, int size);
struct SignalInfo *ReadSigFile(char *filepath, int *num_sig, char **header);


#endif