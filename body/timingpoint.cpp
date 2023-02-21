#include "timingpoint.h"

#include <string>

#include "../util.h"

TimingPoint::TimingPoint(int time_, double modifier_, int meter_, int sampleset_, int sampleindex_, int volume_, bool uninherited_, int effects_)
	: time(time_), modifier(modifier_), meter(meter_), sampleset(sampleset_), sampleindex(sampleindex_), 
	volume(volume_), uninherited(uninherited_), effects(effects_)
{
}

bool TimingPoint::isInherited() const
{
	return !uninherited;
}

double TimingPoint::get_modifier() const
{
	return modifier;
}

int TimingPoint::get_time() const
{
	return time;
}

std::string TimingPoint::GetFormattedValue(int format, bool disable_hitsounds, double modifier_mult) const
{
	int time_fixed = time - OLD_VERSIONS_TIMING_OFFSET * (format < OFFSET_FIX_VERSION);
	std::string result;

	result = std::to_string(time_fixed) + ',' + to_string_prec(modifier_mult * modifier, 15);
	if (format >= 4) result += ',' + std::to_string(meter) + ',' + std::to_string(sampleset) + ',' + (disable_hitsounds ? "0" : std::to_string(sampleindex));
	if (format >= 5) result += ',' + std::to_string(volume) + ',' + std::to_string(uninherited) + ',' + std::to_string(effects);
	if (format < 6 and !uninherited) result = "";
	else result += '\n';

	
	return result;
}

std::string TimingPoint::GetFormattedValue_InheritanceFix(int format, double last_uninherited, bool disable_hitsounds, double modifier_mult) const
{
	int time_fixed = time - OLD_VERSIONS_TIMING_OFFSET * (format < OFFSET_FIX_VERSION);
	std::string result;

	if (!uninherited)
	{
		result = std::to_string(time_fixed) + ',' + to_string_prec(-last_uninherited * modifier_mult * modifier / 100, 15);
		if (format >= 4) result += ',' + std::to_string(meter) + ',' + std::to_string(sampleset) + ',' + (disable_hitsounds ? "0" : std::to_string(sampleindex));
		if (format >= 5) result += ',' + std::to_string(volume) + ',' + '1' + ',' + std::to_string(effects);
		result += '\n';
	}
	else {
		result = std::to_string(time_fixed) + ',' + to_string_prec(modifier_mult * modifier, 15);
		if (format >= 4) result += ',' + std::to_string(meter) + ',' + std::to_string(sampleset) + ',' + (disable_hitsounds ? "0" : std::to_string(sampleindex));
		if (format >= 5) result += ',' + std::to_string(volume) + ',' + std::to_string(uninherited) + ',' + std::to_string(effects);
		if (format < 6 and !uninherited) result = "";
		else result += '\n';
	}

	return result;
}
