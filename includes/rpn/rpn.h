#ifndef RPN_H
#define RPN_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include "../queue/MyQueue.h"
#include "../stack/MyStack.h"
#include "../token/token.h"
#include "../table/typedefs.h"
#include "../token/token_str.h"
#include "../token/relational.h"
#include "../token/result_set.h"
#include "../error_code/error_code.h"

using namespace std;

class RPN
{
public:
    RPN(){;}
    RPN(const Queue<Token*> &postfix) : _postfix(postfix){;}
    vectorlong operator()(vector<mmap_sl>& record_indicies, map_sl& field_indicies) throw(Error_Code)
    {
        assert(_postfix.begin() && "Cannot evaluate an empty queue");
        Error_Code error_code;
        const bool debug = false;
        if(debug)
        {
            cout<<"----------Entered RPN------------\n";
            for(Queue<Token*>::Iterator it = _postfix.begin(); it != _postfix.end(); it++)
            {
                // cout<<"*it "<<*it<<"\n";
                cout<<"*it ";
                (*it)->print_value();
                cout<<"\n";
            }
        }
        Stack<Token*> rpn_stack;
        int token_str_count = 0;
        for(Queue<Token*>::Iterator it = _postfix.begin(); it != _postfix.end(); ++it)
        {
            Token* first_pop;
            Token* second_pop;
            ResultSet result;
            if(debug)
                cout <<"switch: "<<(*it)->get_type()<< "\n";
            if(debug)
                cout << (*it)->get_val() << " ";
            if(token_str_count == 2 && (*it)->get_type() != OPERATOR)
            {
                token_str_count = 0;
                error_code._error_token = (*rpn_stack.begin())->get_val();
                if(!field_indicies.contains((*it)->get_val()))
                {
                    error_code._code = SYNTAX_ERR_AT_NEAR;
                    error_code._modify_to_postgre = true;
                }
                else
                    error_code._code = EXPECT_A_RELATIONAL;
                throw error_code;
            }
            else if(token_str_count == 2 && (*it)->get_type() == OPERATOR)
                token_str_count = 0;
            switch((*it)->get_type())
            {
                case TOKEN_STR:
                if(debug)
                    cout << "case TOKEN_STR RPN\n";
                token_str_count++;
                rpn_stack.push(*it);
                break;
                case OPERATOR:
                if(debug)
                    cout << "case OPERATOR (RELATIONAL / LOGICAL) RPN\n";
                if(rpn_stack.size() < 2)
                {
                    //missing argument
                    //its not empty, pop it and look at it to see if the token in the token_str popped is a field name or not
                    //if not, say and (*it) is holding a relational operator, say missing RHS argument
                    //else, if the field_indicies map does not contain the token_str, say LHS argument missing
                    //else if (*it) is holding a logical operator, say logical op is missing an argument
                    if(rpn_stack.empty())
                    {
                        error_code._error_token = (*it)->get_val();
                        error_code._code = MISSING_ARGUMENTS;
                    }
                    else
                    {
                        if(static_cast<Operator*>(*it)->get_operator_type() == RELATIONAL)
                        {
                            error_code._error_token = (*it)->get_val();
                            first_pop = rpn_stack.pop();
                            if(field_indicies.contains(first_pop->get_val()))
                            {
                                error_code._code = RELATIONAL_MISSING_RIGHT_ARG;
                            }
                            else
                            {
                                error_code._code = RELATIONAL_MISSING_LEFT_ARG;
                            }
                        }
                        else
                        {
                            error_code._error_token = (*it)->get_val();
                            error_code._code = LOGICAL_MISSING_AN_ARGUMENT;
                        }
                    }
                    throw error_code;
                }
                first_pop = rpn_stack.pop();
                second_pop = rpn_stack.pop();
                if(first_pop->get_type() != second_pop->get_type())
                {
                    //invalid usage of an operator
                    string token_str = "";
                    if(first_pop->get_type() == TOKEN_STR)
                        token_str = first_pop->get_val();
                    else
                        token_str = second_pop->get_val();
                    error_code._error_token = (*it)->get_val();
                    if(static_cast<Operator*>(*it)->get_operator_type() == RELATIONAL)
                    {
                        error_code._error_token = (*it)->get_val();
                        if(field_indicies.contains(token_str))
                        {
                            error_code._code = RELATIONAL_MISSING_RIGHT_ARG;
                        }
                        else
                        {
                            error_code._code = RELATIONAL_MISSING_LEFT_ARG;
                        }
                    }
                    else
                    {
                        error_code._code = INVALID_USAGE_OF_OP;
                    }
                    throw error_code;
                }
                if(first_pop->get_type() == TOKEN_STR && static_cast<Operator*>(*it)->get_operator_type() == LOGICAL)
                {
                    //expected a relational operator instead of a logical operator behind a token_str token
                    error_code._error_token = (*it)->get_val();
                    error_code._code = EXPECT_RELATIONAL;
                    throw error_code;
                }
                rpn_stack.push(new ResultSet((*it)->evaluate(second_pop, first_pop, record_indicies, field_indicies)));
                break;
                default:
                break;
            }
        }
        if(debug)
        {
            Stack<Token*>::Iterator it_debug= rpn_stack.begin();
            // cout << "Last element in stack: "<< static_cast<Integer*>(*it_debug)->get_val()<<"\n";
            cout<<"Last element in rpn_veclong_stack: ";
            (*it_debug)->print_value();
            cout<<"\n";
            cout << "RPN loop end\n";
        }
        if(rpn_stack.size() == 2)
        {
            error_code._code = EXPECT_A_RELATIONAL;
            throw error_code;
        }
        assert(rpn_stack.size() == 1 && "Result stack cannot contain more than one element");
        // v for debug
        // vectorlong debug_vector_l = static_cast<ResultSet*>(rpn_stack.pop())->get_val_list();
        // if(debug)
        //     cout<<"vector from popped stack: "<<debug_vector_l<<"\n";
        Stack<Token*>::Iterator it = rpn_stack.begin();
        if((*it)->get_type() != RESULT_SET)
        {
            error_code._code = EXPECT_A_RELATIONAL;
            throw error_code;
        } 
        return static_cast<ResultSet*>(rpn_stack.pop())->get_val_list();
    }
    friend ostream& operator << (ostream& outs, const RPN& print_me)
    {
        outs<< print_me._postfix;
        return outs;
    }
    void set_input(const Queue<Token*> &postfix)
    {
        _postfix = postfix;
    }

private:
    Queue<Token*> _postfix;
};



#endif