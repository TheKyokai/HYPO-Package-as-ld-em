#include <emulator/memory.hpp>

#include <iostream>
#include <fstream>
#include <iomanip>


bool memory::init_memory(string &hex)
{
    ifstream hex_file(hex, ios::binary);

    if (!hex_file)
    {
        cerr << "Cannot open hex file: " << hex << '\n';
        return false;
    }


    uint32_t magic_num;

    hex_file.read(
        reinterpret_cast<char*>(&magic_num),
        sizeof(magic_num)
    );

    if (!hex_file)
    {
        cerr << "Invalid hex file\n";
        return false;
    }

    if (magic_num != 0x4D454D45)
    {
        cerr << "Invalid hex file magic num\n";
        return false;
    }


    while (hex_file)
    {
        uint32_t address;

        hex_file.read(
            reinterpret_cast<char*>(&address),
            sizeof(address)
        );

        if (!hex_file)
            break;


        uint8_t data[8];

        hex_file.read(
            reinterpret_cast<char*>(data),
            sizeof(data)
        );

        if (!hex_file)
        {
            cerr << "Invalid hex format\n";
            return false;
        }


        for (int i = 0; i < 8; i++)
            write_byte(address + i, data[i]);
    }

    return true;
}

void memory::write_byte(uint32_t address, uint8_t value)
{
    uint32_t pg_num = (address & ~0xFFF) >> 12;
    uint32_t offset = address & 0xFFF;

    mem_page* page = get_page(pg_num);

    page->contents[offset] = value;
}

uint32_t memory::read(uint32_t address)
{
    uint32_t pg_num = (address & ~0xFFF) >> 12;
    uint32_t offset = address & 0xFFF;
    mem_page* page = get_page(pg_num);
    uint32_t value = 0;
    for (int i=0; i<4; i++)
    {
        value = value | (page->contents[offset] << (8 * i));
        offset++;
        if (offset > 0xFFF)
        {
            offset = 0;
            page = get_page(pg_num + 1);
        }
    }
    return value;
}

void memory::write(uint32_t address, uint32_t value)
{
    uint32_t pg_num = (address & ~0xFFF) >> 12;
    uint32_t offset = address & 0xFFF;
    mem_page* page = get_page(pg_num);
    for (int i=0; i<4; i++)
    {
        page->contents[offset] = value & 0xFF;
        value >>= 8;

        offset++;
        if (offset > 0xFFF)
        {
            offset = 0;
            page = get_page(pg_num + 1);
        }
    }
}

void memory::allocate_page(uint32_t pg_num)
{
    pmt[pg_num] = loaded_page_cnt++;
    mem.push_back(new mem_page());
}

memory::mem_page* memory::get_page(uint32_t pg_num)
{
    auto it = pmt.find(pg_num);
    if (it == pmt.end())
        allocate_page(pg_num);
    return mem[pmt[pg_num]];
}


void memory::print_memory()
{   
    cout << hex << uppercase;

    for (auto& [pg_num, index] : pmt)
    {
        uint32_t base_address = pg_num << 12;

        cout << "Page 0x"
             << setw(8)
             << setfill('0')
             << base_address
             << '\n';

        mem_page* page = mem[index];

        for (int i = 0; i < 0x1000; i += 8)
        {
            cout << setw(8)
                 << setfill('0')
                 << (base_address + i)
                 << ": ";

            for (int j = 0; j < 8; j++)
            {
                cout << setw(2)
                     << static_cast<unsigned>(page->contents[i + j])
                     << " ";
            }

            cout << '\n';
        }

        cout << '\n';
    }

    cout << dec << nouppercase << setfill(' ');
}

memory::~memory()
{
    for (mem_page* pg:mem)
        delete pg;
}
