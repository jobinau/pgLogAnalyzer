#include "../logFileParser.h"
#include "../Helper.h"
#include <iostream>
#include <string>
#include <vector>
#include <tuple>

//A copy of lineTokenizer::printloglinelocation as local helper function.
void printloglinelocation(vector<tuple<char, int, int, char>> loglinelocation) noexcept
{
    cout << "====loglinelocation=====\n";
    for (auto i : loglinelocation)
    {
        cout << "(" << get<0>(i) << "," << get<1>(i) << "," << get<2>(i) << "," << get<3>(i) << ")" << endl;
    }
    cout << "========================\n";
}

int main(int argc, char *argv[])
{
    
    vector<tuple<char, int, int, char>> loglinelocation;
    ifstream logfile;

    logFileParser FileParser(logfile,loglinelocation);  //Parameters are passed in as reference

 
    cout<<"=====Test case 1 : Line format is :%m [%p] %q%u@%d ============\n";
    loglinelocation = {{'m', 0, 0, ' '}, {'p', 1, 3, ']'}, {'q', 2, 3, ' '}, {'u', 3, 1, '@'}, {'d', 4, 2, ' '}};
    logfile.open ("../samples/postgresql-2020-05-11_093620.log", std::ifstream::in);
    FileParser.parse();
    logfile.close();


    //Test case 2 : Line format is "%m [%p] "
    cout<<"======Test case 2 : Line format is : %m [%p]  ================\n";
    loglinelocation = {{'m', 0, 0, ' '}, {'p', 1, 3, ']'}};
    logfile.open ("../samples/postgresql-2020-05-11_102203.log", std::ifstream::in);
    FileParser.parse();
    logfile.close();


     //Test case 3 : Line format is "%m [%p] "
     //Check for Unknown log entries like startup / shutdown lines
    cout<<"======Test case 3 : Line format is : %t [%p]: [%l-1] user=%u,db=%d,app=%a,client=%h  ================\n";
    loglinelocation = {{'t',0,0,' '},{'p', 1, 3, ']'},{'l',2,5,'-'},{'u',3,10,','},{'d',4,5,','},{'a',5,6,','},{'h',6,9,' '}};
    logfile.open ("../samples/postgresql-2020-05-17_164935.log", std::ifstream::in);
    FileParser.parse();
    logfile.close();


    cout<<"======Test case 4 : Line format is : %p-%s-%c-%l-%h-%u-%d-%m  ================\n";
    //Test for parse with regex parse(true) - Roberts complex log_line_prefix
    loglinelocation = {{'p',0,0,'-'}, {'s',1,2,'-'}, {'c',2,2,'-'}, {'l',3,2,'-'}, {'h',4,2,'-'}, {'u',5,2,'-'}, {'d',6,2,'-'}, {'m',7,2,' '}};
    logfile.open ("../samples/postgresql-2020-08-04_000000_UTParser.log", std::ifstream::in);
    FileParser.parse(true);
    logfile.close();

}