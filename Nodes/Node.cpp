#include <iostream>
using namespace std;

// Base class
class AnyNode
{
	public:

	void test() {
		cout << "test";
	}
};

class SNode : public AnyNode
{
	public:

	void readTest() {
		cout << "test 1";
	}
};

class FNode : public AnyNode
{

};

class ANode : public AnyNode
{
	public:
		AnyNode *arr[16];
};

class ENode : public AnyNode
{
	public:
		ANode parent;
		int parentPos;
		ANode narrow;
		int hash;
		int level;
		ANode wide;
};


int main()
{
	cout << "hello";
	SNode sn;
	sn.test();
	return 0;
}