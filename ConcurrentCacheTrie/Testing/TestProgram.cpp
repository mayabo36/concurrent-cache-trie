#include "../DataStructures/CacheTrie.h"
#include <pthread.h>
#include <iostream>
#include <functional>
#include <cstdlib>

#define NUM_THREADS 2

CacheTrie* ctrie;

void *runTest(void* arg) {
	int *thread_id = (int*) arg;
	int tid = *thread_id;
	ctrie->testInsert(tid);
}

// void *test(void *threadid) {
//    long* tid;
//    tid = (long*)threadid;
//    std::cout << "Hello World! Thread ID, " << tid << std::endl;
// }

int main() {
	ctrie = new CacheTrie();
 	
	pthread_t threads[NUM_THREADS];
	int thread_ids[NUM_THREADS];	

	for(int id = 1; id <= NUM_THREADS; id++) {
		thread_ids[id] = id;
		int ret =  pthread_create(&threads[id], NULL, runTest, (void *)&thread_ids[id]); //
		if(ret != 0) {
				printf("Error: pthread_create() failed\n");
				exit(EXIT_FAILURE);
		}
	}

	for(int id = 1; id <= NUM_THREADS; id++) {
		pthread_join(threads[id], NULL);
	}
	
	ANode* tempRoot = &ctrie->root->anode;
	std::cout << "\n\nTree Print:" << std::endl;
	//ctrie->printTree(tempRoot);

	std::cout << "\n\nLookup Print:" << std::endl;

	// SEG FAULTS bigger numbers 
	int n = 0;
	for(int i = 600; i <= ((NUM_THREADS * 600) + 600); i++) {
		int value = ctrie->lookup(i);
		if (value != 0) {
			n++;
			//std::cout << value << std::endl;
		} 
	}
	std::cout << "Items successfully inserted/found: " << n << std::endl;

	return 0;
}
