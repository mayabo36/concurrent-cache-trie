/**
	Simple test trie for words. Feel free to play around with this.
**/

#include <cstddef>
#include <string>
#include <functional>
#include <iostream>
#include <unistd.h>
#include <atomic> 
#include <chrono>
#include <thread>
#include "CacheTrie.h"
#include "../Nodes/ANode.h"
#include "../Nodes/AnyNode.h"
#include "../Nodes/SNode.h"

bool DEBUG = false;
int MAX_TRIES = 10;

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

	if (DEBUG) std::cout << "[0]attempting to insert " << value << " at " << position << " and level " << level << std::endl;

	AnyNode* old;
	if (current->anode.isWide) old = current->anode.wide[position];
	else old = current->anode.narrow[position];

	// Check if the position is empty, if so insert the node there
	if (old == 0) {

		AnyNode* newNode = new AnyNode;
		newNode->snode.hash = hash;
		newNode->snode.value = value;
		newNode->nodeType = SNODE;

		#ifdef _USE_TSX
		__transaction_atomic {
		#endif
			if (current->anode.isWide) {
				if (current->anode.wide[position] == old){
					if (DEBUG) std::cout << "[1]inserting " << value << " at wide " << position << " and level " << level << "\n" << std::endl;
					current->anode.wide[position] = newNode;
					return true;
				} 
			}
			else {
				if (current->anode.narrow[position] == old) {
					if (DEBUG) std::cout << "[2]inserting " << value << " at narrow " << position << " and level " << level << "\n" << std::endl;
					current->anode.narrow[position] = newNode;
					return true;
				} 
			}
		#ifdef _USE_TSX
		}
		#endif

		if (DEBUG) std::cout << "failed to insert at empty node" << std::endl;
		return insert (value, hash, level, current, previous);
	}

	// Check if the position is occupied by an ANode
	else if (old->nodeType == ANODE) {
		return insert(value, hash, level + 4, old, current);
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

				bool succ = false;

				#ifdef _USE_TSX
				__transaction_atomic {
				#endif					
					if (newNode->txn == txn) {
						if (current->anode.wide[position] == old){
							if (DEBUG) std::cout << "[3]inserting " << value << " at wide " << position << " and level " << level << "\n" << std::endl;
							current->anode.wide[position] = newNode;
							return true;
						}
						succ = true;
					}
				#ifdef _USE_TSX	
				}
				#endif

				if (!succ) {
					if (DEBUG) std::cout << "failed to update node" << std::endl;
					return insert(value, hash, level, current, previous);
				} 
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

				bool succ = false;

				#ifdef _USE_TSX
				__transaction_atomic {
				#endif
					if (previous->anode.wide[previousPos] == parentANode) {
						if (DEBUG) std::cout << "[4]inserting enode at wide " << previousPos << " and level " << (level - 4) << "\n" << std::endl;
						previous->anode.wide[previousPos] = newNode;
						succ = true;
					}
				#ifdef _USE_TSX
				}
				#endif

				if (succ) {
					completeExpansion(newNode);
					AnyNode* wide = newNode->enode.parent->anode.wide[newNode->enode.parentPos];
					return insert(value, hash, level, wide, previous);
				}
				else {
					if (DEBUG) std::cout << "failed to insert enode" << std::endl;
					return insert(value, hash, level, current, previous);
				}
			}
			else {
				AnyNode* newNode = new AnyNode;
				newNode->anode.level = level + 4;
				newNode->nodeType = ANODE;

				AnyNode* temp;

				// Insert previous snode into new anode
				AnyNode* snode1 = new AnyNode;
				snode1->snode.hash = old->snode.hash;
				snode1->snode.value = old->snode.value;
				snode1->nodeType = SNODE;
				int snode1Pos = (snode1->snode.hash >> (newNode->anode.level)) & (4 - 1);
				AnyNode* temp1 = newNode->anode.narrow[snode1Pos];

				AnyNode* t = NULL;

				if (insert(old->snode.value, old->snode.hash, newNode->anode.level, newNode, current)) {//newNode->anode.narrow[snode1Pos].compare_exchange_weak(temp1, snode1)){
					if (DEBUG) std::cout << "[5]inserting " << snode1->snode.value << " at narrow " << snode1Pos << " and level " << newNode->anode.level << "\n" << std::endl;
				}else {
					std::cout << "failure to insert " << snode1->snode.value << " at narrow " << snode1Pos << " and level " << newNode->anode.level << "\n" << std::endl;
				}

				// #ifdef _USE_TSX
				// __transaction_atomic {
				// #endif
				// 	if (newNode->anode.narrow[(snode1->snode.hash >> (newNode->anode.level)) & (4 - 1)] == temp) {
				// 		if (DEBUG) std::cout << "[5]inserting " << snode1->snode.value << " at narrow " << ((snode1->snode.hash >> (newNode->anode.level)) & (4 - 1)) << " and level " << newNode->anode.level << "\n" << std::endl;
				// 		newNode->anode.narrow[(snode1->snode.hash >> (newNode->anode.level)) & (4 - 1)] = snode1;
				// 	}
				// #ifdef _USE_TSX	
				// }
				// #endif

				// Insert new snode into new anode
				AnyNode* snode2 = new AnyNode;
				snode2->snode.hash = hash;
				snode2->snode.value = value;
				snode2->nodeType = SNODE;
				int snode2Pos = (snode2->snode.hash >> (newNode->anode.level)) & (4 - 1);
				AnyNode* temp2 = newNode->anode.narrow[snode2Pos];
				
				if (insert(value, hash, newNode->anode.level, newNode, current)){//newNode->anode.narrow[snode2Pos].compare_exchange_weak(temp2, snode2)){
					if (DEBUG) std::cout << "[5]inserting " << snode2->snode.value << " at narrow " << snode2Pos << " and level " << newNode->anode.level << "\n" << std::endl;
				}else {
					std::cout << "failure to insert " << snode2->snode.value << " at narrow " << snode2Pos << " and level " << newNode->anode.level << "\n" << std::endl;
				}

				// #ifdef _USE_TSX
				// __transaction_atomic {
				// #endif
				// 	if (newNode->anode.narrow[(snode2->snode.hash >> (newNode->anode.level)) & (4 - 1)] == temp) { 
				// 		if (DEBUG) std::cout << "[6]inserting " << snode2->snode.value << " at narrow " << ((snode2->snode.hash >> (newNode->anode.level)) & (4 - 1)) << " and level " << newNode->anode.level << "\n" << std::endl;
				// 		newNode->anode.narrow[(snode2->snode.hash >> (newNode->anode.level)) & (4 - 1)] = snode2;
				// 	}
				// #ifdef _USE_TSX	
				// }
				// #endif

				#ifdef _USE_TSX
				__transaction_atomic {
				#endif
					if (old->txn == txn) {
						if (current->anode.isWide && current->anode.wide[position] == old){
							if (DEBUG) std::cout << "[7]inserting anode at wide " << position << " and level " << level << "\n" << std::endl;
							current->anode.wide[position] = newNode;
						} 
						else if (!current->anode.isWide && current->anode.narrow[position] == old){
							if (DEBUG) std::cout << "[8]inserting anode at narrow " << position << " and level " << level << "\n" << std::endl;
							current->anode.narrow[position] = newNode;
						} 
						return true;
					}
				#ifdef _USE_TSX
				}
				#endif
				if (DEBUG) std::cout << "failed to add new narrow level" << std::endl;
				return insert(value, hash, level, current, previous);
			}
		}
		// SNode is frozen
		else if (txn == FSNode) {
			return false;
		}
		else { // Otherwise help finish the operation that another thread is performing and try again
			//std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ HERE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~`"  << txn <<  std::endl;
			//std::this_thread::sleep_for(std::chrono::milliseconds(100));
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
	int tryNum = 1;

	if (!insert(value, std::hash<int>{}(value), 0, root, node)){
		while (tryNum <= MAX_TRIES){
			if (insert(value)) return true;
			else {
				tryNum++;
			}
		}
		return false;
	}

	return true;
}

