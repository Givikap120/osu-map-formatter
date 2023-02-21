#include "../body/beatmap.h"

#include <sstream>

#include "../util.h"
#include "parsing_util.h"

Event* GetBackgroundPtr(std::string::const_iterator& str_ptr, std::string::const_iterator str_end)
{
	std::string buf;

	buf = ReadNextSubstr(str_ptr, str_end, ',');
	if (buf != "0") return nullptr;

	std::string filename = ReadNextSubstr(str_ptr, str_end, ',');

	buf = ReadNextSubstr(str_ptr, str_end, ',');
	int offset_x = 0;
	if (IsInt(buf)) offset_x = std::stoi(buf);

	buf = ReadNextSubstr(str_ptr, str_end, ',');
	int offset_y = 0;
	if (IsInt(buf)) offset_y = std::stoi(buf);

	return new EventBaseBackground("", filename, offset_x, offset_y);
}

Event* GetVideoPtr(std::string::const_iterator& str_ptr, std::string::const_iterator str_end, int format)
{
	std::string buf;

	buf = ReadNextSubstr(str_ptr, str_end, ',');
	int time = 0;
	if (IsInt(buf)) time = std::stoi(buf) + OLD_VERSIONS_TIMING_OFFSET * (format < OFFSET_FIX_VERSION);

	std::string filename = ReadNextSubstr(str_ptr, str_end, ',');

	buf = ReadNextSubstr(str_ptr, str_end, ',');
	int offset_x = 0;
	if (IsInt(buf)) offset_x = std::stoi(buf);

	buf = ReadNextSubstr(str_ptr, str_end, ',');
	int offset_y = 0;
	if (IsInt(buf)) offset_y = std::stoi(buf);

	return new EventBaseVideo("", time, filename, offset_x, offset_y);
}

Event* GetBreakPtr(std::string::const_iterator& str_ptr, std::string::const_iterator str_end, int format)
{
	std::string buf;

	buf = ReadNextSubstr(str_ptr, str_end, ',');
	if (!IsInt(buf)) return nullptr;
	int start_time = std::stoi(buf) + OLD_VERSIONS_TIMING_OFFSET * (format < OFFSET_FIX_VERSION);

	buf = ReadNextSubstr(str_ptr, str_end, ',');
	if (!IsInt(buf)) return nullptr;
	int end_time = std::stoi(buf) + OLD_VERSIONS_TIMING_OFFSET * (format < OFFSET_FIX_VERSION);

	return new EventBaseBreak("", start_time, end_time);
}

Event* GetEvent3Ptr(std::string::const_iterator& str_ptr, std::string::const_iterator str_end, int format)
{
	std::string buf;

	buf = ReadNextSubstr(str_ptr, str_end, ',');
	if (!IsInt(buf)) return nullptr;
	int time = std::stoi(buf) + OLD_VERSIONS_TIMING_OFFSET * (format < OFFSET_FIX_VERSION);

	buf = ReadNextSubstr(str_ptr, str_end, ',');
	if (!IsUint(buf)) return nullptr;
	int color_r = std::stoi(buf);

	buf = ReadNextSubstr(str_ptr, str_end, ',');
	if (!IsUint(buf)) return nullptr;
	int color_g = std::stoi(buf);

	buf = ReadNextSubstr(str_ptr, str_end, ',');
	if (!IsUint(buf)) return nullptr;
	int color_b = std::stoi(buf);

	return new EventBase3("", time, color_r, color_g, color_b);
}

Event* ChooseEventBase(std::string str, int format)
{
	auto str_ptr = str.begin();
	auto str_end = str.end();

	std::string identifier = ReadNextSubstr(str_ptr, str_end, ',');
	if (identifier == "0" or identifier == "Background") return GetBackgroundPtr(str_ptr, str_end);
	if (identifier == "1" or identifier == "Video") return GetVideoPtr(str_ptr, str_end, format);
	if (identifier == "2" or identifier == "Break") return GetBreakPtr(str_ptr, str_end, format);
	if (identifier == "3") return GetEvent3Ptr(str_ptr, str_end, format);
	else return new EventBase(str);
}

Event* GetEventPtrFromString(const std::string& str, int format)
{
	if (str.empty()) return nullptr;

	Event* result;
	if (str.length() > 1 and str[0] == '/' and str[1] == '/') result = new EventComment(str);
	else if (str[0] == ' ') result = new EventSub(str);
	else result = ChooseEventBase(str, format);

	return result;
}

