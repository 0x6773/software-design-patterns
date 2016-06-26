// NeuralNetworks.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
using namespace std;

struct Neuron
{
	vector<Neuron*> in, out;
	int id;
	Neuron()
	{
		static int id = 1;
		this->id = id++;
	}
	Neuron* begin() { return this; }
	Neuron* end() { return this + 1; }
	void connect_to(Neuron& other)
	{
		out.push_back(&other);
		other.in.push_back(this);
	}
	template<typename T> void connect_to(T& other)
	{
		for (Neuron& target : other)
			connect_to(target);
	}
	friend std::ostream& operator<<(std::ostream& os, const Neuron& obj)
	{
		for(Neuron* n : obj.in)
		{
			os << n->id << "\t-->\t[" << obj.id << "]" << endl;
		}
		for(Neuron* n : obj.out)
		{ 
			os << "[" << obj.id << "]\t-->\t" << n->id << endl;
		}
		return os;
	}
};

struct NeuronLayer : vector<Neuron>
{
	NeuronLayer(int count)
	{
		while(count-- > 0)
		{
			emplace_back(Neuron{});
		}
	}

	friend std::ostream& operator<<(std::ostream& os, const NeuronLayer& obj)
	{
		for (auto& n : obj) os << n;
		return os;
	}

	template<typename T> void connect_to(T& other)
	{
		for (Neuron& from : *this)
			for (Neuron& to : other)
				from.connect_to(to);
	}
};

int main()
{
	Neuron n1, n2;
	n1.connect_to(n2);

	cout << n1 << n2 << endl;

	NeuronLayer l1{ 5 };
	Neuron n3;
	l1.connect_to(n3);

	cout << "Neuron " << n3.id << endl << n3 << endl;
	cout << "Layer " << endl << l1 << endl;
	
	NeuronLayer l2{ 2 }, l3{ 3 };
	l2.connect_to(l3);
	cout << "Layer 2 " << endl << l2 << endl;
	cout << "Layer 3 " << endl << l3 << endl;


	_getch();
    return 0;
}

