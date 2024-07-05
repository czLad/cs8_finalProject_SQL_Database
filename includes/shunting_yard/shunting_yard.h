#ifndef SHUNTING_YARD_H
#define SHUNTING_YARD_H

// #include "gtest/gtest.h"
#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include "../queue/MyQueue.h"
#include "../stack/MyStack.h"
#include "../token/token.h"
#include "../token/leftparen.h"
#include "../token/operator.h"
#include "../token/rightparen.h"
#include "../error_code/error_code.h"

using namespace std;

class ShuntingYard
{
public:
    ShuntingYard()
    {
        _stub = 0;
        _sql_shunting_yard = false;
        _infix = Queue<Token*> ();
    }
    ShuntingYard(const Queue<Token*> &infix)
    {
        _stub = 0;
        _sql_shunting_yard = false;
        _infix = infix;
    }
    friend ostream& operator << (ostream& outs, const ShuntingYard& print_me)
    {
        outs<< print_me._infix;
        return outs;
    }
    void set_input(const Queue<Token*> &infix) //(const Queue<Token*> &postfix)
    {
        _stub = 0;
        _postfix = Queue<Token*>();
        // _postfix = postfix;
        _infix = infix;
    }
    Queue<Token*> postfix(const Queue<Token*> &infix = Queue<Token*>()) throw(Error_Code)
    {
        Error_Code error_code;
        const bool debug = false;
        if(_infix.empty())
        {
            _infix = infix;
        }
        if(debug)
            cout<<"Postfix of shuntingyard\n";
        //Shunting Yard does clear its postfix after evaluation(very latest update)
        _postfix.clear();
        Stack<Token*> operator_stack;
        int relational_count = 0;
        int token_str_count = 0;
        if(debug)
        {
            cout<<"_infix in shunting yard\n";
            if(!_infix.empty())
            {
                for(Queue<Token*>::Iterator it = _infix.begin(); it != _infix.end(); it++)
                {
                    (*it)->print_value();
                    cout<<"|";
                }
            }
            cout<<"\n";
        }
        for(Queue<Token*>::Iterator it = _infix.begin(); it != _infix.end(); ++it)
        {
            if(debug)
                cout <<"switch: "<<(*it)->get_type()<< "\n";
            switch((*it)->get_type())
            {
            case TOKEN_STR:
            if(debug)
                cout << "case 1 SY\n";
            token_str_count++;
            if(token_str_count == 3)
            {
                if(_sql_shunting_yard)
                {
                    if(_sql_field_indicies->contains((*it)->get_val()))
                    {
                        if(static_cast<Operator*>(*(operator_stack.begin()))->get_operator_type() == RELATIONAL)
                            error_code._code = EXPECT_LOGICAL;
                        else
                            error_code._code = EXPECT_A_RELATIONAL;
                    }
                    else
                    {
                        error_code._error_token = _postfix.back()->get_val();
                        error_code._code = SYNTAX_ERR_AT_NEAR;
                        error_code._modify_to_postgre = true;
                    }
                }
                else
                    error_code._code = INVALID_CONDITION;
                throw error_code;
            }
            _postfix.push(*it);

            break;
            case OPERATOR:
            if(debug)
                    cout << "case 2 SY\n";
            if(operator_stack.empty())
                operator_stack.push(*it);
            else
            {
                if(debug)
                {
                    cout << "static_cast<Operator*>(*stack_operator)->get_precedence(): " <<
                    static_cast<Operator*>(*(operator_stack.begin()))->get_precedence()<<"\n";
                    cout << "static_cast<Operator*>(*it)->get_precedence(): " <<
                    static_cast<Operator*>(*it)->get_precedence()<<"\n";
                }
                //v while() checks if stack is empty, if left parenthesis is on top of operator_stack, if precedence of op on top of op_stack
                // is greater than or equal to that of _infix queue.
                while(!operator_stack.empty() 
                    && (*operator_stack.begin())->get_type() != LPAREN
                    && static_cast<Operator*>(*(operator_stack.begin()))->get_precedence() >= static_cast<Operator*>(*it)->get_precedence())
                {
                    if(debug)
                        cout << "static_cast<Operator*>(*stack_operator)->get_precedence() >= static_cast<Operator*>(*it)->get_precedence())\n";
                    if(static_cast<Operator*>(*(operator_stack.begin()))->get_operator_type() == RELATIONAL && 
                    static_cast<Operator*>(*(operator_stack.begin()))->get_precedence() == static_cast<Operator*>(*it)->get_precedence())
                    {
                        error_code._error_token = (*it)->get_val();
                        error_code._code = INVALID_USAGE_OF_OP;
                        throw error_code;
                    }
                    _postfix.push(operator_stack.pop());
                    token_str_count = 0;
                }
                operator_stack.push(*it);
            }
            break;
            case LPAREN:
            if(debug)
                cout << "case 3 SY\n";
            token_str_count = 0;
            operator_stack.push(*it);
            break;
            case RPAREN:
            if(debug)
                cout << "case 4 SY\n";
            token_str_count = 0;
            if(operator_stack.empty())
            {
                error_code._code = MISSING_LEFT_PAREN;
                throw error_code;
            }
            while((*operator_stack.begin())->get_type() != LPAREN)
            {
                if(debug)
                    cout << "not LeftParen\n";
                _postfix.push(operator_stack.pop());
                if(operator_stack.empty())
                {
                    error_code._code = MISSING_LEFT_PAREN;
                    throw error_code;
                }
            }
            operator_stack.pop();
            break;
            default:
            break;
            }
        }
        while(!operator_stack.empty())
        {
            if(debug)
                cout << "Shunting Yard while.\n";
            
            _postfix.push(operator_stack.pop());
        }
        if(debug)
        {
            cout<<"_postfix in shunting yard\n";
            for(Queue<Token*>::Iterator it = _postfix.begin(); it != _postfix.end(); it++)
            {
                (*it)->print_value();
                cout<<"|";
            }
            cout<<"\n";
        }
        if(_postfix.back()->get_type() == LPAREN)
        {
            error_code._code = MISSING_RIGHT_PAREN;
            throw error_code;
        }
        return _postfix;
    }
    void infix(const Queue<Token*> &infix)
    {
        _infix = infix;
    }
    void set_sql_shuting_yard(bool sql_shunting_yard, const map_sl* sql_field_indicies){
        _sql_shunting_yard = true;
        _sql_field_indicies = sql_field_indicies;
    }
    bool get_sql_shunting_yard(){return _sql_shunting_yard;}

private:
    int _stub;
    Queue<Token*> _infix;
    Queue<Token*> _postfix;
    bool _sql_shunting_yard;
    const map_sl* _sql_field_indicies;
};



#endif