#ifndef ANODE_H_GUARD
#define ANODE_H_GUARD

struct ANode {
	struct AnyNode* wide;
	struct AnyNode* narrow;
	bool isWide;

	ANode();
};

#endif

// class ANode{
// public:
// 	virtual ~ANode() {}

// 	struct AnyNode* wide;
// 	struct AnyNode* narrow;
// 	bool isWide; 

// 	ANode() {
// 		// Initialize values to 0
// 		wide = (AnyNode*)malloc(16 * sizeof(struct AnyNode));

// 		narrow = (AnyNode*)malloc(4 * sizeof(struct AnyNode));

// 		isWide = false;
// 	}
// };