#pragma once
#include <string>

#include "sliderpath.h"

class HitObject
{
protected:
	int coord_x, coord_y;
	int time;
	__int8 flags;
	int hitsound;
	int normalset, additionset, index;
	int volume;
	std::string custom_hs_name;
public:
	virtual std::string GetFormattedValue(int format) const;

	static bool IsValidFlag(int flags);
	static bool IsValidSliderLetter(char letter, int format);

	HitObject(int coord_x_, int coord_y_, int time_, __int8 flags_, int hitsound_,
		int normalset_ = 0, int additionset_ = 0, int index_ = 0, int volume_ = 0, std::string custom_hs_name_ = "");
};

class HitObjectCircle : public HitObject
{
public:
	std::string GetFormattedValue(int format) const override;

	HitObjectCircle(int coord_x_, int coord_y_, int time_, __int8 flags_, int hitsound_,
		int normalset_ = 0, int additionset_ = 0, int index_ = 0, int volume_ = 0, std::string custom_hs_name_ = "");
};

class HitObjectSlider : public HitObject
{
	SliderPath sliderpath;
	int slides;
	double length;
	std::string edgesounds, edgesets;

public:
	std::string GetFormattedValue(int format) const override;

	HitObjectSlider(int coord_x_, int coord_y_, int time_, __int8 flags_, int hitsound_, 
		SliderPath sliderpath_, int slides_, double length_, std::string edgesounds_, std::string edgesets_,
		int normalset_ = 0, int additionset_ = 0, int index_ = 0, int volume_ = 0, std::string custom_hs_name_ = "");
};

class HitObjectSpinner : public HitObject
{
	int time_end;
public:
	std::string GetFormattedValue(int format) const override;

	HitObjectSpinner(int coord_x_, int coord_y_, int time_, __int8 flags_, int hitsound_, int _time_end,
		int normalset_ = 0, int additionset_ = 0, int index_ = 0, int volume_ = 0, std::string custom_hs_name_ = "");
};




