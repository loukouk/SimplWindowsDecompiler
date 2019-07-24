#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "BinReader.c"

#ifndef STR_SIZE
	#define STR_SIZE 256
#endif

#define TEMPLATE_BIN_FILEPATH "./SimplWindowsTemplate.bin"
#define TEMPLATE_SMW_FILEPATH "./SimplWindowsTemplate.smw"

struct SmwSymbolDefinition
{
	char name[STR_SIZE]; 
	int SmC;
};

struct SmwSymbolDefinition *ReadSmwFile(char *filepath, int *num_symbols)
{
	HANDLE smw_file;
	char iobuffer[BUF_SIZE], line_buffer[BUF_SIZE];
	char line_index;
	DWORD num_bytes_read;
	unsigned char c;
	unsigned long total_bytes_read = 0;
	struct SmwSymbolDefinition *syms;
	int read_name;

	smw_file = CreateFile(filepath,							// file to open
                      	  GENERIC_READ,						// open for reading
                      	  FILE_SHARE_READ,       			// share for reading
                      	  NULL,								// default security
                     	  OPEN_EXISTING,					// existing file only
                      	  FILE_ATTRIBUTE_NORMAL,			// normal file
                      	  NULL);

    if (smw_file == INVALID_HANDLE_VALUE) 
    { 
        printf(TEXT("Error: unable to open \"%s\" for read.\nPress any key to continue..."), filepath);
        getc(stdin);
        exit(EXIT_FAILURE);
    }

    syms = malloc(sizeof(struct SmwSymbolDefinition)*SYM_SIZE_DEFAULT);

	do
	{
    	memset(iobuffer, '\0', BUF_SIZE);
	    if (FALSE == ReadFile(smw_file, iobuffer, (DWORD)BUF_SIZE, &num_bytes_read, NULL))
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
	    		printf("c: %c\n",c);


	    	if (c == 0x0d)
	    	{
	    		//ignore carriage returns
	    	}
	    	else if (c != 0x0a)
	    	{
	    		if (line_index >= BUF_SIZE)
	    		{
	    			printf("Error: Buffer size exceeded for a single line. File improperly formatted.");
	    			getc(stdin);
        			exit(EXIT_FAILURE); 
	    		}

	    		line_buffer[line_index] = c;
	    		line_index++;
	    	}
			else
			{
				if(line_buffer[0] = '[')
				{
					//do nothing
				}
				else if (line_buffer[0] = ']')
				{
					read_name = 0;
				}
				else if (!strcmp(line_buffer, "Obj_Tp=Sym"))
			}
	    } //for

	} while (num_bytes_read >= BUF_SIZE);

	CloseHandle(smw_file);

	return 
}


void ReadSmwSymbolLib()
{
	int num_syms = 0, num_defs = 0, max_id = 0;

	struct SymbolInfo *syms = ReadBinFile(TEMPLATE_BIN_FILEPATH, &num_syms, NULL);

	struct SmwSymbolDefinition *defs = ReadSmwFile(TEMPLATE_SMW_FILEPATH, &num_defs);




	for (int i = 0; i < num_syms; i++)
	{
		if (syms[i].id > max_id)
			max_id = syms[i].id;
	}
	//check max id with num_defs




}
