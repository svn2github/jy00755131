/****************************************
*	Ex3_08.cpp							*
*	Summing integers with a for loop	*
****************************************/
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

int main()
{
	int i = 0, sum = 0;
	const int max = 10;
	for(i = 1; i <= max; i++)
		sum += i;
	cout << endl
		<< "sum = " << sum
		<< endl
		<< "i = " << i
		<< endl;
	return 0;
}