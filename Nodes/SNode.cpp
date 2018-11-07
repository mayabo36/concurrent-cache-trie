#include <iostream>
#include <typeinfo>
#include <string>
#include <cstdlib>
#include <atomic>
#include "SNode.h"

SNode::SNode() {
    SNode::hash = 0;
    SNode::value = "";
    SNode::txn = NoTxn;
}