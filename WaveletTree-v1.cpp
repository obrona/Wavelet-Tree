#include <bits/stdc++.h>
using namespace std;

struct Node {
    vector<int> mapLeft; // don't need mapRight, mapRight[i] = i + 1 - mapLeft[i]
                         // the node can be empty if there is no elem with val in [s, e],
                         // in this case mapLeft.size() == 0
                         // mapLeft[i] means for the array of all elems in [s, e] in same order
                         // we have mapLeft[i] elems <= m up till index i
};

struct WavTree {
    int S, E; // alphabet set is [S, E]
    vector<Node> tree;

    WavTree(int S, int E, const vector<int>& arr): S(S), E(E), tree(4 * (E - S + 1), Node()) {
        build(0, S, E, arr);
    }

    int midpoint(int s, int e) {
        return s + ((e - s) >> 1);
    }

    void build(int p, int s, int e, const vector<int>& arr) {
        if (arr.empty() || s == e) return;

        int m = midpoint(s, e);
        vector<int> left, right;
        int cntL = 0;
        Node& curr = tree[p];

        for (int i = 0; i < arr.size(); i ++) {
            if (arr[i] <= m) {
                cntL ++;
                left.push_back(arr[i]);
            } else {
                right.push_back(arr[i]);
            }

            curr.mapLeft.push_back(cntL);
        }

        build((p << 1) + 1, s, m, left);
        build((p << 1) + 2, m + 1, e, right);

    }

    // k here is 1 index, for 1st elem k = 1, 2nd elem k = 2
    // also k has to be valid, i.e k <= r - l + 1
    int quantile(int p, int s, int e, int l, int r, int k) {
        if (s == e) {
            return s;
        } else if (tree[p].mapLeft.empty()) {
            cerr << "Shouldnt be here" << endl;
            return 0;
        }

        int m = midpoint(s, e);
        Node& curr = tree[p];
        int cntLeqM = curr.mapLeft[r] - ((l == 0) ? 0 : curr.mapLeft[l - 1]);

        if (k <= cntLeqM) {
            int newL = (l == 0) ? 0 : curr.mapLeft[l - 1], newR = curr.mapLeft[r] - 1;
            return quantile((p << 1) + 1, s, m, newL, newR, k);
        } else {
            int newL = (l == 0) ? 0 : (l - 1 + 1 - curr.mapLeft[l - 1]), newR = (r + 1 - curr.mapLeft[r]) - 1;
            return quantile((p << 1) + 2, m + 1, e, newL, newR, k - cntLeqM);
        }
    }
};


void test1() {
    vector<int> arr = {4,3,2,1,0};
    WavTree wavtree(0, 4, arr);

    assert(wavtree.quantile(0, wavtree.S, wavtree.E, 0, 3, 1) == 1);
}

void test2() {
    vector<int> arr;
    for (int i = 1e5; i >= 1; i --) arr.push_back(i);
    WavTree wavtree(1, 1e5, arr);

    for (int i = 0; i < arr.size() - 4; i ++) {
        assert(wavtree.quantile(0, 1, 1e5, i, min(i + 4, 99999), 5) == 100000 - i);
    }
    //cout << wavtree.quantile(0, 1, 1e5, 99998, 99999, 2) << endl;
}

void test3() {
    vector<int> arr = {4,1,2,6,2,5,7,1,2,10};
    WavTree wavtree(1, 10, arr);

    assert(wavtree.quantile(0, 1, 10, 3, 6, 3) == 6);
    assert(wavtree.quantile(0, 1, 10, 5, 9, 5) == 10); 
    assert(wavtree.quantile(0, 1, 10, 5, 9, 3) == 5); 
    assert(wavtree.quantile(0, 1, 10, 4, 8, 3) == 2); 


}
int main() {
    test1();
    test2();
    test3();
}