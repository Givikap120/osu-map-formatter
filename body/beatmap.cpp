#include "beatmap.h"

#include <sstream>
#include <algorithm>
#include <iostream>
#include <filesystem>

#include "../util.h"

#define fs std::filesystem

Beatmap::Beatmap()
	: format(-1) 
{
}

Beatmap::Beatmap(std::string filename) 
{ 
	_ImportFromFileRaw(filename);
}

void Beatmap::Clear()
{
	format = -1;

	std::for_each(events.begin(), events.end(), [](Event* ptr) { delete ptr; });
	std::for_each(timingpoints.begin(), timingpoints.end(), [](TimingPoint* ptr) { delete ptr; });
	std::for_each(hitobjects.begin(), hitobjects.end(), [](HitObject* ptr) { delete ptr; });

	general.clear();
	editor.clear();
	metadata.clear();
	difficulty.clear();
	events.clear();
	timingpoints.clear();
	hitobjects.clear();
}

void Beatmap::ImportFromFile(std::string filename)
{ 
	Clear(); 
	_ImportFromFileRaw(filename); 
}

void Beatmap::ExportToFile(std::string filename, int format, ConvertSettings settings)
{
	settings.transform(format);

	std::ofstream file(filename);
	_ExportIn(file, format, settings);
	file.close();
}

dict* Beatmap::_GetDictPtr(const std::string& name)
{
	if (name == "General") return &general;
	if (name == "Editor") return &editor;
	if (name == "Metadata") return &metadata;
	if (name == "Difficulty") return &difficulty;
	if (name == "Colours") return &colours;
	else return nullptr;
}

Beatmap::~Beatmap()
{
	std::for_each(events.begin(), events.end(), [](Event* ptr) { delete ptr; });
	std::for_each(timingpoints.begin(), timingpoints.end(), [](TimingPoint* ptr) { delete ptr; });
	std::for_each(hitobjects.begin(), hitobjects.end(), [](HitObject* ptr) { delete ptr; });
}

void TransformAllFilesInDirectory(std::string path, int format, ConvertSettings settings)
{
	settings.transform(format);

	fs::path black_bg_path = fs::current_path().concat("\\blackbg.jpg");

	if (settings.black_bg_point and !fs::exists(black_bg_path) and settings.black_bg_point)
	{
		std::cout << "ERROR: NO BLACK BG FILE\n";
		return;
	}

	fs::path work_path(path);

	fs::path new_file_path;

	Beatmap bm;

	if (work_path.has_filename())
	{
		bm.ImportFromFile(work_path.string());
		bm.ExportToFile(work_path.string(), format, settings);
		new_file_path = fs::absolute(work_path).parent_path().concat("\\blackbg.jpg");
		if (settings.black_bg_point and !fs::exists(new_file_path)) fs::copy_file(black_bg_path, new_file_path);
		bm.Clear();

		std::cout << work_path << " was succesfuly converted\n";
	}

	else for (auto i : fs::recursive_directory_iterator(work_path))
		if (i.path().extension() == ".osu") {
			bm.ImportFromFile(i.path().string());
			bm.ExportToFile(i.path().string(), format, settings);
			new_file_path = i.path().parent_path().concat("\\blackbg.jpg");
			if(settings.black_bg_point and !fs::exists(new_file_path)) fs::copy_file(black_bg_path, new_file_path);
			bm.Clear();

			std::cout << i.path() << " was succesfuly converted\n";
		}
}

void CopyAndTransform(std::string input_path, std::string output_path, int format, ConvertSettings settings)
{
	fs::path from(input_path), to(output_path);

	try {
		fs::copy(from, to, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
		TransformAllFilesInDirectory(output_path, format, settings);
	}
	catch (std::exception e)
	{
		std::cout << "ERROR: CAN'T USE GIVEN FILES/DIRECTORIES\n";
		std::cout << "EXCEPTION CODE: \"" << e.what() << "\"\n";
	}
}
