#include "dsets.h"

void DisjointSets::addelements(int num) {
    for (int i = 0; i < num; ++i) {
        d_vec.push_back(-1);
    }
}

int DisjointSets::find(int elem) {
    int root = elem;
    while (d_vec[root]>=0) {
        root = d_vec[root];
    }
    while (elem!=root) {
        int parent = d_vec[elem];
        d_vec[elem]=root;
        elem=parent;
    }
    return root;
}

void DisjointSets::setunion(int a, int b) {
    int a_temp = find(a);
    int b_temp = find(b);
    
    if (a_temp == b_temp) {
        return;
    }

    bool sizeg = d_vec[a_temp] <= d_vec[b_temp];
    int larger = sizeg ? a_temp : b_temp;
    int smaller = sizeg ? b_temp : a_temp;

    d_vec[larger] += d_vec[smaller];
    d_vec[smaller] = larger;
}

int DisjointSets::size(int elem) {
    int root = find(elem);
    return -d_vec[root];
}
