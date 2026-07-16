#include <assembler/assembler.hpp>
#include <iostream>
#include <fstream>

using namespace std;

#include "assembler/parser.hpp"
extern yy::parser::symbol_type yylex();
extern FILE* yyin;


void Assembler::process_source()
{
    FILE* fp = fopen(this->src_filename.c_str(), "r");

    if (!fp)
    {
        std::cerr << "Cannot open file\n";
        return;
    }

    yyin = fp;

    yy::parser parse(*this);
    
    parse();

    fclose(fp);

    symtab.print_sym_tab();
}

void Assembler::process_instruction(string instr)
{
    cout << instr;
}


void Assembler::start_new_section(string sctn_name)
{
    //TODO Placeholder for symtab testing -> Write section code
    int status = symtab.insert_symbol(0, symbol_table::TYPE::SCTN, symbol_table::BIND::LOC, sctn_name);
    //TODO Add error handling -> Let the parser parse the whole file, but dont generate anything
}
