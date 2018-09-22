#include "pch.h"
#include <iostream>

// Base class
class AnyNode
{
	public:
		SNode snode;
		ANode anode;
		FNode fnode;
		ENode enode;

		AnyNode()
		{

		}
};

class SNode : public AnyNode
{

};

class FNode : public AnyNode
{

};

class ANode : public AnyNode
{
	public:
		AnyNode *arr[16];
};

class ENode : public AnyNode
{
	public:
		ANode parent;
		int parentPos;
		ANode narrow;
		int hash;
		int level;
		ANode wide;
};


int main()
{

	return 0;
}

