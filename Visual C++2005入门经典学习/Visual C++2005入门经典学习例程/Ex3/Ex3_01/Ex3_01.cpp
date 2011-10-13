/************************************
*	Ex3_01.cpp						*
*	A nested if demonstration		*
************************************/
#include <iostream>
using std::cout;
using std::cin;
using std::endl;
int main()
{
	char letter = 0;
	cout << endl
		<< "Enter a letter: ";
	cin >> letter;
	if (letter >= 'A')
		if (letter <= 'Z')
		{
			cout << endl
				<< "You entered a capital letter."
				<< endl;
//			letter += 'a' - 'A';
			return 0;
		}
	if (letter >= 'a')
		if (letter <= 'z')
		{
			cout << endl
				<< "You entered a small letter."
				<< endl;
			return 0;
		}
	cout << endl << "You did not enter a letter." << endl;
	return 0;
}