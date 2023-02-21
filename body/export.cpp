#include "beatmap.h"

#include <fstream>
#include <vector>

#include "../util.h"


struct DictElement {
	//dict* dict_ptr;
	std::string key;
	int format_start, format_end;
	bool is_required;
	std::string default_value;

	std::string str(std::string value, int format) const
	{
		if (format >= format_start and format <= format_end)
		{
			if (value.empty())
			{
				if (is_required) return key + ": " + default_value + "\n";
				else return "";
			}
			else return key + ": " + value + "\n";
		}
		else return "";
	}
};

const std::vector<DictElement> general_names
{
	{"AudioFilename",INT_MIN, INT_MAX, true, "ERROR"},
	{"AudioLeadIn", 5, INT_MAX, true, "0"},
	{"AudioHash",INT_MIN, 4, false, ""},
	{"PreviewTime",INT_MIN, INT_MAX, true, "0"},
	{"Countdown", 5, INT_MAX, true, "-1"},
	{"SampleSet",INT_MIN, INT_MAX, true, "Soft"},
	{"StackLeniency",5,INT_MAX,true, "0.7"},
	{"Mode",5,INT_MAX,true, "0"},
	{"LetterboxInBreaks",5,INT_MAX,true, "0"},
	{"EditorDistanceSpacing",5,5,false, "1"},
	{"WidescreenStoryboard",12,INT_MAX,true, "0"}
};

void PrintGeneral(std::ostream& output, Beatmap* bm, int format, bool soft_sampleset)
{
	output << "\n[General]\n";

	for (auto i : general_names)
	{
		if (format < OFFSET_FIX_VERSION and i.key == "PreviewTime")
			output << i.key + ": " << std::stoi(map_safe_at(bm->general, i.key, 1)) + OLD_VERSIONS_TIMING_OFFSET << "\n";
		else if(soft_sampleset and i.key == "SampleSet") output << i.str(map_safe_at(bm->general, "Soft"), format);
		else output << i.str(map_safe_at(bm->general, i.key), format);
	}
}

const std::vector<DictElement> editor_names
{
	{"Bookmarks",INT_MIN, INT_MAX, false, ""},
	{"DistanceSpacing", INT_MIN, INT_MAX, false, "1"},
	{"BeatDivisor",INT_MIN, INT_MAX, false, "4"},
	{"GridSize",INT_MIN, INT_MAX, false, "16"},
	{"TimelineZoom",13, INT_MAX, false, "1"}
};

void PrintEditor(std::ostream& output, Beatmap* bm, int format)
{
	output << "\n[Editor]\n";

	for (auto i : editor_names)
		output << i.str(map_safe_at(bm->editor, i.key), format);
}

const std::vector<DictElement> metadata_names
{
	{"Title",INT_MIN, INT_MAX, true, "ERROR"},
	{"TitleUnicode", 10, INT_MAX, false, ""},
	{"Artist",INT_MIN, INT_MAX, true, "ERROR"},
	{"ArtistUnicode",10, INT_MAX, false, ""},
	{"Creator", INT_MIN, INT_MAX, true, "ERROR"},
	{"Version",INT_MIN, INT_MAX, true, "ERROR"},
	{"Source",5,INT_MAX,true, ""},
	{"Tags",5,INT_MAX,true, ""},
	{"BeatmapID",10,INT_MAX,true, "0"},
	{"BeatmapSetID",10,INT_MAX,true, "-1"},
};

void PrintMetadata(std::ostream& output, Beatmap* bm, int format)
{
	output << "\n[Metadata]\n";

	for (auto i : metadata_names)
	{
		output << i.str(map_safe_at(bm->metadata, i.key), format);
	}
}

const std::vector<DictElement> difficulty_names
{
	{"HPDrainRate",INT_MIN, INT_MAX, true, "5"},
	{"CircleSize", INT_MIN, INT_MAX, true, "4"},
	{"ApproachRate",8, INT_MAX, true, "9"},
	{"OverallDifficulty",INT_MIN, INT_MAX, true, "8"},
	{"SliderMultiplier", INT_MIN, INT_MAX, true, "1.8"},
	{"SliderTickRate",INT_MIN, INT_MAX, true, "1"}
};

void PrintDifficulty(std::ostream& output, Beatmap* bm, int format, bool fix_max_sv)
{
	output << "\n[Difficulty]\n";

	std::string hp = map_safe_at(bm->difficulty, difficulty_names[0].key, 1),
		cs = map_safe_at(bm->difficulty, difficulty_names[1].key, 1),
		ar = map_safe_at(bm->difficulty, difficulty_names[2].key, 1),
		od = map_safe_at(bm->difficulty, difficulty_names[3].key, 1);

	if (format < 13)
	{
		if(!hp.empty()) hp = std::to_string((int)strtod_edit(hp));
		if (!cs.empty()) cs = std::to_string((int)strtod_edit(cs));
		if (!od.empty()) od = std::to_string((int)std::round(strtod_edit(od)));
		if (!ar.empty()) ar = std::to_string((int)std::round(strtod_edit(ar)));
	}
	if (format < 8) od = ar;

	for (auto i : difficulty_names)
	{
		if (i.key == "HPDrainRate") output << i.str(hp, format);
		else if (i.key == "CircleSize") output << i.str(cs, format);
		else if (i.key == "OverallDifficulty") output << i.str(od, format);
		else if (i.key == "ApproachRate") output << i.str(ar, format);
		else if (i.key == "SliderMultiplier" and fix_max_sv) output << "SliderMultiplier: " << strtod_edit(map_safe_at(bm->difficulty, i.key, 1)) / 2 << '\n';
		else output << i.str(map_safe_at(bm->difficulty, i.key), format);
	}
}

