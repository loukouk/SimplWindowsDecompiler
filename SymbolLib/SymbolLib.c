#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SymbolLib.h"


void _AddSmwSymbol(struct SmwSymbolDefinition *syms, int id, char *nm, int smc, int min_in, int min_out, int min_param)
{
	if (id < 1)
		return;

	if (syms[id].SmC != 0)
	{
		printf("Warning: Symbol ID %d has an already assigned SimplWindows definition.\n", id);
		printf("\t\tAssigned symbol name: %s\n", syms[id].name);
		printf("\t\tNew definition symbol name: %s\n", nm);
		printf("Ignoring new definition and continuing...\n");
		return;
	}

	syms[id].SmC = smc;
	syms[id].min_inputs = min_in;
	syms[id].min_outputs = min_out;
	syms[id].min_params = min_param;
	strcpy(syms[id].name, nm);
}

struct SmwSymbolDefinition *InitLibArray(int size)
{
	struct SmwSymbolDefinition *ret = malloc(sizeof(struct SmwSymbolDefinition) * size);
	if (ret == NULL)
	{
		printf("Error: malloc() failed. Out of memory?\r\n");
		getc(stdin);
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < size; i++)
	{

		ret[i].SmC = 0;
		ret[i].min_inputs = 0;
		ret[i].min_outputs = 0;
		memset(ret[i].name, '\0', STR_SIZE);
	}

	return ret;
}

void DeallocateLibArray(struct SmwSymbolDefinition *arr, int *arr_size)
{
	free(arr);
	*arr_size = 0;
}

struct SmwSymbolDefinition *ReadSmwSymbolLib(int *lib_size)
{
	struct SmwSymbolDefinition *syms;
	int size = 300;

	syms = InitLibArray(size);

