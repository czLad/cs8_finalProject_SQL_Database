#ifndef ZAC_TABLE_H
#define ZAC_TABLE_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <string>
#include <cassert>
#include "typedefs.h"
#include "../stl_utils/vector_utilities.h"
#include "../token/token.h"
#include "../queue/MyQueue.h"
#include "../token/relational.h"
#include "../token/logical.h"
#include "../token/token_str.h"
#include "../token/leftparen.h"
#include "../token/rightparen.h"
#include "../binary_files/utilities.h"
#include "table_constants.h"
#include "../rpn/rpn.h"
#include "../shunting_yard/shunting_yard.h"
#include "../binary_files/file_record.h"

using namespace std;

class Table{
public:
    static int serial;
    Table();
    Table(const string& str, const vectorstr& string_vec);
    Table(const string& str);
    void insert_into(const vectorstr& insert_vec);
    friend ostream& operator<<(ostream& outs,
                               const Table& print_me);
    Table select_all();
    Table select_all(vectorstr field);
    Table select(vectorstr string_vec, const string& field, const string& relational, const string& condition);
    Table select(vectorstr string_vec, vectorstr condition) throw(Error_Code);
    Table select(vectorstr condition) throw(Error_Code);
    vector<long> select_recnos();
    Table select(vectorstr string_vec, Queue<Token*> token_q);
    void print_field_names(ostream& outs=cout) const;
    Table vector_to_table(const vector<long>& build_vector, const vectorstr& field_name_vec);
    void set_tablenames_table(bool tablenames_table);
    bool get_tablenames_table(){return _tablenames_table;}
    friend Table operator + (const Table& lhs, const Table& rhs)
    {
        //combinig keys
        return Table();
    }
private:
    string _table_name;
    string _txt_filename;
    string _bin_filename;
    string _rec_count_filename;  
    vector<mmap_sl> _record_indicies;
    vectorstr _field_name_vec;
    map_sl _field_indicies;
    int _record_count;
    int _last_record_number;
    int _field_max_len;
    int _field_count;
    vector<long> _build_vector;
    bool _tablenames_table;
    void create_field_indicies(map_sl& field_i_s);
    void init_record_indicies_vector(vector<mmap_sl>& list);
    void create_record_indicies(vector<mmap_sl>& record_i_s, const string& bin_fi_name);
    void push_into_attribute_mmaps(vectorstr insert_vec, const long& recno);
    void push_into_attribute_mmaps(char insert_record_arr[][FIELD_MAX_LEN], const long& recno);
    int get_init_record_count();
    vectorstr vec_from_record(char insert_record_arr[][FIELD_MAX_LEN], const vectorstr& field_vector);

};


#endif // ZAC_TABLE_