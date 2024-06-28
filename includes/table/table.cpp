#ifndef ZAC_TABLE_CPP
#define ZAC_TABLE_CPP

#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <string>
#include <cassert>
#include "table.h"

using namespace std;

Table::Table()
{
    const bool debug = false;
    if (debug)
    {
        cout << "Table CTOR Fired.\n";
    }
    _tablenames_table = false;
}
Table::Table(const string &str, const vectorstr &string_vec)
{
    // str is name of the table //string_vec is the attributes in there
    _tablenames_table = false;
    const bool light_hearted_debug = false;
    if(light_hearted_debug)
        cout << "Two argument table CTOR\n";
    const bool debug = false;
    if (debug)
    {
        cout << "Table (const string& str, const vectorstr& string_vec) CTOR Fired.\n";
    }
    if (debug)
        cout << "serial: " << serial << "\n";
    _table_name = str;
    _txt_filename = str + "_fields.txt";
    _bin_filename = str + "_fields.bin";
    // v old init rec_count mechanism
    //  _rec_count_filename = str + "_rec_count.txt";
    //^
    fstream f;
    FileRecord r_temp;
    _field_max_len = r_temp.MAX + 1;
    init_create_file(f, (_bin_filename).c_str());
    // takes the string_vec and writes all the field names to txt file
    // if file already exists will write it twice
    init_write_to_file_txt(_txt_filename, string_vec);
    // create a file to keep track of record count in tb
    // v old init rec_count mechanism
    //  init_write_to_file_txt(_rec_count_filename, {"0"});
    // get rec count
    //  _record_count = get_init_record_count();
    //^
    // v new init rec count mechanism
    _record_count = 0;
    _last_record_number = _record_count - 1;
    // get field count
    vectorstr field_name_vec = read_from_file_txt(_txt_filename);
    _field_count = field_name_vec.size();
    // get field names into a vector
    _field_name_vec = read_from_file_txt(_txt_filename);
    // multimap of string keys and indicies longs
    init_record_indicies_vector(_record_indicies);
    // map doesn't care about size so create _field_indicies map
    create_field_indicies(_field_indicies);
    // read the record inidicies back from bin
    // builds the indicies structure for records if there are records
    create_record_indicies(_record_indicies, _bin_filename);
    if (debug)
        cout << _field_indicies << "\n";
}
Table::Table(const string &str)
{
    _tablenames_table = false;
    const bool light_hearted_debug = false;
    if(light_hearted_debug)
        cout << "One argument table CTOR\n";
    const bool debug = false;
    if (debug)
    {
        cout << "Table (const string& str) Fired.\n";
    }
    if (debug)
        cout << "serial: " << serial << "\n";
    _table_name = str;
    _txt_filename = str + "_fields.txt";
    _bin_filename = str + "_fields.bin";
    // _rec_count_filename = str + "_rec_count.txt";
    fstream f;
    FileRecord r_temp;
    _field_max_len = r_temp.MAX + 1;
    // get rec count
    _record_count = get_init_record_count();
    _last_record_number = _record_count - 1;
    // get field count
    vectorstr field_name_vec = read_from_file_txt(_txt_filename);
    _field_count = field_name_vec.size();
    // get field names into a vector
    _field_name_vec = read_from_file_txt(_txt_filename);
    // multimap of string keys and indicies longs
    init_record_indicies_vector(_record_indicies);
    // map doesn't care about size so create _field_indicies map
    create_field_indicies(_field_indicies);
    // read the record inidicies back from bin
    // builds the indicies structure for records if there are records
    create_record_indicies(_record_indicies, _bin_filename);
    if (debug)
        cout << _field_indicies << "\n";
}
void Table::insert_into(const vectorstr &insert_vec)
{
    // write to bin file
    // get record no from it
    const bool debug = false;
    fstream f;
    open_fileRW(f, _bin_filename.c_str());
    FileRecord r(insert_vec);
    long recno = r.write(f);
    if (debug)
        cout << "recno: " << recno << "\n";
    // arithmetic should work out
    // push the key record num into mmap of each field
    // save the recno + 1 to know total record count in table
    // v old init rec_count mechanism
    //  write_to_file_txt(_rec_count_filename, {to_string(recno+1)});
    //^
    _record_count++;
    _last_record_number++;
    push_into_attribute_mmaps(insert_vec, recno);
    if (debug)
    {
        for (int i = 0; i < insert_vec.size(); i++)
        {
            // this works?!?
            cout << "mmap of attributes field[" << i << "]:\n"
                 << _record_indicies[i] << "\n";
        }
    }
}

