#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

#include "prewritten-code/templates/common_header.hpp"

#include "prewritten-code/algorithms/fft.hpp"

void fft_test() {
    {
        FFT<double> fft;

        vi a{1, 2, 3, 4, 5};
        vi b{5, 4, 3, 2, -1};

        auto slowRes = fft.slowMultiply<long long>(a, b);
        auto res = fft.multiply<long long>(a, b);

        cout << "slow:" << endl;
        cout << slowRes << endl;
        cout << "fast:" << endl;
        cout << res << endl;

        assert(slowRes == res);
        cout << "correctness: " << "OK" << endl << endl;
    }

    {
        FFT<double> fft;

        for (int logn = 10; logn <= 20; ++logn) {
            int tmp = 0;
            double sum_t = 0;
            const int TESTS = 15;
            for (int test = 1; test <= TESTS; ++test) {
                vi a;
                for (int i = 0; i < (1 << logn); ++i) {
                    a.push_back(rand() % 10);
                }
                vi b;
                for (int i = 0; i < (1 << logn); ++i) {
                    b.push_back(rand() % 10);
                }

                auto st = get_time();
                auto c = fft.multiply<int>(a, b);
                sum_t += get_time() - st;
                tmp += accumulate(c.begin(), c.end(), 0);
            }
            cout << "logn = " << logn << ", avg time = " << sum_t / TESTS << endl;
        }
    }
}
