#include <iostream>
#include <typeinfo>
#include <string>
#include <cstdlib>
#include "ANode.h"
#include "SNode.h"
#include "AnyNode.h"

ANode::ANode() {
    ANode::isWide = true;

    // for(int i = 0; i < 16; i++) {
    //     wide[i] = 0;
    // }
}