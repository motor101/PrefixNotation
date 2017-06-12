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
defines a stack
*/

#ifndef STACK_H
#define STACK_H

namespace fmi
{
namespace sdp
{

template<typename T>
class Stack
{
public:
	Stack();

	Stack(const Stack<T>& other);

	Stack<T>& operator=(const Stack<T>& other);

	~Stack();

public:

	bool isEmpty() const;

	bool isFull() const;

	int getSize()const { return size; }

	int getCapacity()const { return capacity; }

public:

	void push(const T& element);

	T pop();


	static const int defaultCapacity;

private:
	T* arr;
	int size;
	int capacity;


private:
	//reallocates new dinamically memory for arr with a bigger capacity
	void extendCapacity(const int newCapacity);

	void clear();

};

template<typename T>
const int Stack<T>::defaultCapacity = 10;

#include "stackRealization.hpp"

}//sdp
}//fmi

#endif //STACK_H
