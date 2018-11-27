#include "../DataStructures/CacheTrie.h"
#include <pthread.h>
#include <iostream>
#include <functional>
#include <cstdlib>
#include <chrono> 

#define NUM_THREADS 8

CacheTrie* ctrie;
std::string thread_results[NUM_THREADS];

void *runTest(void* arg) {
	int *thread_id = (int*) arg;
	int tid = *thread_id;
	thread_results[tid] = ctrie->testTrie(tid);
}

int main() {
	ctrie = new CacheTrie();
 	
	pthread_t threads[NUM_THREADS];
	int thread_ids[NUM_THREADS];

	auto startTime = std::chrono::high_resolution_clock::now();
	
	for(int id = 1; id <= NUM_THREADS; id++) {
		thread_ids[id] = id;
		int ret =  pthread_create(&threads[id], NULL, runTest, (void *)&thread_ids[id]);
		if(ret != 0) {
				printf("Error: pthread_create() failed\n");
				exit(EXIT_FAILURE);
		}
	}

	for(int id = 1; id <= NUM_THREADS; id++) {
		pthread_join(threads[id], NULL);
	}

	auto endTime = std::chrono::high_resolution_clock::now();

	for(int id = 1; id <= NUM_THREADS; id++) {
		std::cout << thread_results[id] << std::endl;
	}

	auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);

	std::cout << "Nanoseconds to complete all operations: " << duration.count() << std::endl;

	// ANode* tempRoot = &ctrie->root->anode;
	// std::cout << "\n\nTree Print:" << std::endl;
	// ctrie->printTree(tempRoot);

	//std::cout << "\n\nLookup Print:" << std::endl;

	// int n = 0;
	// for(int i = 1; i <= 5000000; i++) {
	// 	int value = ctrie->lookup(i);
	// 	if (value != 0) {
	// 		n++;
	// 		std::cout << value << std::endl;
	// 	} 
	// }
	// std::cout << "Items successfully inserted/found: " << n << std::endl;

	return 0;
}
