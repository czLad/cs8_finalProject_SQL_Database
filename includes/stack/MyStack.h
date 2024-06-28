#ifndef MY_STACK_H
#define MY_STACK_H

#include "../../includes/linked_list_functions/linkedlistfunctionsTemplated.h"

using namespace std;

template <typename T>
class Stack{
public:
    class Iterator{
    public:
        friend class Stack;                     //give access to list to access _ptr
        Iterator(){_ptr = NULL;}                //default ctor
        Iterator(node<T>* p){_ptr =p;}          //Point Iterator to where
                                                //  p is pointing to
         operator bool()
        {
            return _ptr;
        }                                //casting operator: true if _ptr not NULL
                                                            //      this turned out to be a pain!
        T& operator *()
        {
            return _ptr->_item;
        }                        //dereference operator
        T* operator ->()                       //member access operator
        {
            T* item_ptr = &_ptr->_item;
            return item_ptr;
        }
        bool is_null()
        {
            return !_ptr;
        }                        //true if _ptr is NULL
        friend bool operator !=(const Iterator& left,
                                const Iterator& right)
        {
            return !(left == right);
        }                                //true if left != right
        friend bool operator ==(const Iterator& left,
                                const Iterator& right)
        {
            return left._ptr == right._ptr;
        }                                //true if left == right
        Iterator& operator++()                         //member operator: ++it; or ++it = new_value
        {
            _ptr = _ptr->_next;
            return *this;
        }
        friend Iterator operator++(Iterator& it,
                                   int unused)
        {
            it._ptr = it._ptr->_next;
            return Iterator(it._ptr);
                //hold on to current value, inc, then return the value you were holding.
        }         //friend operator: it++
    private:
        node<T>* _ptr;                          //pointer being encapsulated
    };

    Stack();
    Stack(const Stack<T>& copyMe);
    ~Stack();
    Stack<T>& operator=(const Stack<T>& RHS);
    T top();
    bool empty();
    void push(T item);
    T pop();
    template<typename TT>
    friend ostream& operator<<(ostream& outs, 
                                        const Stack<TT>& printMe);

    Iterator begin() const;              //Iterator to the head node
    Iterator end() const;                //Iterator to NULL
    int size() const { return _size; }

private:
    node<T>* _top;
    int _size;
};

//Definitions
//Big Three
template <typename T>
Stack<T>::Stack()
{
   const bool debug = false;
    if(debug)
    {
        cout << "Stack CTOR() Fired.\n";
    }
    _top = nullptr;
    _size = 0;
}
template <typename T>
Stack<T>::Stack(const Stack<T>& copyMe)
{
    const bool debug = false;
    if(debug)
    {
        cout << "Stack Copy CTOR() Fired.\n";
    }
    if(copyMe._top == nullptr)
    {
        _top = nullptr;
        _size = 0;
    }
    else
    {
        _top = nullptr;
        _copy_list(_top, copyMe._top);
        _size = copyMe._size;
    }
}
template <typename T>
Stack<T>::~Stack()
{
    const bool debug = false;
    if(debug)
    {
        cout << "Stack DTOR() Fired.\n";
    }
    _clear_list(_top);
    _top = nullptr;
    _size = 0;
}
template <typename T>
Stack<T>& Stack<T>::operator=(const Stack<T>& RHS)
{
    const bool debug = false;
    if(debug)
    {
        cout << "Stack Assignment Operator Fired.\n";
    }
    if(this == &RHS)
        return *this;
    else
    {
        _clear_list(_top);
        _top = nullptr;
        _copy_list(_top, RHS._top);
        _size = RHS._size;
        return *this;
    }
}
//Public Functions
template <typename T>
T Stack<T>::top()
{
    return _top->_item;
}
template <typename T>
bool Stack<T>::empty()
{
    return _top == nullptr;
}
template <typename T>
void Stack<T>::push(T item)
{
    _size++;
    _insert_head(_top, item);
}
template <typename T>
T Stack<T>::pop()
{
    assert(_top != nullptr && "Cannot pop an Empty Stack.");
    _size--;
    return _remove_head(_top);
}
template<typename T>
typename Stack<T>::Iterator Stack<T>::begin() const
{
    return Iterator(_top);
}
template<typename T>
typename Stack<T>::Iterator Stack<T>::end() const
{
    return nullptr;
}  
//Friend functions
template<typename TT>
ostream& operator<<(ostream& outs, const Stack<TT>& printMe)
{
    outs << "Stack:Head->";
    for(node<TT>* walker = printMe._top; walker != nullptr; walker = walker->_next)
    {
        outs << *walker;
    }
    outs << "|||\n";
    return outs;
}


#endif