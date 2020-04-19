/* 
 * File:   logFileParser.cpp
 * Author: Jobin Augustine
 */

#include "logFileParser.h"
#include "Helper.h"
#include <ctime>
#include <iostream>

logFileParser::logFileParser(ifstream & logfile_in, const vector<tuple<char,int,int,char>> & loglinelocation_in)
            :logfile(logfile_in),loglinelocation(loglinelocation_in)
{
  //initialize class variables before parsing starts
  flushPending = false;
  curDuration = 0;
  curStatementType= 0;  
}


logFileParser::~logFileParser() = default;

bool logFileParser::resetFD(){
    logfile.clear();
    logfile.seekg(0, ios::beg);
    return true;
}

bool logFileParser::parse(){
    
    int totChars = 0;
    int totLines = 0;
    int totErrorLines = 0;
    int unixTime = 0;
    size_t curpos = 0;     //current position in the logline
    size_t endoffset;
    size_t linelength = 0;
    string logline;
    struct tm tm{};
    flushPending = false;
    cout<<"==============Again Starting  =============="<<endl;
    while (getline(logfile, logline))
    {
        //if the line is continuation of previous SQL statement, append the line to statement and continue the loop
        if(flushPending && (isspace(logline[0]) != 0) ){
                curStatement = curStatement + " " + trim_inplace(logline);
                continue;
        }
        
        //if the line is a new log entry
        linelength = logline.length();
        for (auto i : loglinelocation){
            //cout<<"("<< get<0>(i)<<","<<get<1>(i)<<","<<get<2>(i)<<","<<get<3>(i)<<"),";
            curpos = curpos + get<2>(i);
            switch(get<0>(i)){
                case 't': {
                    
                    //Check whether initial part of line can be treated as timestamp                   
                    if(strptime(logline.substr(curpos,19).c_str(), "%Y-%m-%d %H:%M:%S", &tm)== nullptr){
                        totErrorLines++;
                        goto nextline;
                    }
                    
                    unixTime = mktime(&tm);
                    cout<<"TS : "<<unixTime;
                    curpos = curpos + 22;
                    break;
                }
                case 'p' : {
                    endoffset =  logline.find(get<3>(i),curpos)-curpos;
                    //cout << " end : " << endoffset ;
                    cout<<" Proc: " << logline.substr(curpos,endoffset);
                    curpos = curpos + endoffset - 1;
                    break;
                } 
                case 'l' :{
                    endoffset =  logline.find(get<3>(i),curpos)-curpos;
                    //cout << " end : " << endoffset ;
                    cout<<" Session: " << logline.substr(curpos,endoffset);
                    curpos = curpos + endoffset - 1;
                    break;
                }
                case 'u' : {
                    endoffset =  logline.find(get<3>(i),curpos)-curpos;
                    //cout << " end : " << endoffset ;
                    cout<<" User : " << logline.substr(curpos,endoffset);
                    curpos = curpos + endoffset - 1;
                    break;
                }
                case 'a' : {
                    endoffset =  logline.find(get<3>(i),curpos)-curpos;
                    //cout << " end : " << endoffset ;
                    cout<<" APP : " << logline.substr(curpos,endoffset);
                    curpos = curpos + endoffset - 1;
                    break;
                }
                case 'd' : {
                    endoffset =  logline.find(get<3>(i),curpos)-curpos;
                    //cout << " end : " << endoffset ;
                    cout<<" DB : " << logline.substr(curpos,endoffset);
                    curpos = curpos + endoffset - 1;
                    break;
                }
                case 'h' : {
                    endoffset =  logline.find(get<3>(i),curpos)-curpos;
                    //cout << " end : " << endoffset ;
                    cout<<" host : " << logline.substr(curpos,endoffset);
                    curpos = curpos + endoffset - 1;
                    break;
                }
            }
        }
        //after the prefix part of the logline,
        curpos = curpos+ 2;
        switch (logline[curpos]){
            case 'L' :{   //LOG:
                if (flushPending) { doFlush();
}
                curpos = logline.find(':',curpos);
                curpos++;
                while(curpos < linelength){
                    char i = logline[curpos];
                    char ii= logline[curpos-1];
                    if(static_cast<int>(ii) == 32 && static_cast<int>(i) > 96 && static_cast<int>(i) < 123){
                        switch (i){
                            case 'e':{   //execute :
                                if(logline.compare(curpos,3,"exe") == 0){
                                    StatementHandler(logline,curpos,endoffset);
                                }
                                break;
                            }
                            case 'd' :{  //duration :
                                if (logline.compare(curpos,4,"dura") == 0){
                                    cout<<" Got Duration : ";
                                    GetDuration(logline,curpos,endoffset);
                                    locateNextChar(logline,curpos,endoffset);
                                    //cout<<"Remaining : "<<logline.substr(curpos)<<endl;
                                    goto whilecontinue;
                                }
                                break;
                            }
                            case 's' : { //statement :
                                if ( logline.compare(curpos,5,"state") == 0){
                                //cout <<" Got Statement : ";
                                 StatementHandler(logline,curpos,endoffset);
                                }
                                break;
                            }
                            case 'c' :{
                                if ( logline.compare(curpos,19,"checkpoint complete") == 0){
                                    //cout<<"Checkpoint:";
                                    curpos = curpos + 19;
                                    getCheckpointDtls(logline,curpos,endoffset);
                                }
                            }
                        }
                        //break;
                    }
                    curpos++;
                    whilecontinue:;
                }
                //cout <<endl<<"SQL :" << logline.substr(curpos);
                //cout <<" HASH : "<< hash<string>{}(logline.substr(curpos));
                break;
                }
            case 'D' :{
                cout << " Got a DETAILS";
                break;
            }
            case 'W' :{
                cout << " Got a WARNING";
                break;
            }
            case 'E' :{
                cout << " Got a ERROR :" << logline.substr(curpos);
                break;
            }
            case 'F' :{
                cout << " Got a FATAL";
                break;
            }
            case 'P' :{
                cout << " Got a PANIC";
                break;
            }
            case 'S' :{
                //cout << " Got a STATEMENT"<<logline.substr(curpos);
                StatementHandler(logline,curpos,endoffset);
                break;
            }
            case 'H' :{
                cout << " Got a HINT";
                HintHandler(logline,curpos,endoffset);
                break;
            }       
            case 'C' :{
                cout << " Got a CONTEXT";
                break;
            }       
        }
        
        //Handle non-prefix section : LOG|WARNING|ERROR|FATAL|PANIC|DETAIL|STATEMENT|HINT|CONTEXT|LOCATION
        cout<<endl;
        nextline:;
        curpos = 0;
        totChars = totChars + logline.length();
        totLines++;
    } 
    cout<<"Tot. Chars :"<<totChars<<" Tot. Lines :"<< totLines <<" Tot. Error Lines :" << totErrorLines <<endl;
    return true;
}

