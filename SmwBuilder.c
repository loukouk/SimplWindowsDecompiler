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
						"[\nObjTp=Sm\nNm=%s\nH=%d\nSmC=%d\nObjVer=1\nSmVr=1084\nPrH=4\nn1I=%d\nn1O=%d\nmI=%d\n",
						lib[sym.id].name, idx, lib[sym.id].SmC, sym.num_ins, sym.num_outs, sym.num_ins);

	if (buf_bytes > size)
		return -1;

	for (int j = 1; j <= sym.num_ins; j++)
	{
		buf_bytes += snprintf(buf + buf_bytes, size - buf_bytes, "I%d=%d\n", j, 4);

		if (buf_bytes > size)
			return -1;
	}

	buf_bytes += snprintf(buf + buf_bytes, size - buf_bytes, "mO=%d\n", sym.num_outs);
	if (buf_bytes > size)
		return -1;

	for (int j = 1; j <= sym.num_outs; j++)
	{
		buf_bytes += snprintf(buf + buf_bytes, size - buf_bytes, "O%d=%d\n", j, 4);

		if (buf_bytes > size)
			return -1;
	}

	buf_bytes += snprintf(buf + buf_bytes, size - buf_bytes, "mP=%d\n", sym.num_params);
	if (buf_bytes > size)
		return -1;

	for (int j = 1; j <= sym.num_params; j++)
	{
		buf_bytes += snprintf(buf + buf_bytes, size - buf_bytes, "P%d=%s\n", j, "1d");

		if (buf_bytes > size)
			return -1;
	}

	buf_bytes += snprintf(buf + buf_bytes, size - buf_bytes, "]\n");
	if (buf_bytes > size)
		return -1;

	return buf_bytes;
}

void _AddStringToArray(char **str, int idx, char ***arr, int *size)
{
	while (idx >= *size)
	{
		char **temp;
		int temp_size;

		temp_size = *size * 2;
		temp = malloc(sizeof(char*) * temp_size);
		if (temp == NULL)
		{
			printf("Error: malloc() failed. Out of memory?\n");
			getc(stdin);
			exit(EXIT_FAILURE);
		}
		
		for (int i = 0; i < temp_size; i++)
		{
			if (i < *size)
				temp[i] = (*arr)[i];
			else
				temp[i] = NULL;
		}

		free(*arr);
		*arr = temp;
		*size = temp_size;
	}

	(*arr)[idx] = *str;
}

char **ReadSmwFile(char *filepath, int *arr_len)
{
	HANDLE smw_file;
	char read_buffer[BUF_SIZE];
	char *str_buffer, **str_arr;
	int str_buf_size = STR_SIZE, arr_index = 0, str_index = 0, arr_size = 0;
	DWORD num_bytes_read, total_bytes_read;
	unsigned char c;

	smw_file = CreateFile(filepath,					// file to open
                      	  GENERIC_READ,				// open for reading
                      	  FILE_SHARE_READ,			// share for reading
                      	  NULL,						// default security
                     	  OPEN_EXISTING,			// existing file only
                      	  FILE_ATTRIBUTE_NORMAL,	// normal file
                      	  NULL);

    if (smw_file == INVALID_HANDLE_VALUE) 
    { 
        printf(TEXT("Error: unable to open \"%s\" for reading.\nPress the Enter key to close..."), filepath);
        getc(stdin);
        exit(EXIT_FAILURE);
    }

    str_buffer = malloc(sizeof(char) * str_buf_size);
    if (str_buffer == NULL)
    {
    	printf("Error: malloc() failed. Out of memory?\n");
    	getc(stdin);
    	exit(EXIT_FAILURE);
    }
    arr_size = SYM_SIZE_DEFAULT;
    str_arr = malloc(sizeof(char*) * arr_size);
    if (str_arr == NULL)
    {
    	printf("Error: malloc() failed. Out of memory?\n");
    	getc(stdin);
    	exit(EXIT_FAILURE);
    }
    memset(str_buffer, '\0', str_buf_size);
    memset(str_arr, '\0', arr_size * sizeof(char *));

    do
    {
		memset(read_buffer, '\0', BUF_SIZE);
		if (FALSE == ReadFile(smw_file, read_buffer, (DWORD)BUF_SIZE, &num_bytes_read, NULL))
		{
			printf("Error: Could not read from \"%s\".\nPress the Enter key to close...", filepath);
			getc(stdin);
			exit(EXIT_FAILURE);
		}
		total_bytes_read += num_bytes_read;

		for (int read_index = 0; read_index < num_bytes_read; read_index++)
		{
			c = read_buffer[read_index];

			str_buffer[str_index] = c;
			str_index++;
			if (c == ']')
			{
				_AddStringToArray(&str_buffer, arr_index, &str_arr, &arr_size);
				arr_index++;

				str_buf_size = STR_SIZE;
				str_buffer = malloc(sizeof(char) * str_buf_size);
				if (str_buffer == NULL)
				{
					printf("Error: malloc() failed. Out of memory?\n");
					getc(stdin);
					exit(EXIT_FAILURE);
				}
				memset(str_buffer, '\0', str_buf_size);
				str_index = 0;
			}
			else if (str_index >= str_buf_size-1)
			{
				ResizeString(&str_buffer, &str_buf_size);
			}
	    }

    } while(num_bytes_read >= BUF_SIZE);

    CloseHandle(smw_file);

    free(str_buffer);

    *arr_len = arr_index;
    return str_arr;
}

