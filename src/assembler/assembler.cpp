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

    cout << "------ASM DUMP------\n\n";

    symtab.print_sym_tab();
    for (section* sec: sections)
        sec->print_section();

}


// Symbol declaration/definition
void Assembler::start_new_section(string sctn_name)
{
    auto it = sctn_name_map.find(sctn_name);
    if (it != sctn_name_map.end())
    {
        cerr << "Section '" << sctn_name << "' already exists" << endl;
        return;
    }

    //TODO Could be better -> decide what type of check to use -> map or symtab
    
    int status = symtab.insert_symbol(0, symbol_table::TYPE::SCTN, symbol_table::BIND::LOC, sctn_name);
    if (status)
    {
        cerr << "Unexpected error" << endl;
    }

    symbol_table::symbol* sctn_sym = symtab.get_symbol_by_name(sctn_name);
    current_section = new section(sctn_name, sctn_sym->ndx);
    sections.push_back(current_section);
    //TODO Add error handling -> Let the parser parse the whole file, but dont generate anything
}

void Assembler::set_cpdir(DIR cpd)
{
    current_proc_dir = cpd;
}

void Assembler::process_symbol(string sym)
{   
    switch (current_proc_dir)
    {
    case GLOBAL: // SYM in directive list
    {
        symbol_table::symbol* symbol = symtab.get_symbol_by_name(sym);
        if (symbol == nullptr)
            // global symbol declaration
            symtab.insert_symbol(0, symbol_table::TYPE::NOTYP, symbol_table::BIND::GLOB, sym);
        else
            if (symbol->bind == symbol_table::BIND::GLOB && symbol->ndx == symbol_table::NDX::EXT)
                cerr << "Declaring external symbol '" << sym << "' global" << endl;
            else
            {
                // declaring an existing symbol global -> multiple global declarations valid
                symbol->bind = symbol_table::BIND::GLOB;
                cout << "Symbol '" << sym << "' declared global" << endl;
            }
        break;
    }
    case EXTERN: // SYM in directive list
    {   
        symbol_table::symbol* symbol = symtab.get_symbol_by_name(sym);
        if (symbol == nullptr)
            // external symbol declaration
            symtab.insert_symbol(0, symbol_table::TYPE::NOTYP, symbol_table::BIND::GLOB, sym, true, symbol_table::NDX::EXT);
        else
            if (symbol->ndx != symbol_table::NDX::EXT)
                cerr << "Declaring a defined symbol '" << sym << "' external" << endl;
        break;
    }
    case NONE: // LABEL
    {   
        symbol_table::symbol* symbol = symtab.get_symbol_by_name(sym);
        if (symbol == nullptr)
            // new symbol_definition
            symtab.insert_symbol(current_section->get_location_ctr(),
                symbol_table::TYPE::NOTYP,
                symbol_table::BIND::LOC,
                sym, false, current_section->get_ndx());
        else
        {
            if (symbol->ndx == symbol_table::NDX::EXT)
            {
                cerr << "Defining '" << sym << "' when it has been declared external" << endl;
                return;
            }
            if (symbol->ndx != symbol_table::NDX::UND)
            {    
                cerr << "Redefinition of already existing symbol '" << sym << "'" << endl;
                return;
            }    
            // declared but undefined global symbol
            symbol->value = current_section->get_location_ctr();
            symbol->ndx = current_section->get_ndx();
        }
        break;
    }
    default:
        cerr << "Unexpected symbol declaration/definition" << endl;
        break;
    }
}


// .word and .skip
void Assembler::emit_symbol(string sym)
{
    // Relocation entry should be created for everything except ABS symbols here since their actual value is needed
    cout << "emit " << sym << " value\n";
}

void Assembler::emit_num_literal(int num)
{
    current_section->emit_dword(num);
}

void Assembler::skip_bytes(int num)
{
    while (num >= 4)
    {
        current_section->emit_dword(0x00000000);
        num -= 4;
    }
    while (num >= 2)
    {
        current_section->emit_word(0x0000);
        num -= 2;
    }
    while (num > 0)
    {
        current_section->emit_byte(0x00);
        num--;
    }
}


void Assembler::emit_byte(uint8_t byte)
{
    if (!current_section)
    {
        cerr << "Unexpected generation" << endl;
        return;
    }
    current_section->emit_byte(byte);
}

void Assembler::emit_word(uint16_t word)
{
    if (!current_section)
    {
        cerr << "Unexpected generation" << endl;
        return;
    }
    current_section->emit_word(word);
}

void Assembler::emit_dword(uint32_t dword)
{
    if (!current_section)
    {
        cerr << "Unexpected generation" << endl;
        return;
    }
    current_section->emit_dword(dword);
}
