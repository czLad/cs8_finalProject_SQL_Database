#ifndef ZAC_RELATIONAL_CPP
#define ZAC_RELATIONAL_CPP

#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <string>
#include <cassert>
#include "relational.h"

using namespace std;

Relational::Relational()
{
    _val = "";
    set_token_type(OPERATOR);
    set_operator_type(RELATIONAL);
    set_precedence(3);
}
Relational::Relational(const string& val) : Operator()
{
    _val = val;
    set_token_type(OPERATOR);
    set_operator_type(RELATIONAL);
    set_precedence(3);
}
string Relational::get_val()
{
    const bool debug = false;
    if(debug)
        cout<<"Entered virtual get_val in relational\n";
    return _val;
}
vectorlong Relational::evaluate(Token* field_token, Token* condition_token, vector<mmap_sl> &record_indicies, map_sl &field_indicies) throw(Error_Code)
{
    Error_Code error_code;
    string field = static_cast<TokenStr*>(field_token)->get_val();
    string condition = static_cast<TokenStr*>(condition_token)->get_val();
    if(!field_indicies.contains(field))
    {
        error_code._error_token = field;
        error_code._code = UNKNOWN_COLUMN;
        error_code._modify_to_postgre = true;
        throw error_code;
    }
    const bool debug = false;
    if(debug)
        cout<<"Virtual evaluate() of relational class fired.\n";
    //the string vec has field names
    //_record_indicies[field_indicies[field]] <- this gives the right multimap
    // _record_indicies[_field_indicies[field]][condition] <- this is getting the condition from the multimap
    vectorlong build_vector;
    string relational = _val;
    if(relational == "=")
    {
        build_vector = record_indicies[field_indicies[field]][condition];
    }
    else if(relational == "<")
    {
        for(mmap_sl::Iterator it = record_indicies[field_indicies[field]].begin(); 
        it !=  record_indicies[field_indicies[field]].lower_bound(condition); ++it)
        {
            for(int i = 0; i < (*it).value_list.size(); i++)
                build_vector.push_back((*it).value_list[i]);
        }
    }
    else if(relational == ">")
    {
        for(mmap_sl::Iterator it = record_indicies[field_indicies[field]].upper_bound(condition); 
        it !=  record_indicies[field_indicies[field]].end(); ++it)
        {
            for(int i = 0; i < (*it).value_list.size(); i++)
                build_vector.push_back((*it).value_list[i]);
        }
    }
    else if(relational == ">=")
    {
        for(mmap_sl::Iterator it = record_indicies[field_indicies[field]].lower_bound(condition); 
        it !=  record_indicies[field_indicies[field]].end(); ++it)
        {
            for(int i = 0; i < (*it).value_list.size(); i++)
                build_vector.push_back((*it).value_list[i]);
        }
    }
    else
    {
        //less than equal
        for(mmap_sl::Iterator it = record_indicies[field_indicies[field]].begin(); 
        it !=  record_indicies[field_indicies[field]].upper_bound(condition); ++it)
        {
            for(int i = 0; i < (*it).value_list.size(); i++)
                build_vector.push_back((*it).value_list[i]);
        }
    }
    //if not less than equal then, throw that there's a syntax error at or near the operator relational token is holding
    if(debug)
        cout<<"build_vector: "<<build_vector<<"\n";
    return build_vector;
}
void Relational::print_value()
{
    cout<<_val;
}

#endif // ZAC_RELATIONAL_