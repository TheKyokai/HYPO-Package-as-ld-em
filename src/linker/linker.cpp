#include <linker/linker.hpp>

#include <iostream>
#include <fstream>
#include <iomanip>
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

    if (!resolve_relocation())
    {
        cerr << "No output files will be generated" << endl;   
        return;
    }

    //DEBUG PRINT
    // symtab.print_sym_tab(cout);

    // for (ld_section* sctn:sections)
    //     sctn->print_section(cout);
    
    if (options.hex)
        generate_hex();
    else
        generate_relocatable();

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
        section->set_ndx(current_ndx++);
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

        uint32_t value = current_section->get_address() + sym->value;
        if (current_section != original_section)
            value += original_section->get_address();
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
    for (ld_obj* obj:input_objs)
        if (!resolve_single_obj_relocation(obj))
            return false;
    return true;
}

bool linker::resolve_single_obj_relocation(ld_obj* obj)
{
    bool resolution_error = false;
    for (ld_section* section:obj->sections)
    {
        
        ld_section* current_section = section->get_first_section();
        if (!current_section)
            current_section = section;

        for (ld_section::rela_entry& entry:section->get_rela())
        {
            if (entry.type != ld_section::RELA_TYPE::R_32 && entry.type != ld_section::RELA_TYPE::R_32L)
            {
                cerr << "Unknown relocation type" << endl;
                resolution_error = true;
                continue;
            }
            
            ld_symbol_table::symbol* old_symbol = obj->symtab.get_symbols()[entry.symbol];
            ld_symbol_table::symbol* symbol = symtab.get_symbol_by_name(old_symbol->name);
            uint32_t ndx = symtab.get_symbol_num(symbol->name);
            uint32_t old_offs = entry.offset;
            uint32_t addend_val = entry.addend;

            if (section == current_section)
            {
                entry.offset += current_section->get_address();
                entry.symbol = ndx;    
            }
            else
            {
                uint32_t new_offs = entry.offset + current_section->get_address() + section->get_address();
                old_offs += section->get_address();
                if (entry.type == ld_section::RELA_TYPE::R_32L)
                    addend_val += section->get_address();
                current_section->insert_rela_entry(new_offs, entry.type, ndx, addend_val);
            }

            uint32_t val = symbol->value + addend_val;
            current_section->get_data()[old_offs] = val & 0xFF;
            current_section->get_data()[old_offs + 1] = (val >> 8) & 0xFF;
            current_section->get_data()[old_offs + 2] = (val >> 16) & 0xFF;
            current_section->get_data()[old_offs + 3] = (val >> 24) & 0xFF;
        }
    }
    return !resolution_error;
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


void linker::generate_hex()
{
    bool first = true, undefined_sym = false;
    for (ld_symbol_table::symbol* symbol:symtab.get_symbols())
    {
        if (first)
        {
            first = false;
            continue;
        }
        if (symbol->ndx == ld_symbol_table::NDX::UND)
        {
            cerr << "Undefined symbol '" << symbol->name << "'\n";
            undefined_sym = true;
        }
    }
    if (undefined_sym)
    {
        cerr << "No output files will be generated" << endl;   
        return;
    }    
    

    string text_out;
    size_t dot_index = options.output_file.find_last_of('.');
    if (dot_index == string::npos)
        text_out = options.output_file + ".out";
    else
        text_out = options.output_file.substr(0, dot_index) + ".out";

    ofstream out(text_out);

    if (!out)
    {
        cerr << "Cannot open output file\n";
        return;
    }

    ofstream hex_out(options.output_file, ios::binary);

    if (!hex_out)
    {
        cerr << "Cannot open output file\n";
        return;
    }

    vector<ld_section*> sorted_sections = sections;

    sort(
        sorted_sections.begin(),
        sorted_sections.end(),
        [](ld_section* a, ld_section* b) { return a->get_address() < b->get_address(); }
    );


    uint32_t magic_num = 0x4D454D45;

    hex_out.write(
        reinterpret_cast<char*>(&magic_num),
        sizeof(magic_num)
    );


    // HEX Generation

    uint32_t current_row_addr = 0;
    uint8_t current_row[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    bool row_valid = false;


    for (ld_section* section : sorted_sections)
    {
        uint32_t address = section->get_address();
        vector<uint8_t>& data = section->get_data();

        for (size_t i = 0; i < data.size(); i++)
        {
            uint32_t byte_addr = address + i;
            uint32_t row_addr = byte_addr & ~0x7;
            uint32_t offset = byte_addr & 0x7;

            
            if (!row_valid)
            {
                current_row_addr = row_addr;
                row_valid = true;
            }
            else if (row_addr != current_row_addr)
            {
                write_row(out, hex_out, current_row_addr, current_row, row_valid);
                current_row_addr = row_addr;
                row_valid = true;
            }

            current_row[offset] = data[i];
        }
    }

    // Write row
    write_row(out, hex_out, current_row_addr, current_row, row_valid);


    // Generation done
    out.close();
    hex_out.close();
}

void linker::write_row(ofstream& out, ofstream& hex_out, uint32_t row_addr, uint8_t row[], bool& row_valid)
{
    if (!row_valid)
            return;

    // Out
    out << "0x"
        << uppercase
        << hex
        << setw(8)
        << setfill('0')
        << row_addr
        << ": ";

    for (int i = 0; i < 8; i++)
    {
        out << setw(2)
            << static_cast<int>(row[i]);

        if (i != 7)
            out << " ";
    }

    out << '\n';


    // Hex
    hex_out.write(
        reinterpret_cast<char*>(&row_addr),
        sizeof(row_addr)
    );

    hex_out.write(
        reinterpret_cast<char*>(row),
        sizeof(uint8_t) * 8
    );

    row_valid = false;
    for (size_t i = 0; i < 8; i++)
        row[i] = 0x00;
}

void linker::generate_relocatable()
{
    string text_out;
    size_t dot_index = options.output_file.find_last_of('.');
    if (dot_index == string::npos)
        text_out = options.output_file + ".out";
    else
        text_out = options.output_file.substr(0, dot_index) + ".out";

    ofstream out(text_out);

    if (!out)
    {
        cerr << "Unable to open out file\n";
        return;
    }

    symtab.print_sym_tab(out);

    for (ld_section* sctn : sections)
        sctn->print_section(out);

    out.close();

    ofstream obj(options.output_file, ios::binary);

    if (!obj)
    {
        cerr << "Cannot open object file\n";
        return;
    }

    bin_hdr header;
    header.magic = 0x41534D4F;
    header.section_count = sections.size();
    header.symbol_count = symtab.get_symbols().size();
    header.relocation_count = 0;
    for (ld_section* sctn:sections)
    {
        header.relocation_count += sctn->get_rela().size();
    }

    header.file_size = -1;

    obj.write(
        reinterpret_cast<char*>(&header),
        sizeof(header)
    );

    vector<ld_section::section_hdr> section_headers;
    for (ld_section* sctn:sections)
    {
        ld_section::section_hdr shdr = sctn->get_header();
        section_headers.push_back(shdr);
        obj.write(
            reinterpret_cast<char*>(&shdr),
            sizeof(shdr)
        );
    }
    
    header.symbol_table_offset = obj.tellp();
    header.string_table_offset = generate_symtab_bin(obj);
    header.relocation_table_offset = obj.tellp();

    uint32_t current_rela_offset = 0;
    for (size_t i = 0; i < section_headers.size(); i++)
    {
        ld_section::section_hdr& shdr = section_headers[i];
        ld_section* sctn = sections[i];
        
        shdr.relocation_offset = current_rela_offset;
        for (ld_section::rela_entry& entry:sctn->get_rela())
        {
            obj.write(
                reinterpret_cast<char*>(&entry),
                sizeof(entry)
            );
            current_rela_offset += sizeof(entry);
        }
    }

    header.data_offset = obj.tellp();

    uint32_t current_data_offset = 0;
    for (size_t i = 0; i < section_headers.size(); i++)
    {
        ld_section::section_hdr& shdr = section_headers[i];
        ld_section* sctn = sections[i];
        
        shdr.data_offset = current_data_offset;
        for (uint8_t& data_byte:sctn->get_data())
        {
            obj.put(data_byte);
            current_data_offset++;
        }
    }

    header.file_size = obj.tellp();


    // Offset fixup
    obj.seekp(20);
    obj.write(
        reinterpret_cast<char*>(&header.symbol_table_offset),
        sizeof(header.symbol_table_offset)
    );
    obj.write(
        reinterpret_cast<char*>(&header.string_table_offset),
        sizeof(header.string_table_offset)
    );
    obj.write(
        reinterpret_cast<char*>(&header.relocation_table_offset),
        sizeof(header.relocation_table_offset)
    );
    obj.write(
        reinterpret_cast<char*>(&header.data_offset),
        sizeof(header.data_offset)
    );
    obj.write(
        reinterpret_cast<char*>(&header.file_size),
        sizeof(header.file_size)
    );

    for (size_t i = 0; i < section_headers.size(); i++)
    {
        ld_section::section_hdr& shdr = section_headers[i];
        obj.seekp(header.section_table_offset + i * sizeof(ld_section::section_hdr) + sizeof(shdr.name) + sizeof(shdr.relocation_count) + sizeof(shdr.size));
        obj.write(
            reinterpret_cast<char*>(&shdr.relocation_offset),
            sizeof(shdr.relocation_offset)
        );
        obj.write(
            reinterpret_cast<char*>(&shdr.data_offset),
            sizeof(shdr.data_offset)
        );
    }

    obj.close();
}

uint32_t linker::generate_symtab_bin(ostream& obj)
{
    vector<char> string_table;
    for (ld_symbol_table::symbol* sym:symtab.get_symbols())
    {
        ld_symbol_table::symbol_hdr symhdr;
        symhdr.name_offset = string_table.size();
        for (char& c:sym->name)
            string_table.push_back(c);
        string_table.push_back('\0');

        symhdr.value = sym->value;
        symhdr.size = sym->size;
        symhdr.ndx = sym->ndx;
        symhdr.type = static_cast<uint8_t>(sym->type);
        symhdr.bind = static_cast<uint8_t>(sym->bind);

        obj.write(
            reinterpret_cast<char*>(&symhdr),
            sizeof(symhdr)
        );
    }

    uint32_t string_table_offset = obj.tellp();


    for (char& c:string_table)
        obj.put(c);
    
    return string_table_offset;
}

