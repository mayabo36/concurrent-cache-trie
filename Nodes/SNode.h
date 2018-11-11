#include <typeinfo>
#include <string>
#include <cstdlib>
#include <atomic>
#ifndef SNODE_H_GUARD
#define SNODE_H_GUARD

enum Txn { NoTxn, FSNode, FVNode};

struct SNode {
	std::size_t hash;
	std::string value;

	SNode();
 };

#endif