void PrintColours(std::ostream& output, Beatmap* bm, int format)
{
	output << "\n[Colours]\n";
	int counter = 1;
	std::string current_color_name = "Combo1 ";
	while (bm->colours.find(current_color_name) != bm->colours.end())
	{
		output << current_color_name << ' ' << map_safe_at(bm->colours, current_color_name);
		++counter;
		current_color_name = "Combo" + std::to_string(counter) + " ";
	}
}

void Beatmap::_PrintEventsFormatted(std::ostream& output, int format, bool black_bg, bool ignore_storyboard)
{
	output << "\n[Events]\n";

	for (auto i : events)
		switch (i->GetType())
		{
		case Event::Comment:
			if (format < 5) break;
			output << i->GetFormattedValue();
			break;
		case Event::Base:

			switch (i->GetBaseType())	
			{
			case Event::NullBase:
				if (ignore_storyboard or format < 5) break;
				output << i->GetFormattedValue();
				break;
			case Event::Background:
				if (black_bg) { output << "0,0,\"blackbg.jpg\"" << (format >= 12 ? ",0,0\n" : "\n"); break; }
				output << i->GetFormattedValue();
				break;
			default:
				output << i->GetFormattedValue();
			}
			break;
		case Event::Sub:
			if (ignore_storyboard or format < 5) break;
			output << i->GetFormattedValue();
		}
}

void Beatmap::_PrintTimingpointsFormatted(std::ostream& output, int format, bool disable_hitsounds, bool fix_max_sv)
{
	output << "\n[TimingPoints]\n";

	for (auto i : timingpoints) {
		output << i->GetFormattedValue(format, disable_hitsounds, 1.0 - 0.5 * fix_max_sv);
	}
}

void Beatmap::_PrintTimingpointsFormatted_InheritanceFix(std::ostream& output, int format, bool disable_hitsounds, bool fix_max_sv)
{
	output << "\n[TimingPoints]\n";

	double last_bpm = 0;;
	for (auto i : timingpoints)
		if (!i->isInherited()) {
			last_bpm = i->get_modifier();
			break;
		}

	auto ptr_current = timingpoints.begin();
	const auto ptr_last = timingpoints.end();

	if (ptr_current == ptr_last) return;
	auto ptr_next = ptr_current + 1;

	while (ptr_next != ptr_last) {
		if ((*ptr_current)->get_time() == (*ptr_next)->get_time() and (*ptr_current)->isInherited() != (*ptr_next)->isInherited())
		{
			TimingPoint* inherited;
			TimingPoint* uninherited;
			if ((*ptr_current)->isInherited())
			{
				inherited = *ptr_current;
				uninherited = *ptr_next;
			}
			else
			{
				uninherited = *ptr_current;
				inherited = *ptr_next;
			}

			last_bpm = uninherited->get_modifier();
			output << inherited->GetFormattedValue_InheritanceFix(format, last_bpm, disable_hitsounds, 1.0 - 0.5 * fix_max_sv);
			++ptr_current; ++ptr_next;
		}
		else 
		{
			if (!(*ptr_current)->isInherited()) last_bpm = (*ptr_current)->get_modifier();
			output << (*ptr_current)->GetFormattedValue_InheritanceFix(format, last_bpm, disable_hitsounds, 1.0 - 0.5 * fix_max_sv);
		}

		++ptr_current; ++ptr_next;
	}
	output << (*ptr_current)->GetFormattedValue_InheritanceFix(format, last_bpm, disable_hitsounds, 1.0 - 0.5 * fix_max_sv);
}

void Beatmap::_PrintHitobjectsFormatted(std::ostream& output, int format)
{
	output << "\n[HitObjects]\n";

	for (auto i : hitobjects)
		output << i->GetFormattedValue(format);
}

void Beatmap::_ExportIn(std::ostream& output, int format, ConvertSettings settings)
{
	output << "osu file format v" << format << "\n";

	std::string bm_sv_mult = map_safe_at(difficulty, "SliderMultiplier");
	bool fix_max_sv = (format < 7 and IsFloat(bm_sv_mult) and strtod_edit(bm_sv_mult) > OLD_VERSIONS_MAX_SV);


	PrintGeneral(output, this, format, settings.soft_sampleset_point);
	if (format >= 6) PrintEditor(output, this, format);
	PrintMetadata(output, this, format);
	PrintDifficulty(output, this, format, fix_max_sv);

	_PrintEventsFormatted(output, format, settings.black_bg_point, settings.disable_storyboard);
	settings.inheritance_fix_point ?
		_PrintTimingpointsFormatted_InheritanceFix(output, format, settings.disable_hitsounds_point, fix_max_sv)
		: _PrintTimingpointsFormatted(output, format, settings.disable_hitsounds_point, fix_max_sv);

	if (!settings.disable_colors_point and format >= 5) PrintColours(output, this, format);
	_PrintHitobjectsFormatted(output, format);
}