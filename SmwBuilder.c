#ifndef __CRESTRON_SMW_BUILDER_C__
#define __CRESTRON_SMW_BUILDER_C__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>


#include "BinReader/BinReader.h"
#include "SigReader/SigReader.h"
#include "SymbolLib/SymbolLib.h"


#ifndef BUF_SIZE
	#define BUF_SIZE 1024
#endif

#ifndef STR_SIZE
	#define STR_SIZE 256
#endif

#define DEBUG 0


void AppendString(char **str, int *len, int *size, char *app)
{
	int app_len = strlen(app);

	while (*len + app_len >= *size)
	{
		ResizeString(str, size);
	}

	strcpy(&((*str)[*len]), app);
	*len = *len + app_len;

}

int WriteSymbol(char *buf, int size, int idx, struct BinSymbol sym, struct SignalInfo *sigs, int sig_size, struct SmwSymbolDefinition *lib, int lib_size)
{
	int buf_bytes = 0;

	buf_bytes = snprintf(buf + buf_bytes, size - buf_bytes, 
						"[\r\nObjTp=Sm\r\nNm=%s\r\nH=%d\r\nSmC=%d\r\nObjVer=1\r\nSmVr=1084\r\nPrH=4\r\nn1I=%d\r\nn1O=%d\r\nmI=%d\r\n",
						lib[sym.id].name, idx, lib[sym.id].SmC, sym.num_ins, sym.num_outs, sym.num_ins);

	if (buf_bytes > size)
		return -1;

	for (int j = 1; j <= sym.num_ins; j++)
	{
		buf_bytes += snprintf(buf + buf_bytes, size - buf_bytes, "I%d=%d\r\n", j, 4);

		if (buf_bytes > size)
			return -1;
	}

	buf_bytes += snprintf(buf + buf_bytes, size - buf_bytes, "mO=%d\r\n", sym.num_outs);
	if (buf_bytes > size)
		return -1;

	for (int j = 1; j <= sym.num_outs; j++)
	{
		buf_bytes += snprintf(buf + buf_bytes, size - buf_bytes, "O%d=%d\r\n", j, 4);

		if (buf_bytes > size)
			return -1;
	}

	buf_bytes += snprintf(buf + buf_bytes, size - buf_bytes, "mP=%d\r\n", sym.num_params);
	if (buf_bytes > size)
		return -1;

	for (int j = 1; j <= sym.num_params; j++)
	{
		buf_bytes += snprintf(buf + buf_bytes, size - buf_bytes, "P%d=%s\r\n", j, "1d");

		if (buf_bytes > size)
			return -1;
	}

	buf_bytes += snprintf(buf + buf_bytes, size - buf_bytes, "]\r\n");
	if (buf_bytes > size)
		return -1;

	return buf_bytes;
}


void SmwBuilder(char *filepath, struct BinSymbol *syms, int sym_size, struct SignalInfo *sigs, int sig_size, struct SmwSymbolDefinition *lib, int lib_size)
{
	HANDLE smw_file;
	char read_buffer[BUF_SIZE];
	char itoa_buf[BUF_SIZE];
	char *write_buffer;
	DWORD num_bytes_read;
	DWORD num_bytes_written;
	int write_buffer_size, write_idx = 21, write_buffer_bytes;
	unsigned char c;
	unsigned long total_bytes_read = 0;
	int loop_counter = 0;

	smw_file = CreateFile(filepath,							// file to open
                      	  GENERIC_READ | GENERIC_WRITE,		// open for reading and writing
                      	  0,       							// no sharing
                      	  NULL,								// default security
                     	  OPEN_EXISTING,					// existing file only
                      	  FILE_ATTRIBUTE_NORMAL,			// normal file
                      	  NULL);

    if (smw_file == INVALID_HANDLE_VALUE) 
    { 
        printf(TEXT("Error: unable to open \"%s\" for reading and writing.\nPress the Enter key to close..."), filepath);
        getc(stdin);
        exit(EXIT_FAILURE);
    }

    ReadSmwFile()

	write_buffer_size = BUF_SIZE;
    write_buffer = malloc(sizeof(char) * write_buffer_size);
    if (write_buffer == NULL)
    {
    	printf("Error: malloc() failed. Out of memory?\n");
    	getc(stdin);
    	exit(EXIT_FAILURE);
    }

    for (int i = 0; i < sym_size; i++)
    {
    	if (lib[syms[i].id].name != NULL && lib[syms[i].id].SmC != 0)
    	{
			memset(write_buffer, '\0', write_buffer_size);

	    	while ((write_buffer_bytes = WriteSymbol(write_buffer, write_buffer_size, write_idx, syms[i], sigs, sig_size, lib, lib_size)) < 0)
	    	{
	    		if (++loop_counter > 20)
	    		{
	    			printf("Error: failed to resize write buffer. Exiting...");
	    			getc(stdin);
	    			exit(EXIT_FAILURE);
	    		}

	    		free(write_buffer);

	    		write_buffer_size = write_buffer_size * 2;
	    		write_buffer = malloc(sizeof(char) * write_buffer_size);
	    		if (write_buffer == NULL)
	    		{
			    	printf("Error: malloc() failed. Out of memory?\n");
			    	getc(stdin);
			    	exit(EXIT_FAILURE);
	    		}
	    		memset(write_buffer, '\0', write_buffer_size);
	    	}


	    	if (FALSE == WriteFile(smw_file, write_buffer, (DWORD)strlen(write_buffer), &num_bytes_written, NULL))
	    	{
				printf("Error: Could not write to \"%s\" (0x%x).\nPress the Enter key to close...", filepath, (int)GetLastError());
				getc(stdin);
				exit(EXIT_FAILURE); 
	    	}

	    	write_idx++;
	    	loop_counter = 0;
	    }
    }


    free(write_buffer);

	CloseHandle(smw_file);
}

#endif