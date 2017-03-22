#ifndef STACK_INCLUDED
#define STACK_INCLUDED

#include"stack_ptr.h"

#define unshareable 0

using namespace std;

template<typename T>
class TwoSideStack
{
private:
    StackPtr<T> *_stackPtr;

public:
    TwoSideStack(size_t = 10);

    TwoSideStack(TwoSideStack &&);
    TwoSideStack & operator=(TwoSideStack &&);

    TwoSideStack(const TwoSideStack &);
    TwoSideStack & operator=(const TwoSideStack &);

    friend std::ostream & operator<<(std::ostream & os, const TwoSideStack<T> & rhs)
    {
        os << *rhs._stackPtr;
        return os;
    }

    friend void swap(TwoSideStack<T> & f_s, TwoSideStack<T> & s_s) throw() {
        swap(f_s._stackPtr, s_s._stackPtr);
    }

    ~TwoSideStack();

    bool push_top(const T);
    bool push_bottom(const T);

    void pop_top();
    void pop_bottom();

    bool get_top(T & );
    bool get_bottom(T & );

    void printStack();

    void reallocation();

    void deep_copy();

    class iterator{
    protected:
        T * _it_data_ptr;
        T * _begin;
        T * _end;
        T * _top;
        T * _bottom;
    public:
        iterator(T * data, T * begin, T * end, T * top, T * bottom){
            _it_data_ptr = data;
            _begin = begin;
            _end = end;
            _top = top;
            _bottom = bottom;
        }

        iterator& operator++(){
            if(_it_data_ptr == _bottom - 1)
                throw;

            if(_it_data_ptr == _begin)
                _it_data_ptr = _end;
            else
                --_it_data_ptr;

            return *this;
        }

        iterator& operator--(){
            if(_it_data_ptr == _top + 1)
                throw;

            if(_it_data_ptr == _end)
                _it_data_ptr = _begin;
            else
                ++_it_data_ptr;

            return *this;
        }

        iterator operator++(int){
            if(_it_data_ptr == _bottom - 1)
                throw;

            T * t_m =  _it_data_ptr;
            if(_it_data_ptr == _begin)
                _it_data_ptr = _end;
            else
                --_it_data_ptr;

            return iterator(t_m, _begin, _end, _top, _bottom);
        }

        iterator operator--(int){
            if(_it_data_ptr == _top + 1)
                throw;

            T * t_m =  _it_data_ptr;
            if(_it_data_ptr == _end)
                _it_data_ptr = _begin;
            else
                ++_it_data_ptr;

            return iterator(t_m, _begin, _end, _top, _bottom);
        }

        bool operator<(const iterator & it)const{
            if((_it_data_ptr <= _top && it._it_data_ptr <= _top) ||
                (_it_data_ptr >= _bottom && it._it_data_ptr >= _bottom))
                return _it_data_ptr > it._it_data_ptr;

            return _it_data_ptr < it._it_data_ptr;
        }

        bool operator==(const iterator & it) const{
            return _it_data_ptr == it._it_data_ptr;
        }

        bool operator>(const iterator & it)const{
            return !((*this < it) || (*this == it));
        }

        bool operator>=(const iterator & it)const{
            if(_it_data_ptr < _bottom && _it_data_ptr > _top)
                return false;
            return !(*this < it);
        }

        bool operator<=(const iterator & it)const{
            if(_it_data_ptr < _bottom && _it_data_ptr > _top)
                return false;

            return (*this < it) || (*this == it);
        }

        bool operator!=(const iterator & it)const{
            return !(*this == it);
        }

        T & operator*()const {
            return *_it_data_ptr;
        }
    };

    class const_iterator: public iterator{
    public:
        const T & operator*()const {
            return *(this->_it_data_ptr);
        }

        const_iterator(T * data, T * begin, T * end, T * top, T * bottom):
            iterator(data, begin, end, top, bottom){}
    };

