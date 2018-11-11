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
	//Txn txn = NoTxn;
	std::atomic<Txn> txn {NoTxn};

	SNode();
	//SNode(const SNode& node);
	//SNode(std::size_t hash, std::string value, Txn txn);

	//void swap(SNode& first, SNode& second);

	// SNode& operator = (SNode other) {
	// 	swap(*this, other);
	// 	return *this;
	// };

 };

#endif