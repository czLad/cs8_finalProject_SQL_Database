#ifndef ZAC_TOKEN_STR_H
#define ZAC_TOKEN_STR_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <string>
#include <cassert>
#include "token.h"

using namespace std;

class TokenStr : public Token
{
public:
    TokenStr();
    TokenStr(string val);
    string get_val();
    void print_value();
    
private:
    string _val;
};


#endif // ZAC_TOKEN_STR_