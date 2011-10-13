/****************************************
*	Ex3_10.cpp							*
*	Using an infinite for loop to 		*
*	compute an average					*
****************************************/
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

int main()
{
	double value = 0.0;
	double sum = 0.0;
	int i = 0;
	char indicator = 'n';
	for (;;)
	{
		cout << endl
			<< "Enter a value: ";
		cin >> value;
		++i;
		sum += value;
		cout << endl
			<< "Do you want to enter another value (enter n to end)?";
		cin >> indicator;
		if ((indicator == 'n') || (indicator == 'N'))
			break;
	}
	cout << endl
		<< "The average of the " << i
		<< " values you entered is " << sum/i << "."
		<< endl;
	return 0;
}