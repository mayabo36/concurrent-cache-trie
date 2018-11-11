#include "ANode.h"
#include "SNode.h"
#include "ENode.h"
#ifndef ANYNODE_H_GUARD
#define ANYNODE_H_GUARD

enum NodeType {ANODE, SNODE, ENODE};

struct AnyNode {
	ANode anode;
	SNode snode;
	ENode enode;
	NodeType nodeType;

	AnyNode();
	void sayHi();
};

#endif