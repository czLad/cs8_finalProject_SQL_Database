#ifndef ZAC_PARSER_CPP
#define ZAC_PARSER_CPP

#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <string>
#include <cassert>
#include "parser.h"
#include "parser_state_machine_functions.h"
#include "../array_functions/array_functions.h"

using namespace std;

Parser::Parser()
{
    //should not be using defualt CTOR
    cout<<"Should not be using default CTOR\n";
}
Parser::Parser(char cmd_str[300])
{
    init_table(_table);
    make_table();
    build_keyword_map();
    set_string(cmd_str);
    //buil
}
//To Update Parser Commands must make changes to this function v 
mmap_ss Parser::parse_tree() throw(Error_Code)
{
    const bool debug = false;
    Error_Code error_code;
    STokenizer stk(_input_buffer);
    SToken t;
    Queue<string> input_q;
    string quotation_str = "";
    int quotation_count = 0;
    bool waiting_for_alpha = false;
    stk>>t;
    while(stk.more())
    {
        //process token here...
        bool punc_tok_contains_quotation = false;
        if(t.type_string() == "PUNC")
        {
            //v if double quote in one punc token, this wont be true
            if(t.token_str() == "\"")
                quotation_count++;
            else if(t.token_str() == ",")
               input_q.push(t.token_str());
            else
            {
                for(int i = 0; i < t.token_str().size(); i++)
                {
                    if(t.token_str()[i] == '\"')
                    {
                        quotation_count++;
                        punc_tok_contains_quotation = true;
                    }
                }
            }
            
        }
        else if((t.type_string() == "ALPHA" || t.type_string() == "NUMBER" 
        || t.type_string() == "OPERATOR" || t.type_string() == "PAREN" || t.token_str() == ",") && quotation_count == 0)
        {
        //   cout<<setw(10)<<t.type_string()<<setw(10)<<t<<endl;
          input_q.push(t.token_str());
        }

        if(quotation_count == 1 && t.token_str() != "\"" && !punc_tok_contains_quotation)
            quotation_str += t.token_str();
        if(quotation_count == 2)
        {
            quotation_count = 0;
            if(punc_tok_contains_quotation)
            {
                int i = 0;
                while(t.token_str()[i] != '\"')
                {
                    quotation_str += t.token_str()[i];
                    i++;
                }
            }
            if(!quotation_str.empty())
                input_q.push(quotation_str);
            quotation_str.clear();
        }
        t = SToken();
        stk>>t;
    }
    if(debug)
    {
        cout<<"------Strings in input q------------\n";
        for(Queue<string>::Iterator it = input_q.begin(); it != input_q.end(); it++)
        {
          cout<<*it<<"\n";
        }
    }
    //^ get the input token queues
    if(input_q.empty())
    {
      error_code._code = 1;
      throw error_code;
    }
    string cause_of_failure = "";
    int current_state =0;
    long new_state= 0;
    fail = false;
    int last_state_before_fail = 0;
    // int select_field_name_count = 0;
    // int select_comma_count = 0;
    Queue<string>::Iterator it;
    for(it = input_q.begin(); it != input_q.end() && current_state != -1; it++)
    {
        if(debug)
          cout<<"current state: "<<current_state<<"\n";
        if(_keywords_map.contains(*it))
        {
          new_state = _keywords_map[*it];
          current_state = _table[current_state][new_state];
        }
        else
        {
          if(!current_state)
          {
            error_code._code = 2;
            throw error_code;
          }
          new_state = SYM;
          current_state = _table[current_state][new_state];
        }
        switch (current_state)
        {
        case SELECT:
          _ptree["command"] += *it;
          break;
        case STAR:
          _ptree["fields"] += *it;
          break;
        case TABLENAME:
          _ptree["table_name"] += *it;
          break;
        case MAKE_OR_CREATE:
          _ptree["command"] += *it;
          break;
        case NEWTABLENAME:
          _ptree["table_name"] += *it;
          break;
        case FIELDNAME:
          _ptree["col"] += *it;
          break;
        case INTO:
          _ptree["command"] += string("insert");
          break;
        case OPENTABLENAME:
          _ptree["table_name"] += *it;
          break;
        case VALUENAME:
          _ptree["values"] += *it;
          break;
        case SELECTFIELDNAME:
          _ptree["fields"] += *it;
          break;
        case WHERE:
          _ptree["where"] += string("yes");
          break;
        case CONDITIONNAME:
          _ptree["condition"] += *it;
          break;
        case TABLES:
          _ptree["command"] += string("show ") + *it;
          break;
        case BATCH:
          _ptree["command"] += *it;
          break;
        default:
          break;
        }
        if(current_state == -1)
          cause_of_failure = *it;
        else
          last_state_before_fail = current_state;
    }
    if(current_state == -1)
    {
        fail = true;
        _ptree.clear();
        if(debug)
            cout<<"actual failed state\n";

        if(_keywords_map.contains(cause_of_failure))
        {
          if(cause_of_failure == ",")
          {
            error_code._code = 10;
            throw error_code;
          }
          if(cause_of_failure == "where")
          {
            error_code._code = 5;
            throw error_code;
          }
          error_code._code = 3;
          throw error_code;
        }
        else
        {
          //throw with an expect string
          
          if(last_state_before_fail == SELECTFIELDNAME || last_state_before_fail == STAR)
          {
            while(it != input_q.end())
            {
              if(*it == "from")
              {
                error_code._code = 4;
                throw error_code;
              }
              it++;
            }
            error_code._code = 12;
            throw error_code;
          }
          error_code._code = 13;
          throw error_code;
        }
    }
    if(debug)
    {
        cout<<_ptree<<"\n";
        cout<<"reached the end of parse_tree:\n";
    }
    return _ptree;
}
void Parser::set_string(char cmd_str[300])
{
    const bool debug = false;
    strcpy(_input_buffer, cmd_str);
    _ptree.clear();
    if(debug)
        cout<<"after _ptree is cleared: "<<_ptree<<"\n";
}
//private
//To Update Parser Commands must make changes to this function v 
void Parser::make_table()
{
    const bool debug = false;
    if(debug)
    {
        cout << "---Before Making Table------\n";
        print_table(_table);
    }
    //v  will update repetitive states later //
    //init all rows in col zero

    //for select
    mark_fail(_table, 0);
    mark_fail(_table, SELECT); //ENUMS represents what
    mark_fail(_table, STAR); //commands tokens are
    mark_fail(_table, FROM);
    mark_success(_table, TABLENAME);
    mark_fail(_table, SELECTFIELDNAME);
    //v last minute addition comma
    mark_fail(_table, COMMA); //this COMMA row is never used
    mark_fail(_table, SELECTFIELDNAMECOMMA);
    //^ last minute addition comma
    mark_fail(_table, WHERE);
    mark_success(_table, CONDITIONNAME);

    //for make
    mark_fail(_table, MAKE_OR_CREATE);
    mark_fail(_table, TABLE);
    mark_fail(_table, NEWTABLENAME);
    mark_fail(_table, FIELDS);
    mark_success(_table, FIELDNAME);
    //v last minute addition comma
    mark_fail(_table, FIELDNAMECOMMA);
    //^ last minute addition comma

    //for insert
    mark_fail(_table, INSERT);
    mark_fail(_table, INTO);
    mark_fail(_table, OPENTABLENAME);
    mark_fail(_table, VALUES);
    mark_success(_table, VALUENAME);
    //v last minute addition comma
    mark_fail(_table, VALUENAMECOMMA);
    //^ last minute addition comma

    //for show
    mark_fail(_table, SHOW);
    mark_success(_table, TABLES);

    //for batch
    mark_success(_table, BATCH);
    
    //v Marking initial states 
    //mark the expected token previous row's as 
    //the next state(if expected tok is received) you want to go 

    //for select
    mark_cell(0, _table, SELECT, SELECT);
    mark_cell(SELECT, _table, STAR, STAR);
    mark_cell(STAR, _table, FROM, FROM);
    //v marking symbol column in from row to go to stage 4 
    mark_cell(FROM, _table, SYM, TABLENAME);
    //for advanced select
    mark_cell(SELECT, _table, SYM, SELECTFIELDNAME);
    //v last minute comma addition
    mark_cell(SELECTFIELDNAME, _table, COMMA, SELECTFIELDNAMECOMMA);
    mark_cell(SELECTFIELDNAMECOMMA, _table, SYM, SELECTFIELDNAME);
    //^ last minute comma addition
    mark_cell(SELECTFIELDNAME, _table, FROM, FROM);
    mark_cell(FROM, _table, SYM, TABLENAME);
    mark_cell(TABLENAME, _table, WHERE, WHERE);
    mark_cell(WHERE, _table, SYM, CONDITIONNAME);
    mark_cell(CONDITIONNAME, _table, SYM, CONDITIONNAME);


    //for make or create //if get make or create, then make or create
    mark_cell(0, _table, MAKE_OR_CREATE, MAKE_OR_CREATE);
    mark_cell(MAKE_OR_CREATE, _table, TABLE, TABLE);
    mark_cell(TABLE, _table, SYM, NEWTABLENAME);
    mark_cell(NEWTABLENAME, _table, FIELDS, FIELDS);
    mark_cell(FIELDS, _table, SYM, FIELDNAME);
    mark_cell(FIELDNAME, _table, SYM, FIELDNAME);
    //V last minute comma addition
    mark_cell(FIELDNAME, _table, COMMA, FIELDNAMECOMMA);
    mark_cell(FIELDNAMECOMMA, _table, SYM, FIELDNAME);
    //^ last minute comma addition

    //for insert
    mark_cell(0, _table, INSERT, INSERT);
    mark_cell(INSERT, _table, INTO, INTO);
    mark_cell(INTO, _table, SYM, OPENTABLENAME);
    mark_cell(OPENTABLENAME, _table, VALUES, VALUES);
    mark_cell(VALUES, _table, SYM, VALUENAME);
    mark_cell(VALUENAME, _table, SYM, VALUENAME);
    //V last minute comma addition
    mark_cell(VALUENAME, _table, COMMA, VALUENAMECOMMA);
    mark_cell(VALUENAMECOMMA, _table, SYM, VALUENAME);
    //^ last minute comma addition

    //for show
    mark_cell(0, _table, SHOW, SHOW);
    mark_cell(SHOW, _table, TABLES, TABLES);
    
    //for batch
    mark_cell(0, _table, BATCH, BATCH);

    if(debug)
    {
        cout << "---After Making Table------\n";
        print_table(_table);
    }

}
//To Update Parser Commands must make changes to this function v 
void Parser::build_keyword_map()
{
    const bool debug = false;

    _keywords_map["select"] = SELECT;
    _keywords_map["*"] = STAR;
    _keywords_map["from"] = FROM;
    _keywords_map["where"] = WHERE;
    _keywords_map["make"] = MAKE_OR_CREATE;
    _keywords_map["create"] = MAKE_OR_CREATE;
    _keywords_map["table"] = TABLE;
    _keywords_map["fields"] = FIELDS;
    _keywords_map["insert"] = INSERT;
    _keywords_map["into"] = INTO;
    _keywords_map["values"] = VALUES;
    _keywords_map["show"] = SHOW;
    _keywords_map["tables"] = TABLES;
    //last minute addition comma
    _keywords_map[","] = COMMA;
    _keywords_map["batch"] = BATCH;

    if(debug)
        cout<<"_keywords_map:\n"<<_keywords_map;
}

int Parser::_table[MAX_ROWS_PARSER][MAX_COLUMNS_PARSER];

#endif // ZAC_PARSER_