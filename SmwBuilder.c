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
						"\r\n[\r\nObjTp=Sm\r\nNm=%s\r\nH=%d\r\nSmC=%d\r\nObjVer=1\r\nSmVr=1084\r\nPrH=4\r\nn1I=%d\r\nn1O=%d\r\nmI=%d\r\n",
						lib[sym.id].name, idx, lib[sym.id].SmC, sym.num_ins, sym.num_outs, sym.num_ins);

	if (buf_bytes > size)
		return -1;

	for (int j = 1; j <= sym.num_ins; j++)
	{
		buf_bytes += snprintf(buf + buf_bytes, size - buf_bytes, "I%d=\r\n", j);

		if (buf_bytes > size)
			return -1;
	}

	buf_bytes += snprintf(buf + buf_bytes, size - buf_bytes, "mO=%d\r\n", sym.num_outs);
	if (buf_bytes > size)
		return -1;

	for (int j = 1; j <= sym.num_outs; j++)
	{
		buf_bytes += snprintf(buf + buf_bytes, size - buf_bytes, "O%d=\r\n", j);

		if (buf_bytes > size)
			return -1;
	}

	buf_bytes += snprintf(buf + buf_bytes, size - buf_bytes, "mP=%d\r\n", sym.num_params);
	if (buf_bytes > size)
		return -1;

	for (int j = 1; j <= sym.num_params; j++)
	{
		buf_bytes += snprintf(buf + buf_bytes, size - buf_bytes, "P%d=\r\n", j);

		if (buf_bytes > size)
			return -1;
	}

	buf_bytes += snprintf(buf + buf_bytes, size - buf_bytes, "]");
	if (buf_bytes > size)
		return -1;

	return buf_bytes;
}

