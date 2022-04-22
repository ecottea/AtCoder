#pragma once
#include "header.h"
#include "FPS(抽象).h"
// ■■■■■ 集合のゼータ変換・メビウス変換など ■■■■■


//【上位集合でのゼータ・メビウス変換，積集合畳込み】
/*
* superset_zeta(vT& a) : O(2^n n)
*   a[0..2^n) を A[0..2^n) s.t. A[set] = Σsup⊃set a[sup] に上書きする．
*  （上位集合での高速ゼータ変換）
*
* superset_mobius(vT& A) : O(2^n n)
*   A[0..2^n) を a[0..2^n) s.t. A[set] = Σsup⊃set a[sup] に上書きする．
*  （上位集合での高速メビウス変換）
*
* vT and_convolution(vT a, vT b) : O(2^n n)
*   c[set] = Σ(sup1∩sup2 = set) a[sup1] b[sup2] なる c[0..2^n) を返す．
*/
template <typename T> void superset_zeta(vector<T>& f) {
    // 具体例を書いてみると，次のようにシェルピンスキーのギャスケットのパターンが見える：
    //	A[0] = a[0] + a[1] + a[2] + a[3] + a[4] + a[5] + a[6] + a[7] + ...
    //	A[1] =      + a[1]        + a[3]        + a[5]        + a[7] + ...
    //	A[2] =             + a[2] + a[3]               + a[6] + a[7] + ...
    //	A[3] =                    + a[3]                      + a[7] + ...
    //	A[4] =                           + a[4] + a[5] + a[6] + a[7] + ...
    //	A[5] =                                  + a[5]        + a[7] + ...
    //	A[6] =                                         + a[6] + a[7] + ...
    //	A[7] =                                                + a[7] + ...

    int n = msb(sz(f));

    rep(i, n) {
        repb(set, n) {
            if (!(set & (1 << i))) f[set] += f[set + (1 << i)];
        }
    }
}
template <typename T> void superset_mobius(vector<T>& f) {
    int n = msb(sz(f));

    rep(i, n) {
        repb(set, n) {
            if (!(set & (1 << i))) f[set] -= f[set + (1 << i)];
        }
    }
}
template <typename T> vector<T> and_convolution(vector<T> a, vector<T> b) {
    // 参考 : https://kazuma8128.hatenablog.com/entry/2018/05/31/144519
    // verify : https://judge.yosupo.jp/problem/bitwise_and_convolution

    int n = msb(sz(a));

    superset_zeta(a); superset_zeta(b);
    repb(set, n) a[set] *= b[set];
    superset_mobius(a);

    return a;
}



//【下位集合でのゼータ・メビウス変換，和集合畳込み】
/*
* subset_zeta(vT& a) : O(2^n n)
*   a[0..2^n) を A[0..2^n) s.t. A[set] = Σsub⊂set a[sub] に上書きする．
*  （下位集合での高速ゼータ変換）
*
* subset_mobius(vT& A) : O(2^n n)
*   A[0..2^n) を a[0..2^n) s.t. A[set] = Σsub⊂set a[sub] に上書きする．
*  （下位集合での高速メビウス変換）
*
* vT or_convolution(vT a, vT b) : O(2^n n)
*   c[set] = Σ(sub1∪sub2 = set) a[sub1] b[sub2] なる c[0..2^n) を返す．
*/
template <typename T> void subset_zeta(vector<T>& f) {
    // 具体例を書いてみると，次のようにシェルピンスキーのギャスケットのパターンが見える：
    //	A[0] = a[0]
    //	A[1] = a[0] + a[1]
    //	A[2] = a[0] +      + a[2]
    //	A[3] = a[0] + a[1] + a[2] + a[3]
    //	A[4] = a[0]                      + a[4]
    //	A[5] = a[0] + a[1]               + a[4] + a[5]
    //	A[6] = a[0] +      + a[2]        + a[4]        + a[6]
    //	A[7] = a[0] + a[1] + a[2] + a[3] + a[4] + a[5] + a[6] + a[7]

    int n = msb(sz(f));

    rep(i, n) {
        repb(set, n) {
            if (!(set & (1 << i))) f[set + (1 << i)] += f[set];
        }
    }
}
template <typename T> void subset_mobius(vector<T>& f) {
    int n = msb(sz(f));

    rep(i, n) {
        repb(set, n) {
            if (!(set & (1 << i))) f[set + (1 << i)] -= f[set];
        }
    }
}
template <typename T> vector<T> or_convolution(vector<T> a, vector<T> b) {
    // 参考 : https://kazuma8128.hatenablog.com/entry/2018/05/31/144519
    // verify : https://judge.yosupo.jp/problem/bitwise_and_convolution

    int n = msb(sz(a));

    subset_zeta(a); subset_zeta(b);
    repb(set, n) a[set] *= b[set];
    subset_mobius(a);

    return a;
}


