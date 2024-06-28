#ifndef ARRAY_FUNCTIONS_H
#define ARRAY_FUNCTIONS_H

#include <iostream>
#include <iomanip>
#include <cassert>

using namespace std;

const int MINIMUM_CAPACITY = 3;

template<class T>
T* allocate(int capacity= MINIMUM_CAPACITY);

template<class T>
T* reallocate(T* a, int size, int capacity);


template<class T>
void print_array(T* a, int size, 
            int capacity = 0, ostream& outs = cout);

template <class T>
void print(T* a, unsigned int how_many, 
                                ostream& outs = cout);

template<class T>
T* search_entry(T* a, int size, const T& find_me);

template <class T>
int search(T* a, int size, const T& find_me);

template <class T>
void shift_left(T* a, int& size, int shift_here);

template <class T>
void shift_left(T* a, int& size, T* shift_here); 
 
template <class T>
void shift_right(T *a, int &size, int shift_here);

template <class T>
void shift_right(T *a, int &size, T* shift_here);

template<class T>
void copy_array(T *dest, const T* src, 
                                int many_to_copy);

template <class T>
T* copy_array(const T *src, int size);

template <class T>
string array_string(const T *a, int size); 

template <class T>
void _append(T *a, int &size, T val);

template<class T>
void init_array(T* a, int capacity, T val);

//Extra
// template<class T>
// void split(T* src, T* dest, int &s_size, int &d_size);

//Definitions

template<class T> //define and declare in the file is easier(.cpp gets created after looking at what type T is)
T* allocate(int capacity) //defining, paramater is variable
{
    T* p = new T[capacity];
    return p;
}
            //allocate 'capacity' 
                                                        //      elements. 
                                                       //  return array
template<class T>
T* reallocate(T* a, int size, int capacity) //size of old array
{
    assert(a != nullptr && "Array cannot be empty.");
    T *arr_New = new T[capacity];
    copy_array(arr_New, a, size);
    delete[] a;
    
    return arr_New; // delete by caller
}            //take array, resize it 
                                                        //  return new array. 
                                                        //  delete old array
// template<class T>
// T* init_array(T* a, int capacity, T val)
// {
//     T *walker = a;
//     for(int i = 0; i < capacity; i++)
//     {
//         *walker = val;
//         walker++;
//     }
// }

template<class T>
void init_array(T* a, int capacity, T val)
{
    T *walker = a;
    for(int i = 0; i < capacity; i++)
    {
        *walker = val;
        walker++;
    }
}

template<class T>
void print_array(T* a, int size, 
            int capacity, ostream& outs)
{
    assert(a != nullptr && "Array cannot be empty.");
    T* walker = a;
    outs << "( " << size << "/" << capacity << " )" << "[|";
    for(int i = 0; i < size; i++, walker++)
    {
        outs << *walker << "|";
    }
    outs << "]\n";
}
                                                        //prints 
                                                        //  (size/capacity)
                                                        //  for debugging

template <class T>
void print(T* a, unsigned int how_many, 
                                ostream& outs)  //print array
{
    assert(a != nullptr && "Array cannot be empty.");
    T* walker = a;
    outs << "( how_many: " << how_many << " )" << "[|";
    for(int i = 0; i < how_many; i++, walker++)
    {
        outs << *walker << "|";
    }
    outs << "]\n";
}

template<class T>
T* search_entry(T* a, int size, const T& find_me)      //search for 'find me'
{
    assert(size < -1 && "Array cannot be empty.");
    T* walker = a;

    for(int i = 0; i < size; i++, walker++)
    {
        if(*walker == find_me)
            return walker;
    }
    return nullptr;
}

template <class T>
int search(T* a, int size, const T& find_me)           //search for 'find_me' 
{
    T* walker = a;

    for(int i = 0; i < size; i++, walker++)
    {
        if(*walker == find_me)
            return (walker - a);
    }
    return -1;
}

template <class T>
void shift_left(T* a, int& size, int shift_here)       //shift left @ pos:
{
    T* walker = a;
    int pos = shift_here;
    walker = a + pos;
    assert(pos < size && "Key cannot be found in array");
    for(int i = pos; i < size; i++)
    {
        *walker = *(walker + 1);
        walker++;
    } 
    size--;
}

template <class T>
void shift_left(T* a, int& size, T* shift_here)        //shift left @ pos: 
{
    int shift_index = shift_here - a;
    shift_left(a, size, shift_index);
}

template <class T>
void shift_right(T *a, int &size, int shift_here)      //shift right: 
{
    T* walker = a + size - 1;
    int pos = shift_here;
    assert(pos < size && "Key cannot be found in array");
    size++;
    for(int i = pos; i < size -1; i++)
    {
        *(walker+1) = *walker;
        walker--;
    }

}                                                         //      make a hole
                                                           //   make a hole
template <class T>
void shift_right(T *a, int &size, T* shift_here)       //shift right: 
{
    int shift_index = shift_here - a;                  
    shift_right(a, size, shift_index);
}                                                       //   make a hole

template<class T>
void copy_array(T *dest, const T* src, 
                                int many_to_copy)      //copy from src to dest
{
    assert(src != nullptr && "Array cannot be empty.");
    assert(dest != nullptr && "Array cannot be empty.");
    const T *walker_src = src; 
    T *walker_dest = dest;
    for(int i = 0; i < many_to_copy; i++, walker_dest++, walker_src++)
        *walker_dest = *walker_src;
}

template <class T>
T* copy_array(const T *src, int size)                  //return a 
{
    T *copy_arr = new T[size];
    T *walker_cpy = copy_arr; // delete in caller
    const T *walker_src = src;
    for(int i = 0; i < size; i++, walker_cpy++, walker_src++)
        *walker_cpy = *walker_src; 

    return copy_arr;
}                                                        //  copy of src

template <class T>
string array_string(const T *a, int size)
{
    const T* walker_a = a;
    string a_string;
    a_string = "\n[|";
    for(int i = 0; i < size; i++, walker_a++)
    {
        a_string += to_string(*walker_a) + "|";
    }
    a_string += "] [size: " + to_string(size) + "]\n"; 
    
    return a_string;
}

template <class T>                                  //append array
void _append(T *a, int &size, T val)
{
    T *end = a + size;
    *end = val;
    size++;
}

//Extra from Exam 1

template<class T>
void split(T* src, T* dest, int &s_size, int &d_size)
{
    int half_size = s_size / 2;
    d_size = 0;
    dest = reallocate(dest, d_size, half_size);
    T* back_src_walker = src + s_size - 1;
    for(int i = 0; i < half_size; i++)
    {
        dest = add_entry(dest, *back_src_walker, d_size, half_size);
        back_src_walker++;
    }
}

#endif