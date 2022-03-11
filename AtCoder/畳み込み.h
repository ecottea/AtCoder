#pragma once
#include "header.h"
// ■■■■■ 畳み込み ■■■■■


//【mod 998244353 での畳込み】O((|a| + |b|) log(|a| + |b|))
/*
* ACL の vm convolution(vm a, vm b) を利用すればよい．
* 
* 制約：|a| + |b| - 1 <= 8388608 = 2^23
*/


//【mod 998244353 での畳込み（やや長い配列）】O((n + m) log(n + m))
/*
* a[0..n) と b[0..m) の mod 998244353 での畳み込みを res[0..n+m-1) に格納する．
*
* 制約：n + m - 1 <= 16777216 = 2^24
*/
void convolution998244353_long(vm& a, vm& b, vm& res) {
	// verify : https://atcoder.jp/contests/abc240/tasks/abc240_g

	//【方法】
	// ACL の convolution() が結果の大きさ 2^23 以下までしか対応していないので，
	// もしそれより大きい結果になりそうなら列を等分し，畳み込み結果を統合する．

	int n = sz(a), m = sz(b);

	// 畳み込み結果の大きさが 2^23 以下なら ACL が対応している．
	if (n + m - 1 <= (1 << 23)) {
		res = convolution(a, b);
		return;
	}

	res.resize(n + m - 1);

	// |a| >= |b| とする．
	bool swap_flag = false;
	if (n < m) {
		a.swap(b);
		swap_flag = true;
	}

	// a を等分する．
	int n1 = n / 2, n2 = n - n1;
	vm a1(n1), a2(n2);
	rep(i, n1) a1[i] = a[i];
	rep(i, n2) a2[i] = a[n1 + i];

	// a を等分することにより畳み込み結果の大きさが 2^23 以下になる場合は b はそのまま
	if (n2 + m - 1 <= (1 << 23)) {
		vm c1 = convolution(a1, b);
		vm c2 = convolution(a2, b);

		if (!c1.empty()) rep(i, n1 + m - 1) res[i] += c1[i];
		if (!c2.empty()) rep(i, n2 + m - 1) res[n1 + i] += c2[i];
	}
	// そうでないなら a, b 共に等分する．
	else {
		int m1 = m / 2, m2 = m - m1;
		vm b1(m1), b2(m2);
		rep(i, m1) b1[i] = b[i];
		rep(i, m2) b2[i] = b[m1 + i];

		vm c11 = convolution(a1, b1);
		vm c12 = convolution(a1, b2);
		vm c21 = convolution(a2, b1);
		vm c22 = convolution(a2, b2);

		if (!c11.empty()) rep(i, n1 + m1 - 1) res[i] += c11[i];
		if (!c12.empty()) rep(i, n1 + m2 - 1) res[m1 + i] += c12[i];
		if (!c21.empty()) rep(i, n2 + m1 - 1) res[n1 + i] += c21[i];
		if (!c22.empty()) rep(i, n2 + m2 - 1) res[n1 + m1 + i] += c22[i];
	}

	if (swap_flag) a.swap(b);
}


//【mod 998244353 での畳込み（長い配列）】O((n + m)^2)（遅い）
/*
* a[0..n) と b[0..m) の mod 998244353 での畳み込みを res[0..n+m-1) に格納する．
*/
void convolution998244353_long(const vm& a, const vm& b, vm& res) {
	// verify : https://atcoder.jp/contests/abc240/tasks/abc240_g

	//【方法】
	// ACL の convolution() が結果の大きさ 2^23 以下までしか対応していないので，
	// もしそれより大きい結果になりそうなら列を分割し，畳み込み結果を統合する．

	const int len = (1 << 22);
	int n = sz(a), m = sz(b);
	res.resize(n + m - 1);

	for (int i = 0; i < n; i += len) {
		vm a_sub{ a.begin() + i, a.begin() + min(i + len, n) };

		for (int j = 0; j < m; j += len) {
			vm b_sub{ b.begin() + j, b.begin() + min(j + len, m) };

			vm c = convolution(a_sub, b_sub);

			rep(k, sz(c)) res[i + j + k] += c[k];
		}
	}
}


