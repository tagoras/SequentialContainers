#ifndef VECTOR_H
#define VECTOR_H

#include <memory>
#include <stdexcept>
#include <utility>

namespace custom{

template<typename T>
class Iterator{
    public:
        using iterator_category =  std::bidirectional_iterator_tag;

        Iterator(T* element_pointer);

        T& operator*() const {return *m_pointer;}
        T* operator->();

        Iterator& operator++() { ++m_pointer; return *this; }

        Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

        Iterator& operator--() {--m_pointer; return *this; }

        Iterator operator--(int) {Iterator tmp = *this; --(*this); return tmp;}

        template<typename U>
        friend bool operator==(const Iterator<U> &lhs, const Iterator<U> &rhs);

        template<typename U>
        friend bool operator!=(const Iterator<U> &lhs, const Iterator<U> &rhs);
    protected:
        T* m_pointer;
};

template<typename T>
Iterator<T>::Iterator(T* element_pointer): m_pointer{element_pointer} {}

template<typename T>
bool operator==(const Iterator<T> &lhs, const Iterator<T> &rhs) {return lhs.m_pointer == rhs.m_pointer;}

template<typename T>
bool operator!=(const Iterator<T> &lhs, const Iterator<T> &rhs) {return lhs.m_pointer != rhs.m_pointer;}

template<typename T>
class Constant_Iterator : public Iterator<T>{
    public:
        Constant_Iterator(T* element_pointer);

        /*
            For some reason, if the following code tries to do
            
            {return *m_pointer}
            This causes a compilation error "m_pointer was not declared in this scope".

            This is rather weird, as it means that m_pointer does not depend on the template arguments, which, 
            in my opinion, it does.
            
        */
        const T& operator*() const {return *(this->m_pointer);}
};

template<typename T>
Constant_Iterator<T>::Constant_Iterator(T* element_pointer): Iterator<T>{element_pointer} {}


/*
    This is a high-level generic vector class that attempts to provide the core functionality that is 
    provided in the STL Vector container class.

    T* m_start; denotes the memory location of the first element
    T* m_first_unfilled; denotes the memory location of the first empty memory address
    int capacity; the total amount of elements the container can currently hold
*/


template<typename T>
class Vector{
    public:
        using iterator = Iterator<T>;
        using const_iterator = Constant_Iterator<T>;

        int capacity{0};
        T* m_start{nullptr};
        T* m_first_unfilled{nullptr};
    
        // Copy control members
        Vector() = default; // +
        Vector(std::initializer_list<T>); // +
        Vector(int);

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
        void reserve(int);
        void shrink_to_fit();

        //Modifiers
        void clear();

        void insert(const T &);
        void insert(std::initializer_list<T> elements);
        
        T erase(const T &);
        T erase(int pos);
        T erase(int m_start, int end);

        void push_back(const T &);
        void push_back(std::initializer_list<T> elements);
        void push_back(const Vector<T> &other);

        T pop_back();
        void resize(); // +

        void swap(const Vector<T> &other);

        template <T>
        friend std::ostream& operator<<(std::ostream &os, const Vector<T> vec);
};

/*
    Constructor that initializes the vector from std::initializer_list (to allow brace-list initialization).
    
    Initialises the capacity to double of the number of elements in the initializer list. Allocates memory for
    capacity elements and then copies each element in the initializer list. After 
*/

template <typename T>
Vector<T>::Vector(std::initializer_list<T> elements): capacity{elements.size() * 2},
    m_start{new T[capacity]}, m_first_unfilled{m_start} 
{
    for(T& element : elements)
    {
        *m_first_unfilled = element;
        ++m_first_unfilled;
    }
}

template <typename T>
Vector<T>::Vector(int size): capacity{size}, m_start{new T[size]}, m_first_unfilled{m_start} {}

/*
    Copy constructor. After copying the elements we move the m_first_unfilled by the size of the original
    vector so that m_first_unfilled now points to the first empty memory location
*/

template <typename T>
Vector<T>::Vector(const Vector<T> &original): capacity{original.capacity}, m_start{new T[capacity]},
m_first_unfilled{m_start}
{
    for(int i = 0; i < original.size(); i++)
    {
        m_first_unfilled[i] = original.at(i);
    }

    /*
        If the original vector had 10 elements, then m_first_unfilled will now point to element at index 10
        Which is 1 beyond the last element
    */
    m_first_unfilled += original.size();
}

/*
    Copy assignment with initializer_list.

    @param: std::initialized_list<T> for brace-list initialization
    @return: the assigneed-to vector
*/

template<typename T>
Vector<T>& Vector<T>::operator=(std::initializer_list<T> elements)
{
    delete[] m_start;
    
    // Add one before multiplying in case vector is assigned to empty list i.e. {}
    capacity = 2 * (elements.size() + 1);
    m_start = new T[capacity];
    m_first_unfilled = m_start;

    for(T& element : elements)
    {
        *m_first_unfilled = element;
        ++m_first_unfilled;
    }

    return *this;
}

/*
    Copy assignment operator. Deallocates the current memory and deep copies each element to the 
    current vector.

    @param: const Vector<T>
    @return: Vector<T>& - assigned-to vector
*/

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector<T> &rhs)
{
    if(this == &rhs)
        return *this;
    
    delete[] m_start;

    capacity = rhs.capacity;
    m_start = new T[capacity];
    // m_first_unfilled previously pointed to deallocated memory, now points to m_start
    m_first_unfilled = m_start;

    for(int i = 0; i < rhs.size(); i++)
    {
        m_start[i] = rhs.m_start[i];
    }
    // Have to use other object for size because size() function works by subtracting m_start from m_first_unfilled
    m_first_unfilled += rhs.size();

    return *this;
}