void CacheTrie::testInsert(int thread_id) {

	for (int i = (100 * thread_id); i <= ((100 * thread_id) + 50); i++) {
		insert(i);
	}

	// if (thread_id == 1) {
	// 	insert(16);
	// 	insert(32);
	// 	insert(48);
	// 	insert(64);
	// 	insert(80);
	// 	insert(96);
	// 	insert(112);
	// 	insert(128);
	// 	insert(144);
	// 	insert(160);
	// 	insert(176);
	// 	insert(192);
	// 	insert(208);
	// 	insert(224);
	// 	insert(240);
	// 	insert(256);
	// }

	// if (thread_id == 2) {
	// 	insert(512);
	// 	insert(768);
	// 	insert(1024);
	// 	insert(1280);
	// 	insert(1536);
	// 	insert(1792);
	// 	insert(2048);
	// 	insert(2304);
	// 	insert(2560);
	// 	insert(2816);
	// 	insert(3072);
	// 	insert(3328);
	// 	insert(3584);
	// 	insert(3840);
	// 	insert(4096);
	// 	insert(4352);
	// }
	
	// if (thread_id == 3) {
	// 	insert(8704);
	// 	insert(13056);
	// 	insert(17408);
	// 	insert(21760);
	// 	insert(26112);
	// 	insert(30468);
	// 	insert(34820);
	// 	insert(39172);
	// 	insert(43524);
	// 	insert(47876);
	// 	insert(52228);
	// 	insert(56580);
	// 	insert(60932);
	// 	insert(65284);
	// 	insert(69636);
	// 	insert(73988);
	// }	
}

