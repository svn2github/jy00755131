/********************************
*	Ex2_05.cpp					*
*	Calculating how many rolls	*
*	of wallpaper are required	*
*	for a room					*
********************************/
#include <iostream>
using std::cout;
using std::cin;
using std::endl;
int main()
{
	double height = 0.0, width = 0.0, length = 0.0;
	double perimeter = 0.0;
	const double rollwidth = 21.0;
	const double rolllength = 12.0 * 33.0;
	int strios_per_roll = 0;
	int strios_reqd = 0;
	int nrolls = 0;

	cout << endl
		<< "Enter the height of the room in inches:";
	cin >> height;

	cout << endl
		<< "Now enter the length and width in inches:";
	cin >> length >> width;

	strios_per_roll = rolllength / height;
	perimeter = 2.0 * (length + width);
	strios_reqd = perimeter / rollwidth;
	nrolls = strios_reqd / strios_per_roll;

	cout << endl
		<< "For your room you need " << nrolls << " rolls of wallpaper."
		<< endl;

	return 0;
}