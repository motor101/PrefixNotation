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
Implements a system of functions that
1) Open a file with operators and extract it's information 
2) Open a file containing a prefix expression avaluate it and 
convert it to an equivalent postfix expression
*/


#include "stdafx.h"
#include<iostream>
#include<cctype>	//this header is needed for isspace
#include<fstream>
#include"stack.h"
#include"myString.h"
#include<iomanip>	//for setprecision

namespace fmi
{
namespace sdp
{

//values that show the output
enum FunctionResult
{
	NORMAL,
	UNABLE_TO_OPEN_FILE,
	FILE_IS_EMPTY,
	SINTACTICAL_ERROR,
	UNKNOWN_ERROR,
	LOGICAL_ERROR,
	FUNCTION_RESULT_COUNT
};

//represents the associativity of an operation
enum Associativity
{
	FROM_LEFT_TO_RIGHT,
	FROM_RIGHT_TO_LEFT,
	ASSOCIATIVITY_COUNT
};


//represents a programmming operation
struct Operation
{
	Associativity associativity;
	char actualOperatoration;
	char symbol;
};

//Every correct postfix expression can be evaluated
struct PostfixExpression
{
	double value;
	String notationString;
};

}//sdp
}//fmi


using namespace std;
using namespace fmi::sdp;

//checks if symbol is a valid operator symbol from operationsList
bool isTheSymbolAnOperator(char symbol, const Operation* operationsList, const int operationsListSize)
{
	for (int i = 0; i < operationsListSize;++i)
	if (symbol == operationsList[i].symbol)
		return true;

	return false;
}

//returns which operator from operationsList the symbol is 
char whichOperatorIsTheSymbol(char symbol, const Operation* operationsList, const int operationsListSize)
{
	for (int i = 0; i < operationsListSize; ++i)
	if (symbol == operationsList[i].symbol)
		return operationsList[i].actualOperatoration;

	return -1;
}

double executeOperation(const double firstNumber, const char operation, const double secondNumber)
{
	switch (operation)
	{
	case '+':	return firstNumber + secondNumber;

	case '-':	return firstNumber - secondNumber;

	case '*':	return firstNumber * secondNumber;

	case '/':	if (secondNumber == 0)
		throw invalid_argument("can not devide by zero");

		return firstNumber / secondNumber;

	default:throw invalid_argument("invalid operation");
	}
}


//we have found a string in prefixNotationFile containing no whitespaces
//this string starts at position firstPos and ends in position lastPos-1
//lastPos is actually the position of the first
//whitespace character from the right side of the string
FunctionResult  extractAndComputeInformationFromPrefixFile
( ifstream& prefixNotationFile
, const char* fileContentString
, const int firstPos
, const int lastPos
, Stack<PostfixExpression>& postfixExpressionStack
, const Operation *operationsList
, const int operationsListSize
)
{
	//the function should be receving a file in good state.
	//If the file doesn't ecaluate to true, the error
	//should be in the caller function
	if (!prefixNotationFile)
	{
		return UNKNOWN_ERROR;
	}

	//we want to start the reading from the position the string starts
	prefixNotationFile.seekg(firstPos, ios::beg);
	
	//we'll use this variable to estract real numbers from the file
	//with operator>> 
	double floatingPointNumber;

	prefixNotationFile >> floatingPointNumber;

	//if the input really was a floating point number
	if (prefixNotationFile)
	{
		//clear the file in case we have reached the eof
		prefixNotationFile.clear();
		
		//if the whole string between firstPos and lastPos is a floatong point number
		if ( lastPos == prefixNotationFile.tellg())
			postfixExpressionStack.push
		(PostfixExpression { floatingPointNumber, String(&fileContentString[firstPos], lastPos - firstPos) });
		
		//if not the whole string is a floating point number
		else
		{
			cerr << "There is an invalid symbol at position " << prefixNotationFile.tellg() << '\n';
			return SINTACTICAL_ERROR;
		}
	}
	//the input must be an operator
	else
	{
		prefixNotationFile.clear();
		
		//every operator has a length of 1
		if (firstPos + 1 != lastPos)
		{
			cerr << "The string, starting at position " << firstPos << " isn't a valid operator\n";
			return SINTACTICAL_ERROR;
		}

		//the string consists only of one symbol
		//let's check if the symbol is a valid operator
		else
		{
			char symbol = fileContentString[firstPos];
			if (isTheSymbolAnOperator(symbol, operationsList, operationsListSize))
			{

				//every operator takes two arguments
				if (postfixExpressionStack.getSize() < 2)
				{
					cerr << "There aren't enough arguments for operation " 
						 << symbol << " at position " << firstPos<<'\n';
					return SINTACTICAL_ERROR;
				}
				else
				{
					PostfixExpression postfixExpression1 = postfixExpressionStack.pop();
					PostfixExpression postfixExpression2 = postfixExpressionStack.pop();
					double operationResult;
					try
					{
					operationResult=
						executeOperation
						(postfixExpression1.value
						, whichOperatorIsTheSymbol(symbol, operationsList, operationsListSize)
						, postfixExpression2.value);
					}
					catch (const invalid_argument& except)
					{
						cerr << except.what() << '\n';
						return LOGICAL_ERROR;
					}
					//we know the result of the operation and can now push
					//it on top of the postfixExpressionStack along with the concatenation
					//of the previous two postfixExpressions and the operation
					postfixExpressionStack.push(PostfixExpression{ operationResult
						, concatenateTwoStringsAndOperationWithSpaces
						(postfixExpression1.notationString, postfixExpression2.notationString, symbol) });
				}
			}
			else
			{
				cerr << "The symbol at position " << firstPos << " isn't a valid operator\n";
				return SINTACTICAL_ERROR;
			}
		}
	}

	return NORMAL;
}


