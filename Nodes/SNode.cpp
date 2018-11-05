#include <iostream>
#include <typeinfo>
#include <string>
#include <cstdlib>
#include "SNode.h"

SNode::SNode() {
    hash = 0;
    //key = 0;
    txn = NoTxn;
}