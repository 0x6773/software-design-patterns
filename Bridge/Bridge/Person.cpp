#include "stdafx.h"
#include "Person.h"
#include <iostream>

struct Person::PersonImpl
{
	static void greet(Person*);
};

void Person::PersonImpl::greet(Person* p)
{
	std::cout << "Hello my name is " << p->name << std::endl;
}


Person::Person()
	: name()
	,impl(std::make_unique<Person::PersonImpl>())
{
}


Person::~Person()
{
	
}

void Person::greet()
{
	impl->greet(this);
}
