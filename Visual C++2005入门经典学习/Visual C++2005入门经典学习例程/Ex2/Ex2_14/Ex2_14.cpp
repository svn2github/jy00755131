// Ex2_14.cpp : main project file.

#include "stdafx.h"

using namespace System;

enum class Suit : int{Clubs = 1, Diamonds = 1, Hearts, Spades};
enum Suit2 : int{Clubs = 4, Diamonds = 0, Hearts, Spades};
int main(array<System::String ^> ^args)
{
	Suit suit = Suit::Clubs;
	int value = safe_cast<int>(suit);
	Console::WriteLine(L"Suit is {0} and the value is {1} ", suit, value);
	suit = Suit::Diamonds;
	value = safe_cast<int>(suit);
	Console::WriteLine(L"Suit is {0} and the value is {1} ", suit, value);
	suit = Suit::Hearts;
	value = safe_cast<int>(suit);
	Console::WriteLine(L"Suit is {0} and the value is {1} ", suit, value);
	suit = Suit::Spades;
	value = safe_cast<int>(suit);
	Console::WriteLine(L"Suit is {0} and the value is {1} ", suit, value);

	Suit2 suit2 = Clubs;
	Console::WriteLine(suit2);
	suit2 = Diamonds;
	Console::WriteLine(suit2);
	suit2 = Hearts;
	Console::WriteLine(suit2);
	suit2 = Spades;
	Console::WriteLine(suit2);
    return 0;
}
