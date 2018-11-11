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

// SNode::SNode(std::size_t hash, std::string value, Txn txn) {
//     this->hash = hash;
//     this->value = value;
//     std::atomic<Txn> t {txn};
//     std::swap(this->txn, t);
// }

// SNode::SNode(const SNode & node) {
//     // this->hash = node.hash;
//     // this->value = node.value;
//     // this->txn = node.txn;
// }

// void SNode::swap(SNode& first, SNode& second) {
//     std::swap(first.hash, second.hash);
//     std::swap(first.value, second.value);
//     std::swap(first.txn, second.txn);
// }