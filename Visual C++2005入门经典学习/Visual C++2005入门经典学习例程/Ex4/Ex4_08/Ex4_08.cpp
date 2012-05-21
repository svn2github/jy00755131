/********************************************
*	Ex4_08.cpp								*
*	Flexible array management using sizeof  *
********************************************/
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
	int count = (sizeof pstr) / (sizeof pstr[0]);
	cout << endl
		<< "Pick a lucky star!"
		<< "Enter a number between 1 and " << count << ": ";
	cin >> dice;
	cout << endl;
	if (dice >=1 && dice <=count)
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