#pragma once
#include "header.h"
#include "数論.h"
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
* 
* 制約 : n は 2 の冪乗
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
* 
* 制約 : n は 2 の冪乗
*/
template <typename T> void fwt_and(vector<T>& f) {
	int n = sz(f);
    for (int i = 1; i < n; i <<= 1) {
        rep(j, n) {
            if ((j & i) == 0) f[j] += f[j | i];
        }
    }
}
template <typename T> void ifwt_and(vector<T>& f) {
    int n = sz(f);
    for (int i = 1; i < n; i <<= 1) {
        rep(j, n) {
            if ((j & i) == 0) f[j] -= f[j | i];
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
* 
* 制約 : n は 2 の冪乗
*/
template <typename T> void fwt_or(vector<T>& f) {
	int n = sz(f);
    for (int i = 1; i < n; i <<= 1) {
        rep(j, n) {
            if ((j & i) == 0) f[j | i] += f[j];
        }
    }
}
template <typename T> void ifwt_or(vector<T>& f) {
    int n = sz(f);
    for (int i = 1; i < n; i <<= 1) {
        rep(j, n) {
            if ((j & i) == 0) f[j | i] -= f[j];
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


//【添字 max での畳込み】
/*
* convolution_max(a, b) : O(n)
*   c[k] = Σ_(max(i, j) = k) a[i] b[j] なる c を返す．
*
* zeta_max(a) : O(n)
*   A[j] = Σ_(i <= j) a[i] なる A を返す．
*  （ゼータ変換，下からの累積和）
*
* mobius_max(A) : O(n)
*   A[j] = Σ_(i <= j) a[i] なる a を返す．
*  （メビウス変換，上からの差分）
*/
template <typename T> void zeta_max(vector<T>& f) {
	int n = sz(f);
	repi(i, 1, n - 1) f[i] += f[i - 1LL];
}
template <typename T> void mobius_max(vector<T>& f) {
	int n = sz(f);
	repir(i, n - 1, 1) f[i] -= f[i - 1LL];
}
template <typename T> vector<T> convolution_max(vector<T> a, vector<T> b) {
	// 参考 : https://qiita.com/convexineq/items/afc84dfb9ee4ec4a67d5

	int n = sz(a);
	zeta_max(a); zeta_max(b);
	rep(i, n) a[i] *= b[i];
	mobius_max(a);
	return a;
}


//【添字 min での畳込み】
/*
* convolution_min(a, b) : O(n)
*   c[k] = Σ_(min(i, j) = k) a[i] b[j] なる c を返す．
*
* zeta_min(a) : O(n)
*   A[j] = Σ_(i >= j) a[i] なる A を返す．
*  （ゼータ変換，上からの累積和）
*
* mobius_min(A) : O(n)
*   A[j] = Σ_(i >= j) a[i] なる a を返す．
*  （メビウス変換，下からの差分）
*/
template <typename T> void zeta_min(vector<T>& f) {
	int n = sz(f);
	repir(i, n - 2, 0) f[i] += f[i + 1LL];
}
template <typename T> void mobius_min(vector<T>& f) {
	int n = sz(f);
	repi(i, 0, n - 2) f[i] -= f[i + 1LL];
}
template <typename T> vector<T> convolution_min(vector<T> a, vector<T> b) {
	// 参考 : https://qiita.com/convexineq/items/afc84dfb9ee4ec4a67d5

	int n = sz(a);
	zeta_min(a); zeta_min(b);
	rep(i, n) a[i] *= b[i];
	mobius_min(a);
	return a;
}


//【添字 gcd での畳込み】
/*
* GCD_convolution(n) : O(n log(log n))
*   n までの素数を持って初期化する．
*
* convolution_gcd(a, b) : O(n log(log n))
*   c[k] = Σ_(gcd(i, j) = k) a[i] b[j] なる c を返す．
*
* zeta(a) : O(n log(log n))
*   A[j] = Σ_(j | i) a[i] なる A を返す．
*  （ゼータ変換，約数への累積和）
*
* mobius(A) : O(n log(log n))
*   A[j] = Σ_(j | i) a[i] なる a を返す．
*  （メビウス変換，倍数への差分）
*
* 制約：1-indexed とし，a[0], b[0] は使用しない．
*
* 利用：【素数の列挙／エラトステネスの篩】
*/
template <typename T> struct GCD_convolution {
	// 参考 : https://qiita.com/convexineq/items/afc84dfb9ee4ec4a67d5

	vi ps; // 素数のリスト

	GCD_convolution() {}
	GCD_convolution(int n) { eratosthenes(n, ps); }

	void zeta(vector<T>& f) {
		int n = sz(f);

		// 各素因数ごとに上からの累積和をとる
		repe(p, ps) {
			repir(i, (n - 1) / p, 1) f[i] += f[(ll)p * i];
		}
	}

	void mobius(vector<T>& f) {
		int n = sz(f);

		// 各素因数ごとに下からの差分をとる
		repe(p, ps) {
			repi(i, 1, (n - 1) / p) f[i] -= f[(ll)p * i];
		}
	}

	vector<T> convolution_gcd(vector<T> a, vector<T> b) {
		int n = sz(a);

		// 各素因数の min をとったものが gcd なので min 畳み込みを行う．
		zeta(a); zeta(b);
		rep(i, n) a[i] *= b[i];
		mobius(a);
		return a;
	}
};


//【添字 lcm での畳込み】
/*
* LCM_convolution(n) : O(n log(log n))
*   n までの素数を持って初期化する．
*
* convolution_lcm(a, b) : O(n log(log n))
*   c[k] = Σ_(lcm(i, j) = k) a[i] b[j] なる c を返す．
*   ただし c[n] を含めそれ以降は切り捨てる．
*
* zeta(a) : O(n log(log n))
*   A[j] = Σ_(i | j) a[i] なる A を返す．
*  （ゼータ変換，倍数への累積和）
*
* mobius(A) : O(n log(log n))
*   A[j] = Σ_(i | j) a[i] なる a を返す．
*  （メビウス変換，約数への差分）
*
* 制約：1-indexed とし，a[0], b[0] は使用しない．
*
* 利用：【素数の列挙／エラトステネスの篩】
*/
template <typename T> struct LCM_convolution {
	// 参考 : https://qiita.com/convexineq/items/afc84dfb9ee4ec4a67d5

	vi ps; // 素数のリスト

	LCM_convolution() {}
	LCM_convolution(int n) { eratosthenes(n, ps); }

	void zeta(vector<T>& f) {
		int n = sz(f);

		// 各素因数ごとに下からの累積和をとる
		repe(p, ps) {
			repi(i, 1, (n - 1) / p) f[(ll)p * i] += f[i];
		}
	}

	void mobius(vector<T>& f) {
		int n = sz(f);

		// 各素因数ごとに上からの差分をとる
		repe(p, ps) {
			repir(i, (n - 1) / p, 1) f[(ll)p * i] -= f[i];
		}
	}

	vector<T> convolution_lcm(vector<T> a, vector<T> b) {
		int n = sz(a);

		// 各素因数の max をとったものが lcm なので max 畳み込みを行う．
		zeta(a); zeta(b);
		rep(i, n) a[i] *= b[i];
		mobius(a);
		return a;
	}
};


//【数論変換】
/*
* NTT() : O(1)
*	1 の原始 2^i 乗根（i ≦ 23）を持って初期化を行う．
*
* ntt(const vi& a, vi& A) : O(n (log n)^2) ?
*	a に対し mod 998244353 で数論変換を行った結果を A に格納する．
*
* intt(const vi& A, vi& a) : O(n (log n)^2) ?
*	A に対し mod 998244353 で逆数論変換を行った結果を a に格納する．
*
* 制約 : n は 2 の冪乗
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


