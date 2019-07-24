#ifndef __CRESTRON_SMW_BUILDER_C__
#define __CRESTRON_SMW_BUILDER_C__

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

#define DEBUG 0

void SmwBuilder(char *filepath)
{
	HANDLE smw_file;
	char iobuffer[BUF_SIZE];
	DWORD num_bytes_read;
	unsigned char c;
	unsigned long total_bytes_read = 0;

	smw_file = CreateFile(filepath,							// file to open
                      	  GENERIC_READ | GENERIC_WRITE,		// open for reading and writing
                      	  0,       							// no sharing
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
	    } //for

	} while (num_bytes_read >= BUF_SIZE);

	CloseHandle(smw_file);
}

#endif