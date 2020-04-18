/* 
 * File:   lineTokenizer.cpp
 * Author: Jobin Augustine
 */

#include "lineTokenizer.h"
#include <iostream>

//Currently this default constructor is used in the main
lineTokenizer::lineTokenizer() = default;


// lineTokenizer::lineTokenizer(const lineTokenizer&  /*orig*/) {
// }

lineTokenizer::~lineTokenizer() = default;

void lineTokenizer::genExprStr(const string & lineFormat_in) noexcept {
    exprStr = lineFormat_in;
    lineFormat = lineFormat_in;

    auto loc=exprStr.find("%t");

    if (loc<200) 
        exprStr.replace(loc, 2 , R"EXPR(^(\d+-\d+-\d+ \d+:\d+:\d+[\.0-9]) [A-Z]{3})EXPR");

    loc=exprStr.find("%m");
    if (loc<200)
        exprStr.replace(loc, 2 , R"EXPR(^(\d+-\d+-\d+ \d+:\d+:\d+\.\d+) [A-Z]{3})EXPR");

    loc = exprStr.find("%p");
    if (loc<200)
        exprStr.replace(loc,2,R"EXPR([0-9]{3,9})EXPR");
    
    loc = exprStr.find("%l");
    if (loc < 200)
        exprStr.replace(loc ,2,R"EXPR([0-9]{1,8})EXPR");
    
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
    //expr = std::move(expr1);
}


//Prepare Log Line Location datastructure from given sample line
//Retunrs "true" if match is found. "false" if match is not found.
bool lineTokenizer::prepareLogLineLocation(const string & logline){
    smatch matches;
    int order = 0;
    int offset = 0;
    //preparing regular expression everytime could be a problem
    static regex expr(exprStr);
    auto match = regex_search(logline,matches,expr);
    //cout<<"Line format is :"<<lineFormat<<endl;
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
        //after reading 200 lines break
        if ((matched+notmatched)> 200) {
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

void lineTokenizer::printloglinelocation() noexcept {
    cout<<"printloglinelocation"<<endl;
    for (auto i : loglinelocation){
        cout<<"("<< get<0>(i)<<","<<get<1>(i)<<","<<get<2>(i)<<","<<get<3>(i)<<")"<<endl;
    }
}

vector<tuple<char,int,int,char>>& lineTokenizer::getToken(){
    return loglinelocation;
}