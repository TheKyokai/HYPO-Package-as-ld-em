#ifndef TERMINAL_HPP
#define TERMINAL_HPP

#include <termios.h>
#include <cstdint>


using namespace std;

class cpu;

class terminal
{
public:
    
    ~terminal();

    void connect(cpu* emu_cpu) { this->emu_cpu = emu_cpu; };

    void start();
    void check();

    uint32_t terminal_read();
    void terminal_write(uint32_t value);

private:
    cpu* emu_cpu = nullptr;

    termios old_termios;

    uint8_t input_buffer = 0;
    

    void enable_raw_mode();
    void disable_raw_mode();
};

#endif