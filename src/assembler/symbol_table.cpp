#include <assembler/symbol_table.hpp>
#include <iostream>
#include <iomanip>


symbol_table::symbol* symbol_table::get_symbol_by_name(string& sym_name)
{
    auto it = index_map.find(sym_name);
    if (it == index_map.end())
        return nullptr;
    
    int index = it->second;
    return sym_tab[index];
}

vector<symbol_table::symbol*>& symbol_table::get_symbols()
{
    return sym_tab;
}

int symbol_table::get_symbol_num(string& sym_name)
{
    auto it = index_map.find(sym_name);
    if (it == index_map.end())
        return -1;
    
    return it->second;
}


int symbol_table::insert_symbol(uint32_t value, TYPE type, BIND bind, string& sym_name, uint32_t ndx, bool defined)
{
    symbol* sym = get_symbol_by_name(sym_name);
    if (sym != nullptr)
        return -1;
    
    // Defined non-SCTN, non-extern symbol (extern: UND - defined)
    if (type == TYPE::NOTYP && defined && ndx != NDX::UND)
    {
        // Invalid ndx
        if (ndx > sym_tab.size())
            return -2;

        // Symbol at ndx not SCTN type
        if (sym_tab[ndx]->type != TYPE::SCTN)
            return -3;
    }

    // SCTN refers to itself
    uint32_t index = type == TYPE::NOTYP ? ndx : current_num;

    sym = new symbol(current_num, value, type, bind, index, sym_name, defined);
    sym_tab.push_back(sym);
    index_map[sym_name] = current_num++;

    return 0;
}


int symbol_table::set_section_size(string& section_name, uint32_t size)
{
    symbol* sym = get_symbol_by_name(section_name);
    if (sym == nullptr)
        return -1;

    sym->size = size;
    return 0;
}


int symbol_table::insert_forward_reference_entry(string& referenced_symbol, string& sctn, uint32_t patch_address)
{
    symbol* sym = get_symbol_by_name(referenced_symbol);
    if (sym == nullptr)
        return -1;
    
    if (sym->forward_reference_table == nullptr)
    {
        sym->forward_reference_table = new vector<symbol::forward_reference_entry*>;
        if (sym->forward_reference_table == nullptr)
            return -2;
    }
    symbol::forward_reference_entry* entry = new symbol::forward_reference_entry(sctn, patch_address);
    sym->forward_reference_table->push_back(entry);
    return 0;
}

symbol_table::symbol_table()
{   
    string sym_name_und("");
    symbol* sym = new symbol(current_num, 0, TYPE::NOTYP, BIND::LOC, NDX::UND, sym_name_und, true);
    sym_tab.push_back(sym);
    index_map[sym_name_und] = current_num++;
}


symbol_table::~symbol_table()
{
    for (symbol* sym:sym_tab)
    {
        if (sym->forward_reference_table)
        {
            for (symbol::forward_reference_entry* entry: *sym->forward_reference_table)
                delete entry;
            delete sym->forward_reference_table;
        }
        delete sym;
    }
}


void symbol_table::print_sym_tab(ostream& os)
{
    os << ".symtab\n\n";

    os << left
              << setw(5)  << "Num"
              << setw(12) << "Value"
              << setw(8)  << "Size"
              << setw(10) << "Type"
              << setw(10) << "Bind"
              << setw(8)  << "Ndx"
              << setw(10) << "Defined"
              << "Name"
              << endl;

    os << string(72, '-') << endl;

    for (symbol* sym : sym_tab)
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

        string ndx;
        switch (sym->ndx)
        {
        case UND:
            ndx = "UND";
            break;
        case ABS:
            ndx = "ABS";
            break;
        default:
            ndx = to_string(sym->ndx);
            break;
        }

        os << left
             << setw(5) << sym->num;

        os << "0x"
             << uppercase << hex
             << setw(8) << setfill('0') << right << sym->value;

        os << dec << nouppercase << setfill(' ') << left;

        os << "  "
             << setw(8)  << sym->size
             << setw(10) << type
             << setw(10) << bind
             << setw(8)  << ndx
             << setw(10) << (sym->defined ? "YES" : "NO")
             << sym->name
             << '\n';
    }

    os << endl;
}


