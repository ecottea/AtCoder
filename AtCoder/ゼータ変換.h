#pragma once
#include "header.h"
#include "FPS(抽象).h"
// ■■■■■ 集合のゼータ変換・メビウス変換など ■■■■■


//【ゼータ変換（上位集合）】O(2^n n)
/*
* a[0..2^n) を
*       A[set] = Σsup⊃set a[sup]
* なる A[0..2^n) に上書きする．（上位集合の値全てを自身に加える）
*/
template <class T> void superset_zeta(vector<T>& a) {
    // verify : https://judge.yosupo.jp/problem/bitwise_and_convolution

    // 具体例を書いてみると，次のようにシェルピンスキーのギャスケットのパターンが見える：
    //	A[0] = a[0] + a[1] + a[2] + a[3] + a[4] + a[5] + a[6] + a[7] + ...
    //	A[1] =      + a[1]        + a[3]        + a[5]        + a[7] + ...
    //	A[2] =             + a[2] + a[3]               + a[6] + a[7] + ...
    //	A[3] =                    + a[3]                      + a[7] + ...
    //	A[4] =                           + a[4] + a[5] + a[6] + a[7] + ...
    //	A[5] =                                  + a[5]        + a[7] + ...
    //	A[6] =                                         + a[6] + a[7] + ...
    //	A[7] =                                                + a[7] + ...

    int n = msb(sz(a));

    rep(i, n) {
        repb(set, n) {
            if (!(set & (1 << i))) a[set] += a[set + (1 << i)];
        }
    }
}


//【メビウス変換（上位集合）】O(2^n n)
/*
* A[0..2^n) を
*       A[set] = Σsup⊃set a[sup]
* なる a[0..2^n) に上書きする．（上位集合からの自身の値への寄与を取り除く）
*/
template <class T> void superset_mobius(vector<T>& A) {
    // verify : https://judge.yosupo.jp/problem/bitwise_and_convolution
    
    int n = msb(sz(A));

    rep(i, n) {
        repb(set, n) {
            if (!(set & (1 << i))) A[set] -= A[set + (1 << i)];
        }
    }
}


//【積集合畳込み】O(2^n n)
/*
* 与えられた a[0..2^n), b[0..2^n) に対して
*       c[set] = Σ(sup1∩sup2 = set) a[sup1] b[sup2] 
* なる c[0..2^n) を返す．
* 
* 利用：【ゼータ変換（上位集合）】，【メビウス変換（上位集合）】
*/
template <class T> vector<T> and_convolution(vector<T> a, vector<T> b) {
    // 参考 : https://kazuma8128.hatenablog.com/entry/2018/05/31/144519
    // verify : https://judge.yosupo.jp/problem/bitwise_and_convolution

    int n = msb(sz(a));

    superset_zeta(a);
    superset_zeta(b);

    repb(set, n) a[set] *= b[set];

    superset_mobius(a);

    return a;
}


//【max ゼータ変換（上位集合）】O(2^n n)
/*
* a[0..2^n) を
*       A[set] = MAX(sup⊃set) a[sup]
* なる A[0..2^n) に上書きする．
*/
template <class T> void superset_max_zeta(vector<T>& a) {
    int n = msb(sz(a));

    rep(i, n) {
        repb(set, n) {
            if (!(set & (1 << i))) chmax(a[set], a[set + (1 << i)]);
        }
    }
}


//【上位積集合トロピカル畳込み】O(2^n n)
/*
* 与えられた a[0..2^n), b[0..2^n) に対して
*       c[set] = MAX(sup1∩sup2 ⊃ set) (a[sup1] + b[sup2])
* なる c[0..2^n) を返す．
*
* 利用：【max ゼータ変換（上位集合）】
*/
template <class T> vector<T> superset_and_tropical_convolution(vector<T> a, vector<T> b) {
    int n = msb(sz(a));

    superset_max_zeta(a);
    superset_max_zeta(b);

    repb(set, n) a[set] += b[set];

    return a;
}


