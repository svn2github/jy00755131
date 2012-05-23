/********************************************************
*	Ex5_02.cpp											*
*	A futile attempt to modify caller arguments			*
********************************************************/
#include <iostream>
using std::cout;
using std::endl;

int incr10(int num);

int main(void)
{
	int num = 3;
	cout << endl
		<< "incr10(num) = " << incr10(num)
		<< endl
		<< "num = " << num;
	cout << endl;
	return 0;
}
int incr10(int num)
{
	num += 10;
	return num;
}