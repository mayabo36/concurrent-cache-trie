// #include "CacheTrie.h"
// #include <pthread.h>
// #include <iostream>
// #include <functional>
// #include <cstdlib>

// void *print_message(void* arg) {
// 	CacheTrie* ctrie = static_cast<CacheTrie *>(arg);
// 	ctrie->testInsert();
// 	return nullptr;
// 	//std::cout << "test" << std::endl;
// }

// int main() {
// 	CacheTrie* ctrie = new CacheTrie();
//  	int rs;
	
// 	pthread_t t0; //(&CacheTrie::testInsert, std::ref(ctrie));
// 	//pthread_t t1; //(&CacheTrie::testInsert, std::ref(ctrie));

// 	pthread_create(&t0, NULL, print_message, ctrie);

// 	void* status;

// 	pthread_join(t0, &status);


// 	ANode* tempRoot = &ctrie->root->anode;
// 	std::cout << "\n\nTree Print:" << std::endl;
// 	ctrie->printTree(tempRoot);

// 	std::cout << "\n\nLookup Print:" << std::endl;

// 	// SEG FAULTS bigger numbers 
// 	for(int i = 1; i < 251; i++) {
// 		int value = ctrie->lookup(i);
// 		if (value != 0) std::cout << value << std::endl;
// 	}

// 	return 0;
// }
