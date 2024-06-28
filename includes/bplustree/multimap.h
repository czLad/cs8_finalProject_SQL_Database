#ifndef ZAC_MULTIMAP_H
#define ZAC_MULTIMAP_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <string>
#include <cassert>
#include "mpair.h"
#include "../stl_utils/vector_utilities.h"
#include "bplustree.h"
using namespace std;

template <typename K, typename V>
class MMap
{
public:
    typedef BPlusTree<MPair<K, V> > map_base;
    class Iterator{
    public:
        friend class MMap;
        Iterator()
        {
            _it = nullptr;
        }
        Iterator(typename map_base::Iterator it)
        {
            _it = it;
        }
        Iterator operator ++(int unused)
        {
            return Iterator(_it++);
        }
        Iterator operator ++()
        {
            return Iterator(++_it);
        }
        MPair<K, V> operator *()
        {
            return *_it;
        }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs)
        {
            return lhs._it == rhs._it;
        }
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs)
        {
            return !(lhs == rhs);
        }

    private:
        typename map_base::Iterator _it;
    };

    MMap():mmap(true)
    {

    }

//  Iterators
    Iterator begin()
    {
        return Iterator(mmap.begin());
    }
    Iterator end()
    {
        return Iterator(mmap.end());
    }

//  Capacity
    int size() const
    {
        return mmap.elements_count();
    }
    bool empty() const
    {
        return !size();
    }

//  Element Access
    const vector<V>& operator[](const K& key) const
    {
        // const vector<V>* temp;
        // vector<V>* temp;
        return mmap.get(key).value_list;
    }
    vector<V>& operator[](const K& key)
    {
        return mmap.get(key).value_list;
    }

//  Modifiers
    void insert(const K& k, const V& v)
    {
        MPair<K, V> p(k, v);
        mmap.insert(p);
    }
    void erase(const K& key)
    {
        mmap.remove();
    }
    void clear()
    {
        mmap.clear_tree();
    }

//  Operations:
    bool contains(const K& key) const
    {
        return mmap.contains(key);
    }
    vector<V> &get(const K& key)
    {
        return mmap.get(key).value_list;
    }
    //Zac
    //at
    // vector<V> at(const K& key)
    // {
    //     return mmap.get(key).value_list;
    // }
    vector<V> &at(const K& key)
    {
        return mmap.get(key).value_list;
    }

    Iterator find(const K& key)
    {
        return Iterator(mmap.find(key));
    }
    int count(const K& key)
    {
        return mmap.elements_count();
    }
    // I have not writtent hese yet, but we will need them:
    //    int count(const K& key);
    //    lower_bound
    //    upper_bound
    //    equal_range:
    // MMap<int, string>::Iterator it;
    Iterator lower_bound(const K& key)
    {
        return Iterator(mmap.lower_bound(key));
    }
    Iterator upper_bound(const K& key)
    {
        return Iterator(mmap.upper_bound(key));
    }
    Iterator equal_range(const K& key)
    {
        return Iterator();
    }


    bool is_valid()
    {
        return mmap.is_valid();
    }

    friend ostream& operator<<(ostream& outs, const MMap<K, V>& print_me){
        outs<<print_me.mmap<<endl;
        return outs;
    }
    void print_lookup()
    {
        const bool debug = false;
        if(debug)
        {
            cout<<"entered print_lookup\n";
            cout<<"mmap.size(): "<<size()<<"\n";
        }
        if(!empty())
        {
            for(Iterator it = mmap.begin(); it != mmap.end(); it++)
            {
                cout<<setw(20)<<*it<<"\n";
            }
        }
        if(debug)
            cout<<"mmap.begin(): "<<*(mmap.begin())<<"\n";
    }

private:
    BPlusTree<MPair<K, V> > mmap;
};


#endif // ZAC_MULTIMAP_