#include <emulator/cpu.hpp>

#include <iomanip>


void cpu::reset()
{
    gpr_file[REG::PC] = 0x40000000;
    for (int i=0; i<15; i++)
        gpr_file[i] = 0;
    for (int i=0; i<3; i++)
        csr_file[i] = 0;
}

void cpu::step()
{
    gpr_file[0] = 0;
    uint32_t raw_instruction = fetch_instruction();
    instruction decoded_instruction = decode_instruction(raw_instruction);
    execute_instruction(decoded_instruction);
    check_and_handle_interrupt();
}


uint32_t cpu::fetch_instruction()
{
    uint32_t instruction = mem.read(gpr_file[REG::PC]);
    gpr_file[REG::PC] += 4;
    return instruction;
}

cpu::instruction cpu::decode_instruction(uint32_t raw_instruction)
{
    uint32_t op_code = (raw_instruction >> 28) & 0xF;
    uint32_t mod = (raw_instruction >> 24) & 0xF;
    int gprA = (raw_instruction >> 20) & 0xF;
    int gprB = (raw_instruction >> 16) & 0xF;
    int gprC = (raw_instruction >> 12) & 0xF;
    uint16_t decoded_disp = raw_instruction & 0xFFF;

    int disp = decoded_disp;
    // Sign extension
    if (decoded_disp & 0x800)
        disp |= 0xFFFFF000;
    
    if (op_code > 0x9)
        op_code = 0xA;
    
    instruction instr;
    instr.opc = static_cast<cpu::OPC>(op_code);
    instr.mod = static_cast<cpu::MOD>(mod);
    instr.gprA = gprA;
    instr.gprB = gprB;
    instr.gprC = gprC;
    instr.disp = disp;
    
    return instr;
}

void cpu::execute_instruction(instruction& instr)
{
    // cout << hex << gpr_file[REG::PC];
    // cout << " Instr: " << instr.opc << "-" << instr.mod << " A: " << instr.gprA << " B: " << instr.gprB << " C: " << instr.gprC << " Disp: " << hex << instr.disp << endl;
    switch(instr.opc)
    {
        case HALT:
            execute_halt(instr);
            break;
        case INT:
            execute_int(instr);
            break;
        case CALL:
            execute_call(instr);
            break;
        case JUMP:
            execute_jump(instr);
            break;
        case XCHG:
            execute_xchg(instr);
            break;
        case ARTH:
            execute_arth(instr);
            break;
        case LOG:
            execute_log(instr);
            break;
        case SH:
            execute_sh(instr);
            break;
        case ST:
            execute_st(instr);
            break;
        case LD:
            execute_ld(instr);
            break;
        default:
            raise_illegal_instruction();
            break;
    }
}

void cpu::check_and_handle_interrupt()
{
    if (!pending_interrupt)
        return;
    uint32_t cause = 0;
    if (pending_interrupt & 0x8)
    {
        pending_interrupt &= ~0x8;
        cause = 1;
    }
    else if (csr_file[REG::STATUS] & 0x4)
        return;
    else if (!(csr_file[REG::STATUS] & 0x2) && (pending_interrupt & 0x2))
    {
        pending_interrupt &= ~0x2;
        cause = 3;
    }
    else if (!(csr_file[REG::STATUS] & 0x1) && (pending_interrupt & 0x1))
    {
        pending_interrupt &= ~0x1;
        cause = 2;
    }

    if (!cause)
        return;
    
    gpr_file[REG::SP] -= 4;
    mem.write(gpr_file[REG::SP], csr_file[REG::STATUS]);

    gpr_file[REG::SP] -= 4;
    mem.write(gpr_file[REG::SP], gpr_file[REG::PC]);

    csr_file[REG::CAUSE] = cause;

    csr_file[REG::STATUS] |= 0x4;

    gpr_file[REG::PC] = csr_file[REG::HANDLER];
    
}

void cpu::raise_illegal_instruction()
{
    pending_interrupt |= 0x8;
}



void cpu::execute_halt(instruction &instr)
{
    switch (instr.mod)
    {
    case MOD_0:
        halted = true;
        break;
    default:
        raise_illegal_instruction();
        break;
    }
}

void cpu::execute_int(instruction &instr)
{
    switch (instr.mod)
    {
    case MOD_0:
        gpr_file[REG::SP] -= 4;
        mem.write(gpr_file[REG::SP], csr_file[REG::STATUS]);

        gpr_file[REG::SP] -= 4;
        mem.write(gpr_file[REG::SP], gpr_file[REG::PC]);

        csr_file[REG::CAUSE] = 0x4;

        csr_file[REG::STATUS] &= ~0x1;

        gpr_file[REG::PC] = csr_file[REG::HANDLER];
        cout << "PC after int " << hex << gpr_file[REG::PC] << endl;
        break;
    default:
        raise_illegal_instruction();
        break;
    }
}

