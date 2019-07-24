#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SymbolLib.h"


void _AddSmwSymbol(struct SmwSymbolDefinition *syms, int id, char *nm, int smc)
{
	if (id < 1)
		return;

	syms[id].SmC = smc;
	strcpy(syms[id].name, nm);
}

void DeallocateLibArray(struct SmwSymbolDefinition *arr, int *arr_size)
{
	free(arr);
	*arr_size = 0;
}

struct SmwSymbolDefinition *ReadSmwSymbolLib(int *lib_size)
{
	struct SmwSymbolDefinition *syms = malloc(sizeof(struct SmwSymbolDefinition)*300);
	*lib_size = 300;

	_AddSmwSymbol(syms, 84, "Analog 2's Offset Convertor", 79);
	_AddSmwSymbol(syms, 67, "Analog Buffer", 46);
	_AddSmwSymbol(syms, 136, "Analog DivMod", 114);
	_AddSmwSymbol(syms, 51, "Analog Equate", 38);
	_AddSmwSymbol(syms, 87, "Analog Flip", 58);
	_AddSmwSymbol(syms, 230, "Analog Increment", 543);
	_AddSmwSymbol(syms, 236, "Analog Increment with Optional Feedback", 605);
	_AddSmwSymbol(syms, 38, "Analog Initialize", 32);
	_AddSmwSymbol(syms, 78, "Analog Integral", 185);
	_AddSmwSymbol(syms, 237, "Analog Min/Max Clamp", 606);
	_AddSmwSymbol(syms, 219, "Analog Min/Max Scaler", 494);
	_AddSmwSymbol(syms, 44, "Analog Non-Volatile Ramp", 31);
	_AddSmwSymbol(syms, 21, "Analog Preset", 23);
	_AddSmwSymbol(syms, 20, "Analog Ramp", 22);
	_AddSmwSymbol(syms, 223, "Analog Ramp (Bounds Limited)", 540);
	_AddSmwSymbol(syms, 80, "Analog Rate Limiter", 66);
	_AddSmwSymbol(syms, 30, "Analog Scaler", 24);
	_AddSmwSymbol(syms, 99, "Analog Scaler w/o Zero Pass", 65);
	_AddSmwSymbol(syms, 225, "Analog Scaler w/Overflow Handling", 538);
	_AddSmwSymbol(syms, 226, "Analog Scaler w/Overflow Handling w/o Zero Pass", 539);
	_AddSmwSymbol(syms, 224, "Analog Scaler with I/O Limits", 541);
	_AddSmwSymbol(syms, 68, "Analog Scaling Buffer", 47);
	_AddSmwSymbol(syms, 98, "Analog Scaling Buffer about 50%", 64);
	_AddSmwSymbol(syms, 37, "Analog Step", 45);
	_AddSmwSymbol(syms, 100, "Analog Sum", 57);
	_AddSmwSymbol(syms, 106, "Analog To Digital", 84);
	_AddSmwSymbol(syms, 131, "Analog to Floating Point", 113);
	_AddSmwSymbol(syms, 86, "Analog Value Sample", 122);
	_AddSmwSymbol(syms, 39, "Analog Variable Preset", 44);
	_AddSmwSymbol(syms, 232, "AntiLog with Limits", 547);
	_AddSmwSymbol(syms, 52, "Decade", 78);
	//_AddSmwSymbol(syms, 100, "Digital Sum", 61); //SAME AS ANALOG SUM
	_AddSmwSymbol(syms, 81, "Digital to Analog", 85);
	_AddSmwSymbol(syms, 55, "Digital To Scaled Analog", 36);
	_AddSmwSymbol(syms, 239, "Double-Precision Analog Initialize", 635);
	_AddSmwSymbol(syms, 240, "Double-Precision Analog Variable Preset", 636);
	_AddSmwSymbol(syms, 130, "Floating Point to Analog", 116);
	_AddSmwSymbol(syms, 231, "Log with Limits", 546);
	_AddSmwSymbol(syms, 221, "Multiple Analog Preset", 493);
	_AddSmwSymbol(syms, 41, "Numeric Keypad", 39);
	_AddSmwSymbol(syms, 233, "Ramp In Progress", 566);

	_AddSmwSymbol(syms, 45, "Analog Compare", 35);
	_AddSmwSymbol(syms, 220, "Analog Comparison (Full Set)", 492);
	_AddSmwSymbol(syms, 0, "AND", 1);
	_AddSmwSymbol(syms, 76, "Binary Decoder", 49);
	_AddSmwSymbol(syms, 18, "Buffer", 20);
	_AddSmwSymbol(syms, 5, "Exclusive NOR", 112);
	_AddSmwSymbol(syms, 4, "Exclusive OR", 5);
	_AddSmwSymbol(syms, 206, "Multiple NOT", 418);
	_AddSmwSymbol(syms, 1, "NAND", 2);
	_AddSmwSymbol(syms, 7, "Negative Transition Gate", 7);
	_AddSmwSymbol(syms, 3, "NOR", 4);
	_AddSmwSymbol(syms, 1, "NOT", 205);
	_AddSmwSymbol(syms, 2, "OR", 3);
	_AddSmwSymbol(syms, 6, "Transition Gate", 6);
	_AddSmwSymbol(syms, 19, "Truth Table", 28);

