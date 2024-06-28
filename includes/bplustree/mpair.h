#ifndef ZAC_MPAIR_H
#define ZAC_MPAIR_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <string>
#include <cassert>
#include "../stl_utils/vector_utilities.h"
using namespace std;

template <typename K, typename V>
struct MPair{
    K key;
    vector<V> value_list;

    //--------------------------------------------------------------------------------
    /*      MPair CTORs:
     *  must have these CTORs:
     *  - default CTOR / CTOR with a key and no value: this will create an empty vector
     *  - CTOR with a key AND a value: pushes the value into the value_list
     *  _ CTOR with a key and a vector of values (to replace value_list)
     */
    //--------------------------------------------------------------------------------
    MPair(const K& k=K())
    {
        key = k;
    }
    MPair(const K& k, const V& v)
    {
        key = k;
        value_list = v;
    }
    MPair(const K& k, const vector<V>& vlist)
    {
        key = k;
        for(int i = 0; i < vlist.size(); i++)
        {
            value_list.push_back(vlist[i]);
        }
    }
    //Zac
    //Assignment operator for insert// No need to define assignment operator default assignment operator should be enough
    //--------------------------------------------------------------------------------

    //You'll need to overlod << for your vector:
    friend ostream& operator <<(ostream& outs, const MPair<K, V>& print_me)
    {
        outs <<print_me.key<<": "<<setw(10)<< print_me.value_list;
        return outs;
    }
    
    friend bool operator ==(const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        return lhs.key == rhs.key;
    }
    friend bool operator < (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        return lhs.key < rhs.key;
    }
    friend bool operator <= (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        return !(lhs > rhs);
    }
    friend bool operator > (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        return lhs.key > rhs.key;
    }
    friend bool operator >= (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        return !(lhs < rhs);
    }
    friend MPair<K, V> operator + (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        vector<V> added_vector = lhs.value_list;
        for(int i = 0; i < rhs.value_list.size(); i++)
        {
            added_vector.push_back(rhs.value_list[i]);
        }
        return MPair<K, V>(lhs.key, added_vector);
    }
};

#endif // ZAC_MPAIR_