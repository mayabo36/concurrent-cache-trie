//After the node is frozen, it is guaranteed that no operation will ever modify that node again. 
//This is important for expanding a narrow ANode.


def completeExpansion(ENode en) { 
    freeze(en.narrow);
    ANode wide = new Array <Any >(16) copy(en.narrow, wide, en.level);
    if (!atomic_compare_exchange(en.wide, NULL, wide)) {
        wide = READ(en.wide);
    }
    atomic_compare_exchange(en.parent[en.parentpos], en, wide);
}

def freeze(ANode cur) {
    int i = 0;
    while (i < cur.length){ 
        ANode node = *cur.wide[cur[i]]; //READ(cur[i]);
        nodeType = typeid(node).name();
        if(nodeType == NULL) {
            if (!atomic_compare_exchange(cur[i], node, FVNode)){
                i -= 1;
            }
        } 
        else if (nodeType == typeid(SNode).name()){
            Txn txn = READ(node.txn); //txn only in Snode
            if (txn == NoTxn) {
                if (!atomic_compare_exchange(node.txn, NoTxn, FSNode)){
                    i -= 1;
                } 
            }
            else if (txn 􏰀!= FSNode){
                atomic_compare_exchange(cur[i], node , txn);
                i -= 1;
            }
        }
        else if (nodeType == typeid(ANode).name()){
            val fn = new FNode(node);
            compare_exchange_weak(cur[i], node , fn);
            i -= 1;
        }
        else if (nodeType == typeid(FNode).name()) {
            freeze(node.frozen);
         } 
         else if (nodeType == typeid(ENode).name()){
            completeExpansion(node)
            i -= 1; 
         }
        i += 1;
    }
} //end of freeze


int main(int argc, char** argv)
{

    cout << "entered Lookup operation\n";
    // Lookup subroutine, which given a key, its hash, the current level 
    // and the inner node cur, starts by extracting the relevant position bits of the hash-code. 
    
    //lookup(key, hash(key), 0, root) ? cout << "Yes\n" : cout << "No\n";
    
    return 0;                                             
}
