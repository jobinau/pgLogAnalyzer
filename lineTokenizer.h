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
    regex regExpr;
public:

    void genExprStr(const string & lineFormat_in) noexcept;
    bool prepareLogLineLocation(const string & logline);
    bool logFileTokenize(ifstream & logfile);
    void clearLogLineLocation() noexcept;
    void printloglinelocation() noexcept;
    vector<tuple<char,int,int,char>> getToken();
    
};

#endif /* LINETOKENIZER_H */

