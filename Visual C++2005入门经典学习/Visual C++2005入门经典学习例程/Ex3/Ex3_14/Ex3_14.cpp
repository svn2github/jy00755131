/****************************************
*	Ex3_14.cpp							*
*	Using nested loops to generate a	*
*	multiplication tables				*
****************************************/
#include <iostream>
#include <iomanip>
using std::setw;
using std::cout;
using std::endl;
int main()
{
	const int size = 12;
	int i = 0, j = 0;
	cout << endl
		<< size << " by " << size
		<< " Mutiplication Table" << endl << endl;
	cout << endl << "    |";
	for (i = 1; i <= size; i++)
		cout << setw(3) << i << " ";
	cout << endl;
	for (i = 0; i <= size; i++)
		cout << "_____";
	for (i = 1; i <= size; i++)
	{
		cout << endl
			<< setw(3) << i << " |";

		for (j = 1; j <= size; j++)
			cout << setw(3) << i*j << " ";
	}
	cout << endl;
	return 0;
}