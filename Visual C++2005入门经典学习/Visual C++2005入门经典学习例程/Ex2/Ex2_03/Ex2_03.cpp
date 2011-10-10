/********************************
*	Ex2_03.cpp					*
*	Exercising output			*
********************************/
#include <iostream>
#include <iomanip>
using std::cout;
using std::endl;
using std::setw;
int main()
{
	int num1 = 1234;
	int num2 = 5678;
	cout << endl;
	cout << setw(6) << num1 << setw(6) << num2 << endl;
	cout << endl;
	return 0;
}