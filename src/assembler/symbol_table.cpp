#include <assembler/symbol_table.hpp>
#include <iostream>
#include <iomanip>


symbol_table::sym_tab_row* symbol_table::get_symbol_by_name(string& sym_name)
{
    auto it = index_map.find(sym_name);
    if (it == index_map.end())
        return nullptr;
    
    int index = it->second;
    return sym_tab[index];
}


vector<symbol_table::sym_tab_row*>& symbol_table::get_symbols()
{
    return sym_tab;
}


int symbol_table::insert_symbol(uint32_t value, TYPE type, BIND bind, string& sym_name, bool extern_sym, uint32_t ndx)
{
    sym_tab_row* sym = get_symbol_by_name(sym_name);
    if (sym != nullptr)
        return -1;
    
    if (type == TYPE::NOTYP && !extern_sym)
    {
        // Invalid ndx
        if (sym_tab.size() < ndx)
            return -2;

        // Symbol at ndx not SCTN type
        if (sym_tab[ndx]->type != TYPE::SCTN)
            return -3;
    }

    // SCTN refers to itself
    uint32_t index = type == TYPE::NOTYP ? ndx : current_num;

    sym = new sym_tab_row(current_num, value, type, bind, index, sym_name);
    sym_tab.push_back(sym);
    index_map[sym_name] = current_num++;

    return 0;
}


int symbol_table::set_section_size(string& section_name, uint32_t size)
{
    sym_tab_row* sym = get_symbol_by_name(section_name);
    if (sym == nullptr)
        return -1;

    sym->size = size;
    return 0;
}


symbol_table::symbol_table()
{   
    string sym_name_und("");
    sym_tab_row* sym = new sym_tab_row(current_num, 0, TYPE::NOTYP, BIND::LOC, NDX::UND, sym_name_und);
    sym_tab.push_back(sym);
    index_map[sym_name_und] = current_num++;
}


symbol_table::~symbol_table()
{
    for (sym_tab_row* row:sym_tab)
        delete row;
}


void symbol_table::print_sym_tab()
{
    cout << ".symtab\n\n";

    cout << left
              << setw(5)  << "Num"
              << setw(12) << "Value"
              << setw(8)  << "Size"
              << setw(10) << "Type"
              << setw(10) << "Bind"
              << setw(8)  << "Ndx"
              << "Name"
              << endl;

    cout << string(60, '-') << endl;

    for (sym_tab_row* sym : sym_tab)
    {
        string type;
        switch (sym->type)
        {
            case NOTYP:
                type = "NOTYP";
                break;
            case SCTN:
                type = "SCTN";
                break;
        }

        string bind;
        switch (sym->bind)
        {
            case LOC:
                bind = "LOC";
                break;
            case GLOB:
                bind = "GLOB";
                break;
        }

        cout << left
                  << setw(5)  << sym->num
                  << setw(12) << sym->value
                  << setw(8)  << sym->size
                  << setw(10) << type
                  << setw(10) << bind
                  << setw(8)  << sym->ndx
                  << sym->name
                  << endl;
    }
}
