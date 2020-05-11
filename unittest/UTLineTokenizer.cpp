/*
Author : Jobin Augustine 
Unit Test Program for Line Tockenizer
*/
#include "../lineTokenizer.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    lineTokenizer tokenizer;
    string lineFormat;

    cout<<"Look for unhanlded '%' symbols and 'Match Failed' messages\n";
    
    cout<<"=========Test Case 1=============\n";
    lineFormat="%m [%p] %q%u@%d";
    tokenizer.genExprStr(lineFormat);
    tokenizer.clearLogLineLocation();
    if (tokenizer.prepareLogLineLocation("2020-04-17 09:09:53.652 IST [27919] postgres@postgres LOG:  duration: 2.294 ms  statement: xxx ")){
       tokenizer.printloglinelocation();
    }else{
        cout<<"Match failed\n";
    }
    
    cout<<"=========Test Case 2=============\n";
    lineFormat="%t %s %d %r %i";
    tokenizer.genExprStr(lineFormat);
    tokenizer.clearLogLineLocation();
    if (tokenizer.prepareLogLineLocation("2019-07-31 00:00:00 CDT 2019-07-30 23:55:42 CDT ihr2 10.123.200.65(60612) SELECT LOG:  duration: 46290.326 ms  execute <unnamed>: SELECT XXXX")){
        tokenizer.printloglinelocation();
    }else{
        cout<<"Match failed\n";
    }

    cout<<"=========Test Case 3=============\n";
    lineFormat="%m [%p] ";
    tokenizer.genExprStr(lineFormat);
    tokenizer.clearLogLineLocation();
    if (tokenizer.prepareLogLineLocation("2020-05-11 10:24:31.731 IST [13931] LOG:  duration: 1178.177 ms  statement: copy pgbench_accounts from stdin")){
        tokenizer.printloglinelocation();
    }else{
        cout<<"Match failed\n";
    }

    //TODO : Add more test cases for unit testing
}