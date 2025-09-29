#include <iostream>
#include <vector>
#include <map>
#include <utility>
using namespace std;
using ll = long long;

class BIT {
    int n;
    vector<ll> tree;
public:
    BIT(int size) : n(size + 1), tree(n + 1) {}
    
    void update(int i, ll val) {
        while (i < n) {
            tree[i] += val;
            i += i & -i;
        }
    }
    ll query(int i) {
        ll res = 0;
        while (i > 0) {
            res += tree[i];
            i -= i & -i;
        }
        return res;
    }
};

void solve(int n, const vector<ll>& a) {
    map<ll, vector<int>> mappings;
    for (int i = 0; i < n; i++) mappings[a[i]].push_back(i);
    ll res = 0;
    BIT bit(n);
    for (int min = 0; min < n; min++) {
        auto it = mappings.find((ll)min - 1);
        if (it != mappings.end()) {
            for (int i : it->second) {
                res += (ll)i - bit.query(i);
                bit.update(i + 1, 1);
            }
        }
        cout << res << '\n';
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    freopen("haircut.in","r",stdin);
    freopen("haircut.out","w",stdout);
    int n;
    cin >> n;
    vector<ll> arr(n);
    for (int i = 0; i < n; i++) cin >> arr[i];
    solve(n, arr);
    return 0;
}
