#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <stdint.h>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class memory
{
public:

    bool init_memory(string& hex);

    uint32_t read(uint32_t address);
    void write(uint32_t address, uint32_t value);

    void print_memory();

    ~memory();

private:

    struct mem_page
    {
        uint8_t contents[0xFFF] = {};
    };

    unordered_map<uint32_t, size_t> pmt;
    vector<mem_page*> mem;
    uint32_t loaded_page_cnt = 0;
    vector<uint32_t> loaded_pages;

    void allocate_page(uint32_t pg_num);
    mem_page* get_page(uint32_t pg_num);

    void write_byte(uint32_t address, uint8_t value);

};

#endif