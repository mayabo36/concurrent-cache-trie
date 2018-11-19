#include "../Nodes/AnyNode.h"

#ifndef CACHETRIE_H_GUARD
#define CACHETRIE_H_GUARD

// A cache is an array of pointers to ANodes and SNodes at a specific level
// The first entry is a special CacheNode object containing the pointer parent to another cache level

struct CacheTrie {

    AnyNode* root;
    AnyNode** cacheHead;
    int num_threads;
    int max_misses;

    CacheTrie();
    bool insert(int value, std::size_t hash, int level, AnyNode *& current, AnyNode *& previous);
    bool insert(int value);
    void completeExpansion(AnyNode *& enode);
    void copyToWide(AnyNode *& node);
    void freeze(AnyNode *& current);
    int lookup(std::size_t hash, int level, AnyNode *& current);
    int fastLookup(int value);
    int lookup(int value);
    void printTree(ANode* anode);
    void* testInsert(void);
    AnyNode** createCache(int level, AnyNode* parent []);
    void inhabit(AnyNode** cache, AnyNode* newValue, std::size_t hash, int cacheeLevel);
    void recordCacheMiss(int thread_id);

};

#endif
