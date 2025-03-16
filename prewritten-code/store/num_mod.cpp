// https://codeforces.com/contest/2082/problem/C
#include <cstdio>
#include <iostream>
#include <cmath>
#include <utility>
#include <memory>
#include <memory.h>
#include <cstdlib>
#include <set>
#include <map>
#include <cassert>
#include <bitset>
#include <unordered_set>
#include <unordered_map>
#include <functional>
#include <numeric>
#include <algorithm>
#include <complex>
#include <vector>
#include <random>
#include <ctime>
#include <ostream>
#include <queue>
#include <array>

using namespace std;

#define all(c) (c).begin(), (c).end()
#define sz(c) int((c).size())
#define pb push_back
#define mp make_pair
#define fst first
#define snd second
#define forn(i, n) for (int i = 0; i < int(n); ++i)
#define ford(i, n) for (int i = int(n) - 1; i >= 0; --i)

#define FILE "a"

#ifdef LOCAL
#define eprintf(args...) fprintf(stderr, args), fflush(stdout)
#else
#define eprintf(args...) ;
#endif

using ll = long long;
using vi = vector<int>;
using vvi = vector<vi>;
using pii = pair<int, int>;
using ull = unsigned long long;
using vll = vector<ll>;

template<int MOD>
struct Num {
    int x;

    Num(int val) : x(val) {
        if (x >= MOD) x %= MOD;
        if (x < 0) {
            x %= MOD;
            x += MOD;
            x %= MOD;
        }
    }

    Num& operator += (const Num& other) {
        x += other.x;
        if (x >= MOD) {
            x -= MOD;
        }
        return *this;
    }

    Num inv() const {
        int ans = 1;
        int xx = x;
        int p = MOD - 2;
        while (p) {
            if (p & 1) {
                ans = (ans * 1ll * xx) % MOD;
            }
            xx = (xx * 1ll * xx) % MOD;
            p >>= 1;
        }
        assert((x * 1ll * ans) % MOD == 1);
        return Num(ans);
    }

    Num& operator -= (const Num& other) {
        x -= other.x;
        if (x < 0) {
            x += MOD;
        }
        return *this;
    }
    
    Num& operator *= (const Num& other) {
        x = (x * 1ll * other.x) % MOD;
        return *this;
    }
    
    Num& operator /= (const Num& other) {
        return (*this *= other.inv());
    }

    friend Num operator + (Num a, Num b) {
        return a += b;
    }

    friend Num operator - (Num a, Num b) {
        return a -= b;
    }

    friend Num operator * (Num a, Num b) {
        return a *= b;
    }

    friend Num operator / (Num a, Num b) {
        return a /= b;
    }
};


using Number = Num<int(1e9) + 7>;

int n;
vector<char> s;

bool read() {
    if (!(cin >> n)) {
        return 0;
    }
    string buf;
    cin >> buf;
    s.clear();
    for (auto c : buf) {
        assert(c == '0' || c == '1');
        s.pb(c - '0');
    }
    return 1;
}

int solve() {
    int n = sz(s);
    vector<Number> dp(n, 0);
    for (int i = n - 1; i > 0; --i) {
        // receive 1 from the right
        if (s[i] == 1) {
            dp[i - 1] += dp[i];
        } else {
            dp[i - 1] += dp[i] / 2;
        }

        // don't
        auto prob = 1 - dp[i];
        if (s[i] == 1) {
            dp[i - 1] += prob / 2;
        } else {

        }
    }

    int base = n - 1;
    return (base + dp[0]).x;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

#ifdef LOCAL
    freopen(FILE ".in", "r", stdin);
    // freopen(FILE ".out", "w", stdout);
#endif

    int tt;
    cin >> tt;

    forn(t, tt) {
        assert(read());
        cout << solve() << '\n';

        // break;
    }

#ifdef LOCAL
    eprintf("Time: %.5f\n", clock() * 1.0 / CLOCKS_PER_SEC);
#endif
}

