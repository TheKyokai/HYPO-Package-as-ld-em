#ifndef SECTION_HPP
#define SECTION_HPP


#include <string>
#include <stdint.h>
#include <vector>


using namespace std;

class section
{
public:

    enum RELA_TYPE
    {
        R_32,
        R_PC32
    };

    struct rela_entry
    {
        uint32_t offset;
        RELA_TYPE type;
        uint32_t symbol;
        uint32_t addend;

        rela_entry(uint32_t offset, RELA_TYPE type, uint32_t symbol, uint32_t addend): offset(offset), type(type), symbol(symbol), addend(addend) {}
    };

    section(string sctn_name): name(sctn_name) {}

    int insert_rela_entry(uint32_t offset, RELA_TYPE type, uint32_t symbol, uint32_t addend);
    void emit_byte(uint8_t byte);
    void emit_word(uint16_t word);
    void emit_dword(uint32_t word);

    uint32_t get_location_ctr();
    vector<rela_entry>& get_rela();
    vector<uint8_t>& get_data();


    void print_rela();
    void print_data();

    void print_section();

private:
    uint32_t location_counter = 0;
    string name;
    vector<uint8_t> data;
    vector<rela_entry> rela;
};


#endif