#include "SigReader.h"


void InitSigArray(struct SignalInfo *arr, int size)
{
	for (int i = 0; i < size; i++)
	{
		arr[i].name = NULL;
		arr[i].type = 0;
		arr[i].number = 0;
		arr[i].footer[0] = 0;
		arr[i].footer[1] = 0;
	}
}

struct SignalInfo *ResizeSigArray(struct SignalInfo *arr, int *size)
{
	struct SignalInfo *ret;
	int i;

	ret = malloc(sizeof(struct SignalInfo) * (*size) * 2);

	if (ret == NULL)
	{
		printf("malloc() failed. Out of memory?\n");
		getc(stdin);
		exit(EXIT_FAILURE);
	}

	InitSigArray(ret, (*size) * 2);

	for (i = 0; i < (*size); i++)
	{
		ret[i].name = arr[i].name;
		ret[i].type = arr[i].type;
		ret[i].number = arr[i].number;
		ret[i].footer[0] = arr[i].footer[0];
		ret[i].footer[1] = arr[i].footer[1];
	}

	(*size) = (*size) * 2;
	free(arr);

	return ret;
}

void _CopySignal(struct SignalInfo *dest, struct SignalInfo src)
{
	dest->type = src.type;
	dest->number = src.number;
	dest->footer[0] = src.footer[0];
	dest->footer[1] = src.footer[1];

	if (dest->name != NULL)
		free(dest->name);

	dest->name = malloc(sizeof(char) * strlen(src.name));
	if (dest->name == NULL)
	{
		printf("malloc() failed. Out of memory?\n");
		getc(stdin);
		exit(EXIT_FAILURE);
	}

	strcpy(dest->name, src.name);
}

struct SignalInfo *CreateIndexedSigArray(struct SignalInfo *arr, int size, int *idx_size)
{
	struct SignalInfo *ret;
	int ret_size = 0;

	for (int i = 0; i < size; i++)
	{
		if (arr[i].number > ret_size)
		{
			ret_size = arr[i].number;
		}
	}
	ret_size++;

	ret = malloc(sizeof(struct SignalInfo)*ret_size);
	if (ret == NULL)
    {
    	printf("Error: malloc() failed. Out of memory?\n");
    	getc(stdin);
    	exit(EXIT_FAILURE);
    }
    InitSigArray(ret, ret_size);

    for (int i = 0, num = 0; i < size; i++)
    {
    	num = arr[i].number;

    	if (num >= ret_size || num < 0)
    	{
    		printf("Error: Couldn't index signal, signal number is invalid.\n");
    		getc(stdin);
    		exit(EXIT_FAILURE);
    	}
    	
    	_CopySignal(&ret[num], arr[i]);
    }

	*idx_size = ret_size;
	return ret;
}

void DeallocateSigArray(struct SignalInfo *signals, int *size)
{
	if (signals == NULL)
			return;

	for (int i = 0; i < (*size); i++)
	{
		if (signals[i].name != NULL)
		{
			free(signals[i].name);
		}
	}
	free(signals);

	signals = NULL;
	*size = 0;
}

void PrintSigArray(struct SignalInfo *signals, int sig_index)
{
	if (signals == NULL)
		return;

	//printf("%*s%*s     Footer\n",60, "Signal Name", 12, "Number");
	printf("Number\t\t\tType\t\tName\n");

	for (int i = 0; i < sig_index; i++)
	{
		if (signals[i].name != NULL)
		{
			//printf("%*s%*u     0x%02x 0x%02x\n", 60, signals[i].name, 12, signals[i].number, signals[i].footer[0], signals[i].footer[1]);
			printf("0x%08x (%d)  \t0x%02x 0x%02x\t%s\n", signals[i].number, signals[i].number, signals[i].footer[0], signals[i].footer[1], signals[i].name);
		}
	}
	
}

int FindDuplicateSignal(struct SignalInfo *signals, int size)
{
	for (int i=0; i < size; i++)
	{
		for (int j = 0; j < i; j++)
		{
			if (signals[i].number == signals[j].number)
				return signals[j].number;
		}
	}
	return 0;
}

struct SignalInfo *ReadSigFile(char *filepath, int *num_sig, char **header)
{
	HANDLE sig_file;
	char iobuffer[BUF_SIZE], *file_header;
	DWORD num_bytes_read;
	int state = 0, sig_size = SIG_SIZE_DEFAULT, name_num_bytes;
	int name_index = 0, sig_index = 0;
	struct SignalInfo *signals;
	unsigned char c;
	unsigned int sig_num;
	unsigned long total_bytes_read = 0;

	sig_file = CreateFile(filepath,            // file to open
                      	  GENERIC_READ,          // open for reading
                      	  FILE_SHARE_READ,       // share for reading
                      	  NULL,                  // default security
                     	  OPEN_EXISTING,         // existing file only
                      	  FILE_ATTRIBUTE_NORMAL, // normal file
                      	  NULL);

    if (sig_file == INVALID_HANDLE_VALUE) 
    { 
        printf(TEXT("Error: unable to open \"%s\" for read.\nPress any key to continue..."), filepath);
        getc(stdin);
        exit(EXIT_FAILURE);
    }

