CC = g++
CFLAGS = -std=c++11 -g -Wall
pgloganalyzer : main.o Helper.o logFileParser.o lineTokenizer.o
	$(CC) $(CFLAGS)  main.o Helper.o logFileParser.o lineTokenizer.o -o pgloganalyzer
main.o : main.cpp
	$(CC) $(CFLAGS) -c main.cpp
Helper.o : Helper.cpp Helper.h
	$(CC) $(CFLAGS) -c Helper.cpp
logFileParser.o : logFileParser.cpp logFileParser.h
	$(CC) $(CFLAGS) -c logFileParser.cpp
lineTokenizer.o : lineTokenizer.cpp lineTokenizer.h
	$(CC) $(CFLAGS) -c lineTokenizer.cpp
clean :
	rm *.o pgloganalyzer