void CacheTrie::completeExpansion(AnyNode *& enode) {
	
	freeze(enode->enode.narrow);

	copyToWide(enode->enode.narrow);

	#ifdef _USE_TSX
	__transaction_atomic {
	#endif
		if (enode->enode.parent->anode.wide[enode->enode.parentPos] == enode) {
			enode->enode.parent->anode.wide[enode->enode.parentPos] = enode->enode.narrow;
		}
	#ifdef _USE_TSX
	}
	#endif

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

				#ifdef _USE_TSX
				__transaction_atomic {
				#endif
					if (curr->txn == txn) curr->txn = NoTxn;
					if (node->anode.wide[pos] == temp){
						if (DEBUG) std::cout << "[9]inserting " << curr->snode.value << " at wide " << pos << " and level " << node->anode.level << "\n" << std::endl;
						node->anode.wide[pos] = curr;
					} else {
						std::cout << "failure to insert " << curr->snode.value << " at wide " << pos << " and level " << node->anode.level << "\n" << std::endl;
					}
				#ifdef _USE_TSX
				}
				#endif
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

			#ifdef _USE_TSX
			__transaction_atomic {
			#endif
				if (current->anode.isWide){
					if (current->anode.wide[i] == node) current->anode.wide[i] = initNode;
				}
				else {
					if (current->anode.narrow[i] == node) current->anode.narrow[i] = initNode;
				}
			#ifdef _USE_TSX
			}
			#endif

			// if (current->anode.isWide) {
			// 	current->anode.wide[i].compare_exchange_weak(node, initNode);
			// } else{
			// 	current->anode.narrow[i].compare_exchange_weak(node, initNode);
			// }

			node = (current->anode.isWide ? current->anode.wide[i] : current->anode.narrow[i]);

			Txn oldTxn = node->txn;

			#ifdef _USE_TSX
			__transaction_atomic {
			#endif
				if (node->txn == oldTxn) {
					node->txn = FVNode;
				}
				else{
					i--;
				}
			#ifdef _USE_TSX
			}
			#endif

			// if (!node->txn.compare_exchange_weak(oldTxn, FVNode))
			// 	i--;
		}
		else if (node->nodeType == SNODE) {
			Txn oldTxn = node->txn;
			if (oldTxn == NoTxn) {
				#ifdef _USE_TSX
				__transaction_atomic {
				#endif
					if (node->txn == oldTxn) {
						node->txn = FSNode;
					}
					else{
						i--;
					}
				#ifdef _USE_TSX
				}
				#endif
				//if (!node->txn.compare_exchange_weak(oldTxn, FSNode)) i--;
			}
			else if (oldTxn != FSNode) {
				Txn oldTxn = node->txn;
				// commit the pending changes ? //HERE?????????????
				i--;
			}
		}
		else if (node->nodeType == ANODE) {
			AnyNode* newNode;
			newNode->nodeType = FNODE;
			newNode->fnode.frozen = node;
			if (current->anode.isWide) {
				#ifdef _USE_TSX
				__transaction_atomic {
				#endif
					if (current->anode.wide[i] == node) current->anode.wide[i] = newNode;
				#ifdef _USE_TSX
				}
				#endif
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
	printTree(anode, "");
}

void CacheTrie::printTree(ANode* anode, std::string formatString) {

	int length = (anode->isWide ? 16 : 4);

	std::cout << formatString << "Beginning new search at level " << anode->level << " type " << (anode->isWide ? "wide" : "narrow") << std::endl;

	for (int i = 0; i < length; i++) {

		AnyNode* node;
		if (anode->isWide) { node = anode->wide[i]; }
		else node = anode->narrow[i];

		if (node != 0) {
			switch (node->nodeType) {
			case SNODE:
				std::cout << formatString << node->snode.value << " at location " << i << std::endl;
				break;
			case ANODE:
				std::cout << formatString << "Traverse anode at location " << i << " and level " << (node->anode.level) << std::endl;
				std::string newFormatString = formatString + "\t";
				printTree(&node->anode, newFormatString);
				std::cout << formatString << "End traversal anode at location " << i << " and level " << (node->anode.level) << std::endl;
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
