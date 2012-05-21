/************************************************
*	Ex4_10.cpp									*
*	Counting string characters using a pointer  *
************************************************/
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
int main()
{
	const int MAX = 80;
	char buffer[MAX];
	char* pbuffer = buffer;
	cout << endl
		<< "Enter a string of less than "
		<< MAX << " characters:"
		<< endl;
	cin.getline(buffer, MAX, '\n');
	while(*pbuffer)
	{
		pbuffer++;
	}
	cout << endl
		<< "The string \"" << buffer << "\" has " << pbuffer - buffer << " characters.";
	cout << endl;
	return 0;
}