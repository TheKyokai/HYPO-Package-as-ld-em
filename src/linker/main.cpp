#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

#include <linker/ld_options.hpp>
#include <linker/linker.hpp>

using namespace std;


bool parse_address(const string& str, uint32_t& value)
{
    try
    {
        value = stoul(str, nullptr, 0);
        return true;
    }
    catch (...)
    {
        return false;
    }
}


bool parse_place(const string& arg, section_placement& placement)
{
    size_t pos = arg.find('@');

    if (pos == string::npos)
        return false;

    placement.section_name = arg.substr(0, pos);

    string addr_str = arg.substr(pos + 1);

    return parse_address(addr_str, placement.address);
}


int main(int argc, char** argv)
{
    linker_options options;

    for (int i = 1; i < argc; i++)
    {
        string arg = argv[i];

        if (arg == "-o")
        {
            if (i + 1 >= argc)
            {
                cerr << "Missing output file\n";
                return 1;
            }

            options.output_file = argv[++i];
        }
        else if (arg == "-hex")
        {
            options.hex = true;
        }
        else if (arg == "-relocatable")
        {
            options.relocatable = true;
        }
        else if (arg.rfind("-place=", 0) == 0)
        {
            string value = arg.substr(7);

            section_placement placement;

            if (!parse_place(value, placement))
            {
                cerr << "Invalid place option: " << arg << '\n';
                return 1;
            }

            options.placements.push_back(placement);
        }
        else if (arg[0] == '-')
        {
            cerr << "Unknown option: "
                      << arg << '\n';
            return 1;
        }
        else
        {
            // input object file
            options.input_files.push_back(arg);
        }
    }

    if (options.hex == options.relocatable)
    {
        cerr << "Exactly one of -hex or -relocatable must be specified\n";
        return 1;
    }

    if (options.output_file.empty())
    {
        cerr << "Output file not specified\n";
        return 1;
    }

    if (options.input_files.empty())
    {
        cerr << "No input files\n";
        return 1;
    }

    linker ld(options);

    ld.link_input_obj();

    return 0;
}