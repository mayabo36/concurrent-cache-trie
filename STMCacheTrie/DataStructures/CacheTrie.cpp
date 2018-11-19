/**
	Simple test trie for words. Feel free to play around with this.
**/

#include <cstddef>
#include <string>
#include <functional>
#include <iostream>
#include <unistd.h>
#include <../rstm/include/common/locks.hpp>
#include <../rstm/include/common/platform.hpp>
#include "CacheTrie.h"
#include "../Nodes/ANode.h"
#include "../Nodes/AnyNode.h"
#include "../Nodes/SNode.h"


CacheTrie::CacheTrie() {

	// instantiate root node to be a wide [16] array of anynodes
	CacheTrie::root = new AnyNode;
	root->anode.isWide = true;
	root->nodeType = ANODE;

	// Instantiate the cache head
	CacheTrie::cacheHead = NULL;

	//CacheTrie::num_threads = num_threads;
	CacheTrie::max_misses = 2048;
}

// Key is the word being inserted
bool CacheTrie::insert(int value, std::size_t hash, int level, AnyNode *& current, AnyNode *& previous) {

	int position = (hash >> (level)) & ((current->anode.isWide ? 16 : 4) - 1);

	AnyNode* old;
	if (current->anode.isWide) old = current->anode.wide[position];
	else old = current->anode.narrow[position];

	// Check if the position is empty, if so insert the node there
	if (old == 0) {

		AnyNode* newNode = new AnyNode;
		newNode->snode.hash = hash;
		newNode->snode.value = value;
		newNode->nodeType = SNODE;

		if (current->anode.isWide) {
			//if ( current->anode.wide[position].compare_exchange_weak(old, newNode)){
			BEGIN_TRANSACTION;
			current->anode.wide[position] = newNode;
			END_TRANSACTION;
			return true; 
            
			//else return insert(value, hash, level, current, previous);
		}
		else {
			//if (current->anode.narrow[position].compare_exchange_weak(old, newNode)){
			BEGIN_TRANSACTION;
			current->anode.narrow[position] = newNode;
			END_TRANSACTION;
			return true; 
			//else return insert(value, hash, level, current, previous);
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
				
				BEGIN_TRANSACTION;
					newNode->txn = txn;
					ON_RETRY {
						return insert(value, hash, level, current, previous);
					}
					current->anode.wide[position] = newNode;	
				END_TRANSACTION;
				return true;

				// if (newNode->txn.compare_exchange_weak(txn, txn)) { // Making sure NoTxn
				// 	current->anode.wide[position].compare_exchange_weak(old, newNode);
				// 	return true;
				// }
				// else {
				// 	return insert(value, hash, level, current, previous);
				// }
			}
			else if (!current->anode.isWide) {
				int previousPos = (hash >> (level - 4)) & ((previous->anode.isWide ? 16 : 4) - 1);

				AnyNode* newNode = new AnyNode;
				newNode->enode.parentPos = previousPos;
				newNode->enode.hash = hash;
				newNode->enode.level = level;
				newNode->enode.parent = previous;
				newNode->enode.narrow = current;
				newNode->nodeType = ENODE;

				AnyNode* parentANode = previous->anode.wide[previousPos];

				BEGIN_TRANSACTION;
					previous->anode.wide[previousPos] = newNode;
					ON_RETRY{
						return insert(value, hash, level, current, previous);
					}
				END_TRANSACTION;

				completeExpansion(newNode);
				AnyNode* wide = newNode->enode.parent->anode.wide[newNode->enode.parentPos];
				return insert(value, hash, level, wide, previous);				

				// if (previous->anode.wide[previousPos].compare_exchange_weak(parentANode, newNode)) {
				// 	completeExpansion(newNode);
				// 	AnyNode* wide = newNode->enode.parent->anode.wide[newNode->enode.parentPos];
				// 	return insert(value, hash, level, wide, previous);
				// }
				// else {
				// 	return insert(value, hash, level, current, previous);
				// }
			}
			else {
				AnyNode* newNode = new AnyNode;
				newNode->anode.level = level + 4;
				newNode->nodeType = ANODE;

				//AnyNode* temp;

				// Insert previous snode into new anode
				AnyNode* snode1 = new AnyNode;
				snode1->snode.hash = old->snode.hash;
				snode1->snode.value = old->snode.value;
				snode1->nodeType = SNODE;

				BEGIN_TRANSACTION;
				newNode->anode.narrow[(snode1->snode.hash >> (newNode->anode.level)) & (4 - 1)] = snode1;
				END_TRANSACTION;
				//newNode->anode.narrow[(snode1->snode.hash >> (newNode->anode.level)) & (4 - 1)].compare_exchange_weak(temp, snode1);

				// Insert new snode into new anode
				AnyNode* snode2 = new AnyNode;
				snode2->snode.hash = hash;
				snode2->snode.value = value;
				snode2->nodeType = SNODE;

				BEGIN_TRANSACTION;
				newNode->anode.narrow[(snode2->snode.hash >> (newNode->anode.level)) & (4 - 1)] = snode2;
				END_TRANSACTION;
				//newNode->anode.narrow[(snode2->snode.hash >> (newNode->anode.level)) & (4 - 1)].compare_exchange_weak(temp, snode2);

				BEGIN_TRANSACTION;
					old->txn = txn;
					ON_RETRY {
						return insert(value, hash, level, current, previous);
					}
					if (current->anode.isWide) current->anode.wide[position] = newNode;
					else current->anode.narrow[position] = newNode;
				END_TRANSACTION;

				return true;

				// if (old->txn.compare_exchange_weak(txn, txn)) {
				// 	if (current->anode.isWide) {
				// 		current->anode.wide[position].compare_exchange_weak(old, newNode);
				// 	}
				// 	else {
				// 		current->anode.narrow[position].compare_exchange_weak(old, newNode);
				// 	}
				// 	return true;
				// }
				// else {
				// 	return insert(value, hash, level, current, previous);
				// }
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

bool CacheTrie::insert(int value) {
	AnyNode* node = NULL;

	if (!insert(value, std::hash<int>{}(value), 0, root, node))
		insert(value);


}

void *CacheTrie::testInsert(void) {

	for (int i = 1; i < 251; i++) {
		insert(i);
	}
}

void CacheTrie::completeExpansion(AnyNode *& enode) {
	
	freeze(enode->enode.narrow);

	copyToWide(enode->enode.narrow);

	BEGIN_TRANSACTION;
	enode->enode.parent->anode.wide[enode->enode.parentPos] = enode->enode.narrow;
	END_TRANSACTION;
	//enode->enode.parent->anode.wide[enode->enode.parentPos].compare_exchange_weak(enode, enode->enode.narrow);
}

void CacheTrie::copyToWide(AnyNode *& node) {

	int pos;
	Txn txn;
	AnyNode* temp = new AnyNode;

	for (int i = 0; i < 4; i++) {
		AnyNode* curr = node->anode.narrow[i];
		Txn txn = curr->txn;
		if (curr != 0 && txn != FVNode) {
			switch (curr->nodeType) {
			case SNODE:
				// recalculate position and insert to wide
				pos = (curr->snode.hash >> (node->anode.level)) & (16 - 1);
				temp = node->anode.wide[pos];
				txn = curr->txn;
				// Note: we need to change the txn value of the snode in the wide array to NoTxn after copying so other threads can modify it

				BEGIN_TRANSACTION;
				curr->txn = NoTxn;
				node->anode.wide[pos] = curr;
				END_TRANSACTION;
				// curr->txn.compare_exchange_weak(txn, NoTxn);
				// node->anode.wide[pos].compare_exchange_weak(temp, curr);
				break;
			case ANODE:
				std::cout << "anode found in copy..uh oh" << std::endl;
				break; // hmm
			}

		}
	}

	node->anode.isWide = true;
}

void CacheTrie::freeze(AnyNode *& current) {
	int i = 0;
	int length = ((current->anode.isWide) ? 16 : 4);

	while (i < length) {
		AnyNode* node = (current->anode.isWide ? current->anode.wide[i] : current->anode.narrow[i]);
		
		if (node == 0) {
			AnyNode* initNode = new AnyNode;

			BEGIN_TRANSACTION;
			if (current->anode.isWide) {
				current->anode.wide[i] = initNode;
				//current->anode.wide[i].compare_exchange_weak(node, initNode);
			} else{
				current->anode.narrow[i] = initNode;
				//current->anode.narrow[i].compare_exchange_weak(node, initNode);
			}
			END_TRANSACTION;

			node = (current->anode.isWide ? current->anode.wide[i] : current->anode.narrow[i]);

			Txn oldTxn = node->txn;

			BEGIN_TRANSACTION;
				node->txn = FVNode;
				ON_RETRY {
					i--;
				}
			END_TRANSACTION;
			// logic?????????????????????
			// if (!node->txn.compare_exchange_weak(oldTxn, FVNode))
			// 	i--;
		}
		else if (node->nodeType == SNODE) {
			Txn oldTxn = node->txn;
			if (oldTxn == NoTxn) {
				BEGIN_TRANSACTION;
					node->txn = FSNode;
					ON_RETRY {
						i--;
					}
				END_TRANSACTION;
				// ??
				//if (!node->txn.compare_exchange_weak(oldTxn, FSNode)) i--;
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
			newNode->fnode.frozen = node;
			if (current->anode.isWide) {
				BEGIN_TRANSACTION;
					current->anode.wide[i] = newNode;
				END_TRANSACTION;
				//current->anode.wide[i].compare_exchange_weak(node, newNode);
			}
		}
		else if (node->nodeType == FNODE) {
			freeze(node->fnode.frozen);
		}
		else if (node->nodeType == ENODE) {
			completeExpansion(node);
			i--;
		}
		i++;
	}
}

int CacheTrie::lookup(std::size_t hash, int level, AnyNode *& current) {
    
    int position = (hash >> (level)) & ((current->anode.isWide ? 16 : 4) - 1);

    AnyNode* old = (current->anode.isWide ? current->anode.wide[position] : current->anode.narrow[position]);

    Txn txn;
    if (old != 0) txn = old->txn;
    
    if (old == 0 || txn == FVNode) {
        return 0;
    }
    else if (old->nodeType == ANODE) {
        return lookup(hash, level + 4, old);
    }
    else if (old->nodeType == SNODE) {
        if (old->snode.hash == hash) {
            return old->snode.value;
        }
        else return 0;
    }
    else if (old->nodeType == ENODE) {
        AnyNode* an = old->enode.narrow;
        return lookup(hash, level + 4, an);
    }
    else if (old->nodeType == FNODE) {
        return lookup(hash, level + 4, old->fnode.frozen);
    }
}

int CacheTrie::lookup(int value) {
	return lookup(std::hash<int>{}(value), 0, root);
}

int CacheTrie::fastLookup(int value) {
	std::size_t hash = std::hash<int>{}(value);
	AnyNode** cache = cacheHead;

	// if we dont have a cache, perform a slow lookup
	if (cache == NULL) {
		return lookup(hash, 0, root); // include cache level later
	}

	int cacheLength = sizeof cache/sizeof cache[0];
	int topLevel = cacheLength - 1; // countTrailingZeros(cache.length - 1)

	while (cache != NULL) {
		int position = 1 + (hash & (cacheLength - 2));
		AnyNode* cachee = cache[position];
		int level = cacheLength - 1; // countTrailingZeros(cache.length - 1)

		if (cachee->nodeType == SNODE) {
			Txn txn = cachee->txn; // old.txn?

			if (txn == NoTxn) {
				if (cachee->snode.value == value) return cachee->snode.value;
				else return 0;
			}
		}
		else if (cachee->nodeType == ANODE) {
			int cacheeLength = sizeof cachee/sizeof cachee[0];
			int cachePosition = (hash >> level) & (cacheeLength - 1);

			AnyNode* old = &cachee[cachePosition]; // BREAK HERE??????????

			// Maybe do this if (old != 0)
			Txn txn = old->txn;

			if (txn == FVNode || old->nodeType == FNODE) continue;

			if (old->nodeType == SNODE) {
				if (txn == FSNode) continue;
			}

			return lookup(hash, level, cachee); // include cache level later
		}

		cache = &(cache[0]->cachenode).parent;
	}

	return lookup(hash, 0, root);
}

AnyNode** CacheTrie::createCache(int level, AnyNode* parent []) {
	AnyNode* cache [1 + (1 << level)];
	int misses [num_threads]; // num_threads * THROUGHPUT_FACTOR ??

	AnyNode* newCacheHead = new AnyNode;
	newCacheHead->nodeType = CACHENODE;
	newCacheHead->cachenode.misses = misses;
	newCacheHead->cachenode.parent = NULL;

	cache[0] = newCacheHead;

	AnyNode** cachePtr = cache;
	return cachePtr;
}

void CacheTrie::printTree(ANode* anode) {

	int length = (anode->isWide ? 16 : 4);

	for (int i = 0; i < length; i++) {

		AnyNode* node;
		if (anode->isWide) { node = anode->wide[i]; }
		else node = anode->narrow[i];

		if (node != 0) {
			switch (node->nodeType) {
			case SNODE:
				std::cout << node->snode.value << " at location " << i << std::endl;
				break;
			case ANODE:
				std::cout << "Traverse anode at " << i << " and level " << (node->anode.level + 4) << std::endl;
				printTree(&node->anode);
				std::cout << "End traversal anode at " << i << std::endl;
				break;
			}
		}
	}
}

void CacheTrie::inhabit(AnyNode** cache, AnyNode* newValue, std::size_t hash, int cacheeLevel) {
	if (cache == NULL) {
		if (cacheeLevel >= 12) {
			cache = createCache(8, NULL);
			cacheHead = cache;
			inhabit(cache, newValue, hash, cacheeLevel);
		}
	}
	else {
		int length = sizeof cache/sizeof cache[0];
		int cacheLevel = length - 1; // countTrailingZeros(length - 1)

		if (cacheLevel == cacheeLevel) {
			int position = 1 + (hash & (length - 2));
			cache[position] = newValue;
		}
	}
}

void CacheTrie::recordCacheMiss(int thread_id) {
	AnyNode** cache = cacheHead;

	if (cache != NULL) {
		CacheNode cn = cache[0]->cachenode;
		int missLength = sizeof cn.misses / sizeof cn.misses[0];
		int counterId = thread_id % missLength;
		
		int count = cn.misses[counterId];
		
		if (count > max_misses) {
			cn.misses[counterId] = 0;
			//sampleAndAdjustCache(cache);
		}
		else {
			cn.misses[counterId];
			count++;
		}
	}
}