void cpu::execute_call(instruction &instr)
{
    switch (instr.mod)
    {
    case MOD_0:
    {
        gpr_file[REG::SP] -= 4;
        mem.write(gpr_file[REG::SP], gpr_file[REG::PC]);

        gpr_file[REG::PC] = gpr_file[instr.gprA] + gpr_file[instr.gprB] + instr.disp;
        break;
    }
    case MOD_1:
    {
        gpr_file[REG::SP] -= 4;
        mem.write(gpr_file[REG::SP], gpr_file[REG::PC]);

        gpr_file[REG::PC] = mem.read(gpr_file[instr.gprA] + gpr_file[instr.gprB] + instr.disp);
        break;
    }
    default:
        raise_illegal_instruction();
        break;
    }
}

void cpu::execute_jump(instruction &instr)
{
    switch (instr.mod)
    {
    case MOD_0:
    {
        gpr_file[REG::PC] = gpr_file[instr.gprA] + instr.disp;
        break;
    }
    case MOD_1:
    {
        if (gpr_file[instr.gprB] == gpr_file[instr.gprC])
            gpr_file[REG::PC] = gpr_file[instr.gprA] + instr.disp;
        break;
    }
    case MOD_2:
    {
        if (gpr_file[instr.gprB] != gpr_file[instr.gprC])
            gpr_file[REG::PC] = gpr_file[instr.gprA] + instr.disp;
        break;
    }
    case MOD_3:
    {
        if (static_cast<int>(gpr_file[instr.gprB]) > static_cast<int>(gpr_file[instr.gprC]))
            gpr_file[REG::PC] = gpr_file[instr.gprA] + instr.disp;
        break;
    }
    case MOD_8:
    {
        gpr_file[REG::PC] = mem.read(gpr_file[instr.gprA] + instr.disp);
        break;
    }
    case MOD_9:
    {
        if (gpr_file[instr.gprB] == gpr_file[instr.gprC])
            gpr_file[REG::PC] = mem.read(gpr_file[instr.gprA] + instr.disp);
        break;
    }
    case MOD_A:
    {
        if (gpr_file[instr.gprB] != gpr_file[instr.gprC])
            gpr_file[REG::PC] = mem.read(gpr_file[instr.gprA] + instr.disp);
        break;
    }
    case MOD_B:
    {
        if (static_cast<int>(gpr_file[instr.gprB]) > static_cast<int>(gpr_file[instr.gprC]))
            gpr_file[REG::PC] = mem.read(gpr_file[instr.gprA] + instr.disp);
        break;
    }
    default:
        raise_illegal_instruction();
        break;
    }
}

void cpu::execute_xchg(instruction &instr)
{
    switch (instr.mod)
    {
    case MOD_0:
    {
        uint32_t temp = gpr_file[instr.gprB];
        gpr_write(instr.gprB, gpr_file[instr.gprC]);
        gpr_write(instr.gprC, temp);
        break;
    }
    default:
        raise_illegal_instruction();
        break;
    }
}

void cpu::execute_arth(instruction &instr)
{
    switch (instr.mod)
    {
    case MOD_0:
    {
        gpr_write(instr.gprA,gpr_file[instr.gprB] + gpr_file[instr.gprC]);
        break;
    }
    case MOD_1:
    {
        gpr_write(instr.gprA, gpr_file[instr.gprB] - gpr_file[instr.gprC]);
        break;
    }
    case MOD_2:
    {
        gpr_write(instr.gprA, gpr_file[instr.gprB] * gpr_file[instr.gprC]);
        break;
    }
    case MOD_3:
    {
        gpr_write(instr.gprA, gpr_file[instr.gprB] / gpr_file[instr.gprC]);
        break;
    }
    default:
        raise_illegal_instruction();
        break;
    }
}

void cpu::execute_log(instruction &instr)
{
    switch (instr.mod)
    {
    case MOD_0:
    {
        gpr_write(instr.gprA, ~gpr_file[instr.gprB]);
        break;
    }
    case MOD_1:
    {
        gpr_write(instr.gprA, gpr_file[instr.gprB] & gpr_file[instr.gprC]);
        break;
    }
    case MOD_2:
    {
        gpr_write(instr.gprA, gpr_file[instr.gprB] | gpr_file[instr.gprC]);
        break;
    }
    case MOD_3:
    {
        gpr_write(instr.gprA, gpr_file[instr.gprB] ^ gpr_file[instr.gprC]);
        break;
    }
    default:
        raise_illegal_instruction();
        break;
    }
}

void cpu::execute_sh(instruction &instr)
{
    switch (instr.mod)
    {
    case MOD_0:
    {
        gpr_write(instr.gprA, gpr_file[instr.gprB] << gpr_file[instr.gprC]);
        break;
    }
    case MOD_1:
    {
        gpr_write(instr.gprA, gpr_file[instr.gprB] >> gpr_file[instr.gprC]);
        break;
    }
    default:
        raise_illegal_instruction();
        break;
    }
}

