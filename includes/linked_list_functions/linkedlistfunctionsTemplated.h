#ifndef LIST_H
#define LIST_H

#include "../node/node.h"

using namespace std;

//Linked List General Functions:
//Doubly Linked List
template <typename T>
void _print_list(node<T>* head)
{
    cout << "Head->";
    for(node<T>* walker = head; walker != nullptr; walker = walker->_next)
    {
        cout << *walker;
    }
    cout << "|||\n";
}

//recursive fun! :)
template <typename T>
void _print_list_backwards(node<T> *head)
{

}

//return ptr to key or NULL
template <typename T>
node<T>* _search_list(node<T>* head,
                            T key)
{
    for(node<T>* walker = head; walker != nullptr; walker = walker->_next)
    {
        if(walker->_item == key)
        {
            return walker;
        }
    }
    return nullptr;
}


template <typename T>
node<T>* _insert_head(node<T> *&head,
                            T insert_this)
{
    node<T>* insert_node_ptr = new node<T>(insert_this);
    //insert_node_ptr's _previous is defaulted to NULL
    //check condition because for doubly linked list an adding head to an empty list behaves differently
    //from a filled one
    if(head == nullptr)
    {
        insert_node_ptr->_next = head;
        head = insert_node_ptr;
    }
    else
    {
        insert_node_ptr->_next = head;
        head->_prev = insert_node_ptr;
        head = insert_node_ptr;
    }
    return head;
}
template <typename T>
T _remove_head(node<T>* &head)
{
    assert(head != nullptr && "Linked list cannot be empty.");
    T removed_item = head->_item;
    //Needs remove_ptr to delete
    node<T>* remove_ptr = head;
    head = remove_ptr->_next;
    delete remove_ptr;
    remove_ptr = nullptr;
    //Without condition, removing only one head would cause a crash
    if(head != nullptr)
        head->_prev = nullptr;
    return removed_item;
}

//insert after ptr: insert head if marker null
template <typename T>
node<T>* _insert_after(node<T>*& head,
                                node<T> *after_this,
                                T insert_this)
{
    if(head == nullptr)
        return _insert_head(head, insert_this);
    else
    {
        if(after_this == nullptr)
            return _insert_head(head, insert_this);
        node<T>* insert_node_ptr = new node<T>(insert_this);
        insert_node_ptr->_next = after_this->_next;
        insert_node_ptr->_prev = after_this;
        after_this->_next = insert_node_ptr;
        //insert_node_ptr holds old after_this's next node
        //next node's previous node points to after_this but 
        //next node's previous node should be insert_node_ptr
        // Without the condition, inserting to the end of a list would crash
        if(insert_node_ptr->_next != nullptr)
            insert_node_ptr->_next->_prev = insert_node_ptr;
        return insert_node_ptr;
    }
}
//insert before ptr: insert head if marker null
template <typename T>
node<T>* _insert_before(node<T>*& head,
                                node<T>* before_this,
                                T insert_this)
{
    //Check head == nullptr to not waster processing time
    if(head == nullptr)
    {
        return _insert_head(head, insert_this);
    }
    else
    {
        node<T>* previous_node = before_this->_prev;
        return _insert_after(head, previous_node, insert_this);
    }
}
//ptr to previous node
template <typename T>
node<T>* _previous_node(node<T>* prev_to_this)
{
    assert(prev_to_this != nullptr && "prev_to_this cannot be nullptr");
    return prev_to_this->_prev;
}

//delete, return item
//delete_this cannot be nullptr
template <typename T>
T _delete_node(node<T>*& head, node<T>* delete_this)
{
    if(head == delete_this)
        return _remove_head(head);
    else
    {
        T deleted_item = delete_this->_item;
        node<T>* previous_node = delete_this->_prev;
        node<T>* next_node = delete_this->_next;
        if(next_node == nullptr)
            previous_node->_next = delete_this->_next;
        else
        {
            previous_node->_next = delete_this->_next;
            next_node->_prev = previous_node;
        }
        delete delete_this;
        delete_this = nullptr;
        return deleted_item;
    }
    
}

//duplicate the list...
template <typename T>
node<T>* _copy_list(node<T>* head)
{
    node<T>* copy_head = nullptr;
    return_copy_list(copy_head, head);
}

//duplicate the list, return pointer to last node in dest... 
//     use this function in your queue big three 
template <typename T>
node<T> *_copy_list(node<T> *&dest, node<T> *src)
{
    _clear_list(dest);
    if(src == nullptr)
        return nullptr;
    node<T>* src_walker = src;
    dest = new node<T>(src_walker->_item);
    node<T>* dest_walker = dest;
    //^ without this dest_walker -> next will be null after first copy and crash
    for(src_walker = src->_next; src_walker != nullptr; src_walker = src_walker->_next)
    {
        dest_walker->_next = new node<T>(src_walker->_item);
        node<T>* previous_node = dest_walker;
        dest_walker = dest_walker->_next;
        dest_walker->_prev = previous_node;
    }

    return dest_walker;
}


//delete all the nodes
template <typename T>
void _clear_list(node<T>*& head)
{
    while(head != nullptr)
        _remove_head(head);
}

//_item at this position
template <typename T>
T& _at(node<T>* head, int pos)
{
    int size = 0;
    for(node<T>* walker = head; walker != nullptr; walker = walker->_next)
    {
        if(size == pos)
            return walker->_item;
        size++;
    }
    assert(pos < size && "Index must be less than size");
}

#endif