// UsingBoostSpirit.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "overmath.hpp"

using namespace std;

int main()
{
	wstring func =
		L"void func(x:f64, z:f32)"
		L"{ "
		L"x = 10;"
		L"y = 20;"
		//L"z = x + y;"
		L"}";
	cout << "Parsed of : " << endl;
	wcout << func << "\n\n";
	wcout << overmath::parse(begin(func), end(func));

	_getch();
    return 0;
}

