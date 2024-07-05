#ifndef ZAC_SQL_CPP
#define ZAC_SQL_CPP

#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <string>
#include <cassert>
#include "sql.h"

using namespace std;

SQL::SQL()
{
    //init_write_to_file_txt()
    const bool debug = false;
    _error_state = false;
    if(debug)
    {
        cout<<"SQL Default CTOR Fired.\n";
    }
    _sql_table_names_txt = "sql_table_names.txt";
    sql_init_write_to_file_txt(_sql_table_names_txt);
    vectorstr sql_table_names = read_from_file_txt(_sql_table_names_txt);
    cout<<"--Tables SQL is managing--\n";
    for(int i = 0; i < sql_table_names.size(); i++)
    {
        _tables[sql_table_names[i]] = Table(sql_table_names[i]);
        cout<<sql_table_names[i]<<"\n";
        // cout<<"SQL CTOR _tables:\n"<<_tables<<"\n";
    }
    cout<<"--------------------------\n\n";
    //need to build a table of tablenames on init take care of case where DB might not be managinig any tables
    // cout<<"SQL CTOR _tables:\n"<<_tables<<"\n";
}
    
Table SQL::command(string cmd_str)
{
    // Table t;
    const bool debug = false;
    try
    {
        _error_state = false;
        Error_Code error_code;
        char cmd_s[300];
        strcpy(cmd_s, cmd_str.c_str());
        Parser parser(cmd_s);
        _ptree.clear();
        _ptree = parser.parse_tree();
        if(debug)
            _ptree.print_lookup();
        if(_ptree["command"][0] == "make" || _ptree["command"][0] == "create")
        {
            //two argument CTOR to create new table
            if(_tables.contains(_ptree["table_name"][0]))
            {
                //error handling should go here
                //user trying to create an exisiting table
                error_code._code = CANNOT_CREATE_PRE_EXISTING_TABLE;
                throw error_code;
                return _tables[_ptree["table_name"][0]];

            }
            else
            {
                Table table(_ptree["table_name"][0], _ptree["col"]);
                _tables[_ptree["table_name"][0]] = table;
                write_to_file_txt_app(_sql_table_names_txt, {_ptree["table_name"][0]});
                if(debug)
                    cout<<"Brand New Table created.\n";
                return table;
            }
            //^ delete all txt and bin files than test
        }
        else if(_ptree["command"][0] == "insert")
        {
            //one argument CTOR to open an existing table to insert into
            // Table table(_ptree["table_name"][0]);
            if(!_tables.contains(_ptree["table_name"][0]))
            {
                error_code._code = INSERT_NON_EXISTENT;
                throw error_code;
            }
            if(debug)
            {
                cout<<_ptree["table_name"][0]<<" table opened\n";
                cout<<"Inserting :"<<_ptree["values"]<<"into "
                <<_ptree["table_name"][0]<<"\n";
            }
            _tables[_ptree["table_name"][0]].insert_into(_ptree["values"]);
            return _tables[_ptree["table_name"][0]];
        }
        else if(_ptree["command"][0] == "select")
        {
            // Table table(_ptree["table_name"][0]);
            //^ this command is now inside the _tables map

            //where means there is a condition
            if(!_ptree.contains("table_name"))
            {
                error_code._code = SELECT_EXPECT_TABLE_NAME;
                throw error_code;
            }
            if(!_tables.contains(_ptree["table_name"][0]))
            {
                error_code._code = SELECT_NON_EXISTENT;
                throw error_code;
            }
            if(_ptree["fields"][0] == "*" && _ptree.contains("where"))
            {
                //select * from student where lname = Yao
                //select fname, lname from student
                //v show all fields but select with condition
                if(!_ptree.contains("condition"))
                {
                    error_code._code = EXPECT_CONDITION;
                    throw error_code;
                }
                Table result_table = _tables[_ptree["table_name"][0]].select(_ptree["condition"]);
                _select_recnos = _tables[_ptree["table_name"][0]].select_recnos();
                return result_table;
            }
            else if(_ptree["fields"][0] == "*")
            {
                //show all fields and select all with no condition
                Table result_table = _tables[_ptree["table_name"][0]].select_all();
                _select_recnos = _tables[_ptree["table_name"][0]].select_recnos();
                return result_table;
            }
            else if(_ptree.contains("where"))
            {
                //want to select fields with condition
                if(!_ptree.contains("condition"))
                {
                    error_code._code = EXPECT_CONDITION;
                    throw error_code;
                }
                Table result_table = _tables[_ptree["table_name"][0]].select(_ptree["fields"], _ptree["condition"]);
                _select_recnos = _tables[_ptree["table_name"][0]].select_recnos();
                return result_table;
            }
            else
            {
                //want to show specific fields and select all with no condition
                Table result_table = _tables[_ptree["table_name"][0]].select_all(_ptree["fields"]);
                _select_recnos = _tables[_ptree["table_name"][0]].select_recnos();
                return result_table;
            }
        }
        else if(_ptree["command"][0] == "show tables")
        {
            // cout<<"SQL CTOR _tables:\n"<<_tables<<"\n";
            return get_tablenames_table();
        }
        else if(_ptree["command"][0] == "batch")
        {
            batch();
            _error_state = true;
            return Table();
        }
        else if(_ptree["command"][0] == "drop")
        {
            //throw if trying to drop a non exisiting table
            //throw if trying to drop without giving the name of a table
            //delete from the map
            //actually delete the txt file and bin associated with the table_name 
            if(!_ptree.contains("table_name"))
            {
                error_code._code = DROP_EXPECT_TABLENAME;
                throw error_code;
            }
            if(!_tables.contains(_ptree["table_name"][0]))
            {
                error_code._code = DROP_NON_EXISTENT;
                throw error_code;
            }
            string removed_table_name = _ptree["table_name"][0];
            // cout<<"Before removing "<<removed_table_name<<" from _tables map\n";
            // cout<<_tables;
            if(remove((removed_table_name + "_fields.txt").c_str()) != 0)
                cout<<"Could not remove the file: "<<_ptree["table_name"][0] + "_fields.txt\n";
            if(remove((removed_table_name + "_fields.bin").c_str()) != 0)
                cout<<"Could not remove the file: "<<_ptree["table_name"][0] + "_fields.bin\n";
            _tables.erase(removed_table_name);
            // cout<<"After removing "<<removed_table_name<<" from _tables map\n";
            // cout<<_tables;
            vectorstr before_remove_sql_table_names = read_from_file_txt(_sql_table_names_txt);
            vectorstr sql_table_names;
            for(int i = 0; i < before_remove_sql_table_names.size(); i++)
            {
                if(_tables.contains(before_remove_sql_table_names[i]))
                {
                    sql_table_names.push_back(before_remove_sql_table_names[i]);
                    _tables[before_remove_sql_table_names[i]] = Table(before_remove_sql_table_names[i]);
                }
            }
            write_to_file_txt(_sql_table_names_txt, sql_table_names);
            _error_state = true;
            return Table();
        }
        else
        {
            assert(false && "Invalid Command");
        }
    }
    catch(Error_Code error)
    {
        //v this function will add the cmd_str to the error_code obj so that it can use it to
        // display errors according to postgre sql standards
        modify_error_string_postgre(error, cmd_str);
        cout<<error.get_error_string()<<"\n";
        _error_state = true;
        return Table();
    }
    cout<<"Reached the end of command\n";
}

