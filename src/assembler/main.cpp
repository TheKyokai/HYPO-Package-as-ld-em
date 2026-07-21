#include <assembler/assembler.hpp>

#include <iostream>
#include <string>

using namespace std;


int report_error(string msg, int status)
{
    cerr << "ERROR: " << msg << endl;
    return status;
}



int main(int argc, char* argv[])
{

    char *input_file = nullptr;
    char *output_file = nullptr;
  
    for (int i = 1; i < argc; i++)
    {
        string arg(argv[i]);
        if (arg == "-o")
        {
            if (output_file != nullptr)
                return report_error("Output file already provided", 1);

            i++;
            if (i >= argc)
                return report_error("Missing file for option -o, correct: -o <file.o>", 1);

            output_file = argv[i];
        }
        else
        {
            if (input_file != nullptr)
                return report_error("Input file already provided", 1);
            
            input_file = argv[i];
        }
    }

    if (argc > 4)
        return report_error("More arguments provided than allowed", 1);

    if (input_file == nullptr)
        return report_error("Assembler source file not provided", 1);


    string src_file(input_file);
    string obj_file;
    string out_file;

    if (output_file == nullptr)
    {
        size_t dot_index = src_file.find_last_of('.');
        if (dot_index == string::npos)
            obj_file = src_file + ".o";
        else
            obj_file = src_file.substr(0, dot_index) + ".o";
    }
    else
        obj_file = output_file;
    
    size_t dot_index = obj_file.find_last_of('.');
    if (dot_index == string::npos)
        out_file = obj_file + ".out";
    else
        out_file = obj_file.substr(0, dot_index) + ".out";
    
    
    Assembler assembler(src_file, obj_file, out_file);

    assembler.process_source();

    return 0;
}