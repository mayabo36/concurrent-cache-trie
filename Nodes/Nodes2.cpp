#include "pch.h"
#include <iostream>

class SNode
{
	// Access specifier:
	public:
		int hash;
		int key;
		char value;
		//Txn txn;
};

class ANode
{
	public:
		void *arr[16];
};

class ENode 
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
	ANode node;
	SNode nodeLeaf;

	nodeLeaf.key = 3;
	node.arr[0] = &nodeLeaf;
	
	std::cout << node.arr[0] << std::endl;
	std::cout << &nodeLeaf << std::endl;

	return 0;
}

