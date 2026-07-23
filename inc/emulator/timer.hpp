#ifndef TIMER_HPP
#define TIMER_HPP

#include <stdint.h>

class cpu;

class timer
{
public:
    void connect(cpu* emu_cpu) { this->emu_cpu = emu_cpu; }

    void tick();

    uint32_t timer_read();
    void timer_write(uint32_t value);

private:
    cpu* emu_cpu = nullptr;

    uint32_t tim_cfg = 0;
    uint64_t cycle_counter = 0;

    const uint64_t hs_cycle_cnt = 500000;

    const uint64_t periods[8] =
    {
        hs_cycle_cnt,         // 500 ms
        hs_cycle_cnt * 2,     // 1 s
        hs_cycle_cnt * 3,     // 1.5 s
        hs_cycle_cnt * 4,     // 2 s
        hs_cycle_cnt * 10,    // 5 s
        hs_cycle_cnt * 20,    // 10 s
        hs_cycle_cnt * 60,    // 30 s
        hs_cycle_cnt * 120    // 60 s
    };
};

#endif