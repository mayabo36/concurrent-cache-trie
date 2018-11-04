#include <iostream>
#include <typeinfo>
#include <string>
#include <cstdlib>

enum Txn { NoTxn, FSNode, FVNode};

struct ANode;
struct SNode;

struct AnyNode {
	struct ANode anode;
	struct SNode snode;
};


struct ANode {
	struct AnyNode* wide;
	struct AnyNode* narrow;
	bool isWide;

	ANode() {
		wide = (struct AnyNode *) malloc(16 * sizeof(struct AnyNode));
		narrow = (struct AnyNode *) malloc(4 * sizeof(struct AnyNode));
		isWide = false;
	}

};

struct SNode {
	std::size_t hash;
	int key;
	std::string value;
	Txn txn;
	SNode() : hash(0), key(0), txn(NoTxn) {}
};




// 3. (Test) program that uses the class defined in the API
// int main(int argc, char** argv) {
// 	FNode f1;
// 	FNode f2;
// 	FNode f3;
// 	SNode s1;
// 	ENode e1;
// 	e1.parentPos = 1;

// 	ANode a1;

// 	a1.wide[0] = &f1;
// 	a1.wide[1] = &f2;
// 	a1.wide[2] = &f3;
// 	a1.wide[3] = &s1;
// 	a1.wide[4] = &e1;

// 	for (int k = 0; k < 5; k++) {
// 		std::string type = typeid(*a1.wide[k]).name();
// 		if (type == typeid(FNode).name()) {
// 			FNode * temp = dynamic_cast<FNode*>(a1.wide[k]);
// 			std::cout << "FNode stuff... " << std::endl;
// 			temp->fun();
// 		}
// 		else if (type == typeid(SNode).name()) {
// 			SNode * temp = dynamic_cast<SNode*>(a1.wide[k]);
// 			std::cout << "SNode stuff... " << std::endl;
// 			temp->readTest();
// 		}
// 		else if (type == typeid(ENode).name()) {
// 			ENode * temp = dynamic_cast<ENode*>(a1.wide[k]);
// 			std::cout << "ENode stuff... " << std::endl;
// 			std::cout << "Parent position is: " << temp->parentPos << std::endl;
// 			temp->avoid5();
// 		}
// 		else {
// 			std::cout << "dont know this one..." << std::endl;
// 		}
// 	}

// 	return 0;
// }
