#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

#define READ_SIZE 1024
#define WRITE_SIZE (8*READ_SIZE)
#define STR_SIZE 256
#define SIG_SIZE_DEFAULT 16

char iToHexChar(int val)
{
	switch(val)
	{
		case 0:
			return '0';
			break;
		case 1:
			return '1';
			break;
		case 2:
			return '2';
			break;
		case 3:
			return '3';
			break;
		case 4:
			return '4';
			break;
		case 5:
			return '5';
			break;
		case 6:
			return '6';
			break;
		case 7:
			return '7';
			break;
		case 8:
			return '8';
			break;
		case 9:
			return '9';
			break;
		case 10:
			return 'a';
			break;
		case 11:
			return 'b';
			break;
		case 12:
			return 'c';
			break;
		case 13:
			return 'd';
			break;
		case 14:
			return 'e';
			break;
		case 15:
			return 'f';
			break;
		default:
			return '_';
			break;
	}
}

int main(int argc, char **argv)
{
	HANDLE my_file, ret_file;
	char ret_filename[STR_SIZE];
	char read_buf[READ_SIZE], write_buf[WRITE_SIZE];
	DWORD num_bytes_read;
	int write_index;
	unsigned char c[2];
	unsigned long total_bytes_read = 0;

	if (argc != 2)
	{
		printf("No filepath entered. Press the Enter key to close...");
		getc(stdin);
		exit(EXIT_FAILURE);
	}

	my_file = CreateFile(argv[1],            // file to open
                      	  GENERIC_READ,          // open for reading
                      	  FILE_SHARE_READ,       // share for reading
                      	  NULL,                  // default security
                     	  OPEN_EXISTING,         // existing file only
                      	  FILE_ATTRIBUTE_NORMAL, // normal file
                      	  NULL);

	snprintf(ret_filename, STR_SIZE, "%s.16bit", argv[1]);

	ret_file = CreateFile(ret_filename,      // file to open
                  	  GENERIC_WRITE,         // open for writing
                  	  0,      				 // not shared
                  	  NULL,                  // default security
                 	  CREATE_ALWAYS,         // always creat new, overwrite
                  	  FILE_ATTRIBUTE_NORMAL, // normal file
                  	  NULL);

    if (my_file == INVALID_HANDLE_VALUE) 
    { 
        printf(TEXT("Error: unable to open \"%s\" for read.\nPress any key to continue..."), argv[1]);
        getc(stdin);
        exit(EXIT_FAILURE);
    }

	do
	{
    	memset(read_buf, '\0', READ_SIZE);
    	memset(write_buf, '\0', WRITE_SIZE);
    	write_index = 0;

	    if (FALSE == ReadFile(my_file, read_buf, (DWORD)READ_SIZE, &num_bytes_read, NULL))
	    {
	    	printf("Error: Could not read from \"%s\".\nPress the Enter key to close...", argv[1]);
	    	getc(stdin);
        	exit(EXIT_FAILURE); 
	    }
	    total_bytes_read += num_bytes_read;

	    for (int read_index = 0 ; read_index < num_bytes_read; read_index += 2)
	    {

	    	if (read_index + 1 >= num_bytes_read)
	    	{
	    		write_buf[write_index] = read_buf[read_index];
	    		printf("Warning: Lonely last character\n");
	    		break;
	    	}

	    	c[1] = read_buf[read_index];
	    	c[0] = read_buf[read_index + 1];

	    	if (c[0] == 0x00 && ((c[1] >= ' ' && c[1] < 0x7f) || c[1] == '\n'))
	    	{
	    		write_buf[write_index] = c[1];
	    		write_index++;
	    	}
	    	else
	    	{
	    		write_buf[write_index+0] = '<';
	    		write_buf[write_index+1] = '0';
	    		write_buf[write_index+2] = 'x';
	    		write_buf[write_index+3] = iToHexChar((int)c[0] >> 4);
	    		write_buf[write_index+4] = iToHexChar((int)c[0] & 0x0F);
	    		write_buf[write_index+5] = iToHexChar((int)c[1] >> 4);
	    		write_buf[write_index+6] = iToHexChar((int)c[1] & 0x0F);
	    		write_buf[write_index+7] = '>';
	    		write_index += 8;
	    	}


	    } //for

	    if (FALSE == WriteFile(ret_file, write_buf, (DWORD)write_index, NULL, NULL))
	    {
	    	printf("Error: Could not write to \"%s\".\nPress the Enter key to close...", ret_filename);
	    	getc(stdin);
        	exit(EXIT_FAILURE); 
	    }

	} while (num_bytes_read >= READ_SIZE);

	printf("Bytes read: %lu. Done.\n", total_bytes_read);
	getc(stdin);

	return EXIT_SUCCESS;
}