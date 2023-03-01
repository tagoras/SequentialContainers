#ifndef VECTOR_H
#define VECTOR_H

#include <memory>
#include <stdexcept>
#include <utility>

namespace custom {

	// Inspiration on how to write a custom iterator taken from:
	// https://www.internalpointers.com/post/writing-custom-iterators-modern-cpp

	template<typename T>
	class Iterator {
	public:
		using iterator_category = std::bidirectional_iterator_tag;

		Iterator(T* element_pointer);

		T& operator*() const { return *m_pointer; }
		T* operator->();

		Iterator& operator++() { ++m_pointer; return *this; }

		Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

		Iterator& operator--() { --m_pointer; return *this; }

		Iterator operator--(int) { Iterator tmp = *this; --(*this); return tmp; }

		template<typename U>
		friend bool operator==(const Iterator<U>& lhs, const Iterator<U>& rhs);

		template<typename U>
		friend bool operator!=(const Iterator<U>& lhs, const Iterator<U>& rhs);
	protected:
		T* m_pointer;
	};

	template<typename T>
	Iterator<T>::Iterator(T* element_pointer) : m_pointer{ element_pointer } {}

	template<typename T>
	T* Iterator<T>::operator->() { return *m_pointer; }

	template<typename T>
	bool operator==(const Iterator<T>& lhs, const Iterator<T>& rhs) { return lhs.m_pointer == rhs.m_pointer; }

	template<typename T>
	bool operator!=(const Iterator<T>& lhs, const Iterator<T>& rhs) { return lhs.m_pointer != rhs.m_pointer; }

	template<typename T>
	class Constant_Iterator : public Iterator<T> {
	public:
		Constant_Iterator(T* element_pointer);

		/*
			For some reason, if the following code tries to do

			{return *m_pointer}
			This causes a compilation error "m_pointer was not declared in this scope".
			This is rather weird, as it means that m_pointer does not depend on the template arguments, which,
			in my opinion, it does.

		*/
		const T& operator*() const { return *(this->m_pointer); }
	};

	template<typename T>
	Constant_Iterator<T>::Constant_Iterator(T* element_pointer) : Iterator<T>{ element_pointer } {}


	/*
		This is a high-level generic vector class that attempts to provide the core functionality that is
		provided in the STL Vector container class.
		T* m_start; denotes the memory location of the first element
		T* m_first_unfilled; denotes the memory location of the first empty memory address
		int capacity; the total amount of elements the container can currently hold
	*/


	template<typename T>
	class Vector {
	public:
		using iterator = Iterator<T>;
		using const_iterator = Constant_Iterator<T>;

		// Copy control members
		Vector() = default; // +
		Vector(const std::initializer_list<T>&); // +
		Vector(int);

		Vector(const Vector<T>&); // +
		Vector<T>& operator=(std::initializer_list<T>); // +
		Vector<T>& operator=(const Vector<T>&); // +

		~Vector(); // +

		Vector(Vector<T>&&); // +
		Vector<T>& operator=(Vector<T>&&); // +

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
		T& front() const; // +
		T& back() const; // +

		// Capacity Functions
		bool empty() const; // +
		int size() const; // +
		void reserve(int); // +
		//void shrink_to_fit();

		//Modifiers
		//void clear();

		void insert(const_iterator pos, const T&);
		void insert(const_iterator pos, int count, const T&);

		T erase(const T&);
		T erase(int pos);
		T erase(int m_start, int end);

		void push_back(const T&); // +
		void push_back(std::initializer_list<T> elements); // +
		void push_back(const Vector<T>& other); // +

		void pop_back();
		void resize(int count = -1); // +

		void swap(const Vector<T>& other);

		template <T>
		friend std::ostream& operator<<(std::ostream& os, const Vector<T> vec);
	private:
		std::size_t capacity{ 0 };
		T* m_start{ nullptr };
		T* m_first_unfilled{ nullptr };
	};

	/*
		Constructor that initializes the vector from std::initializer_list (to allow brace-list initialization).

		Initialises the capacity to double of the number of elements in the initializer list. Allocates memory for
		capacity elements and then copies each element in the initializer list. After
	*/

	template <typename T>
	Vector<T>::Vector(const std::initializer_list<T>& elements) : capacity{ elements.size() * 2 },
		m_start{ new T[capacity] }, m_first_unfilled{ m_start }
	{
		for (const T& element : elements)
		{
			*m_first_unfilled = element;
			++m_first_unfilled;
		}
	}

	template <typename T>
	Vector<T>::Vector(int size) : capacity{ size }, m_start{ new T[size] }, m_first_unfilled{ m_start } {}

	/*
		Copy constructor. After copying the elements we move the m_first_unfilled by the size of the original
		vector so that m_first_unfilled now points to the first empty memory location
	*/