void logFileParser::StatementHandler(string & logline,size_t & curpos,size_t & endoffset){
    endoffset = logline.find(':',curpos);
    //line for debugging
    //auto tmpStr = logline.substr(curpos);
    if (endoffset != string::npos){
                curpos = endoffset;
    }
    //line for debugging
    //tmpStr = logline.substr(curpos);
    for (auto i : logline.substr(curpos)){
        if(static_cast<int>(i) > 64 && static_cast<int>(i) < 123){
            switch (i){
            case 's':
            case 'S':
                curStatementType = 1;
                //cout<<" Select Statement:";
                goto forexit;
                break;
            case 'i':
            case 'I':
                curStatementType = 2;
                //cout<<" Insert Statement:";
                goto forexit;
                break;
            case 'u':
            case 'U':
                curStatementType = 3;
                //cout<<" Update Statement:";
                goto forexit;
                break;
            case 'd':
            case 'D':
                if (strUpper(logline.substr(curpos,4)) == "DELE"){
                    curStatementType = 4;
                    //cout<<" Delete Statement: "<<endl;
                }
                if (strUpper(logline.substr(curpos,4)) == "DROP"){
                    curStatementType = 21;
                    //cout<<" Drop Statement: "<<endl;
                }
                goto forexit;
                break;
            case 'a':
            case 'A':
                if (strUpper(logline.substr(curpos,4)) == "ALTE"){
                    curStatementType = 22;
                    //cout<<" Alter Statement: "<<endl;
                }
                goto forexit;
                break;
            case 'c':
            case 'C':
                if (strUpper(logline.substr(curpos,4)) == "COPY"){
                    curStatementType = 5;
                    //cout<<"Copy statement"<<endl;
                }
                else if (strUpper(logline.substr(curpos,4)) == "CREA"){
                    curStatementType = 23;
                    //cout<<" Create Statement:"<<endl;
                }
                else if (strUpper(logline.substr(curpos,4)) == "COMM"){
                    curStatementType = 6;
                    //cout<<" Commit Statement:"<<endl;
                }
                goto forexit;
                break;
            case 'v':
            case 'V':
                curStatementType = 7;
                //cout<<" Vacuum Statement:";
                goto forexit;
                break;
            case 'w':
            case 'W':
                curStatementType = 8;
                goto forexit;
                break;
            default:
                curStatementType = 41;
                goto forexit;
                //cout<<"Unknown Statment:";
            }
        }
        curpos++;
    }
    forexit:;
    //cout<<"\""<<logline.substr(curpos)<<"\""<<endl;
    curStatement = logline.substr(curpos);
    flushPending = true;
    curpos = logline.length();
}

