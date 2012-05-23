/********************************************************
*	Ex5_01.cpp											*
*	Declaring, defining, and using a function			*
********************************************************/
#include <iostream>
using std::cout;
using std::endl;

double power(double x, int n);
int main(void)
{
	int index = 3;
	double x = 3.0;
	double y = 0.0;
	y = power(5.0, 3);
	cout << endl
		<< "5.0 cubed = " << y;
	cout << endl
		<< "3.0 cubed = " << power(3.0, index);
	x = power(x, static_cast<int>(power(2.0, 2)));
	cout << endl
		<< "x = " << x;
	cout << endl;
	return 0;
}

double power(double x, int n)
{
	double result = 1.0;
	for (int i = 1; i <= n; i++)
	{
		result *= x;
	}
	return result;
}