//【ゼータ変換（下位集合）】O(2^n n)
/*
* a[0..2^n) を
*       A[set] = Σsub⊂set a[sub]
* なる A[0..2^n) に上書きする．（下位集合の値全てを自身に加える）
*/
template <class T> void subset_zeta(vector<T>& a) {
    // verify : https://judge.yosupo.jp/problem/bitwise_and_convolution

    // 具体例を書いてみると，次のようにシェルピンスキーのギャスケットのパターンが見える：
    //	A[0] = a[0]
    //	A[1] = a[0] + a[1]
    //	A[2] = a[0] +      + a[2]
    //	A[3] = a[0] + a[1] + a[2] + a[3]
    //	A[4] = a[0]                      + a[4]
    //	A[5] = a[0] + a[1]               + a[4] + a[5]
    //	A[6] = a[0] +      + a[2]        + a[4]        + a[6]
    //	A[7] = a[0] + a[1] + a[2] + a[3] + a[4] + a[5] + a[6] + a[7]

    int n = msb(sz(a));

    rep(i, n) {
        repb(set, n) {
            if (!(set & (1 << i))) a[set + (1 << i)] += a[set];
        }
    }
}


//【メビウス変換（下位集合）】O(2^n n)
/*
* A[0..2^n) を
*       A[set] = Σsub⊂set a[sub]
* なる a[0..2^n) に上書きする．（下位集合からの自身の値への寄与を取り除く）
*/
template <class T> void subset_mobius(vector<T>& A) {
    // verify : https://judge.yosupo.jp/problem/bitwise_and_convolution
    
    int n = msb(sz(A));

    rep(i, n) {
        repb(set, n) {
            if (!(set & (1 << i))) A[set + (1 << i)] -= A[set];
        }
    }
}


//【和集合畳込み】O(2^n n)
/*
* 与えられた a[0..2^n), b[0..2^n) に対して
*       c[set] = Σ(sub1∪sub2 = set) a[sub1] b[sub2]
* なる c[0..2^n) を返す．
*
* 利用：【ゼータ変換（下位集合）】，【メビウス変換（下位集合）】
*/
template <class T> vector<T> or_convolution(vector<T> a, vector<T> b) {
    // 参考 : https://kazuma8128.hatenablog.com/entry/2018/05/31/144519
    // verify : https://judge.yosupo.jp/problem/bitwise_and_convolution

    int n = msb(sz(a));

    subset_zeta(a);
    subset_zeta(b);

    repb(set, n) a[set] *= b[set];

    subset_mobius(a);

    return a;
}


//【max ゼータ変換（下位集合）】O(2^n n)
/*
* a[0..2^n) を
*       A[set] = MAX(sub⊂set) a[sub]
* なる A[0..2^n) に上書きする．
*/
template <class T> void subset_max_zeta(vector<T>& a) {
     int n = msb(sz(a));

    rep(i, n) {
        repb(set, n) {
            if (!(set & (1 << i))) chmax(a[set + (1 << i)], a[set]);
        }
    }
}


//【下位和集合トロピカル畳込み】O(2^n n)
/*
* 与えられた a[0..2^n), b[0..2^n) に対して
*       c[set] = MAX(sub1∪sub2 ⊂ set) (a[sub1] + b[sub2])
* なる c[0..2^n) を返す．
*
* 利用：【max ゼータ変換（下位集合）】
*/
template <class T> vector<T> subset_or_tropical_convolution(vector<T> a, vector<T> b) {
    int n = msb(sz(a));

    subset_max_zeta(a);
    subset_max_zeta(b);

    repb(set, n) a[set] += b[set];

    return a;
}


