// HomeMadeFlyweight.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
using namespace std;
using namespace boost;

using key = uint32_t;

struct User
{

	User(const string& first_name, const string& last_name)
		: first_name(add(first_name)),
		  last_name(add(last_name))
	{
	}

	const string& get_first_name() const
	{
		return names.left.find(first_name)->second;
	}

	const string& get_last_name() const
	{
		return names.left.find(last_name)->second;
	}


	friend std::ostream& operator<<(std::ostream& os, const User& obj)
	{
		return os
			<< "first_name: " << obj.first_name << " " << obj.get_first_name()
			<< " last_name: " << obj.last_name << " " << obj.get_last_name();
	}

protected:
	static bimap<key, string> names;
	static int seed;

	static key add(const string& s)
	{
		auto it = names.right.find(s);
		if(it == names.right.end())
		{
			key id = ++seed;
			names.insert(bimap<key, string>::value_type(seed, s));
			return id;
		}
		return it->second;
	}

	key first_name, last_name;
};
int User::seed = 0;
bimap<key, string> User::names{};

int main()
{
	User john_doe{ "john", "doe" };
	User jane_doe{ "jane", "doe" };

	cout << "john_doe : " << john_doe << endl;
	cout << "jane_doe : " << jane_doe << endl;

	_getch();
	return 0;
}
