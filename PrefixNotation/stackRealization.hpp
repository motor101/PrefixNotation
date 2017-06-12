/**
*
* Solution to homework task
* Data Structures Course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2016/2017
*
* @author Kamen Ivanov
* @idnumber 61894
* @task 1
* @compiler VC
*
*/

/*
implements a stack
*/


#ifndef STACK_REALIZATION_HPP
#define STACK_REALIZATION_HPP

#include<stdexcept>
#include<cassert>


template<typename T>
inline Stack<T>::Stack()
{
	arr = new T[defaultCapacity];
	capacity = defaultCapacity;
	size = 0;
}

template<typename T>
inline Stack<T>::Stack(const Stack<T>& other)
{
	capacity = (other.capacity >= defaultCapacity) ? other.capacity : defaultCapacity;
	
	arr = new T [capacity];

	size = other.size;

	for (int i = 0; i < size; ++i)
		arr[i] = other.arr[i];
}

template<typename T>
inline Stack<T>& Stack<T>::operator=(const Stack<T>& other)
{
	if (this != &other)
	{
		int newCapacity = (other.capacity >= defaultCapacity) ? other.capacity : defaultCapacity;

		T *newArr = new T[newCapacity];

		for (int i = 0; i < other.size; ++i)
			newArr[i] = other.arr[i];

		clear();

		capacity = newCapacity;
		size = other.size;
		arr = newArr;
	}
	return *this;
}

template<typename T>
inline Stack<T>::~Stack()
{
	clear();
}

template<typename T>
inline bool Stack<T>::isEmpty() const
{
	return size == 0;
}


template<typename T>
inline bool Stack<T>::isFull() const
{
	return size == capacity;
}


template<typename T>
inline void Stack<T>::push(const T& element)
{
	if (isFull())
	{
		int newCapacity = (capacity < defaultCapacity) ? 2*defaultCapacity : 2*capacity;
		extendCapacity(newCapacity);
	}

	arr[size++] = element;

}

template<typename T>
inline T Stack<T>::pop()
{
	if (isEmpty())
		throw exception("stack is empty");

	else
		return arr[--size];
}


template<typename T>
inline void Stack<T>::extendCapacity(const int newCapacity)
{
	assert(newCapacity > capacity);

	T *newArr = new T[newCapacity];

	for (int i = 0; i < size; ++i)
		newArr[i] = arr[i];

	delete[] arr;
	arr = newArr;

	capacity = newCapacity;
}

template<typename T>
inline void Stack<T>::clear()
{
	delete[] arr;
	arr = NULL;

	size = 0;

	capacity = 0;
}

#endif  // STACK_REALIZATION_HPP
