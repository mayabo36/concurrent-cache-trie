/**
    Simple test trie for words. Feel free to play around with this.
**/

#include <cstddef>
#include <string>
#include <functional>
#include <iostream>
#include "../Nodes/Node.cpp"


// Key is the word being inserted
void insert(ANode root, std::string value) {

    std::size_t hash = std::hash<std::string>{}(value);
    std::cout << "The hash of " << value << " is " << hash << "\n";

    // add logic for wide vs narrow; for now assume wide
    int location = hash % 16;
    std::cout << "Location is " << location << "\n";

    // insert an SNode for the key in root at location
    SNode sn;
    sn.hash = hash;
    sn.value = value;
    root.wide[location] = &sn;

    //std::cout << root.wide[location] << std::endl;

    // for(int i = 0; i < key.length(); i++) {
    //     int index = key[i] - 'a';
        
    //     // check if null
    //     if( !pCrawl->children[index] ) {
    //         pCrawl->children[index] = getNode();
    //     }

    //     pCrawl = pCrawl->children[index];
    // }

    // // mark last node as leaf
    // pCrawl->isEndOfWord = true;
}

bool search(ANode root, std::string key) {
    // struct TrieNode *pCrawl = root;

    // for(int i = 0; i < key.length(); i++) {
    //     int index =  nkey[i] - 'a';

    //     if( !pCrawl->children[index] )
    //         return false;
        
    //     pCrawl = pCrawl->children[index];
    // }

    // return (pCrawl != NULL && pCrawl->isEndOfWord);
}

int main() {
    std::string values[] = {"melissa", "emily", "ashton", "rebeca"};
    int n = sizeof(values) / sizeof(values[0]);

    ANode an;
    SNode sn;
    // sn.value = "test";
    // an.wide[0] = &sn;
    // SNode sn2;
    // sn2.value = "test 2";
    // an.wide[1] = &sn;

    for(int i = 0; i < n; i++) {
        std::cout << "inserting " << values[i] << std::endl;

        // this may not work bc c++ does not pass by reference? need to change to ptr?
        insert(an, values[i]);

        // SNode sn;
        // sn.hash = 123;
        // sn.value = values[i];
        // an.wide[i] = &sn;
    }

    std::cout << "\n" << std::endl;

    for(int i = 0; i < 16; i++) {
        if(an.wide[i] != nullptr || an.wide[i] != NULL){
            std::cout << "test" << std::endl;
            std::string type = typeid(an.wide[i]).name();
            if (type == typeid(SNode).name()) {
                SNode * temp = dynamic_cast<SNode*>(an.wide[i]);
                std::cout << temp->value << std::endl;
            }
        }
    }

    return 0;
}