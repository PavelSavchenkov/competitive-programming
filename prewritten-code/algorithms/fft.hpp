#pragma once

#include <type_traits>
#include <vector>
#include <cmath>
#include <complex>

template<typename T, typename ibabanin = void>
class FFT;

template<typename T>
class FFT<T, typename std::enable_if<std::is_floating_point<T>::value>::type>
{
    class TT {
    public:
        TT() : x(0), y(0) {}

        explicit TT(T x) : x(x), y(0) {}

        TT(T x, T y) : x(x), y(y) {}

        TT conj() const {
            return TT(x, -y);
        }

        TT operator + (const TT& tt) const {
            return TT(x + tt.x, y + tt.y);
        }

        TT& operator += (const TT& tt) {
            return (*this = (*this + tt));
        }

        TT operator - (const TT& tt) const {
            return TT(x - tt.x, y - tt.y);
        }

        TT& operator -= (const TT& tt) {
            return (*this = (*this - tt));
        }

        TT operator * (const TT& tt) const {
            return TT(x * tt.x - y * tt.y, x * tt.y + tt.x * y);
        }

        TT& operator *= (const TT& tt) {
            return (*this = (*this * tt));
        }

        TT operator / (const TT& tt) const {
            return *this * tt.inv();
        }

        TT& operator /= (const TT& tt) {
            return (*this = (*this / tt));
        }

        TT operator * (const T& t) const {
            return TT(x * t, y * t);
        }

        TT& operator *= (const T& t) {
            return (*this = (*this * t));
        }

        TT operator / (const T& t) const {
            return TT(x / t, y / t);
        }

        TT& operator /= (const T& t) {
            return (*this = (*this / t));
        }

        TT operator -() const {
            return TT(-x, -y);
        }

        T real() const {
            return x;
        }

        T imag() const {
            return y;
        }

        void real(const T x) {
            this->x = x;
        }

        void imag(const T y) {
            this->y = y;
        }

        friend std::ostream& operator << (std::ostream& os, const TT& tt) {
            os << "(" << tt.x << ", " << tt.y << ")";
            return os;
        }

    private:

        TT inv() const {
            return conj() / (x * x + y * y);
        }

        T x, y;
    };

public:
    FFT() {
        maxLogn = 0;
        roots.push_back(TT(1));
    }

    template<typename outT, typename inT>
    std::vector<outT> multiply(const std::vector<inT>& a, const std::vector<inT>& b) {
        int logn = 0;
        while ((1 << logn) < std::max(a.size(), b.size())) {
            ++logn;
        }
        ++logn;

        ensureSize(logn);

        const int N = 1 << logn;

        std::vector<TT> AB(N);
        for (size_t i = 0; i < a.size(); ++i) {
            AB[i].real(a[i]);
        }
        for (size_t i = 0; i < b.size(); ++i) {
            AB[i].imag(b[i]);
        }

        fft(AB.data(), logn, false);
        std::vector<TT> C(N);
        for (int i = 0; i < N; ++i) {
            const TT r = AB[i];
            const TT rr = AB[i ? N - i : 0];
            TT val = (r + rr) / 2;
            TT A, B;
            A.real(val.real());
            B.real(val.imag());
            val = (r - rr) / 2;
            A.imag(val.imag());
            B.imag(-val.real());
            C[i] = A * B;
        }
        fft(C.data(), logn, true);

        std::vector<outT> res(N);
        for (int i = 0; i < N; ++i) {
            const T val = C[i].real();
            const auto outVal = static_cast<outT>(std::round(val));
            assert(std::fabs(val - outVal) < 0.2);
            res[i] = outVal;
        }
        while (!res.empty() && res.back() == 0) {
            res.pop_back();
        }
        return res;
    }

    template<typename outT, typename inT>
    std::vector<outT> slowMultiply(const std::vector<inT>& a, const std::vector<inT>& b) {
        std::vector<outT> res(a.size() + b.size());
        for (size_t i = 0; i < a.size(); ++i) {
            for (size_t j = 0; j < b.size(); ++j) {
                res[i + j] += outT(a[i]) * outT(b[j]);
            }
        }
        while (!res.empty() && res.back() == 0) {
            res.pop_back();
        }
        return res;
    }

private:
    void ensureSize(const int wantLogn) {
        while (maxLogn < wantLogn) {
            const size_t startPrev = roots.size() - (1u << maxLogn);
            const size_t startCur = roots.size();
            ++maxLogn;
            const int N = 1 << maxLogn;
            const T angle = 2 * M_PI / N;
            const TT coef = TT(std::cos(angle), std::sin(angle));
            roots.resize(roots.size() + N);
            for (int i = 0; i < N; ++i) {
                TT res = roots[startPrev + (i / 2)];
                if (i % 2 == 1) {
                    res *= coef;
                }
                roots[startCur + i] = res;
            }
            if (maxLogn == wantLogn) {
                rev.resize(N);
                for (int i = 1; i < N; ++i) {
                    rev[i] = (rev[i >> 1] >> 1) ^ ((i & 1) << (maxLogn - 1));
                }
            }
        }
    }

    void fft(TT* a, int logn, bool inv) {
        const int N = 1 << logn;
        const int shift = maxLogn - logn;
        assert(shift >= 0);
        for (int i = 0; i < N; ++i) {
            const int j = rev[i] >> shift;
            if (i < j) {
                std::swap(a[i], a[j]);
            }
        }

        if (inv) {
            for (int i = 0; i < N; ++i) {
                a[i] /= N;
            }
        }

        int ptr = 1;
        for (int len = 1; len < N; len *= 2) {
            const TT* cur_roots = &roots[ptr]; // block = (len*2)
            for (int j = 0; j < N; j += len * 2) {
                for (int i = 0; i < len; ++i) {
                    const auto z = cur_roots[i] * a[j + i + len];
                    a[j + i + len] = a[j + i] - z;
                    a[j + i] = a[j + i] + z;
                }
            }
            ptr += len * 2;
        }
        if (inv) {
            std::reverse(a + 1, a + N);
        }
    }

    std::vector<TT> roots; // [len = 2**0] [len = 2**1] ... [len = 2**max_logn]
    std::vector<int> rev;
    int maxLogn;
};