//defines the values the stateFlag can have
//OUT - the last symbol read was a whitespace
//IN - the last symbol read was not a whitespace
//stateFlag is an internal boolean variable
//in openAndCaltulateInfixNotationFile()
#define OUT 0
#define IN 1


//opens the prefix notation file,
//extracts the strings
//and applies an algorithm for conversion
//to postfix notation and
//an algorithm for evaluation of the prefix notation
FunctionResult openAndCaltulatePrefixNotationFile
(const int operationsListSize
, const Operation* operationsList
, const char* nameOfPrefixNotationFile
)
{
	//this is the file containing the prefix notation expression
	//We open it in binary mode, because the function read() will be used
	ifstream prefixNotationFile(nameOfPrefixNotationFile, ios::in|ios::binary);
	if (!prefixNotationFile)
	{
		cerr << "Could not open " << nameOfPrefixNotationFile << " for reading\n";
		return UNABLE_TO_OPEN_FILE;
	}

	prefixNotationFile.seekg(0, ios::end);

	const int sizeOfFile = prefixNotationFile.tellg();
	

	//if the file is empty
	if (sizeOfFile == 0)
	{
		prefixNotationFile.close();
		return FILE_IS_EMPTY;
	}

	//this is a string, containing the whole content of prefixNotationFile
	char* fileContentString = new char[sizeOfFile];

	prefixNotationFile.seekg(0, ios::beg);
	prefixNotationFile.read(fileContentString, sizeOfFile);
	
	prefixNotationFile.clear();


	//we're going to apply an algorithm for evaluating a 
	//prefix expression and converting it to a postfix expression
	//The algorithm scans all elements of the prefix expression 
	//from right to left


	//shows if the last symbol read was a whitespace or not
	//if whitespace - OUT
	//else IN
	bool stateFlag = OUT;
	
	int currentPos;

	//shows the position of the last whitespace character read
	int lastPos = sizeOfFile;

	//shows the position of the last non-whitespace character read
	int firstPos = sizeOfFile;

	//this is the stack in which we'll collect the postfix 
	//expressions we get as we read the file from
	//right to left and calculate the prefix expressions
	Stack<PostfixExpression> postfixExpressionStack;

	FunctionResult functionResult;

	//we check all elements in the file
	for (currentPos = sizeOfFile - 1; currentPos >= 0; --currentPos)
	{
		//we wont change firstPos in this case
		if (isspace(fileContentString[currentPos]))
		{
			if (stateFlag == OUT)
			{
				--lastPos;
			}

			//if stateFlag=IN
			else
			{
				functionResult = extractAndComputeInformationFromPrefixFile
					( prefixNotationFile
					, fileContentString
					, firstPos
					, lastPos
					, postfixExpressionStack
					, operationsList
					, operationsListSize
					);

				if (functionResult != NORMAL)
				{
					prefixNotationFile.close();
					delete[] fileContentString;
					return functionResult;
				}


				lastPos = currentPos;
				stateFlag = OUT;
			}
		}
		//we wont change lastPos in this case
		else 
		{
			if (stateFlag == OUT)
			{
				firstPos = currentPos;
				stateFlag = IN;
			}

			//stateFlag = IN
			else
			{
				--firstPos;
			}
		}
	}

	//we also have to read the first argument of the prefixExpression
	if (stateFlag == IN )
	{
		functionResult = extractAndComputeInformationFromPrefixFile
			(prefixNotationFile
			, fileContentString
			, firstPos
			, lastPos
			, postfixExpressionStack
			, operationsList
			, operationsListSize
			);

		if (functionResult != NORMAL)
		{
			prefixNotationFile.close();
			delete[] fileContentString;
			return functionResult;
		}
	}

	//we don't need the fileContentString and the prefixNotationFile anymore
	delete[] fileContentString;
	prefixNotationFile.close();

	//after the algorithm finishes, there nust be only one element in the stack
	if (postfixExpressionStack.getSize() != 1)
	{
		if (postfixExpressionStack.getSize() <= 0)
			return UNKNOWN_ERROR;
		else
		{
			cerr << "There is an operation that takes more arguments than it uses\n";
			return SINTACTICAL_ERROR;
		}
	}
	
	const PostfixExpression postfixExpressionResult = postfixExpressionStack.pop();

	postfixExpressionResult.notationString.print();
	
	cout << fixed ;
	cout << setprecision(5);

	cout << postfixExpressionResult.value << endl;

	return NORMAL;
}

