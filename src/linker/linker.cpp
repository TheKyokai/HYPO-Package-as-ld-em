#include <linker/linker.hpp>

#include <iostream>
#include <fstream>
#include <algorithm>

linker::~linker()
{
    for (ld_obj* o:input_objs)
    {
        for (ld_section* sctn:o->sections)
            delete sctn;
        delete o;
    }
}

void linker::link_input_obj()
{
    for (string& input_file:options.input_files)
        if (!read_obj_file(input_file))
            {
                cerr << "No output files will be generated" << endl;   
                return;
            }
    
    if (!layout())
    {
        cerr << "No output files will be generated" << endl;   
        return;
    }

    if (!resolve_symbols())
    {
        cerr << "No output files will be generated" << endl;   
        return;
    }

    symtab.print_sym_tab(cout);

    for (ld_section* sctn:sections)
        sctn->print_section(cout);

}


bool linker::read_obj_file(string obj_file)
{
    ifstream obj(obj_file, ios::binary);

    if (!obj)
    {
        cerr << "Cannot open object file: " << obj_file << endl;
        return false;
    }

    // Header
    bin_hdr header;
    obj.read(
        reinterpret_cast<char*>(&header),
        sizeof(header)
    );

    if (!obj)
    {
        cerr << "Failed to read object header from " << obj_file << endl;
        return false;
    }

    if (header.magic != 0x41534D4F)
    {
        cerr << obj_file << " is not a valid object file." << endl;
        return false;
    }

    ld_obj* input = new ld_obj();
    input->filename = obj_file;
    
    // Section headers
    vector<ld_section::section_hdr> section_headers(header.section_count);

    obj.seekg(header.section_table_offset);

    obj.read(
        reinterpret_cast<char*>(section_headers.data()),
        header.section_count * sizeof(ld_section::section_hdr)
    );

    
    // Symbol table
    vector<ld_symbol_table::symbol_hdr> symbol_headers(header.symbol_count);

    obj.seekg(header.symbol_table_offset);

    obj.read(
        reinterpret_cast<char*>(symbol_headers.data()),
        header.symbol_count * sizeof(ld_symbol_table::symbol_hdr)
    );

    
    // String table
    uint32_t string_table_size = header.relocation_table_offset - header.string_table_offset;

    vector<char> string_table(string_table_size);

    obj.seekg(header.string_table_offset);

    obj.read(
        string_table.data(),
        string_table_size
    );


    // Symbol table reconstruction
    for (auto& sh : symbol_headers)
    {
        string name(&string_table[sh.name_offset]);

        input->symtab.insert_symbol(
            sh.value,
            static_cast<ld_symbol_table::TYPE>(sh.type),
            static_cast<ld_symbol_table::BIND>(sh.bind),
            name,
            sh.ndx
        );

        input->symtab.set_section_size(name, sh.size);
    }

    // Section reconstruction
    for (size_t i = 0; i < section_headers.size(); i++)
    {
        auto& sh = section_headers[i];

        ld_section* section =
            new ld_section(sh.name, i + 1);

        obj.seekg(header.data_offset + sh.data_offset);

        for (uint32_t j = 0; j < sh.size; j++)
        {
            char byte;
            obj.get(byte);

            section->emit_byte(
                static_cast<uint8_t>(byte)
            );
        }

        obj.seekg(header.relocation_table_offset + sh.relocation_offset);

        for (uint32_t j = 0; j < sh.relocation_count; j++)
        {
            ld_section::rela_entry rel(0,
                                       ld_section::R_32,
                                       0,
                                       0);

            obj.read(
                reinterpret_cast<char*>(&rel),
                sizeof(rel)
            );

            section->insert_rela_entry(
                rel.offset,
                rel.type,
                rel.symbol,
                rel.addend
            );
        }

        input->sections.push_back(section);
    }

    input_objs.push_back(input);
    return true;
}