	_AddSmwSymbol(syms, 84, "Analog 2's Offset Convertor", 79, 1, 1, 0);
	_AddSmwSymbol(syms, 67, "Analog Buffer", 46, 2, 1, 0);
	_AddSmwSymbol(syms, 136, "Analog DivMod", 114, 1, 2, 1);
	_AddSmwSymbol(syms, 51, "Analog Equate", 38, 2, 1, 1);
	_AddSmwSymbol(syms, 87, "Analog Flip", 58, 1, 1, 0);
	_AddSmwSymbol(syms, 230, "Analog Increment", 543, 3, 1, 6);
	_AddSmwSymbol(syms, 236, "Analog Increment with Optional Feedback", 605, 4, 1, 6);
	_AddSmwSymbol(syms, 38, "Analog Initialize", 32, 1, 1, 1);
	_AddSmwSymbol(syms, 78, "Analog Integral", 185, 1, 1, 1);
	_AddSmwSymbol(syms, 237, "Analog Min/Max Clamp", 606, 3, 1, 0);
	_AddSmwSymbol(syms, 219, "Analog Min/Max Scaler", 494, 3, 1, 0);
	_AddSmwSymbol(syms, 44, "Analog Non-Volatile Ramp", 31, 3, 1, 1);
	_AddSmwSymbol(syms, 21, "Analog Preset", 23, 1, 1, 2);
	_AddSmwSymbol(syms, 20, "Analog Ramp", 22, 3, 1, 1);
	_AddSmwSymbol(syms, 223, "Analog Ramp (Bounds Limited)", 540, 3, 1, 4);
	_AddSmwSymbol(syms, 80, "Analog Rate Limiter", 66, 1, 1, 1);
	_AddSmwSymbol(syms, 30, "Analog Scaler", 24, 1, 1, 3);
	_AddSmwSymbol(syms, 99, "Analog Scaler w/o Zero Pass", 65, 1, 1, 3);
	_AddSmwSymbol(syms, 225, "Analog Scaler w/Overflow Handling", 538, 1, 1, 3);
	_AddSmwSymbol(syms, 226, "Analog Scaler w/Overflow Handling w/o Zero Pass", 539, 1, 1, 3);
	_AddSmwSymbol(syms, 224, "Analog Scaler with I/O Limits", 541, 1, 1, 5);
	_AddSmwSymbol(syms, 68, "Analog Scaling Buffer", 47, 2, 1, 0);
	_AddSmwSymbol(syms, 98, "Analog Scaling Buffer about 50%", 64, 2, 1, 0);
	_AddSmwSymbol(syms, 37, "Analog Step", 45, 2, 1, 1);
	_AddSmwSymbol(syms, 100, "Analog Sum", 57, 1, 1, 0);
	_AddSmwSymbol(syms, 106, "Analog To Digital", 84, 1, 1, 0);
	_AddSmwSymbol(syms, 131, "Analog to Floating Point", 113, 3, 4, 0);		//TODO: 2 sets of inputs!!! n1I=2 and n2I=1
	_AddSmwSymbol(syms, 86, "Analog Value Sample", 122, 3, 1, 0);			//TODO: 2 sets of inputs!!! n1I=2 and n2I=1
	_AddSmwSymbol(syms, 39, "Analog Variable Preset", 44, 3, 2, 0);			//TODO: 2 sets of inputs!!! n1I=2 and n2I=1
	_AddSmwSymbol(syms, 232, "AntiLog with Limits", 547, 1, 1, 2);
	_AddSmwSymbol(syms, 52, "Decade", 78, 2, 10, 0);
	//_AddSmwSymbol(syms, 100, "Digital Sum", 61, 1, 1); //SAME AS ANALOG SUM
	_AddSmwSymbol(syms, 81, "Digital to Analog", 85, 1, 1, 0);
	_AddSmwSymbol(syms, 55, "Digital To Scaled Analog", 36, 1, 1, 0);
	_AddSmwSymbol(syms, 239, "Double-Precision Analog Initialize", 635, 1, 2, 1);			//TODO: 2 sets of inputs!!!
	_AddSmwSymbol(syms, 240, "Double-Precision Analog Variable Preset", 636, 4, 2, 0);		//TODO: 2 sets of inputs!!! n1I=3 and n2I=1
	_AddSmwSymbol(syms, 130, "Floating Point to Analog", 116, 4, 2, 0);
	_AddSmwSymbol(syms, 231, "Log with Limits", 546, 1, 1, 2);
	_AddSmwSymbol(syms, 221, "Multiple Analog Preset", 493, 1, 1, 2);
	_AddSmwSymbol(syms, 41, "Numeric Keypad", 39, 14, 2, 2);
	_AddSmwSymbol(syms, 233, "Ramp In Progress", 566, 1, 1, 0);

	_AddSmwSymbol(syms, 45, "Analog Compare", 35, 3, 2, 0);
	_AddSmwSymbol(syms, 220, "Analog Comparison (Full Set)", 492, 2 ,6, 0);
	_AddSmwSymbol(syms, 0, "AND", 1, 1, 1, 0);
	_AddSmwSymbol(syms, 76, "Binary Decoder", 49, 2, 1, 0);
	_AddSmwSymbol(syms, 18, "Buffer", 20, 2, 1, 0);
	_AddSmwSymbol(syms, 5, "Exclusive NOR", 112, 1, 1, 0);
	_AddSmwSymbol(syms, 4, "Exclusive OR", 5, 1, 1, 0);
	_AddSmwSymbol(syms, 206, "Multiple NOT", 418, 1, 1, 0);
	_AddSmwSymbol(syms, 1, "NAND", 2, 1, 1, 0);
	_AddSmwSymbol(syms, 7, "Negative Transition Gate", 7, 1, 1, 0);
	_AddSmwSymbol(syms, 3, "NOR", 4, 1, 1, 0);
	//_AddSmwSymbol(syms, 1, "NOT", 205, 1, 1, 0);		//same as NAND
	_AddSmwSymbol(syms, 2, "OR", 3, 1, 1, 0);
	_AddSmwSymbol(syms, 6, "Transition Gate", 6, 1, 1, 0);
	_AddSmwSymbol(syms, 19, "Truth Table", 28, 1, 1, 2);

	_AddSmwSymbol(syms, 12, "Binary Counter", 18, 3, 1, 0);
	//_AddSmwSymbol(syms, 23, "Ring Counter", 17, 3, 1, 0);
	_AddSmwSymbol(syms, 23, "Ring Counter with Seed", 451, 4, 1, 0);

