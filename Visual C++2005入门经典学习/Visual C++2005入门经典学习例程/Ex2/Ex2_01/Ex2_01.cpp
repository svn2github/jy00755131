/********************************
*	Ex2_01.cpp					*
*	A Simple Program Example	*
********************************/
#include <iostream>
using std::cout;
using std::endl;
int main()
{
	int apples, oranges;
	int fruit;
	apples = 5;
	oranges = 6;
	fruit = apples + oranges;
	cout << "Oranges are not the only fruit..." << endl
		<< "- and we have " << fruit << " fruits in all.";
	cout << endl;
	return 0;
}