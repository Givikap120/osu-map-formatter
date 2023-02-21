#pragma once
#include <string>
#include <map>
#include <vector>
#include <fstream>

#include "event.h"
#include "timingpoint.h"
#include "hitobject.h"

typedef std::map<std::string, std::string> dict;

struct ConvertSettings
{
	bool transformed;

	int inheritance_fix_point;
	int soft_sampleset_point; 
	int black_bg_point; 
	int disable_hitsounds_point;
	int disable_colors_point;
	bool disable_storyboard; 
	
	ConvertSettings(int inheritance_fix_point_ = 5,
		int soft_sampleset_point_ = 4,
		int black_bg_point_ = 7,
		int disable_hitsounds_point_ = 7,
		int disable_colors_point_ = 7,
		bool disable_storyboard_ = 1)

		: transformed(false),
		inheritance_fix_point(inheritance_fix_point_),
		soft_sampleset_point(soft_sampleset_point_),
		black_bg_point(black_bg_point_),
		disable_hitsounds_point(disable_hitsounds_point_),
		disable_colors_point(disable_colors_point_),
		disable_storyboard(disable_storyboard_)
	{}

	void transform(int format)
	{
		if (inheritance_fix_point != 1) inheritance_fix_point = format <= inheritance_fix_point ? 1 : 0;
		if (soft_sampleset_point != 1) soft_sampleset_point = format <= soft_sampleset_point ? 1 : 0;
		if (black_bg_point != 1) black_bg_point = format <= black_bg_point ? 1 : 0;
		if (disable_hitsounds_point != 1) disable_hitsounds_point = format <= disable_hitsounds_point ? 1 : 0;
		if (disable_colors_point != 1) disable_colors_point = format <= disable_colors_point ? 1 : 0;
		transformed = true;
	}
};

class Beatmap
{
	void _ImportFromFileRaw(const std::string& filename);
	std::string _ReadBlock(std::ifstream& file, std::string block_name);
	dict* _GetDictPtr(const std::string& name);

	void _ExportIn(std::ostream& output, int format, ConvertSettings settings);

	void _PrintEventsFormatted(std::ostream& output, int format, bool black_bg = 1, bool ignore_storyboard = 1);
	void _PrintTimingpointsFormatted(std::ostream& output, int format, bool disable_hitsounds = 0, bool fix_max_sv = 0);
	void _PrintTimingpointsFormatted_InheritanceFix(std::ostream& output, int format, bool disable_hitsounds = 0, bool fix_max_sv = 0);
	void _PrintHitobjectsFormatted(std::ostream& output, int format);
public:
	int format;
	dict general;
	dict editor;
	dict metadata;
	dict difficulty;
	std::vector<Event*> events;
	std::vector<TimingPoint*> timingpoints;
	dict colours;
	std::vector<HitObject*> hitobjects;

	Beatmap();
	Beatmap(std::string filename);

	void Clear();
	void ImportFromFile(std::string filename);
	void ExportToFile(std::string filename, int format, ConvertSettings settings);
	

	~Beatmap();
};

void TransformAllFilesInDirectory(std::string path, int format, ConvertSettings settings);
void CopyAndTransform(std::string input_path, std::string output_path, int format, ConvertSettings settings);

