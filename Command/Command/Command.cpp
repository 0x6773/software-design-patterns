// Command.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;

struct BankAccount
{
	int balance = 0;
	int overdraft_limit = -500;
	
	void deposit(int amount)
	{
		balance += amount;
		cout << "deposited : " << amount << ", balance now : " << balance << endl;
	}

	void withdraw(int amount)
	{
		if (balance - amount >= overdraft_limit)
		{
			balance -= amount;
			cout << "withdrew : " << amount << ", balance now : " << balance << endl;
		}
	}
};

struct ICommand
{
	virtual ~ICommand() = default;
	virtual void call() const = 0;
	virtual void undo() const = 0;
};

struct Command : ICommand
{
	BankAccount& account;
	enum class Action { deposit, withdraw } action;
	int amount;

	Command(BankAccount& account, Action action, int amount)
		: account(account),
		  action(action),
		  amount(amount)
	{
	}

	void call() const override
	{
		switch (action)
		{
		case Action::deposit: 
			account.deposit(amount);
			break;
		case Action::withdraw: 
			account.withdraw(amount);
			break;
		default: break;
		}
	}

	void undo() const override
	{
		switch (action)
		{
		case Action::withdraw:
			account.deposit(amount);
			break;
		case Action::deposit:
			account.withdraw(amount);
			break;
		default: break;
		}
	}
};

struct CommandList : vector<Command>, ICommand
{
	CommandList(const ::std::initializer_list<value_type>& _Ilist)
		: vector<Command>(_Ilist)
	{
	}

	void call() const override
	{
		for (auto& cmd : *this)
			cmd.call();
	}

	void undo() const override
	{
		for_each(rbegin(), rend(),
			[](const Command& cmd) {cmd.undo(); });
	}
};

int main()
{
	BankAccount ba;
	CommandList commands{
		Command{ ba, Command::Action::deposit, 100 },
		Command{ ba, Command::Action::withdraw, 200 }
	};

	cout << ba.balance << endl;

	commands.call();

	cout << ba.balance << endl;

	commands.undo();

	cout << ba.balance << endl;

	_getch();
    return 0;
}

