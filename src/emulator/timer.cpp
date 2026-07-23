#include <emulator/timer.hpp>
#include <emulator/cpu.hpp>


void timer::tick()
{
    cycle_counter += 1;

    if (cycle_counter >= periods[tim_cfg])    
    {
        cycle_counter = 0;

        if (emu_cpu)
            emu_cpu->timer_interrupt();
    }
}

uint32_t timer::timer_read()
{
    return tim_cfg;
}

void timer::timer_write(uint32_t value)
{
    tim_cfg = value & 0x7;
    cycle_counter = 0;
}