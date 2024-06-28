#ifndef ZAC_PARSER_CONSTANTS_H
#define ZAC_PARSER_CONSTANTS_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <string>
#include <cassert>
using namespace std;

const int MAX_ROWS_PARSER = 26;
const int MAX_COLUMNS_PARSER = 26;
//MAX ALWAYS HAVE TWO MORE THAN BIGGEST KEY STATE
enum key_states
{
    ZERO,
    SELECT, //SELECT
    STAR,
    FROM,
    TABLENAME,
    SELECTFIELDNAME, //ADVANCED SELECT //NEEDS COMMA
    SELECTFIELDNAMECOMMA,
    WHERE,
    CONDITIONNAME, 
    MAKE_OR_CREATE, //MAKE || CREATE
    TABLE,
    NEWTABLENAME,
    FIELDS,
    FIELDNAME,
    FIELDNAMECOMMA,
    INSERT, //INSERT
    INTO,
    OPENTABLENAME,
    VALUES,
    VALUENAME, //NEEDS COMMA
    VALUENAMECOMMA,
    SHOW, //SHOW TABLES
    TABLES,
    COMMA, //THIS COMMA WILL ALSO BE USED BY VALUENAME
    BATCH
};

const int SYM = MAX_COLUMNS_PARSER - 1;
#endif // ZAC_CONSTANTS_