void _AddStringToArray(char *str, int idx, char ***arr, int *size)
{
	while (idx >= *size)
	{
		char **temp;
		int temp_size;

		temp_size = *size * 2;
		temp = malloc(sizeof(char*) * temp_size);
		if (temp == NULL)
		{
			printf("Error: malloc() failed. Out of memory?\r\n");
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

	(*arr)[idx] = str;
}

int WriteLogicSymbol(char *buf, int size, int h, int h_start, int num_sym)
{
	int buf_bytes = 0;

	buf_bytes = snprintf(buf + buf_bytes, size - buf_bytes,
				"\r\n[\r\nObjTp=Sm\r\nH=%d\r\nSmC=156\r\nNm=Logic\r\nObjVer=1\r\nmC=%d\r\n", h, num_sym);

	if (buf_bytes > size)
		return -1;

	for (int j = 1; j <= num_sym; j++)
	{
		buf_bytes += snprintf(buf + buf_bytes, size - buf_bytes, "C%d=%d\r\n", j, h_start + j - 1);

		if (buf_bytes > size)
			return -1;
	}

	buf_bytes += snprintf(buf + buf_bytes, size - buf_bytes, "]");
	if (buf_bytes > size)
		return -1;

	return buf_bytes;
}

char **ReadSmwFile(char *filepath, int *arr_len, int *arr_max_size)
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
        printf(TEXT("Error: unable to open \"%s\" for reading.\r\nPress the Enter key to close..."), filepath);
        getc(stdin);
        exit(EXIT_FAILURE);
    }

    str_buffer = malloc(sizeof(char) * str_buf_size);
    if (str_buffer == NULL)
    {
    	printf("Error: malloc() failed. Out of memory?\r\n");
    	getc(stdin);
    	exit(EXIT_FAILURE);
    }
    arr_size = SYM_SIZE_DEFAULT;
    str_arr = malloc(sizeof(char*) * arr_size);
    if (str_arr == NULL)
    {
    	printf("Error: malloc() failed. Out of memory?\r\n");
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
			printf("Error: Could not read from \"%s\".\r\nPress the Enter key to close...", filepath);
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
				_AddStringToArray(str_buffer, arr_index, &str_arr, &arr_size);
				arr_index++;

				str_buf_size = STR_SIZE;
				str_buffer = malloc(sizeof(char) * str_buf_size);
				if (str_buffer == NULL)
				{
					printf("Error: malloc() failed. Out of memory?\r\n");
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
    *arr_max_size = arr_size;
    return str_arr;
}

void ParseSmwArr(char **str_arr, int arr_len, int *H_start, int *PrH_start, int *logic_idx)
{
	int h = 0, prh = 0, logic = 0, obj_ver = 0;

	for (int i = 0; i < arr_len; i++)
	{
		if (sscanf(str_arr[i], "\r\n[\r\nObjTp=Sm\r\nH=%d\r\nSmC=156\r\nNm=Logic\r\nObjVer=%d", &h, &obj_ver) > 1)
		{
			logic = i;
			prh = h;
		}
	}

	if (!logic || !h || !prh || !obj_ver)
	{
		printf("Error: Could not find where to write data. Is this file a properly formatted SMW file?\r\n");
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
	int smw_arr_len, smw_arr_size;
	int H_start, PrH_start, logic_idx;


    swm_str_arr = ReadSmwFile(filepath, &smw_arr_len, &smw_arr_size);
    ParseSmwArr(swm_str_arr, smw_arr_len, &H_start, &PrH_start, &logic_idx);

    write_idx = H_start;

    for (int i = 0; i < sym_size; i++)
    {
    	if (lib[syms[i].id].name != NULL && lib[syms[i].id].SmC != 0)
    	{
    		write_buffer_size = STR_SIZE;
		    write_buffer = malloc(sizeof(char) * write_buffer_size);
		    if (write_buffer == NULL)
		    {
		    	printf("Error: malloc() failed. Out of memory?\r\n");
		    	getc(stdin);
		    	exit(EXIT_FAILURE);
		    }
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
			    	printf("Error: malloc() failed. Out of memory?\r\n");
			    	getc(stdin);
			    	exit(EXIT_FAILURE);
	    		}
	    		memset(write_buffer, '\0', write_buffer_size);
	    	}
			write_idx++;
	    	loop_counter = 0;

			_AddStringToArray(write_buffer, smw_arr_len, &swm_str_arr, &smw_arr_size);
			smw_arr_len++;
			write_buffer = NULL;
	    }
    }

	write_buffer_size = BUF_SIZE;
	write_buffer = malloc(sizeof(char) * write_buffer_size);
	if (write_buffer == NULL)
	{
		printf("Error: malloc() failed. Out of memory?\r\n");
		getc(stdin);
		exit(EXIT_FAILURE);
	}
	memset(write_buffer, '\0', write_buffer_size);
	while ((write_buffer_bytes = WriteLogicSymbol(write_buffer, write_buffer_size, PrH_start, H_start, write_idx - H_start)) < 0)
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
	    	printf("Error: malloc() failed. Out of memory?\r\n");
	    	getc(stdin);
	    	exit(EXIT_FAILURE);
		}
		memset(write_buffer, '\0', write_buffer_size);
	}
	free(swm_str_arr[logic_idx]);
	swm_str_arr[logic_idx] = write_buffer;
	write_buffer = NULL;



	smw_file = CreateFile("decompiled.smw",					// file to open
                      	  GENERIC_WRITE,					// writing only
                      	  0,       							// no sharing
                      	  NULL,								// default security
                     	  CREATE_ALWAYS,					// always create a new file
                      	  FILE_ATTRIBUTE_NORMAL,			// normal file
                      	  NULL);

    if (smw_file == INVALID_HANDLE_VALUE) 
    { 
        printf(TEXT("Error: unable to open \"%s\" for writing.\r\nPress the Enter key to close..."), filepath);
        getc(stdin);
        exit(EXIT_FAILURE);
    }

	for (int i = 0; i < smw_arr_len; i++)
	{
		if (FALSE == WriteFile(smw_file, swm_str_arr[i], (DWORD)strlen(swm_str_arr[i]), &num_bytes_written, NULL))
		{
			printf("Error: Could not write to \"%s\" (0x%x).\r\nPress the Enter key to close...", filepath, (int)GetLastError());
			getc(stdin);
			exit(EXIT_FAILURE); 
		}
		total_bytes_written += num_bytes_written;
	}

	printf("%d bytes written\r\n", (int)total_bytes_written);

	CloseHandle(smw_file);
}

#endif