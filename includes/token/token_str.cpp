#ifndef ZAC_TOKEN_STR_CPP
#define ZAC_TOKEN_STR_CPP

#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <string>
#include <cassert>
#include "token_str.h"
using namespace std;

TokenStr::TokenStr()
{
    _val = "";
    set_token_type(TOKEN_STR);
}
TokenStr::TokenStr(string val) : Token()
{
    _val = val;
    set_token_type(TOKEN_STR);
}
// TokenStr(const string& val) : Token()
// {
//     _val = val;
//     set_token_type(TOKEN_STR);
// }
string TokenStr::get_val()
{
    return _val;
}
void TokenStr::print_value()
{
    cout<<_val;
}

#endif // ZAC_TOKEN_STR_