void ParseSmwArr(char **str_arr, int arr_len, int *H_start, int *PrH_start, int *logic_idx)
{
	int h = 0, prh = 0, logic = 0, obj_ver = 0;

	for (int i = 0; i < arr_len; i++)
	{
		if (sscanf(str_arr[i], "\n[\nObjTp=Sm\nH=%d\nSmC=156\nNm=Logic\nObjVer=%d", &h, &obj_ver) > 1)
		{
			logic = i;
			prh = h;
		}
	}

	if (!logic || !h || !prh || !obj_ver)
	{
		printf("Error: Could not find where to write data. Is this file a properly formatted SMW file?\n");
		getc(stdin);
		exit(EXIT_FAILURE);
	}

	*H_start = h+1;
	*PrH_start = prh;
	*logic_idx = logic;
}

void SmwBuilder(char *filepath, struct BinSymbol *syms, int sym_size, struct SignalInfo *sigs, int sig_size, struct SmwSymbolDefinition *lib, int lib_size)
{
	HANDLE smw_file;
	char *write_buffer;
	DWORD num_bytes_written, total_bytes_written = 0;
	int write_buffer_size, write_idx, write_buffer_bytes;
	int loop_counter = 0;
	char **swm_str_arr;
	int smw_arr_len;
	int H_start, PrH_start, logic_idx;


    swm_str_arr = ReadSmwFile(filepath, &smw_arr_len);
    ParseSmwArr(swm_str_arr, smw_arr_len, &H_start, &PrH_start, &logic_idx);

    write_idx = H_start;

	smw_file = CreateFile("decompiled.smw",							// file to open
                      	  GENERIC_WRITE,					// create new file for writing
                      	  0,       							// no sharing
                      	  NULL,								// default security
                     	  CREATE_ALWAYS,					// open existing file
                      	  FILE_ATTRIBUTE_NORMAL,			// normal file
                      	  NULL);

    if (smw_file == INVALID_HANDLE_VALUE) 
    { 
        printf(TEXT("Error: unable to open \"%s\" for writing.\nPress the Enter key to close..."), filepath);
        getc(stdin);
        exit(EXIT_FAILURE);
    }

	write_buffer_size = BUF_SIZE;
    write_buffer = malloc(sizeof(char) * write_buffer_size);
    if (write_buffer == NULL)
    {
    	printf("Error: malloc() failed. Out of memory?\n");
    	getc(stdin);
    	exit(EXIT_FAILURE);
    }

    for (int i = 0; i < smw_arr_len; i++)
    {
		if (FALSE == WriteFile(smw_file, swm_str_arr[i], (DWORD)strlen(swm_str_arr[i]), &num_bytes_written, NULL))
		{
			printf("Error: Could not write to \"%s\" (0x%x).\nPress the Enter key to close...", filepath, (int)GetLastError());
			getc(stdin);
			exit(EXIT_FAILURE); 
		}
		total_bytes_written += num_bytes_written;
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
			total_bytes_written += num_bytes_written;

	    	write_idx++;
	    	loop_counter = 0;
	    }
    }

    free(write_buffer);

	CloseHandle(smw_file);
}

#endif