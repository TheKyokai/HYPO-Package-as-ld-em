#include <assembler/assembler.hpp>
#include <iostream>
#include <fstream>
#include <algorithm>

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

    ensure_literal_pool(0, true);
    if (current_section)
        symtab.set_section_size(current_section->get_name(), current_section->get_location_ctr());

    forward_reference_fixup();
    rela_fixup();

    generate_output();
}

void Assembler::generate_output()
{
    if (error_detected || undefined_symbols)
    {
        cerr << "Error detected - No output will be generated" << endl;
        return;   
    }

    // Generate text out_file

    ofstream out(out_filename);

    if (!out)
    {
        cerr << "Unable to open " << out_filename << '\n';
        return;
    }

    symtab.print_sym_tab(out);

    for (section* sctn : sections)
        sctn->print_section(out);

    out.close();

    // Generate bin obj_file 

    ofstream obj(obj_filename, ios::binary);

    if (!obj)
    {
        cerr << "Cannot open object file\n";
        return;
    }

    bin_hdr header;
    header.section_count = sections.size();
    header.symbol_count = symtab.get_symbols().size();
    header.relocation_count = 0;
    for (section* sctn:sections)
    {
        header.relocation_count += sctn->get_rela().size();
    }

    header.file_size = -1;

    obj.write(
        reinterpret_cast<char*>(&header),
        sizeof(header)
    );

    vector<section::section_hdr> section_headers;
    for (section* sctn:sections)
    {
        section::section_hdr shdr = sctn->get_header();
        section_headers.push_back(shdr);
        obj.write(
            reinterpret_cast<char*>(&shdr),
            sizeof(shdr)
        );
    }
    
    header.symbol_table_offset = obj.tellp();
    header.string_table_offset = generate_symtab_bin(obj);
    header.relocation_table_offset = obj.tellp();

    uint32_t current_rela_offset = 0;
    for (size_t i = 0; i < section_headers.size(); i++)
    {
        section::section_hdr& shdr = section_headers[i];
        section* sctn = sections[i];
        
        shdr.relocation_offset = current_rela_offset;
        for (section::rela_entry& entry:sctn->get_rela())
        {
            obj.write(
                reinterpret_cast<char*>(&entry),
                sizeof(entry)
            );
            current_rela_offset += sizeof(entry);
        }
    }

    header.data_offset = obj.tellp();

    uint32_t current_data_offset = 0;
    for (size_t i = 0; i < section_headers.size(); i++)
    {
        section::section_hdr& shdr = section_headers[i];
        section* sctn = sections[i];
        
        shdr.data_offset = current_data_offset;
        for (uint8_t& data_byte:sctn->get_data())
        {
            obj.put(data_byte);
            current_data_offset++;
        }
    }

    header.file_size = obj.tellp();


    // Offset fixup
    obj.seekp(20);
    obj.write(
        reinterpret_cast<char*>(&header.symbol_table_offset),
        sizeof(header.symbol_table_offset)
    );
    obj.write(
        reinterpret_cast<char*>(&header.string_table_offset),
        sizeof(header.string_table_offset)
    );
    obj.write(
        reinterpret_cast<char*>(&header.relocation_table_offset),
        sizeof(header.relocation_table_offset)
    );
    obj.write(
        reinterpret_cast<char*>(&header.data_offset),
        sizeof(header.data_offset)
    );
    obj.write(
        reinterpret_cast<char*>(&header.file_size),
        sizeof(header.file_size)
    );

    for (size_t i = 0; i < section_headers.size(); i++)
    {
        section::section_hdr& shdr = section_headers[i];
        obj.seekp(header.section_table_offset + i * sizeof(section::section_hdr) + sizeof(shdr.name) + sizeof(shdr.relocation_count) + sizeof(shdr.size));
        obj.write(
            reinterpret_cast<char*>(&shdr.relocation_offset),
            sizeof(shdr.relocation_offset)
        );
        obj.write(
            reinterpret_cast<char*>(&shdr.data_offset),
            sizeof(shdr.data_offset)
        );
    }

    obj.close();
}


