#ifndef ZAC_OPERATOR_CPP
#define ZAC_OPERATOR_CPP

#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <string>
#include <cassert>
#include "operator.h"

using namespace std;

Operator::Operator()
{
    _val = "";
    set_token_type(OPERATOR);
    set_operator_type(TOKEN_END+1);
}
Operator::Operator(string val) : Token()
{
    _val = val;
    set_token_type(OPERATOR);
    set_operator_type(TOKEN_END+1);
}
// TokenStr(const string& val) : Token()
// {
//     _val = val;
//     set_token_type(TOKEN_STR);
// }
string Operator::get_val()
{
    return _val;
}
void Operator::set_operator_type(const int& op_type)
{
    _op_type = op_type;
}
int Operator::get_operator_type()
{
    return _op_type;
}
int Operator::get_precedence()
{
    return _precedence;
}
void Operator::set_precedence(const int& precedence)
{
    _precedence = precedence;
}
void Operator::print_value()
{
    cout<<_val;
}

#endif // ZAC_OPERATOR_