TimingPoint* GetTimingPointPtrFromString(const std::string& str, int format)
{
	std::string buf;
	auto str_ptr = str.begin();
	auto str_end = str.end();

	buf = ReadNextSubstr(str_ptr, str_end, ',');
	if (!IsInt(buf)) return nullptr;
	int time = std::stoi(buf) + OLD_VERSIONS_TIMING_OFFSET * (format < OFFSET_FIX_VERSION);

	buf = ReadNextSubstr(str_ptr, str_end, ',');
	if (!IsFloat(buf)) return nullptr;
	double modifier = strtod_edit(buf);

	buf = ReadNextSubstr(str_ptr, str_end, ',');
	int meter = 4;
	if (IsUint(buf)) meter = std::stoi(buf);

	buf = ReadNextSubstr(str_ptr, str_end, ',');
	int sampleset = 0;
	if (IsUint(buf)) sampleset = std::stoi(buf);

	buf = ReadNextSubstr(str_ptr, str_end, ',');
	int sampleindex = 0;
	if (IsUint(buf)) sampleindex = std::stoi(buf);

	buf = ReadNextSubstr(str_ptr, str_end, ',');
	int volume = 100;
	if (IsUint(buf)) volume = std::stoi(buf);

	buf = ReadNextSubstr(str_ptr, str_end, ',');
	bool uninherited = 1;
	if (IsUint(buf)) uninherited = std::stoi(buf);

	buf = ReadNextSubstr(str_ptr, str_end, ',');
	int effects = 0;
	if (IsUint(buf)) effects = std::stoi(buf);

	return new TimingPoint(time, modifier, meter, sampleset, sampleindex, volume, uninherited, effects);
}

void ReadCustomHitsounds(const std::string& custom_hitsounds, int& normalset, int& additionset, int& index, int& volume, std::string& custom_hs_name)
{
	auto str_ptr = custom_hitsounds.begin();
	auto str_end = custom_hitsounds.end();

	std::string buf = ReadNextSubstr(str_ptr, str_end, ':');
	if (IsUint(buf)) normalset = std::stoi(buf);

	buf = ReadNextSubstr(str_ptr, str_end, ':');
	if (IsUint(buf)) additionset = std::stoi(buf);

	buf = ReadNextSubstr(str_ptr, str_end, ':');
	if (IsUint(buf)) index = std::stoi(buf);

	buf = ReadNextSubstr(str_ptr, str_end, ':');
	if (IsUint(buf)) volume = std::stoi(buf);

	custom_hs_name = ReadNextSubstr(str_ptr, str_end, ':');
}

HitObject* ContinueAsSpinner(std::string::const_iterator& str_ptr, const std::string::const_iterator& str_end,
	int time, int format)
{
	std::string buf;

	buf = ReadNextSubstr(str_ptr, str_end, ',');
	if (!IsUint(buf)) return nullptr;
	int hitsound = std::stoi(buf);

	buf = ReadNextSubstr(str_ptr, str_end, ',');
	if (!IsInt(buf)) return nullptr;
	int time_end = std::stoi(buf) + OLD_VERSIONS_TIMING_OFFSET * (format < OFFSET_FIX_VERSION);

	std::string custom_hitsounds = ReadNextSubstr(str_ptr, str_end, ',');
	int normalset = 0, additionset = 0, index = 0;
	int volume = 0;
	std::string custom_hs_name = "";
	ReadCustomHitsounds(custom_hitsounds, normalset, additionset, index, volume, custom_hs_name);

	//Spinners always have coords 256 192 and flagged as 12 (SPINNER FLAG)
	return new HitObjectSpinner(256, 192, time, SPINNER_FLAG, hitsound, time_end, normalset, additionset, index, volume, custom_hs_name);
}

HitObject* ContinueAsSlider(std::string::const_iterator& str_ptr, const std::string::const_iterator& str_end,
	int coord_x, int coord_y, int time, __int8 flags, int hitsound, std::string parameters, int format)
{
	SliderPath sliderpath(coord_x, coord_y, parameters[0], parameters.substr(1, parameters.length() - 1));

	std::string buf = ReadNextSubstr(str_ptr, str_end, ',');
	if (!IsUint(buf)) return nullptr;
	int slides = std::stoi(buf);

	buf = ReadNextSubstr(str_ptr, str_end, ',');
	if (!IsUfloat(buf)) return nullptr;
	double length = strtod_edit(buf);

	std::string edgesounds = ReadNextSubstr(str_ptr, str_end, ',');

	std::string edgesets = ReadNextSubstr(str_ptr, str_end, ',');

	std::string custom_hitsounds = ReadNextSubstr(str_ptr, str_end, ',');
	int normalset = 0, additionset = 0, index = 0;
	int volume = 0;
	std::string custom_hs_name = "";
	ReadCustomHitsounds(custom_hitsounds, normalset, additionset, index, volume, custom_hs_name);

	return new HitObjectSlider(coord_x, coord_y, time, flags, hitsound, sliderpath, slides, length, edgesounds, edgesets,
		normalset, additionset, index, volume, custom_hs_name);
}

