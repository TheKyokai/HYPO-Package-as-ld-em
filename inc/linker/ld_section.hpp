#ifndef LD_SECTION_HPP
#define LD_SECTION_HPP

#include <string>
#include <stdint.h>
#include <vector>

using namespace std;

class ld_section
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

    ld_section(string sctn_name, uint32_t ndx): name(sctn_name), ndx(ndx) {}

    int insert_rela_entry(uint32_t offset, RELA_TYPE type, uint32_t symbol, uint32_t addend);
    void emit_byte(uint8_t byte);
    void emit_word(uint16_t word);
    void emit_dword(uint32_t word);

    uint32_t get_location_ctr();
    string& get_name();
    vector<rela_entry>& get_rela();
    vector<uint8_t>& get_data();
    
    uint32_t get_ndx();
    void set_ndx(uint32_t ndx) { this->ndx = ndx; }

    void print_rela(ostream& os);
    void print_data(ostream& os);
    void print_section(ostream& os);

    struct section_hdr
    {
        char name[32];

        uint32_t relocation_count;
        uint32_t size;

        uint32_t relocation_offset; 
        uint32_t data_offset;
    };

    section_hdr get_header();

    size_t get_size();
    void set_address(uint32_t addr);
    uint32_t get_address();

    void set_first_section(ld_section* sctn) { first_section = sctn; }
    ld_section* get_first_section() { return first_section; }

    void expand_data(vector<uint8_t>& expansion);

private:
    uint32_t location_counter = 0;
    string name;
    vector<uint8_t> data;
    vector<rela_entry> rela;

    uint32_t ndx;

    ld_section* first_section = nullptr;
    uint32_t address = 0; // Address for the first section, offset from first for the others with the same name
};

#endif