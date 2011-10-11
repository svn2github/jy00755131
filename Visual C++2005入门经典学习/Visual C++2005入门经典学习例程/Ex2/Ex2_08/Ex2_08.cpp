/************************************
*	Ex2_08.cpp						*
*	Demonstrating variable scope	*
************************************/
#include <iostream>
using std::cout;
using std::endl;

int count1 = 100;
int main()
{
	int count1 = 10;
	int count3 = 50;
	cout << endl
		<<"Value of outer count1 = " << count1
		<< endl;
	cout << "Value of global count1 = " << ::count1
		<<endl;
	{
		int count1 = 20;
		int count2 = 30;
		cout << "Value of inner count1 = " << count1
			<< endl;
		cout << "Value of global count1 = " << ::count1
			<<endl;
		count1 += 3;
		count3 += count2;
	}
	cout << "Value of outer count1 = " << count1
		<< endl
		<< "Value of outer count3 = " << count3
		<< endl;
	return 0;
}