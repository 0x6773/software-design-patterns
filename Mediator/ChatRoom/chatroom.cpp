#ifndef __CHATROOM_CPP__
#define __CHATROOM_CPP__

#include "chatroom.h"

person* chatroom::personReference::operator->() const
{
	return &people[index];
}

void chatroom::broadcast(const std::string& origin, const std::string& message)
{
	for(auto& p : people)
		if(p.name != origin)
			p.receive(origin, message);
}

chatroom::personReference chatroom::join(person&& p)
{
	std::string join_msg = p.name + " joins the chat";
	broadcast("room", join_msg);

	p.room = this;
	people.emplace_back(p);
	return { people, people.size() - 1 };
}

void chatroom::message(const std::string& origin, const std::string& who, const std::string& message)
{
	auto target = std::find_if(begin(people), end(people),
			[&](const person& p) { return p.name == who; });
	if(target != end(people))
	{
		target->receive(origin, message);
	}
}

#endif
