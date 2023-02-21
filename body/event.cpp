#include "event.h"

Event::Event(std::string str) : content(str)
{
}

EventComment::EventComment(std::string str) : Event(str)
{
}

EventBase::EventBase(std::string str) : Event(str)
{
}


EventSub::EventSub(std::string str) : Event(str)
{
}


Event::Type Event::GetType() const
{
	return Event::Null;
}

Event::Type EventComment::GetType() const
{
	return Event::Comment;
}

Event::Type EventBase::GetType() const
{
	return Event::Base;
}

Event::Type EventSub::GetType() const
{
	return Event::Sub;
}


EventBaseBackground::EventBaseBackground(std::string initial_str, std::string filename_, int offset_x_, int offset_y_)
	: EventBase(initial_str), filename(filename_), offset_x(offset_x_), offset_y(offset_y_)
{
}

EventBaseVideo::EventBaseVideo(std::string initial_str, int time_, std::string filename_, int offset_x_, int offset_y_)
	: EventBase(initial_str), time(time_), filename(filename_), offset_x(offset_x_), offset_y(offset_y_)
{
}

EventBaseBreak::EventBaseBreak(std::string initial_str, int start_time_, int end_time_)
	: EventBase(initial_str), start_time(start_time_), end_time(end_time_)
{
}

EventBase3::EventBase3(std::string initial_str, int time_, int color_r_, int color_g_, int color_b_)
	: EventBase(initial_str), time(time_), color_r(color_r_), color_g(color_g_), color_b(color_b_)
{
}

std::string Event::GetFormattedValue(int format) const
{
	return content + '\n';
}

std::string EventComment::GetFormattedValue(int format) const
{
	return (format >= 5 ? content + '\n' : "");
}

std::string EventBaseBackground::GetFormattedValue(int format) const
{
	std::string result = "0,0," + filename;
	if (format >= 12) result += ',' + std::to_string(offset_x) + ',' + std::to_string(offset_y);
	return result + '\n';
}

std::string EventBaseVideo::GetFormattedValue(int format) const
{
	std::string result = "1," + std::to_string(time) + ',' + filename;
	if (format >= 12) result += ',' + std::to_string(offset_x) + ',' + std::to_string(offset_y);
	return result + '\n';
}

std::string EventBaseBreak::GetFormattedValue(int format) const
{
	return std::string("2,") + std::to_string(start_time) + ',' + std::to_string(end_time) + '\n';
}

std::string EventBase3::GetFormattedValue(int format) const
{
	if (format >= 14) return "";
	return std::string("3,") + std::to_string(time) + ',' 
		+ std::to_string(color_r) + ',' + std::to_string(color_g) + ',' + std::to_string(color_b) + '\n';
}


Event::BaseType Event::GetBaseType() const
{
	return EventBase::NullBase;
}

Event::BaseType EventBaseBackground::GetBaseType() const
{
	return EventBase::Background;
}

Event::BaseType EventBaseVideo::GetBaseType() const
{
	return EventBase::Video;
}

Event::BaseType EventBaseBreak::GetBaseType() const
{
	return EventBase::Break;
}

Event::BaseType EventBase3::GetBaseType() const
{
	return EventBase::Event3;
}
