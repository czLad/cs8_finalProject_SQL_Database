#ifndef ZAC_MAP_H
#define ZAC_MAP_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <string>
#include <cassert>
#include "pair.h"
#include "bplustree.h"

using namespace std;

template <typename K, typename V>
class Map
{
public:
    typedef BPlusTree<Pair<K, V> > map_base;
    class Iterator{
    public:
        friend class Map;
        Iterator()
        {

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
        Pair<K, V> operator *()
        {
            // return Pair<K, V>();
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

    Map():map(false)
    {

    }
//  Iterators
    Iterator begin()
    {
        return Iterator(map.begin());
    }
    Iterator end()
    {
        return Iterator(map.end());
    }

//  Capacity
    int size() const
    {
        return map.elements_count();
    }
    bool empty() const
    {
        return !size();
    }

//  Element Access
    V& operator[](const K& key)
    {
        return map.get(key).value;
    }
    const V& operator[](const K& key) const
    {
        // const vector<V>* temp;
        // vector<V>* temp;
        return map.get(key).value;
    }
    bool contains(const K& key) const
    {
        return map.contains(key);
    } 
    V& at(const K& key)
    {
        return map.get(key).value;
    }
    const V& at(const K& key) const
    {
        return map.get(key).value;
    }


//  Modifiers
    void insert(const K& k, const V& v)
    {
        Pair<K, V> p(k, v);
        map.insert(p);
    }
    void erase(const K& key)
    {
        map.remove(key);
    }
    void clear()
    {
        map.clear_tree();
    }
    V get(const K& key)
    {
        return map.get(key).value;
    }

//  Operations:
    Iterator find(const K& key)
    {
        return Iterator(map.find(key));
    }
    bool contains(const Pair<K, V>& target) const
    {
        return map.contains(target);
    }

    // I have not writtent hese yet, but we will need them:
    //    int count(const K& key);
    //    lower_bound
    //    upper_bound
    //    equal_range:
    Iterator lower_bound(const K& key)
    {
        //Performance made better using recursion
        return Iterator(map.lower_bound(key));
    }
    Iterator upper_bound(const K& key)
    {
        return Iterator(map.upper_bound(key));
    }
    Iterator equal_range(const K& key)
    {
        return Iterator();
    }

    bool is_valid(){return map.is_valid();}

    friend ostream& operator<<(ostream& outs, const Map<K, V>& print_me){
        outs<<print_me.map<<endl;
        return outs;
    }
private:
    int key_count;
    BPlusTree<Pair<K, V> > map;
};


#endif // ZAC_MAP_