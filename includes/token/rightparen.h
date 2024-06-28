#ifndef RIGHTPAREN_H
#define RIGHTPAREN_H

#include "token.h"

using namespace std;

class RParen : public Token
{
public:
    RParen();
    void print_value();
private:
    string _stub;
};


#endif