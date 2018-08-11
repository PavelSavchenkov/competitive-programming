#pragma once

#include <vector>

class DSU {
public:
    explicit DSU(int n = 0) : par(n, -1) {}

    int get(int x) {
        return par[x] < 0 ? x : par[x] = get(par[x]);
    }

    bool unite(int x, int y) {
        x = get(x);
        y = get(y);
        if (x == y) {
            return false;
        }
        if (-par[x] < -par[y]) {
            std::swap(x, y);
        }
        par[x] += par[y];
        par[y] = x;
        return true;
    }

private:
    std::vector<int> par;
};
