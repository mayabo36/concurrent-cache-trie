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
AnyNode * root;

// Declare functions
bool insert(std::string value, std::size_t hash, int level, AnyNode *& current, AnyNode *& previous);
bool insert(std::string value);
void completeExpansion(AnyNode *& enode);
void copyToWide(AnyNode *& node);
void freeze(AnyNode *& current);
std::string lookup(std::size_t hash, int level, AnyNode *& current);
void printTree(ANode* anode);


// Key is the word being inserted
bool insert(std::string value, std::size_t hash, int level, AnyNode *& current, AnyNode *& previous) {
    
    int position = (hash >> (level)) & ((current->anode.isWide ? 16 : 4) - 1);

    std::cout << "performing an insert on '" << value << "'" << "at pos " << position << std::endl;

    AnyNode* old;
    if (current->anode.isWide) old = current->anode.wide[position];
    else old = current->anode.narrow[position];

    // Check if the position is empty, if so insert the node there
    if (old == 0) {

        AnyNode* newNode = new AnyNode;
        newNode->snode.hash = hash;
        newNode->snode.value = value;
        newNode->nodeType = SNODE;

        if(current->anode.isWide){
            if (current->anode.wide[position].compare_exchange_weak(old, newNode)) return true;
            else return insert(value, hash, level, current, previous);
        } else {
            if (current->anode.narrow[position].compare_exchange_weak(old, newNode)) return true;
            else return insert(value, hash, level, current, previous);
        }

    }

    // Check if the position is occupied by an ANode
    else if (old->nodeType == ANODE) {
        //ANode* oldANode = &old->anode;
        return insert(value, hash, level + 4, old, current); // May break everything, needs testing
    }

    // If the posistion is occupied by an SNode
    else if (old->nodeType == SNODE) {
        Txn txn = old->txn;

        if (txn == NoTxn) {
            if (old->snode.value == value) {

                AnyNode* newNode = new AnyNode;
                newNode->snode.hash = hash;
                newNode->snode.value = value;
                newNode->nodeType = SNODE;
                
                if (newNode->txn.compare_exchange_weak(txn, txn)){ // Making sure NoTxn
                    current->anode.wide[position].compare_exchange_weak(old, newNode);
                    return true;
                }
                else {
                    return insert(value, hash, level, current, previous);
                }
            } else if (!current->anode.isWide) {
                int previousPos = (hash >> (level - 4)) & ((previous->anode.isWide ? 16 : 4) - 1);

                AnyNode* newNode = new AnyNode;
                newNode->enode.parentPos = previousPos;
                newNode->enode.hash = hash;
                newNode->enode.level = level;
                newNode->enode.parent = previous;
                newNode->enode.narrow = current;
                newNode->nodeType = ENODE;

                AnyNode* parentANode = previous->anode.wide[previousPos];
                if (previous->anode.wide[previousPos].compare_exchange_weak(parentANode, newNode)) {
                    completeExpansion(newNode);
                    AnyNode* wide = newNode->enode.parent->anode.wide[newNode->enode.parentPos];
                    return insert(value, hash, level, wide, previous);
                }
                else {
                    return insert(value, hash, level, current, previous);
                }
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

                if (old->txn.compare_exchange_weak(txn, txn)) {
                    if (current->anode.isWide) {
                        current->anode.wide[position].compare_exchange_weak(old, newNode);
                    } else {
                        current->anode.narrow[position].compare_exchange_weak(old, newNode);
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
        completeExpansion(old);
    }

    return false;
}

bool insert(std::string value){
    AnyNode* node = NULL;

    if ( !insert(value, std::hash<std::string>{}(value), 0, root, node) )
        insert(value);
}

void completeExpansion(AnyNode *& enode) {
    freeze(enode->enode.narrow);

    copyToWide(enode->enode.narrow);

    enode->enode.parent->anode.wide[enode->enode.parentPos].compare_exchange_weak(enode, enode->enode.narrow);

}

void copyToWide(AnyNode *& node) {
    
    int pos;
    AnyNode* temp = new AnyNode;

    for (int i = 0; i < 4; i++){
        AnyNode* curr = node->anode.narrow[i];
        if (curr != 0) {
             switch(curr->nodeType) {
                case SNODE:
                    // recalculate position and insert to wide
                    pos = (curr->snode.hash >> (node->anode.level)) & (16- 1);
                    temp = node->anode.wide[pos];
                    node->anode.wide[pos].compare_exchange_weak(temp, curr);
                    break;
                case ANODE:
                    std::cout << "anode found in copy..uh oh" << std::endl;
                    break; // hmm
            }           
        
        }
    }

    node->anode.isWide = true;
}

void freeze(AnyNode *& current) {
    int i  = 0;
    int length = ((current->anode.isWide) ? 16 : 4);

    while (i < length) {
        AnyNode* node = (current->anode.isWide ? current->anode.wide[i] : current->anode.narrow[i]);

        if (node == 0) {
            Txn oldTxn = node->txn;
            if (!node->txn.compare_exchange_weak(oldTxn, FVNode)) i--;
        }
        else if (node->nodeType == SNODE) {
             Txn oldTxn = node->txn;
             if (oldTxn == NoTxn) {
                 if (!node->txn.compare_exchange_weak(oldTxn, FSNode)) i--;
             }
             else if (oldTxn != FSNode) {
                Txn oldTxn = node->txn;
                // commit the pending changes ?
                i--;
             }
        }
        else if (node->nodeType == ANODE) {
            AnyNode* newNode;
            newNode->nodeType = FNODE;
            if (current->anode.isWide) {
                current->anode.wide[i].compare_exchange_weak(node, newNode);
            }
        }
        else if (node->nodeType == ENODE) {
            completeExpansion(node);
            i--;
        }
        i++;
    }
}

std::string lookup(std::size_t hash, int level, AnyNode *& current) {
    int position = (hash >> (level)) & ((current->anode.isWide ? 16 : 4) - 1);

    AnyNode* old = (current->anode.isWide ? current->anode.wide[position] : current->anode.narrow[position]);
    Txn txn = old->txn;

    if (old == 0 || txn == FVNode) {
        return NULL;
    }
    else if (old->nodeType == ANODE) {
        return lookup(hash, level + 4, old);
    }
    else if (old->nodeType == SNODE) {
        if (old->snode.hash == hash) {
            return old->snode.value;
        }
        else return NULL;
    }
    else if (old->nodeType == ENODE) {
        AnyNode* an = old->enode.narrow;
        return lookup(hash, level + 4, an);
    }
    else if (old->nodeType == FNODE) {
        return lookup(hash, level + 4, old->fnode.frozen);
    }
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

    root = new AnyNode;
    root->anode.isWide = true;

    for(int i = 0; i < n; i++) {
        //insert(std::rand());
        insert(values[i]);
    }
    
    //copyToWide(root);

    ANode* tempRoot = &root->anode;
    printTree(tempRoot);

    return 0;
}