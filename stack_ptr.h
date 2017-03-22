#ifndef STACK_PTR_H_INCLUDED
#define STACK_PTR_H_INCLUDED

using namespace std;

template<typename T>
class StackPtr{

private:

    unsigned long _cRefs;
    size_t _size;
    int _indexTop;
    int _indexBottom;

public:

    T * _data;

    StackPtr(size_t = 11);
    StackPtr(T *, size_t, int indexTop, int indexBottom);

    StackPtr(StackPtr &&);
    StackPtr & operator=(StackPtr &&);

    StackPtr(const StackPtr &);
    StackPtr & operator=(const StackPtr &);

    ~StackPtr();

    size_t get_size() const;

    unsigned long  get_refs() const{return _cRefs;}
    void set_refs(unsigned long  rfs){_cRefs = rfs;}

    int get_index_top() const {return _indexTop;}
    int get_index_bottom() const {return _indexBottom;}

    T & operator[](const size_t);

    void reallocation();

    bool push_top(const T&);
    bool push_bottom(const T&);

    void pop_top();
    void pop_bottom();

    bool get_top(T &);
    bool get_bottom(T &);

    void printStack();

    friend void swap(StackPtr<T> & f_s, StackPtr<T> & s_s) throw(){
        swap(f_s._cRefs, s_s._cRefs);
        swap(f_s._size, s_s._size);
        swap(f_s._indexTop, s_s._indexTop);
        swap(f_s._indexBottom, s_s._indexBottom);
    }

    friend std::ostream & operator<<(std::ostream & cout, const StackPtr<T> & rhs){
        if(rhs._indexTop > -1)
            for (int i = rhs._indexTop; i > -1 ; --i)
                cout << " | " << rhs._data[i] << endl;
        else
            cout<<"null\n"<<endl;

        cout<<endl;

        if(rhs._indexBottom < rhs._size)
            for (int i = rhs._indexBottom; i < rhs._size; ++i)
                cout << " | " << rhs._data[i] << endl;
        else
            cout<<"null\n"<<endl;

        cout<< "cRefs " << rhs._cRefs << endl;
        cout<<endl;

        return cout;
        }

};


template <typename T>
bool StackPtr<T>::get_top(T & elem){
    if(_indexTop < 0)
        return false;

    elem = _data[_indexTop];
    return true;
}

template <typename T>
bool StackPtr<T>::get_bottom(T & elem){
    if(_indexBottom >= _size)
        return false;
    elem = _data[_indexBottom];
    return true;
}

template<typename T>
StackPtr<T>::StackPtr(const StackPtr & other):
    _size(other._size),
    _cRefs(other._cRefs),
    _indexTop(other._indexTop),
    _indexBottom(other._indexBottom){
    _data = other._data;
}

template<typename T>
StackPtr<T> & StackPtr<T>::operator=(const StackPtr<T> & other){
    if (this != &other){
        _size = other._size;
        _cRefs = other._cRefs;
        _indexTop = other._indexTop;
        _indexBottom = other._indexBottom;

        _data = other._data;
    }
    return *this;
}

template<typename T>
StackPtr<T> & StackPtr<T>::operator=(StackPtr<T> && other){
    if(_data)
        delete[] _data;

    _data = other._data;

    other._data = nullptr;

    _cRefs = 1;
    _size = other._size;
    _indexTop = other._indexTop;
    _indexBottom = other._indexBottom;

    return *this;
}

template<typename T>
StackPtr<T>::StackPtr(StackPtr<T>&& other){
    if(_data)
        delete[] _data;

    _data = other._data;

    other._data = nullptr;

    _cRefs = 1;
    _size = other._size;
    _indexTop = other._indexTop;
    _indexBottom = other._indexBottom;
}

template <typename T>
bool StackPtr<T>::push_top(const T & value){
    if (_indexTop + 1 < _indexBottom){
        _data[++_indexTop] = value;
        return true;
    }else
    {
        try{
            reallocation();
            _data[++_indexTop] = value;
            return true;
        }
        catch(exception& e){
            cout<<"Error in push_top "<< e.what() << endl;
            return false;
        }
    }
}


template <typename T>
bool StackPtr<T>::push_bottom(const T & value){
    if (_indexBottom - 1 > _indexTop){
        _data[--_indexBottom] = value;
        return true;
    }else
    {
        try{
            reallocation();
            _data[--_indexBottom] = value;
            return true;
        }
        catch(exception& e){
            std::cout<<"Error in push_bottom "<<std::endl;
            return false;
        }
    }
}

template<typename T>
void StackPtr<T>::pop_top(){
    if(_indexTop >= 0){
        T t = _data[_indexTop];
        _data[_indexTop--] = 0;
    }
}

template <typename T>
void StackPtr<T>::printStack(){
    if(_indexTop > -1)
        for (int i = _indexTop; i > -1 ; --i)
            cout << " | " << _data[i] << endl;
    else
        cout<<"null\n"<<endl;

    cout<<endl;

    if(_indexBottom < _size)
        for (int i = _indexBottom; i < _size; ++i)
            cout << " | " << _data[i] << endl;
    else
        cout<<"null\n"<<endl;

    cout<< "cRefs" << _cRefs << endl;
    cout<<endl;

}

template<typename T>
void StackPtr<T>::reallocation(){
    int badSize = _size;
    _size *= 2;


    try{
        T * newStackPtr =  new T[_size];

        for(int i = 0; i < _indexTop + 1; ++i)
            newStackPtr[i] = _data[i];

        for(int i = _indexBottom; i < badSize; ++i)
            newStackPtr[i + badSize] = _data[i];

        delete[] _data;

        _data = newStackPtr;
        _indexBottom += badSize;

    }catch(exception & e){
        cout << e.what() << endl;
    }
}

template<typename T>
StackPtr<T>::StackPtr(size_t sz){
    cout << "constructor Stackptr " << sz << endl;
    if(_data)
        delete[] _data;

    _data = new T[sz];
    _cRefs = 1;
    _indexTop = -1;
    _indexBottom = sz;
    _size = sz;
}

template<typename T>
StackPtr<T>::~StackPtr(){
    cout << "destructor StackPtr "<< _size << endl;
    delete[] _data;
}

template<typename T>
T & StackPtr<T>::operator[](size_t ind){
    try{
        return _data[ind];
    }catch(exception& e){
        cout << "Error in operator[] " << e.what() << endl;
    }
}

template<typename T>
StackPtr<T>::StackPtr(T * arr, size_t sz, int iTop, int iBottom){
    _data = arr;

    _cRefs = 1;
    _indexTop = iTop;
    _indexBottom = iBottom;
    _size = sz;
}

template<typename T>
size_t StackPtr<T>::get_size() const{
    return _size;
}

template<typename T>
void StackPtr<T>::pop_bottom(){
    if(_indexBottom < _size){
        T t = _data[_indexBottom];
        _data[_indexBottom++] = 0;
    }
}
#endif // STACK_PTR_H_INCLUDED
