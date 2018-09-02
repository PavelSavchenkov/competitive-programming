#pragma once

#include "prewritten-code/algorithms/DSU.hpp"
#include "prewritten-code/templates/common_header.hpp"

void DSU_test() {
    using std::cout;
    using std::endl;
    
    const int n = 10;
    DSU dsu(n);
    forn(i, n - 1) {
        dsu.unite(i, i + 1);
    }
    forn(i, n) {
        assert(dsu.get(i) == dsu.get(0));
    }
    cout << "DSU is OK" << endl;
}