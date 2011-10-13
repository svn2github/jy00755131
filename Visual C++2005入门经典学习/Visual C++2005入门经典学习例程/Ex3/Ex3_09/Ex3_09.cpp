/****************************************
*	Ex3_09.cpp							*
*	Using multiple conters to show		*
*	powers of 2							*
****************************************/
#include <iostream>
#include <iomanip>

using std::cin;
using std::cout;
using std::endl;
using std::setw;

int main()
{
	long i = 0, power = 0;
	const int max = 10;
	for(i = 0, power = 1; i <= max; i++, power += power)
		cout << endl
			<< setw(10) << i << setw(10) << power;
	cout << endl;
	return 0;
}