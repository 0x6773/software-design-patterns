#pragma once
#include <memory>
#include <string>

struct Person
{
	std::string name;
	Person();
	~Person();
	void greet();
private:
	struct PersonImpl;
	std::unique_ptr<PersonImpl> impl;
};

