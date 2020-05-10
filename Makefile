##Compiler of Choice : use clang++ if required
CC = g++
##Platform specific build is possible by passing CFLAGS : make CFLAGS="-std=c++11 -O3 -march=native"
CFLAGS = -std=c++11 -g -Wall
##Full build includes multiple exes
fullbuild : pgloganalyzer
##How to build full pgloganalyzer
pgloganalyzer : main.o Helper.o logFileParser.o lineTokenizer.o 
	$(CC) $(CFLAGS) main.o Helper.o logFileParser.o lineTokenizer.o -o pgloganalyzer
main.o : main.cpp
	$(CC) $(CFLAGS) -c main.cpp
Helper.o : Helper.cpp Helper.h
	$(CC) $(CFLAGS) -c Helper.cpp
logFileParser.o : logFileParser.cpp logFileParser.h
	$(CC) $(CFLAGS) -c logFileParser.cpp
lineTokenizer.o : lineTokenizer.cpp lineTokenizer.h
	$(CC) $(CFLAGS) -c lineTokenizer.cpp
##Clean up the build
clean :
	rm *.o pgloganalyzer
