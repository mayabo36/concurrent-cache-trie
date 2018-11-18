#include "ANode.h"
#ifndef ENODE_H_GUARD
#define ENODE_H_GUARD

struct ENode {
	AnyNode* parent;
    int parentPos;
    AnyNode* narrow;
    int hash;
    int level;
    ANode wide;

    ENode();
};

#endif