	_AddSmwSymbol(syms, 104, "Analog Debugger", 69, 1, 0, 0);
	//_AddSmwSymbol(syms, -1, "Analog Force", 472);
	//_AddSmwSymbol(syms, -1, "Digital Force", 471);
	_AddSmwSymbol(syms, 34, "Message to Computer Port", 94, 1, 0, 1);
	_AddSmwSymbol(syms, 59, "Serial Binary To Hex", 59, 1, 0, 0);
	_AddSmwSymbol(syms, 54, "Serial Debugger (ASCII)", 68, 1, 0, 0);
	_AddSmwSymbol(syms, 115, "Serial Debugger (Hex)", 191, 1, 0, 0);
	//_AddSmwSymbol(syms, -1, "Serial Force", 470);
	//_AddSmwSymbol(syms, -1, "Signal Debugger with Label", 2848);
	_AddSmwSymbol(syms, 234, "User Event Logger", 577, 3, 0, 0);

	_AddSmwSymbol(syms, 149, "ASCII to KB Scan Code", 187, 1, 1, 0);
	_AddSmwSymbol(syms, 209, "Mouse Simulator", 437, 12, 1, 0);
	_AddSmwSymbol(syms, 83, "Pesa Switch Command", 81, 6, 1, 0);
	_AddSmwSymbol(syms, 137, "Unity Reponse Parser", 119, 1, 1, 1);
	//_AddSmwSymbol(syms, -1, "Virtual IR Device for Macro Connection", 284);
	//_AddSmwSymbol(syms, -1, "Virtual Serial Driver for Macro Connection", 285);
	_AddSmwSymbol(syms, 132, "Westec Security Packet Filter", 189, 1, 1, 0);

	//_AddSmwSymbol(syms, 85, "Emailbox", 351);
	//_AddSmwSymbol(syms, 85, "EmailViewer-10 msg inbox", 349);
	//_AddSmwSymbol(syms, 85, "EmailViewer-12 msg inbox", 350, 37, 69);
	//_AddSmwSymbol(syms, 85, "EmailViewer-4 msg inbox", 347);
	//_AddSmwSymbol(syms, 85, "EmailViewer-8 msg inbox", 348);

	_AddSmwSymbol(syms, 36, "Analog RAM", 34, 4, 1, 0);			//TODO: 2 sets of inputs!!! n1I=3 and n2I=5
	//_AddSmwSymbol(syms, 36, "Analog RAM From Database", 221);
	_AddSmwSymbol(syms, 8, "D Flip Flop", 14, 4, 2, 0);
	//_AddSmwSymbol(syms, 36, "Digital RAM", 90);
	_AddSmwSymbol(syms, 128, "FIFO Queue", 120, 4, 3, 0);			//TODO: 2 sets of inputs!!!
	_AddSmwSymbol(syms, 11, "Interlock", 19, 3, 1, 0);
	_AddSmwSymbol(syms, 207, "Interlock-Toggle", 419, 3, 1, 0);
	_AddSmwSymbol(syms, 13, "JK Flip Flop", 15, 5, 2, 0);
	_AddSmwSymbol(syms, 32, "Memory Interlock", 25, 3, 1, 0);		//TODO: 2 sets of inputs!!! n1I=2 and n2I=1
	_AddSmwSymbol(syms, 144, "Serial Memory Search", 186, 4, 1, 1);
	_AddSmwSymbol(syms, 113, "Serial Queue", 71, 3, 1, 1);
	_AddSmwSymbol(syms, 66, "Serial RAM", 53, 5, 1, 1);
	//_AddSmwSymbol(syms, 66, "Serial RAM from database", 82);
	_AddSmwSymbol(syms, 9, "Set/Reset Latch", 12, 2, 2, 0);
	_AddSmwSymbol(syms, 10, "Toggle", 13, 3, 2, 0);

	_AddSmwSymbol(syms, 31, "Comment", 121, 0, 0, 1);
	//_AddSmwSymbol(syms, -1, "SUBSYSTEM", 156);

