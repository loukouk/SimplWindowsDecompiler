CXX = gcc
CFLAGS = -Wall -pedantic-errors -O2 -std=gnu99
BINREADER_PATH = ./BinReader/
SIGREADER_PATH = ./SigReader/
SYMBOLLIB_PATH = ./SymbolLib/
OBJECTS = ${BINREADER_PATH}BinReader.o ${SIGREADER_PATH}SigReader.o ${SYMBOLLIB_PATH}SymbolLib.o
TARGET = main


default: all


all: ${OBJECTS} target
	${CXX} ${CFLAGS} ${OBJECTS} ${TARGET}.o -o ${TARGET}.exe


target:
	${CXX} ${CFLAGS} -c ${TARGET}.c -o ${TARGET}.o


BinReader: ${BINREADER_PATH}BinReader.o
	${CXX} ${CFLAGS} -c ${BINREADER_PATH}main.c -o ${BINREADER_PATH}main.o
	${CXX} ${CFLAGS} ${BINREADER_PATH}BinReader.o ${BINREADER_PATH}main.o -o ${BINREADER_PATH}main.exe

${BINREADER_PATH}BinReader.o:
	${CXX} ${CFLAGS} -c ${BINREADER_PATH}BinReader.c -o ${BINREADER_PATH}BinReader.o


SigReader: ${SIGREADER_PATH}SigReader.o
	${CXX} ${CFLAGS} -c ${SIGREADER_PATH}main.c -o ${SIGREADER_PATH}main.o
	${CXX} ${CFLAGS} ${SIGREADER_PATH}SigReader.o ${SIGREADER_PATH}main.o -o ${SIGREADER_PATH}main.exe

${SIGREADER_PATH}SigReader.o:
	${CXX} ${CFLAGS} -c ${SIGREADER_PATH}SigReader.c -o ${SIGREADER_PATH}SigReader.o


SymbolLib: ${SYMBOLLIB_PATH}SymbolLib.o
	${CXX} ${CFLAGS} -c ${SYMBOLLIB_PATH}main.c -o ${SYMBOLLIB_PATH}main.o
	${CXX} ${CFLAGS} ${SYMBOLLIB_PATH}SymbolLib.o ${SYMBOLLIB_PATH}main.o -o ${SYMBOLLIB_PATH}main.exe

${SYMBOLLIB_PATH}SymbolLib.o:
	${CXX} ${CFLAGS} -c ${SYMBOLLIB_PATH}SymbolLib.c -o ${SYMBOLLIB_PATH}SymbolLib.o
	

debug:
	${CXX} ${CFLAGS} ${TARGET}.c -o ${TARGET}.exe -DEBUG

clean:
	del ${OBJECTS}
	del /S *.exe