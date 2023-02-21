//osu-map-formatter by givikap120
//version 0.1, 04.02.23
//boost required

#include <iostream>
#include "body/beatmap.h"
#include <boost/program_options.hpp>

namespace po = boost::program_options;

inline ConvertSettings GetSettings(po::variables_map& vm)
{
    return { vm["inhertitancefix"].as<int>(),
    vm["softsampleset"].as<int>(),
    vm["blackbg"].as<int>(),
    vm["disablehitsample"].as<int>(),
    vm["disablecolors"].as<int>(),
    !vm["enablestoryboard"].as<bool>()
    };
}

int main(int ac, char** av)
{
    std::setlocale(LC_ALL, "");

    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("in", po::value<std::string>(), "file/directory to convert")
        ("out", po::value<std::string>(), "(optional) outputs in different (from input) file/directory")
        ("dir", po::value<std::vector<std::string>>(), "(optional) you can skip \"in\" and \"out\" keywords, those 1 or 2 arguments would be treated as \"in\" and \"out\"")
        ("format,f", po::value<int>(), "format to convert (from 3 to 14)")
        ("inhertitancefix,i", po::value<int>()->default_value(5), "transforms all inherited timing points into uninhertited (reccomended for format<6)")
        ("softsampleset,s", po::value<int>()->default_value(4), "changes sampleset to soft (useful in osu 2007)")
        ("blackbg,b", po::value<int>()->default_value(7), "changes background to black")
        ("disablehitsample,h", po::value<int>()->default_value(7), "disables all custom hitsounds (wokrs only for v5+)")
        ("disablecolors,c", po::value<int>()->default_value(7), "disables map combo colors")
        ("enablestoryboard,b", po::bool_switch()->default_value(false), "enable storyboard copying");

    po::positional_options_description pos;
    pos.add("dir", 2);

    po::variables_map vm;
    po::store(po::command_line_parser(ac, av).options(desc).positional(pos).allow_unregistered().run(), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << "\n";
    }

    else if (vm.count("in") or vm.count("dir")) {
        if (!vm.count("format")) {
            std::cout << "ERROR: FORMAT NOT FOUND\n";
            return -1;
        }

        int format = vm["format"].as<int>();
        if (format < 3) format = 3;
        if (format > 14) format = 14;

        ConvertSettings settings = GetSettings(vm);

        std::string in_path;
        if (vm.count("in"))
            in_path = vm["in"].as<std::string>();
        else
            in_path = vm["dir"].as<std::vector<std::string>>()[0];

        if (vm.count("out") or (vm.count("dir") and 
            (vm.count("in") or vm.at("dir").as<std::vector<std::string>>().size() >= 2)))
        {
            std::string out_path;
            if(vm.count("out"))
                out_path = vm["out"].as<std::string>();
            else if (vm.at("dir").as<std::vector<std::string>>().size() >= 2)
                out_path = vm["dir"].as<std::vector<std::string>>()[1];
            else
                out_path = vm["dir"].as<std::vector<std::string>>()[0];

            CopyAndTransform(in_path, out_path, format, settings);

        }
        else {
            TransformAllFilesInDirectory(in_path, format, settings);
        }

        std::cout << "Exec is finished\n";
    }


    else {
        std::cout << "Use --help to get list of commands\n";
    }

}
