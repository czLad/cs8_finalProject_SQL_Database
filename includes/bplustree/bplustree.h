#ifndef ZAC_BPLUSTREE_H
#define ZAC_BPLUSTREE_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <string>
#include <cassert>
#include "btree_array_funcs.h"

using namespace std;

template <class T>
class BPlusTree
{
public:
    class Iterator{
    public:
        friend class BPlusTree;
        Iterator(BPlusTree<T>* _it=NULL, int _key_ptr = 0):
                                                it(_it), key_ptr(_key_ptr){}

        T operator *()
        {
            return it->data[key_ptr];
        }

        Iterator operator++(int un_used)
        {
            //postfix means return first and then increment - so return the value before incrementation?
            int prev_key_ptr = key_ptr;
            BPlusTree<T>*prev_it = it;
            if(key_ptr < it->data_count - 1)
                key_ptr++;
            else
            {
                it = it->next;
                key_ptr = 0;
            }

            return Iterator(prev_it, prev_key_ptr);
        }
        Iterator operator++()
        {
            //prefix means fix first and then return - source:youtube
            if(key_ptr < it->data_count - 1)
                key_ptr++;
            else
            {
                it = it->next;
                key_ptr = 0;
            }
            return *this;
        }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs)
        {
            return lhs.it == rhs.it && lhs.key_ptr == rhs.key_ptr;
        }
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs)
        {
            return !(lhs == rhs);
        }
        void print_Iterator(){
            if (it){
                cout<<"iterator: ";
                print_array(it->data, it->data_count, key_ptr);
            }
            else{
                cout<<"iterator: NULL, key_ptr: "<<key_ptr<<endl;
            }
        }
        bool is_null(){return !it;}
        void info(){
            cout<<endl<<"Iterator info:"<<endl;
            cout<<"key_ptr: "<<key_ptr<<endl;
            cout<<"it: "<<*it<<endl;
        }
        void get_data(vector<T>& data_vec)
        {
            for(int i = 0; i < it->data_count; i++)
                data_vec.push_back(it->data[i]);
        }
        void next()
        {
            it = it->next;
        }

    private:
        BPlusTree<T>* it;
        int key_ptr;
    };

    BPlusTree(bool dups = false):dups_ok(dups), data_count(0),
                                   child_count(0), next(nullptr)
    {
        const bool debug = false;
        if(debug)
        {
            cout<<"BPlusTree CTOR Fired\n";
        }
         //MINIMUM by default is 1
        //init data array and pointer array
        //init the buffer as well?
        for(int i = 0; i < MAXIMUM + 2; i++)
        {
            if(i < MAXIMUM + 1)
                data[i] = T();
            subset[i] = nullptr;
        }
    }
    BPlusTree(T *a, int size, bool dups = false): data_count(0), child_count(0), dups_ok(dups)
    {
        const bool debug = false;
        if(debug)
        {
            cout<<"BPlusTree(T *a, int size, bool dups = false) CTOR Fired\n";
        }
        T* walker = a;
        for(int i = 0; i < size; i++, walker++)
            insert(*walker);
    }

    //big three:
    BPlusTree(const BPlusTree<T>& other): data_count(0), child_count(0)
    {
        const bool debug = false;
        if(debug)
        {
            cout<<"BPlusTree Copy CTOR Fired\n";
        }
        copy_tree(other);
        dups_ok = other.dups_ok;
    }
    ~BPlusTree()
    {
        const bool debug = false;
        if(debug)
        {
            cout<<"BPlusTree DTOR Fired\n";
        }
        clear_tree();
    }
    BPlusTree<T>& operator =(const BPlusTree<T>& RHS)
    {
        const bool debug = false;
        if(debug)
        {
            cout<<"BPlusTree Assignment Operator Fired\n";
        }
        if(this == &RHS)
            return *this;
        clear_tree();
        copy_tree(RHS);
        dups_ok = RHS.dups_ok;
        return *this;
    }

    void copy_tree(const BPlusTree<T>& other)  //copy other into this object
    {
        //newly changed bug 5/30/24
        BPlusTree<T>* last_leaf= new BPlusTree<T>;
        copy_tree(other, last_leaf);
        last_leaf->next = nullptr;
    }
    void copy_tree(const BPlusTree<T>& other, BPlusTree<T>*& last_node)
    {
        if(other.is_leaf())
        {
            data_count = other.data_count;
            copy_array(data, other.data, data_count);
            //bug
            last_node->next = this;
            last_node = this;
        }
        else
        {
            data_count = other.data_count;
            child_count = other.child_count;
            copy_array(data, other.data, data_count);
            for(int i = 0; i < child_count; i++)
            {
                subset[i] = new BPlusTree<T>();
                subset[i]->copy_tree(*(other.subset[i]), last_node);
            }
        }
    }

    void insert(const T& entry)   //insert entry into the tree
    {
        // cout<<"hello from insert bplus tree\n";
        loose_insert(entry);
        grow_tree();
    }
    void remove(const T& entry)   //remove entry from the treege
    {
        if(data_count > 0)
        {
            loose_remove(entry);
            if(!is_leaf())
                shrink_tree();
        }
    }
    void clear_tree()             //clear this object (delete all nodes etc.)
    {
        if(is_leaf())
        {
            data_count = 0;
            //should we set data[0] to empty? could have more than one data tho
            //need to do this?
            next = nullptr;
        }
        else
        {
            for(int i = 0; i < child_count; i++)
            {
                subset[i]->clear_tree();
                subset[i]->data_count = 0;
                subset[i]->child_count = 0;
                delete subset[i];
                subset[i] = nullptr;
            }
            child_count = 0;
            data_count = 0;
        }
    }
    //v might have a bug for B+Tree pasted from BTree
    void shrink_tree()
    {
        if(data_count < MINIMUM)
        {
            //for BTree MIN 1
            BPlusTree<T>* shrink_ptr = subset[0];
            shallow_copy_remove();
            shrink_ptr->data_count = 0;
            shrink_ptr->child_count = 0;
            delete shrink_ptr;
            shrink_ptr = nullptr;
        }
    }
    void shallow_copy_remove()
    {
        data_count = subset[0]->data_count;
        child_count = subset[0]->child_count;
        //Last bug^ have to set counts before copying to the subset array
        copy_array(data, subset[0]->data, subset[0]->data_count);
        copy_array(subset, subset[0]->subset, subset[0]->child_count);
    }
    void make_tree()
    {
        data[0] = 100;
        data[1] = 200;
        data_count = 2;
        subset[0] = new BPlusTree<T>();
        subset[1] = new BPlusTree<T>();
        subset[2] = new BPlusTree<T>();
        child_count = 3;
        //first_subtree
        BPlusTree* first_child_ptr = subset[0];
        first_child_ptr->data[0] = 50;
        first_child_ptr->data[1] = 70;
        first_child_ptr->data_count = 2;
        first_child_ptr->subset[0] = new BPlusTree<T>();
        first_child_ptr->subset[1] = new BPlusTree<T>();
        first_child_ptr->subset[2] = new BPlusTree<T>();
        first_child_ptr->child_count = 3;
        //first_subtree's subtree or grandchild
        BPlusTree* first_grandchild_1_ptr = first_child_ptr->subset[0];
        BPlusTree* first_grandchild_2_ptr = first_child_ptr->subset[1];
        BPlusTree* first_grandchild_3_ptr = first_child_ptr->subset[2];
        first_grandchild_1_ptr->data[0] = 20;
        first_grandchild_1_ptr->data_count = 1;
        //connecting leaf nodes first_grandchild is a leaf
        first_grandchild_1_ptr->next = first_grandchild_2_ptr;
        //child count by default zero
        first_grandchild_2_ptr->data[0] = 50;
        first_grandchild_2_ptr->data_count = 1;
        first_grandchild_2_ptr->next = first_grandchild_3_ptr;

        first_grandchild_3_ptr->data[0] = 70;
        first_grandchild_3_ptr->data[1] = 90;
        first_grandchild_3_ptr->data_count = 2;

        //second_subtree 
        BPlusTree* second_child_ptr = subset[1];
        second_child_ptr->data[0] = 150;
        second_child_ptr->data[1] = 175;
        second_child_ptr->data_count = 2;
        second_child_ptr->subset[0] = new BPlusTree<T>();
        second_child_ptr->subset[1] = new BPlusTree<T>();
        second_child_ptr->subset[2] = new BPlusTree<T>();
        second_child_ptr->child_count = 3;
        BPlusTree* second_grandchild_1_ptr = second_child_ptr->subset[0];
        BPlusTree* second_grandchild_2_ptr = second_child_ptr->subset[1];
        BPlusTree* second_grandchild_3_ptr = second_child_ptr->subset[2];
        second_grandchild_1_ptr->data[0] = 100;
        second_grandchild_1_ptr->data_count = 1;
        first_grandchild_3_ptr->next = second_grandchild_1_ptr;
        //child count by default zero
        second_grandchild_2_ptr->data[0] = 150;
        second_grandchild_2_ptr->data_count = 1;
        second_grandchild_1_ptr->next = second_grandchild_2_ptr;

        second_grandchild_3_ptr->data[0] = 175;
        second_grandchild_3_ptr->data_count = 1;
        second_grandchild_2_ptr->next = second_grandchild_3_ptr;

        //third_subtree 
        BPlusTree* third_child_ptr = subset[2];
        third_child_ptr->data[0] = 250;
        third_child_ptr->data_count = 1;
        third_child_ptr->subset[0] = new BPlusTree<T>();
        third_child_ptr->subset[1] = new BPlusTree<T>();
        third_child_ptr->child_count = 2;
        BPlusTree* third_grandchild_1_ptr = third_child_ptr->subset[0];
        BPlusTree* third_grandchild_2_ptr = third_child_ptr->subset[1];
    
        third_grandchild_1_ptr->data[0] = 200;
        third_grandchild_1_ptr->data[1] = 235;
        third_grandchild_1_ptr->data_count = 2;
        second_grandchild_3_ptr->next = third_grandchild_1_ptr;
        //child count by default zero

        third_grandchild_2_ptr->data[0] = 250;
        third_grandchild_2_ptr->data_count = 1;
        third_grandchild_1_ptr->next = third_grandchild_2_ptr;

    }
    void grow_tree()
    {
        // cout<<"child_count: "<<child_count<<"\n";
        if(data_count > MAXIMUM)
        {
            shallow_copy();
            fix_excess(0);
        }
    }
    void shallow_copy()
    {
        BPlusTree* growth_ptr = new BPlusTree<T>();
        copy_array(growth_ptr->data, data, data_count);
        growth_ptr->data_count = data_count;
        copy_array(growth_ptr->subset, subset, child_count);
        growth_ptr->child_count = child_count;
        //set up self for fix_excessive
        data_count = 0;
        //Preventive measure for dangling pointers
        for(int i = 0; i < child_count; i++)
            subset[i] = nullptr;
        subset[0] = growth_ptr;
        child_count = 1;
    }
    bool contains(const T& entry) const //true if entry can be found
    {
        // cout<<"beginning of contains: "<<"\n";
        // cout<<entry<<"\n";
        // cout<<"conatins entry: "<<entry<<"\n";
        // cout<<"typeid: "<<typeid(entry).name()<<"\n";
        // cout<<"before ge_i\n";
        int ge_i = first_ge(data, data_count, entry);
        // int ge_i = 0;
        // cout<<"ge_i from contains: "<<"\n";
        bool found = ge_i < data_count 
                    && data[ge_i] == entry;
        // cout<<"ge_i from contains 1: "<<"\n";
        if(found)
            return true;
        if(!is_leaf())
            return subset[ge_i]->contains(entry);
        return false;
    }
    T& get(const T& entry) //return a reference to entry
    {
    //If entry is not in the tree, add it to the tree
        // cout<<"hello from get inside Bplus tree\n";
        const bool debug = false;
        if (!contains(entry))
            insert(entry);

        return get_existing(entry);
    }
    const T& get(const T& entry)const   //return a reference to entry
    {
        const bool debug = false;
        if (!contains(entry))
            insert(entry);

        return get_existing(entry);
    }
    T& get_existing(const T& entry)     //return a reference to entry
    {
        int ge_i = first_ge(data, data_count, entry);
        bool found = ge_i < data_count 
                    && data[ge_i] == entry;
        if(found)
        {
            if(is_leaf())
                return data[ge_i];
            else
                return subset[ge_i+1]->get_existing(entry);
        }
        if(!is_leaf())
            return subset[ge_i]->get_existing(entry);
        assert(found && "Key does not exit in BPlusTree\n");
    }
    Iterator find(const T& key)         //return an iterator to this key. //     NULL if not there.
    {
        int ge_i = first_ge(data, data_count, key);
        bool found = ge_i < data_count 
                    && data[ge_i] == key;
        if(found)
        {
            if(!is_leaf())
                return subset[ge_i+1]->find(key);
            else
                return Iterator(this, ge_i);        
        }
        if(!is_leaf())
            return subset[ge_i]->find(key);
        return Iterator();
    }
    Iterator lower_bound(const T& key)  //return first that goes NOT BEFORE
    {
        int ge_i = first_ge(data, data_count, key);
        bool found = ge_i < data_count 
                    && data[ge_i] == key;
        if(found)
        {
            if(!is_leaf())
                return subset[ge_i+1]->lower_bound(key);
            else
                return Iterator(this, ge_i);        
        }
        if(!is_leaf())
            return subset[ge_i]->lower_bound(key);
        else
        {
            if(ge_i < data_count)
                return Iterator(this, ge_i);
            else
                return Iterator(next, 0);
        }
    }                                     // key entry or next if does not
                                         // exist: >= entry
    Iterator upper_bound(const T& key)  //return first that goes AFTER key
    {
        Iterator it = lower_bound(key);
        if(it.is_null() || *it > key)
            return it;
        else
            return ++it;
    }                                    //exist or not, the next entry  >entry

    void get_size(int &size) const                    //count the number of elements
    {
        if(!is_leaf())
        {
            //print remaining subtrees
            for(int i = 0; i < data_count; i++)
            {
                subset[i]->get_size(size);
                size++;
            }
            //print last subtree
            subset[data_count]->get_size(size);
        }
        else
        {
            for(int i = 0; i < data_count; i++)
                size++;
        }
    }
    int size() const                    //count the number of elementsd
    {
        return data_count;
    }
    int elements_count() const
    {
        int num_elements = 0;
        get_size(num_elements);
        return num_elements;
    }
    bool empty() const                  //true if the tree is empty
    {
        return !size();
    }
    void print_tree(int level = 0,
                    ostream &outs=cout) const
    {
        const bool debug = true;
        if(is_leaf())
        {
            // if(next)
            //     outs <<setw(4 * level)<<"          "<<"^\n";
            // else
            //     outs <<setw(4 * level)<<"         "<<"|||\n";
            for(int i = data_count - 1; i > -1; i--)
                outs <<setw(4 * level)<<""<<"["<<data[i]<<"]\n";
            if(debug)
                outs <<setw(4 * level)<<""<<"("<<data_count<<": "<<child_count<<")\n";
        }
        else
        {
            //print remaining subtrees
            //print last subtree
            subset[data_count]->print_tree(level+1);
            for(int i = data_count - 1; i > -1; i--)
            {
                outs<<setw(4 * level)<<""<<"["<<data[i]<<"]\n";
                subset[i]->print_tree(level+1);
            }
            if(debug)
                outs <<setw(4 * level)<<""<<"("<<data_count<<": "<<child_count<<")\n";
        }
    }
    friend ostream& operator<<(ostream& outs,
                               const BPlusTree<T>& print_me){
        print_me.print_tree(0, outs);
        return outs;
    }

    bool is_valid()
    {
        //should check that every data[i] < data[i+1]
    // if (is_leaf())
    //     return true;
    

    //data[data_count-1] must be less than equal to 
    //  every subset[child_count-1]->data[ ]
    
    //every data[i] is greater than every subset[i]->data[ ]
    
    //B+Tree: Every data[i] is equal to subset[i+1]->smallest
    
    //Recursively validate every subset[i]

        return true;
    }
    string in_order()
    {
        string in_order_str = "";
        create_in_order_str(in_order_str);
        return in_order_str;
    }   
    string pre_order()
    {
        string pre_order_str = "";
        create_pre_order_str(pre_order_str);
        return pre_order_str;
    }
    string post_order()
    {
        string post_order_str = "";
        create_post_order_str(post_order_str);
        return post_order_str;
    }

    Iterator begin()
    {
        return Iterator(get_smallest_node());
    }
    Iterator end()
    {
        return Iterator();
    }
    ostream& list_keys(Iterator from = NULL, Iterator to = NULL){
        if (from == NULL) from = begin();
        if (to == NULL) to = end();
        for (Iterator it = from; it != to; it++)
            cout<<*it<<" ";
        return cout;
    }
    //Zac Functions to Test
    Iterator find_leftmost_leaf(int i=0)
    {
        if(is_leaf())
            return Iterator(this, 0);
        else
            return subset[0]->find_leftmost_leaf(0);
    }

