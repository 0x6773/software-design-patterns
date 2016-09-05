#ifndef __CHATROOM_H__
#define __CHATROOM_H__

struct chatroom;

#include "person.h"
#include <vector>
#include <string>
#include <algorithm>

struct chatroom
{
	std::vector<person> people = { };

	class personReference
	{
		std::vector<person>& people;
		unsigned long index;
	public:
		personReference(std::vector<person>& persons, const unsigned long _index)
			: people(persons), index(_index)
		{ }

		person* operator->() const;
	};

	void broadcast(const std::string&, const std::string&);
	personReference join(person&&);	
	void message(const std::string&, const std::string&, const std::string&);
};

#endif
