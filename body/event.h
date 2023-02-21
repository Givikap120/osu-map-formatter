#pragma once
#include <string>



class Event
{
protected:
	std::string content;
public:
	enum Type {
		Null,
		Comment,
		Base,
		Sub
	};

	enum BaseType {
		NullBase,
		Background,
		Video,
		Break,
		Event3
	};

	Event(std::string str);

	virtual std::string GetFormattedValue(int format = 0) const;
	virtual Type GetType() const;
	virtual BaseType GetBaseType() const;
};

class EventComment : public Event
{
public:
	EventComment(std::string str);

	Type GetType() const override;

	std::string GetFormattedValue(int format) const override;
};

class EventBase : public Event
{
public:
	

	EventBase(std::string str);

	Type GetType() const override;
};

class EventBaseBackground : public EventBase
{
	std::string filename;
	int offset_x, offset_y;
public:
	EventBaseBackground(std::string initial_str, std::string filename, int offset_x, int offset_y);

	std::string GetFormattedValue(int format) const override;
	BaseType GetBaseType() const override;
};

class EventBaseVideo : public EventBase
{
	int time;
	std::string filename;
	int offset_x, offset_y;
public:
	EventBaseVideo(std::string initial_str, int time, std::string filename, int offset_x, int offset_y);

	std::string GetFormattedValue(int format) const override;
	BaseType GetBaseType() const override;
};

class EventBaseBreak : public EventBase
{
	int start_time;
	int end_time;
public:
	EventBaseBreak(std::string initial_str, int start_time, int end_time);

	std::string GetFormattedValue(int format) const override;
	BaseType GetBaseType() const override;
};

class EventBase3 : public EventBase
{
	int time;
	int color_r, color_g, color_b;
public:
	EventBase3(std::string initial_str, int time, int color_r, int color_g, int color_b);

	std::string GetFormattedValue(int format) const override;
	BaseType GetBaseType() const override;
};

//class EventBaseSprite : public EventBase
//{
//
//};
//
//class EventBaseAnimation : public EventBase
//{
//
//};
//
//class EventBaseSample : public EventBase
//{
//
//};

class EventSub : public Event
{
public:
	EventSub(std::string str);

	Type GetType() const override;
};

