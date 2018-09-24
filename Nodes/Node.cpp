#include <iostream>
#include <typeinfo>

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

class SNode : public AnyNode {
public:
	SNode() = default;
	virtual ~SNode() {}
	
	void readTest() {
		std::cout << "test" << std::endl;
	}
};
class ENode : public AnyNode {
public:
	ENode() = default;
	virtual ~ENode() {}
	
	void avoid5() {
		std::cout << "this is that N which has the fifth glyph ..." << std::endl;
	}
};

class ANode : public AnyNode {
public:
	ANode() = default;
	virtual ~ANode() {}
	
	AnyNode * arr[16];
};

// 3. (Test) program that uses the class defined in the API
int main(int argc, char** argv) {
	FNode f1;
	FNode f2;
	FNode f3;
	SNode s1;
	ENode e1;
	
	ANode a1;
	
	a1.arr[0] = &f1;
	a1.arr[1] = &f2;
	a1.arr[2] = &f3;
	a1.arr[3] = &s1;
	a1.arr[4] = &e1;

	for (int k = 0; k < 5; k++) {
		std::string type = typeid(*a1.arr[k]).name();
		if (type == typeid(FNode).name()) {
			FNode * temp = dynamic_cast<FNode*>(a1.arr[k]);
			std::cout << "FNode stuff... " << std::endl;
			temp->fun();
		} else if (type == typeid(SNode).name()) {
			SNode * temp = dynamic_cast<SNode*>(a1.arr[k]);
			std::cout << "SNode stuff... " << std::endl;
			temp->readTest();
		} else if (type == typeid(ENode).name()) {
			ENode * temp = dynamic_cast<ENode*>(a1.arr[k]);
			std::cout << "ENode stuff... " << std::endl;
			temp->avoid5();
		} else {
			std::cout << "dont know this one..." << std::endl;
		}
	}
	
	return 0;
}