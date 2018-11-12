#include "ANode.h"
#include "SNode.h"
#include "ENode.h"
#include "FNode.h"
#ifndef ANYNODE_H_GUARD
#define ANYNODE_H_GUARD

enum NodeType {ANODE, SNODE, ENODE, FNODE};
enum Txn { NoTxn, FSNode, FVNode};

struct AnyNode {
	ANode anode;
	SNode snode;
	ENode enode;
	FNode fnode;
	NodeType nodeType;
	std::atomic<Txn> txn {NoTxn};

	AnyNode();
	void sayHi();
};

#endif