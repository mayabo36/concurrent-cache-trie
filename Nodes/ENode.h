#include "ANode.h"
#ifndef ENODE_H_GUARD
#define ENODE_H_GUARD

struct ENode {
	ANode parent;
    int parentPos;
    ANode narrow;
    int hash;
    int level;
    ANode wide;

    ENode();
};

#endif