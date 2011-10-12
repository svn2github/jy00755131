// Ex2_12.cpp : main project file.

#include "stdafx.h"

using namespace System;

int main(array<System::String ^> ^args)
{
	Int32 apples, oranges;
	Int32 fruit;
	apples = 5;
	oranges = 6;
	fruit = apples + oranges;
	Console::WriteLine(L"\nOranges are not the only fruit...");
	Console::Write(L"- and we have ");
	Console::Write(fruit);
    Console::Write(L" fruits in all.\n");

	int packageCount = 25;
	Console::Write(L"There are ");
	Console::Write(packageCount);
	Console::WriteLine(L" packages.");

	Console::WriteLine(L"There are {0} packages.", packageCount);

	double packageWeight = 7.5;
	Console::WriteLine(L"There are {0} packages weighting {1} pounds.", packageCount, packageWeight);

	Console::WriteLine(L"There are {1} packages weighting {0} pounds.", packageWeight, packageCount);

	Console::WriteLine(L"There are {0} packages weighting {1:F2} pounds.", packageCount, packageWeight);

	Console::WriteLine(L"There are {0, 3} packages weighting {1, 5:F2} pounds.", packageCount, packageWeight);
    
	Console::WriteLine(L"There are {0, -3} packages weighting {1, -5:F2} pounds.", packageCount, packageWeight);
	return 0;
}
