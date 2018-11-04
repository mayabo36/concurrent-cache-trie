#include "ANode.h"
#include "SNode.h"
#ifndef ANYNODE_H_GUARD
#define ANYNODE_H_GUARD

struct AnyNode {
	ANode anode;
	SNode snode;

	void sayHi();
};

#endif