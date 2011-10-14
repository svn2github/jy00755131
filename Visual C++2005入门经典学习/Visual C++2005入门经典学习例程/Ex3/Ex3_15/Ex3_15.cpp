// Ex3_15.cpp : main project file.

#include "stdafx.h"

using namespace System;

int main(array<System::String ^> ^args)
{
	wchar_t letter = 0;
	Console::Write(L"Enter a letter: ");
	letter = Console::Read();
	if (letter >= 'A')
		if (letter <= 'Z')
		{
			Console::WriteLine(L"You entered a capital letter.");
			return 0;
		}
		if (letter >= 'a')
			if (letter <= 'z')
			{
				Console::WriteLine(L"You entered a small letter.");
				return 0;
			}
    Console::WriteLine(L"You did not enter a letter.");
    return 0;
}