//【添字 xor での畳込み】
/*
* vT convolution_xor(vT a, vT b) : O(n log n)
*   c[k] = Σ_(i xor j = k) a[i] b[j] なる c[0..n) を返す．
*
* fwt_xor(vT& a) : O(n log n)
*   a[0..n) を高速アダマール変換する．
*
* ifwt_xor(vT& A) : O(n log n)
*   A[0..n) を逆高速アダマール変換する．
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
	// verify : https://judge.yosupo.jp/problem/bitwise_xor_convolution

    int n = sz(a);
    fwt_xor(a); fwt_xor(b); // A = fwt_xor(a), B = fwt_xor(b) を計算 : O(n log n)
    rep(i, n) a[i] *= b[i]; // C[k] = A[k] * B[k] を計算 : O(n)
    ifwt_xor(a);            // c = ifwt_xor(C) を計算 : O(n log n)
    return a;
}


//【上位集合，添字 and での畳込み】
/*
* vT convolution_and(vT a, v T b) : O(n log n)
*   c[k] = Σ_(i and j = k) a[i] b[j] なる c[0..n) を返す．
*
* fwt_and(vT& a) : O(n log n)
*   A[j] = Σ_(i ⊃ j) a[i] なる A[0..n) に上書きする．
*  （上位集合での高速ゼータ変換）
*
* ifwt_and(vT& A) : O(n log n)
*   a[j] = Σ_(i ⊃ j) (-1)^(i - j) A[i] なる a[0..n) に上書きする．
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
	// verify : https://judge.yosupo.jp/problem/bitwise_and_convolution

    int n = sz(a);
    fwt_and(a); fwt_and(b);
    rep(i, n) a[i] *= b[i];
    ifwt_and(a);
    return a;
}


//【下位集合，添字 or での畳込み】
/*
* vT convolution_or(vT a, vT b) : O(n log n)
*   c[k] = Σ_(i or j = k) a[i] b[j] なる c[0..n) を返す．
*
* fwt_or(vT& a) : O(n log n)
*   A[j] = Σ_(i ⊂ j) a[i] なる A[0..n) に上書きする．
*  （下位集合での高速ゼータ変換）
*
* ifwt_or(vT& A) : O(n log n)
*   a[j] = Σ_(i ⊂ j) (-1)^(i - j) A[i] なる a[0..n) に上書きする．
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


//【添字 min での畳込み】
/*
* convolution_min(a, b) : O(n)
*   c[k] = Σ_(min(i, j) = k) a[i] b[j] なる c を返す．
*
* zeta_min(a) : O(n)
*   A[j] = Σ_(i >= j) a[i] なる A に上書きする．
*  （上位ゼータ変換，上からの累積和）
*
* mobius_min(A) : O(n)
*   A[j] = Σ_(i >= j) a[i] なる a に上書きする．
*  （上位メビウス変換，下からの差分）
*/
template <typename T> void zeta_min(vector<T>& f) {
	int n = sz(f);
	repir(i, n - 2, 0) f[i] += f[i + 1];
}
template <typename T> void mobius_min(vector<T>& f) {
	int n = sz(f);
	repi(i, 0, n - 2) f[i] -= f[i + 1];
}
template <typename T> vector<T> convolution_min(vector<T> a, vector<T> b) {
	// 参考 : https://qiita.com/convexineq/items/afc84dfb9ee4ec4a67d5

	int n = sz(a);
	zeta_min(a); zeta_min(b);
	rep(i, n) a[i] *= b[i];
	mobius_min(a);
	return a;
}


//【添字 max での畳込み】
/*
* convolution_max(a, b) : O(n)
*   c[k] = Σ_(max(i, j) = k) a[i] b[j] なる c を返す．
*
* zeta_max(a) : O(n)
*   A[j] = Σ_(i <= j) a[i] なる A に上書きする．
*  （下位ゼータ変換，下からの累積和）
*
* mobius_max(A) : O(n)
*   A[j] = Σ_(i <= j) a[i] なる a に上書きする．
*  （下位メビウス変換，上からの差分）
*/
template <typename T> void zeta_max(vector<T>& f) {
	int n = sz(f);
	repi(i, 1, n - 1) f[i] += f[i - 1];
}
template <typename T> void mobius_max(vector<T>& f) {
	int n = sz(f);
	repir(i, n - 1, 1) f[i] -= f[i - 1];
}
template <typename T> vector<T> convolution_max(vector<T> a, vector<T> b) {
	// 参考 : https://qiita.com/convexineq/items/afc84dfb9ee4ec4a67d5

	int n = sz(a);
	zeta_max(a); zeta_max(b);
	rep(i, n) a[i] *= b[i];
	mobius_max(a);
	return a;
}


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
* convolution(a, b) : O(n (log n)^2) ?
*	a と b の畳み込み積を返す．
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
			r[i] = r[i + 1] * r[i + 1];
			r_inv[i] = r_inv[i + 1] * r_inv[i + 1];
		}
	}

	// x を (y, z) に分割する
	void butterfly(const vm& x, vm& y, vm& z) {
		int n = sz(x) / 2;
		y = z = vm(n);

		rep(i, n) {
			y[i] = x[i] + x[i + n];
			z[i] = (x[i] - x[i + n]) * r[msb(n) + 1].pow(i); // ここが遅い
		}
	}

	// x を (y, z) に分割する（逆変換用）
	void butterfly_inv(const vm& x, vm& y, vm& z) {
		int n = sz(x) / 2;
		y = z = vm(n);

		rep(i, n) {
			y[i] = x[i] + x[i + n];
			z[i] = (x[i] - x[i + n]) * r_inv[msb(n) + 1].pow(i); // ここが遅い
		}
	}

	// (y, z) を x に統合する
	void riffle(const vm& y, const vm& z, vm& x) {
		int n = sz(y);
		x = vm(2 * n);

		rep(i, n) {
			x[2 * i] = y[i];
			x[2 * i + 1] = z[i];
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

	// a と b の畳み込み積を返す．
	vm convolution(vm a, vm b) {
		// verify : https://judge.yosupo.jp/problem/convolution_mod

		int n = sz(a), m = sz(b);

		int k = 1 << (msb(n + m - 2) + 1);
		a.resize(k);
		b.resize(k);
		vm A, B;
		ntt(a, A);
		ntt(b, B);
		rep(i, sz(A)) A[i] *= B[i];
		intt(A, a);
		a.resize(n + m - 1);

		return a;
	}
};