private:
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2 * MINIMUM;

    bool dups_ok;                        //true if duplicate keys are allowed
    int data_count;                      //number of data elements
    T data[MAXIMUM + 1];                 //holds the keys
    int child_count;                     //number of children
    BPlusTree* subset[MAXIMUM + 2];      //subtrees
    BPlusTree* next;
    bool is_leaf() const
    {return child_count==0;}             //true if this is a leaf node

    T* find_ptr(const T& entry)         //return a pointer to this key.
    {
        return &entry;
    }                                     // NULL if not there.

    //insert element functions
    void loose_insert(const T& entry)   //allows MAXIMUM+1 data elements in
    {
        int ge_i = first_ge(data, data_count, entry);
        bool found = ge_i < data_count 
                    && data[ge_i] == entry;
        if(found)
        {
            if(!is_leaf())
            {
                //subset[i+1] //i+1 must less than or equal data_count
                subset[ge_i+1]->loose_insert(entry);
                fix_excess(ge_i+1);
            }
            else
            {
                if(dups_ok)
                {
                    data[ge_i] =  data[ge_i] + entry;
                }
                else
                    data[ge_i] = entry;
                
            }
        }
        else
        {
            if(!is_leaf())
            {
                // cout<<"ge_i before returning from leaf: "<<ge_i<<"\n";
                subset[ge_i]->loose_insert(entry);
                // cout<<"ge_i after returning from leaf: "<<ge_i<<"\n";
                fix_excess(ge_i);
            }
            else
            {
                if(ge_i < data_count)
                    insert_item(data, ge_i, data_count, entry);
                else
                {
                    assert(ge_i == data_count && "ge_i cannot be greater than data_count");
                    //pass data count in to update
                    attach_item(data, data_count, entry);
                }
            }
        }
    }                                     //   the root
    void fix_excess(int i)              //fix excess in child i
    {
        if(subset[i]->data_count > MAXIMUM)
        {
            //Make a new subset
            BPlusTree* new_BPT = new BPlusTree<T>();
            //If need to insert the new tree at the very end of subset
            //use attach item
            // cout<<"Before new tree i: "<<i<<"\n";
            int new_BPT_index = i + 1;
            if(new_BPT_index == child_count)
                attach_item(subset, child_count, new_BPT);//Pass in child count to update
            else
                insert_item(subset, new_BPT_index, child_count, new_BPT);
            // cout<<"Before split i: "<<i<<"\n";
            //i is static
            //Split children, use the new subset
            split(subset[i]->data, subset[i]->data_count, new_BPT->data, new_BPT->data_count);
            split(subset[i]->subset, subset[i]->child_count, new_BPT->subset, new_BPT->child_count); //child count updated here
            //Manage yourself(Parent)
            T mid_node_data;
            //v Go fetch mid_node_data
            detach_item(subset[i]->data, subset[i]->data_count, mid_node_data);
            int ge_i_of_mid = first_ge(data, data_count, mid_node_data);
            if(ge_i_of_mid < data_count)
            {
                insert_item(data, ge_i_of_mid, data_count, mid_node_data);
                //BPlus tree inserting mid_node_data at new Tree's left most node
            }
            else
            {
                attach_item(data, data_count, mid_node_data);
            }
            if(subset[i]->is_leaf())
            {
                insert_item(new_BPT->data, 0, new_BPT->data_count, mid_node_data);
                //^the newly splilt data array ofTree will always have > 0 data_count
                new_BPT->next = subset[i]->next;
                subset[i]->next = new_BPT;
            }
        }
    }
    //remove element functions:
    void loose_remove(const T& entry)  //allows MINIMUM-1 data elements
    {
        int ge_i = first_ge(data, data_count, entry);
        bool found = ge_i < data_count 
                    && data[ge_i] == entry;
        if(found)
        {
            if(is_leaf())
            {
                delete_item(data, ge_i, data_count, entry);
            }
            else
            {
                //BPlus Tree Specific Mechanisms here for removing inner nodes
                assert(ge_i < child_count - 1 && "the index greater than equal mapped must be less than child_count - 1\n");
                subset[ge_i + 1]->loose_remove(entry);
                BPlusTree* fixed_subset = fix_shortage(ge_i + 1);

                //v Replacing the sister inner node of the removed element appropriately
                // T smallest;
                // subset[ge_i + 1]->get_smallest(smallest);

                if(fixed_subset)
                {
                    int i_of_data_ge_i_after_fix;
                    if(is_present(data, data_count, entry, i_of_data_ge_i_after_fix))
                    {
                        assert(i_of_data_ge_i_after_fix != -1 && "If not present in data arr should not be here\n");
                        // cout<<"hello from fixed_subset\n";
                        T smallest;
                        subset[ge_i + 1]->get_smallest(smallest);
                        //if present data[ge_i] should not change positions?(by me) // if it did use i_of_data_...
                        data[ge_i] = smallest;
                        //^ This if clause works
                    }
                    else
                    {
                        //Go into subset[ge_i]
                        //Look for old_data_ge_i in subset[ge_i]->data
                        //Call first ge on new data arr
                        int new_ge_i = first_ge(data, data_count, entry);
                        if(!subset[new_ge_i]->is_leaf())
                        {
                            is_present(subset[new_ge_i]->data, subset[new_ge_i]->data_count, entry, i_of_data_ge_i_after_fix); //<- can assert here becuase should be present
                            //Look for the smallest element in subset[ge_i]->subset[i_of_data_ge_i_after_fix + 1]
                            T smallest;
                            //Respect nullptrs

                            subset[new_ge_i]->subset[i_of_data_ge_i_after_fix + 1]->get_smallest(smallest);
                            subset[new_ge_i]->data[i_of_data_ge_i_after_fix] = smallest;

                            // else //Have no leaf to get smallest from and don't need to do anything
                            //Replace the sister node in subset[ge_i]->data with smallest
                        }
                    }
                }
                else
                {
                    T smallest;
                    subset[ge_i + 1]->get_smallest(smallest);
                    //Replace data[i] with smallest element in subset[i + 1] // i = ge_i
                    data[ge_i] = smallest;
                }
                //if(data[i] is there replace it with smallest if not no need to do anything
                //if shortage and if not shortage

                //Because merge left as priority will not need to search for entry in subset[i] the entry will not exist if fix_shortage merged
            }
        }
        else
        {
            if(!is_leaf())
            {
                subset[ge_i]->loose_remove(entry);
                fix_shortage(ge_i);
            }
        }
    }                                    //  in the root

    BPlusTree<T>* fix_shortage(int i)  //fix shortage in child i
    {
        if(subset[i]->data_count < MINIMUM)
        {
            //If checking for case where i is the last node
            if(i + 1 < child_count)
            {
                //If you(i) + 1 is not less than child_count, you are not allowed to check
                //subset[i + 1]
                if(subset[i + 1]->data_count > MINIMUM)
                {
                    //Borrow from right
                    //Leaf case
                    //Not leaf case
                    //First step: switching nodes
                    //My rotate_left does all mechanisms of borrow from right
                    transfer_left(i);
                }
                else if(i - 1 >= MINIMUM -1)
                {
                    if(subset[i - 1]->data_count > MINIMUM)
                    {
                        //Borrow from left
                        transfer_right(i);
                    }
                    else
                    {
                        //subset[i - 1]->data_count <= MINIIMUM
                        //merge with left
                        merge_with_next_subset(i - 1);
                    }
                }
                else
                {
                    //Tried borrowing left and right and tried merging with left so only options is to merge with right
                    //left most subtree
                    merge_with_next_subset(i);
                }
            }
            else
            {
                //rightmost subtree
                if(i - 1 > -1)
                {
                    if(subset[i - 1]->data_count > MINIMUM)
                    {
                        transfer_right(i);
                    }
                    else
                    {
                        merge_with_next_subset(i - 1);
                    }
                }
            }
            return subset[i];
        }
        else
            return nullptr;
    }
    // and return the smallest key in this subtree
    BPlusTree<T>* get_smallest_node()
    {
        if(is_leaf())
            return this;
        else
            return subset[0]->get_smallest_node();
    }
    void get_smallest(T& entry)      //entry := leftmost leaf //use to replace data[i] after searching for where data[i] is
    {
        if(is_leaf())
            entry = data[0];
        else
            subset[0]->get_smallest(entry);
    }
    void get_biggest(T& entry)       //entry := rightmost leaf
    {

    }
    void remove_biggest(T& entry)    //remove the biggest child of tree->entry
    {

    }
    void transfer_left(int i)        //transfer one element LEFT from child i
    {
        rotate_left(i);
    }
    void transfer_right(int i)       //transfer one element RIGHT from child i
    {
        rotate_right(i);
    }
    //Helpers to transfer
    void rotate_right(int i)                  //transfer one element RIGHT
    {
        //v Using BTree rotate right mechanisms for non leaves 
        if(!subset[i]->is_leaf())
        {
            T temp = data[i - 1];
            BPlusTree<T>* temp_ptr = subset[i - 1]->subset[subset[i - 1]->child_count - 1];
            //v Replacing data[i - 1] with left child's biggest element in data arr
            data[i - 1] = subset[i - 1]->data[subset[i - 1]->data_count - 1];
            //v Kind of extra could have gotten away with just deleting the last element in subset[i - 1]
            subset[i - 1]->data[subset[i - 1]->data_count - 1] = temp;
            //if the thin child has a node you insert but
            //else you append
            //v Pulling down from data[i - 1] to rotate right
            if(subset[i]->data_count > 0)
                insert_item(subset[i]->data, 0, subset[i]->data_count ,temp);
            else
                attach_item(subset[i]->data, subset[i]->data_count, temp);
            delete_item(subset[i - 1]->data, subset[i - 1]->data_count - 1, subset[i - 1]->data_count, temp);
            //v Managing children (pointers)
            insert_item(subset[i]->subset, 0, subset[i]->child_count, temp_ptr);
            detach_item(subset[i - 1]->subset, subset[i - 1]->child_count, temp_ptr);
            //^bug, not managing pointers (resolved)
        }
        else //v Using BPlusTree specific rotate right mechanisms for leaves 
        {
            //Better have access to subset[i - 1] to borrow from left!
            //v Will hold onto last element of subset[i - 1] data arr
            T temp;
            //v Removing the last element of subset[i - 1] data arr and giving temp the last element
            detach_item(subset[i - 1]->data, subset[i - 1]->data_count, temp);
            //v Attaching temp as the first element of subset[i]->data arr
            if(subset[i]->data_count > 0)
                insert_item(subset[i]->data, 0, subset[i]->data_count ,temp);
            else
                attach_item(subset[i]->data, subset[i]->data_count, temp);
            //v Replacing data[i - 1] with temp
            data[i - 1] = temp;
            // cout<<"temp: "<<temp<<"\n";
        }
    }
    void rotate_left(int i)                    //transfer one element LEFT
    {
        //v Using BTree rotate left mechanisms for non leaves 
        if(!subset[i]->is_leaf())
        {
            //Holding onto data[i]
            T temp = data[i];
            BPlusTree<T>* temp_ptr = subset[i + 1]->subset[0];
            //Replacing data[i] with the first element in subset[i + 1]
            data[i] = subset[i + 1]->data[0];
            //v Kind of extra does not matter what subset[i+1]->data[0] is because will be deleted later
            subset[i + 1]->data[0] = temp;
            attach_item(subset[i]->data, subset[i]->data_count, temp);
            delete_item(subset[i + 1]->data, 0, subset[i + 1]->data_count, temp);
            //pointer
            attach_item(subset[i]->subset, subset[i]->child_count, temp_ptr);
            delete_item(subset[i + 1]->subset, 0, subset[i + 1]->child_count, temp_ptr);
        }
        else //v Using BPlusTree specific rotate left mechanisms for leaves 
        {
            T temp = data[i];
            //temp will have the item subset[i + 1]->data[0] which is the same as data[0]
            delete_item(subset[i + 1]->data, 0, subset[i + 1]->data_count, temp);
            attach_item(subset[i]->data, subset[i]->data_count, temp);
            //Replacing data[i] with the left most element in subset[i + 1] data arr
            data[i] = subset[i + 1]->data[0];
        }
        //am not caring abt pointer beyond size
    } 
    BPlusTree<T> *merge_with_next_subset(int i) //merge subset i with  i+1
    {
        if(!subset[i]->is_leaf()) //v Using BTree Mechanisms to merge inner nodes
        {
            //Bringin down data[i] to subset[i]
            attach_item(subset[i]->data, subset[i]->data_count, data[i]);
            //Deleting the element to merge in data arr
            if(i < data_count - 1)
            {
                //ge_i should be less than data_count or//delete_item
                T deleted_data;
                delete_item(data, i, data_count, deleted_data);
            }
            else
            {
                T stub;
                //should have called detach item from delete_item
                detach_item(data, data_count, stub);
            }
            //if I don't have access to subset[i + 1] I should not be here
            merge(subset[i]->data, subset[i]->data_count, subset[i + 1]->data, subset[i + 1]->data_count);
            // if(!(subset[i]->is_leaf()))
            // {
                merge(subset[i]->subset, subset[i]->child_count, subset[i + 1]->subset, subset[i + 1]->child_count);
                // cout <<"subset["<<data[i]<<"]"<<"is not a leaf\n";
                // cout<<"subset[i+1]->data[i]: "<<subset[i+1]->data[i]<<"\n";
                // cout<<"subset[i+1]->child_count: "<<subset[i+1]->child_count<<"\n";
                // cout <<"subset["<<data[i]<<"]"<<"is not a leaf\n";
                // // cout<<"subset[i]->data[i]: "<<subset[i+1]->data[i]<<"\n";
                // cout<<"subset[i]->child_count: "<<subset[i]->child_count<<"\n";
                // cout<<"subset[i]->subset[1]->data[0] "<<subset[i]->subset[1]->data[0]<<"\n";
                // cout<<"subset[i]->subset[2]->data[0] "<<subset[i]->subset[2]->data[0]<<"\n";
            // }
            // merge(subset[i]->subset, subset[i]->child_count, subset[i + 1]->subset, subset[i + 1]->child_count);
            // cout<<"subset[i+1]->data[i]: "<<subset[i+1]->data[i]<<"\n";
            // cout<<"subset[i+1]->child_count: "<<subset[i+1]->child_count<<"\n";
            //if root is in shortage, you are at subset which will the next root

            BPlusTree* right_child_temp_ptr = subset[i + 1];

            // if(right_child_temp_ptr->is_leaf() && !right_child_temp_ptr->subset[0]->is_leaf())
                right_child_temp_ptr->child_count = 0;

            if(i + 1 < child_count - 1)
            {
                BPlusTree* deleted_tree;
                delete_item(subset, i + 1, child_count, deleted_tree);
            }
            else
            {
                //should be less than data_count - 1//delete_item
                BPlusTree* stub;
                detach_item(subset, child_count, stub);
            }
            //its releasing the extra pointer in subset
            delete right_child_temp_ptr;
            right_child_temp_ptr = nullptr;
        }
        else
        {
            if(i < data_count - 1)
            {
                //ge_i should be less than data_count or//delete_item
                T deleted_data;
                delete_item(data, i, data_count, deleted_data);
            }
            else
            {
                T stub;
                //should have called detach item from delete_item
                detach_item(data, data_count, stub);
            }
            //if I don't have access to subset[i + 1] I should not be here
            merge(subset[i]->data, subset[i]->data_count, subset[i + 1]->data, subset[i + 1]->data_count);
            merge(subset[i]->subset, subset[i]->child_count, subset[i + 1]->subset, subset[i + 1]->child_count);
                
            BPlusTree* right_child_temp_ptr = subset[i + 1];

            // if(right_child_temp_ptr->is_leaf() && !right_child_temp_ptr->subset[0]->is_leaf())
            right_child_temp_ptr->child_count = 0;

            if(i + 1 < child_count - 1)
            {
                BPlusTree* deleted_tree;
                delete_item(subset, i + 1, child_count, deleted_tree);
            }
            else
            {
                //should be less than data_count - 1//delete_item
                BPlusTree* stub;
                detach_item(subset, child_count, stub);
            }
            //v Fixing next's links before deleting
            subset[i]->next = right_child_temp_ptr->next;
            right_child_temp_ptr->next = nullptr;
            //its releasing the extra pointer in subset
            delete right_child_temp_ptr;
            right_child_temp_ptr = nullptr;
        }
        return subset[i];
    }
    void create_in_order_str(string &in_order_str)
    {
        
        if(!is_leaf())
        {
            //print remaining subtrees
            for(int i = 0; i < data_count; i++)
            {
                subset[i]->create_in_order_str(in_order_str);
                in_order_str += to_string(data[i]) + '|';
            }
            //print last subtree
            subset[data_count]->create_in_order_str(in_order_str);
        }
        else
        {
            for(int i = 0; i < data_count; i++)
                in_order_str += to_string(data[i]) + '|';
        }
    }
    void create_pre_order_str(string &pre_order_str)
    {
        
        if(!is_leaf())
        {
            //print remaining subtrees
            for(int i = 0; i < data_count; i++)
            {
                pre_order_str += to_string(data[i]) + '|';
                subset[i]->create_pre_order_str(pre_order_str);
            }
            //print last subtree
            subset[data_count]->create_pre_order_str(pre_order_str);
        }
        else
        {
            for(int i = 0; i < data_count; i++)
                pre_order_str += to_string(data[i]) + '|';
        }
    }
    void create_post_order_str(string &post_order_str)
    {
        //there has to be a better way
        if(!is_leaf())
        {
            //print remaining subtrees
            for(int i = 0; i < data_count; i++)
            {
                subset[i]->create_post_order_str(post_order_str);
                if(i > 0)
                    post_order_str += to_string(data[i-1]) + '|';
            }
            subset[data_count]->create_post_order_str(post_order_str);
            post_order_str += to_string(data[data_count - 1]) + '|';

        }
        else
        {
            for(int i = 0; i < data_count; i++)
                post_order_str += to_string(data[i]) + '|';
        }
    }
};


#endif // ZAC_BPLUSTREE_