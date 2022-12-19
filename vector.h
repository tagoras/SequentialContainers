#ifndef VECTOR_H
#define VECTOR_H

#include <memory>
#include <stdexcept>

namespace custom{

template<typename T>
class Vector{
    private:
        T* start{nullptr};
        T* first_unfilled{nullptr};
        int capacity{};
    public:
        // Copy control members
        Vector() = default;
        Vector(std::initializer_list<T>);

        Vector(const Vector<T> &);
        operator=(std::initializer_list<T>);
        operator=(const Vector<T> &);

        ~Vector();

        Vector(const Vector<T> &&);
        operator=(const Vector<T> &&);

        //Iterator retrieval
        /*
            Implement functions that give back iterators:
            begin()
            end()
            cbegin()
            cend()
        */

        //Access Functions
        T& at(int) const;
        T& operator[](int) const;
        T& front() const;
        T& back() const;

        // Capacity Functions
        bool empty() const;
        int size() const;
        int max_size() const;
        void reserve();
        void shrink_to_fit();

        //Modifiers
        void clear();

        void insert(const T &);
        void insert(std::initializer_list<T> elements);
        
        T erase(const T &);
        T erase(int pos);
        T erase(int start, int end);

        void push_back(const T &);
        void push_back(std::initializer_list<T> elements);
        void push_back(const Vector<T> &other);

        T pop_back();
        void resize();

        void swap(const Vector<T> other);
};

template <typename T>
Vector<T>::Vector(std::initializer_list<T> elements){
    capacity = elements.size() * 2;
    start = new T[capacity];

    T* copy_start = start;
    for(T element : elements)
    {
        *copy_start = element;
        ++copy_start;
    }
    first_unfilled = copy_start + 1;
}

template <typename T>
Vector<T>::Vector(const Vector<T> &original): capacity{original.capacity}
{
    start = new T[capacity];
    T* start_copy = start;

    for(int i = 0; i < original.size(); i++)
    {
        start_copy[i] = original.at(i);
    }

    first_unfilled += original.size();
}

template <typename T>
T& Vector<T>::at(int index) const {
    if(index >= size()) throw std::out_of_range("Tried to access element out of range");
    return *(start + index);
}

template <typename T>
T& Vector<T>::operator[](int index) const {
    return *(start + index);
}

template <typename T>
int Vector<T>::size() const {
    return first_unfilled - start;
}

}

#endif