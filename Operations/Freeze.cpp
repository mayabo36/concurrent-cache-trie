//After the node is frozen, it is guaranteed that no operation will ever modify that node again. 
//This is important for expanding a narrow ANode.

/*
def completeExpansion(en: ENode) = freeze(en.narrow)
var wide = new Array <Any >(16) copy(en.narrow, wide, en.level) if (!CAS(en.wide, null, wide))
wide = READ(en.wide) CAS(en.parent[en.parentpos], en, wide)
def freeze(cur: ANode) = vari=0
141
while (i < val node if (node
cur.length)
= READ(cur[i]) == null)
if (!CAS(cur[i], node, FVNode)) i -= 1 else if (node ∈ SNode)
val txn = READ(node.txn) if (txn == NoTxn)
if (!CAS(node.txn, NoTxn, FSNode)) i -= 1 else if (txn 􏰀 FSNode)
CAS(cur[i], node , txn)
i -= 1
else if (node ∈ ANode)
val fn = new FNode(node) CAS(cur[i], node , fn)
i -= 1
else if (node ∈ FNode) freeze(node.frozen) else if (node ∈ ENode)
completeExpansion(node)
i -= 1 i += 1
*/

int main(int argc, char** argv)
{

    cout << "entered Lookup operation\n";
    // Lookup subroutine, which given a key, its hash, the current level 
    // and the inner node cur, starts by extracting the relevant position bits of the hash-code. 
    
    //lookup(key, hash(key), 0, root) ? cout << "Yes\n" : cout << "No\n";
    
    return 0;                                             
}
