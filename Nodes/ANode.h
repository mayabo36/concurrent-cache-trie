#include <atomic> 

#ifndef ANODE_H_GUARD
#define ANODE_H_GUARD

struct ANode {
	std::atomic<struct AnyNode *> wide [16] = {};
	std::atomic<struct AnyNode *> narrow [4] = {};
	bool isWide;

	ANode();
};

#endif