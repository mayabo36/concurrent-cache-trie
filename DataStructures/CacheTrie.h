#include "../Nodes/AnyNode.h"
#include <atomic>

#ifndef CACHETRIE_H_GUARD
#define CACHETRIE_H_GUARD

// A cache is an array of pointers to ANodes and SNodes at a specific level
// The first entry is a special CacheNode object containing the pointer parent to another cache level

struct CacheTrie {

    AnyNode* root;
    AnyNode* cacheHead;
    std::atomic<int> max_level {0}; // update in complete expansion

    CacheTrie();
    bool insert(int value, std::size_t hash, int level, AnyNode *& current, AnyNode *& previous);
    bool insert(int value);
    void completeExpansion(AnyNode *& enode);
    void copyToWide(AnyNode *& node);
    void freeze(AnyNode *& current);
    int lookup(std::size_t hash, int level, AnyNode *& current);
    int lookup(int value);
    void printTree(ANode* anode);
    void testInsert();
};

#endif