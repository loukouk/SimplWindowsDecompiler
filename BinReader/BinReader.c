#include "BinReader.h"

void ResizeString(char **str, int *size)
{
	char *ret = malloc(sizeof(char) * (*size) * 2);
	if (ret == NULL)
	{
		printf("malloc() failed. Out of memory?\n");
		getc(stdin);
		exit(EXIT_FAILURE);
	}
	memset(ret, '\0', (*size)*2);

	for (int i = 0; i < (*size); i++)
	{
		ret[i] = (*str)[i];
	}

	free(*str);
	(*size) *= 2;

	*str = ret;
}

void PrintStringCrestronStyle(char* str, int len)
{
	unsigned char c;

	if (len == 0)
		return;

	for (int i = 0; i < len; i++)
	{
		c = str[i];
		if (c >= 0x20 && c < 0x7f)
		{
			printf("%c", c);
		}
		else
		{
			printf("\\x%02x", c);
		}
	}
}

void PrintBinArray(struct BinSymbol *syms, int size)
{
	for (int i = 0 ; i < size; i++)
	{
		printf("%s - id:%u", syms[i].number, syms[i].id);
		if (syms[i].expand != 0)
			printf(" exp:%d", syms[i].expand);
		printf("\n");

		if (syms[i].num_ins != 0 && syms[i].sig_inputs != NULL)
		{
			printf("\tinput signals\n");
			for (int j = 0; j < syms[i].num_ins; j++)
			{
				printf("\t\t%u\n",syms[i].sig_inputs[j]);
			}
		}

		if (syms[i].num_outs != 0 && syms[i].sig_outputs != NULL)
		{
			printf("\toutput signals\n");
			for (int j = 0; j < syms[i].num_outs; j++)
			{
				printf("\t\t%u\n",syms[i].sig_outputs[j]);
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

void DeallocateBinArray(struct BinSymbol *arr, int *size)
{
	if (arr == NULL)
		return;

	for (int i = 0; i < (*size); i++)
	{
		if (arr[i].params != NULL)
			free(arr[i].params);

		if (arr[i].sig_outputs != NULL)
			free(arr[i].sig_outputs);

		if (arr[i].sig_inputs != NULL)
			free(arr[i].sig_inputs);
	}
	
	free(arr);
	arr = NULL;
	*size = 0;
}

void InitBinArray(struct BinSymbol *arr, int size)
{
	for (int i = 0 ; i < size; i++)
	{
		arr[i].id = -1;
		arr[i].num_params = 0;
		arr[i].num_ins = 0;
		arr[i].num_outs = 0;
		arr[i].number = NULL;
		arr[i].params = NULL;
		arr[i].sig_outputs = NULL;
		arr[i].sig_inputs = NULL;
		arr[i].expand = 0;
	}	
}

void ResizeBinArray(struct BinSymbol **arr, int *size)
{
	struct BinSymbol *ret;
	int i;

	ret = malloc(sizeof(struct BinSymbol) * (*size) * 2);

	if (ret == NULL)
	{
		printf("malloc() failed. Out of memory?\n");
		getc(stdin);
		exit(EXIT_FAILURE);
	}

	InitBinArray(ret, (*size)*2);
	for (i = 0; i < (*size); i++)
	{
		ret[i].id = (*arr)[i].id;
		ret[i].number = (*arr)[i].number;
		ret[i].num_params = (*arr)[i].num_params;
		ret[i].num_outs = (*arr)[i].num_outs;
		ret[i].num_ins = (*arr)[i].num_ins;
		ret[i].params = (*arr)[i].params;
		ret[i].sig_outputs = (*arr)[i].sig_outputs;
		ret[i].sig_inputs = (*arr)[i].sig_inputs;
		ret[i].expand = (*arr)[i].expand;
	}

	(*size) = (*size) * 2;
	free(*arr);
	*arr = ret;
}

struct BinSymbol *ReadBinFile(char *filepath, int *num_symbols, char **file_header)
{
	HANDLE bin_file;
	char iobuffer[BUF_SIZE], *temp_str, *header;
	DWORD num_bytes_read;
	int STATE = 0, sym_size = SYM_SIZE_DEFAULT;
	int str_index = 0, sym_index = -1, header_size = STR_SIZE;
	struct BinSymbol *symbols;
	unsigned char c[2];
	unsigned long total_bytes_read = 0;
	unsigned int counter = 0, limit = 0, limit_outs = 0, limit_ins = 0;
	unsigned int word, dword, str_len = 0, sig_index = 0, *temp_ptr;

	bin_file = CreateFile(filepath,            // file to open
                      	  GENERIC_READ,          // open for reading
                      	  FILE_SHARE_READ,       // share for reading
                      	  NULL,                  // default security
                     	  OPEN_EXISTING,         // existing file only
                      	  FILE_ATTRIBUTE_NORMAL, // normal file
                      	  NULL);

    if (bin_file == INVALID_HANDLE_VALUE) 
    { 
        printf(TEXT("Error: unable to open \"%s\" for read.\nPress any key to continue..."), filepath);
        getc(stdin);
        exit(EXIT_FAILURE);
    }

    symbols = malloc(sizeof(struct BinSymbol)*sym_size);
   	if (symbols == NULL)
	{
		printf("malloc() failed. Out of memory?\n");
		getc(stdin);
		exit(EXIT_FAILURE);
	}
    InitBinArray(symbols, sym_size);

    header = malloc(sizeof(char) * header_size);
   	if (header == NULL)
	{
		printf("malloc() failed. Out of memory?\n");
		getc(stdin);
		exit(EXIT_FAILURE);
	}
    memset(header, '\0', header_size);

	do
	{
    	memset(iobuffer, '\0', BUF_SIZE);
	    if (FALSE == ReadFile(bin_file, iobuffer, (DWORD)BUF_SIZE, &num_bytes_read, NULL))
	    {
	    	printf("Error: Could not read from \"%s\".\nPress the Enter key to close...", filepath);
	    	getc(stdin);
        	exit(EXIT_FAILURE); 
	    }
	    total_bytes_read += num_bytes_read;

	    for (int buf_index = 0; buf_index < num_bytes_read; buf_index += 2)
	    {
	    	c[0] = iobuffer[buf_index];

	    	if (buf_index == num_bytes_read - 1)
	    		c[1] = 0x00;
	    	else
	    		c[1] = iobuffer[buf_index + 1];

	    	word =  (unsigned int)c[1] << 8U;
	    	word += (unsigned int)c[0];

	    	if (DEBUG)
	    		printf("c[0]: %x, c[1]: %x, word: %u, state: %d, counter: %d, sym_index: %d\n", (int)c[0],(int)c[1],word, STATE, counter,sym_index);

	    	switch(STATE)
	    	{
				case BIN_STATE_FILE_HEADER_SIZE:
					str_index = 0;
					limit = word/2;
					counter = 1;
					STATE++;

					if (limit <= counter)
					{
						printf("Error: Parsing header failed.\n");
	    				getc(stdin);
        				exit(EXIT_FAILURE);
					}
					break;
				case BIN_STATE_FILE_HEADER:
					if ((word >= 0x20 && word < 0x7f) || word == 0x0a || word == 0x09)
					{
						if (str_index >= header_size-1)
						{
							ResizeString(&header, &header_size);
						}
						header[str_index] = (char)word;
						str_index++;
					}
					
					counter++;
					if (counter >= limit)
					{
						STATE++;
					}
					break;
				case BIN_STATE_SYMBOL_HEADER_LOW_WORD:
					dword = word & 0xFFFF;
					counter = 1;
					STATE++;
					break;
				case BIN_STATE_SYMBOL_HEADER_HIGH_WORD:
					dword |= word << 16;
					if (dword % 2 || dword < 8)
					{
						printf("Error: Parsing symbol header failed, number of bytes is invalid (%d).\n", word);
	    				getc(stdin);
        				exit(EXIT_FAILURE);
					}
					counter++;
					limit = dword/2;

					if (DEBUG)
						printf("counter limit: %d\n", limit);

					STATE++;
					break;
				case BIN_STATE_SYMBOL_ID_LOW_WORD:
					dword = word & 0xFFFF;
					counter++;
					STATE++;
					break;
				case BIN_STATE_SYMBOL_ID_HIGH_WORD:
					dword |= word << 16;

					counter++;
					if (dword > 0x00ff)		//if device dev, skip for now
					{
						STATE = BIN_STATE_DEVICE_DEFINITION;
					}
					else 
					{
						sym_index++;
						if (sym_index >= sym_size)
						{
							ResizeBinArray(&symbols, &sym_size);
						}

						symbols[sym_index].id = dword;

						if (DEBUG)
							printf("sym ID: %d\n", dword);

						STATE++;
					}
					break;
				case BIN_STATE_SYMBOL_SIG_INPUTS_LOW_WORD:
					dword = word & 0xFFFF;
					counter++;
					STATE++;
					break;
				case BIN_STATE_SYMBOL_SIG_INPUTS_HIGH_WORD:
					dword |= word << 16;
					limit_ins = dword/2;
					symbols[sym_index].num_ins = (limit - limit_ins) / 2;

					if (DEBUG)
						printf("limit_ins: %d, num_ins: %d\n", limit_ins, symbols[sym_index].num_ins);

					counter++;
					STATE++;
					break;
				case BIN_STATE_SYMBOL_SIG_OUTPUTS_LOW_WORD:
					dword = word & 0xFFFF;
					counter++;
					STATE++;
					break;
				case BIN_STATE_SYMBOL_SIG_OUTPUTS_HIGH_WORD:
					dword |= word << 16;
					limit_outs = dword/2;

					if (limit_outs > limit_ins)
					{
						printf("Error: Parsing input and output signals failed.\n");
	    				getc(stdin);
        				exit(EXIT_FAILURE); 
					}
					symbols[sym_index].num_outs = (limit_ins - limit_outs) / 2;

					if (DEBUG)
						printf("limit_outs: %d, num_outs: %d\n", limit_outs, symbols[sym_index].num_outs);

					if (symbols[sym_index].num_ins != 0)
					{
						symbols[sym_index].sig_inputs = malloc(sizeof(unsigned int)*symbols[sym_index].num_ins);
					   	if (symbols[sym_index].sig_inputs == NULL)
						{
							printf("malloc() failed. Out of memory?\n");
							getc(stdin);
							exit(EXIT_FAILURE);
						}
					}

					if (symbols[sym_index].num_outs != 0)
					{
						symbols[sym_index].sig_outputs = malloc(sizeof(unsigned int)*(symbols[sym_index].num_outs+1));
						if (symbols[sym_index].sig_outputs == NULL)
						{
							printf("malloc() failed. Out of memory?\n");
							getc(stdin);
							exit(EXIT_FAILURE);
						}
					}

					counter++;
					STATE++;
					break;
				case BIN_STATE_SYMBOL_NUMBER_LENGTH:
					str_len = (word/2) - 1;
					str_index = 0;

					symbols[sym_index].number = malloc(sizeof(char)*(str_len+1));
					if (symbols[sym_index].number == NULL)
					{
						printf("malloc() failed. Out of memory?\n");
						getc(stdin);
						exit(EXIT_FAILURE);
					}
					memset(symbols[sym_index].number, '\0', str_len+1);

					counter++;
					STATE++;
					break;
				case BIN_STATE_SYMBOL_NUMBER_UNKNOWN:
					counter++;
					STATE++;
					break;
				case BIN_STATE_SYMBOL_NUMBER_VALUE:
					symbols[sym_index].number[str_index] = (char)(word & 0xFF);
					str_index++;
					counter++;
					if (str_index >= str_len)
					{
						if (DEBUG)
							printf("name: %s\n", symbols[sym_index].number);

						sig_index = 0;
						symbols[sym_index].num_params = 0;

						if (counter == limit)
						{
							STATE = BIN_STATE_SYMBOL_HEADER_LOW_WORD;
						}
						else if (counter == limit_ins)
						{
							STATE = BIN_STATE_SYMBOL_SIG_INPUT_VALUE_LOW_WORD;
						}
						else if (counter == limit_outs)
						{
							STATE = BIN_STATE_SYMBOL_SIG_OUTPUT_VALUE_LOW_WORD;
						}
						else if (counter < limit_outs)
						{
							STATE = BIN_STATE_SYMBOL_PARAMETER_LENGTH;
						}
						else
						{
							printf("Error: Parsing parameters and signals failed.\n");
							getc(stdin);
							exit(EXIT_FAILURE); 
						}
					}
					break;
				case BIN_STATE_SYMBOL_PARAMETER_LENGTH:				//TODO PARAMETERS
					str_len = (word/2) - 1;

					if (str_len < 0)
					{
						printf("Error: Parsing parameter. Header size negative?\n");
						getc(stdin);
        				exit(EXIT_FAILURE);
					}

					counter++;
					STATE++;
					break;
				case BIN_STATE_SYMBOL_PARAMETER_TYPE:
					counter++;
					switch (word)
					{
						case PARAM_TYPE_NUM_PARAMS:
							if (symbols[sym_index].num_params != 0 || sig_index != 0)
							{
								printf("Error: Parsing parameter. Number of parameters is already defined\n");
								getc(stdin);
        						exit(EXIT_FAILURE);
							}
							STATE = BIN_STATE_SYMBOL_PARAMETER_NUM_LOW_WORD;
							break;
						case PARAM_TYPE_USHORT:
							if (str_len != 2)
							{
								printf("Error: Parsing parameter. Unexpected parameter length for ushort\n");
								getc(stdin);
        						exit(EXIT_FAILURE);
							}
							if (sig_index >= symbols[sym_index].num_params)
							{
								printf("Error: Parsing parameter. More parameters available than defined (num_params=%d)\n",symbols[sym_index].num_params);
								getc(stdin);
        						exit(EXIT_FAILURE);
							}
							STATE = BIN_STATE_SYMBOL_PARAMETER_USHORT_LOW_WORD;
							break;

						case PARAM_TYPE_STRING:
							if (sig_index >= symbols[sym_index].num_params)
							{
								printf("Error: Parsing parameter. More parameters available than defined\n");
								getc(stdin);
        						exit(EXIT_FAILURE);
							}

							if (DEBUG)
								printf("string parameter len: %d\n", str_len);

							str_index = 0;
							symbols[sym_index].params[sig_index].type = PARAM_TYPE_STRING;
							symbols[sym_index].params[sig_index].len = str_len;
							
							if (str_len == 0)
							{
								if (counter == limit)
								{
									STATE = BIN_STATE_SYMBOL_HEADER_LOW_WORD;
								}
								else if (counter == limit_ins)
								{
									sig_index = 0;
									STATE = BIN_STATE_SYMBOL_SIG_INPUT_VALUE_LOW_WORD;
								}
								else if (counter == limit_outs)
								{
									sig_index = 0;
									STATE = BIN_STATE_SYMBOL_SIG_OUTPUT_VALUE_LOW_WORD;
								}
								else if (counter < limit_outs)
								{
									sig_index++;
									if (sig_index < symbols[sym_index].num_params)
										STATE = BIN_STATE_SYMBOL_PARAMETER_LENGTH;
									else
										STATE = BIN_STATE_SYMBOL_PARAMETER_SKIP;
								}
								else
								{
									printf("Error: Parsing parameters and signals failed.\n");
									getc(stdin);
									exit(EXIT_FAILURE); 
								}
							}
							else
							{
								temp_str = malloc(sizeof(char) * str_len);
								if (temp_str == NULL)
								{
									printf("malloc() failed. Out of memory?\n");
									getc(stdin);
									exit(EXIT_FAILURE);
								}
								memset(temp_str, '\0', str_len);

								STATE = BIN_STATE_SYMBOL_PARAMETER_STRING;
							}
							break;

						case PARAM_TYPE_EXPAND:
							if (str_len != 2)
							{
								printf("Error: Parsing parameter. Unexpected parameter length for Expansion param (%d)\n", str_len);
								getc(stdin);
        						exit(EXIT_FAILURE);
							}
							STATE = BIN_STATE_SYMBOL_PARAMETER_EXPAND_LOW_WORD;
							break;
						default:
							if (counter == limit)
							{
								STATE = BIN_STATE_SYMBOL_HEADER_LOW_WORD;
							}
							else if (counter == limit_ins)
							{
								sig_index = 0;
								STATE = BIN_STATE_SYMBOL_SIG_INPUT_VALUE_LOW_WORD;
							}
							else if (counter == limit_outs)
							{
								sig_index = 0;
								STATE = BIN_STATE_SYMBOL_SIG_OUTPUT_VALUE_LOW_WORD;
							}
							else if (counter < limit_outs)
							{
								if (sig_index < symbols[sym_index].num_params)
									STATE = BIN_STATE_SYMBOL_PARAMETER_LENGTH;
								else
									STATE = BIN_STATE_SYMBOL_PARAMETER_SKIP;
							}
							else
							{
								printf("Error: Parsing parameters and signals failed.\n");
								getc(stdin);
								exit(EXIT_FAILURE); 
							}
							break;
					}

					if ( (word == PARAM_TYPE_USHORT || word == PARAM_TYPE_STRING) && symbols[sym_index].num_params == 0)
					{
						symbols[sym_index].params = (void*) malloc(sizeof(struct Parameter));
						if (symbols[sym_index].params == NULL)
						{
							printf("malloc() failed. Out of memory?\n");
							getc(stdin);
							exit(EXIT_FAILURE);
						}
						symbols[sym_index].num_params = 1;
					}
					break;

				case BIN_STATE_SYMBOL_PARAMETER_NUM_LOW_WORD:
					dword = word & 0xFFFF;
					counter++;
					STATE++;
					break;

				case BIN_STATE_SYMBOL_PARAMETER_NUM_HIGH_WORD:
					dword |= word << 16;
					symbols[sym_index].num_params = dword;

					if (DEBUG)
						printf("symbol num parameters: %d\n", dword);

					symbols[sym_index].params = malloc(sizeof(struct Parameter) * dword);
					if (symbols[sym_index].params == NULL)
					{
						printf("malloc() failed. Out of memory?\n");
						getc(stdin);
						exit(EXIT_FAILURE);
					}
					counter++;

					if (counter == limit)
					{
						STATE = BIN_STATE_SYMBOL_HEADER_LOW_WORD;
					}
					else if (counter == limit_ins)
					{
						sig_index = 0;
						STATE = BIN_STATE_SYMBOL_SIG_INPUT_VALUE_LOW_WORD;
					}
					else if (counter == limit_outs)
					{
						sig_index = 0;
						STATE = BIN_STATE_SYMBOL_SIG_OUTPUT_VALUE_LOW_WORD;
					}
					else if (counter < limit_outs)
					{
						if (sig_index < symbols[sym_index].num_params)
							STATE = BIN_STATE_SYMBOL_PARAMETER_LENGTH;
						else
							STATE = BIN_STATE_SYMBOL_PARAMETER_SKIP;
					}
					else
					{
						printf("Error: Parsing parameters and signals failed.\n");
						getc(stdin);
						exit(EXIT_FAILURE); 
					}
					break;
				case BIN_STATE_SYMBOL_PARAMETER_USHORT_LOW_WORD:
					dword = word & 0xFFFF;
					counter++;
					STATE++;
					break;
				case BIN_STATE_SYMBOL_PARAMETER_USHORT_HIGH_WORD:
					temp_ptr = malloc(sizeof(unsigned int));
					if (temp_ptr == NULL)
					{
						printf("malloc() failed. Out of memory?\n");
						getc(stdin);
						exit(EXIT_FAILURE);
					}
					
					dword |= word << 16;
					*temp_ptr = dword;
					symbols[sym_index].params[sig_index].value = (void*)temp_ptr;
					symbols[sym_index].params[sig_index].type = PARAM_TYPE_USHORT;
					symbols[sym_index].params[sig_index].len = 2;
					counter++;
					sig_index++;

					if (counter == limit)
					{
						STATE = BIN_STATE_SYMBOL_HEADER_LOW_WORD;
					}
					else if (counter == limit_ins)
					{
						sig_index = 0;
						STATE = BIN_STATE_SYMBOL_SIG_INPUT_VALUE_LOW_WORD;
					}
					else if (counter == limit_outs)
					{
						sig_index = 0;
						STATE = BIN_STATE_SYMBOL_SIG_OUTPUT_VALUE_LOW_WORD;
					}
					else if (counter < limit_outs)
					{
						if (sig_index < symbols[sym_index].num_params)
							STATE = BIN_STATE_SYMBOL_PARAMETER_LENGTH;
						else
							STATE = BIN_STATE_SYMBOL_PARAMETER_SKIP;
					}
					else
					{
						printf("Error: Parsing parameters and signals failed.\n");
						getc(stdin);
						exit(EXIT_FAILURE); 
					}
					break;

				case BIN_STATE_SYMBOL_PARAMETER_EXPAND_LOW_WORD:
					dword = word & 0xFFFF;
					counter++;
					STATE++;
					break;

				case BIN_STATE_SYMBOL_PARAMETER_EXPAND_HIGH_WORD:
					dword |= word << 16;
					symbols[sym_index].expand = dword;
					counter++;

					if (counter == limit)
					{
						STATE = BIN_STATE_SYMBOL_HEADER_LOW_WORD;
					}
					else if (counter == limit_ins)
					{
						sig_index = 0;
						STATE = BIN_STATE_SYMBOL_SIG_INPUT_VALUE_LOW_WORD;
					}
					else if (counter == limit_outs)
					{
						sig_index = 0;
						STATE = BIN_STATE_SYMBOL_SIG_OUTPUT_VALUE_LOW_WORD;
					}
					else if (counter < limit_outs)
					{
						if (sig_index < symbols[sym_index].num_params)
							STATE = BIN_STATE_SYMBOL_PARAMETER_LENGTH;
						else
							STATE = BIN_STATE_SYMBOL_PARAMETER_SKIP;
					}
					else
					{
						printf("Error: Parsing parameters and signals failed.\n");
						getc(stdin);
						exit(EXIT_FAILURE); 
					}
					break;

				case BIN_STATE_SYMBOL_PARAMETER_STRING:
					temp_str[str_index] = (char) word;
					str_index++;
					counter++;

					if (str_index >= str_len)
					{
						symbols[sym_index].params[sig_index].value = (void*) temp_str;
						temp_str = NULL;
						sig_index++;

						if (counter == limit)
						{
							STATE = BIN_STATE_SYMBOL_HEADER_LOW_WORD;
						}
						else if (counter == limit_ins)
						{
							sig_index = 0;
							STATE = BIN_STATE_SYMBOL_SIG_INPUT_VALUE_LOW_WORD;
						}
						else if (counter == limit_outs)
						{
							sig_index = 0;
							STATE = BIN_STATE_SYMBOL_SIG_OUTPUT_VALUE_LOW_WORD;
						}
						else if (counter < limit_outs)
						{
							if (sig_index < symbols[sym_index].num_params)
								STATE = BIN_STATE_SYMBOL_PARAMETER_LENGTH;
							else
								STATE = BIN_STATE_SYMBOL_PARAMETER_SKIP;
						}
						else
						{
							printf("Error: Parsing parameters and signals failed.\n");
							getc(stdin);
							exit(EXIT_FAILURE); 
						}
					}
					break;

				case BIN_STATE_SYMBOL_PARAMETER_SKIP:
					counter++;
					if (counter == limit)
					{
						STATE = BIN_STATE_SYMBOL_HEADER_LOW_WORD;
					}
					else if (counter == limit_ins)
					{
						sig_index = 0;
						STATE = BIN_STATE_SYMBOL_SIG_INPUT_VALUE_LOW_WORD;
					}
					else if (counter == limit_outs)
					{
						sig_index = 0;
						STATE = BIN_STATE_SYMBOL_SIG_OUTPUT_VALUE_LOW_WORD;
					}
					else if (counter < limit_outs)
					{
						if (sig_index < symbols[sym_index].num_params)
							STATE = BIN_STATE_SYMBOL_PARAMETER_LENGTH;
						else
							STATE = BIN_STATE_SYMBOL_PARAMETER_SKIP;
					}
					else
					{
						printf("Error: Parsing parameters and signals failed.\n");
						getc(stdin);
						exit(EXIT_FAILURE); 
					}
					break;

				case BIN_STATE_SYMBOL_SIG_OUTPUT_VALUE_LOW_WORD:
					dword = word & 0xFFFF;
					counter++;
					STATE++;
					break;

				case BIN_STATE_SYMBOL_SIG_OUTPUT_VALUE_HIGH_WORD:
					dword |= word << 16;
					symbols[sym_index].sig_outputs[sig_index] = dword;
					sig_index++;
					counter++;
					
					if (sig_index >= symbols[sym_index].num_outs)
					{
						if (counter == limit_ins && symbols[sym_index].num_ins != 0)
						{
							sig_index = 0;
							STATE = BIN_STATE_SYMBOL_SIG_INPUT_VALUE_LOW_WORD;
						}
						else
						{
							sig_index = 0;
							STATE = BIN_STATE_SYMBOL_HEADER_LOW_WORD;
						}
					}
					else
					{
						STATE = BIN_STATE_SYMBOL_SIG_OUTPUT_VALUE_LOW_WORD;
					}
					break;				
				case BIN_STATE_SYMBOL_SIG_INPUT_VALUE_LOW_WORD:
					dword = word & 0xFFFF;
					counter++;
					STATE++;
					break;
				case BIN_STATE_SYMBOL_SIG_INPUT_VALUE_HIGH_WORD:
					dword |= word << 16;
					symbols[sym_index].sig_inputs[sig_index] = dword;
					sig_index++;
					counter++;
					
					if (sig_index >= symbols[sym_index].num_ins)
					{
						if (counter != limit)
						{
							printf("Error: Unexpected data.\n");
		    				getc(stdin);
		    				exit(EXIT_FAILURE); 
						}
						sig_index = 0;
						STATE = BIN_STATE_SYMBOL_HEADER_LOW_WORD;
					}
					else
					{
						STATE = BIN_STATE_SYMBOL_SIG_INPUT_VALUE_LOW_WORD;
					}
					break;
				default:	//BIN_STATE_DEVICE_DEFINITION
					counter++;
					if (counter >= limit)
					{
						STATE = BIN_STATE_SYMBOL_HEADER_LOW_WORD;
					}
					break;
	    	} //switch

	    } //for

	} while (num_bytes_read >= BUF_SIZE);

	CloseHandle(bin_file);

	if (file_header != NULL)
		*file_header = header;

	if (num_symbols != NULL)
		*num_symbols = sym_index + 1;

	return symbols;
}