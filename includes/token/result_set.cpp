#ifndef ZAC_RESULT_SET_CPP
#define ZAC_RESULT_SET_CPP

#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <string>
#include <cassert>
#include "result_set.h"
using namespace std;

ResultSet::ResultSet()
{
    _val_list = {};
    set_token_type(RESULT_SET);
}
ResultSet::ResultSet(vectorlong val_list) : Token()
{
    _val_list = val_list;
    set_token_type(RESULT_SET);
}
ResultSet ResultSet::operator =(const vectorlong& val_list)
{
    set_token_type(RESULT_SET);
    _val_list = val_list;
}
vectorlong ResultSet::get_val_list()
{
    return _val_list;
}

#endif // ZAC_RESULT_SET_