//【下位和集合トロピカル畳込み（重複禁止）】O(2^n n)
/*
* 与えられた a[0..2^n) に対して
*       b[set] = MAX(sub1∪sub2 ⊂ set, sub1!=sub2) (a[sub1] + a[sub2])
* なる b[0..2^n) を返す．
*/
template <class T> vector<T> distinct_subset_or_tropical_convolution(const vector<T>& a) {
    // verify : https://atcoder.jp/contests/arc100/tasks/arc100_c

    int n = msb(sz(a));
    T T_MIN = numeric_limits<T>::min();

    // A[set] : sub⊂set についての a[sub] の {max, 2nd max}
    vector<pair<T, T>> A(1LL << n);
    rep(x, n) {
        repb(set, n) {
            A[set] = { a[set], T_MIN };
        }
    }

    // 下位集合での max ゼータ変換を行う．
    rep(i, n) {
        repb(set, n) {
            if (!(set & (1 << i))) {
                int nset = set + (1 << i);

                vector<T> vals(4);
                vals[0] = A[nset].first;
                vals[1] = A[nset].second;
                vals[2] = A[set].first;
                vals[3] = A[set].second;
                sort(all(vals), greater<T>());

                A[nset] = { vals[0], vals[1] };
            }
        }
    }
    dump(A);

    vector<T> b(1LL << n);
    repb(set, n) {
        b[set] = A[set].first + A[set].second;
    }
    b[0] = T_MIN;

    return b;
}


//【非交和畳込み】O(2^n n^2)
/*
* 与えられた a[0..2^n), b[0..2^n) に対して
*       c[set] = Σ(sub1凵sub2 = set) a[sub1] b[sub2]
* なる c[0..2^n) を返す．
* 凵 は集合の非交和であり，sub1∪sub2 = set かつ sub1∩sub2 = {} であることを表す．
*
* 利用：【形式的冪級数（可換環）】，【和集合畳込み】
*/
template <class T> T add_duc(T x, T y) { return x + y; }
template <class T> T o_duc() { return 0; }
template <class T> T mi_duc(T x) { return -x; }
template <class T> T mul_duc(T x, T y) { return x * y; }
template <class T> T e_duc() { return 1; }
#define Add_mul_cring_duc T, add_duc, o_duc, mi_duc, mul_duc, e_duc
template <class T> vector<T> disjoint_union_convolution(const vector<T>& a, const vector<T>& b) {
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


//【アダマール変換】: O(2^n n)
/*
* a[0..2^n) をアダマール変換したものに上書きする．
*/
template <class T> void hadamard(vector<T>& a) {
    // verify : https://judge.yosupo.jp/problem/bitwise_xor_convolution

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

    int n = msb(sz(a));

    rep(i, n) {
        repb(set, n) {
            if (!(set & (1 << i))) {
                T x = a[set], y = a[set | (1 << i)];
                a[set] = x + y, a[set + (1 << i)] = x - y;
            }
        }
    }
}


//【逆アダマール変換】: O(2^n n)
/*
* A[0..2^n) を逆アダマール変換したものに上書きする．
*/
template <class T> void hadamard_inv(vector<T>& A) {
    // verify : https://judge.yosupo.jp/problem/bitwise_xor_convolution
    
    int n = msb(sz(A));

    rep(i, n) {
        repb(set, n) {
            if (!(set & (1 << i))) {
                T x = A[set], y = A[set | (1 << i)];
                A[set] = (x + y) / 2, A[set + (1 << i)] = (x - y) / 2;
            }
        }
    }
}


//【対称差畳込み】O(2^n n)
/*
* 与えられた a[0..2^n), b[0..2^n) に対して
*       Σ(set1△set2 = set) a[set1] b[set2]
* なる c[0..2^n) を返す．
* △ は集合の対称差であり，添字でいうと XOR である．
*
* 利用：【アダマール変換】，【逆アダマール変換】
*/
template <class T> vector<T> symmetric_difference_convolution(vector<T> a, vector<T> b) {
    // 参考 : https://kazuma8128.hatenablog.com/entry/2018/05/31/144519
    // verify : https://judge.yosupo.jp/problem/bitwise_xor_convolution

    int n = msb(sz(a));

    hadamard(a);
    hadamard(b);

    repb(set, n) a[set] *= b[set];

    hadamard_inv(a);

    return a;
}


