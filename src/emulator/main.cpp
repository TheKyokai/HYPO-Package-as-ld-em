#include <iostream>
#include <string>

#include <emulator/emulator.hpp>

using namespace std;

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        cerr << "Invalid format - Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    string input_file = argv[1];

    emulator emu(input_file);

    emu.run();

    return 0;
}