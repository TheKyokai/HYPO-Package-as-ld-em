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
    
    if (current_section)
        symtab.set_section_size(current_section->get_name(), current_section->get_location_ctr());

    int status = symtab.insert_symbol(0, symbol_table::TYPE::SCTN, symbol_table::BIND::LOC, sctn_name);
    switch (status)
    {
        case 0:
        {
            // New symbol added to symtab
            symbol_table::symbol* sctn_sym = symtab.get_symbol_by_name(sctn_name);
            current_section = new section(sctn_name, sctn_sym->ndx);
            sections.push_back(current_section);  
            break;
        }
        case -1:
        {
            // Previously used symbol
            symbol_table::symbol* sctn_sym = symtab.get_symbol_by_name(sctn_name);
            if (sctn_sym->defined)
            {
                cerr << "Already defined symbol '" + sctn_name + "' defined as a section" << endl;
                return;
            }
            sctn_sym->type = symbol_table::TYPE::SCTN;
            sctn_sym->ndx = symtab.get_symbol_num(sctn_name);
            sctn_sym->defined = true;
            break;
        }
        default:
        {
            cerr << "Unexpected error" << endl;
            return;
        }
    }
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
            symtab.insert_symbol(0, symbol_table::TYPE::NOTYP, symbol_table::BIND::GLOB, sym, symbol_table::NDX::UND, false);
        else
            if (symbol->bind == symbol_table::BIND::GLOB && symbol->ndx == symbol_table::NDX::UND && symbol->defined)
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
            symtab.insert_symbol(0, symbol_table::TYPE::NOTYP, symbol_table::BIND::GLOB, sym, symbol_table::NDX::UND, true);
        else
            if (symbol->ndx != symbol_table::NDX::UND && symbol->defined)
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
                sym, current_section->get_ndx());
        else
        {
            if (symbol->defined)
            {
                if (symbol->ndx == symbol_table::NDX::UND)
                {
                    cerr << "Defining '" << sym << "' when it has been declared external" << endl;
                    return;
                }
                if (symbol->ndx != symbol_table::NDX::UND)
                {    
                    cerr << "Redefinition of already existing symbol '" << sym << "'" << endl;
                    return;
                }
            }
            // undefined global or previously used local symbol
            symbol->value = current_section->get_location_ctr();
            symbol->ndx = current_section->get_ndx();
            symbol->defined = true;
        }
        break;
    }
    default:
        cerr << "Unexpected symbol declaration/definition" << endl;
        break;
    }
}


// Helper when using a symbol that may not yet be declared or defined
symbol_table::symbol* Assembler::get_symbol_for_usage(string& sym_name)
{
    symbol_table::symbol* sym = symtab.get_symbol_by_name(sym_name);
    if (!sym)
    {
        symtab.insert_symbol(0, symbol_table::TYPE::NOTYP, symbol_table::BIND::LOC, sym_name, symbol_table::NDX::UND, false);
        sym = symtab.get_symbol_by_name(sym_name);
    }
    return sym;
}


// .word and .skip
void Assembler::emit_symbol(string sym_name)
{
    // Relocation entry should be created for everything except ABS symbols here since their actual value is needed
    if (!current_section)
    {
        cerr << "No section started" << endl;
        return;
    }
    symbol_table::symbol* sym = get_symbol_for_usage(sym_name);
    if (!sym->defined)
    {
        symtab.insert_forward_reference_entry(sym_name, current_section->get_name(), current_section->get_location_ctr());
    }
    // No relocation entry needed for ABS symbols
    if (sym->ndx == symbol_table::NDX::ABS)
    {
        emit_dword(sym->value);
        return;
    }
    // Create a relocation entry and allocate memory with zeroes
    uint32_t symbol_num, addend = 0x00000000;
    if (sym->bind == symbol_table::BIND::GLOB)    
        symbol_num = symtab.get_symbol_num(sym_name);
    else
    {
        symbol_num = sym->ndx;
        addend = sym->value;
    }

    // In case of undefined symbol rela_entry will also be changed during fixup
    current_section->insert_rela_entry(current_section->get_location_ctr(), section::RELA_TYPE::R_32, symbol_num, addend);
    emit_dword(0x00000000);
}

void Assembler::emit_num_literal(int num)
{
    if (!current_section)
    {
        cerr << "No section started" << endl;
        return;
    }
    current_section->emit_dword(num);
}

void Assembler::skip_bytes(int num)
{
    if (!current_section)
    {
        cerr << "No section started" << endl;
        return;
    }
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

void Assembler::emit_string(string str)
{
    if (!current_section)
    {
        cerr << "Unexpected generation" << endl;
        return;
    }
    for (char c: str)
        current_section->emit_byte(c);
    current_section->emit_byte(0x00);
}

void Assembler::prepare_operand(OPERAND_TYPE type, uint32_t lit, string sym_name, int gpr)
{
    operand_type = type;
    operand_lit = lit;
    operand_gpr = gpr;
    operand_sym = get_symbol_for_usage(sym_name);

    //cout << "\n OPERAND: " << type << ' ' << lit << ' ' << operand_sym->name << ' ' << gpr << '\n';
}

void Assembler::clear_prepared_operand()
{
    operand_type = OPERAND_TYPE::UNUSED;
    operand_lit = 0;
    operand_sym = nullptr;
    operand_gpr = -1;
}


void Assembler::process_call()
{
    cout << "\nCALL ";
    switch(operand_type)
    {
        case LIT:
        {
            cout << operand_lit << '\n';
            break;
        }
        case SYM:
        {
            cout << operand_sym->name << '\n';
            break;
        }
        default:
            cerr << "Unexpected jump operand" << endl;
    }
    clear_prepared_operand();
}

void Assembler::process_jump(JUMP_TYPE type, int gpr_1, int gpr_2)
{
    cout << '\n' << type << '\n';

    clear_prepared_operand();
}

