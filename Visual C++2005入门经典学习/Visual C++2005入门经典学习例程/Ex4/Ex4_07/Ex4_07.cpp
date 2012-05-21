/****************************************
*	Ex4_07.cpp							*
*	Initializing pointers with strings  *
****************************************/
#include <iostream>
using std::cin;
using std::cout;
using std::endl;

int main()
{
	char* pstr[] = {"Robert Redford",
					"Hopalong Cassidy",
					"Lassie",
					"Slim Pickens",
					"Boris Karloff",
					"Oliver Hardy"
					};
	char* pstart = "Your lucky star is ";

	int dice = 0;

	cout << endl
		<< "Pick a lucky star!"
		<< "Enter a number between 1 and 6:";
	cin >> dice;
	cout << endl;
	if (dice >=1 && dice <=6)
	{
		cout << pstart << pstr[dice - 1];
	}
	else
	{
		cout << "Sorry, you haven't got a lucky star.";
	}
	cout << endl;
	return 0;
}