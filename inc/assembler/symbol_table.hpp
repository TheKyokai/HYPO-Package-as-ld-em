#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP


#include <stdint.h>
#include <string>
#include <vector>
#include <unordered_map>


using namespace std;


class symbol_table
{
public:

    enum TYPE {
        NOTYP,
        SCTN
    };

    enum BIND {
        LOC,
        GLOB
    };
    
    enum NDX {
        UND = 0x00000000,
        EXT = 0xFFFFFFFE,
        ABS = 0xFFFFFFFF
    };

    struct symbol {
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

    //TODO Potential extern_sym param removal
    int insert_symbol(uint32_t value, TYPE type, BIND bind, string& sym_name, bool extern_sym = false, uint32_t ndx = NDX::UND);
    int set_section_size(string& section_name, uint32_t size);

    symbol_table();
    ~symbol_table();

    void print_sym_tab();
private:

    uint32_t current_num = 0;

    unordered_map<string, int> index_map;
    vector<symbol*> sym_tab;
};


#endif