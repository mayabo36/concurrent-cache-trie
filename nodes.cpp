#include "pch.h"
#include <iostream>

class ANode
{
	void *array[16];
};

class ENode
{
	public:
		ANode parent;
		const int parentPos;
		ANode narrow;
		const int hash;
		const int level;
		ANode wide;
};

int main()
{

	return 0;
}

