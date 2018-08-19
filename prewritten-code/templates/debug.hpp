#pragma once

#include <iostream>
#include <set>
#include <map>
#include <vector>

template<typename T>
std::ostream& operator <<(std::ostream& os, const std::vector<T>& vec) {
    os << "{ ";
    bool first = true;
    for (const auto& it : vec) {
        if (!first) {
            os << ", ";
        }
        os << it;
        first = false;
    }
    os << " }";
    return os;
}

template<typename T>
std::ostream& operator <<(std::ostream& os, const std::set<T>& s) {
    os << "{ ";
    bool first = true;
    for (const auto& it : s) {
        if (!first) {
            os << ", ";
        }
        os << it;
        first = false;
    }
    os << " }";
    return os;
}


template<typename K, typename V>
std::ostream& operator <<(std::ostream& os, const std::map<K, V>& m) {
    os << "{" << "\n";
    for (const auto& it : m) {
        os << " " << it.first << " -> " << it.second << "\n";
    }
    os << "}" << "\n";
    return os;
}

void debug_out() {
    std::cerr << std::endl;
}

template<typename Head, typename... Tail>
void debug_out(Head H, Tail... T) {
    std::cerr << " " << H;
    debug_out(T...);
}

#ifdef LOCAL
    #define eprintf(...) fprintf(stderr, __VA_ARGS__), fflush(stderr)
    #define debug(...) std::cerr << "[" << #__VA_ARGS__ <<  "]:", debug_out(__VA_ARGS__);
#else
#define eprintf(...)
    #define cerr if (false) cerr
    #define debug(...)
#endif
