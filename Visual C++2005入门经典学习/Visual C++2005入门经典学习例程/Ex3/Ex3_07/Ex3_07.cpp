/************************************
*	Ex3_07.cpp						*
*	Creating a loop with an if and	*
*	a goto							*
************************************/
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

int main()
{
	int i = 0, sum = 0;
	const int max = 10;

	i = 1;
loop:
	sum += i;
	if(++i <= max)
		goto loop;
	cout << endl
		<< "sum = " << sum
		<< endl
		<< "i = " << i
		<< endl;
	return 0;
}