//【非交和畳込み】O(2^n n^2)
/*
* c[set] = Σ(sub1凵sub2 = set) a[sub1] b[sub2] なる c[0..2^n) を返す．
* 凵 は集合の非交和であり，sub1∪sub2 = set かつ sub1∩sub2 = {} であることを表す．
*
* 利用：【下位集合でのゼータ・メビウス変換，和集合畳込み】，【形式的冪級数（可換環）】
*/
template <typename T> T add_duc(T x, T y) { return x + y; }
template <typename T> T o_duc() { return 0; }
template <typename T> T mi_duc(T x) { return -x; }
template <typename T> T mul_duc(T x, T y) { return x * y; }
template <typename T> T e_duc() { return 1; }
#define Add_mul_cring_duc T, add_duc, o_duc, mi_duc, mul_duc, e_duc
template <typename T> vector<T> disjoint_union_convolution(const vector<T>& a, const vector<T>& b) {
    // 参考 : https://37zigen.com/subset-convolution/
    // verify : https://judge.yosupo.jp/problem/subset_convolution

    int n = msb(sz(a));

    vector<FPS<Add_mul_cring_duc>> f(1LL << n), g(1LL << n);
    repb(set, n) {
        f[set] = a[set] * FPS<Add_mul_cring_duc>::monomial(popcount(set));
        g[set] = b[set] * FPS<Add_mul_cring_duc>::monomial(popcount(set));
    }

    auto h = or_convolution(f, g);

    vector<T> c(1LL << n);
    repb(set, n) {
        c[set] = h[set][popcount(set)];
    }

    return c;
}


//【アダマール変換，対称差畳込み】
/*
* hadamard(vT& a) : O(2^n n)
*   a[0..2^n) を高速アダマール変換する．
*
* hadamard_inv(vT& A) : O(2^n n)
*   A[0..2^n) を逆高速アダマール変換する．
*
* vT symmetric_difference_convolution(vT a, vT b) : O(2^n n)
*   c[set] = Σ(set1△set2 = set) a[set1] b[set2] なる c[0..2^n) を返す．
*   △ は集合の対称差であり，添字でいうと XOR である．
*/
template <typename T> void hadamard(vector<T>& f) {
    // 具体例：
    //	A[0] = a[0] + a[1] + a[2] + a[3] + a[4] + a[5] + a[6] + a[7] + ...
    //	A[1] = a[0] - a[1] + a[2] - a[3] + a[4] - a[5] + a[6] - a[7] + ...
    //	A[2] = a[0] + a[1] - a[2] - a[3] + a[4] + a[5] - a[6] - a[7] + ...
    //	A[3] = a[0] - a[1] - a[2] + a[3] + a[4] - a[5] - a[6] + a[7] + ...
    //	A[4] = a[0] + a[1] + a[2] + a[3] - a[4] - a[5] - a[6] - a[7] + ...
    //	A[5] = a[0] - a[1] + a[2] - a[3] - a[4] + a[5] - a[6] + a[7] + ...
    //	A[6] = a[0] + a[1] - a[2] - a[3] - a[4] - a[5] + a[6] + a[7] + ...
    //	A[7] = a[0] - a[1] - a[2] + a[3] - a[4] + a[5] + a[6] - a[7] + ...
    //
    // 係数行列の + の部分だけ書くと，
    //	+ + + + + + + +
    //	+   +   +   +  
    //  + +     + +    
    //  +     + +     +
    //  + + + +        
    //  +   +     +   +
    //  + +         + +
    //  +     +   + +  
    // となり，シェルピンスキーのギャスケットっぽいがゴミが付いている．

    int n = msb(sz(f));

    rep(i, n) {
        repb(set, n) {
            if (!(set & (1 << i))) {
                T x = f[set], y = f[set | (1 << i)];
                f[set] = x + y, f[set + (1 << i)] = x - y;
            }
        }
    }
}
template <typename T> void hadamard_inv(vector<T>& f) {
    int n = msb(sz(f));

    rep(i, n) {
        repb(set, n) {
            if (!(set & (1 << i))) {
                T x = f[set], y = f[set | (1 << i)];
                f[set] = (x + y) / 2, f[set + (1 << i)] = (x - y) / 2;
            }
        }
    }
}
template <typename T> vector<T> symmetric_difference_convolution(vector<T> a, vector<T> b) {
    // 参考 : https://kazuma8128.hatenablog.com/entry/2018/05/31/144519
    // verify : https://judge.yosupo.jp/problem/bitwise_xor_convolution

    int n = msb(sz(a));

    hadamard(a); hadamard(b);
    repb(set, n) a[set] *= b[set];
    hadamard_inv(a);

    return a;
}


