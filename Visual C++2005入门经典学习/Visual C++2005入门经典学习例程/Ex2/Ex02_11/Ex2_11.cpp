/************************************
*	Ex2_11.cpp						*
*	Using a using directive			*
************************************/
#include <iostream>
namespace myStuff
{
	int value = 0;
}
using namespace myStuff;
int main()
{
	std::cout << "enter an integer: ";
	std::cin >> value;
	std::cout << "\nYou entered " << value
		<< std::endl;
	return 0;
}