	_AddSmwSymbol(syms, 12, "Binary Counter", 18);
	_AddSmwSymbol(syms, 23, "Ring Counter", 17);
	_AddSmwSymbol(syms, 23, "Ring Counter with Seed", 451);

	_AddSmwSymbol(syms, 104, "Analog Debugger", 69);
	//_AddSmwSymbol(syms, -1, "Analog Force", 472);
	//_AddSmwSymbol(syms, -1, "Digital Force", 471);
	_AddSmwSymbol(syms, 34, "Message to Computer Port", 94);
	_AddSmwSymbol(syms, 59, "Serial Binary To Hex", 59);
	_AddSmwSymbol(syms, 54, "Serial Debugger (ASCII)", 68);
	_AddSmwSymbol(syms, 115, "Serial Debugger (Hex)", 191);
	//_AddSmwSymbol(syms, -1, "Serial Force", 470);
	//_AddSmwSymbol(syms, -1, "Signal Debugger with Label", 2848);
	_AddSmwSymbol(syms, 234, "User Event Logger", 577);

	_AddSmwSymbol(syms, 149, "ASCII to KB Scan Code", 187);
	_AddSmwSymbol(syms, 209, "Mouse Simulator", 437);
	_AddSmwSymbol(syms, 83, "Pesa Switch Command", 81);
	_AddSmwSymbol(syms, 137, "Unity Reponse Parser", 119);
	//_AddSmwSymbol(syms, -1, "Virtual IR Device for Macro Connection", 284);
	//_AddSmwSymbol(syms, -1, "Virtual Serial Driver for Macro Connection", 285);
	_AddSmwSymbol(syms, 132, "Westec Security Packet Filter", 189);

	//_AddSmwSymbol(syms, 85, "Emailbox", 351);
	//_AddSmwSymbol(syms, 85, "EmailViewer-10 msg inbox", 349);
	_AddSmwSymbol(syms, 85, "EmailViewer-12 msg inbox", 350);
	//_AddSmwSymbol(syms, 85, "EmailViewer-4 msg inbox", 347);
	//_AddSmwSymbol(syms, 85, "EmailViewer-8 msg inbox", 348);

	_AddSmwSymbol(syms, 36, "Analog RAM", 34);
	//_AddSmwSymbol(syms, 36, "Analog RAM From Database", 221);
	_AddSmwSymbol(syms, 8, "D Flip Flop", 14);
	//_AddSmwSymbol(syms, 36, "Digital RAM", 90);
	_AddSmwSymbol(syms, 128, "FIFO Queue", 120);
	_AddSmwSymbol(syms, 11, "Interlock", 19);
	_AddSmwSymbol(syms, 207, "Interlock-Toggle", 419);
	_AddSmwSymbol(syms, 13, "JK Flip Flop", 15);
	_AddSmwSymbol(syms, 32, "Memory Interlock", 25);
	_AddSmwSymbol(syms, 144, "Serial Memory Search", 186);
	_AddSmwSymbol(syms, 113, "Serial Queue", 71);
	_AddSmwSymbol(syms, 66, "Serial RAM", 53);
	//_AddSmwSymbol(syms, 66, "Serial RAM from database", 82);
	_AddSmwSymbol(syms, 9, "Set/Reset Latch", 12);
	_AddSmwSymbol(syms, 10, "Toggle", 13);

	_AddSmwSymbol(syms, 31, "Comment", 121);
	//_AddSmwSymbol(syms, -1, "SUBSYSTEM", 156);

	_AddSmwSymbol(syms, 138, "Button Presser", 99);
	_AddSmwSymbol(syms, 139, "Stepper", 98);
	_AddSmwSymbol(syms, 241, "Stepper with Progress & Reset", 777);

