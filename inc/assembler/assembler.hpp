#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

#include <string>
#include <assembler/symbol_table.hpp>

using namespace std;

class Assembler 
{
    public:
        Assembler(string& src_file, string& obj_file):src_filename(src_file), obj_filename(obj_file) {}
 
        void process_source();
    
        void process_instruction(string instr);

        void start_new_section(string sctn_name);

        void start_dir_global() {}
        void end_dir_global() {}

        void start_dir_extern() {}
        void end_dir_extern() {}

    private:
        
        string src_filename;
        string obj_filename;
        symbol_table symtab;
};

#endif // ASSEMBLER_HPP