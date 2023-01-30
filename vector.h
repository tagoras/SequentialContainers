#ifndef VECTOR_H
#define VECTOR_H

#include <memory>
#include <stdexcept>

namespace custom{

    /*
        This is a high-level generic vector class that attempts to provide the core functionality that is 
        provided in the STL Vector container class.

        T* start; denotes the memory location of the first element
        T* first_unfilled; denotes the memory location of the first empty memory address
        int capacity; the total amount of elements the container can currently hold
    */

template<typename T>
class Vector{
    public:
    
        T* start{nullptr};
        T* first_unfilled{nullptr};
        int capacity{};
    
        // Copy control members
        Vector() = default; // +
        Vector(std::initializer_list<T>); // +

        Vector(const Vector<T> &); // +
        Vector<T>& operator=(std::initializer_list<T>); // +
        Vector<T>& operator=(const Vector<T> &); // +

        ~Vector(); // +

        Vector(Vector<T> &&); // +
        Vector<T>& operator=(Vector<T> &&); // +

        //Iterator retrieval
        /*
            Implement functions that give back iterators:
            begin()
            end()
            cbegin()
            cend()
        */
        iterator begin() const; 
        iterator end() const; 
        const_iterator cbegin() const; 
        const_iterator cend() const; 

        //Access Functions
        T& at(int) const; // +
        T& operator[](int) const; // +
        T& front() const;
        T& back() const;

        // Capacity Functions
        bool empty() const;
        int size() const; // +
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
        void resize(); // +

        void swap(const Vector<T> &other);

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

template<typename T>
Vector<T>& Vector<T>::operator=(std::initializer_list<T> elements)
{
    delete[] start;
    
    capacity = 2 * elements.size();
    start = new T[capacity];
    first_unfilled = start;

    for(T element : elements)
    {
        *first_unfilled = element;
        first_unfilled++;
    }
}

template <typename T>
Vector<T>::~Vector()
{   
    // Don't think setting first_unfilled to nullptr is necessary
    //first_unfilled = nullptr;
    delete[] start;
}
/*
    Move constructor. Copies capacity into the vector and transfers ownership from one vector to another.
    The moved-from vector's start and first_unfilled pointers are set to nullptr and capacity of 0
*/
template<typename T>
Vector<T>::Vector(Vector<T> &&original): capacity{original.capacity}
{
    std::cout << "Move constructor applied" << std::endl;
    start = original.start;
    first_unfilled = original.first_unfilled;

    original.capacity = 0;
    original.start = nullptr;
    original.first_unfilled = nullptr;
}

/*
    Move assignment operator. Works like ^ move constructor but returns a reference to this object and guards against
    self assignment.
*/
template<typename T>
Vector<T>& Vector<T>::operator=(Vector<T> &&original)
{
    std::cout << "Coppy assignment operator applied" << std::endl;
    if(this == &original)
        return *this;

    // Now same as in move constructor

    start = original.start;
    first_unfilled = original.first_unfilled;

    original.capacity = 0;
    original.start = nullptr;
    original.first_unfilled = nullptr;

    return *this;    
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

template<typename T>
void Vector<T>::resize()
{
    capacity *= 2;
    // If vector was default initialized multiplication of capacity will not change the capacity
    if(!capacity) capacity = 10;

    T* new_begin = new T[capacity];
    T* new_first_unfilled = new_begin;

    
    for(T& element : *this)
    {
        *new_first_unfilled = element;
        new_first_unfilled++;
    }

    delete[] start;
    start = new_begin;

    first_unfilled = new_first_unfilled;
}

template<typename T>
void Vector<T>::push_back(const T &element)
{
    if(size() == capacity) resize();
    
    *first_unfilled = element;
    first_unfilled++;
}       

template<typename T>
void Vector<T>::push_back(std::initializer_list<T> elements)
{
    for(T& element: elements)
    {
        if(size() == capacity) resize();
        *first_unfilled = element;
        first_unfilled++;
    }
}
        
template<typename T>
void Vector<T>::push_back(const Vector<T> &other)
{
    for(T& element : other)
    {
        if(size() == capacity) resize();
        *first_unfilled = element;
        first_unfilled++;
    }
}

}

#endif