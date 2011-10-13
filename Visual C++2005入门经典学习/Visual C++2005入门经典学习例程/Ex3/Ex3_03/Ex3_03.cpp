/************************************
*	Ex3_03.cpp						*
*	Testing for a letter using		*
*	logical operators				*
************************************/
#include <iostream>
using std::cout;
using std::cin;
using std::endl;
int main()
{
	char letter = 0;
	cout << endl
		<< "Enter a character: ";
	cin >> letter;
	if (((letter >= 'A') && (letter <= 'Z'))||
		((letter >= 'a') && (letter <= 'z')))
		cout << endl
			<< "You entered a letter." << endl;
	else
		cout << endl
			<< "You didn't entered a letter." << endl;
	return 0;
}