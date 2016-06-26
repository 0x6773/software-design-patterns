// Composite.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;
using namespace boost::accumulators;

struct Expression
{
	virtual ~Expression() { }
	virtual double eval() = 0;
	virtual void collect(vector<double>&) = 0;
};

struct Literal: Expression
{
	double value;
	explicit Literal(const double value)
		: value(value)
	{
	}

	~Literal() override { }
	double eval() override
	{
		return value;
	}
	void collect(vector<double>& v) override
	{
		v.push_back(value);
	}
};

struct AdditionExpression : Expression
{
	std::shared_ptr<Expression> left, right;
	~AdditionExpression() override { }

	AdditionExpression(const std::shared_ptr<Expression>& expression, const std::shared_ptr<Expression>& expression1)
		: left(expression),
		  right(expression1)
	{
	}

	double eval() override
	{
		return left->eval() + right->eval();
	}

	void collect(vector<double>& v) override
	{
		left->collect(v);
		right->collect(v);
	}
};

int main()
{
	AdditionExpression sum{
		make_shared<Literal>(2),
		make_shared<AdditionExpression>(
			make_shared<Literal>(3),
			make_shared<Literal>(4)
			)
	};
	cout << "2 + (3 + 4) = " << sum.eval() << endl;

	vector<double> v;
	sum.collect(v);

	for (auto val : v)
		cout << val << " ";
	cout << endl;

	vector<double> values{ 1,2,3,4 };

	accumulator_set<double, stats<tag::mean>> acc;
	for (auto x : values) acc(x);
	cout << "Average is : " << mean(acc) << endl;



	_getch();
	return 0;
}
