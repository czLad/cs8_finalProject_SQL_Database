#ifndef ZAC_SQL_H
#define ZAC_SQL_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <string>
#include <cassert>
#include "../table/table.h"
#include "../parser/parser.h"
#include "../error_code/error_code.h"

using namespace std;

class SQL{
public:
    SQL();
    Table command(string cmd_str);
    vectorlong select_recnos(); 
    bool error_state(){return _error_state;}
    void print_tables_names();
    void batch();
private:
    mmap_ss _ptree;
    vectorlong _select_recnos;
    Map<string, Table> _tables;
    string _sql_table_names_txt;
    bool _error_state;
    void sql_init_write_to_file_txt(string filename);
    Table get_tablenames_table();
};


#endif // ZAC_SQL_