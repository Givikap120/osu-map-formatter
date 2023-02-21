#include "hitobject.h"

#include "../util.h"

bool HitObject::IsValidFlag(int flags)
{
    return true;
}

bool HitObject::IsValidSliderLetter(char letter, int format)
{
    if (letter == 'B' or letter == 'L' or letter == 'C') return true;
    if (letter == 'P' and format > 9) return true;
    return false;
}


HitObject::HitObject(int coord_x_, int coord_y_, int time_, __int8 flags_, int hitsound_, 
    int normalset_, int additionset_, int index_, int volume_, std::string custom_hs_name_)
    : coord_x(coord_x_), coord_y(coord_y_), time(time_), flags(flags_), hitsound(hitsound_), 
    normalset(normalset_), additionset(additionset_), index(index_), volume(volume_), custom_hs_name(custom_hs_name_)
{
}

HitObjectCircle::HitObjectCircle(int coord_x_, int coord_y_, int time_, __int8 flags_, int hitsound_,
    int normalset_, int additionset_, int index_, int volume_, std::string custom_hs_name_)
    : HitObject(coord_x_, coord_y_, time_, flags_, hitsound_, normalset_, additionset_, index_, volume_, custom_hs_name_)
{
}

HitObjectSlider::HitObjectSlider(int coord_x_, int coord_y_, int time_, __int8 flags_, int hitsound_,
    SliderPath sliderpath_, int slides_, double length_, std::string edgesounds_, std::string edgesets_, 
    int normalset_, int additionset_, int index_, int volume_, std::string custom_hs_name_)
    : HitObject(coord_x_, coord_y_, time_, flags_, hitsound_, normalset_, additionset_, index_, volume_, custom_hs_name_),
    sliderpath(sliderpath_), slides(slides_), length(length_), edgesounds(edgesounds_), edgesets(edgesets_)
{
}

HitObjectSpinner::HitObjectSpinner(int coord_x_, int coord_y_, int time_, __int8 flags_, int hitsound_, int _time_end,
    int normalset_, int additionset_, int index_, int volume_, std::string custom_hs_name_)
    : HitObject(coord_x_, coord_y_, time_, flags_, hitsound_, normalset_, additionset_, index_, volume_, custom_hs_name_),
    time_end(_time_end)
{
}

std::string HitObject::GetFormattedValue(int format) const
{
    return "ERROR: Hit Object not determined (format = v"+std::to_string(format)+")";
}

std::string HitObjectCircle::GetFormattedValue(int format) const
{
    int time_fixed = time - OLD_VERSIONS_TIMING_OFFSET * (format < OFFSET_FIX_VERSION);

    std::string result = std::to_string(coord_x) + ',' + std::to_string(coord_y)
        + ',' + std::to_string(time_fixed) + ',' + std::to_string(flags) + ',' + std::to_string(hitsound);
    if (format < 4) result += ',';
    if (format >= 10) result += ',' + std::to_string(normalset) + ':' + std::to_string(additionset) + ':' + std::to_string(index);
    if (format >= 12) result += ':' + std::to_string(volume) + ':' + custom_hs_name;

    return result + '\n';
}

std::string HitObjectSlider::GetFormattedValue(int format) const
{
    int time_fixed = time - OLD_VERSIONS_TIMING_OFFSET * (format < OFFSET_FIX_VERSION);

    std::string result = std::to_string(coord_x) + ',' + std::to_string(coord_y)
        + ',' + std::to_string(time_fixed) + ',' + std::to_string(flags) + ',' + std::to_string(hitsound)
        + ',' + sliderpath.str(format) + ',' + std::to_string(slides) + ',' + to_string_prec(length, 15);
    if (format >= 6 and !edgesounds.empty()) result += ',' + edgesounds;
    if (format >= 10) result += (!edgesets.empty() ? "," + edgesets : "")
        + ',' + std::to_string(normalset) + ':' + std::to_string(additionset) + ':' + std::to_string(index);
    if (format >= 12) result += ':' + std::to_string(volume) + ':' + custom_hs_name;

    return result + '\n';
}

std::string HitObjectSpinner::GetFormattedValue(int format) const
{
    int time_fixed = time - OLD_VERSIONS_TIMING_OFFSET * (format < OFFSET_FIX_VERSION);
    int time_end_fixed = time_end - OLD_VERSIONS_TIMING_OFFSET * (format < OFFSET_FIX_VERSION);

    std::string result = std::to_string(coord_x) + ',' + std::to_string(coord_y)
        + ',' + std::to_string(time_fixed) + ',' + std::to_string(flags) + ',' + std::to_string(hitsound) + ',' + std::to_string(time_end_fixed);
    if (format >= 10) result += ',' + std::to_string(normalset) + ':' + std::to_string(additionset) + ':' + std::to_string(index);
    if (format >= 12) result += ':' + std::to_string(volume) + ':' + custom_hs_name;

    return result + '\n';
}
