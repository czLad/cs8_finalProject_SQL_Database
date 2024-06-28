#ifndef ZAC_BTREE_ARRAY_FUNCTIONS_H
#define ZAC_BTREE_ARRAY_FUNCTIONS_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <string>
#include <cassert>
#include "../array_functions/array_functions.h"
#include "pair.h"
#include "mpair.h"
using namespace std;

template <class T>
T maximal(const T& a, const T& b)                      //return the larger of the two items
{
    return a > b? a: b;
}

template <class T>
void swap(T& a, T& b)  //swap the two items
{
    T temp = a;
    a = b;
    b = temp;
}

template <class T>
int index_of_maximal(T data[ ], int n)                 //return index of the largest item in data
{
    int max_index = 0;
    T max = data[0];
    for(int i = 1; i < n; i++)
    {
        if(data[i] > max)
        {
            max = data[i];
            max_index = i;
        }
    }
    return max_index;
}
template <class T>
void ordered_insert(T data[ ], int& n, T entry){
    for (int i=0; i<n; i++) {
        cout << "hehe" << endl;
    }
}        //insert entry into the sorted array
         //data with length n

template <class T>
int first_ge(const T data[ ], int n, const T& entry)   //return the first element in data that is
{
    // cout<<"first_ge n: "<<n<<"\n";
    // cout<<"first_ge entry: "<<entry<<"\n";
    for(int i = 0; i < n; i++)
    {
        if(data[i] >= entry)
            return i;
    }
    return n;
}
template <class T>
bool is_present(const T data[ ], int n, const T& entry, int &found_index)   //return the first element in data that is
{
    for(int i = 0; i < n; i++)
    {
        if(data[i] == entry)
        {
            found_index = i;
            return true;
        }
    }
    found_index = -1;
    return false;
}                                                          //not less than entry
//Pre condi n must be less than capacity
template <class T>
void attach_item(T data[ ], int& n, const T& entry)    //append entry to the right of data
{
    data[n++] = entry;
    // cout<<"n: "<<n<<"\n";
    // cout<<"data[n]: "<<data[n]<<"\n";
    // n++;
}
template <class T>
void insert_item(T data[ ], int i, int& n, T entry)    //insert entry at index i in data
{
    // cout<<"i: "<<i<<"\n";
    // cout<<"n: "<<n<<"\n";
    assert(i < n && "i must be less than n\n");
    shift_right(data, n, i);
    data[i] = entry;
}
template <class T>
void detach_item(T data[ ], int& n, T& entry)          //remove the last element in data and place
{
    entry = data[n - 1];
    n--;
}                                                        //it in entry

template <class T>
void delete_item(T data[ ], int i, int& n, const T& entry)   //delete item at index i and place it in entry
{
    shift_left(data, n, i);
}
template <class T>
void merge(T data1[ ], int& n1, T data2[ ], int& n2)   //append data2 to the right of data1
{
    for(int i = 0; i < n2; i++)
        attach_item(data1, n1, data2[i]);
}
template <class T>
void split(T data1[ ], int& n1, T data2[ ], int& n2)   //move n/2 elements from the right of data1
{
    //n1 is datacount that's in excess (odd #)
    //n2 + 1 is the index of data1 to start copying 
    //n2 is the size of data2
    n2 = n1/2;
    for(int i = n1 - n2; i < n1; i++)
    {
        data2[i - (n1 - n2)] = data1[i];
    }
    n1 = n1 - n2;
}
                                                        //and move to data2
//not using this copy array, potential bug if Professor uses only this header file
template <class T>
void copy_array(T dest[], const T src[],
                int& dest_size, int src_size);              //copy src[] into dest[]

template <class T>
void print_array(const T data[], int n, int pos = -1);  //print array data

template <class T>
bool is_gt(const T data[], int n, const T& item)       //item > all data[i]
{
    for(int i = 0; i < n; i++)
    {
        if(item <= data[i])
            return false;
    }
    return true;
}

template <class T>
bool is_le(const T data[], int n, const T& item)       //item <= all data[i]
{
    for(int i = 0; i < n; i++)
    {
        if(item > data[i])
            return false;
    }
    return true;
}

//-------------- Vector Extra operators: ---------------------

template <typename T>
ostream& operator <<(ostream& outs, const vector<T>& list); //print vector list

template <typename T>
vector<T>& operator +=(vector<T>& list, const T& addme); //list.push_back addme



#endif // ZAC_BTREE_ARRAY_FUNCTIONS_