	_AddSmwSymbol(syms, 138, "Button Presser", 99, 1, 1, 0);
	_AddSmwSymbol(syms, 139, "Stepper", 98, 1, 2, 2);
	_AddSmwSymbol(syms, 241, "Stepper with Progress & Reset", 777, 2, 5, 2);

	_AddSmwSymbol(syms, 82, "Analog to Serial", 77, 2, 1, 2);
	_AddSmwSymbol(syms, 109, "ASCII Keypad", 123, 16, 1, 1);
	_AddSmwSymbol(syms, 65, "ASCII Serial Decoder", 54, 2, 14, 1);
	//_AddSmwSymbol(syms, -1, "Convert To ASCII", 3054);
	//_AddSmwSymbol(syms, -1, "Convert To UTF-16", 3056);
	_AddSmwSymbol(syms, 135, "Duple Decoder", 127, 1, 1, 0);
	_AddSmwSymbol(syms, 134, "Duple Encoder", 115, 1, 1, 0);
	_AddSmwSymbol(syms, 205, "Make String Permanent", 858, 1, 0, 1);
	//_AddSmwSymbol(syms, 205, "Make String Permanent v1 (CUZ 3.117 and below)", 417);
	//_AddSmwSymbol(syms, -1, "Mark As ASCII/UTF-16", 3053);
	//_AddSmwSymbol(syms, -1, "Multiple Discrete Serial Send", 3106);
	_AddSmwSymbol(syms, 58, "Multiple Serial Send", 1477, 1, 1, 1);
	_AddSmwSymbol(syms, 247, "Send As Raw Data", 1076, 32, 0, 0);
	_AddSmwSymbol(syms, 88, "Serial Buffer", 56, 2, 1, 0);
	_AddSmwSymbol(syms, 122, "Serial Concatenation", 126, 1, 1, 0);
	_AddSmwSymbol(syms, 107, "Serial Demultiplexor", 86, 2, 1, 1);
	_AddSmwSymbol(syms, 112, "Serial Demultiplexor (Special)", 124, 1, 1, 1);
	_AddSmwSymbol(syms, 93, "Serial Gather", 62, 1, 1, 2);
	_AddSmwSymbol(syms, 97, "Serial I/O", 101, 3, 2, 3);
	_AddSmwSymbol(syms, 65, "Serial Memory Dialer", 236, 2, 13, 1);
	_AddSmwSymbol(syms, 111, "Serial Multiplexor (Special)", 125, 1, 1, 1);
	_AddSmwSymbol(syms, 117, "Serial Pacer", 73, 1, 1, 2);
	//_AddSmwSymbol(syms, 58, "Serial Send", 89, 1, 1);
	//_AddSmwSymbol(syms, 94, "Serial Substring", 63);
	_AddSmwSymbol(syms, 94, "Serial Substring (Expandable)", 1317, 1, 1, 2);
	_AddSmwSymbol(syms, 252, "Serial Substring w/Empty string pass", 1316, 1, 1, 2);
	_AddSmwSymbol(syms, 103, "Serial to Analog", 96, 1, 1, 1);
	_AddSmwSymbol(syms, 114, "Serial/Analog One-Shot", 72, 1, 2, 1);
	_AddSmwSymbol(syms, 254, "String Length", 2506, 1, 1, 0);
	//_AddSmwSymbol(syms, 109, "Telephone Dialing Keypad", 212, 15, 1);
	_AddSmwSymbol(syms, 208, "Text Append", 462, 3, 1, 0);

	_AddSmwSymbol(syms, 214, "Control Crosspoint Routing.", 447, 129, 85, 1);		//TODO: 3 sets of inputs!!! n1I=43 and n2I=42
	_AddSmwSymbol(syms, 213, "Equipment Crosspoint Routing.", 446, 129, 85, 1);		//TODO: 3 sets of inputs!!! n1I=43 and n2I=42
	_AddSmwSymbol(syms, 215, "Equipment/Control Crosspoint Connect.", 448, 7, 0, 0);

