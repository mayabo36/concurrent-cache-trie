#include <iostream>
#include <typeinfo>
#include <string>

enum Txn { NoTxn, FSNode, FVNode };

class AnyNode {
public:
	virtual void test() {
		std::cout << "test";
	}
};

class FNode : public AnyNode {
public:
	FNode() = default;
	virtual ~FNode() {}

	void fun() {
		std::cout << "two plus two equals four" << std::endl << "... the more you know" << std::endl;
	}
};

class ANode : public AnyNode {
public:
	ANode() = default;
	virtual ~ANode() {}

	AnyNode * wide[16];
	AnyNode * narrow[4];
	bool isWide; 

	ANode() {
		isWide = false;
	}
};

class SNode : public AnyNode {
public:
	virtual ~SNode() {}

	std::size_t hash;
	int key;
	std::string value;
	Txn txn;

	SNode() {
		std::size_t hash = 0;
		int key = 0;
		Txn txn = NoTxn;
	}

	void readTest() {
		std::cout << "test" << std::endl;
	}
};
class ENode : public AnyNode {
public:
	virtual ~ENode() {}

	ANode parent;
	int parentPos;
	ANode narrow;
	int hash;
	int level;
	ANode wide;

	ENode() {
		int parentPos = 0;
		int hash = 0;
		int level = 0;
	}

	void avoid5() {
		std::cout << "this is that N which has the fifth glyph ..." << std::endl;
	}
};

// Cache that contains an array of pointers to type AnyNode.
class Cache : public AnyNode {
public:
	virtual ~Cache() {}

	AnyNode * arr[16];

	Cache() {
	}
};

class CacheNode : public AnyNode {
	public virtual CacheNode() {};

	AnyNode parent[];
	
	int misses[];

	CacheNode() {
		
	}
};



// 3. (Test) program that uses the class defined in the API
int main(int argc, char** argv) {
	FNode f1;
	FNode f2;
	FNode f3;
	SNode s1;
	ENode e1;
	e1.parentPos = 1;

	ANode a1;

	a1.wide[0] = &f1;
	a1.wide[1] = &f2;
	a1.wide[2] = &f3;
	a1.wide[3] = &s1;
	a1.wide[4] = &e1;

	for (int k = 0; k < 5; k++) {
		std::string type = typeid(*a1.wide[k]).name();
		if (type == typeid(FNode).name()) {
			FNode * temp = dynamic_cast<FNode*>(a1.wide[k]);
			std::cout << "FNode stuff... " << std::endl;
			temp->fun();
		}
		else if (type == typeid(SNode).name()) {
			SNode * temp = dynamic_cast<SNode*>(a1.wide[k]);
			std::cout << "SNode stuff... " << std::endl;
			temp->readTest();
		}
		else if (type == typeid(ENode).name()) {
			ENode * temp = dynamic_cast<ENode*>(a1.wide[k]);
			std::cout << "ENode stuff... " << std::endl;
			std::cout << "Parent position is: " << temp->parentPos << std::endl;
			temp->avoid5();
		}
		else {
			std::cout << "dont know this one..." << std::endl;
		}
	}

	return 0;
}
