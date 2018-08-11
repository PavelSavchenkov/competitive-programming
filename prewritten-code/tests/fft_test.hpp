#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

#include "../algorithms/fft.hpp"

void fft_test() {
    {
        FFT<double> fft;

        std::vector<int> a{1, 2, 3, 4, 5};
        std::vector<int> b{5, 4, 3, 2, -1};

        auto slowRes = fft.slowMultiply(a, b);
        auto res = fft.multiply(a, b);

        std::cout << "slow:" << std::endl;
        for (const auto& x : slowRes) {
            std::cout << x << " ";
        }
        std::cout << std::endl << std::endl;
        std::cout << "fast:" << std::endl;
        for (const auto& x : res) {
            std::cout << x << " ";
        }
        std::cout << std::endl;

        assert(slowRes == res);
        std::cout << "OK" << std::endl << std::endl;
    }

    {
        FFT<double> fft;

        auto gett = []() {
            return clock() * 1.0 / CLOCKS_PER_SEC;
        };

        for (int logn = 10; logn <= 20; ++logn) {
            int tmp = 0;
            double sum_t = 0;
            const int TESTS = 15;
            for (int test = 1; test <= TESTS; ++test) {
                std::vector<int> a;
                for (int i = 0; i < (1 << logn); ++i) {
                    a.push_back(rand() % 10);
                }
                std::vector<int> b;
                for (int i = 0; i < (1 << logn); ++i) {
                    b.push_back(rand() % 10);
                }

                auto st = gett();
                auto c = fft.multiply(a, b);
                sum_t += gett() - st;
                tmp += std::accumulate(c.begin(), c.end(), 0);
            }
            std::cout << "logn = " << logn << ", avg time = " << sum_t / TESTS << std::endl;
        }
    }
}