bool linker::layout()
{
    // Merging sections with the same name
    for (ld_obj* obj:input_objs)
        if (!layout_single_obj(obj))    
            return false;
    
    // Nothing more needed for -relocatable
    if (options.relocatable)
        return true;

    // Placing section if -hex 
    vector<ld_section*> placed;
    for (section_placement& placement:options.placements)
    {
        ld_section* section = nullptr;
        for (ld_section* sctn:sections)
            if (sctn->get_name() == placement.section_name)
                section = sctn;
        if (section)
            section->set_address(placement.address);
        else 
        {
            cerr << "Unkown section used in -place '" << placement.section_name << '\n';
            continue;
        }
        
        uint32_t section_start = section->get_address();
        uint32_t section_end = section_end + section->get_size();

        for (ld_section* other:placed)
        {
            uint32_t other_start = other->get_address();
            uint32_t other_end = other_start + other->get_size();

            if (section_start < other_end && other_start < section_end)
                return false;
        }
        placed.push_back(section);
        highest_free_after_data = max(highest_free_after_data, section->get_address() + static_cast<uint32_t>(section->get_size()));
    }
    for (ld_section* section:sections)
    {
        if (find(placed.begin(), placed.end(), section) != placed.end())
            continue; // Already placed
        section->set_address(highest_free_after_data);
        highest_free_after_data += section->get_size();
        placed.push_back(section);
    }
    
    return true;
}

bool linker::layout_single_obj(ld_obj* obj)
{
    for (ld_section* sctn:obj->sections)
    {
        ld_section* first = nullptr;
        for (ld_section* nsctn:sections)
            if (nsctn->get_name() == sctn->get_name())
                first = nsctn;
        if (!first)
            sections.push_back(sctn);
        else
        {
            sctn->set_first_section(first);
            sctn->set_address(first->get_size());
            first->expand_data(sctn->get_data());
        }
    }
    return true;
}


bool linker::resolve_symbols()
{
    uint32_t current_ndx = 1;
    for (ld_section* section:sections)
    {
        symtab.insert_symbol(section->get_address(), ld_symbol_table::TYPE::SCTN, ld_symbol_table::BIND::LOC, section->get_name());
        section->set_ndx(current_ndx);
    }
    for (ld_obj* obj:input_objs)
        if (!resolve_single_obj_symbols(obj))
            return false;
    return true;
}

bool linker::resolve_single_obj_symbols(ld_obj* obj)
{
    bool first = true, resolution_error = false;
    for (ld_symbol_table::symbol* sym:obj->symtab.get_symbols())
    {
        if (first)    
        {
            first = false;
            continue;
        }

        // Sections are already in the symtab
        if (sym->bind != ld_symbol_table::BIND::GLOB)
            continue;
        
        if (sym->ndx == ld_symbol_table::NDX::UND)
        {
            // Nothing to do even if already defined
            symtab.insert_symbol(sym->value, sym->type, sym->bind, sym->name, sym->ndx);
            continue;
        }

        if (sym->ndx == ld_symbol_table::NDX::ABS)
        {
            int status = symtab.insert_symbol(sym->value, sym->type, sym->bind, sym->name, sym->ndx);
            if (status == -1)
            {
                cerr << "Multiple definitions of symbol '" << sym->name << "'\n";
                resolution_error = true;
            }
            continue;
        }

        
        string section_name = obj->symtab.get_symbols()[sym->ndx]->name;
        ld_section* original_section = get_section_by_name(obj->sections, section_name);
        ld_section* current_section = get_section_by_name(sections, section_name);

        uint32_t value = current_section->get_address() + original_section->get_address() + sym->value;
        uint32_t ndx = current_section->get_ndx();

        ld_symbol_table::symbol* found_sym = symtab.get_symbol_by_name(sym->name);
        if (found_sym && found_sym->ndx == ld_symbol_table::NDX::UND)
        {
            found_sym->value = value;
            found_sym->ndx = ndx;
            continue;
        }
        if (found_sym)
        {
            cerr << "Multiple definitions of symbol '" << sym->name << "'\n";
            resolution_error = true;
            continue;
        }
        symtab.insert_symbol(value, sym->type, sym->bind, sym->name, ndx);
    }
    return !resolution_error;
}

bool linker::resolve_relocation()
{
    return true;
}

bool linker::resolve_single_obj_relocation(ld_obj* obj)
{
    return true;
}

ld_section* linker::get_section_by_name(vector<ld_section*>& sections, string& section_name)
{
    ld_section* section = nullptr;
    for (ld_section* sctn:sections)
        if (sctn->get_name() == section_name)
        {
            section = sctn;
            break;
        }
    return section;
}
