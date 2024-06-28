#ifndef PARSER_STATE_MACHINE_CPP
#define PARSER_STATE_MACHINE_CPP

#include "iomanip"
#include "iostream"
#include "string.h"
#include "parser_state_machine_functions.h"

using namespace std;

//Fill all cells of the array with -1
void init_table(int _table[][MAX_COLUMNS_PARSER])
{
    for(int i = 0; i < MAX_ROWS_PARSER; i++)
        for(int j = 0; j < MAX_COLUMNS_PARSER; j++)
            _table[i][j] = -1;
}

//v Functions Need testing

//Mark this state (row) with a 1 (success)
void mark_success(int _table[][MAX_COLUMNS_PARSER], int state)
{
    _table[state][0] = 1;
}

//Mark this state (row) with a 0 (fail)
void mark_fail(int _table[][MAX_COLUMNS_PARSER], int state)
{
    _table[state][0] = 0;
}

//true if state is a success state
bool is_success(int _table[][MAX_COLUMNS_PARSER], int state)
{
    return _table[state][0];
}

//Mark a range of cells in the array. 
void mark_cells(int row, int _table[][MAX_COLUMNS_PARSER], int from, int to, int state)
{
    for(int j = from; j <= to; j++)
        _table[row][j] = state;
}

//Mark columns represented by the string columns[] for this row
void mark_cells(int row, int _table[][MAX_COLUMNS_PARSER], const char columns[], int state)
{
    for(int j = 0; j < strlen(columns); j++)
        _table[row][columns[j]] = state;
}

//Mark this row and column
void mark_cell(int row, int table[][MAX_COLUMNS_PARSER], int column, int state)
{
    table[row][column] = state;
}

//This can realistically be used on a small table
void print_table(int _table[][MAX_COLUMNS_PARSER])
{
    for(int i = 0; i < MAX_ROWS_PARSER; i++)
    {
        cout<<"[|";
        for(int j = 0; j < MAX_COLUMNS_PARSER; j++)
        {
            cout <<_table[i][j]<<"|";
        }
        cout<<"]\n";
    }
}

void print_table(int _table[][MAX_COLUMNS_PARSER], int row_from, int row_to ,int col_from, int col_to)
{
    for(int i = row_from; i <= row_to; i++)
    {
        cout<<"[|";
        for(int j = col_from; j <= col_to; j++)
        {
            cout <<_table[i][j]<<"|";
        }
        cout<<"]\n";
    }
}

//show string s and mark this position on the string:
//hello world   pos: 7
//       ^
void parser_show_string(char s[], int _pos)
{
    cout <<"string:\n";
    cout <<s<<"\n";
    for(int i = 0; i < strlen(s); i++)
    {
        if(i == _pos)
            cout << "^\n";
    }
}

#endif