    file_header = malloc(sizeof(char) * STR_SIZE);
    if (file_header == NULL)
    {
    	printf("Error: malloc() failed. Out of memory?\n");
    	getc(stdin);
    	exit(EXIT_FAILURE);
    }
    memset(file_header, '\0', STR_SIZE);

    signals = malloc(sizeof(struct SignalInfo)*sig_size);
    if (signals == NULL)
    {
		printf("Error: malloc() failed. Out of memory?\n");
		getc(stdin);
    	exit(EXIT_FAILURE);
    }
	InitSigArray(signals, sig_size);

	do
	{
    	memset(iobuffer, '\0', BUF_SIZE);
	    if (FALSE == ReadFile(sig_file, iobuffer, (DWORD)BUF_SIZE, &num_bytes_read, NULL))
	    {
	    	printf("Error: Could not read from \"%s\".\nPress the Enter key to close...", filepath);
	    	getc(stdin);
        	exit(EXIT_FAILURE); 
	    }
	    total_bytes_read += num_bytes_read;

	    for (int buf_index = 0; buf_index < num_bytes_read; buf_index++)
	    {
	    	c = iobuffer[buf_index];

	    	if (DEBUG)
	    		printf("c: %c, state: %d, sig: %d\n", c, state, sig_index);

	    	switch(state)
	    	{
	    		case SIG_STATE_FILE_HEADER:
	    			file_header[name_index] = c;
	    			if (c == ']')
	    			{
	    				name_index = 0;
	    				state = SIG_STATE_SIGNAL_HEADER;
	    			}
	    			else 
	    			{
	    				name_index++;

	    				if (name_index >= STR_SIZE)
		    			{
		    				printf("Header too long. Is this a Crestron .sig file?\n");
		    				getc(stdin);
		    				exit(EXIT_FAILURE);
		    			}
	    			}
					break;

				case SIG_STATE_SIGNAL_HEADER:
					name_num_bytes = (((int) c) - 8) / 2;
					if (name_num_bytes < 1)	//sanity check
					{
						printf("File not properly formatted. Is this a Crestron .sig file?\n");
						getc(stdin);
						exit(EXIT_FAILURE);
					}

					if (sig_index >= sig_size)
					{
						signals = ResizeSigArray(signals, &sig_size);
						//printf("new size: %d\n", sig_size);
					}

					signals[sig_index].name = malloc(sizeof(char) * (name_num_bytes+1));
					if (signals[sig_index].name == NULL)
					{
						printf("Error: malloc() failed. Out of memory?\n");
						getc(stdin);
						exit(EXIT_FAILURE);
					}
					memset(signals[sig_index].name, '\0', name_num_bytes+1);

					name_index = 0;
					state = SIG_STATE_SIGNAL_NAME_EMPTY;
					break;

				case SIG_STATE_SIGNAL_NAME_EMPTY:
					state = SIG_STATE_SIGNAL_NAME_CHAR;
					break;

				case SIG_STATE_SIGNAL_NAME_CHAR:
					if (name_index >= STR_SIZE)
					{
						printf("Signal Name too long. Is this a Crestron .sig file?\n");
						getc(stdin);
						exit(EXIT_FAILURE);
					}
					signals[sig_index].name[name_index] = c;
					name_index++;

					if (name_index < name_num_bytes)
					{
						state = SIG_STATE_SIGNAL_NAME_EMPTY;
					}
					else
					{
						state = SIG_STATE_SIGNAL_NUMBER_EMPTY;
					}
					break;

				case SIG_STATE_SIGNAL_NUMBER_EMPTY:
					state = SIG_STATE_SIGNAL_NUMBER_BYTE1;
					break;

				case SIG_STATE_SIGNAL_NUMBER_BYTE1:
					sig_num = (unsigned int) c;
					state = SIG_STATE_SIGNAL_NUMBER_BYTE2;
					break;

				case SIG_STATE_SIGNAL_NUMBER_BYTE2:
					sig_num |= ((unsigned int) c) << 8U;
					state = SIG_STATE_SIGNAL_NUMBER_BYTE3;
					break;

				case SIG_STATE_SIGNAL_NUMBER_BYTE3:
					sig_num |= ((unsigned int) c) << 16U;
					state = SIG_STATE_SIGNAL_NUMBER_BYTE4;
					break;

				case SIG_STATE_SIGNAL_NUMBER_BYTE4:
					sig_num |= ((unsigned int) c) << 24U;
					signals[sig_index].number = sig_num;
					state = SIG_STATE_SIGNAL_FOOTER_BYTE1;
					break;

				case SIG_STATE_SIGNAL_FOOTER_BYTE1:
					signals[sig_index].footer[0] = (int) c;
					state = SIG_STATE_SIGNAL_FOOTER_BYTE2;
					break;

				case SIG_STATE_SIGNAL_FOOTER_BYTE2:
					signals[sig_index].footer[1] = (int) c;
					state = SIG_STATE_SIGNAL_HEADER;
					sig_index++;
					break;
	    	}

	    } //for

	} while (num_bytes_read >= BUF_SIZE);

	CloseHandle(sig_file);

	if (header != NULL)
		*header = file_header;

	if (num_sig != NULL)
		*num_sig = sig_index;
	
	return signals;
}
