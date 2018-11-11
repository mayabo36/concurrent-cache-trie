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
    
    int position = (hash >> (level)) & ((current->isWide ? 16 : 4) - 1);

    std::cout << "performing an insert on '" << value << "'" << "at pos " << position << std::endl;

    AnyNode* old;
    if (current->isWide) old = current->wide[position];
    else old = current->narrow[position];

    // Check if the position is empty, if so insert the node there
    if (old == 0) {

        AnyNode* newNode = new AnyNode;
        newNode->snode.hash = hash;
        newNode->snode.value = value;
        newNode->nodeType = SNODE;

        if(current->isWide){
            if (current->wide[position].compare_exchange_weak(old, newNode)) return true;
            else return insert(value, hash, level, current, previous);
        } else {
            if (current->narrow[position].compare_exchange_weak(old, newNode)) return true;
            else return insert(value, hash, level, current, previous);
        }

    }

    // Check if the position is occupied by an ANode
    else if (old->nodeType == ANODE) {
        ANode* oldANode = &old->anode;
        return insert(value, hash, level + 4, oldANode, current); // May break everything, needs testing
    }

    // If the posistion is occupied by an SNode
    else if (old->nodeType == SNODE) {
        Txn txn = old->snode.txn;

        if (txn == NoTxn) {
            if (old->snode.value == value) {

                AnyNode* newNode = new AnyNode;
                newNode->snode.hash = hash;
                newNode->snode.value = value;
                newNode->nodeType = SNODE;
                
                if (newNode->snode.txn.compare_exchange_weak(txn, txn)){ // Making sure NoTxn
                    current->wide[position].compare_exchange_weak(old, newNode);
                    return true;
                }
                else {
                    return insert(value, hash, level, current, previous);
                }
            } else if (!current->isWide) {
                int previousPos = (hash >> (level - 4)) & ((previous->isWide ? 16 : 4) - 1);

                AnyNode* newNode = new AnyNode;
                newNode->enode.parentPos = previousPos;
                newNode->enode.hash = hash;
                newNode->enode.level = level;
                newNode->enode.parent = previous;
                newNode->nodeType = ENODE;

                // AnyNode* parentANode = previous->wide[previousPos];
                // if (previous->wide[previousPos].compare_exchange_weak(&parentANode->anode, newNode)) {

                // }


            } else {
                AnyNode* newNode = new AnyNode;
                newNode->anode.level = level + 4;
                newNode->nodeType = ANODE;

                AnyNode* temp;

                // Insert previous snode into new anode
                AnyNode* snode1 = new AnyNode;
                snode1->snode.hash = old->snode.hash;
                snode1->snode.value = old->snode.value;
                snode1->nodeType = SNODE;
                newNode->anode.narrow[(snode1->snode.hash >> (newNode->anode.level)) & (4 - 1)].compare_exchange_weak(temp, snode1);

                // Insert new snode into new anode
                AnyNode* snode2 = new AnyNode;
                snode2->snode.hash = hash;
                snode2->snode.value = value;
                snode2->nodeType = SNODE;
                newNode->anode.narrow[(snode2->snode.hash >> (newNode->anode.level)) & (4 - 1)].compare_exchange_weak(temp, snode2);

                // int pos1 = (snode1->snode.hash >> (level + 4)) & (4 - 1);
                // int pos2 = (snode2->snode.hash >> (level + 4)) & (4 - 1);
                // std::cout << pos1 << " and " << pos2 << std::endl;

                if (old->snode.txn.compare_exchange_weak(txn, txn)) {
                    if (current->isWide) {
                        current->wide[position].compare_exchange_weak(old, newNode);
                    } else {
                        current->narrow[position].compare_exchange_weak(old, newNode);
                    }
                    return true;
                } else {
                    return insert(value, hash, level, current, previous);
                }
            }
        } 
        // SNode is frozen
        else if (txn == FSNode) {
            return false;
        }
        else { // Otherwise help finish the operation that another thread is performing and try again
            return insert(value, hash, level, current, previous);
        }
    }

    // Check if the position is occupied an ENode
    else if (old->nodeType == ENODE) {
        // Complete expansion

    }

    return false;
}

bool insert(std::string value){
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

void printTree(ANode* anode) {

    int length = (anode->isWide ? 16 : 4);

    for(int i = 0; i < length; i++) {

        AnyNode* node;
        if (anode->isWide) {node = anode->wide[i];} 
        else node = anode->narrow[i];

        if(node != 0){
            switch(node->nodeType) {
                case SNODE:
                    std::cout << node->snode.value << " at location " << i << std::endl;
                    break;
                case ANODE:
                    std::cout << "Traverse anode at " << i << std::endl;
                    printTree(&node->anode);
                    break;
            }
        }
    }
}

int main() {
    std::string values[] = {"melissa", "emily", "ashton", "rebeca", "damian", "victor", "tyler", "pichi", "pom", "neo", "precious"};
    int n = sizeof(values) / sizeof(values[0]);

    root = new ANode;
    root->isWide = true;

    for(int i = 0; i < n; i++) {
        //insert(std::rand());
        insert(values[i]);
    }

    ANode* tempRoot = root;
    printTree(tempRoot);

    return 0;
}