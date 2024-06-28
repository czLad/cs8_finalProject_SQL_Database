#ifndef MY_QUEUE_H
#define MY_QUEUE_H

#include "../../includes/linked_list_functions/linkedlistfunctionsTemplated.h"
#include "../../includes/token/token.h"


template <typename T>
class Queue
{
public:
    class Iterator{
    public:
        friend class Queue;          //give access to list to access _ptr
        Iterator(){_ptr = NULL;}                //default ctor
        Iterator(node<T>* p){_ptr =p;}          //Point Iterator to where
                                                //  p is pointing to
        operator bool() //used as boolean e.g if(Iterator)
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
        } //could bug
        friend Iterator operator++(Iterator& it,
                                   int unused)
        {
            it._ptr = it._ptr->_next;
            return Iterator(it._ptr);
            //^ is wrong?
                //hold on to current value, inc, then return the value you were holding.
        }         //friend operator: it++
    private:
        node<T>* _ptr;                          //pointer being encapsulated
    };

    Queue();
    Queue(const Queue<T>& copyMe);
    ~Queue();
    Queue& operator=(const Queue<T>& RHS);

    bool empty();
    T front();
    T back();

    void push(T item);
    T pop();
    //Latest function added 5/23/24
    void clear();

    Iterator begin() const;            //Iterator to the head node
    Iterator end() const;              //Iterator to NULL
    void print_pointers();
    int size() const { return _size; }
    template<typename TT>
    friend ostream& operator << (ostream& outs, const Queue<TT>& printMe);
private:
    node<T>* _front;
    node<T>* _rear;
    int _size;
};

//Definitons
//Big Three
template <typename T>
Queue<T>::Queue()
{
    const bool debug = false;
    if(debug)
    {
        cout << "Queue CTOR() Fired.\n";
    }
    _front = nullptr;
    _rear = nullptr;
    _size = 0;
}
template <typename T>
Queue<T>::Queue(const Queue<T>& copyMe)
{
    const bool debug = false;
    if(debug)
    {
        cout << "Queue Copy CTOR() Fired.\n";
    }
    if(copyMe._front == nullptr)
    {
        assert(copyMe._rear == nullptr && "Rear should be nullptr when front is nullptr.");
        _front = nullptr;
        _rear = nullptr;
        _size = 0;
    }
    else
    {
        _front = nullptr;
        _rear = _copy_list(_front, copyMe._front);
        _size = copyMe._size;
    }
}
template <typename T>
Queue<T>::~Queue()
{
    const bool debug = false;
    if(debug)
    {
        cout << "Queue DTOR() Fired.\n";
    }
    _clear_list(_front);
    _front = nullptr;
    _rear = nullptr;
    _size = 0;
}
template <typename T>
Queue<T>& Queue<T>::
operator=(const Queue<T>& RHS)
{
    const bool debug = false;
    if(debug)
    {
        cout << "Queue Assignment Operator Fired.\n";
    }
    if(this == &RHS)
        return *this;
    else
    {
        _clear_list(_front);
        _front = nullptr;
        _rear = nullptr;
        _rear = _copy_list(_front, RHS._front);
        _size = RHS._size;
        return *this;
    }
}
template <typename T>
bool Queue<T>::empty()
{
    if(_front == nullptr)
    {
        assert(_rear == nullptr && "Rear should be nullptr when front is nullptr.");
        return true;
    }
    else
        return false;
}
template <typename T>
T Queue<T>::front()
{
    return _front->_item;
}
template <typename T>
T Queue<T>::back()
{
    return _rear->_item;
}
template <typename T>
void Queue<T>::push(T item)
{
    if(_front == nullptr)
    {
        assert(_rear == nullptr && "Rear should be nullptr when front is nullptr.");
        _size++;
        _front = _insert_head(_rear, item);
    }
    else
    {
        _size++;
        _rear = _insert_after(_rear, _rear, item);
    }
}
template <typename T>
T Queue<T>::pop()
{
    assert(_front != nullptr && "Cannot pop an Empty Stack.");
    _size--;
    if(_front == _rear)
    {
        _rear = nullptr;
        return _remove_head(_front);
    }
    else
    {
        return _remove_head(_front);
    }
}
template <typename T>
void Queue<T>::clear()
{
    _clear_list(_front);
    _front = nullptr;
    _rear = nullptr;
    _size = 0;
}
template <typename T>
typename Queue<T>::Iterator Queue<T>::begin() const            //Iterator to the head node
{
    return Iterator(_front);
}
template <typename T>
typename Queue<T>::Iterator Queue<T>::end() const              //Iterator to NULL
{
    return nullptr;
}
template <typename T>
void Queue<T>::print_pointers()
{
    _print_list(_front);
}
template<typename TT>
ostream& operator << (ostream& outs, const Queue<TT>& printMe)
{
    outs << "Queue:Head->";
    for(node<TT>* walker = printMe._front; walker != nullptr; walker = walker->_next)
    {
        outs << *walker;
    }
    outs << "|||\n";
    return outs;
}

#endif