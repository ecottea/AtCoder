#pragma once
#include "header.h"
// ■■■■■ 畳み込み ■■■■■



//【数論変換】
/*
* NTT() : O(1)
*	初期化を行う．
*
* ntt(const vi& a, vi& A) : O(n log n log n) ?
*	長さが 2 冪の列 a に対し mod 998244353 で数論変換を行った結果を A に格納する．
*
* intt(const vi& A, vi& a) : O(n log n log n) ?
*	長さが 2 冪の列 A に対し mod 998244353 で逆数論変換を行った結果を a に格納する．
*/
struct NTT {
	// 参考 : https://qiita.com/Sen_comp/items/9401382df736e51564c1

	using mint = modint998244353;
	using vm = vector<mint>;

	// root[i] : 1 の原始 2^i 乗根（i ≦ 23）
	vm r, r_inv;

	NTT() : r(24), r_inv(24) {
		// 1 の原始 2^23 乗根
		// 998244353 = 2^23 * 119 + 1 なので，原始根 3 の 119 乗を計算することで求まる．
		r[23] = mint(3).pow(119);
		r_inv[23] = r[23].inv();

		repir(i, 22, 0) {
			r[i] = r[i + 1LL] * r[i + 1LL];
			r_inv[i] = r_inv[i + 1LL] * r_inv[i + 1LL];
		}
	}

	// x を (y, z) に分割する
	void butterfly(const vm& x, vm& y, vm& z) {
		int n = sz(x) / 2;
		y = z = vm(n);

		rep(i, n) {
			y[i] = x[i] + x[(ll)i + n];
			z[i] = (x[i] - x[(ll)i + n]) * r[msb(n) + 1LL].pow(i); // ここが遅い
		}
	}

	// x を (y, z) に分割する（逆変換用）
	void butterfly_inv(const vm& x, vm& y, vm& z) {
		int n = sz(x) / 2;
		y = z = vm(n);

		rep(i, n) {
			y[i] = x[i] + x[(ll)i + n];
			z[i] = (x[i] - x[(ll)i + n]) * r_inv[msb(n) + 1LL].pow(i); // ここが遅い
		}
	}

	// (y, z) を x に統合する
	void riffle(const vm& y, const vm& z, vm& x) {
		int n = sz(y);
		x = vm(2LL * n);

		rep(i, n) {
			x[2LL * i] = y[i];
			x[2LL * i + 1] = z[i];
		}
	}

	// 長さが 2 冪の列 a に対し mod 998244353 で数論変換を行った結果を A に格納する．
	void ntt(const vm& a, vm& A) {
		int n = sz(a);
		if (n == 1) {
			A = a;
			return;
		}

		vm b, c, B, C;
		butterfly(a, b, c);
		ntt(b, B);
		ntt(c, C);
		riffle(B, C, A);
	}

	// 長さが 2 冪の列 A に対し mod 998244353 で逆数論変換を行った結果を a に格納する．
	void intt(const vm& A, vm& a) {
		intt_sub(A, a);

		// 定数倍の調整
		mint n_inv = mint(sz(A)).inv();
		rep(i, sz(A)) {
			a[i] *= n_inv;
		}
	}
	void intt_sub(const vm& A, vm& a) {
		int n = sz(A);
		if (n == 1) {
			a = A;
			return;
		}

		vm b, c, B, C;
		butterfly_inv(A, B, C);
		intt_sub(B, b);
		intt_sub(C, c);
		riffle(b, c, a);
	}
};


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


