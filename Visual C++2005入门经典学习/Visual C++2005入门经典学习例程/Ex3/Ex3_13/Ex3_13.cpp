/****************************************
*	Ex3_13.cpp							*
*	Demonstrating nested loops to		*
*	compute factorials					*
****************************************/
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

int main()
{
	char indicator = 'n';
	long value = 0, factorial = 0;
	do 
	{
		cout << endl
			<< "Enter an integer value: ";
		cin >> value;
		factorial = 1;
		for (int i = 2; i <= value; i++)
			factorial *= i;
		cout << "Factorial " << value << " is " << factorial;
		cout << endl
			<< "Do you want to enter another value (y or n)?";
		cin >> indicator;
	} while ((indicator == 'Y') || (indicator == 'y'));

	return 0;
}