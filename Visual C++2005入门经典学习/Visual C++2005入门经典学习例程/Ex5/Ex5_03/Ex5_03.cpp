/********************************************************
*	Ex5_03.cpp											*
*	A successful attempt to modify caller arguments		*
********************************************************/
#include <iostream>
using std::cout;
using std::endl;

int incr10(int* num);
int main(void)
{
	int num = 3;
	int* pnum = &num;
	cout << endl
		<< "Address passed = " << pnum;
	cout << endl
		<< "incr10(pnum) = " << incr10(pnum);
	cout << endl
		<< "num = " << num;
	cout << endl;
	return 0;
}
int incr10(int* num)
{
	cout << endl
		<< "Address received = " << num;
	*num += 10;
	return *num;
}