Concurrent Cache Trie Data Structure
------------------------------------
There are two versions of the data structure: a concurrent cache trie and a STM cache trie using GCC transactional memory.

There is a race condition in the code for both versions of the cache trie which causes segmentation faults. Further debugging is needed to fix this. For now, running the code with the gdb debugger hides this race condition and allows the code to execute successfully. Both cache trie versions can run up to 20,000 transactions per thread and up to 8 threads before the program crashes.

The files `ConcurrentCacheTrie/Testing/TestProgram.cpp` and `GCC_STM_CacheTrie/Testing/TestProgram.cpp` contain code for testing the two versions of the data structure. In the file, you can change the number of threads to run the program with, the default is 8. Each thread will perform 20,000 transactions (10,000 inserts and 10,000 lookups). 

To test the cache trie data structure follow the instructions below:

Note: Requires *cmake*, *make*, *gcc* and *gdb* to be installed.

-----------------------------------------------------------------------------------------------------------------------

##### Concurrent Cache Trie
1. Go to the `ConcurrentCacheTrie` folder
1. Create a folder called `build`
2. Go into the `build` folder
3. In the terminal, run `cmake ..`
4. Run `make`
5. Execute the test program with one of these commands: 
    - `./Concurrent-cache-trie` (May result in segmentation fault)
    - `gdb ./Concurrent-cache-trie` 

##### GCC TM Cache Trie
1. Go to the `GCC_TM_CacheTrie` folder
1. Create a folder called `build`
2. Go into the `build` folder
3. In the terminal, run `cmake ..`
4. Run `make`
5. Execute the test program with one of these commands: 
    - `./Concurrent-cache-trie` (May result in segmentation fault)
    - `gdb ./Concurrent-cache-trie` 
-----------------------------------------------------------------------------------------------------------------------

Note: For Windows using GitBash, cmake command is "cmake.exe -G "MinGW Makefiles" -DCMAKE_SH="CMAKE_SH-NOTFOUND" .."