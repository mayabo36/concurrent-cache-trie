/**
    Simple test trie for words. Feel free to play around with this.
**/

#include <cstddef>
#include <string>
#include <functional>
#include <iostream>
#include <atomic> 
#include "../Nodes/ANode.h"
#include "../Nodes/AnyNode.h"
#include "../Nodes/SNode.h"

// Cache-trie global variables
ANode * root;


// Key is the word being inserted
bool insert(const std::string value, std::size_t hash, int level, ANode *& current, ANode *& previous) {

    int position;
    if (current->isWide) position = hash % 16;
    else position = hash % 4;

    AnyNode* old = current->wide[position];

    // Check if the location is empty, if so insert the node there
    if (old == 0) {

        // Setup node to be inserted
        SNode snode;
        snode.hash = hash;
        snode.value = value;

        AnyNode* newNode = new AnyNode;
        newNode->snode = snode;
        newNode->isSNode = true;

        if (current->wide[position].compare_exchange_weak(old, newNode)) return true;

        else return insert(value, hash, level, current, previous);
    }

    // Check if the location is occupied by an ANode
    else if (!old->isSNode) {

    }

    // If the posistion is occupied by an SNode
    else if (old->isSNode) {
        Txn txn = old->snode.txn;

        if (txn == NoTxn) {
            if (old->snode.value == value) {
                SNode snode;
                snode.hash = hash;
                snode.value = value;

                AnyNode* newNode = new AnyNode;
                newNode->snode = snode;
                newNode->isSNode = true;

                // Make Txn in snode atomic
                // CAS on the txn to announce the snode
                // then CAS on the AnyNode?

                //if (current->wide[position].compare_exchange_weak(old, newNode)) return true;
            }
        }
        return true;
    }

    // Check if location contains a reference to an ANode or if it is open or occupied by an SNode
    // If there is an ANode reference, then we must traverse deeper into the trie

    // If not, then we either have an open or occupied location 

    // if occupied, then we must do some extra checks

    // if the keys match, then we can just update the value in the SNode

    // if the keys do not match, and the ANode is narrow then we must expand the ANode
    // after expanding we can insert the key-value pair

    // if the keys do not match, and the ANode is wide then we create a new level in the trie
    // this is done by inserting an ANode reference at location and adding the key-value pair at this new level
    return false;
}

bool insert(const std::string value){
    std::cout << "performing an insert..." << std::endl;

    ANode* node = NULL;

    if ( !insert(value, std::hash<std::string>{}(value), 0, root, node) )
        insert(value);
}

//bool search(ANode root, std::string key) {
//
//}

int main() {
    std::string values[] = {"melissa", "emily", "ashton", "rebeca"};
    int n = sizeof(values) / sizeof(values[0]);

    root = new ANode;

    for(int i = 0; i < n; i++) {
        std::cout << "inserting a value" << std::endl;
        insert( values[i]);
    }

    ANode* tempRoot = root;
    for(int i = 0; i < 16; i++) {
        AnyNode* node = tempRoot->wide[i];
        if(node != 0){
            if(node->isSNode) {
                std::cout << node->snode.value << std::endl;
            }
        }
    }

    return 0;
}