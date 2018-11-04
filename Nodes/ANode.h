#ifndef ANODE_H_GUARD
#define ANODE_H_GUARD

struct ANode {
	struct AnyNode* wide;
	struct AnyNode* narrow;
	bool isWide;

	ANode();
};

#endif