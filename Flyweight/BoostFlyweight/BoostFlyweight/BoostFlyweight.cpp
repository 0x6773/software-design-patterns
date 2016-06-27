// BoostFlyweight.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
using namespace std;
using namespace boost;

struct User
{
	flyweight<string> first_name, last_name;

	User(const string& first_name, const string& last_name)
		: first_name(first_name),
		  last_name(last_name)
	{
	}

	friend std::ostream& operator<<(std::ostream& os, const User& obj)
	{
		return os
			<< "first_name: " << obj.first_name
			<< " last_name: " << obj.last_name;
	}
};

int main()
{
	User john_doe{ "john", "doe" };
	User jane_doe{ "jane", "doe" };

	cout << "john_doe : " << john_doe << endl;
	cout << "jane_doe : " << jane_doe << endl;

	cout << boolalpha
		<< (&john_doe.last_name.get() == &jane_doe.last_name.get());


	_getch();
    return 0;
}

