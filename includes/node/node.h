#ifndef NODE_H
#define NODE_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <list>
#include <cassert>

using namespace std;

template <class T>
struct node
{
    T _item;
    node<T>* _next;
    node<T>* _prev;
    //CTOR
    node(const T& item = T(), node<T>* next = NULL, node<T>* prev = NULL):
                                                                       _item(item), 
                                                                       _next(next),
                                                                       _prev(prev){}
    friend ostream& operator << (ostream& outs, const node<T>& print_me){
        outs<<"<-["<<print_me._item<<"]->";
        return outs;
    }
};

#endif