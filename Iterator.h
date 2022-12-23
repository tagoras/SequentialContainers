#ifndef Iterator_H
#define Iterator_H

#include <iterator>

template <typename T>
class Iterator{
    public:
        using Iterator_category = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
    
    Iterator(T* element);

    virtual T& operator*();
    
    Iterator& operator++();
    Iterator operator++(int);

    /*
        In the following template friend functions the typename is always gonna match T, but simply writing
        template<typename T> will say that T is shadowed. Therefore, declaring U is just work around.
    */
    template<typename U>
    friend bool operator==(const Iterator<U> &lhs, const Iterator<U> &rhs);

    template<typename U>
    friend bool operator!=(const Iterator<U> &lhs, const Iterator<U> &rhs);

    T* getElement() const;

    private:
        T* element{nullptr};
        
};

template <typename T>
Iterator<T>::Iterator(T* element): element{element} {}

template <typename T>
bool operator==(const Iterator<T> &lhs, const Iterator<T> &rhs)
{
    return lhs.element == rhs.element;
}

template<typename T>
bool operator!=(const Iterator<T> &lhs, const Iterator<T> &rhs)
{
    return !(lhs == rhs);
}

template<typename T>
T& Iterator<T>::operator*() {
    return *element;
}

template<typename T>
T* Iterator<T>::getElement() const {
    return *element;
}

template<typename T>
Iterator<T>& Iterator<T>::operator++()
{
    element++;
    return *this;
}

template<typename T>
Iterator<T> Iterator<T>::operator++(int)
{
    element++;
    return *this;
}

/*
Below is a const implementation of Iterator
*/



//There are some issues with the implementation of const iterator
template<typename T>
class Const_Iterator : public Iterator<T>{
    public:
        Const_Iterator(T* pointer): Iterator<T>{pointer} {}
        const T& operator*();
};

// Removing Const_Iterator<T>:: from the return statement causes a compile time error
template<typename T>
const T& Const_Iterator<T>::operator*()
{
    return Const_Iterator<T>::getElement();
}

#endif