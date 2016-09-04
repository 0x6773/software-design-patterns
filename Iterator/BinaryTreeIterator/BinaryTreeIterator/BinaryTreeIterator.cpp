// BinaryTreeIterator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;

template<typename T> struct BinaryTree;

template<typename T> struct Node
{
	T value = T();
	Node<T>* left = nullptr;
	Node<T>* right = nullptr;
	Node<T>* parent = nullptr;
	BinaryTree<T>* tree = nullptr;

	explicit Node(const T& value)
		: value(value)
	{
	}

	Node(const T& value, Node<T>* left, Node<T>* right)
		: value(value),
		  left(left),
		  right(right)
	{
		this->left->parent = this->right->parent = this;
	}

	void set_tree(BinaryTree<T>* t)
	{
		tree = t;
		if (left)
			left->set_tree(t);
		if (right)
			right->set_tree(t);
	}

	~Node()
	{
		if (left) delete left;
		if (right) delete right;
	}
};

template<typename T> struct BinaryTree	
{
	Node<T>* root = nullptr;

	template<typename U>
	struct BinaryTreeIterator
	{
		Node<U>* current;

		explicit BinaryTreeIterator(Node<U>* current)
			: current(current)
		{
		}

		bool operator!=(const BinaryTreeIterator<U>& other)
		{
			return current != other.current;
		}

		Node<U>& operator*() { return *current; }

		BinaryTreeIterator<U>& operator++()
		{
			if(current->right)
			{
				current = current->right;
				while (current->left)
					current = current->left;
			}
			else
			{
				Node<T>* p = current->parent;
				while(p && current == p->right)
				{
					current = p;
					p = p->parent;
				}
				current = p;
			}
			return *this;
		}
	};
	typedef BinaryTreeIterator<T> iterator;

	explicit BinaryTree(Node<T>* root)
		: root(root)
	{
		root->set_tree(this);
	}

	~BinaryTree()
	{
		if (root)
			delete root;
	}

	iterator begin()
	{
		Node<T>* n = root;
		if (n)
			while (n->left)
				n = n->left;
		return iterator{ n };
	}

	iterator end()
	{
		return iterator{ nullptr };
	}
};

int main()
{
	BinaryTree<string> family{
		// ReSharper disable CppNonReclaimedResourceAcquisition
		new Node<string>{"me",
			new Node<string>{"mother",
				new Node<string>{ "mother's mother" },
				new Node<string>{ "mother's father" }
			},
		new Node<string>{ "father" },
		}
		// ReSharper restore CppNonReclaimedResourceAcquisition
	};

	for(auto it = family.begin(); it != family.end(); ++it)
		cout << (*it).value << endl;

	_getch();

    return 0;
}
