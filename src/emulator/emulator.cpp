#include <emulator/emulator.hpp>



void emulator::run()
{
    
    if (!mem.init_memory(input_hex))
    {
        cerr << "Emulation will not begin\n";
        return;   
    }

    emu_cpu.reset();
    term.connect(&emu_cpu);
    tim.connect(&emu_cpu);

    term.start();

    while(!emu_cpu.is_halted())
    {
        emu_cpu.step();
        term.check();
        tim.tick();
    }

    emu_cpu.dump_registers(cout);
}

