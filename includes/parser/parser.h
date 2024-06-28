#ifndef ZAC_PARSER_H
#define ZAC_PARSER_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <string>
#include <cassert>
#include "typedefs.h"
#include "../bplustree/multimap.h"
#include "parser_constants.h"
#include "parser_state_machine_functions.h"
#include "../tokenizer/stokenize.h"
#include "../error_code/error_code.h"

using namespace std;

class Parser{
public:
    Parser();
    Parser(char cmd_str[300]);
    mmap_ss parse_tree() throw(Error_Code);
    void set_string(char cmd_str[300]);


private:
    mmap_ss _ptree;
    map_sl _keywords_map;
    static int _table[MAX_ROWS_PARSER][MAX_COLUMNS_PARSER];
    bool fail;
    Queue<string>input_q;
    char _input_buffer[300];
    void make_table();
    void build_keyword_map();
};

#endif // ZAC_PARSER_