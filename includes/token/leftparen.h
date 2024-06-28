#ifndef LEFTPAREN_H
#define LEFTPAREN_H

#include "token.h"

using namespace std;

class LParen : public Token
{
public:
    LParen();
    void print_value();
private:
    string _stub;
};


#endif