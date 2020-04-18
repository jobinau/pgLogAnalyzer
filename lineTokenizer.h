/* 
 * File:   lineTokenizer.h
 * Author: Jobin Augustine

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
    vector<tuple<char,int,int,char>> loglinelocation;
    string lineFormat;
    string exprStr;
public:
    lineTokenizer();
    //lineTokenizer(vector<tuple<char,int,int,char>> & loglinelocation);
    //lineTokenizer(const lineTokenizer& orig);
    void genExprStr(const string & lineFormat_in) noexcept;
    bool prepareLogLineLocation(const string & logline);
    bool logFileTokenize(ifstream & logfile);
    void printloglinelocation() noexcept;
    vector<tuple<char,int,int,char>>& getToken();
    virtual ~lineTokenizer();
};

#endif /* LINETOKENIZER_H */

