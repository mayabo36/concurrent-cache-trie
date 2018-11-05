#include <typeinfo>
#include <string>
#include <cstdlib>
#ifndef SNODE_H_GUARD
#define SNODE_H_GUARD

enum Txn { NoTxn, FSNode, FVNode};

struct SNode {
	std::size_t hash;
	//int key;
	std::string value;
	Txn txn;

	SNode();
};

#endif