HitObject* ContinueAsCircle(std::string::const_iterator& str_ptr, const std::string::const_iterator& str_end,
	int coord_x, int coord_y, int time, __int8 flags, int hitsound, std::string parameters, int format)
{
	std::string custom_hitsounds = ReadNextSubstr(str_ptr, str_end, ',');
	int normalset = 0, additionset = 0, index = 0;
	int volume = 0;
	std::string custom_hs_name = "";
	ReadCustomHitsounds(custom_hitsounds, normalset, additionset, index, volume, custom_hs_name);

	return new HitObjectCircle(coord_x, coord_y, time, flags, hitsound, normalset, additionset, index, volume, custom_hs_name);
}

HitObject* GetHitObjectPtrFromString(const std::string& str, int format)
{
	std::string buf;
	auto str_ptr = str.begin();
	auto str_end = str.end();

	buf = ReadNextSubstr(str_ptr, str_end, ',');
	if (!IsUint(buf)) return nullptr;
	int coord_x = std::stoi(buf);

	buf = ReadNextSubstr(str_ptr, str_end, ',');
	if (!IsUint(buf)) return nullptr;
	int coord_y = std::stoi(buf);

	buf = ReadNextSubstr(str_ptr, str_end, ',');
	if (!IsInt(buf)) return nullptr;
	int time = std::stoi(buf) + OLD_VERSIONS_TIMING_OFFSET * (format < OFFSET_FIX_VERSION);

	buf = ReadNextSubstr(str_ptr, str_end, ',');
	if (!IsUint(buf)) return nullptr;
	int flags = std::stoi(buf);
	if (!HitObject::IsValidFlag(flags)) return nullptr;
	//Checking for spinner
	if (flags == SPINNER_FLAG) return ContinueAsSpinner(str_ptr, str_end, time, format);

	buf = ReadNextSubstr(str_ptr, str_end, ',');
	if (!IsUint(buf)) return nullptr;
	int hitsound = std::stoi(buf);

	buf = ReadNextSubstr(str_ptr, str_end, ',');

	//Choosing slider or circle
	if (!buf.empty() and HitObject::IsValidSliderLetter(buf[0], format))
		return ContinueAsSlider(str_ptr, str_end, coord_x, coord_y, time, flags, hitsound, buf, format);
	else return ContinueAsCircle(str_ptr, str_end, coord_x, coord_y, time, flags, hitsound, buf, format);
}

void Beatmap::_ImportFromFileRaw(const std::string& filename)
{
	std::ifstream file(filename);
	std::string buf;

	std::getline(file, buf); format = GetFileFormat(buf);

	buf = SkipTillFirstBlock(file);
	while (!buf.empty())
	{
		buf = _ReadBlock(file, buf);
	}

	file.close();
}

std::string Beatmap::_ReadBlock(std::ifstream& file, std::string block_name)
{
	std::string buf;
	block_name = block_name.substr(1, block_name.length() - 2);

	dict* dict_ptr = _GetDictPtr(block_name);
	if (dict_ptr)
	{
		std::pair<std::string, std::string> dict_pair;
		bool ok;

		while (true) {
			std::getline(file, buf);
			if (file.eof() or IsBlockName(buf)) break;

			dict_pair = GetDictPair(buf, ok);
			if (ok) dict_ptr->insert(dict_pair);
		}

		if (block_name == "Difficulty" and
			dict_ptr->find("OverallDifficulty") != dict_ptr->end() and
			dict_ptr->find("ApproachRate") == dict_ptr->end())
			dict_ptr->insert({ "ApproachRate", dict_ptr->at("OverallDifficulty") });

		return buf;
	}
	else {
		if (block_name == "Events") {
			Event* temp;
			while (true) {
				std::getline(file, buf);
				if (file.eof() or IsBlockName(buf)) return buf;
				temp = GetEventPtrFromString(buf, format);
				if (temp) events.push_back(temp);
			}
		}
		else if (block_name == "TimingPoints") {
			TimingPoint* temp;
			while (true) {
				std::getline(file, buf);
				if (file.eof() or IsBlockName(buf)) return buf;
				temp = GetTimingPointPtrFromString(buf, format);
				if (temp) timingpoints.push_back(temp);
			}
		}
		if (block_name == "HitObjects") {
			HitObject* temp;
			while (true) {
				std::getline(file, buf);
				if (file.eof() or IsBlockName(buf)) return buf;
				temp = GetHitObjectPtrFromString(buf, format);
				if (temp) hitobjects.push_back(temp);
			}
		}
		else while (true) {
			std::getline(file, buf);
			if (file.eof() or IsBlockName(buf)) return buf;
		}
	}
}