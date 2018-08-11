#include "prewritten-code/templates/all_includes.hpp"
#include "prewritten-code/templates/types.hpp"
#include "prewritten-code/templates/macros.hpp"

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
