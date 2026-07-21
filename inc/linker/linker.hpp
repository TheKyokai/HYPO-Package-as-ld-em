#ifndef LINKER_HPP
#define LINKER_HPP


#include <string>
#include <vector>
#include <linker/ld_options.hpp>
#include <linker/ld_symbol_table.hpp>
#include <linker/ld_section.hpp>

using namespace std;

class linker
{
public:

    linker(linker_options& options): options(options) {}

    ~linker();

    void link_input_obj();

private:

    linker_options options;
    bool ld_error_detected = false;

    struct ld_obj
    {
        string filename;
        ld_symbol_table symtab;
        vector<ld_section*> sections; 
    };

    vector<ld_obj*> input_objs;

    // Linked obj
    ld_symbol_table symtab;
    vector<ld_section*> sections; 

    struct bin_hdr
    {
        uint32_t magic; // should be ASMO

        uint32_t section_count;
        uint32_t symbol_count;
        uint32_t relocation_count;

        uint32_t section_table_offset;
        uint32_t symbol_table_offset;
        uint32_t string_table_offset;
        uint32_t relocation_table_offset;
        uint32_t data_offset;

        uint32_t file_size;
    };

    bool read_obj_file(string obj_file);


    uint32_t highest_free_after_data = 0;
    bool layout();
    bool layout_single_obj(ld_obj* obj);

    bool resolve_symbols();
    bool resolve_single_obj_symbols(ld_obj* obj);

    bool resolve_relocation();
    bool resolve_single_obj_relocation(ld_obj* obj);

    ld_section* get_section_by_name(vector<ld_section*>& sections, string& section_name);
};



#endif