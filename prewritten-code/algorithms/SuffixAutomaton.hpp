#pragma once

#include <array>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <functional>

template<int MAXLEN, int Lc = 'a', int Rc = 'z'>
class SuffixAutomaton {
public:
    static constexpr int A = Rc - Lc + 1;
    static constexpr int MAXN = MAXLEN * 2;

    struct node;
    using nodePtr = node*;

private:
    std::vector<node> pool{};
    int ptr_pool{};

public:
    struct node {
        std::map<int, nodePtr> to{};
        nodePtr link{};
        int len{};

        node() = default;

        void* operator new(size_t) = delete;
    };

    nodePtr newNode() {
        int pos = ptr_pool++;
        assert(pos < MAXN);
        pool[pos] = node();
        return pool.data() + pos;
    }

    SuffixAutomaton() : pool(MAXN) {
        dropNodesPool();
    }

    void dropNodesPool() {
        ptr_pool = 0;
    }

    nodePtr root;

    void build(const std::string& s) {
        std::vector<int> ss(s.size());
        for (size_t i = 0; i < s.size(); ++i) {
            ss[i] = static_cast<int>(s[i]);
        }
        build(ss);
    }

    void build(const std::vector<int>& s) {
        root = newNode();
        nodePtr last = root;
        for (const auto& c : s) {
            add_c(encode(c), last);
        }
    }

    int id_node(const nodePtr v) const {
        return v ? v - pool.data() : -1;
    }

    nodePtr get_node(const int id) {
        assert(id != -1);
        return pool.data() + id;
    }

    int cnt_alive_nodes() const {
        return ptr_pool;
    }

    static int encode(const int c) {
        return c - Lc;
    }

    static int decode(const int c) {
        return c + Lc;
    }

    void output(std::ostream& os) const {
        assert(root);
        std::map<nodePtr, std::vector<std::string>> S;

        const std::function<void(nodePtr, std::string)> dfs = [&](nodePtr v, std::string cur = "") {
            if (!v) {
                return;
            }
            S[v].push_back(cur);
            for (const auto& it : v->to) {
                const int c = it.first;
                const nodePtr to = it.second;
                cur.push_back(c);
                dfs(to, cur);
                cur.pop_back();
            }
        };

        dfs(root, "");

        for (const auto& it : S) {
            const nodePtr v = it.first;
            const int id = id_node(v);
            os << id << ": ";

            auto ss = S[v];
            std::sort(ss.begin(), ss.end(), [](const std::string& a, const std::string& b) { return a.size() < b.size(); });
            std::string prev;
            for (const auto& s : ss) {
                if (!prev.empty()) {
                    assert(s.size() == prev.size() + 1);
                    assert(prev == s.substr(1));
                }
                prev = s;
            }

            if (!ss.empty()) {
                const auto a = ss.front();
                const auto b = ss.back();
                os << "[" << b.substr(0, b.size() - a.size()) << "[" << a << "]";
            }
            os << ", ";
            os << "link = " << id_node(v->link) << ", len = " << v->len << std::endl;
        }
    }

private:
    void add_c(const int c, nodePtr& last) {
        nodePtr p = last;
        nodePtr nw = newNode();
        nw->len = last->len + 1;
        last = nw;

        for (; p && !p->to.count(c); p = p->link) {
            p->to[c] = nw;
        }

        if (!p) {
            nw->link = root;
            return;
        }

        const nodePtr q = p->to[c];
        if (q->len == p->len + 1) {
            nw->link = q;
            return;
        }

        nodePtr clone = newNode();
        clone->to = q->to;
        clone->len = p->len + 1;
        clone->link = q->link;
        nw->link = q->link = clone;
        for (; p && p->to.count(c) && p->to[c] == q; p = p->link) {
            p->to[c] = clone;
        }
    }
};