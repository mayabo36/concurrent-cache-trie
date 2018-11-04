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
void insert(ANode *root, std::string value) {

    // check if we can insert or not
    // Operations.insert()

    std::size_t hash = std::hash<std::string>{}(value);
    //std::cout << "The hash of " << value << " is " << hash << "\n";

    // we always start at the root node, which is wide
    // Calculate the hash to find corresponding ANode location
    int location = hash % 16;
    //std::cout << "Location is " << location << "\n";

    // Check if location contains a reference to an ANode or if it is open or occupied by an SNode
    // If there is an ANode reference, then we must traverse deeper into the trie

    // If not, then we either have an open or occupied location

    // if open insert an SNode here
    // For now, we immediately insert an SNode for the key in root at location
    AnyNode an;
    SNode sn;
    sn.hash = hash;
    sn.value = value;

    an.snode = sn;
    root->wide[location] = an; // THIS LINE SEG FAULTS :(

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

    ANode *an;
    // SNode *sn;
    // sn->value = "test";
    // an.wide[0] = &sn;
    // SNode sn2;
    // sn2.value = "test 2";
    // an.wide[1] = &sn;

    for(int i = 0; i < n; i++) {
        std::cout << "inserting " << values[i] << std::endl;

        // this may not work bc c++ does not pass by reference? need to change to ptr?
        insert(an, values[i]);

        // SNode sn;
        // sn.hash = 123;
        // sn.value = values[i];
        // an->wide[i] = &sn;
    }

    std::cout << "test" << std::endl;

    for(int i = 0; i < 16; i++) {
      an->wide[i].sayHi();
    }

    return 0;
}