#undef OUT
#undef IN



//Finds the number of operations in the file called nameOfOperations
//and their characteristics.
//Stores them in operatinsListSize and operationsList.
//!!!If the function executes successfully, operationsList starts pointing to a dinamic memory
FunctionResult InitializeOperationsList
(int& operationsListSize
, Operation*& operationsList
, const char* nameOfOperationsFile
)
{
	//this is the file containing information about the operations
	ifstream operationsFile(nameOfOperationsFile, ios::in);
	if (!operationsFile)
	{
		cerr << "Could not open " << nameOfOperationsFile << " for reading\n";
		return UNABLE_TO_OPEN_FILE;
	}



	operationsFile.seekg(0, ios::end);

	const int sizeOfFile = operationsFile.tellg();

	if (sizeOfFile == 0)
	{
		operationsFile.close();
		return FILE_IS_EMPTY;
	}
	operationsFile.seekg(0, ios::beg);


	//no opearations have yet been read, so their number is 0 
	operationsListSize = 0;


	//we'll use this variable to traverse through all operations in the file
	Operation operationBuffer;

	//the format of the file must be as follows: {<symbol> <operator> <assosiativity>}
	while (operationsFile >> operationBuffer.symbol
		>> operationBuffer.actualOperatoration
		>> (int&)operationBuffer.associativity
		)
		++operationsListSize;

	operationsFile.clear();

	//checking if all characters have been read
	if (sizeOfFile != operationsFile.tellg())
	{
		//we add 1 to operationsListSize, because the first expression number 1 and so on...
		cerr << "There's an error in operator expression number " << operationsListSize+1
			<<'\n';
		operationsFile.close();
		return SINTACTICAL_ERROR;
	}
	
	if (operationsListSize > 0)
	{
		operationsList = new Operation[operationsListSize];

		operationsFile.seekg(0, ios::beg);

		for (int i = 0; i < operationsListSize; ++i)
		{
			operationsFile
				>> operationsList[i].symbol
				>> operationsList[i].actualOperatoration
				>> (int&)operationsList[i].associativity;
		}

		operationsFile.clear();

		//checking if all characters have been read
		if (sizeOfFile != operationsFile.tellg())
		{
			//This means that the last symbol of the file is either '+' or '-'
			//or there are whitespaces at the end of the file.
			//This is a sintactical error
			delete[] operationsList;
			operationsList = NULL;
			operationsFile.close();

			cerr << 
				"There's an error with the assosiativity in the last operator expression\n";

			return SINTACTICAL_ERROR;
		}

		operationsFile.close();
		return NORMAL;
	}


	 //operationsListSize is 0 and we have read all the symbols.
	//This means that the last symbol of the file is either '+' or '-'
	//and there is only information about one operator in the file
	//This is a sintactical error
	else
	{
		operationsFile.close();
		return	SINTACTICAL_ERROR;
	}

}

//prints all the operations, read from the operationsFile
void printOperationsInfo(const int  operationsListSize, Operation *operationsList)
{
	for (int i = 0; i < operationsListSize; ++i)
	{
		cout << "symbol= " << operationsList[i].symbol << ' '
			<< "operator= " << operationsList[i].actualOperatoration << ' '
			<< "associativity= " << operationsList[i].associativity << endl;
	}
}