ostream &operator<<(ostream &outs,
                    const Table &print_me)
{
    if(!print_me._tablenames_table)
    {
        outs << "Table name: " << print_me._table_name << ", records: " << print_me._record_count << "\n";
        fstream f;
        outs << setw(20) << "record";
        print_me.print_field_names(outs);
        outs << "\n";
        open_fileRW(f, print_me._bin_filename.c_str());
        for (int i = 0; i < print_me._record_count; i++)
        {
            FileRecord r;
            r.read(f, i);
            outs << setw(20) << i << r;
        }
    }
    else
    {
        fstream f;
        open_fileRW(f, print_me._bin_filename.c_str());
        outs<<"--Tables SQL is managing--\n";
        for (int i = 0; i < print_me._record_count; i++)
        {
            FileRecord r;
            r.read(f, i);
            for(int i = 0; i < r.ROW; i++)
                outs<<r._record[i];
            outs<<"\n";
        }
        outs<<"--------------------------\n\n";
    }
    // outs<<"record numbers from original table: "<<print_me._orig_recno_vec<<"\n";
    return outs;
}
Table Table::select_all()
{
    // put inidicies in vector
    // put the vector throught vector_to_table func and return
    _build_vector.clear();
    for (long i = 0; i < _record_count; i++)
    {
        _build_vector.push_back(i);
    }
    // pass in a field name vector as well into vector_to_table
    // read from text_file only for select_all
    vectorstr field_name_vec = read_from_file_txt(_txt_filename);
    return vector_to_table(_build_vector, field_name_vec);
    // Table temp;
    // return temp;
}
// select all with just fields can be implemented like the func abouve
Table Table::select_all(vectorstr field)
{
    // put field into vector_to_table
    _build_vector.clear();
    for (long i = 0; i < _record_count; i++)
    {
        _build_vector.push_back(i);
    }
    // pass in a field name vector as well into vector_to_table
    // read from text_file only for select_all
    return vector_to_table(_build_vector, field);
}
Table Table::select(vectorstr string_vec, const string &field, const string &relational, const string &condition)
{
    const bool debug = false;
    // the string vec has field names
    //_record_indicies[_field_indicies[field]] <- this gives the right multimap
    //  _record_indicies[_field_indicies[field]][condition] <- this is getting the condition from the multimap
    _build_vector.clear();
    if (relational == "=")
    {
        _build_vector = _record_indicies[_field_indicies[field]][condition];
    }
    else if (relational == "<")
    {
        for (mmap_sl::Iterator it = _record_indicies[_field_indicies[field]].begin();
             it != _record_indicies[_field_indicies[field]].lower_bound(condition); ++it)
        {
            for (int i = 0; i < (*it).value_list.size(); i++)
                _build_vector.push_back((*it).value_list[i]);
        }
    }
    else if (relational == ">")
    {
        for (mmap_sl::Iterator it = _record_indicies[_field_indicies[field]].upper_bound(condition);
             it != _record_indicies[_field_indicies[field]].end(); ++it)
        {
            for (int i = 0; i < (*it).value_list.size(); i++)
                _build_vector.push_back((*it).value_list[i]);
        }
    }
    else if (relational == ">=")
    {
        for (mmap_sl::Iterator it = _record_indicies[_field_indicies[field]].lower_bound(condition);
             it != _record_indicies[_field_indicies[field]].end(); ++it)
        {
            for (int i = 0; i < (*it).value_list.size(); i++)
                _build_vector.push_back((*it).value_list[i]);
        }
    }
    else
    {
        // less than equal
        for (mmap_sl::Iterator it = _record_indicies[_field_indicies[field]].begin();
             it != _record_indicies[_field_indicies[field]].upper_bound(condition); ++it)
        {
            for (int i = 0; i < (*it).value_list.size(); i++)
                _build_vector.push_back((*it).value_list[i]);
        }
    }
    if (debug)
        cout << "_build_vector: " << _build_vector << "\n";
    // string vec is the field name vec
    return vector_to_table(_build_vector, string_vec);
}
vector<long> Table::select_recnos()
{
    return _build_vector;
}
Table Table::select(vectorstr string_vec, Queue<Token *> token_q)
{
    RPN rpn_1(token_q);
    _build_vector = rpn_1(_record_indicies, _field_indicies);
    return vector_to_table(_build_vector, string_vec);
}
Table Table::select(vectorstr string_vec, vectorstr condition) throw(Error_Code)
{
    const bool debug = false;
    Queue<Token *> infix;
    for (int i = 0; i < condition.size(); i++)
    {
        if (condition[i] == "(")
        {
            infix.push(new LParen);
        }
        else if (condition[i] == ")")
        {
            infix.push(new RParen);
        }
        else if (condition[i] == "<" || condition[i] == "<=" || condition[i] == ">" || condition[i] == ">=" || condition[i] == "=")
        {
            infix.push(new Relational(condition[i]));
        }
        else if (condition[i] == "and" || condition[i] == "or")
        {
            infix.push(new Logical(condition[i]));
        }
        else
        {
            infix.push(new TokenStr(condition[i]));
        }
    }
    ShuntingYard sy(infix);
    RPN rpn_1(sy.postfix());
    _build_vector = rpn_1(_record_indicies, _field_indicies);
    if (debug)
        cout << "_build_vector: " << _build_vector << "\n";
    // string vec is field_name vec
    return vector_to_table(_build_vector, string_vec);
}
Table Table::select(vectorstr condition) throw(Error_Code)
{
    const bool debug = false;
    Queue<Token *> infix;
    for (int i = 0; i < condition.size(); i++)
    {
        if (condition[i] == "(")
        {
            infix.push(new LParen);
        }
        else if (condition[i] == ")")
        {
            infix.push(new RParen);
        }
        else if (condition[i] == "<" || condition[i] == "<=" || condition[i] == ">" || condition[i] == ">=" || condition[i] == "=")
        {
            infix.push(new Relational(condition[i]));
        }
        else if (condition[i] == "and" || condition[i] == "or")
        {
            infix.push(new Logical(condition[i]));
        }
        else
        {
            infix.push(new TokenStr(condition[i]));
        }
    }
    ShuntingYard sy(infix);
    RPN rpn_1(sy.postfix());
    _build_vector = rpn_1(_record_indicies, _field_indicies);
    if (debug)
        cout << "_build_vector: " << _build_vector << "\n";
    // pass in field_name_vec
    return vector_to_table(_build_vector, _field_name_vec);
}
void Table::print_field_names(ostream &outs) const
{
    for (int i = 0; i < _field_name_vec.size(); i++)
    {
        outs << setw(20) << _field_name_vec[i];
    }
    outs << "\n";
}
Table Table::vector_to_table(const vector<long> &build_vector, const vectorstr &field_name_vec)
{
    // field_name_vec is the order of fields of the desired record
    // Table new_Table(_table_name + to_string(serial+1),  );
    serial++;
    Table temp(_table_name + "_" + to_string(serial), field_name_vec);
    fstream f;
    FileRecord r;
    open_fileRW(f, _bin_filename.c_str());
    for (int i = 0; i < build_vector.size(); i++)
    {
        r.read(f, build_vector[i]);
        // after reading will have array;
        // get the rows(fields) from the array;
        // create a insert_into vector with the two d array
        vectorstr insert_into_vec = vec_from_record(r._record, field_name_vec);
        temp.insert_into(insert_into_vec);
    }
    f.close();
    return temp;
}

