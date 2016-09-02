// PointerCOR.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;

struct Creature
{
	string name;
	int attack, defence;

	Creature(const string& name, int attack, int defence)
		: name(name),
		  attack(attack),
		  defence(defence)
	{
	}

	friend std::ostream& operator<<(std::ostream& os, const Creature& obj)
	{
		return os
			<< "name: " << obj.name
			<< " attack: " << obj.attack
			<< " defence: " << obj.defence;
	}
};

class CreatureModifier
{
	CreatureModifier* next = nullptr;
protected:
	Creature& creature;
public:

	explicit CreatureModifier(Creature& creature)
		: creature(creature)
	{
	}

	virtual ~CreatureModifier() = default;

	void add(CreatureModifier* cm)
	{
		if (next) next->add(cm);
		else next = cm;
	}

	virtual void handle()
	{
		if (next) next->handle();
	}
};

class DoubleAttackModifier : public CreatureModifier
{
public:
	explicit DoubleAttackModifier(Creature& creature)
		: CreatureModifier(creature)
	{
	}

	void handle() override
	{
		creature.attack *= 2;
		CreatureModifier::handle();
	}
};

class IncreaseDefenceModifier : public CreatureModifier
{
public:

	explicit IncreaseDefenceModifier(Creature& creature)
		: CreatureModifier(creature)
	{
	}

	void handle() override
	{
		if (creature.attack <= 2)
			creature.defence++;
		CreatureModifier::handle();
	}
};

class NoBonusModifier : public CreatureModifier
{
public:
	explicit NoBonusModifier(Creature& creature)
		: CreatureModifier(creature)
	{
	}

	void handle() override
	{
	}
};

int main()
{
	Creature goblin{ "Goblin",1,1 };
	CreatureModifier root{ goblin };
	DoubleAttackModifier r1{ goblin };
	DoubleAttackModifier r1_2{ goblin };
	IncreaseDefenceModifier r2{ goblin };

	NoBonusModifier no{ goblin };

	//root.add(&no);

	root.add(&r1);
	root.add(&r1_2);
	root.add(&r2);

	root.handle();

	cout << goblin << endl;

	getchar();
    return 0;
}

