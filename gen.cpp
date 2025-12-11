#include<bits/stdc++.h>
using namespace std;

using ll = long long;

mt19937_64 rnd(chrono::steady_clock::now().time_since_epoch().count());

ll random(ll L, ll R) {
    return uniform_int_distribution<ll>(L, R)(rnd);
}

int main() {
    int q,mx;
    cin >> q >> mx;
    cout << q << '\n';

    int n_add = 0;
    set<int> alive;
    string t_ops = "+-?";

    while(q--) {
        int op = random(0, 2);
        
        if (op == 1 and alive.empty()) {
            op = 2;
        }

        cout << t_ops[op] << ' ';
        if (op == 0) {
            int l = random(1, mx - 1);
            int r = random(l + 1, mx);
            int h = random(1, mx);
            alive.emplace(++n_add);
            cout << l << " " << r << " " << h << '\n';
        } else if (op == 1) {
            int x = random(1, mx);
            auto it = alive.lower_bound(x);
            if (it == alive.end()) it--;
            cout << *it << "\n";
            alive.erase(it);
        } else {
            int l = random(1, mx - 1);
            int r = random(l + 1, mx);
            cout << l << " " << r << '\n'; 
        }
    }

    return 0;
}
