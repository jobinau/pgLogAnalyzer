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

    cout<<"=========Test Case 4=============\n";
    lineFormat="%t %u %d %h ";
    tokenizer.genExprStr(lineFormat);
    tokenizer.clearLogLineLocation();
    if (tokenizer.prepareLogLineLocation("2020-06-06 17:40:58 IST postgres postgres 127.0.0.1 LOG:  duration: 50.743 ms  statement: vacuum analyze pgbench_branches")){
        tokenizer.printloglinelocation();
    }else{
        cout<<"Match failed\n";
    }

    cout<<"=========Test Case 4.1=============\n";
    lineFormat="%t %u %d %h ";
    tokenizer.genExprStr(lineFormat);
    tokenizer.clearLogLineLocation();
    if (tokenizer.prepareLogLineLocation("2020-06-06 17:39:16 IST postgres postgres [local] LOG:  duration: 92.673 ms  statement: alter table pgbench_branches add primary key (bid)")){
        tokenizer.printloglinelocation();
    }else{
        cout<<"Match failed\n";
    }


    cout<<"=========Test Case 5=============\n";
    lineFormat="%p-%s-%c-%l-%h-%u-%d-%m";
    tokenizer.genExprStr(lineFormat);
    tokenizer.clearLogLineLocation();
    if (tokenizer.prepareLogLineLocation("31189-2020-08-04 00:00:04 PDT-5f290774.79d5-1-oshnodeapp-10-103-6-85.us-east4-c.c.saas-cstack-use4-231700.internal-sc52022udwh-sc52022udwh-2020-08-04 00:00:05.096 PDTLOG:  process 31189 still waiting for AccessShareLock on relation 51098774 of database 1814248 after 1000.193 ms at character 317")){
        tokenizer.printloglinelocation();
    }else{
        cout<<"Match failed\n";
    }

    //TODO : Add more test cases for unit testing
}