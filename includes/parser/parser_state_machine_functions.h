#ifndef PARSER_STATE_MACHINE_FUNCTIONS
#define PARSER_STATE_MACHINE_FUNCTIONS

#include "parser_constants.h"
#include "iomanip"
#include "iostream"

using namespace std;

//Fill all cells of the array with -1
void init_table(int _table[][MAX_COLUMNS_PARSER]);

//Mark this state (row) with a 1 (success)
void mark_success(int _table[][MAX_COLUMNS_PARSER], int state);

//Mark this state (row) with a 0 (fail) // unsuccessful state is more accurate than fail
void mark_fail(int _table[][MAX_COLUMNS_PARSER], int state);

//true if state is a success state
bool is_success(int _table[][MAX_COLUMNS_PARSER], int state);

//Mark a range of cells in the array. 
void mark_cells(int row, int _table[][MAX_COLUMNS_PARSER], int from, int to, int state);

//Mark columns represented by the string columns[] for this row
void mark_cells(int row, int _table[][MAX_COLUMNS_PARSER], const char columns[], int state);

//Mark this row and column
void mark_cell(int row, int table[][MAX_COLUMNS_PARSER], int column, int state);

//This can realistically be used on a small table
void print_table(int _table[][MAX_COLUMNS_PARSER]);

void print_table(int _table[][MAX_COLUMNS_PARSER], int from, int to);

//show string s and mark this position on the string:
//hello world   pos: 7
//       ^
void show_string(char s[], int _pos);

#endif