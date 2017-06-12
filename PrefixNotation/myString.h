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
defines a string class
*/

#ifndef MY_STRING_H
#define MY_STRING_H

#include<cstring>


namespace fmi
{
namespace sdp
{
	//returns a concatenation of thwo Strings and a symbol of an operator
	//with 2 intervals between them
class String
{
	friend String concatenateTwoStringsAndOperationWithSpaces
	(const String& first
	, const String& second
	, const char operationSymbol
	);
public:
	String(int size = defaultCapacity);

	String(const String& other);

	String& operator=(const String& other);

	String(const char* str, const int len);

	~String();

public:
	int getSize()const;

	void print()const;

private:
	int capacity;
	char* arr;

private:
	static const int defaultCapacity = 20;

private:
	void clear();
};

} //sdp
} //fmi

#endif	// MY_STRING_H