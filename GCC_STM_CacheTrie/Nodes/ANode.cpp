#include <iostream>
#include <typeinfo>
#include <string>
#include <cstdlib>
#include "ANode.h"
#include "SNode.h"
#include "AnyNode.h"

ANode::ANode() {
    ANode::isWide = false;
    level = 0;
}