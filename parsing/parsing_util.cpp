#include "parsing_util.h"

#include <sstream>
#include <fstream>

#include "../util.h"

int GetFileFormat(std::string str)
{
	std::stringstream ss(str);
	std::string buf;

	ss >> buf;
	if (buf != "osu") return -1;

	ss >> buf;
	if (buf != "file") return -1;

	ss >> buf;
	if (buf != "format") return -1;

	ss >> buf;
	if (buf[0] != 'v') return -1;
	buf = buf.substr(1, buf.length() - 1);
	if (!IsUint(buf)) return -1;
	else return std::stoi(buf);
}

std::string SkipTillFirstBlock(std::ifstream& file)
{
	std::string buf;
	if (!file.is_open()) return "";
	do {
		std::getline(file, buf);
	} while (!file.eof() and !IsBlockName(buf));
	return buf;
}

std::pair<std::string, std::string> GetDictPair(std::string input, bool& ok)
{
	ok = false;

	auto double_dot_pos = std::find(input.begin(), input.end(), ':');
	if (double_dot_pos == input.end()) return std::pair<std::string, std::string>();

	std::string key, value;
	key = input.substr(0, double_dot_pos - input.begin());

	while (double_dot_pos != input.end()) {
		if (*double_dot_pos != ' ' and *double_dot_pos != ':') break;
		++double_dot_pos;
	}

	value = input.substr(double_dot_pos - input.begin(), input.end() - double_dot_pos);

	ok = true;
	return std::pair<std::string, std::string>(key, value);
}

std::string ReadNextSubstr(std::string::const_iterator& iter, std::string::const_iterator end, char dividor)
{
	std::string result = "";
	while (iter != end and *iter != dividor)
	{
		result += *iter;
		++iter;
	}
	if (iter != end) ++iter;
	return result;
}