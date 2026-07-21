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
        R_32
    };

    struct rela_entry
    {
        uint32_t offset;
        RELA_TYPE type;
        uint32_t symbol;
        uint32_t addend;

        rela_entry(uint32_t offset, RELA_TYPE type, uint32_t symbol, uint32_t addend): offset(offset), type(type), symbol(symbol), addend(addend) {}
    };

    section(string sctn_name, uint32_t ndx): name(sctn_name), ndx(ndx) {}

    int insert_rela_entry(uint32_t offset, RELA_TYPE type, uint32_t symbol, uint32_t addend);
    void emit_byte(uint8_t byte);
    void emit_word(uint16_t word);
    void emit_dword(uint32_t word);

    uint32_t get_location_ctr();
    string& get_name();
    vector<rela_entry>& get_rela();
    vector<uint8_t>& get_data();
    
    uint32_t get_ndx();

    void print_rela(ostream& os);
    void print_data(ostream& os);
    void print_section(ostream& os);

    struct section_hdr
    {
        char name[32];

        uint32_t size;
        uint32_t data_offset;       

        uint32_t relocation_count;
        uint32_t relocation_offset; 
    };

    section_hdr get_header();
    void print_binary_rela(ostream& os);
    void print_binary_data(ostream& os);

private:
    uint32_t location_counter = 0;
    string name;
    vector<uint8_t> data;
    vector<rela_entry> rela;

    uint32_t ndx;

    
};


#endif