	template <typename T>
	Vector<T>::Vector(const Vector<T>& original) : capacity{ original.capacity }, m_start{ new T[capacity] },
		m_first_unfilled{ m_start }
	{
		for (int i = 0; i < original.size(); i++)
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

		for (T& element : elements)
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
	Vector<T>& Vector<T>::operator=(const Vector<T>& rhs)
	{
		if (this == &rhs)
			return *this;

		delete[] m_start;

		capacity = rhs.capacity;
		m_start = new T[capacity];
		// m_first_unfilled previously pointed to deallocated memory, now points to m_start
		m_first_unfilled = m_start;

		for (int i = 0; i < rhs.size(); i++)
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
	Vector<T>::Vector(Vector<T>&& original) : capacity{ original.capacity }, m_start{ original.m_start },
		m_first_unfilled{ original.m_first_unfilled }
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
	Vector<T>& Vector<T>::operator=(Vector<T>&& original)
	{
		if (this == &original)
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
		return iterator{ m_start };
	}
	/*
		Returns an iterator one past the last element in the container
	*/
	template<typename T>
	typename Vector<T>::iterator Vector<T>::end() const {
		return iterator{ m_first_unfilled };
	}
	/*
		Returns a const_iterator to the first element in the container
	*/
	template<typename T>
	typename Vector<T>::const_iterator Vector<T>::cbegin() const {
		return const_iterator{ m_start };
	}
	/*
		Returns a const_iterator one past the last element in the container
	*/
	template<typename T>
	typename Vector<T>::const_iterator Vector<T>::cend() const {
		return const_iterator{ m_first_unfilled };
	}

	/*
		Function for alternative acceess to vector's members.
		@param: int index - element position to retrieve
		@return: T& - reference to the element at position index
		@throws: if index is invalid throws std::out_of_range()
	*/
	template <typename T>
	T& Vector<T>::at(int index) const {
		if (index >= size() || index < 0) throw std::out_of_range("Tried to access element out of range");
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

	template <typename T>
	T& Vector<T>::front() const {
		return *m_start;
	}

	template <typename T>
	T& Vector<T>::back() const {
		return *m_first_unfilled;
	}

	template <typename T>
	bool Vector<T>::empty() const {
		return m_start == m_first_unfilled;
	}

	/*
		Returns the number of the elements in the container.
	*/
	template <typename T>
	int Vector<T>::size() const {
		return m_first_unfilled - m_start;
	}

	/*
		Increases the amount of memory allocated for the vector. If count <= capacity, the function does not do anything.
		Otherwise, new memory allocation is done and all the elements of the current vector are copied into the new memory space.
	*/

	template <typename T>
	void Vector<T>::reserve(int count) {
		if (count <= capacity) return;

		capacity = count;
		T* new_m_start = new T[capacity];
		T* new_m_first_unfilled = new_m_start;

		for (const T& element : *this)
		{
			*new_m_first_unfilled = element;
			++new_m_first_unfilled;
		}
		delete m_start;

		m_start = new_m_start;
		m_first_unfilled = new_m_first_unfilled;
	}

	template <typename T>
	std::ostream& operator<<(std::ostream& os, const Vector<T> vec)
	{
		for (int i = 0; i < vec.size(); i++)
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
	void Vector<T>::resize(int count)
	{
		capacity = (capacity + 1) * 2;

		T* new_m_start = new T[capacity];
		T* new_m_first_unfilled = new_m_start;

		const_iterator beg{ cbegin() }, end{ cend() };

		while (beg != end)
		{
			*new_m_first_unfilled = *beg;
			++new_m_first_unfilled;
			++beg;
		}

		delete[] m_start;

		m_start = new_m_start;
		m_first_unfilled = new_m_first_unfilled;
	}
	/*
	2023-01-31
	As of now I can not write the following function because I do not understand how to deal with the situation where the element is a primitive data type
	and thus does not have a destructor which I could call.
	template<typename T>
	void Vector<T>::clear()
	{
		iterator it = begin();
		while(it != end()) it->~T();
		m_first_unfilled = m_start;
	}
	*/

	template<typename T>
	void Vector<T>::insert(const_iterator pos, const T&)
	{

	}

	/* Adds an element. First check if the container is full and if it is then resize the container */
	template<typename T>
	void Vector<T>::push_back(const T& element)
	{
		if (size() == capacity) resize();

		*m_first_unfilled = element;
		m_first_unfilled++;
	}

	/* Same as above, but we do this multiple times, checking if the container has been filled each time*/
	template<typename T>
	void Vector<T>::push_back(std::initializer_list<T> elements)
	{
		for (const T& element : elements)
		{
			if (size() == capacity) resize();
			*m_first_unfilled = element;
			m_first_unfilled++;
		}
	}

	/*Push back all the elements from another vector container into this container*/
	template<typename T>
	void Vector<T>::push_back(const Vector<T>& other)
	{
		for (T& element : other)
		{
			if (size() == capacity) resize();
			*m_first_unfilled = element;
			m_first_unfilled++;
		}
	}
	/*
		Currently this does not work. Update. The reason that this does not work is because in C++ if you allocate memory dynamically
		using T* pointer = new T[size]; you must deallocate using delete[] pointer. That is, you allocate an array and de-allocate an array. You are not allowed to
		deallocate one element using simple delete statement if you allocated the whole block using an array version of new operator.

		The current "solution" (a very beautiful word to describe what is essentially an intentional was of storage) is to not deallocate memory and just comment out
		the last statement which is causing an undefined behaviour.
	*/
	template<typename T>
	void Vector<T>::pop_back()
	{
		if (size() == 0) return;
		T* last_element = m_first_unfilled-1;

		--m_first_unfilled;

		//delete last_element;
	}

}

#endif