# pgLogAnalyser
Modern C++ framework and program to analyze postgres logs. Speed matters.
This designed to be modular so that the parts of the code can be reused.
Used only C++ STLs like regex,vector,tuple etc for highest portability.

## Features
* Created as a generic Simple example of Parsing log file of Postgres.
* Code base can be reused in other application.
* Regular expression is used only for initial detection and verification of log line
* Parsing of log file faster string algorithms (currently work in progress)

## Installation
The system Need to have a compiler with Standard C++11 support to build the source code. GCC 6+ or Clang/LLVM version 8+ is recommended.  
Go to directory and just run:  
```./make``` 
</br>After building the root folder you may build the Unit Test codes

```
cd unittest/
./make
```
Running the UTParser can demonstrage how sample files are parsed and contents are extracted


## Usage 
```./pgloganalyzer -f postgres.log```  
or  
```./pgloganalyzer -f ~/postgres.log -p "%t [%p]: [%l-1] user=%u,db=%d,host=%h"```


## Technical Doc
  
The program flow is designed as follows:
1. log_line_prefix is converted into Regular Expression (Comptiable with ECMAScript).
2. Use C++11 standard regular expression libraries for parsing first few lines of the log file.
3. If the Regular expression is successful in finding the matches, A data structure with location and offsets are created for further parsing the log file in a much faster and efficient way. This datastructure (**loglinelocation**) is described in the following sections
4. Start parsing the log file from the biggining till the end.

The Important Data structure in the entire framework is: 

```c++
vector<tuple<char,int,int,char>> loglinelocation;
```

This is a representation of regular expression for parsing the log lines.

### **loglinelocation** explanation:
1. First character of the tuple indicates the the log\_line\_prefix escape character like 't' for '%t', 'p' for '%p' etc.
     Please refer [Postgres documentation here](https://www.postgresql.org/docs/9.5/static/runtime-config-logging.html#GUC-LOG-LINE-PREFIX) for details of escape sequences used in postgres log.
2. Second integer is relative position of the escape character, if log\_line\_prefix is like:
       
   ```log_line_prefix = '%t [%p]: [%l-1] user=%u,db=%d,app=%a,client=%h '```
       
   escape character 't' comes first, 'p' comes second, 'l' comes thired and so on.
     
3. Third integer indicates the intial offset of the starting position from the previous data.
     in the above example immediately after %t, there is a space and '[' before %p, so inital offset becomes 2 for 'p'
     similarly there is ']',':',Space and '[' before %l. so inital offset becomes 4.

4. last char is the termination marker character.
     
     after %t, there is a space, so space becomes termination marker for 't'.
after %p, there is a ']', so ']' becomes the termination marker for 'p'

Enumerations are used for statement types
1 - Select, 2 - Insert, 3 - Update, 4 - Delete, 5 - Copy, 6-Commit, 7-Vacuum, 8 - With
21 - Drop, 22 - Alter, 23 - Create,
41 - Unknown 
