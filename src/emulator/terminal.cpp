#include <emulator/terminal.hpp>

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>

#include <emulator/cpu.hpp>



void terminal::start()
{
    enable_raw_mode();

    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(
        STDIN_FILENO,
        F_SETFL,
        flags | O_NONBLOCK
    );
}


terminal::~terminal()
{
    disable_raw_mode();
}


void terminal::enable_raw_mode()
{
    tcgetattr(STDIN_FILENO, &old_termios);

    termios raw = old_termios;

    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;

    tcsetattr(
        STDIN_FILENO,
        TCSANOW,
        &raw
    );
}


void terminal::disable_raw_mode()
{
    tcsetattr(
        STDIN_FILENO,
        TCSANOW,
        &old_termios
    );
}


void terminal::check()
{
    fd_set set;
    FD_ZERO(&set);
    FD_SET(STDIN_FILENO, &set);

    timeval timeout{};
    
    int result = select(
        STDIN_FILENO + 1,
        &set,
        nullptr,
        nullptr,
        &timeout
    );

    if (result > 0 && FD_ISSET(STDIN_FILENO, &set))
    {
        char c;

        if (read(STDIN_FILENO, &c, 1) > 0)
        {
            input_buffer = static_cast<uint8_t>(c);

            if (emu_cpu)
                emu_cpu->terminal_interrupt();
        }
    }
}


uint32_t terminal::terminal_read()
{
    return input_buffer;
}


void terminal::terminal_write(uint32_t value)
{
    cout << static_cast<char>(value & 0xFF);
    cout.flush();
}