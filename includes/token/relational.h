#ifndef ZAC_RELATIONAL_H
#define ZAC_RELATIONAL_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <string>
#include <cassert>
#include "token.h"
#include "../table/typedefs.h"
#include "token_str.h"
#include "operator.h"

using namespace std;

class Relational : public Operator
{
public:
    Relational();
    Relational(const string& val);
    string get_val();
    vectorlong evaluate(Token* field_token, Token* condition_token, vector<mmap_sl> &record_indicies, map_sl &field_indicies);
    void print_value();
private:
    string _val;
};


#endif // ZAC_RELATIONAL_