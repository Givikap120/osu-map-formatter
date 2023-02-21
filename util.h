#pragma once
#include <string>
#include <map>

const int OLD_VERSIONS_TIMING_OFFSET = 24;
const int OFFSET_FIX_VERSION = 5;
const int SPINNER_FLAG = 12;
const double OLD_VERSIONS_MAX_SV = 2.6;

//converts double to string with given precision
std::string to_string_prec(double d, int precision);
/*safe version of std::map.at
returns "" or "0" if no such element*/
std::string map_safe_at(std::map<std::string, std::string>& m, std::string key, bool is_number = false);

bool IsUint(const std::string& str);
bool IsInt(const std::string& str);
bool IsUfloat(const std::string& str);
bool IsFloat(const std::string& str);

//Converts string to double (edited version of strtod)
double strtod_edit(const char* nptr);
//Converts string to double (edited version of strtod)
inline double strtod_edit(const std::string& str)
{ return strtod_edit(str.c_str()); }