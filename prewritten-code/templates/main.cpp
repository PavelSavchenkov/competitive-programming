#ifdef LOCAL
    #define _GLIBCXX_DEBUG
#endif

#include "prewritten-code/templates/common_header.hpp"

/* *************************** TEMPLATE CODE ENDS HERE ******************************** */



int n;

bool read() {
    if (!(cin >> n)) {
        return 0;
    }
    return 1;
}

void solve() {

}

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout.tie(nullptr);
    cout.precision(12);
    cout << fixed;

#ifdef LOCAL
    #define FILE_NAME "a"
    assert(freopen(FILE_NAME ".in", "r", stdin));
//    freopen(FILE_NAME ".out", "w", stdout);
#endif

    while (read()) {
        solve();
    }

    return 0;
}
