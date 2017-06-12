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

#include"stdafx.h"
#include<stdexcept>
#include<iostream>
#include "myString.h"

/*
implements a string
*/


namespace fmi
{
namespace sdp
{

String::String(int size)
{
	if (size <= 0)
		throw std::invalid_argument("invalid index");

	capacity = size + 1;

	//the initial size = 0
	arr = new char[capacity]{};

}

String::String(const String& other)
{
	if (other.capacity <= 0)
		throw std::exception("the String other has no elements");

	capacity = other.capacity;

	arr = new char[capacity];

	strcpy(arr, other.arr);

}

String& String::operator=(const String& other)
{
	if (this != &other)
	{
		if (capacity >= other.capacity)
		{
			if (other.capacity <= 0)
				throw std::exception("the String other has no elements");
			else
				strcpy(arr, other.arr);
		}
		else
		{
			char *newArr = new char[other.capacity];
			strcpy(newArr, other.arr);

			clear();

			arr = newArr;
			capacity = other.capacity;

		}
	}
	return *this;
}

String::~String()
{
	clear();
}

String::String(const char* str, const int len)
{
	if (len < 0)
		throw std::invalid_argument("invalid length");

	capacity = len + 1;

	arr = new char[capacity];

	strncpy(arr, str, len);

	arr[len] = '\0';
}

int String::getSize()const
{
	return strlen(arr);
}

void String::print()const
{
	if (capacity <= 0)
		throw std::exception("the String has no elements");

	std::cout << arr << std::endl;
}

void String::clear()
{
	delete[] arr;
	arr = NULL;
	capacity = 0;
}

//returns a concatenation of thwo Strings and a symbol of an operator
//with 2 intervals between them
String concatenateTwoStringsAndOperationWithSpaces
	(const String& first
	, const String& second
	, const char operationSymbol
	)
{


	const int size1 = first.getSize();
	const int size2 = second.getSize();

	//there will be two intervals between the three strings
	//That's why we add two times the length of ' ' i.e. two times 1
	String result(size1 + 1 + size2 + 1 + 1);

	strncpy(&result.arr[0], first.arr, size1);

	result.arr[size1] = ' ';

	strncpy(&result.arr[size1 + 1], second.arr, size2);

	result.arr[size1 + 1 + size2] = ' ';

	result.arr[size1 + 1 + size2 + 1] = operationSymbol;

	result.arr[size1 + 1 + size2 + 1 + 1] = '\0';

	return result;
}



}//sdp
}//fmi