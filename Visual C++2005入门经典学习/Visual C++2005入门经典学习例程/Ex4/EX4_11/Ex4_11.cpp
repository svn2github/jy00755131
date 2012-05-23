/********************************************************
*	Ex4_11.cpp											*
*	Calculating primes using dynamic memory allocation  *
********************************************************/
#include <iostream>
#include <iomanip>
using std::cin;
using std::cout;
using std::endl;
using std::setw;
int main()
{
	long* pprime = NULL;
	long trial = 5;
	int count = 3;
	int found = 0;
	int max = 0;
	cout << endl
		<< "Enter the number of primes you would like (at least 4):";
	cin >> max;
	if (max < 4)
	{
		max = 4;
	}
	pprime = new long[max];
	*pprime = 2;
	*(pprime + 1) = 3;
	*(pprime + 2) = 5;
	do 
	{
		trial += 2;
		found = 0;
		for (int i=0; i<count; i++)
		{
			found = (trial % *(pprime + i)) == 0;
			if (found)
			{
				break;
			}
		}
		if (found == 0)
		{
			*(pprime + count++) = trial;
		}
	} while (count < max);
	for (int i=0; i<max; i++)
	{
		if (i % 5 == 0)
		{
			cout << endl;
		}
		cout << setw(10) << *(pprime + i);
	}
	cout << endl;
	delete[] pprime;
	pprime = NULL;
	return 0;
}