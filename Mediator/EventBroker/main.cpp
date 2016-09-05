#include <iostream>
#include <string>
#include <vector>
using namespace std;

#include <boost/signals2.hpp>
using namespace boost::signals2;

struct Event
{
	virtual ~Event() = default;
	virtual void print() const = 0;
};

struct Player;

struct PlayerScored : Event
{
	string player_name;
	int goals_scored_so_far;

	PlayerScored(const string& player_name_, const int goals_scored_so_far_)
		: player_name(player_name_), goals_scored_so_far(goals_scored_so_far_)
	{ }

	virtual void print() const override
	{
		cout << player_name << " has scored! (their " << goals_scored_so_far << " goal)" << "\n";
	}
};

struct Game // event bus/broker
{
	signal<void(Event*)> events= { }; // observer pattern
};

struct Player 
{
	string name;
	int goals_scored = 0;
	Game& game;

	Player(const string& name_, Game& game_)
		: name(name_), game(game_)
	{ } 

	void score()
	{
		goals_scored++;
		PlayerScored ps{ name, goals_scored };
		game.events(&ps);
	}
};

struct Coach
{
	Game& game;
	explicit Coach(Game& game_)
		: game(game_)
	{
		game.events.connect([](Event* e)
				{
					PlayerScored* ps = dynamic_cast<PlayerScored*>(e);
					if(ps && ps->goals_scored_so_far < 3)
					{
						cout << "Coach says Well Done! : " << ps->player_name << "\n";
					}
				});
	}
};


int main()
{
	Game game;
	Player player{ "Govind", game };
	Coach coach{ game };

	player.score();
	player.score();
	player.score();

	return 0;
}

