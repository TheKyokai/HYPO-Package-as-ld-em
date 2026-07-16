#include <assembler/section.hpp>

#include <iostream>
#include <iomanip>


int section::insert_rela_entry(uint32_t offset, RELA_TYPE type, uint32_t symbol, uint32_t addend)
{
    rela_entry entry(offset, type, symbol, addend);
    rela.push_back(entry);
    return 0;
}


void section::emit_byte(uint8_t byte)
{
    data.push_back(byte);
    location_counter++;
}


void section::emit_word(uint16_t word)
{
    data.push_back(word & 0xFF);
    data.push_back((word >> 8) & 0xFF);
    location_counter += 2;
}


void section::emit_dword(uint32_t word)
{
    data.push_back(word & 0xFF);
    data.push_back((word >> 8) & 0xFF);
    data.push_back((word >> 16) & 0xFF);
    data.push_back((word >> 24) & 0xFF);
    location_counter += 4;
}


vector<section::rela_entry>& section::get_rela()
{
    return rela;
}


vector<uint8_t>& section::get_data()
{
    return data;
}


uint32_t section::get_location_ctr()
{
    return location_counter;
}


string& section::get_name()
{
    return name;
}


uint32_t section::get_ndx()
{
    return ndx;
}


void section::print_rela()
{
    cout << "\n.rela." << name << '\n';
    if (rela.empty()) {
        cout << "(empty)\n";
        return;
    }

    cout << left
         << setw(10) << "Offset"
         << setw(15) << "Type"
         << setw(10) << "Symbol"
         << "Addend\n";

    for (const auto& entry : rela) {
        cout << left
             << setw(10) << entry.offset
             << setw(15) << entry.type
             << setw(10) << entry.symbol
             << entry.addend << '\n';
    }
}


void section::print_data()
{
    cout << "\n." << name << ".data\n";
    if (data.empty()) {
        cout << "(empty)\n";
        return;
    }

    for (size_t i = 0; i < data.size(); ++i) {
        if (i % 16 == 0)
            cout << setw(4) << setfill('0') << hex << i << ": ";

        cout << right
             << setw(2)
             << setfill('0')
             << hex
             << uppercase
             << (unsigned)(data[i]) << ' ';

        if ((i + 1) % 16 == 0 || i + 1 == data.size())
            cout << '\n';
    }

    cout << dec << nouppercase << setfill(' ');
}


void section::print_section()
{
    cout << ".section." << name << '\n';
    print_data();
    print_rela();
    cout << '\n' << endl;
}
