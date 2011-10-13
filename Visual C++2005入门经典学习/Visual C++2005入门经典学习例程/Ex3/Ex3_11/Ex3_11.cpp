/****************************************
*	Ex3_11.cpp							*
*	Display ASCII codes for alphabetic	*
*	characters							*
****************************************/
#include <iostream>
#include <iomanip>

using std::cout;
using std::endl;
using std::hex;
using std::dec;
using std::setw;

int main()
{
	for(char capital = 'A', small = 'a'; capital <= 'Z'; capital++, small++)
		cout << endl
			<< "\t" << capital
			<< hex << setw(10) << static_cast<int>(capital)
			<< dec << setw(10) << static_cast<int>(capital)
			<< "\t" << small
			<< hex << setw(10) << static_cast<int>(small)
			<< dec << setw(10) << static_cast<int>(small);
	cout << endl;
	return 0;
}