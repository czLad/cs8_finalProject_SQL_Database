#ifndef ERROR_CODE_H
#define ERROR_CODE_H

#include <iostream>
#include <iomanip>

using namespace std;

enum sql_errors
{
    EMPTY_COMMAND = 1,
    INVALID_COMMAND,
    EXTRA_KEYWORDS,
    EXPECTED_COMMA,
    SELECT_EXPECT_TABLE_NAME,
    CANNOT_CREATE_PRE_EXISTING_TABLE,
    MISSING_RIGHT_PAREN,
    MISSING_LEFT_PAREN,
    INVALID_CONDITION,
    EXPECT_FIELDNAME,
    EXPECT_CONDITION,
    EXPECT_FROM,
    MISSING_KEYWORDS,
    SELECT_NON_EXISTENT,
    INSERT_NON_EXISTENT,
    DROP_NON_EXISTENT,
    DROP_EXPECT_TABLENAME,
    SYNTAX_ERR_AT_NEAR,
    RELATIONAL_MISSING_RIGHT_ARG,
    RELATIONAL_MISSING_LEFT_ARG,
    MISSING_ARGUMENTS,
    LOGICAL_MISSING_AN_ARGUMENT,
    EXPECT_RELATIONAL,
    INVALID_USAGE_OF_OP,
    EXPECT_LOGICAL,
    EXPECT_A_RELATIONAL,
    UNKNOWN_COLUMN
};

struct Error_Code
{
public:
    int _code;
    int _character_count;
    bool _modify_to_postgre;
    string _error_token;
    string _error_input;

    Error_Code(): _code(0), _modify_to_postgre(false), _character_count(0), _error_token(""), _error_input("") {}
    string get_error_string()
    {
        string error_string;
        switch (_code)
        {
        //no need to append \n at the end of the strings. The caller should append \n after calling get()
        case EMPTY_COMMAND:
            error_string = "\033[31mEmpty Command. Please try again.\033[0m";
            break;
        case INVALID_COMMAND:
            error_string = "\033[31mInvalid Command. Please try again.\033[0m";
            break;
        case EXTRA_KEYWORDS:
            error_string = "\033[31mExtra Keywords. Please try again.\033[0m";
            break;
        case EXPECTED_COMMA:
            error_string = "\033[31mExpected comma. Please try again.\033[0m";
            break;
        case SELECT_EXPECT_TABLE_NAME:
            error_string = "\033[31mExpected table name. Please put the name of the table after the keyword \"from\". Please try again.\033[0m";
            break;
        case CANNOT_CREATE_PRE_EXISTING_TABLE:
            error_string = "\033[31mCannot create a pre-existing table. Please pick a different table name.\nTo see existing table names type in the command \"show tables\". Please try again.\033[0m";
            break;
        case MISSING_RIGHT_PAREN:
            error_string = "\033[31mMissing Right Parenthesis\033[0m";
            break;
        case MISSING_LEFT_PAREN:
            error_string = "\033[31mMissing Left Parenthesis\033[0m";
            break;
        case INVALID_CONDITION:
            error_string = "\033[31mInvalid Condition. The condition that follows \"where\" is invalid.\033[0m";
            break;
        case EXPECT_FIELDNAME:
            error_string = "\033[31mExpected field name. Please try again.\033[0m";
            break;
        case EXPECT_CONDITION:
            error_string = "\033[31mExpected condition. Please try again.\033[0m";
            break;
        case EXPECT_FROM:
            error_string = "\033[31mExpected from. Please try again.\033[0m";
            break;
        case MISSING_KEYWORDS:
            error_string = "\033[31mMissing Keywords\033[0m";
            break;
        case SELECT_NON_EXISTENT:
            error_string = "\033[31mError. Selecting from a non-exsitent table\033[0m";
            break;
        case INSERT_NON_EXISTENT:
            error_string = "\033[31mError. Inserting into a non-exsitent table\033[0m";
            break;
        case DROP_NON_EXISTENT:
            error_string = "\033[31mError.Cannot drop a non-exsitent table\033[0m";
            break;
        case DROP_EXPECT_TABLENAME:
            error_string = "\033[31mExpected table name. Please put the name of the table after the keyword \"table\". Please try again.\033[0m";
            break;
        case SYNTAX_ERR_AT_NEAR:
            error_string = "\033[31mERROR: syntax error at or near\033[0m ";
            error_string += "\033[34m" + _error_token + "\033[0m\n";
            error_string += "\033[31m" + _error_input + "\033[0m\n";
            for(int i = 0; i < _character_count; i++)
            {
                error_string += " ";
            }
            error_string += "\033[31m^\033[0m";
            break;
        case RELATIONAL_MISSING_RIGHT_ARG:
            error_string = "\033[31mERROR: Operator \033[34m\"" + _error_token + "\"\033[0m ";
            error_string += "\033[31mmissing right argument\033[0m";
            break;
        case RELATIONAL_MISSING_LEFT_ARG:
            error_string = "\033[31mERROR: Operator \033[34m\"" + _error_token + "\"\033[0m ";
            error_string += "\033[31mmissing left argument (column name)\033[0m";
            break;
        case MISSING_ARGUMENTS:
            error_string = "\033[31mERROR: Operator \033[34m\"" + _error_token + "\"\033[0m ";
            error_string += "\033[31mrequires a LHS and RHS argument\033[0m";
            break;
        case LOGICAL_MISSING_AN_ARGUMENT:
            error_string = "\033[31mERROR: Operator \033[34m\"" + _error_token + "\"\033[0m ";
            error_string += "\033[31mmissing an argument\033[0m";
            break;
        case EXPECT_RELATIONAL:
            error_string = "\033[31mERROR: Expected a relational operator in the place of the logical operator \033[34m" + _error_token + "\033[0m";
            break;
        case INVALID_USAGE_OF_OP:
            error_string = "\033[31mERROR: Invalid usage of the operator \033[34m\"" + _error_token + "\"\033[0m";
            break;
        case EXPECT_LOGICAL:
            error_string = "\033[31mERROR: Expected a logical operator between two conditions\033[0m";
            break;
        case UNKNOWN_COLUMN:
            error_string = "\033[31mERROR: column \033[34m\"" + _error_token + "\"\033[31m does not exist \033[0m\n";
            error_string += "\033[31m" + _error_input + "\033[0m\n";
            for(int i = 0; i < _character_count; i++)
            {
                error_string += " ";
            }
            error_string += "\033[31m^\033[0m";
            break;
        case EXPECT_A_RELATIONAL:
            error_string = "\033[31mERROR: Expected a relational operator between a column name and a value\033[0m";
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