/************************************
*	Ex2_09.cpp						*
*	Demonstrating namespace names	*
************************************/
#include <iostream>
int value = 0;
int main()
{
	std::cout << "enter an integer: ";
	std::cin >> value;
	std::cout << "\nYou entered " << value
		<< std::endl;
	return 0;
}