/*
    Destructor. Deallocates the memory beginning at m_start.
*/

template <typename T>
Vector<T>::~Vector()
{   
    delete[] m_start;
}

/*
    Move constructor. Copies capacity into the vector and transfers ownership from one vector to another.
    The moved-from vector's m_start and m_first_unfilled pointers are set to nullptr and capacity of 0
*/

template<typename T>
Vector<T>::Vector(Vector<T> &&original): capacity{original.capacity}, m_start{original.m_start}, 
m_first_unfilled{original.m_first_unfilled}
{
    original.capacity = 0;
    original.m_start = nullptr;
    original.m_first_unfilled = nullptr;
}

/*
    Move assignment operator. Works like the move constructor but returns a reference to this object and guards against
    self assignment.
*/
template<typename T>
Vector<T>& Vector<T>::operator=(Vector<T> &&original)
{
    if(this == &original)
        return *this;

    // Now same as in move constructor

    capacity = original.capacity;
    m_start = original.m_start;
    m_first_unfilled = original.m_first_unfilled;

    original.capacity = 0;
    original.m_start = nullptr;
    original.m_first_unfilled = nullptr;

    return *this;    
}

/*
    Returns an iterator to the first element in the container;

    P.S As of now I do not understand all the rationale behind typename usage! 2023-01-30
*/
template<typename T>
typename Vector<T>::iterator Vector<T>::begin() const {
    return Iterator{m_start};
}
/*
    Returns an iterator one past the last element in the container
*/
template<typename T>
typename Vector<T>::iterator Vector<T>::end() const {
    return Iterator{m_first_unfilled};
}
/*
    Returns a const_iterator to the first element in the container
*/
template<typename T>
typename Vector<T>::const_iterator Vector<T>::cbegin() const {
    return Constant_Iterator{m_start};
}
/*
    Returns a const_iterator one past the last element in the container
*/
template<typename T>
typename Vector<T>::const_iterator Vector<T>::cend() const {
    return Constant_Iterator{m_first_unfilled};
}

/*
    Function for alternative acceess to vector's members.
    @param: int index - element position to retrieve
    @return: T& - reference to the element at position index
    @throws: if index is invalid throws std::out_of_range()
*/
template <typename T>
T& Vector<T>::at(int index) const {
    if(index >= size() || index < 0) throw std::out_of_range("Tried to access element out of range");
    return *(m_start + index);
}

/*
    Unsafe way to access vector's elemenets
    @param: int index - element position to retrieve
    @return: T& - reference to the element at position index
*/
template <typename T>
T& Vector<T>::operator[](int index) const {
    return *(m_start + index);
}

/*
    Returns the number of the elements in the container.
*/
template <typename T>
int Vector<T>::size() const {
    return m_first_unfilled - m_start;
}

template <typename T>
void Vector<T>::reserve() {

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

/*
    Resizes the current container by allocating two times more memory and copying each element from the old memory
    space to the new memory space and then tweaking the member pointers in the end.
*/
template<typename T>
void Vector<T>::resize()
{
    capacity = (capacity + 1) * 2;

    T* new_m_start = new T[capacity];
    T* new_m_first_unfilled = new_m_start;

    const_iterator beg{cbegin()}, end{cend()};

    while(beg != end)
    {
        *new_m_first_unfilled = *beg;
        ++new_m_first_unfilled;
        ++beg;
    }

    delete[] m_start;

    m_start = new_m_start;
    m_first_unfilled = new_m_first_unfilled;
}

template<typename T>
void Vector<T>::push_back(const T &element)
{
    if(size() == capacity) resize();
    
    *m_first_unfilled = element;
    m_first_unfilled++;
}       

template<typename T>
void Vector<T>::push_back(std::initializer_list<T> elements)
{
    for(T& element: elements)
    {
        if(size() == capacity) resize();
        *m_first_unfilled = element;
        m_first_unfilled++;
    }
}
        
template<typename T>
void Vector<T>::push_back(const Vector<T> &other)
{
    for(T& element : other)
    {
        if(size() == capacity) resize();
        *m_first_unfilled = element;
        m_first_unfilled++;
    }
}

}

#endif