/*
 * Author: Jobin Augustine
 *
 * Created on 21 August, 2017, 4:46 PM
 */

#include<string>
#include <algorithm>
#include <map>

#ifndef HELPER_H
#define HELPER_H

size_t normalizeSQL(std::string &str,std::map<int,std::string> & literals);
std::string& trim_right_inplace( std::string& s, const std::string& delimiters = " \f\n\r\t\v" );
std::string& trim_left_inplace(std::string& s, const std::string& delimiters = " \f\n\r\t\v");
std::string& trim_inplace(std::string& s, const std::string& delimiters = " \f\n\r\t\v");
std::string& strUpper(std::string&& str);
void cleanExtraWhitespace(std::string &str);
#endif /* HELPER_H */
