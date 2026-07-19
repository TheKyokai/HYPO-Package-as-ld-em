#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

#include <string>
#include <assembler/symbol_table.hpp>
#include <assembler/section.hpp>

using namespace std;

class Assembler 
{    
public:

    enum DIR 
    {
        NONE,
        GLOBAL,
        EXTERN
    };

    Assembler(string& src_file, string& obj_file):src_filename(src_file), obj_filename(obj_file) {}

    void process_source();

    void start_new_section(string sctn_name);

    void set_cpdir(DIR cpd);

    void process_symbol(string sym);

    void emit_symbol(string sym_name);
    void emit_num_literal(int num);
    void skip_bytes(int num);

    void emit_byte(uint8_t byte);
    void emit_word(uint16_t word);
    void emit_dword(uint32_t dword);

private:

    DIR current_proc_dir = DIR::NONE;

    string src_filename;
    string obj_filename;
    
    symbol_table symtab;

    section* current_section = nullptr;
    vector<section*> sections;

    unordered_map<string, int> sctn_name_map;
};

#endif // ASSEMBLER_HPP