// private
void Table::create_field_indicies(map_sl &field_i_s)
{
    const bool debug = false;
    // field_i_s is an empty vector of |maps|
    for (int i = 0; i < _field_name_vec.size(); i++)
    {
        field_i_s[_field_name_vec[i]] = i;
    }
    // can test by printing out field_i_s
    if (debug)
        cout << field_i_s << "\n";
}
void Table::init_record_indicies_vector(vector<mmap_sl> &list)
{
    _field_name_vec = read_from_file_txt(_txt_filename);
    for (int i = 0; i < _field_name_vec.size(); i++)
        list.push_back(mmap_sl());
}
void Table::create_record_indicies(vector<mmap_sl> &record_i_s, const string &bin_fi_name)
{
    // builds the indicies structure for records if there are records
    const bool debug = false;
    if (debug)
        cout << "_init_record_count: " << _record_count << "\n";
    // read each record from file
    // look at how many records we have
    fstream f;
    open_fileRW(f, bin_fi_name.c_str());
    for (int i = 0; i < _record_count; i++)
    {
        // i is recno
        FileRecord r;
        r.read(f, i);
        // push each record into the attribute mmaps
        push_into_attribute_mmaps(r._record, i);
    }
    f.close();
    if (debug)
    {
        vectorstr field_name_vec = read_from_file_txt(_txt_filename);
        for (int i = 0; i < field_name_vec.size(); i++)
        {
            cout << "initial mmap of attributes field[" << i << "]:\n"
                 << _record_indicies[i] << "\n";
        }
    }
}
void Table::push_into_attribute_mmaps(vectorstr insert_vec, const long &recno)
{
    const bool debug = false;
    for (int i = 0; i < insert_vec.size(); i++)
    {
        // this works?!?
        _record_indicies[i][insert_vec[i]] += recno;
        if (debug)
            cout << "mmap of attributes field[" << i << "]:\n"
                 << _record_indicies[i] << "\n";
    }
}
void Table::push_into_attribute_mmaps(char insert_record_arr[][FIELD_MAX_LEN], const long &recno)
{
    vectorstr field_name_vec = read_from_file_txt(_txt_filename);
    for (int i = 0; i < field_name_vec.size(); i++)
    {
        // this works?!?
        _record_indicies[i][insert_record_arr[i]] += recno;
        // if(debug)
        //     cout<<"mmap of attributes field["<<i<<"]:\n"<<_record_indicies[i]<<"\n";
    }
}
int Table::get_init_record_count()
{
    // vectorstr rec_count = read_from_file_txt(_rec_count_filename);
    // return stoi(rec_count[0]);
    const bool debug = false;
    fstream f;
    open_fileRW(f, _bin_filename.c_str());
    FileRecord r;
    for (_record_count = 0; r.read(f, _record_count); _record_count++)
    {
    }
    if (debug)
        cout << "_record_count: " << _record_count << "\n";
    return _record_count;
}
vectorstr Table::vec_from_record(char insert_record_arr[][FIELD_MAX_LEN], const vectorstr &field_vector)
{
    // get where each attribute in field vector is located in the current table
    const bool debug = false;
    vectorstr record_vector;
    // field_vector.size() has the new count of fields the new table wants
    for (int i = 0; i < field_vector.size(); i++)
    {
        // insert_record_arr is the 2D array from the disk
        // the 2D array's attribute locations will correspond to current table's _field_indicies
        // thus get the index location from field_indicies map by passing in the key(i.e. the field name you want to grab data from)
        // the key will be the order to push_back record into
        //  cout<<"field_vector[i]: "<<field_vector[i]<<"\n";
        record_vector.push_back(insert_record_arr[_field_indicies.at(field_vector[i])]); // i has to change
    }
    if (debug)
        cout << "record_vector: " << record_vector << "\n";
    // the returned record_vector should be ready for insert_into to just grab and insert
    return record_vector;
}
void Table::set_tablenames_table(bool tablenames_table)
{
    _tablenames_table = tablenames_table;
}

int Table::serial = 0;

#endif // ZAC_TABLE_