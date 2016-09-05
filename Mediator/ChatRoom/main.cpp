#include <iostream>
#include <string>
#include "chatroom.h"
#include "person.h"

int main()
{
	chatroom room;

	auto john = room.join(person{ "john" });
	auto jane = room.join(person{ "jane" });

	john->say("hi room");
	jane->say("oh, hey john");

	auto simon = room.join(person{ "simon" });
	simon->say("hi everyone!");

	jane->pm("simon", "glad you could join us, simon");
	return 0;
}
