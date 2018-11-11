/*
 Author : Jobin Augustine 
*/
#include "Helper.h"

size_t normalizeSQL(std::string &str,std::map<int,std::string> & literals)
{
    const std::string demark {"+-*%=<> ;"};
    std::string literal{};      //string to hold literal
    auto cntLtrl {0};        //count of literal
    //loop though sql string
    for (auto it = str.begin(); it != str.end(); ) {
    //cleanup extra white space
    if(isspace(static_cast<unsigned char>(*it))){
        if( *it != ' ' ){
            *it = ' ';
        }
        if (isspace(static_cast<unsigned char>(*(it+1))) != 0){
            str.erase(it+1);
            continue;
        }
    } 
    //remove number literals
    else if( (isdigit(*it)) && demark.find(*(it-1)) < 8 ){
        cntLtrl++;
        while ((isdigit(*it)) || *it == '.'){
            literal.append(1,*it);
            str.erase(it);
        }
        //literals.push_back(std::move(literal));
        literals.insert(std::pair <int, std::string> (cntLtrl,std::move(literal)));
        str.insert(it,'0'+cntLtrl);
        str.insert(it,'$');
        it++;
    }
    //remove string literals
    else if( *it == '\''  &&  demark.find(*(it-1)) < 8 ){
        cntLtrl++;
        do{
            literal.append(1,*it);
            str.erase(it);
        }while(*it != '\'');
        literal.append(1,*it);
        str.erase(it);
        //literals.push_back(std::move(literal));
        literals.insert(std::pair <int, std::string> (cntLtrl,std::move(literal)));
        str.insert(it,'0'+cntLtrl);
        str.insert(it,'$');
        it++;
    }
    it++;	
   }
 
   //ltrim and rtrim
   str.erase( 0, str.find_first_not_of(' '));
   str.erase( str.find_last_not_of( " ;" ) + 1 );
   return std::hash<std::string>{}(str);
}



std::string& trim_right_inplace( std::string& s, const std::string& delimiters )
{
  return s.erase( s.find_last_not_of( delimiters ) + 1 );
}

std::string& trim_left_inplace(std::string& s, const std::string& delimiters )
{
  return s.erase( 0, s.find_first_not_of( delimiters ) );
}

std::string& trim_inplace(std::string& s, const std::string& delimiters )
{
  return trim_left_inplace( trim_right_inplace( s, delimiters ), delimiters );
}

std::string& strUpper(std::string&& str){
    for (auto & c: str) { c = toupper(c);
    }
    return str;
}

void cleanExtraWhitespace(std::string &str)
{
   for (auto it = str.begin(); it != str.end(); ) {
    if(isspace(static_cast<unsigned char>(*it))){
        if( *it != ' ' ){
            *it = ' ';
        }
        if (isspace(static_cast<unsigned char>(*(it+1)))){
            str.erase(it+1);
            continue;
        }
    }
    /*
    else if(isdigit(*it) && *(it-1) == '' ){
        while (isdigit(*it) || *it = '.'){
            
        }
    }*/
    ++it;	
   }
}