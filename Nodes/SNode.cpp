#include <iostream>
#include <typeinfo>
#include <string>
#include <cstdlib>
#include <atomic>
#include "SNode.h"

SNode::SNode() {
    SNode::hash = 0;
    SNode::value = "";
    //SNode::txn = NoTxn;
}

void SNode::swap(SNode& first, SNode& second) {
    std::swap(first.hash, second.hash);
    std::swap(first.value, second.value);
    std::swap(first.txn, second.txn);
}