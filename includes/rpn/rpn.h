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
        for(Queue<Token*>::Iterator it = _postfix.begin(); it != _postfix.end(); ++it)
        {
            Token* first_pop;
            Token* second_pop;
            ResultSet result;
            if(debug)
                cout <<"switch: "<<(*it)->get_type()<< "\n";
            switch((*it)->get_type())
            {
                case TOKEN_STR:
                if(debug)
                    cout << "case TOKEN_STR RPN\n";
                rpn_stack.push(*it);
                break;
                case OPERATOR:
                if(debug)
                    cout << "case RELATIONAL RPN\n";
                if(rpn_stack.size() < 2)
                {
                    error_code._code = 9;
                    throw error_code;
                }
                first_pop = rpn_stack.pop();
                second_pop = rpn_stack.pop();
                if(first_pop->get_type() != second_pop->get_type())
                {
                    error_code._code = 9;
                    throw error_code;
                }
                if(first_pop->get_type() == TOKEN_STR && static_cast<Operator*>(*it)->get_operator_type() == LOGICAL)
                {
                    error_code._code = 9;
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
        assert(rpn_stack.size() == 1 && "Result stack cannot contain more than one element");
        // v for debug
        // vectorlong debug_vector_l = static_cast<ResultSet*>(rpn_stack.pop())->get_val_list();
        // if(debug)
        //     cout<<"vector from popped stack: "<<debug_vector_l<<"\n";
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