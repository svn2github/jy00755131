// Ex3_16.cpp : main project file.

#include "stdafx.h"

using namespace System;

int main(array<System::String ^> ^args)
{
    Console::WriteLine(L"Press a key combination - press Escape to quit.");

	ConsoleKeyInfo keypress;
	do 
	{
		keypress = Console::ReadKey(true);
		Console::Write(L"You pressed");
		if (safe_cast<int>(keypress.Modifiers) > 0)
			Console::Write(L" {0},", keypress.Modifiers);
		Console::WriteLine(L" {0} which is the {1} character", keypress.Key, Char::ToUpper(keypress.KeyChar));
	} while (keypress.Key != ConsoleKey::Escape);

    return 0;
}
