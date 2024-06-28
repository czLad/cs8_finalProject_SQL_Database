#ifndef ZAC_TOKEN_CPP
#define ZAC_TOKEN_CPP

#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <string>
#include <cassert>
#include "token.h"
using namespace std;

Token::Token()
{
    const bool debug = false;
    if(debug)
    {
        cout << "Token CTOR Fired.\n";
    }
    _token = "";
    _type = 0;
}
Token::Token(string str, int type)
{
     const bool debug = false;
    if(debug)
    {
        cout << "Token(string str, int type) CTOR Fired.\n";
    }
    _token = str;
    _type = type;
}
ostream& operator <<(ostream& outs, const Token& t)
{
    outs<< "|"<<t._token<<"|";
    return outs;
}
int Token::get_type() const
{
    return _type;
}
void Token::set_token_type(int token_type)
{
    _type = token_type; 
}
string Token::type_string() const
{
    switch (_type)
    {
    case TOKEN_NUMBER:
        return "NUMBER";
    case TOKEN_ALPHA:
        return "ALPHA";
    case TOKEN_SPACE:
        return "SPACE";
    case TOKEN_OPERATOR:
        return "OPERATOR";
    case TOKEN_PUNC:
        return "PUNC";
    case TOKEN_UNKNOWN:
        return "UNKNOWN";
    case TOKEN_END:
        return "END";
    // NOTE TO MY CHILDREN, GRANDCHILDREN, GREAT GRANDCHILDREN
    // ALWAYSS ALWAYSS WRITE A DEFAULT ALWAYSSSS
    default:
        return "ERROR";
    }
}
string Token::token_str() const
{
    return _token;
}
//virtual functions
vector<long> Token::evaluate(Token* field_token, Token* condition_token, vector<MMap<string, long>> &record_indicies, 
Map<string, long> &field_indicies)
{
    return vector<long>();
}
void Token::print_value()
{
    cout<<_token;
}

#endif // ZAC_TOKEN_