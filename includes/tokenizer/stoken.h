#ifndef STOKEN_H
#define STOKEN_H

#include "iostream"
#include "iomanip"
#include "string.h"
#include "stoken_constants.h"

using namespace std;

class SToken
{
public:
    //CTOR
    SToken()
    {
        const bool debug = false;
        if(debug)
        {
            cout << "Token CTOR Fired.\n";
        }
        _token = "";
        _type = 0;
    }
    SToken(string str, int type)
    {
         const bool debug = false;
        if(debug)
        {
            cout << "Token(string str, int type) CTOR Fired.\n";
        }
        _token = str;
        _type = type;
    }
    friend ostream& operator <<(ostream& outs, const SToken& t)
    {
        outs<< "|"<<t._token<<"|";
        return outs;
    }
    int type() const
    {
        return _type;
    }
    string type_string() const
    {
        switch (_type)
        {
        case STOKEN_NUMBER:
            return "NUMBER";
        case STOKEN_ALPHA:
            return "ALPHA";
        case STOKEN_SPACE:
            return "SPACE";
        case STOKEN_OPERATOR:
            return "OPERATOR";
        case STOKEN_PUNC:
            return "PUNC";
        case STOKEN_PAREN:
            return "PAREN";
        case STOKEN_UNKNOWN:
            return "UNKNOWN";
        case STOKEN_END:
            return "END";
        // NOTE TO MY CHILDREN, GRANDCHILDREN, GREAT GRANDCHILDREN
        // ALWAYSS ALWAYSS WRITE A DEFAULT ALWAYSSSS
        default:
            return "ERROR";
        }
    }
    string token_str() const
    {
        return _token;
    }
private:
    string _token;
    int _type;
};

#endif