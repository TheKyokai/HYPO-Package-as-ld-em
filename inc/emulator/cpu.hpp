#ifndef CPU_HPP
#define CPU_HPP

#include <emulator/memory.hpp>
#include <emulator/terminal.hpp>
#include <emulator/timer.hpp>

#include <iostream>


class cpu
{
public:

    cpu(memory& mem, terminal& term, timer& tim):mem(mem), term(term), tim(tim) {}
    
    void reset();

    void step();
    bool is_halted() { return halted; }

    void terminal_interrupt() { pending_interrupt |= 0x2; };
    void timer_interrupt() { pending_interrupt |= 0x1; };

    void dump_registers(ostream& os);

private:

    memory& mem;
    terminal& term;
    timer& tim;

    bool halted = false;

    enum REG
    {
        STATUS = 0,
        HANDLER = 1,
        CAUSE = 2,
        SP = 14,
        PC = 15
    };

    uint32_t csr_file[3];
    uint32_t gpr_file[16];


    enum OPC
    {
        HALT = 0x0,
        INT = 0x1,
        CALL = 0x2,
        JUMP = 0x3,
        XCHG = 0x4,
        ARTH = 0x5,
        LOG = 0x6,
        SH = 0x7,
        ST = 0x8,
        LD = 0x9,
        ILLEGAL = 0xA
    };

    enum MOD
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

    struct instruction
    {
        OPC opc;
        MOD mod;
        int gprA;
        int gprB;
        int gprC;
        int disp;
    };

    uint32_t fetch_instruction();
    
    
    instruction decode_instruction(uint32_t raw_instruction);
    

    void execute_instruction(instruction& instr);
    
    void execute_halt(instruction& instr);
    void execute_int(instruction& instr);
    void execute_call(instruction& instr);
    void execute_jump(instruction& instr);
    void execute_xchg(instruction& instr);
    void execute_arth(instruction& instr);
    void execute_log(instruction& instr);
    void execute_sh(instruction& instr);
    void execute_st(instruction& instr);
    void execute_ld(instruction& instr);

    uint32_t pending_interrupt = 0x0;

    void check_and_handle_interrupt();

    void raise_illegal_instruction();
    
    void gpr_write(int gpr, uint32_t val);
    
    uint32_t read_mem(uint32_t address);
    void write_mem(uint32_t address, uint32_t value);

};



#endif