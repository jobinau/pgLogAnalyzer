##Compiler of Choice : use clang++ if required
CC = g++
##Platform specific build is possible by passing CFLAGS : make CFLAGS="-std=c++11 -O3 -march=native"
CFLAGS = -std=c++11 -g -Wall
##Full build includes multiple exes
fullbuild : UTLineTokenizer

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
##Clean up the build
clean :
	rm *.o UTLineTokenizer
