#include <bits/stdc++.h>
using namespace std;

struct Rect {
    int l, r, h; 
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int q;
    if (!(cin >> q)) return 0;

    vector<char> op(q);
    vector<int> idx(q);                  
    vector<Rect> rects;                  
    vector<pair<int,int>> queries;      
    vector<long long> coords;          

    for (int i = 0; i < q; i++) {
        cin >> op[i];
        if (op[i] == '+') {
            long long L, R;
            int H;
            cin >> L >> R >> H;
            idx[i] = (int)rects.size();
            rects.push_back({(int)L, (int)R, H});
            coords.push_back(L);
            coords.push_back(R);
            coords.push_back(R - 1);
        } else if (op[i] == '-') {
            int j; 
            cin >> j;
            --j;              
            idx[i] = j;
        } else if (op[i] == '?') {
            long long L, R;
            cin >> L >> R;
            idx[i] = (int)queries.size();
            queries.emplace_back((int)L, (int)R);
            coords.push_back(L);
            coords.push_back(R);
            coords.push_back(R - 1);
        }
    }

    sort(coords.begin(), coords.end());
    coords.erase(unique(coords.begin(), coords.end()), coords.end());
    int K = (int)coords.size();

    auto get_pos = [&](long long x) -> int {
        return (int)(lower_bound(coords.begin(), coords.end(), x) - coords.begin());
    };

    for (auto &rc : rects) {
        int l_original = rc.l;
        int r_original = rc.r;
        rc.l = get_pos(l_original);
        rc.r = get_pos(r_original - 1);
    }

    for (auto &qr : queries) {
        int L_original = qr.first;
        int R_original = qr.second;
        qr.first  = get_pos(L_original);
        qr.second = get_pos(R_original - 1);
    }

    vector< multiset<int> > H(K); 
    vector<int> B(K, 0);         

    auto recompute_B = [&](int i) {
        if (H[i].empty()) B[i] = 0;
        else B[i] = *H[i].rbegin(); 
    };

    for (int i = 0; i < q; i++) {
        if (op[i] == '+') {
            int id = idx[i];
            Rect rc = rects[id];
            for (int pos = rc.l; pos <= rc.r; ++pos) {
                H[pos].insert(rc.h);
                recompute_B(pos);
            }
        } else if (op[i] == '-') {
            int id = idx[i];
            Rect rc = rects[id];
            for (int pos = rc.l; pos <= rc.r; ++pos) {
                auto it = H[pos].find(rc.h);
                if (it != H[pos].end()) {
                    H[pos].erase(it);
                }
                recompute_B(pos);
            }
        } else {
            int qid = idx[i];
            auto [L, R] = queries[qid];
            int ans = INT_MAX;
            for (int pos = L; pos <= R; ++pos) {
                ans = min(ans, B[pos]);
            }
            cout << ans << '\n';
        }
    }

    return 0;
}

