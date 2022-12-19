#ifndef VECTOR_H
#define VECTOR_H

#include <memory>

namespace custom{

template<typename T>
class Vector{
    public:
        T* start{nullptr};
        int length{};

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
    length = elements.size();
    start = new T[length * 2];

    T* copy_start = start;
    for(T element : elements)
    {
        *copy_start = element;
        ++copy_start;
    }
}

}

#endif