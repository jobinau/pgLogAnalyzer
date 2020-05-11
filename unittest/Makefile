##Compiler of Choice : use clang++ if required
CC = g++
##Platform specific build is possible by passing CFLAGS : make CFLAGS="-std=c++11 -O3 -march=native"
CFLAGS = -std=c++11 -g -Wall
##Full build includes multiple exes
fullbuild : UTLineTokenizer UTParser

##How to build UTLineTokenizer
##For linking against lineTokenizer.o of the parent directory, modify next 2 lines to have ../lineTokenizer.o
##Since the Unit test is used for development of linetockenizer, it is better to be built seperately in this directory
UTLineTokenizer : UTLineTokenizer.o lineTokenizer.o
	$(CC) $(CFLAGS) UTLineTokenizer.o lineTokenizer.o -o UTLineTokenizer
##Compile Individual source files to object files
UTLineTokenizer.o : UTLineTokenizer.cpp
	$(CC) $(CFLAGS) -c UTLineTokenizer.cpp
lineTokenizer.o : ../lineTokenizer.cpp ../lineTokenizer.h
	$(CC) $(CFLAGS) -c ../lineTokenizer.cpp
##Build Steps for UTParser. But link against Helper.o in the parent directory
UTParser : UTParser.o logFileParser.o
	$(CC) $(CFLAGS) UTParser.o logFileParser.o ../Helper.o -o UTParser
UTParser.o : UTParser.cpp
	$(CC) $(CFLAGS) -c UTParser.cpp
logFileParser.o : ../logFileParser.cpp  ../logFileParser.h
	$(CC) $(CFLAGS) -c ../logFileParser.cpp ../logFileParser.h

##Clean up the build
clean :
	rm *.o UTLineTokenizer UTParser
