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
        _infix = Queue<Token*> ();
    }
    ShuntingYard(const Queue<Token*> &infix)
    {
        _stub = 0;
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

                    _postfix.push(operator_stack.pop());
                }
                operator_stack.push(*it);
            }
            break;
            case LPAREN:
            if(debug)
                cout << "case 3 SY\n";
            operator_stack.push(*it);
            break;
            case RPAREN:
            if(debug)
                cout << "case 4 SY\n";
            if(operator_stack.empty())
            {
                error_code._code = 8;
                throw error_code;
            }
            while((*operator_stack.begin())->get_type() != LPAREN)
            {
                if(debug)
                    cout << "not LeftParen\n";
                _postfix.push(operator_stack.pop());
                if(operator_stack.empty())
                {
                    error_code._code = 8;
                    throw error_code;
                }
            }
            operator_stack.pop();
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
            error_code._code = 7;
            throw error_code;
        }
        return _postfix;
    }
    void infix(const Queue<Token*> &infix)
    {
        _infix = infix;
    }

private:
    int _stub;
    Queue<Token*> _infix;
    Queue<Token*> _postfix;
};



#endif