void logFileParser::HintHandler(string & logline,size_t & curpos,size_t & endoffset){
    endoffset = logline.find(':',curpos);
    if (endoffset != string::npos){
                curpos = endoffset;
    }
    for (char i : logline.substr(curpos)){
        if( static_cast<int>(i) >  64 && static_cast<int>(i) <= 123 ) { break;
}
        curpos++;
    }
    cout<<logline.substr(curpos)<<endl;
}

void logFileParser::GetDuration(string & logline,size_t & curpos,size_t & endoffset){
    endoffset = logline.find(':',curpos);
    if (endoffset != string::npos){
                curpos = endoffset;
    }
    
    for (auto i : logline.substr(curpos)){
        if( (static_cast<int>(i) > 47) && (static_cast<int>(i) < 58) ){
            break;
        }
        curpos++;
    }
    
    endoffset = logline.find(' ',curpos) - curpos;
    curDuration = static_cast<int>(stod(logline.substr(curpos,endoffset))*1000);
    cout<<logline.substr(curpos,endoffset)<< " "<<curDuration;
    curpos = curpos + endoffset + 3;
}

void logFileParser::locateNextChar(string & logline,size_t & curpos,size_t &  /*endoffset*/){
    for (auto i : logline.substr(curpos)){
        if(static_cast<int>(i) > 64 && static_cast<int>(i) < 123) { break;
}
        curpos++;
    }
}

void logFileParser::getCheckpointDtls(string & logline,size_t & curpos,size_t & endoffset) noexcept{
    endoffset = logline.find(':',curpos);
    if (endoffset != string::npos){
                curpos = endoffset+1;
    }
    
    endoffset = logline.find("wro",curpos);
    if (endoffset != string::npos){
                curpos = endoffset+6;
    }
    
    //cout<<"Checkpoint Dtls :"<<logline.substr(curpos,20);
    endoffset = logline.find(' ',curpos);
    cout<<"Number of buffers :"<<stoi(logline.substr(curpos,endoffset-curpos));
    
    endoffset = logline.find("total",curpos);
    if (endoffset != string::npos){
                curpos = endoffset+6;
    }
    endoffset = logline.find(' ',curpos);
    cout<<"time taken : "<<stod(logline.substr(curpos,endoffset-curpos));

    endoffset = logline.find("sync",curpos);
    if (endoffset != string::npos){
                curpos = endoffset+11;
    }
    endoffset = logline.find(' ',curpos);
    cout<<"Files sync : "<<stoi(logline.substr(curpos,endoffset-curpos));
    
    endoffset = logline.find("dista",curpos);
    if (endoffset != string::npos){
                curpos = endoffset+9;
    }
    endoffset = logline.find(' ',curpos);
    cout<<"Distance : "<<stoi(logline.substr(curpos,endoffset-curpos));
}

string& logFileParser::normalizequery(string& querysting) noexcept{
    //remove comments
    //convert query to lowercase
    //remove extra space and make it single space
    //remove " begin " if any
    //remove string content comes within '' 
    //remove NULL parameter
    //remove numbers
    //remove hexa decimal numbers
    //remove bind parameter comes after $ like $1,$2 etc
    //remove cursor name
    return querysting;
}

void logFileParser::storeSqlidSqlmap(std::string& sql) noexcept{
    //Nomalize sql before storing
    cleanExtraWhitespace(sql);
    SqlidSqlMap[std::hash<std::string>{}(sql)] = sql;
}

void logFileParser::printSqlidSqlmap() noexcept{
    cout<<"Number of Elements are"<<SqlidSqlMap.size()<<endl;
    for (auto& kv : SqlidSqlMap) {
        cout<<kv.first<<"--->"<<kv.second<<endl;
    }
}

void logFileParser::doFlush(){
    cout<<"Last Statement is :"<<curStatement<<endl;
    storeSqlidSqlmap(curStatement);
    flushPending = false;
}
