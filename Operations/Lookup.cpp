#include <vector>
#include <string>
#include <iostream>
#include "../Nodes/Node.cpp"

/*  The goal of the lookup operation is to find the value 
    associated with the specified key. If the key is not 
    a part of the cache-trie, lookup must return the null value 
*/

//def lookup(key, hash, level, type ANode = val NoTxn val FSNode val FVNode Array <Any > cur: ANode)
class String lookup(String key, int hash, int lev, ANode cur){
    //To find a value associated with a key, 
    // the lookup operation relies on the invariant specified in the previous section. 

    int pos = (hash >> lev)&(cur.size() - 1);

    std::cout << pos;
    
    int old = READ(cur[pos]);
    std::string oldNodeType = typeid(*cur.arr[cur[pos]]).name();
    if (type == typeid(FNode).name()) {
    

    // Upon reaching an ANode at level l, hash-code bits [l, l + 4⟩ 
    // are used as an index to select the pointer to the next level. 
    // This is repeated until reaching an empty entry or an SNode.
    if (oldNodeType == null || oldNodeType == FVNode)
        return null;

    // If the lookup operation encounters a special node, such as an ENode, FNode, FSNode or an FVNode, 
    // then that node contains sufficient information to determine the state of the cache-trie when this node was create
    else if (old.nodeType() == typeid(ANode).name()) {
        // branching factor of each ANode is either 4 or 16, 
        // so either 2 or 4 hash- code bits, starting from level
        return lookup(key, hash, level + 4, old) 
    }
    else if (old.nodeType() == typeid(SNode).name()) 
    {
        if (old.key == key) 
        {
            return old.value
        }
        else 
        {
            return null; 
        }
    }
    else if (old.nodeType() == typeid(ENode).name()) {
        //lookup encountered an expansion from a concurrent insert operation
        //Rather than helping complete the expansion, lookup uses the unexpanded 
        // ANode (a1) version narrow to continue the search
        ANode a1 = old.narrow;
        return lookup(key, hash, level + 4, a1);
    }
    // An FNode contains the corresponding frozen ANode, which is used to continue the search
    //  lookup knows that the wrapped value is an ANode
    else if (old.nodeType() == F_NODE) //old ∈ FNode
        return lookup(key, hash, level + 4, old.frozen); //continue recursivley
    
}; //end of lookup

int main(int argc, char** argv)
{
    cout << "entered Lookup operation\n";
    ANode curr;

    m_parents.insert(1, 2, 3);
    std::cout << ' ' << m_parents;

    lookup(10, 100010, 9, curr);
    
    return 0;                                             
}