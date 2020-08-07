/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   logFileParser.h
 * Author: Jobin Augustine
 *
 * Created on 27 August, 2017, 5:26 PM
 */
#include <fstream>
#include <vector>
#include <tuple>
#include <map>

#ifndef LOGFILEPARSER_H
#define LOGFILEPARSER_H
using namespace std;

class logFileParser {
private:
    ifstream& logfile;
    const vector<tuple<char,int,int,char>>& loglinelocation;   //Refer readme fore details of each of these fields
    bool flushPending; //multi line parsing is pending for flusing.
    
    string curUser;
    string curDb;
    string curApp;
    size_t curDuration;  //in micro seconds
    //size_t curStementHash;  //not used
    string curStatement;
    int curStatementType;
    map<size_t,string> SqlidSqlMap;
public:

    logFileParser(ifstream & logfile_in, const vector<tuple<char,int,int,char>> & loglinelocation_in);
    //logFileParser(const logFileParser& orig);
    virtual ~logFileParser();
    bool resetFD();
    bool parse();
    void StatementHandler(string & logline,size_t & curpos,size_t & endoffset);
    void HintHandler(string & logline,size_t & curpos,size_t & endoffset);
    void GetDuration(string & logline,size_t & curpos,size_t & endoffset);
    void locateNextChar(string & logline,size_t & curpos,size_t & endoffset);
    void getCheckpointDtls(string & logline,size_t & curpos,size_t & endoffset) noexcept;
    string& normalizequery(string& querysting)noexcept;
    void storeSqlidSqlmap(std::string& sql) noexcept;
    void printSqlidSqlmap() noexcept;
    void doFlush();
};

#endif /* LOGFILEPARSER_H */

