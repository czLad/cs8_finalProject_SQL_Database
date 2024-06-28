#ifndef ZAC_LEFTPAREN_CPP
#define ZAC_LEFTPAREN_CPP

#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <string>
#include <cassert>
#include "leftparen.h"

using namespace std;

LParen::LParen()
{
    set_token_type(LPAREN);
    _stub = "(";
}
void LParen::print_value() 
{
    cout<<_stub;
}

#endif // ZAC_LEFTPAREN_