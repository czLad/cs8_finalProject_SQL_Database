#ifndef ZAC_LOGICAL_H
#define ZAC_LOGICAL_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <string>
#include <cassert>
#include "token.h"
#include "result_set.h"
#include "operator.h"
#include "../sort_functions/sort_functions.h"
#include <algorithm>

using namespace std;

class Logical : public Operator
{
public:
    Logical();
    Logical(const string &val);
    string get_val();
    vectorlong evaluate(Token *field_token, Token *condition_token, vector<mmap_sl> &record_indicies, map_sl &field_indicies) throw(Error_Code);
    vectorlong intersect(vectorlong vector_1, vectorlong vector_2);
    // Better performing union_vecs
    vectorlong union_vecs(vectorlong &vector_1, vectorlong &vector_2);
    void print_value();
private:
    string _val;
};

#endif // ZAC_LOGICAL_