	_AddSmwSymbol(syms, 82, "Analog to Serial", 77);
	_AddSmwSymbol(syms, 109, "ASCII Keypad", 123);
	_AddSmwSymbol(syms, 65, "ASCII Serial Decoder", 54);
	//_AddSmwSymbol(syms, -1, "Convert To ASCII", 3054);
	//_AddSmwSymbol(syms, -1, "Convert To UTF-16", 3056);
	_AddSmwSymbol(syms, 135, "Duple Decoder", 127);
	_AddSmwSymbol(syms, 134, "Duple Encoder", 115);
	_AddSmwSymbol(syms, 205, "Make String Permanent", 858);
	//_AddSmwSymbol(syms, 205, "Make String Permanent v1 (CUZ 3.117 and below)", 417);
	//_AddSmwSymbol(syms, -1, "Mark As ASCII/UTF-16", 3053);
	//_AddSmwSymbol(syms, -1, "Multiple Discrete Serial Send", 3106);
	_AddSmwSymbol(syms, 58, "Multiple Serial Send", 1477);
	_AddSmwSymbol(syms, 247, "Send As Raw Data", 1076);
	_AddSmwSymbol(syms, 88, "Serial Buffer", 56);
	_AddSmwSymbol(syms, 122, "Serial Concatenation", 126);
	_AddSmwSymbol(syms, 107, "Serial Demultiplexor", 86);
	_AddSmwSymbol(syms, 112, "Serial Demultiplexor (Special)", 124);
	_AddSmwSymbol(syms, 93, "Serial Gather", 62);
	_AddSmwSymbol(syms, 97, "Serial I/O", 101);
	_AddSmwSymbol(syms, 65, "Serial Memory Dialer", 236);
	_AddSmwSymbol(syms, 111, "Serial Multiplexor (Special)", 125);
	_AddSmwSymbol(syms, 117, "Serial Pacer", 73);
	_AddSmwSymbol(syms, 58, "Serial Send", 89);
	//_AddSmwSymbol(syms, 94, "Serial Substring", 63);
	_AddSmwSymbol(syms, 94, "Serial Substring (Expandable)", 1317);
	_AddSmwSymbol(syms, 252, "Serial Substring w/Empty string pass", 1316);
	_AddSmwSymbol(syms, 103, "Serial to Analog", 96);
	_AddSmwSymbol(syms, 114, "Serial/Analog One-Shot", 72);
	_AddSmwSymbol(syms, 254, "String Length", 2506);
	_AddSmwSymbol(syms, 109, "Telephone Dialing Keypad", 212);
	_AddSmwSymbol(syms, 208, "Text Append", 462);

	_AddSmwSymbol(syms, 214, "Control Crosspoint Routing.", 447);
	_AddSmwSymbol(syms, 213, "Equipment Crosspoint Routing.", 446);
	_AddSmwSymbol(syms, 215, "Equipment/Control Crosspoint Connect.", 448);

	_AddSmwSymbol(syms, 202, "Console", 412);
	_AddSmwSymbol(syms, 50, "Hard Reset", 41);
	//_AddSmwSymbol(syms, 85, "Intersystem Communications", 327);
	_AddSmwSymbol(syms, 85, "Intersystem Communications w/Offset", 102);
	_AddSmwSymbol(syms, 123, "Intersystem Communications w/Status Req", 192);
	_AddSmwSymbol(syms, 49, "Soft Reset", 40);
	_AddSmwSymbol(syms, 235, "User Program Commands", 581);

	//_AddSmwSymbol(syms, 210, "Astronomical Clock", 438);
	_AddSmwSymbol(syms, 210, "Astronomical Clock with Enable", 859);
	_AddSmwSymbol(syms, 62, "Clock Driver", 43);
	//_AddSmwSymbol(syms, 62, "Clock Driver w/out DST", 3078);
	//_AddSmwSymbol(syms, 62, "Extended Clock Driver", 469);
	_AddSmwSymbol(syms, 60, "Past", 70);
	_AddSmwSymbol(syms, 101, "Serialize Date", 60);
	_AddSmwSymbol(syms, 121, "Set System Clock", 154);
	//_AddSmwSymbol(syms, 211, "Time Offset", 443);
	_AddSmwSymbol(syms, 211, "Time Offset with Enable", 860);
	_AddSmwSymbol(syms, 61, "When", 74);

	_AddSmwSymbol(syms, 56, "Debounce", 50);
	_AddSmwSymbol(syms, 27, "Delay", 16);
	_AddSmwSymbol(syms, 203, "Logic Wave Delay", 408);
	_AddSmwSymbol(syms, 204, "Logic Wave Pulse", 409);
	_AddSmwSymbol(syms, 27, "Long Delay", 545);
	//_AddSmwSymbol(syms, -1, "Multiple Logic Wave Pulses", 3104);
	_AddSmwSymbol(syms, 29, "Multiple One Shots", 21);
	_AddSmwSymbol(syms, 15, "One Shot", 9);
	_AddSmwSymbol(syms, 14, "Oscillator", 8);
	_AddSmwSymbol(syms, 16, "Pulse Stretcher", 11);
	_AddSmwSymbol(syms, 17, "Retriggerable One Shot", 10);
	_AddSmwSymbol(syms, 204, "Serial/Analog Logic Wave Pulse", 1309);
	_AddSmwSymbol(syms, 200, "Variable Delay", 411);
	_AddSmwSymbol(syms, 201, "Variable Oscillator", 37);

	_AddSmwSymbol(syms, 47, "Analog to Indirect Text", 37);
	_AddSmwSymbol(syms, 159, "CIP Transmission", 353);
	_AddSmwSymbol(syms, 212, "Indirect Text Broadcast", 208);
	_AddSmwSymbol(syms, 35, "Network Transmission", 266);
	_AddSmwSymbol(syms, 64, "Serial to Buffered Indirect Text", 190);
	//_AddSmwSymbol(syms, 153, "Serial to Ethernet Indirect Text", 279);
	_AddSmwSymbol(syms, 64, "Serial to Indirect Text", 52);
	//_AddSmwSymbol(syms, 64, "Serial to Indirect Text (Alpha Format Only)", 334);

	return syms;
}