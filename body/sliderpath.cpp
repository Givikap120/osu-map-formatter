#include "sliderpath.h"

SliderPath::SliderPath(int start_x, int start_y, char type, std::string sliderpoints) 
	: slider_start_x(start_x), slider_start_y(start_y), curve_type(type), points(sliderpoints)
{
}

std::string SliderPath::str(int format) const
{
	std::string result;

	if (format <= 9 and curve_type == 'P') result = "B";
	else result = curve_type;

	if (format <= 4) result += '|' + std::to_string(slider_start_x) + ':' + std::to_string(slider_start_y);
	result += points;

	return result;
}
