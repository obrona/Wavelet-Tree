#include <bits/stdc++.h>
using namespace std;

struct Node {
    vector<int> mapLeft, mapRight; // mapLeft[i] stores how may elems with index in [0,i] that is <= m
                                   // mapRight[i] stores #elems in [0,i] that is > m
};

struct WavTree {
    int n; // alphabet set is [0, n - 1]
    vector<Node> tree;

    WavTree(int a, vector<int>& arr): n(a), tree(4 * a, Node()) {
        build(0, 0, n - 1, arr);
    }

    void build(int p, int s, int e, const vector<int>& arr) {
        if (s == e || arr.empty()) {
            return;
        }
        
        int m = (s + e) >> 1;
        int cntL = 0, cntR = 0;
        vector<int> left, right;
        Node& currNode = tree[p];
        
        for (int i = 0; i < arr.size(); i ++) {
            if (arr[i] <= m) {
                cntL ++;
                left.push_back(arr[i]);
            } else {
                cntR ++;
                right.push_back(arr[i]);
            }

            currNode.mapLeft.push_back(cntL);
            currNode.mapRight.push_back(cntR);
        }

        build((p << 1) + 1, s, m, left);
        build((p << 1) + 2, m + 1, e, right);
    }

    // invariant, k > 0, k is 1 indexed. k means we want to find elem in [l, r] s.t there are k elems
    // <= itself
    int quantile(int p, int s, int e, int l, int r, int k) {
        //cout << s << " " << e << endl;
        // check if is leaf node first
        if (s == e) {
            return s;
        }

        // then check if it is empty node, i.e no elems in [s, e]
        if (tree[p].mapLeft.empty()) {
            //cout << p << " " << tree[p].mapLeft.size() << endl;
            return -1; // should not reach here. k > 0, so we never go to an empty node
        }
        
        int m = (s + e) >> 1;
        Node& curr = tree[p];
        int cntLeqM = (l <= 0) ? curr.mapLeft[r] : curr.mapLeft[r] - curr.mapLeft[l - 1];
        
        // cout << cntLeqM << endl;
        if (k <= cntLeqM) {
            int newL = (l == 0) ? 0 : curr.mapLeft[l - 1];
            int newR = curr.mapLeft[r] - 1;
            return quantile((p << 1) + 1, s, m, newL, newR, k);
        } else {
            int newL = (l == 0) ? 0 : curr.mapRight[l - 1];
            int newR = curr.mapRight[r] - 1;
            return quantile((p << 1) + 2, m + 1, e, newL, newR, k - cntLeqM);
        }

    }
};

int main() {
    vector<int> arr = {4,3,2,1,0};

    WavTree wavtree(5, arr);
    for (int x : wavtree.tree[0].mapLeft) cout << x << " ";
    cout << endl;
    
    cout << wavtree.quantile(0, 0, 4, 0, 3, 4);
}