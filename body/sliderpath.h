#pragma once
#include <vector>
#include <string>

struct SliderPath
{
	int slider_start_x;
	int slider_start_y;
	char curve_type;
	std::string points;

	SliderPath(int slider_start_x, int slider_start_y, char type, std::string sliderpoints);
	std::string str(int format) const;
};

