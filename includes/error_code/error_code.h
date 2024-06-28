#ifndef ERROR_CODE_H
#define ERROR_CODE_H

#include <iostream>
#include <iomanip>

using namespace std;

struct Error_Code
{
public:
    int _code;
    Error_Code(): _code(0) {}
    string get_error_string()
    {
        string error_string;
        switch (_code)
        {
        case 1:
            error_string = "\033[31mEmpty Command. Please try again.\033[0m";
            break;
        case 2:
            error_string = "\033[31mInvalid Command. Please try again.\033[0m";
            break;
        case 3:
            error_string = "\033[31mExtra Keywords. Please try again.\033[0m";
            break;
        case 4:
            error_string = "\033[31mExpected comma. Please try again.\033[0m";
            break;
        case 5:
            error_string = "\033[31mExpected table name. Please put the name of the table after the keyword \"from\". Please try again.\033[0m";
            break;
        case 6:
            error_string = "\033[31mCannot create a pre-existing table. Please pick a different table name.\nTo see existing table names type in the command \"show tables\". Please try again.\033[0m";
            break;
        case 7:
            error_string = "\033[31mMissing Right Parenthesis\033[0m";
            break;
        case 8:
            error_string = "\033[31mMissing Left Parenthesis\033[0m";
            break;
        case 9:
            error_string = "\033[31mYou are selecting with an Invalid Condition. Missing Argument.\033[0m";
            break;
        case 10:
            error_string = "\033[31mExpected field name. Please try again.\033[0m";
            break;
        case 11:
            error_string = "\033[31mExpected condition. Please try again.\033[0m";
            break;
        case 12:
            error_string = "\033[31mExpected from. Please try again.\033[0m";
            break;
        case 13:
            error_string = "\033[31mMissing Keywords\033[0m";
            break;
        case 14:
            error_string = "\033[31mError. Selecting from a non-exsitent table\033[0m";
            break;
        case 15:
            error_string = "\033[31mError. Inserting into a non-exsitent table\033[0m";
            break;
        default:
            error_string = "Wrong Error Code";
            break;
        }
        return error_string;
    }
    friend ostream& operator << (ostream& outs, const Error_Code& other)
    {
        // outs<<"hi error_code\n";
        outs<<"error_code: [ "<<other._code<<" ]";
        return outs;
    }  
};


#endif