	_AddSmwSymbol(syms, 202, "Console", 412, 1, 1, 0);
	_AddSmwSymbol(syms, 50, "Hard Reset", 41, 1, 0, 0);
	//_AddSmwSymbol(syms, 85, "Intersystem Communications", 327);
	_AddSmwSymbol(syms, 85, "Intersystem Communications w/Offset", 102, 3, 3, 1);			//TODO: 2 sets of inputs!!! n1I=2 and n2I=1
	_AddSmwSymbol(syms, 123, "Intersystem Communications w/Status Req", 192, 3, 3, 0);		//TODO: 2 sets of inputs!!! n1I=2 and n2I=1
	_AddSmwSymbol(syms, 49, "Soft Reset", 40, 1, 0, 0);
	_AddSmwSymbol(syms, 235, "User Program Commands", 581, 0, 1, 0);

	//_AddSmwSymbol(syms, 210, "Astronomical Clock", 438);
	_AddSmwSymbol(syms, 210, "Astronomical Clock with Enable", 859, 9, 6, 2);
	//_AddSmwSymbol(syms, 62, "Clock Driver", 43, 0, 1);
	//_AddSmwSymbol(syms, 62, "Clock Driver w/out DST", 3078);
	_AddSmwSymbol(syms, 62, "Extended Clock Driver", 469, 0, 2, 1);
	_AddSmwSymbol(syms, 60, "Past", 70, 1, 1, 1);
	_AddSmwSymbol(syms, 101, "Serialize Date", 60, 2, 1, 1);
	_AddSmwSymbol(syms, 121, "Set System Clock", 154, 4, 0, 0);
	//_AddSmwSymbol(syms, 211, "Time Offset", 443);
	_AddSmwSymbol(syms, 211, "Time Offset with Enable", 860, 4, 1, 1);
	_AddSmwSymbol(syms, 61, "When", 74, 1, 1, 2);

	_AddSmwSymbol(syms, 56, "Debounce", 50, 1, 1, 1);
	//_AddSmwSymbol(syms, 27, "Delay", 16, 2, 1);
	_AddSmwSymbol(syms, 203, "Logic Wave Delay", 408, 1, 1, 1);
	_AddSmwSymbol(syms, 204, "Logic Wave Pulse", 409, 3, 2, 1);
	_AddSmwSymbol(syms, 27, "Long Delay", 545, 2, 1, 1);
	//_AddSmwSymbol(syms, -1, "Multiple Logic Wave Pulses", 3104);
	_AddSmwSymbol(syms, 29, "Multiple One Shots", 21, 2, 1, 1);
	_AddSmwSymbol(syms, 15, "One Shot", 9, 3, 2, 1);
	_AddSmwSymbol(syms, 14, "Oscillator", 8, 1, 1, 2);
	_AddSmwSymbol(syms, 16, "Pulse Stretcher", 11, 1, 2, 1);
	_AddSmwSymbol(syms, 17, "Retriggerable One Shot", 10, 3, 2, 1);
	_AddSmwSymbol(syms, 204, "Serial/Analog Logic Wave Pulse", 1309, 2, 2, 1);
	_AddSmwSymbol(syms, 200, "Variable Delay", 411, 3, 1, 0);
	_AddSmwSymbol(syms, 201, "Variable Oscillator", 410, 3, 1, 0);

	_AddSmwSymbol(syms, 47, "Analog to Indirect Text", 37, 2, 0, 4);
	_AddSmwSymbol(syms, 159, "CIP Transmission", 353, 1, 0, 1);
	_AddSmwSymbol(syms, 212, "Indirect Text Broadcast", 208, 34, 0, 0);
	_AddSmwSymbol(syms, 35, "Network Transmission", 266, 1, 0, 1);
	_AddSmwSymbol(syms, 64, "Serial to Buffered Indirect Text", 190, 2, 0, 4);
	//_AddSmwSymbol(syms, 153, "Serial to Ethernet Indirect Text", 279);
	//_AddSmwSymbol(syms, 64, "Serial to Indirect Text", 52, 2, 0);
	//_AddSmwSymbol(syms, 64, "Serial to Indirect Text (Alpha Format Only)", 334);

	*lib_size = size;
	return syms;
}