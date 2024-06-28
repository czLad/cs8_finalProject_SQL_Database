#ifndef ZAC_VECTOR_UTILITIES_H
#define ZAC_VECTOR_UTILITIES_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <string>
#include <cassert>
using namespace std;

template<class T>
ostream& operator <<(ostream& outs, const vector<T>& list) //print vector list
{
    outs<<"[";
    for(int i = 0; i < list.size(); i++)
        outs<<list[i]<<"|";
    outs<<"]";
    return outs;
}

template <typename T>
vector<T>& operator +=(vector<T>& list, const T& addme) //list.push_back addme
{
    list.push_back(addme);
    return list;
}

// template<typename T>
// void init_vector(vector<T>& list, const string& txt_fi_name)
// {
    
//     for(int i = 0; i < list.size(); i++)
//         list.push_back(T());
// }


#endif // ZAC_VECTOR_UTILITIES_