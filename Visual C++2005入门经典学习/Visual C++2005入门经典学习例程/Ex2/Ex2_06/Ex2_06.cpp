/************************************
*	Ex2_06.cpp						*
*	Exercising the comma operator	*
************************************/
#include <iostream>
using std::cout;
using std::endl;
int main()
{
	long num1 = 0, num2 = 0, num3 = 0, num4 = 0;
	num4 = (num1 = 10, num2 = 20, num3 = 30);
	cout << endl
		<< "The value of a series of expressions "
		<< "is the value of the rightmost: "
		<< num4;
	cout << endl;
	return 0;
}