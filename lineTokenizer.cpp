/* 
 * File:   lineTokenizer.cpp
 * Author: Jobin Augustine
 */

#include "lineTokenizer.h"
#include <iostream>

//Generate the Regular Expression string based on the log_line_prefix.
//Later use the same to generate the regex object
void lineTokenizer::genExprStr(const string & lineFormat_in) noexcept {
    exprStr = lineFormat_in;
    lineFormat = lineFormat_in;

   //%t => Timestamp without Milliseconds
    auto loc=exprStr.find("%t");

    if (loc<200) 
        exprStr.replace(loc, 2 , R"EXPR(^(\d+-\d+-\d+ \d+:\d+:\d+[\.0-9]) [A-Z]{3})EXPR");

    //%m => Timestamp with Millisecods
    loc=exprStr.find("%m");
    if (loc<200)
        exprStr.replace(loc, 2 , R"EXPR(^(\d+-\d+-\d+ \d+:\d+:\d+\.\d+) [A-Z]{3})EXPR");

    //%s => Timestamp of the Process startup
    loc = exprStr.find("%s");
    if (loc < 200)
        exprStr.replace(loc,2,R"EXPR((\d+-\d+-\d+ \d+:\d+:\d+) [A-Z]{3})EXPR");

    loc = exprStr.find("%p");
    if (loc<200)
        exprStr.replace(loc,2,R"EXPR([0-9]{3,9})EXPR");
    
    loc = exprStr.find("%l");
    if (loc < 200)
        exprStr.replace(loc ,2,R"EXPR([0-9]{1,8})EXPR");
    
    //%r => IP address of the client with remote port in bracket like : 10.123.200.65(60612)
    loc = exprStr.find("%r");
    if (loc < 200) 
        exprStr.replace(loc ,2,R"EXPR([\w\.\(\)]+)EXPR");

    //%i => Statement type like: SELECT
    loc = exprStr.find("%i");
    if (loc < 200) 
        exprStr.replace(loc ,2,R"EXPR(\w+)EXPR");

    loc = exprStr.find("%u");
    if (loc < 200)
        exprStr.replace(loc,2,R"EXPR([\w\[\]]*)EXPR");
    
    loc = exprStr.find("%d");
    if (loc < 200)
        exprStr.replace(loc,2,R"EXPR([\w\[\]]*)EXPR");
    
    loc = exprStr.find("%a");
    if (loc < 200)
        exprStr.replace(loc,2,R"EXPR([\w\[\]]*)EXPR");
    
    loc = exprStr.find("%h");
    if (loc < 200)
        exprStr.replace(loc,2,R"EXPR([\w\.\[\]]*)EXPR");

    //if %q is found, replace it with null string
    loc = exprStr.find("%q");
    if (loc < 200) 
        exprStr.replace(loc,2,R"EXPR()EXPR");

    cout<<"exprStr is :"<<exprStr<<endl;
    regex expr(exprStr);
    regExpr = expr;   //copy assginement

    //regExpr = [=](){regex expr(exprStr); return expr;};

}


//Prepare Log Line Location datastructure from given sample line
//Retunrs "true" if match is found. "false" if match is not found.
bool lineTokenizer::prepareLogLineLocation(const string & logline){
    smatch matches;
    int order = 0;
    int offset = 0;
    //Note preparing regular expression everytime could be a problem, Try shifting to genExprStr function
    regex expr(exprStr);
    //auto match = regex_search(logline,matches,expr);
    auto match = regex_search(logline,matches,regExpr);
    cout<<"Line format is :"<<lineFormat<<endl;
    if(match && loglinelocation.empty()){
        //cout<<"Got the first match"<<endl;
        bool isEscape = false;
        bool getEndMark = false;
        for (auto i : lineFormat){
            if(i == '%'){
                isEscape = true;
                continue;
            }
            if (isEscape && int(i) > 96 && int(i) < 123){
                loglinelocation.push_back(make_tuple(i,order,offset,' '));
                offset = 0;
                isEscape = false;
                getEndMark = true;
                order++;
            }
            else if (getEndMark){
                get<3>(loglinelocation[order-1]) = i;
                getEndMark = false;
            }
            offset++;
        }
        //get<3>(loglinelocation[order-1]) = matches.suffix().str()[0];
        //Temporary workaround for Clang issue with regex.
        get<3>(loglinelocation[order-1]) = ' ';
        //cout<<" Remaining string : "<<matches.suffix().str()[0]<<endl;
    }
    //cout<<"Size of the string is :"<<cnt++<<endl;
    return match;
}

//Open the log file and scan for first few lines and make sure that the  pattern is matching
bool lineTokenizer::logFileTokenize(ifstream & logfile){
    int matched = 0;
    int notmatched = 0;
    string logline;
    cout<<"Line format is :"<<lineFormat<<endl;
    while (std::getline(logfile, logline))
    {
        //auto match = regex_search(logline,matches,expr);
        //cout<<logline<<endl;
        auto match = prepareLogLineLocation(logline);
        //cout<<(match? "Matched" : "Not Matched")<<endl;
        if(match){
            matched++;        
        }
        else{
            notmatched++;
        }
        //after reading 20 lines break
        if ((matched+notmatched)> 20) {
            break;
}
    }
    cout<<"Number of Matches :"<<matched<<" and Number of non-matches :"<<notmatched<<endl;
    if (matched < 1){
        cout<<"Appears that log_line_prefix is different"<<endl;
        return false;
    }
    return true;
}

//Clear the loglinelocation data strcuture using the STL clear()
//This ensures that the loglinelocation.empty() call will be true
void lineTokenizer::clearLogLineLocation() noexcept {
    loglinelocation.clear();
}

//print the data in loglinelocation
void lineTokenizer::printloglinelocation() noexcept {
    cout<<"====loglinelocation=====\n";
    for (auto i : loglinelocation){
        cout<<"("<< get<0>(i)<<","<<get<1>(i)<<","<<get<2>(i)<<","<<get<3>(i)<<")"<<endl;
    }
    cout<<"========================\n";
}

//Return the loglinelocation which is the final outcome of this class.
vector<tuple<char,int,int,char>> lineTokenizer::getToken(){
    return loglinelocation;
}