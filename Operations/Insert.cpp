#include <vector>
#include <string>
#include <functional>
#include <iostream>
#include "../Nodes/Node.cpp"

bool insert(std::string &key, int value, int h, int lev, ANode cur, ANode prev){

int pos = (h >> lev)&(cur.length - 1);

std::string oldNodeType = typeid(*cur.wide[cur[pos]]).name();

int old = READ(cur[pos]);
if (oldNodeType  == NULL) {
    SNode sn = new SNode(h, key, v, NoTxn);
    if (CAS(cur[pos], old, sn)) {
        return true;
    }
    else {
        return insert(key, v, h, lev, cur, prev);
    }
}
else if (oldNodeType == typeid(ANode).name()) {
    return insert(k, v, h, lev + 4, old, cur);
}
else if (oldNodeType == typeid(SNode).name()) {
    val txn = READ(old.txn) 
    if (txn == NoTxn) {
        if (old.key == key){
            val sn = new SNode(h, k, v, NoTxv)
            if (CAS(old.txn, NoTxn, sn)) {
                CAS(cur[pos], old, sn);
                return true;
            }
            else {
                return insert(k, v, h, lev, cur, prev);
            }
        }
        else if (cur.length == 4) 
            val ppos = (h >> (lev- 4)&)prev.length - 1)
            val en = new ENode(prev, ppos, cur, h, lev)
            if (compare_exchange_weak(prev[ppos], cur, en)){
                completeExpansion(en);
                AnyNode wide = READ(en.wide); //type wide is an AnyNode Array
                return insert(key, v, h, lev, wide, prev);
            }
            else {
                return insert (key, v, h, lev, cur, prev);
            }
        }
        else
            val sn = new SNode(h, k, v, NoTxn);
            val an = createANode(old, sn, lev + 4);
            if (compare_exchange_weak(old.txn, NoTxn, an)) {
                compare_exchange_weak(cur[pos], old, an);
                return true;
            }
            else {
                return insert(key, v, h, lev, cur, prev);
            }
    else if (txn == FSNode) {
        return false 
    }
    else {
        compare_exchange_weak(cur[pos], old, txn);
        return insert(k, v, h, lev, cur, prev);
    }
}
else if (oldNodeType == typeid(ENode).name()) {
    completeExpansion(old);
}
return false;
};//end of insert

// 3. (Test) program that uses the class defined in the API
int main(int argc, char** argv) {
    std::string key = "Melissa";
    std::size_t hash = std::hash<std::string>{}(key);
    std::cout << hash << "\n";
    //bool insert = insert(key, v, hash(key), 0, root, null);
    //std::cout << "inserting..." << insert << std::endl; 
    //def insert(key: KeyType , v: ValueType) =
    //if (!insert(key, v, hash(key), 0, root, null))
    //insert(key, v);
    return 0; 
}