void cpu::execute_st(instruction &instr)
{
    switch (instr.mod)
    {
    case MOD_0:
    {
        write_mem(gpr_file[instr.gprA] + gpr_file[instr.gprB] + instr.disp, gpr_file[instr.gprC]);
        break;
    }
    case MOD_1:
    {
        gpr_write(instr.gprA, gpr_file[instr.gprA] + instr.disp);
        write_mem(gpr_file[instr.gprA], gpr_file[instr.gprC]);
        break;
    }
    case MOD_2:
    {
        uint32_t addr = read_mem(gpr_file[instr.gprA] + gpr_file[instr.gprB] + instr.disp);
        // cout << "82 addr: 0x" << hex << addr << '\n';
        write_mem(addr, gpr_file[instr.gprC]);
        break;
    }
    default:
        raise_illegal_instruction();
        break;
    }
}

void cpu::execute_ld(instruction &instr)
{
    switch (instr.mod)
    {
    case MOD_0:
    {
        if (instr.gprB > 2)
        {
            raise_illegal_instruction();
            break;
        }
        gpr_write(instr.gprA, csr_file[instr.gprB]);
        break;
    }
    case MOD_1:
    {
        gpr_write(instr.gprA,gpr_file[instr.gprB] + instr.disp);
        break;
    }
    case MOD_2:
    {
        gpr_write(instr.gprA, read_mem(gpr_file[instr.gprB] + gpr_file[instr.gprC] + instr.disp));
        break;
    }
    case MOD_3:
    {
        gpr_write(instr.gprA, read_mem(gpr_file[instr.gprB]));
        gpr_write(instr.gprB, gpr_file[instr.gprB] + instr.disp);
        break;
    }
    case MOD_4:
    {
        if (instr.gprA > 2)
        {
            raise_illegal_instruction();
            break;
        }
        csr_file[instr.gprA] = gpr_file[instr.gprB];
        csr_file[REG::STATUS] &= 0x7;
        break;
    }
    case MOD_5:
    {
        if (instr.gprA > 2 || instr.gprB > 2)
        {
            raise_illegal_instruction();
            break;
        }
        csr_file[instr.gprA] = csr_file[instr.gprB] | instr.disp;
        csr_file[REG::STATUS] &= 0x7;
        break;
    }
    case MOD_6:
    {
        if (instr.gprA > 2)
        {
            raise_illegal_instruction();
            break;
        }
        csr_file[instr.gprA] = read_mem(gpr_file[instr.gprB] + gpr_file[instr.gprC] + instr.disp);
        csr_file[REG::STATUS] &= 0x7;
        break;
    }
    case MOD_7:
    {
        if (instr.gprA > 2)
        {
            raise_illegal_instruction();
            break;
        }
        csr_file[instr.gprA] = read_mem(gpr_file[instr.gprB]);
        csr_file[REG::STATUS] &= 0x7;
        gpr_write(instr.gprB, gpr_file[instr.gprB] + instr.disp);
        break;
    }
    default:
        raise_illegal_instruction();
        break;
    }
}


void cpu::dump_registers(ostream& os)
{
    os << "-----------------------------------------------------------------\n";
    os << "Emulated processor executed halt instruction\n";
    os << "Emulated processor state:\n";


    for (int i = 0; i < 16; i++)
    {
        
        if (i > 9)
            os  << setw(4)
                << 'r' 
                << dec
                << setw(2)
                << i;
        else
            os  << setw(5) 
                << 'r' 
                << dec
                << setw(1)
                << i;

        os << "=0x";
        os  << uppercase 
            << hex 
            << setw(8)
            << setfill('0')
            << gpr_file[i];

        if ((i + 1) % 4 == 0)
            os << '\n';
        else
            os << "    ";
        os << setfill(' ');
    }


    //DEBUG PRINT
    // os << "status=0x" << hex << setw(8) << right << setfill('0') << csr_file[REG::STATUS] << '\n';
    // os << "handler=0x" << hex << setw(8) << right << setfill('0') << csr_file[REG::HANDLER] << '\n';
    // os << "cause=0x" << hex << setw(8) << right << setfill('0') << csr_file[REG::CAUSE] << '\n';

    os << dec << nouppercase << setfill(' ');
}



void cpu::gpr_write(int gpr, uint32_t val)
{
    if (gpr == 0)
        return;
    gpr_file[gpr] = val;
}
    
uint32_t cpu::read_mem(uint32_t address)
{   
    if (address < 0xFFFFFF00)
        return mem.read(address);
    
    if (address == 0xFFFFFF00)
        return 0;
    
    if (address == 0xFFFFFF04)
        return term.terminal_read();
    
    if (address == 0xFFFFFF10)
        return tim.timer_read();
    
    return 0;
}

void cpu::write_mem(uint32_t address, uint32_t value)
{
    if (address < 0xFFFFFF00)
    {
        mem.write(address, value);
        return;
    }
    
    if (address == 0xFFFFFF00)
    {
        term.terminal_write(value);
        return; 
    }
        
    
    if (address == 0xFFFFFF04)
        return;
    
    if (address == 0xFFFFFF10)
        tim.timer_write(value);
}