#ifndef LD_SYMBOL_TABLE_HPP
#define LD_SYMBOL_TABLE_HPP

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class ld_symbol_table
{
public:

    enum TYPE 
    {
        NOTYP,
        SCTN
    };

    enum BIND 
    {
        LOC,
        GLOB
    };
    
    enum NDX 
    {
        UND = 0x00000000,
        ABS = 0xFFFFFFFF
    };

    struct symbol 
    {
        uint32_t num;
        uint32_t value;
        uint32_t size;
        TYPE type;
        BIND bind;
        uint32_t ndx;
        string name;

        symbol(uint32_t num, uint32_t value, TYPE type, BIND bind, uint32_t ndx, string& name):
            num(num), value(value), size(0), type(type), bind(bind), ndx(ndx), name(name)
        {}
    };

    symbol* get_symbol_by_name(string& sym_name);
    vector<symbol*>& get_symbols();
    int get_symbol_num(string& sym_name);

    
    int insert_symbol(uint32_t value, TYPE type, BIND bind, string& sym_name, uint32_t ndx = NDX::UND);
    int set_section_size(string& section_name, uint32_t size);

    ld_symbol_table();
    ~ld_symbol_table();

    void print_sym_tab(ostream& os);
    
    struct symbol_hdr
    {
        uint32_t name_offset; 

        uint32_t value;
        uint32_t size;

        uint32_t ndx;      

        uint8_t type;
        uint8_t bind;
        uint16_t alignment_padding = 0x0000;
    };


private:

    uint32_t current_num = 0;

    unordered_map<string, int> index_map;
    vector<symbol*> sym_tab;
};

#endif