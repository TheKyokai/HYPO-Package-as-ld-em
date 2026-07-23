#ifndef EMULATOR_HPP
#define EMULATOR_HPP

#include <emulator/memory.hpp>
#include <emulator/cpu.hpp>
#include <emulator/terminal.hpp>
#include <emulator/timer.hpp>

#include <iostream>

using namespace std;

class emulator
{
public:

    emulator(string& hex): input_hex(hex), emu_cpu(mem, term, tim) {}

    void run();
    

private:

    string input_hex;
    memory mem;
    terminal term;
    timer tim;
    cpu emu_cpu;

    bool halted = false;

};

#endif