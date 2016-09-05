#ifndef __PERSON_CPP__
#define __PERSON_CPP__

#include "person.h"

void person::say(const std::string& msg) const
{
	room->broadcast(name, msg);
}

void person::receive(const std::string& origin, const std::string& message)
{
	std::string s{origin + ": \"" + message + "\""};
	std::cout << "[" << name << "'s chat session] " << s << "\n";
	chat_log.emplace_back(s);
}

void person::pm(const std::string& who, const std::string& message)
{
	room->message(name, who, message);
}

#endif
