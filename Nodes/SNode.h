#include <typeinfo>
#include <string>
#include <cstdlib>
#include <atomic>
#ifndef SNODE_H_GUARD
#define SNODE_H_GUARD

struct SNode {
	std::size_t hash;
	int value;

	SNode();
 };

#endif