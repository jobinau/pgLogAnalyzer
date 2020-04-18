CC = g++
CFLAGS = -std=c++11 -g -Wall
##Full build includes multiple exes
fullbuild : UTLineTokenizer pgloganalyzer
##How to build full pgloganalyzer
pgloganalyzer : main.o Helper.o logFileParser.o lineTokenizer.o 
	$(CC) $(CFLAGS) $(CXXFLAGS)  main.o Helper.o logFileParser.o lineTokenizer.o -o pgloganalyzer
##How to build UTLineTokenizer
UTLineTokenizer : UTLineTokenizer.o lineTokenizer.o
	$(CC) $(CFLAGS) UTLineTokenizer.o lineTokenizer.o -o UTLineTokenizer
UTLineTokenizer.o : UTLineTokenizer.cpp
	$(CC) $(CFLAGS) -c UTLineTokenizer.cpp
main.o : main.cpp
	$(CC) $(CFLAGS) -c main.cpp
Helper.o : Helper.cpp Helper.h
	$(CC) $(CFLAGS) -c Helper.cpp
logFileParser.o : logFileParser.cpp logFileParser.h
	$(CC) $(CFLAGS) -c logFileParser.cpp
lineTokenizer.o : lineTokenizer.cpp lineTokenizer.h
	$(CC) $(CFLAGS) -c lineTokenizer.cpp
clean :
	rm *.o pgloganalyzer UTLineTokenizer
