#ifndef CACHENODE_H_GUARD
#define CACHENODE_H_GUARD

// A cache is an array of pointers to ANodes and SNodes at a specific level
// The first entry is a special CacheNode object containing the pointer parent to another cache level

struct CacheNode {

    int* misses;
    struct AnyNode* parent;

    CacheNode();
};

#endif