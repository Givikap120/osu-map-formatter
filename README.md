# osu-map-formatter
Converts osu maps from one format to another (supported formats: 3-14)

Tested only on Windows. However you can try to build it in other operation systems
Boost lib is required for building

## Running

You can download the newest 64-bit binaries from [releases](https://github.com/Givikap120/osu-map-formatter/releases).

After unpacking it to your desired directory, you need to run it using .bat file or a command-line application/terminal

##### Windows cmd:
```
osu-map-formatter <arguments>
```

## Run arguments
* `--help` : shows list of commands
* `--in "file name"` or `--in "directory name"` : maps to transform (directory will be used recursively)
* `--out "file name"` (only if "in" was filename) or `--out "directory name"` : output destination, if not chosen - input files will be rewritten. 
* WARNING: app just copies all files from in to out and then transform all .osu files in out directory, be careful
* `--dir  "file name"` or `--dir "directory name"` : you can use directories without words "in" or "out", first directory would be in, second will be out
* example: "input_directory\" "output_directory\"
* `--format "number"` or `-f "number"` : output format
* RECOMMENDED FORMATS: v3 (osu 2007), v5 (osu 2008), v6 (osu 2009), v7 (osu 2010)
* Explanation for "k" parameter in arguments below: use 0 for false, 1 for true or number from 3 to 14 for the version. If the format <= k, it will be true, false otherwise
* `--inhertitancefix k` or `-i k` : transforms all green lines to red lines, fixing slider velocity in old versions (DEFAULT: true for format<=5)
* `--softsampleset k` or `-s k` : changes default sampleset to soft (DEFAULT: true for format <=4)
* `--blackbg k` or `-b k` : changes background to black (DEFAULT: true for format<=7)
* `--disablehitsample k` or `-h k` : disables custom hitsamples (DEFAULT: true for format<=7)
* `--disablecolors k` or `-c k` : disables map colors (DEFAULT: true for formar<=7)
* `--enablestoryboard` or `-b` : enables storyboard (DEFUALT: storyboard disabled)
* Storyboard conversion is not supported yet: its just gets copied if flag `--enablestoryboard` is set


Examples which should give the same result:

```
<executable> --in "in/" --out "out/" -f 3

<executable> "in/" "out/" -f 3

<executable> in/ out/ -f 3

<executable> in/ out/ -f 3 --inheritancefix 1

<executable> in/ out/ -f 3 -i 1 -s 1 -b 1 -h 1 -c 1
```

## Building the project
You need to clone it or download as a .zip (and unpack it to desired directory)

#### Prerequisites

* Visual Studio
* boost (https://www.boost.org/users/history/version_1_81_0.html)

#### Building and running the project

Download and build boost library

Connect boost library to addonitional include directories and linker options in solution options

Build solution
