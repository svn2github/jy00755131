/****************************************
*	Ex4_01.cpp							*
*	Calculating gas mileage				*
****************************************/
#include <iostream>
#include <iomanip>

using std::cout;
using std::cin;
using std::endl;
using std::setw;
int main()
{
	const int Max = 20;
	double gas[ Max ];
	long miles[ Max ];
	int count = 0;
	char indicator = 'y';
	while (((indicator == 'y') || (indicator == 'Y')) && count < Max)
	{
		cout << endl
			<< "Enter gas quantity: ";
		cin >> gas[count];
		cout << "Enter odometer reading: ";
		cin >> miles[count];

		++count;
		cout << "Do you want to enter another(y or n)?";
		cin >> indicator;
	}
	if (count <= 1)
	{
		cout << endl
			<< "Sorry - at least two readings are necessary.";
		return 0;
	}

	for (int i=1; i<count; i++)
		cout << endl
			<< setw(2) << i << "."
			<< "Gas purchased = " << gas[i] << " gallons"
			<< " resulted in "
			<< (miles[i] - miles[i - 1]) / gas[i] << " miles per gallon.";
	cout << endl;
	return 0;
}