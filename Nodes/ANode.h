#include <vector>

#ifndef ANODE_H_GUARD
#define ANODE_H_GUARD

struct ANode {
	struct AnyNode* wide [16];
	struct AnyNode* narrow [4];
	bool isWide;

	ANode();
};

#endif