vectorlong SQL::select_recnos()
{
    return _select_recnos;
}
void SQL::print_tables_names()
{
    vectorstr sql_table_names = read_from_file_txt(_sql_table_names_txt);
    cout<<"--Tables SQL is managing--\n";
    for(int i = 0; i < sql_table_names.size(); i++)
    {
        cout<<sql_table_names[i]<<"\n";
    }
    cout<<"--------------------------\n\n";
}
//delete this print function later
void SQL::batch()
{
    // vectorstr command_vec = read_from_file_txt("batch.txt");
    const bool debug = false;
    ifstream fin;
    fin.open("batch.txt");
    if(debug)
        cout<<"Loading from batch file\n";
    string command_str;
    int i = 1;
    while(getline(fin, command_str))
    {
        cout<<i<<": "<<command_str<<"\n";
        Table temp = command(command_str);
        if(!error_state())
        {
          cout<<temp;
        }
        i++;
    }
    fin.close();
}

//privates
void SQL::sql_init_write_to_file_txt(string filename)
{
    if(!file_exists(filename.c_str()))
    {
        init_write_to_file_txt(filename, {});
    }
}
Table SQL::get_tablenames_table()
{
    Table sql_tables("sql_tables", {"table_names"});
    sql_tables.set_tablenames_table(true);
    vectorstr sql_table_names = read_from_file_txt(_sql_table_names_txt);
    for(int i = 0; i < sql_table_names.size(); i++)
    {
        sql_tables.insert_into({sql_table_names[i]});
    }
    return sql_tables;
}
void SQL::modify_error_string_postgre(Error_Code& error, string& cmd_str)
{
    const bool debug = false;
    if(error._modify_to_postgre)
    {
        error._error_input = cmd_str;
        char input_buffer[300];
        strcpy(input_buffer, cmd_str.c_str());
        STokenizer stk(input_buffer);
        SToken t;
        stk>>t;
        while(stk.more())
        {
            if(t.token_str() == error._error_token)
                break;

            error._character_count += t.token_str().size();
            t = SToken();
            stk>>t;
        }
    }
    if(debug)
        cout<<"error._character_count: "<<error._character_count<<"\n";
}


#endif // ZAC_SQL_