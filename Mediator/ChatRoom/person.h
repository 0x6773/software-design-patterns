#ifndef __PERSON_H__
#define __PERSON_H__

struct person;

#include <string>
#include <iostream>
#include <vector>
#include "chatroom.h"

struct person
{
	std::string name = { };
	chatroom* room = nullptr;
	std::vector<std::string> chat_log = { };

	explicit person(const std::string& _name) : name(_name) { }
	void say(const std::string&) const;
	void receive(const std::string&, const std::string&);
	void pm(const std::string&, const std::string&);
};



#endif

