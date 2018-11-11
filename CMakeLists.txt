cmake_minimum_required (VERSION 2.6)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLASGS} -std=c++14 -g -Wall")
project (pgloganalyzer)
add_executable(pgloganalyzer main.cpp Helper.cpp logFileParser.cpp lineTokenizer.cpp)
