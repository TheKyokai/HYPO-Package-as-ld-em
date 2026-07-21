#ifndef LD_OPTIONS_HPP
#define LD_OPTIONS_HPP

#include <string>
#include <vector>

using namespace std;

struct section_placement
{
    string section_name;
    uint32_t address;
};


struct linker_options
{
    string output_file;

    vector<string> input_files;

    vector<section_placement> placements;

    bool hex = false;
    bool relocatable = false;
};


#endif