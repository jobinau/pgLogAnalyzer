/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   lineTokenizer.h
 * Author: Jobin Augustine
 *
 * Created on 29 August, 2017, 3:02 PM
 */
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <regex>

#ifndef LINETOKENIZER_H
#define LINETOKENIZER_H
using namespace std;

class lineTokenizer {
private:
    //ifstream logfile;
    vector<tuple<char,int,int,char>> loglinelocation;
    string lineFormat;
    string exprStr;
    //regex expr1;
public:
    lineTokenizer();
    //lineTokenizer(vector<tuple<char,int,int,char>> & loglinelocation);
    lineTokenizer(const lineTokenizer& orig);
    void genExprStr(const string & lineFormat_in) noexcept;
    bool assessLogLine(const string & logline);
    bool tokenize(ifstream & logfile);
    void printloglinelocation() noexcept;
    vector<tuple<char,int,int,char>>& getToken();
    virtual ~lineTokenizer();
};

#endif /* LINETOKENIZER_H */