//if everything is ok this function calculates a prefix expression
//and converts it to postfix expression;
//else returns a function result indicating some error
FunctionResult prefixNotationCalculatorAndConverter
(const char* nameOfPrefixNotationFile, const char* nameOfOperationsFile)
{

	//a list of all valid operations
	Operation *operationsList = NULL;

	//the number of all valid operations
	int operationsListSize;

	
	//this will be the return value of the function
	FunctionResult functionResult;

	//if the function executes normally, operationsList starts pointing 
	//to a dynamic memory
	functionResult =
		InitializeOperationsList(operationsListSize, operationsList, nameOfOperationsFile);

	if (functionResult != NORMAL)
	{
		cerr << "problem with " << nameOfOperationsFile << '\n';
		return functionResult;
	}
	
	functionResult =
		openAndCaltulatePrefixNotationFile(operationsListSize, operationsList, nameOfPrefixNotationFile);

	//we don't need operationsList anymore, so we can
	//free the dynamic memory it points to
	delete[] operationsList;

	if (functionResult != NORMAL)
	{
		cerr << "problem with " << nameOfPrefixNotationFile << '\n';
	}

	return functionResult;
}


//prints the result of the function int the standart i=output stream
//if everything is ok;
//else prints an error message in the error stream
void printFunctionResult(const FunctionResult functionResult)
{
	switch (functionResult)
	{
	case NORMAL: cout << "Normal excecution\n"; break;

	case UNABLE_TO_OPEN_FILE: cerr << "File could not be opened\n"; break;

	case FILE_IS_EMPTY: cerr << "The file is empty\n"; break;

	case SINTACTICAL_ERROR: cerr << "There's a sintactic error\n"; break;

	case UNKNOWN_ERROR: cerr << "There's an unknown error\n"; break;

	case LOGICAL_ERROR: cerr << "There's a logical error\n"; break;

	default: cerr << "This message must not occur\n";
	};
}

//tests the methods of class Stack<T>
void stackTest()
{
	//default ctor
	Stack<int> stack1;

	//getSize()
	cout << "size of stack1 = " << stack1.getSize() << endl;

	//getCapacity()
	cout << "capacity of stack1 = " << stack1.getCapacity() << endl;

	//push
	for (int i = 0; i < Stack<int>::defaultCapacity; ++i)
		stack1.push(i);

	//isFull()
	cout << "stack1 is " << (stack1.isFull() ? "" : "not ") << "full\n";

	cout << "size of stack1 = " << stack1.getSize() << endl;
	cout << "capacity of stack1 = " << stack1.getCapacity() << endl;

	//isEmpty
	cout << "stack1:\n";
	while (!stack1.isEmpty())
	{
		//pop()
		cout << stack1.pop() << ' ';
	}
	cout << endl;

	cout << "size of stack1 = " << stack1.getSize() << endl;
	cout << "capacity of stack1 = " << stack1.getCapacity() << endl;

	for (int i = 0; i < 2*Stack<int>::defaultCapacity; ++i)
		stack1.push(i);

	cout << "size of stack1 = " << stack1.getSize() << endl;
	cout << "capacity of stack1 = " << stack1.getCapacity() << endl;


	cout << "size of stack1 = " << stack1.getSize() << endl;
	cout << "capacity of stack1 = " << stack1.getCapacity() << endl;

	//copy ctor
	Stack<int>stack2(stack1);
	cout << "stack2:\n";
	while (!stack2.isEmpty())
	{
		cout << stack2.pop() << ' ';
	}
	cout << endl;

	cout << "size of stack2 = " << stack2.getSize() << endl;
	cout << "capacity of stack2= " << stack2.getCapacity() << endl;

	//operator=
	cout << "stack2=stack1\n";
	stack2 = stack1;
	cout << "size of stack2= " << stack2.getSize() << endl;
	cout << "capacity of stack2= " << stack2.getCapacity() << endl;


	cout << "stack2:\n";
	while (!stack2.isEmpty())
	{
		cout << stack2.pop() << ' ';
	}
	cout << endl;

	cout << "size of stack2= " << stack2.getSize() << endl;
	cout << "capacity of stack2= " << stack2.getCapacity() << endl;

}

//tests the methods of class String
void stringTest()
{
	String str1("abcdefg",6);
	cout << "size of str1 = " << str1.getSize() << endl;
	str1.print();

	String str3(str1);

	String str2("12345",4);
	cout << "size of str2 = " << str2.getSize() << endl;
	str2.print();

	str1 = str2;

	str1.print();

	cout << "size of str3 = " << str3.getSize() << endl;
	str3.print();

	concatenateTwoStringsAndOperationWithSpaces(str1, str3, '3').print();


}

int main(int argc, char* argv[])
{
	//the first argument of argv is always the name of the current file
	//that's why if we pass two arguments, the value of argc must be 3
	if (argc < 3)
	{
		cerr << "Not enough command arguments have been passed\n";
		return 1;
	}


		FunctionResult functionResult =
		prefixNotationCalculatorAndConverter(argv[1], argv[2]);

		if (functionResult!=NORMAL)
		printFunctionResult(functionResult);

		return 0;
}

