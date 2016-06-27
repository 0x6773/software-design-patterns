// NullObject.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
using namespace std;
using namespace boost;

struct Logger
{
	virtual ~Logger() = default;
	virtual void info(const string& s) = 0;
	virtual void warn(const string& s) = 0;
};

struct BankAccount
{
	std::shared_ptr<Logger> log;
	string name;
	int balance = 0;

	BankAccount(const std::shared_ptr<Logger>& logger, const string& name, int balance)
		: log(logger),
		  name(name),
		  balance(balance)
	{
	}

	void deposite(int amount)
	{
		balance += amount;
		log->info("Deposited $" + lexical_cast<string>(amount) + " to " +
			name + ", balance is now $" + lexical_cast<string>(balance));
	}

	void withdraw(int amount)
	{
		if(balance>=amount)
		{
			balance -= amount;
			log->info("Withdrew $" + lexical_cast<string>(amount) + " from " +
				name + ", balance is now $" + lexical_cast<string>(balance));
		}
		else
		{
			log->warn("Tried to withdraw $" + lexical_cast<string>(amount) + " from " +
				name + " but couldn't due to low balance.");
		}

	}
};

struct ConsoleLogger : Logger
{
	~ConsoleLogger() override { }
	void info(const string& s) override
	{
		cout << "INFO : " << s << endl;
	}
	void warn(const string& s) override
	{
		cout << "WARN! : " << s << endl;
	}
};

struct NullLogger : Logger
{
	~NullLogger() override { }
	void info(const string& s) override { }
	void warn(const string& s) override { }
};

int main()
{
	auto logger = make_shared<ConsoleLogger>();
	BankAccount ba{ logger, "primary account", 1000 };

	ba.deposite(2000);
	ba.withdraw(2500);
	ba.withdraw(1000);


	_getch();
    return 0;
}

