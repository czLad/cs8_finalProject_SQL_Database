#ifndef ZAC_RESULT_SET_H
#define ZAC_RESULT_SET_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <string>
#include <cassert>
#include "token.h"
#include "../table/typedefs.h"

using namespace std;

class ResultSet : public Token
{
public:
    ResultSet();
    ResultSet(vectorlong val_list);
    ResultSet operator =(const vectorlong& val_list);
    vectorlong get_val_list();
    
private:
    vectorlong _val_list;
};

#endif // ZAC_RESULT_SET_