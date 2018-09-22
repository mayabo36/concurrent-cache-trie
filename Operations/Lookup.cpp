/*  The goal of the lookup operation is to find the value 
    associated with the specified key. If the key is not 
    a part of the cache-trie, lookup must return the null value 
*/
#include <cstddef>
#include <string>
#include <iostream>

using namespace std;
//def lookup(key, hash, level, type ANode = val NoTxn val FSNode val FVNode Array <Any > cur: ANode)
class String lookup(String key, int hash, int l, ANode cur){
    //To find a value associated with a key, 
    // the lookup operation relies on the invariant specified in the previous section. 

    val pos = (hash >>> level)&&(cur.length - 1);
    
    val old = READ(cur[pos])

    // Upon reaching an ANode at level l, hash-code bits [l, l + 4⟩ 
    // are used as an index to select the pointer to the next level. 
    // This is repeated until reaching an empty entry or an SNode.
    if (old == null ∨ old == FVNode)
        return null

    // If the lookup operation encounters a special node, such as an ENode, FNode, FSNode or an FVNode, 
    // then that node contains sufficient information to determine the state of the cache-trie when this node was create
    else if (old ∈ ANode) {
        // branching factor of each ANode is either 4 or 16, 
        // so either 2 or 4 hash- code bits, starting from level
        return lookup(key, hash, level + 4, old) 
    }
    else if (old ∈ SNode)
        if (old.key == key) 
            return old.value
        else 
            return null; 
    else if (old ∈ ENode) {
        //lookup encountered an expansion from a concurrent insert operation
        //Rather than helping complete the ex- pansion, lookup uses the unexpanded 
        // ANode (an) version narrow to continue the search
        val an = old.narrow;
        return lookup(key, hash, level + 4, an);
    }
    // An FNode contains the corresponding frozen ANode, which is used to continue the search
    //  lookup knows that the wrapped value is an ANode
    else if (old == FNode) //old ∈ FNode
        return lookup(key, hash, level + 4, old.frozen); //continue recursivley
    
}; //end of lookup

int main(int argc, char** argv)
{

    cout << "entered Lookup operation\n";
    // Lookup subroutine, which given a key, its hash, the current level 
    // and the inner node cur, starts by extracting the relevant position bits of the hash-code. 
    
    //lookup(key, hash(key), 0, root) ? cout << "Yes\n" : cout << "No\n";
    
    return 0;                                             
}
