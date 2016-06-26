// NeuralNetworks.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
using namespace std;

struct GraphicObject
{
	virtual void draw() = 0;
	virtual ~GraphicObject() { }
};

struct Circle : GraphicObject
{
	void draw() override
	{
		cout << "Circle" << endl;
	}
};

struct Group  : GraphicObject
{
	string name;
	vector<GraphicObject*> objects;

	explicit Group(const string& name)
		: name(name)
	{
	}

	void draw() override
	{
		cout << "Group : " << name.c_str() << " contains: " << endl;
		for (auto&& o : objects)
			o->draw();
	}
	~Group() override { }
};

int main()
{
	Group root("root");

	Circle c1, c2;
	root.objects.push_back(&c1);

	Group subgroup("sub");
	subgroup.objects.push_back(&c2);

	root.objects.push_back(&subgroup);

	root.draw();

	_getch();
    return 0;
}

