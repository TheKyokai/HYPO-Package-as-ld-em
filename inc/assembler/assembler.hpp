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

    enum OP_CODE
    {
        OP_HALT = 0x0,
        OP_INT = 0x1,
        OP_CALL = 0x2,
        OP_JUMP = 0x3,
        OP_XCHG = 0x4,
        OP_ARTH = 0x5,
        OP_LOG = 0x6,
        OP_SH = 0x7,
        OP_ST = 0x8,
        OP_LD = 0x9
    };

    enum OP_MOD
    {
        MOD_0 = 0x0,
        MOD_1 = 0x1,
        MOD_2 = 0x2,
        MOD_3 = 0x3,
        MOD_4 = 0x4,
        MOD_5 = 0x5,
        MOD_6 = 0x6,
        MOD_7 = 0x7,
        MOD_8 = 0x8,
        MOD_9 = 0x9,
        MOD_A = 0xA,
        MOD_B = 0xB,
        MOD_C = 0xC,
        MOD_D = 0xD,
        MOD_E = 0xE,
        MOD_F = 0xF
    };

    enum OP_GPR
    {
        GPR_0 = 0x0,
        GPR_1 = 0x1,
        GPR_2 = 0x2,
        GPR_3 = 0x3,
        GPR_4 = 0x4,
        GPR_5 = 0x5,
        GPR_6 = 0x6,
        GPR_7 = 0x7,
        GPR_8 = 0x8,
        GPR_9 = 0x9,
        GPR_10 = 0xA,
        GPR_11 = 0xB,
        GPR_12 = 0xC,
        GPR_13 = 0xD,
        GPR_14 = 0xE,
        GPR_15 = 0xF,
        GPR_SP = 0xE,
        GPR_PC = 0xF
    };

    enum OP_CSR
    {
        CSR_STATUS = 0x0,
        CSR_HANDLER = 0x1,
        CSR_CAUSE = 0x2
    };

    enum JUMP_TYPE
    {
        JMP,
        BEQ,
        BNE,
        BGT
    };

    enum ARTH_TYPE
    {
        ADD,
        SUB,
        MUL,
        DIV
    };

    enum LOG_TYPE
    {
        NOT,
        AND,
        OR,
        XOR
    };

    enum SH_TYPE
    {
        LEFT_SH,
        RIGHT_SH
    };

    enum CSR_OP_TYPE
    {
        CSR_RD,
        CSR_WR
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

    // Non-operand instructions

    void process_halt();
    void process_int();
    void process_ret();
    void process_iret();
    

    void process_xchg(OP_GPR gpr1, OP_GPR gpr2);
    void process_arth(ARTH_TYPE type, OP_GPR gpr1, OP_GPR gpr2);
    void process_log(LOG_TYPE type, OP_GPR gpr1, OP_GPR gpr2);
    void process_sh(SH_TYPE type, OP_GPR gpr1, OP_GPR gpr2);
    void process_csr_op(CSR_OP_TYPE type, OP_CSR csr, OP_GPR gpr);

    void process_push(OP_GPR gpr);
    void process_pop(OP_GPR gpr);

    // Operand instructions

    void process_call();
    void process_jump(JUMP_TYPE type, OP_GPR gpr1 = OP_GPR::GPR_0, OP_GPR gpr2 = OP_GPR::GPR_0);

    void process_ld(OP_GPR gpr);
    void process_st(OP_GPR gpr);

private:

    void emit_op(OP_CODE op_code, OP_MOD op_mod, OP_GPR gpr_A = OP_GPR::GPR_0, OP_GPR gpr_B = OP_GPR::GPR_0, OP_GPR gpr_C = OP_GPR::GPR_0, uint16_t disp = 0x000);

    symbol_table::symbol* get_symbol_for_usage(string& sym_name);
    void clear_prepared_operand();

    // For list processing
    DIR current_proc_dir = DIR::NONE;

    // For operand processing
    OPERAND_TYPE operand_type = OPERAND_TYPE::UNUSED;
    uint32_t operand_lit;
    symbol_table::symbol* operand_sym = nullptr;
    int operand_gpr;


    // Literal pool
    struct lit_pool_usage_entry
    {
        uint32_t lit;
        symbol_table::symbol* sym;
        vector<uint32_t> patch_offsets;

        lit_pool_usage_entry(uint32_t lit, symbol_table::symbol* sym):lit(lit), sym(sym) {}
    };

    void ensure_literal_pool(uint32_t byte_cnt, bool end = false);
    void register_lit_pool_usage(uint32_t patch_offset, symbol_table::symbol* sym, uint32_t lit = 0);


    // Max distance from lit_pool accounting for jmp pc + x pool skip
    const uint16_t MAX_LIT_POOL_DISTANCE = 0x7FB;
    uint32_t lit_pool_use_start = 0;
    vector<lit_pool_usage_entry*>* lit_pool_usages = nullptr;

    void forward_reference_fixup();
    void rela_fixup();

    bool undefined_symbols = false;
    bool error_detected = false;

    string src_filename;
    string obj_filename;
    
    symbol_table symtab;

    section* current_section = nullptr;
    vector<section*> sections;

    unordered_map<string, int> sctn_name_map;
};

#endif // ASSEMBLER_HPP