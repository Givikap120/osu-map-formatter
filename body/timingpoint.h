#pragma once
#include <string>

class TimingPoint
{
	int time;
	double modifier;
	int meter;
	int sampleset, sampleindex;
	int volume;
	bool uninherited;
	int effects;
public:
	TimingPoint(int time_, double modifier_, int meter_ = 4, int sampleset_ = 0, int sampleindex_ = 0, 
		int volume_ = 100, bool uninherited_ = 1, int effects_ = 0);

	bool isInherited() const;
	double get_modifier() const;
	int get_time() const;

	std::string GetFormattedValue(int format, bool disable_hitsounds = 0, double bpm_mult = 1.0) const;
	std::string GetFormattedValue_InheritanceFix(int format, double last_uninherited, bool disable_hitsounds = 0, double bpm_mult = 1.0) const;
};

