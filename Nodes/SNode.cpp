/**
    A leaf node in the cache-trie. Holds a key-value pair, the corresponding hash code,
    and a txn field.
**/

using namespace std;

class SNode {

    // Access specifier:
    public:
    int hash;
    int key;
    string value;
    Txn txn;
}