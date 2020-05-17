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
    cout<<"=====Test case 1 : Line format is :%m [%p] %q%u@%d ============\n";
    vector<tuple<char, int, int, char>> loglinelocation = {{'m', 0, 0, ' '}, {'p', 1, 3, ']'}, {'q', 2, 3, ' '}, {'u', 3, 1, '@'}, {'d', 4, 2, ' '}};
    ifstream logfile ("../samples/postgresql-2020-05-11_093620.log", std::ifstream::in);

    logFileParser FileParser(logfile,loglinelocation);
    //FileParser.resetFD();
    FileParser.parse();
    logfile.close();


    //Test case 2 : Line format is "%m [%p] "
    cout<<"======Test case 2 : Line format is \"%m [%p] \" ================\n";
    loglinelocation = {{'m', 0, 0, ' '}, {'p', 1, 3, ']'}};
    logfile.open ("../samples/postgresql-2020-05-11_102203.log", std::ifstream::in);
    //FileParser.resetFD();
    FileParser.parse();
    logfile.close();
    //printloglinelocation(loglinelocation);
    

    //test
    //loglinelocation = {{'m', 0, 0, 'a'}, {'p', 1, 3, ']'}, {'q', 2, 3, ' '}, {'u', 3, 1, '@'}, {'d', 4, 2, 'b'}};
    //logfile.open('../samples/postgresql-2020-05-11_102203.log',std::ifstream::in);
    //printloglinelocation(loglinelocation);
}