#include <iostream>
#include <string>
#include <vector>
#include <memory>
using namespace std;

class Memento
{
	int balance;
public:
	Memento(const int balance_)
		: balance(balance_)
	{ }
	friend class BankAccount;
	friend class BankAccount2;
};

class BankAccount
{
	int balance;
public:
	explicit BankAccount(const int balance_)
		: balance(balance_)
	{ }

	void restore(const Memento& m)
	{
		balance = m.balance;
	}

	Memento deposit(int amount)
	{
		balance += amount;
		return { balance };
	}

	friend ostream& operator<<(ostream& os, const BankAccount& obj)
	{
		return os << "balance : " << obj.balance;
	}
};

class BankAccount2
{
	int balance;
	vector<shared_ptr<Memento>> changes;
	int current = 0;
public:
	explicit BankAccount2(const int balance_)
		: balance(balance_), changes()
	{ 
		changes.emplace_back(make_shared<Memento>(balance));
		current = 1;
	}

	void restore(const shared_ptr<Memento>& m)
	{
		if(m)
		{
			balance = m->balance;
			changes.push_back(m);
			current = changes.size() - 1;
		}	
	}

	shared_ptr<Memento> deposit(int amount)
	{
		balance += amount;
		auto m = make_shared<Memento>(balance);
		changes.push_back(m);
		++current;
		return m;
	}

	shared_ptr<Memento> undo()
	{
		if(current > 0)
		{
			--current;
			auto m = changes[current];
			balance = m->balance;
			return m;
		}
		return {};
	}

	shared_ptr<Memento> redo()
	{
		if(current + 1u < changes.size())
		{
			++current;
			auto m = changes[current];
			balance = m->balance;
			return m;
		}
		return{};
	}

	friend ostream& operator<<(ostream& os, const BankAccount2& obj)
	{
		return os << "balance : " << obj.balance;
	}
};
int main()
{
	{
		BankAccount ba { 100 };
		auto m1 = ba.deposit(50);
		auto m2 = ba.deposit(25);
		cout << ba << "\n";

		ba.restore(m1);
		cout << ba << "\n";

		ba.restore(m2);
		cout << ba << "\n";
	}
	/* --------------------------------------------- */
	{
		cout << "Undo & Redo" << "\n";

		BankAccount2 ba { 100 };
		ba.deposit(50);
		ba.deposit(25);
		cout << ba << "\n";

		ba.undo();
		cout << ba << "\n";

		ba.undo();
		cout << ba << "\n";

		ba.redo();
		cout << ba << "\n";

	}

	return 0;
}
