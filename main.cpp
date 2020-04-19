/*
Author : Jobin Augustine 

This contains main() of the program.
Functionality:
1. Accepts the command line parameters and parse them.
2. Holds the PostgreSQL parameter file name in inputFileName and log_line_prefix in lineFormat
3. Uses "tokenizer" for preparing loglinelocation. REFER tokenizer class for details.
  where "loglinelocation" data strcuture for holding the markers in a log line.
4. "loglinelocation" must not be empty for further processing

*/

#include "logFileParser.h"
#include "Helper.h"
#include "lineTokenizer.h"
#include <iostream>


//helper function to check wether "opt" is there in "args"
bool chkOptExist(const std::vector<std::string> & args,const std::string & opt){
	return find(args.begin(), args.end(),opt) != args.end();
}

//helper function to retun the value of "opt" in "args"
const std::string & getOptValue(const std::vector<std::string> & args,const std::string & opt){
      std::vector<std::string>::const_iterator iter;
      iter = find(args.begin(), args.end(),opt);
      if (iter != args.end() &&  ++iter != args.end()){
        return *iter;
      }else{
        //Dirty Code: creating a static null string and returning its referece. just because reference cannot be null.
        //TODO(Jobin): C++17 has std::optional and std::nullopt.
        static std::string nullstr{};
        return nullstr;
      }
}

//
int main(int argc, char* argv[]) {

    //Starting of commandline parsing
    //This is to accept the InputFile and log_line_prefix (lineFormat)
    string inputFileName;
    string lineFormat;
   
    //vector with command line arguments
    std::vector<std::string> args { argv, argv + argc };
 
    //check and display help
    if(argc < 3 ||chkOptExist(args,"-h")||chkOptExist(args,"--help")){
       cout<<"USAGE :\n\t"<< args[0] <<" -f postgresql.log [-p log_line_prefix]\n";
       cout<<"\t"<< args[0]<<" -h or --help  for this usage help\n";
       return 0;
    }

    if(argc >= 3 && chkOptExist(args,"-f")){ 
       inputFileName = getOptValue(args,"-f");
       std::cout<<"Input filename is : "<<inputFileName<<"\n";
    }
    if(argc >= 5 && chkOptExist(args,"-p")){
       lineFormat = getOptValue(args,"-p");
       std::cout<<"Line format is : "<<lineFormat<<"\n"; 
    } else {
        lineFormat = R"FRMT(%t [%p]: [%l-1] user=%u,db=%d)FRMT";
        cout<<"Using default log_line_prefix : " <<lineFormat<<endl;
    }
    //End of commandline parsing  

    //Open the logfile for reading.
    ifstream logfile (inputFileName, std::ifstream::in);
    
    //The Most Important Data structure to hold line parsing information. Refer Technical doc for details.
    //vector<tuple<char,int,int,char>>  loglinelocation;
    
    lineTokenizer tokenizer;
    tokenizer.genExprStr(lineFormat);
    tokenizer.logFileTokenize(logfile);
    tokenizer.printloglinelocation();
    
    //tokenizer is not needed beyond this point where it prepares the loglinelocation
    auto loglinelocation = tokenizer.getToken();
    if (loglinelocation.empty() ){
        cout<<"Exiting because unable to get the line format"<<endl;
        return 0;
    }
//End of verification of log_line_prefix.
    
    
// Show the contents of the loglinelocation
/*
    cout<<"loglinelocation content :"<<endl;
    for (auto i : loglinelocation){
        cout<<"("<< get<0>(i)<<","<<get<1>(i)<<","<<get<2>(i)<<","<<get<3>(i)<<")"<<endl;
    }
*/

//Start of Parsing

    logFileParser FileParser(logfile,loglinelocation);
    //Rest the file descriptor back to starting point of the file 
    //because tokenizer alrady advanced the file discriptor for verifying the log line
    FileParser.resetFD();
    FileParser.parse();
    FileParser.printSqlidSqlmap();

//End of Parsing
   
}


