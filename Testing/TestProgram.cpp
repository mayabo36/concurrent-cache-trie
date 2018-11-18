#include "ConcurrentCache-Trie/DataStructures/CacheTrie.h"
#include <thread>
#include <iostream>

int main() {
    CacheTrie* ctrie = new CacheTrie(2);
	
	std::thread t0(&CacheTrie::testInsert, std::ref(ctrie));
	std::thread t1(&CacheTrie::testInsert, std::ref(ctrie));

	t0.join();
	t1.join();

    ANode* tempRoot = &ctrie->root->anode;
    std::cout << "\n\nTree Print:" << std::endl;
    ctrie->printTree(tempRoot);

    std::cout << "\n\nLookup Print:" << std::endl;

	// SEG FAULTS bigger numbers 
    for(int i = 1; i < 251; i++) {
        int value = ctrie->fastLookup(i);
        if (value != 0) std::cout << value << std::endl;
    }

	std::cout << "Max level reached: " << ctrie->max_level << std::endl;

	return 0;
}