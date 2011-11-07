/****************************************
*	Ex4_03.cpp							*
*	Counting string characters			*
****************************************/
#include <iostream>
using std::cin;
using std::cout;
using std::endl;

int main()
{
	const int MAX = 80;
	char buffer[MAX];
	int count = 0;

	cout << "Enter a string of less than 80 characters:\n";
	cin.getline(buffer, MAX, '\n');
	while(buffer[count] != '\0')
		count++;

	cout << endl
		<< "The sting \"" << buffer << "\" has "
		<< count << " characters.";

	cout << endl;
	return 0;
}