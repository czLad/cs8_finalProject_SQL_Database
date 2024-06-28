#ifndef TOKEN_H
#define TOKEN_H

#include "iostream"
#include "iomanip"
#include "string.h"
#include  "constants.h"
#include "../bplustree/map.h"
#include "../bplustree/multimap.h"
#include "vector"

using namespace std;

class Token
{
public:
    //CTOR
    Token();
    Token(string str, int type);
    friend ostream& operator <<(ostream& outs, const Token& t);
    int get_type() const;
    void set_token_type(int token_type);
    string type_string() const;
    string token_str() const;
    //virtual functions
    virtual vector<long> evaluate(Token* field_token, Token* condition_token, vector<MMap<string, long>> &record_indicies, 
    Map<string, long> &field_indicies);
    virtual void print_value();
private:
    string _token;
    int _type;
};

#endif