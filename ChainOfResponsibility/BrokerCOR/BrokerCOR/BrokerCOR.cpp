// BrokerCOR.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;
using namespace boost::signals2;

struct Query
{
	string creature_name;
	enum class Argument { attack, defense } argument;
	int result;

	Query(const string& creature_name, Argument argument, int result)
		: creature_name(creature_name),
		  argument(argument),
		  result(result)
	{
	}
};

struct Game
{
	signal<void(Query&)> queries;
};

struct Creature
{
private:
	Game& game;
	int attack, defense;
public:
	string name;

	Creature(Game& game, int attack, int defense, const string& name)
		: game(game),
		  attack(attack),
		  defense(defense),
		  name(name)
	{
	}

	int GetAttack() const
	{
		Query q{ name, Query::Argument::attack, attack };
		game.queries(q);
		return q.result;
	}

	friend std::ostream& operator<<(std::ostream& os, const Creature& obj)
	{
		return os
			<< "attack: " << obj.GetAttack()
			<< " defense: " << obj.defense
			<< " name: " << obj.name;
	}
};

class CreatureModifier
{
	Game& game;
	Creature& creature;
public:

	CreatureModifier(Game& game, Creature& creature)
		: game(game),
		  creature(creature)
	{
	}

	virtual ~CreatureModifier() = default;
};

class DoubleAttackModifier : public CreatureModifier
{
	connection conn;
public:
	DoubleAttackModifier(Game& game, Creature& creature)
		: CreatureModifier(game, creature)
	{
		cout << "Connecting" << endl;
		conn = game.queries.connect([&](Query& q)
		{
			if(q.creature_name == creature.name && q.argument == Query::Argument::attack)
			{
				q.result *= 2;
			}
		});
	}

	virtual ~DoubleAttackModifier()
	{
		cout << "Disconnecting" << endl;
		conn.disconnect();
	}
};

int main()
{
	Game game;
	Creature goblin{ game, 1,1,"Strong Goblin" };

	cout << goblin << endl;

	{
		DoubleAttackModifier dam{ game, goblin };
		cout << goblin << endl;
	}

	cout << goblin << endl;

	_getch();
    return 0;
}

