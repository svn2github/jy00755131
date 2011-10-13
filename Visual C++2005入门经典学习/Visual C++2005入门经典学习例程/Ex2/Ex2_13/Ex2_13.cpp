// Ex2_13.cpp : main project file.

#include "stdafx.h"

using namespace System;

int main(array<System::String ^> ^args)
{
	double carpetPriceSqYd = 27.95;
	double roomWidth = 13.5;
	double roomLength = 24.75;
	const int feetPerYard = 3;
	double roomWidthYds = roomWidth / feetPerYard;
	double roomLengthYds = roomLength / feetPerYard;
	double carpetPrice = roomLengthYds * roomWidthYds * carpetPriceSqYd;
	
	Console::WriteLine(L"Room size is {0:F2} yards by {1:F2} yards", roomLengthYds, roomWidthYds);
	Console::WriteLine(L"Room area is {0:F2} square yards", roomLengthYds * roomWidthYds);
	Console::WriteLine(L"Carpet price is ${0:F2}", carpetPrice);
	String^ line = Console::ReadLine();
	Console::WriteLine(line);
	char ch = Console::Read();
	Console::WriteLine(ch);
	ConsoleKeyInfo keypress = Console::ReadKey(true);
	Console::WriteLine(L"The key press corresponds to the character: {0}", keypress.KeyChar);
    return 0;
}
