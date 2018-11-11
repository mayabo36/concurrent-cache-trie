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
bool insert(std::string value, std::size_t hash, int level, ANode *& current, ANode *& previous) {

    int position;
    if (current->isWide) position = hash % 16;
    else position = hash % 4;

    AnyNode* old = current->wide[position];

    // Check if the location is empty, if so insert the node there
    if (old == 0) {

        // // Setup node to be inserted
        // SNode snode = SNode();
        // snode.hash = hash;
        // snode.value = value;

        AnyNode* newNode = new AnyNode;
        newNode->snode = SNode();
        newNode->snode.hash = hash;
        newNode->snode.value = value;
        newNode->nodeType = SNODE;

        if (current->wide[position].compare_exchange_weak(old, newNode)) return true;

        else return insert(value, hash, level, current, previous);
    }

    // Check if the location is occupied by an ANode
    else if (old->nodeType == ANODE) {
        ANode* oldANode = &old->anode;
        return insert(value, hash, level + 4, oldANode, current); // May break everything, needs testing
    }

    // If the posistion is occupied by an SNode
    else if (old->nodeType == SNODE) {
        Txn txn = old->snode.txn;

        if (txn == NoTxn) {
            if (old->snode.value == value) {
                // SNode snode = SNode();
                // snode.hash = hash;
                // snode.value = value;

                AnyNode* newNode = new AnyNode;
                newNode->snode = SNode();
                newNode->snode.hash = hash;
                newNode->snode.value = value;
                newNode->nodeType = SNODE;

                // Make Txn in snode atomic
                // CAS on the txn to announce the snode
                // then CAS on the AnyNode?

                //if (current->wide[position].compare_exchange_weak(old, newNode)) return true;
            }
        }
        return true;
    }

    // Check if the position is occupied an ENode
    else if (old->nodeType == ENODE) {
        // Complete expansion

    }

    return false;
}

bool insert(std::string value){
    std::cout << "performing an insert on '" << value << "'" << std::endl;

    ANode* node = NULL;

    if ( !insert(value, std::hash<std::string>{}(value), 0, root, node) )
        insert(value);
}

void completeExpansion(ENode *& en) {
    // ANode wide = ANode();
    // copy values from narrow to wide

    // CAS on en.wide, null, new wide ANode

    // CAS new wide anode where narrow anode was before
}

void freeze(ANode *& current) {
    // int i  = 0;
    // int length = ((current->isWide) ? 16 : 4);

    // while (i < length) {
        
    // }
}

bool search(ANode root, std::string key) {

}

int main() {
    std::string values[] = {"melissa", "emily", "ashton", "rebeca"};
    int n = sizeof(values) / sizeof(values[0]);

    root = new ANode;

    for(int i = 0; i < n; i++) {
        insert( values[i]);
    }

    ANode* tempRoot = root;
    for(int i = 0; i < 16; i++) {
        AnyNode* node = tempRoot->wide[i];
        if(node != 0){
            if(node->nodeType == SNODE) {
                std::cout << node->snode.value << " at location " << i << std::endl;
            }
        }
    }

    return 0;
}