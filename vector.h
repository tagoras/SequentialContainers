#ifndef VECTOR_H
#define VECTOR_H

#include <memory>
#include <stdexcept>

namespace custom{

template<typename T>
class Vector{
    public:
        T* start{nullptr};
        T* first_unfilled{nullptr};
        int capacity{};
    public:
        // Copy control members
        Vector() = default; // +
        Vector(std::initializer_list<T>); // +

        Vector(const Vector<T> &); // +
        Vector<T>& operator=(std::initializer_list<T>);
        Vector<T>& operator=(const Vector<T> &);

        ~Vector();

        Vector(const Vector<T> &&);
        Vector<T>& operator=(const Vector<T> &&);

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

        template <T>
        friend std::ostream& operator<<(std::ostream &os, const Vector<T> vec);
};

template <typename T>
Vector<T>::Vector(std::initializer_list<T> elements){
    capacity = elements.size() * 2;
    start = new T[capacity];
    first_unfilled = start;

    for(T element : elements)
    {
        *first_unfilled = element;
        ++first_unfilled;
    }
}

template <typename T>
Vector<T>::Vector(const Vector<T> &original): capacity{original.capacity}
{
    start = new T[capacity];
    first_unfilled = start;

    for(int i = 0; i < original.size(); i++)
    {
        first_unfilled[i] = original.at(i);
    }

    /*If the original vector had 10 elements, then first_unfilled will now point to element at index 10
      Which is 1 beyond the last element
    */
    first_unfilled += original.size();
}

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector<T> &rhs)
{
    if(this == &rhs)
        return *this;
    
    delete[] start;

    capacity = rhs.capacity;
    start = new T[capacity];
    // first_unfilled previously pointed to deallocated memory, now points to start
    first_unfilled = start;

    for(int i = 0; i < rhs.size(); i++)
    {
        start[i] = rhs.start[i];
    }
    // Have to use other object for size because size() function works by subtracting start from first_unfilled
    first_unfilled += rhs.size();

    return *this;
}

template <typename T>
Vector<T>::~Vector()
{   
    // Don't think setting first_unfilled to nullptr is necessary
    //first_unfilled = nullptr;
    delete[] start;
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

template <typename T>
std::ostream& operator<<(std::ostream &os, const Vector<T> vec)
{
    for(int i = 0; i < vec.size(); i++)
    {
        os << vec[i] << " ";
    }
    return os;
}

}

#endif