    iterator top(){
        return iterator(_stackPtr->_data + _stackPtr->get_index_top(),
                        _stackPtr->_data, _stackPtr->_data + _stackPtr->get_size() - 1,
                        _stackPtr->_data + _stackPtr->get_index_top(),
                        _stackPtr->_data + _stackPtr->get_index_bottom());
    }

    iterator bottom(){
        return iterator(_stackPtr->_data + _stackPtr->get_index_bottom(),
                        _stackPtr->_data, _stackPtr->_data + _stackPtr->get_size() - 1,
                        _stackPtr->_data + _stackPtr->get_index_top(),
                        _stackPtr->_data + _stackPtr->get_index_bottom());
    }

    const_iterator top_const(){
        return const_iterator(_stackPtr->_data + _stackPtr->get_index_top(),
                        _stackPtr->_data, _stackPtr->_data + _stackPtr->get_size() - 1,
                        _stackPtr->_data + _stackPtr->get_index_top(),
                        _stackPtr->_data + _stackPtr->get_index_bottom());
    }

    const_iterator bottom_const(){
        return const_iterator(_stackPtr->_data + _stackPtr->get_index_bottom(),
                        _stackPtr->_data, _stackPtr->_data + _stackPtr->get_size() - 1,
                        _stackPtr->_data + _stackPtr->get_index_top(),
                        _stackPtr->_data + _stackPtr->get_index_bottom());
    }
};


template <typename T>
bool TwoSideStack<T>::get_top(T & elem){
    return _stackPtr->get_top(elem);
}

template <typename T>
bool TwoSideStack<T>::get_bottom(T & elem){
    return _stackPtr->get_bottom(elem);
}

template <typename T>
TwoSideStack<T> & TwoSideStack<T>::operator=(TwoSideStack<T> && other){
    this->_stackPtr = other._stackPtr;
}

template <typename T>
TwoSideStack<T>::TwoSideStack(const TwoSideStack<T> & other){
    cout << "constructor copy" << endl;
    this->_stackPtr = other._stackPtr;
    _stackPtr->set_refs(_stackPtr->get_refs() + 1);
}

template <typename T>
TwoSideStack<T> & TwoSideStack<T>::operator=(const TwoSideStack<T> & other){
    _stackPtr->set_refs(_stackPtr->get_refs() + 1);
    this->_stackPtr = other._stackPtr;
}

template <typename T>
void TwoSideStack<T>::deep_copy(){
    if(_stackPtr->get_refs() > 1)
    {
        _stackPtr->set_refs(_stackPtr->get_refs() - 1);

        size_t sz = _stackPtr->get_size();
        int iTop = _stackPtr->get_index_top();
        int iBottom = _stackPtr->get_index_bottom();
        T * arr = new T[sz];

        std::copy(_stackPtr->_data, _stackPtr->_data + sz, arr);

        _stackPtr = new StackPtr<T>(arr, sz, iTop, iBottom);
    }
}

template <typename T>
TwoSideStack<T>::TwoSideStack(size_t s){
    _stackPtr = new StackPtr<T>(s);
}

template <typename T>
TwoSideStack<T>::~TwoSideStack(){
    if(_stackPtr->get_refs() == 1)
        delete _stackPtr;
    else
        _stackPtr->set_refs(_stackPtr->get_refs() - 1);
}

template <typename T>
bool TwoSideStack<T>::push_top(const T value){
    deep_copy();
    return _stackPtr->push_top(value);
}

template <typename T>
bool TwoSideStack<T>::push_bottom(const T value){
    deep_copy();
    return _stackPtr->push_bottom(value);
}

template <typename T>
void TwoSideStack<T>::reallocation(){
    _stackPtr->reallocation();
}

template<typename T>
void TwoSideStack<T>::pop_top(){
    deep_copy();
    _stackPtr->pop_top();
}

template<typename T>
void TwoSideStack<T>::pop_bottom(){
    deep_copy();
    _stackPtr->pop_bottom();
}

template <typename T>
void TwoSideStack<T>::printStack(){
    _stackPtr->printStack();
}

#endif // STACK_INCLUDED
