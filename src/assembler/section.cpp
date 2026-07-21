#include <assembler/section.hpp>

#include <iostream>
#include <iomanip>
#include <cstring>


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


void section::print_rela(ostream& os)
{
    os << "\n.rela." << name << '\n';

    if (rela.empty()) {
        os << "(empty)\n";
        return;
    }

    os << string(50, '-') << endl;

    os << left
       << setw(13) << "Offset"
       << setw(15) << "Type"
       << setw(10) << "Symbol"
       << "Addend\n";

    for (const auto& entry : rela) {
        string entry_type;

        if (entry.type == RELA_TYPE::R_32)
            entry_type = "R_32";
        else if(entry.type == RELA_TYPE::R_32L)
            entry_type = "R_32L";

        os << left;

        os << "0x"
           << uppercase << hex
           << setw(8) << setfill('0') << right
           << entry.offset;

        os << setfill(' ') << dec << nouppercase << left
           << setw(3) << "";

        os << setw(15) << entry_type
           << setw(10) << entry.symbol;

        os << "0x"
           << uppercase << hex
           << setw(8) << setfill('0') << right
           << entry.addend
           << '\n';

        os << setfill(' ') << dec << nouppercase;
    }
}


void section::print_data(ostream& os)
{
    if (data.empty()) {
        os << "(empty)\n";
        return;
    }

    for (size_t i = 0; i < data.size(); ++i) {
        if (i % 16 == 0)
            os << setw(4) << setfill('0') << hex << i << ": ";

        os << right
             << setw(2)
             << setfill('0')
             << hex
             << uppercase
             << (unsigned)(data[i]) << ' ';

        if ((i + 1) % 16 == 0 || i + 1 == data.size())
            os << '\n';
    }

    os << dec << nouppercase << setfill(' ');
}


void section::print_section(ostream& os)
{
    os << ".section." << name << '\n';
    print_data(os);
    print_rela(os);
    os << '\n' << endl;
}


section::section_hdr section::get_header()
{
    section_hdr header;
    strncpy(header.name, name.c_str(), sizeof(header.name) - 1);
    header.name[sizeof(header.name) - 1] = '\0';

    header.size = data.size();
    header.relocation_count = rela.size();

    header.data_offset = 0;
    header.relocation_offset = 0; 
    return header;
}

