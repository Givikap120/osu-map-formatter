#pragma once
#include <string>

//reads osu file format from string with correctness check
int GetFileFormat(std::string str);
//reads and ignores all lines until [*Name*] is found
std::string SkipTillFirstBlock(std::ifstream& file);
//converts string "Key:Element" to std::pair<>(Key,Element)
std::pair<std::string, std::string> GetDictPair(std::string input, bool& ok);
//reads substring until it reaches the dividor, returns read substr
std::string ReadNextSubstr(std::string::const_iterator& iter, std::string::const_iterator end, char dividor);


inline bool IsBlockName(const std::string& str)
{ return str[0] == '[' and str[str.length() - 1] == ']'; }

