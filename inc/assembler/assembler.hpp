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

    enum OPERAND_TYPE
    {
        UNUSED,
        LIT,
        SYM,
        GPR,
        LIT_IND,
        SYM_IND,
        GPR_IND,
        GPR_LIT_DISP,
        GPR_SYM_DISP
    };

    enum JUMP_TYPE
    {
        JMP,
        BEQ,
        BNE,
        BGT
    };

    Assembler(string& src_file, string& obj_file):src_filename(src_file), obj_filename(obj_file) {}

    void process_source();

    void start_new_section(string sctn_name);

    void set_cpdir(DIR cpd);

    void process_symbol(string sym);

    void emit_symbol(string sym_name);
    void emit_num_literal(int num);
    void skip_bytes(int num);
    void emit_string(string str);

    void emit_byte(uint8_t byte);
    void emit_word(uint16_t word);
    void emit_dword(uint32_t dword);

    void prepare_operand(OPERAND_TYPE type, uint32_t lit, string sym_name, int gpr);

    void process_call();
    void process_jump(JUMP_TYPE type, int gpr_1=0, int gpr_2=0);


private:

    symbol_table::symbol* get_symbol_for_usage(string& sym_name);
    void clear_prepared_operand();

    // For list processing
    DIR current_proc_dir = DIR::NONE;

    // For operand processing
    OPERAND_TYPE operand_type = OPERAND_TYPE::UNUSED;
    uint32_t operand_lit;
    symbol_table::symbol* operand_sym = nullptr;
    int operand_gpr;

    string src_filename;
    string obj_filename;
    
    symbol_table symtab;

    section* current_section = nullptr;
    vector<section*> sections;

    unordered_map<string, int> sctn_name_map;
};

#endif // ASSEMBLER_HPP