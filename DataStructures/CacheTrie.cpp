/**
    Simple test trie for words. Feel free to play around with this.
**/

#include <cstddef>
#include <string>
#include <functional>
#include <iostream>
#include "../Nodes/ANode.h"
#include "../Nodes/AnyNode.h"
#include "../Nodes/SNode.h"


// Key is the word being inserted
void insert(ANode *&root, const std::string value) {

    ANode* tempRoot = root;

    // check if we can insert or not
    // Operations.insert()
    std::size_t hash = std::hash<std::string>{}(value);
    //std::cout << "The hash of " << value << " is " << hash << "\n";

    // we always start at the root node, which is wide
    // Calculate the hash to find corresponding ANode location
    int location = hash % 16;
    std::cout << "Location is " << location << "\n";

    // Check if location contains a reference to an ANode or if it is open or occupied by an SNode
    // If there is an ANode reference, then we must traverse deeper into the trie

    // If not, then we either have an open or occupied location

    // if open insert an SNode here
    // For now, we immediately insert an SNode for the key in root at location
    AnyNode* an = new AnyNode;
    SNode sn;
    sn.hash = hash;
    sn.value = value;

    an->snode = sn;
    an->isSNode = true;
    tempRoot->wide[location] = an; // THIS LINE SEG FAULTS :(

    // if occupied, then we must do some extra checks

    // if the keys match, then we can just update the value in the SNode

    // if the keys do not match, and the ANode is narrow then we must expand the ANode
    // after expanding we can insert the key-value pair

    // if the keys do not match, and the ANode is wide then we create a new level in the trie
    // this is done by inserting an ANode reference at location and adding the key-value pair at this new level

}

bool search(ANode root, std::string key) {

}

int main() {
    std::string values[] = {"melissa", "emily", "ashton", "rebeca"};
    int n = sizeof(values) / sizeof(values[0]);

    ANode* root = new ANode;

    for(int i = 0; i < n; i++) {
        std::cout << "inserting " << values[i] << std::endl;
        insert(root, values[i]);
    }

    for(int i = 0; i < 16; i++) {
        AnyNode* node = root->wide[i];
        std::cout << node->isSNode << std::endl;
        if(node->isSNode) {
            // Un-comment at own risk; spooky prints may occur
            std::cout << node->snode.value << std::endl;
        }
        root->wide[i]->sayHi();
    }

    return 0;
}