#ifndef ZAC_OPERATOR_H
#define ZAC_OPERATOR_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <string>
#include <cassert>
#include "token.h"
using namespace std;

class Operator : public Token
{
public:
    Operator();
    Operator(string val);
    string get_val();
    void set_operator_type(const int& op_type);
    int get_operator_type();
    int get_precedence();
    void set_precedence(const int& precedence);
    void print_value();
private:
    string _val;
    int _op_type;
    int _precedence;
};

#endif // ZAC_OPERATOR_