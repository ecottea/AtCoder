#pragma once
#include "header.h"
// ■■■■■ 畳み込み ■■■■■



//【添字 xor での畳込み】
/*
* convolution_xor(a, b) : O(n log n)
*   c[k] = Σ_(i xor j = k) a[i] b[j] なる c を返す．
*
* fwt_xor(a) : O(n log n)
*   a を高速アダマール変換する．
*
* ifwt_xor(A) : O(n log n)
*   A を逆高速アダマール変換する．
*/
template <typename T> void fwt_xor(vector<T>& f) {
    int n = sz(f);
    for (int i = 1; i < n; i <<= 1) {
        rep(j, n) {
            if ((j & i) == 0) {
                T x = f[j], y = f[j | i];
                f[j] = x + y, f[j | i] = x - y;
            }
        }
    }
}
template <typename T> void ifwt_xor(vector<T>& f) {
    int n = sz(f);
    for (int i = 1; i < n; i <<= 1) {
        rep(j, n) {
            if ((j & i) == 0) {
                T x = f[j], y = f[j | i];
                f[j] = (x + y) / 2, f[j | i] = (x - y) / 2;
            }
        }
    }
}
template <typename T> vector<T> convolution_xor(vector<T> a, vector<T> b) {
    // 参考 : https://kazuma8128.hatenablog.com/entry/2018/05/31/144519

    int n = sz(a);
    fwt_xor(a); fwt_xor(b); // A = fwt_xor(a), B = fwt_xor(b) を計算 : O(n log n)
    rep(i, n) a[i] *= b[i]; // C[k] = A[k] * B[k] を計算 : O(n)
    ifwt_xor(a);            // c = ifwt_xor(C) を計算 : O(n log n)
    return a;
}


//【上位集合，添字 and での畳込み】
/*
* convolution_and(a, b) : O(n log n)
*   c[k] = Σ_(i and j = k) a[i] b[j] なる c を返す．
*
* fwt_and(a) : O(n log n)
*   A[j] = Σ_(i ⊃ j) a[i] なる A を返す．
*  （上位集合での高速ゼータ変換）
*
* ifwt_and(A) : O(n log n)
*   a[j] = Σ_(i ⊃ j) (-1)^(i - j) A[i] なる a を返す．
*  （上位集合での高速メビウス変換）
*/
template <typename T> void fwt_and(vector<T>& f) {
    int n = sz(f);
    for (int i = 1; i < n; i <<= 1) {
        rep(j, n) {
            if ((j & i) == 0) {
                f[j] += f[j | i];
            }
        }
    }
}
template <typename T> void ifwt_and(vector<T>& f) {
    int n = sz(f);
    for (int i = 1; i < n; i <<= 1) {
        rep(j, n) {
            if ((j & i) == 0) {
                f[j] -= f[j | i];
            }
        }
    }
}
template <typename T> vector<T> convolution_and(vector<T> a, vector<T> b) {
    // 参考 : https://kazuma8128.hatenablog.com/entry/2018/05/31/144519

    int n = sz(a);
    fwt_and(a); fwt_and(b);
    rep(i, n) a[i] *= b[i];
    ifwt_and(a);
    return a;
}


//【下位集合，添字 or での畳込み】
/*
* convolution_or(a, b) : O(n log n)
*   c[k] = Σ_(i or j = k) a[i] b[j] なる c を返す．
*
* fwt_or(a) : O(n log n)
*   A[j] = Σ_(i ⊂ j) a[i] なる A を返す．
*  （下位集合での高速ゼータ変換）
*
* ifwt_or(A) : O(n log n)
*   a[j] = Σ_(i ⊂ j) (-1)^(i - j) A[i] なる a を返す．
*  （下位集合での高速メビウス変換）
*/
template <typename T> void fwt_or(vector<T>& f) {
    int n = sz(f);
    for (int i = 1; i < n; i <<= 1) {
        rep(j, n) {
            if ((j & i) == 0) {
                f[j | i] += f[j];
            }
        }
    }
}
template <typename T> void ifwt_or(vector<T>& f) {
    int n = sz(f);
    for (int i = 1; i < n; i <<= 1) {
        rep(j, n) {
            if ((j & i) == 0) {
                f[j | i] -= f[j];
            }
        }
    }
}
template <typename T> vector<T> convolution_or(vector<T> a, vector<T> b) {
    // 参考 : https://kazuma8128.hatenablog.com/entry/2018/05/31/144519

    int n = sz(a);
    fwt_or(a); fwt_or(b);
    rep(i, n) a[i] *= b[i];
    ifwt_or(a);
    return a;
}


