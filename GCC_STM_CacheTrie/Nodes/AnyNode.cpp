#include <iostream>
#include <typeinfo>
#include <string>
#include "ANode.h"
#include "SNode.h"
#include "AnyNode.h"
#include "../DataStructures/CacheNode.h"

AnyNode::AnyNode() {
    AnyNode::nodeType = NodeType::SNODE;
    AnyNode::txn = NoTxn;
}

void AnyNode::sayHi() {
    std::cout << "HI" << std::endl;
}