uint32_t Assembler::generate_symtab_bin(ostream& obj)
{
    vector<char> string_table;
    for (symbol_table::symbol* sym:symtab.get_symbols())
    {
        symbol_table::symbol_hdr symhdr;
        symhdr.name_offset = string_table.size();
        for (char& c:sym->name)
            string_table.push_back(c);
        string_table.push_back('\0');

        symhdr.value = sym->value;
        symhdr.size = sym->size;
        symhdr.ndx = sym->ndx;
        symhdr.type = static_cast<uint8_t>(sym->type);
        symhdr.bind = static_cast<uint8_t>(sym->bind);

        obj.write(
            reinterpret_cast<char*>(&symhdr),
            sizeof(symhdr)
        );
    }

    uint32_t string_table_offset = obj.tellp();


    for (char& c:string_table)
        obj.put(c);
    
    return string_table_offset;
}



// Symbol declaration/definition
void Assembler::start_new_section(string sctn_name)
{
    if (current_section)
    {
        ensure_literal_pool(0, true);
        symtab.set_section_size(current_section->get_name(), current_section->get_location_ctr());
    }

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


// .word
void Assembler::emit_symbol(string sym_name)
{
    // Relocation entry should be created for everything except ABS symbols here since their actual value is needed
    if (!current_section)
    {
        cerr << "No section started" << endl;
        return;
    }

    ensure_literal_pool(4);

    symbol_table::symbol* sym = get_symbol_for_usage(sym_name);
    if (!sym->defined)
        symtab.insert_forward_reference_entry(sym_name, current_section->get_name(), current_section->get_location_ctr());
    
    // No relocation entry needed for ABS symbols
    if (sym->ndx == symbol_table::NDX::ABS)
    {
        emit_dword(sym->value);
        return;
    }
    // Create a relocation entry and allocate memory with zeroes
    // In case of undefined symbol rela_entry will also be changed during fixup -> Initaly treating all symbols as global
    current_section->insert_rela_entry(current_section->get_location_ctr(), section::RELA_TYPE::R_32, symtab.get_symbol_num(sym_name), 0);
    emit_dword(0x00000000);
}

void Assembler::emit_num_literal(int num)
{
    if (!current_section)
    {
        cerr << "No section started" << endl;
        return;
    }
    ensure_literal_pool(4);
    current_section->emit_dword(num);
}

void Assembler::skip_bytes(int num)
{
    if (!current_section)
    {
        cerr << "No section started" << endl;
        return;
    }
    ensure_literal_pool(num);
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
    ensure_literal_pool(str.size() + 1);
    for (char c: str)
        current_section->emit_byte(c);
    current_section->emit_byte(0x00);
}

void Assembler::emit_op(OP_CODE op_code, OP_MOD op_mod, OP_GPR gpr_A, OP_GPR gpr_B, OP_GPR gpr_C, uint16_t disp)
{
    // | op_code | op_mod | gpr_A | gpr_B | gpr_C | disp |
    uint32_t encoded_op = op_code;
    encoded_op = (encoded_op << 4) | op_mod;
    encoded_op = (encoded_op << 4) | gpr_A;
    encoded_op = (encoded_op << 4) | gpr_B;
    encoded_op = (encoded_op << 4) | gpr_C;
    encoded_op = (encoded_op << 12) | (disp & 0xFFF);
    emit_dword(encoded_op);
}


void Assembler::process_halt()
{
    ensure_literal_pool(4);
    emit_op(OP_CODE::OP_HALT, OP_MOD::MOD_0, OP_GPR::GPR_0, OP_GPR::GPR_0, OP_GPR::GPR_0, 0);
}

void Assembler::process_int()
{
    ensure_literal_pool(4);
    emit_op(OP_CODE::OP_INT, OP_MOD::MOD_0, OP_GPR::GPR_0, OP_GPR::GPR_0, OP_GPR::GPR_0, 0);
}

void Assembler::process_ret()
{
    ensure_literal_pool(4);
    emit_op(OP_CODE::OP_LD, OP_MOD::MOD_3, OP_GPR::GPR_PC, OP_GPR::GPR_SP, OP_GPR::GPR_0, 4);
}

void Assembler::process_iret()
{
    ensure_literal_pool(8);
    emit_op(OP_CODE::OP_LD, OP_MOD::MOD_6, OP_GPR::GPR_PC, OP_GPR::GPR_SP, OP_GPR::GPR_0, 4);
    emit_op(OP_CODE::OP_LD, OP_MOD::MOD_3, OP_GPR::GPR_PC, OP_GPR::GPR_SP, OP_GPR::GPR_0, 8);
}


void Assembler::process_xchg(OP_GPR gpr1, OP_GPR gpr2)
{
    ensure_literal_pool(4);
    // 0x40000000 | ($2 << 16) | ($4 << 12)
    emit_op(OP_CODE::OP_XCHG, OP_MOD::MOD_0, OP_GPR::GPR_0, gpr1, gpr2, 0);
}

void Assembler::process_arth(ARTH_TYPE type, OP_GPR gpr1, OP_GPR gpr2)
{   
    ensure_literal_pool(4);
    OP_MOD mod;
    switch(type)
    {
        case ADD:
        {
            mod = OP_MOD::MOD_0;
            break;
        }
        case SUB:
        {
            mod = OP_MOD::MOD_1;
            break;
        }
        case MUL:
        {
            mod = OP_MOD::MOD_2;
            break;
        }
        case DIV:
        {
            mod = OP_MOD::MOD_3;
            break;
        }
        default:
            break;
    }
    emit_op(OP_CODE::OP_ARTH, mod, gpr1, gpr1, gpr2, 0x000);
}

void Assembler::process_log(LOG_TYPE type, OP_GPR gpr1, OP_GPR gpr2)
{
    ensure_literal_pool(4);
    OP_MOD mod;
    switch(type)
    {
        case NOT:
        {
            mod = OP_MOD::MOD_0;
            gpr2 = OP_GPR::GPR_0;
            break;
        }
        case AND:
        {
            mod = OP_MOD::MOD_1;
            break;
        }
        case OR:
        {
            mod = OP_MOD::MOD_2;
            break;
        }
        case XOR:
        {
            mod = OP_MOD::MOD_3;
            break;
        }
        default:
            break;
    }
    emit_op(OP_CODE::OP_LOG, mod, gpr1, gpr1, gpr2, 0x000);
}

void Assembler::process_sh(SH_TYPE type, OP_GPR gpr1, OP_GPR gpr2)
{
    ensure_literal_pool(4);
    OP_MOD mod;
    switch(type)
    {
        case LEFT_SH:
        {
            mod = OP_MOD::MOD_0;
            break;
        }
        case RIGHT_SH:
        {
            mod = OP_MOD::MOD_1;
            break;
        }
        default:
            break;
    }
    emit_op(OP_CODE::OP_SH, mod, gpr1, gpr1, gpr2, 0x000);
}

void Assembler::process_csr_op(CSR_OP_TYPE type, OP_CSR csr, OP_GPR gpr)
{
    ensure_literal_pool(4);
    switch(type)
    {
        case CSR_RD:
        {
            emit_op(OP_CODE::OP_LD, OP_MOD::MOD_0, gpr, static_cast<Assembler::OP_GPR>(csr), OP_GPR::GPR_0, 0x000);
            break;
        }
        case CSR_WR:
        {
            emit_op(OP_CODE::OP_LD, OP_MOD::MOD_5, static_cast<Assembler::OP_GPR>(csr), gpr, OP_GPR::GPR_0, 0x000);
            break;
        }
        default:
            break;
    }
    
    
}

void Assembler::process_push(OP_GPR gpr)
{
    ensure_literal_pool(4);
    // 0x81E00FFC | ($2 << 12)
    emit_op(OP_CODE::OP_ST, OP_MOD::MOD_1, OP_GPR::GPR_SP, OP_GPR::GPR_0, gpr, 0xFFC);
}

void Assembler::process_pop(OP_GPR gpr)
{
    ensure_literal_pool(4);
    // 0x930E0004 | ($2 << 20)
    emit_op(OP_CODE::OP_LD, OP_MOD::MOD_3, gpr, OP_GPR::GPR_SP, OP_GPR::GPR_0, 0x004);
}



void Assembler::prepare_operand(OPERAND_TYPE type, uint32_t lit, string sym_name, int gpr)
{
    operand_type = type;
    operand_lit = lit;
    operand_gpr = gpr;
    if (type == OPERAND_TYPE::SYM || type == OPERAND_TYPE::SYM_IND || type == OPERAND_TYPE::GPR_SYM_DISP)
        operand_sym = get_symbol_for_usage(sym_name);
    else 
        operand_sym = nullptr;
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
    if (!current_section)
    {
        cerr << "No current section" << endl;
        return;
    }
    ensure_literal_pool(4);
    switch(operand_type)
    {
        case LIT:
        {
            register_lit_pool_usage(current_section->get_location_ctr(), nullptr, operand_lit);
            break;
        }
        case SYM:
        {
            register_lit_pool_usage(current_section->get_location_ctr(), operand_sym, 0);
            break;
        }
        default:
            cerr << "Invalid call operand" << endl;
    }
    emit_op(OP_CODE::OP_CALL, OP_MOD::MOD_1, OP_GPR::GPR_0, OP_GPR::GPR_PC, OP_GPR::GPR_0, 0);
    clear_prepared_operand();
}

void Assembler::process_jump(JUMP_TYPE type, OP_GPR gpr1, OP_GPR gpr2)
{
    if (!current_section)
    {
        cerr << "No current section" << endl;
        return;
    }
    ensure_literal_pool(4);
    switch(operand_type)
    {
        case LIT:
        {
            register_lit_pool_usage(current_section->get_location_ctr(), nullptr, operand_lit);
            break;
        }
        case SYM:
        {
            register_lit_pool_usage(current_section->get_location_ctr(), operand_sym, 0);
            break;
        }
        default:
            cerr << "Invalid call operand" << endl;
    }

    OP_MOD mod;
    switch(type)
    {
        case JMP:
        {
            mod = OP_MOD::MOD_8;
            break;
        }
        case BEQ:
        {
            mod = OP_MOD::MOD_9;
            break;
        }
        case BNE:
        {
            mod = OP_MOD::MOD_A;
            break;
        }
        case BGT:
        {
            mod = OP_MOD::MOD_B;
            break;
        }
        default:
            cerr << "Invalid jump type" << endl;
    }
    emit_op(OP_CODE::OP_JUMP, mod, OP_GPR::GPR_PC, gpr1, gpr2, 0x0);    
    clear_prepared_operand();
}


void Assembler::process_ld(OP_GPR gpr)
{
    switch (operand_type)
    {
        case LIT:
        {
            ensure_literal_pool(4);
            register_lit_pool_usage(current_section->get_location_ctr(), nullptr, operand_lit);
            emit_op(OP_CODE::OP_LD, OP_MOD::MOD_2, gpr, OP_GPR::GPR_PC, OP_GPR::GPR_0, 0x000);
            break;
        }
        case LIT_IND:
        {   
            ensure_literal_pool(8);
            register_lit_pool_usage(current_section->get_location_ctr(), nullptr, operand_lit);
            emit_op(OP_CODE::OP_LD, OP_MOD::MOD_2, gpr, OP_GPR::GPR_PC, OP_GPR::GPR_0, 0x000);
            emit_op(OP_CODE::OP_LD, OP_MOD::MOD_2, gpr, gpr, OP_GPR::GPR_0, 0x000);
            break;
        }
        case SYM:
        {
            ensure_literal_pool(4);
            register_lit_pool_usage(current_section->get_location_ctr(), operand_sym, 0);
            emit_op(OP_CODE::OP_LD, OP_MOD::MOD_2, gpr, OP_GPR::GPR_PC, OP_GPR::GPR_0, 0x000);
            break;
        }
        case SYM_IND:
        {
            ensure_literal_pool(8);
            register_lit_pool_usage(current_section->get_location_ctr(), operand_sym, 0);
            emit_op(OP_CODE::OP_LD, OP_MOD::MOD_2, gpr, OP_GPR::GPR_PC, OP_GPR::GPR_0, 0x000);
            emit_op(OP_CODE::OP_LD, OP_MOD::MOD_2, gpr, gpr, OP_GPR::GPR_0, 0x000);
            break;
        }
        case GPR:
        {
            ensure_literal_pool(4);
            emit_op(OP_CODE::OP_LD, OP_MOD::MOD_1, gpr, static_cast<OP_GPR>(operand_gpr), OP_GPR::GPR_0, 0x000);
            break;
        }
        case GPR_IND:
        {
            ensure_literal_pool(8);
            emit_op(OP_CODE::OP_LD, OP_MOD::MOD_1, gpr, static_cast<OP_GPR>(operand_gpr), OP_GPR::GPR_0, 0x000);
            emit_op(OP_CODE::OP_LD, OP_MOD::MOD_2, gpr, gpr, OP_GPR::GPR_0, 0x000);
            break;
        }
        case GPR_LIT_DISP:
        {
            if (operand_lit > 0xFFF)
            {
                cerr << "Literal displacement too large for ld instruction" << endl;
                break;
            }
            ensure_literal_pool(4);
            emit_op(OP_CODE::OP_LD, OP_MOD::MOD_2, gpr, static_cast<OP_GPR>(operand_gpr), OP_GPR::GPR_0, operand_lit);
            break;
        }
        case GPR_SYM_DISP:
        {
            ensure_literal_pool(4);
            if (operand_sym->defined)
                if (operand_sym->ndx == symbol_table::NDX::ABS)
                {
                    if (operand_sym->value > 0xFFF)
                    {
                        cerr << "Value of symbol '" << operand_sym->name << "' too large for [<reg> + <symbol>]" << endl;
                        break;
                    }

                    emit_op(OP_CODE::OP_LD, OP_MOD::MOD_2, gpr, static_cast<OP_GPR>(operand_gpr), OP_GPR::GPR_0, operand_sym->value);
                    break;
                }
                else
                {
                    cerr << "Value of symbol '" << operand_sym->name << "' will not be known during assembly" << endl;
                    break;
                }
            else
                disp_refs.push_back(new disp_ref(operand_sym, current_section->get_location_ctr(), current_section->get_name()));
                emit_op(OP_CODE::OP_LD, OP_MOD::MOD_2, gpr, static_cast<OP_GPR>(operand_gpr), OP_GPR::GPR_0, 0x000);
            break;
        }
    default:
        cerr << "Invalid operand in ld instruction" << endl;
        break;
    }
    clear_prepared_operand();
}

void Assembler::process_st(OP_GPR gpr)
{
    ensure_literal_pool(4);
    switch (operand_type)
    {
        case LIT:
        {
            register_lit_pool_usage(current_section->get_location_ctr(), nullptr, operand_lit);
            emit_op(OP_CODE::OP_ST, OP_MOD::MOD_0, OP_GPR::GPR_0, OP_GPR::GPR_PC, gpr, 0x000);
            break;
        }
        case LIT_IND:
        {   
            register_lit_pool_usage(current_section->get_location_ctr(), nullptr, operand_lit);
            emit_op(OP_CODE::OP_ST, OP_MOD::MOD_2, OP_GPR::GPR_0, OP_GPR::GPR_PC, gpr, 0x000);
            break;
        }
        case SYM:
        {
            register_lit_pool_usage(current_section->get_location_ctr(), operand_sym, 0);
            emit_op(OP_CODE::OP_ST, OP_MOD::MOD_0, OP_GPR::GPR_0, OP_GPR::GPR_PC, gpr, 0x000);
            break;
        }
        case SYM_IND:
        {   
            register_lit_pool_usage(current_section->get_location_ctr(), operand_sym, 0);
            emit_op(OP_CODE::OP_ST, OP_MOD::MOD_2, OP_GPR::GPR_0, OP_GPR::GPR_PC, gpr, 0x000);
            break;
        }
        case GPR:
        {
            emit_op(OP_CODE::OP_ST, OP_MOD::MOD_0, OP_GPR::GPR_0, static_cast<OP_GPR>(operand_gpr), gpr, 0x000);
            break;
        }
        case GPR_IND:
        {
            emit_op(OP_CODE::OP_ST, OP_MOD::MOD_2, OP_GPR::GPR_0, static_cast<OP_GPR>(operand_gpr), gpr, 0x000);
            break;
        }
        case GPR_LIT_DISP:
        {
            if (operand_lit > 0xFFF)
            {
                cerr << "Literal displacement too large for st instruction" << endl;
                break;
            }
            emit_op(OP_CODE::OP_ST, OP_MOD::MOD_2, OP_GPR::GPR_0, static_cast<OP_GPR>(operand_gpr), gpr, operand_lit);
            break;
        }
        case GPR_SYM_DISP:
        {
            ensure_literal_pool(4);
            if (operand_sym->defined)
                if (operand_sym->ndx == symbol_table::NDX::ABS)
                {
                    if (operand_sym->value > 0xFFF)
                    {
                        cerr << "Value of symbol '" << operand_sym->name << "' too large for [<reg> + <symbol>]" << endl;
                        break;
                    }
                    emit_op(OP_CODE::OP_ST, OP_MOD::MOD_2, OP_GPR::GPR_0, static_cast<OP_GPR>(operand_gpr), gpr, operand_sym->value);
                    break;
                }
                else
                {
                    cerr << "Value of symbol '" << operand_sym->name << "' will not be known during assembly" << endl;
                    break;
                }
            else
                disp_refs.push_back(new disp_ref(operand_sym, current_section->get_location_ctr(), current_section->get_name()));
                emit_op(OP_CODE::OP_ST, OP_MOD::MOD_2, OP_GPR::GPR_0, static_cast<OP_GPR>(operand_gpr), gpr, 0x000);
            break;
        }
    default:
        cerr << "Invalid operand in st instruction" << endl;
        break;
    }
    clear_prepared_operand();
}

// literal_pool distance check and emition including fixup
void Assembler::ensure_literal_pool(uint32_t byte_cnt, bool end)
{
    if (!current_section)
    {
        cerr << "Unexpected lit_pool_check" << endl;
        return;
    }
    if (!lit_pool_usages)
        return;
    uint32_t distance_after_byte_emission = current_section->get_location_ctr() - lit_pool_use_start + byte_cnt;
    // No need to emit lit_pool now if the distance is smaller than max or the section is not ending
    if (distance_after_byte_emission < MAX_LIT_POOL_DISTANCE && !end)
        return;
    // Emit the jmp over pool and the pool itself
    emit_op(OP_CODE::OP_JUMP, OP_MOD::MOD_0, OP_GPR::GPR_PC, OP_GPR::GPR_0, OP_GPR::GPR_0, lit_pool_usages->size() * 4);
    
    vector<uint8_t>& section_data = current_section->get_data();
    for (lit_pool_usage_entry* entry:*lit_pool_usages)
    {
        if (entry->sym)
        {
            if (!entry->sym->defined)
                symtab.insert_forward_reference_entry(entry->sym->name, current_section->get_name(), current_section->get_location_ctr());    
            current_section->insert_rela_entry(current_section->get_location_ctr(), section::RELA_TYPE::R_32, symtab.get_symbol_num(entry->sym->name), 0);    
            emit_dword(entry->sym->value);
        }
        else
            emit_dword(entry->lit);

        
        for (uint32_t patch_offset:entry->patch_offsets)
        {
            uint32_t lit_offset = current_section->get_location_ctr() - 4 - patch_offset - 4; // Section offset of the just emited symbol - patch offset - 4 (adjusting for pc being on the next inst)
            section_data[patch_offset] = lit_offset & 0xFF; // Amend whole byte
            section_data[patch_offset + 1] = ((lit_offset >> 8) & 0x0F) | section_data[patch_offset + 1]; // Amend bottom half-byte
        }
        delete entry;
    }
    delete lit_pool_usages;
    lit_pool_usages = nullptr;
    lit_pool_use_start = 0;
}

// register that a sym/lit has been used and the offset needs to be embedded later
void Assembler::register_lit_pool_usage(uint32_t patch_offset, symbol_table::symbol* sym, uint32_t lit)
{
    if (!lit_pool_usages)
    {
        lit_pool_usages = new vector<lit_pool_usage_entry*>;
        lit_pool_use_start = patch_offset + 4;
    }
    for (lit_pool_usage_entry* entry:*lit_pool_usages)
    {
        if ((sym !=nullptr && sym == entry->sym) || (sym == nullptr && lit == entry->lit))
        {   
            entry->patch_offsets.push_back(patch_offset);
            return;
        }
    }
    lit_pool_usage_entry* new_entry = new lit_pool_usage_entry(lit, sym);
    new_entry->patch_offsets.push_back(patch_offset);
    lit_pool_usages->push_back(new_entry);
}

void Assembler::forward_reference_fixup()
{   
    if (error_detected)
        return;
    
    for (disp_ref* ref:disp_refs)
    {
        if (!ref->sym->defined)
            continue;
        if (ref->sym->ndx != symbol_table::NDX::ABS)
        {
            cerr << "Value of symbol '" << ref->sym->name << "' will not be known during assembly" << endl;
            continue;
        }
        if (ref->sym->value > 0xFFF)
        {
            cerr << "Value of symbol '" << ref->sym->name << "' too large for [<reg> + <symbol>] - " << ref->sym->value << endl;
            continue;
        }
        section* sctn = sections[sctn_name_map[ref->sctn_name]];
        vector<uint8_t>& data = sctn->get_data();
        uint32_t val = ref->sym->value;
        data[ref->patch_addr] = val & 0xFF;
        data[ref->patch_addr + 1] |= ((val >> 8) & 0x0F);
    }
    
    
    vector<symbol_table::symbol*>& symbols = symtab.get_symbols();
    for (symbol_table::symbol* sym:symbols)
    {
        if (!sym->defined)
        {
            undefined_symbols = true;
            continue;
        }
        // No need to fixup non-abs symbols
        if (!sym->forward_reference_table || sym->ndx != symbol_table::NDX::ABS)
            continue;
        for (symbol_table::symbol::forward_reference_entry* entry:*sym->forward_reference_table)
        {
            section* sctn = sections[sctn_name_map[entry->sctn]];
            vector<uint8_t>& data = sctn->get_data();
            uint32_t val = sym->value;
            cout << sym->name << endl;
            data[entry->patch_addr] = val & 0xFF;
            data[entry->patch_addr + 1] = (val >> 8) & 0xFF;
            data[entry->patch_addr + 2] = (val >> 16) & 0xFF;
            data[entry->patch_addr + 3] = (val >> 24) & 0xFF;
        }
    }
}

void Assembler::rela_fixup()
{
    if (undefined_symbols)
        return;

    for (section* sctn:sections)
    {   
        for (section::rela_entry& entry:sctn->get_rela())
        {
            symbol_table::symbol* sym = symtab.get_symbols()[entry.symbol];
            if (sym->ndx == symbol_table::NDX::ABS)
            {
                entry.symbol = sym->ndx;
                continue;
            }
            // Already adjusted for global symbols
            if (sym->bind == symbol_table::BIND::GLOB)
                continue;
            entry.symbol = sym->ndx;
            entry.addend = sym->value;
        }
        vector<section::rela_entry>& rela = sctn->get_rela();
        rela.erase(remove_if(rela.begin(), rela.end(), [](section::rela_entry entry) {return entry.symbol == symbol_table::NDX::ABS;}), rela.end());
    }
}  
