#pragma once
#include "header.h"
// ■■■■■ 畳込み ■■■■■


//【畳込みの使い所】
/*
* 数列 a, b が与えられているとする．
* 畳込みは，全ての k <= n についての Σi a[i] b[k-i] を一括 O(n log n) で求めたいときに使う．
* 特定の k に対して求めたいだけであれば，愚直に和をとっても O(k) で計算できる．
* 
* verify : https://atcoder.jp/contests/abc276/tasks/abc276_g
*/


//【畳込み】O(n m)
/*
* a[0..n) と b[0..m) を畳み込んだ数列 c[0..n+m-1) を返す．
*/
template <class T> vector<T> naive_convolution(const vector<T>& a, const vector<T>& b) {
	// verify : https://atcoder.jp/contests/abc214/tasks/abc214_g

	int n = sz(a), m = sz(b);

	// c[i] = Σj∈[0..i] a[j] b[i-j]  (∀i∈[0..n+m-1))
	vector<T> c(n + m - 1);
	rep(i, n + m - 1) {
		repi(j, max(i - (m - 1), 0), min(i, n - 1)) {
			c[i] += a[j] * b[i - j];
		}
	}

	return c;
}


//【上側畳込み】
/*
* 与えられた a[0..n], b[0..n] に対して
*		c[i] = Σj∈[i..n] a[n+i-j] b[j]
* なる c[0..n] を求めたい場合，convolution(a, b)[n..2n] を取得すればよい．
* 
* verify : https://atcoder.jp/contests/abc217/tasks/abc217_g
*/


//【自己畳込み】O(n^2 log k)
/*
* a[0..n) を k 個畳み込んだ数列 c[0..n) を返す．
*
* 利用：【畳込み】
*
*（繰り返し二乗法）
*/
template <class T> vector<T> naive_self_convolution(const vector<T>& a, ll k) {
	// verify : https://atcoder.jp/contests/arc059/tasks/arc059_d

	int n = sz(a);

	vector<T> res(n);
	res[0] = 1;

	vector<T> pow2(a);
	while (k > 0) {
		if ((k & 1) != 0) {
			res = naive_convolution(res, pow2);
			res.resize(n);
		}

		pow2 = naive_convolution(pow2, pow2);
		pow2.resize(n);

		k /= 2;
	}

	return res;
}


//【畳込み（mod 998244353，長さ 4,194,304 以下）】O((|a| + |b|) log(|a| + |b|))
/*
* ACL の vm convolution(vm a, vm b) を利用すればよい．
* 
* 制約：|a| + |b| - 1 <= 8388608 = 2^23
*/


//【畳込み（mod 998244353，長さ 8,388,608 以下）】O(3 (n + m) log(n + m))
/*
* a[0..n) と b[0..m) の mod 998244353 での畳込みを c[0..n+m-1) に格納し c を返す．
*
* 制約：n, m <= 8,388,608 = 2^24
*/
vm convolution_large(const vm& a, const vm& b) {
	// verify : https://atcoder.jp/contests/abc240/tasks/abc240_g

	//【方法】
	// ACL の convolution() が結果の大きさ 2^23 以下までしか対応していないので，
	// 列を分割し，カラツバ法っぽく畳込み結果を統合する．

	int n = sz(a), m = sz(b), d = 1 << 22;
	vm c(n + m - 1);

	// a を分割する．
	int n1 = min(d, n), n2 = n - n1;
	vm a1(n1), a2(n2);
	rep(i, n1) a1[i] = a[i];
	rep(i, n2) a2[i] = a[n1 + i];

	// b を分割する．
	int m1 = min(d, m), m2 = m - m1;
	vm b1(m1), b2(m2);
	rep(j, m1) b1[j] = b[j];
	rep(j, m2) b2[j] = b[m1 + j];

	vm c11 = convolution(a1, b1);
	vm c22 = convolution(a2, b2);

	// a, b の母関数をそれぞれ
	//		f(z) = f1(z) + f2(z) z^d
	//		g(z) = g1(z) + g2(z) z^d
	// とおく．これらの積は
	//		f(z) g(z)
	//		= (f1 + f2 z^d) (g1 + g2 z^d)
	//		= f1 g1 + (f1 g2 + f2 g1) z^d + (f2 g2) z^2d
	// と表される．さらに
	//		f1 g2 + f2 g1 = (f1 + f2)(g1 + g2) - f1 g1 - f2 g2
	// と書き直せば，積の回数を計 3 回に抑えられる．
	rep(i, n2) a1[i] += a2[i];
	rep(j, m2) b1[j] += b2[j];
	vm c12 = convolution(a1, b1);
	if (!c11.empty()) rep(k, n1 + m1 - 1) c12[k] -= c11[k];
	if (!c22.empty()) rep(k, n2 + m2 - 1) c12[k] -= c22[k];

	if (!c11.empty()) rep(k, n1 + m1 - 1) c[k] += c11[k];
	if (!c12.empty()) rep(k, min(n1 + m1 - 1, n + m - 1 - m1)) c[m1 + k] += c12[k];
	if (!c22.empty()) rep(k, n2 + m2 - 1) c[n1 + m1 + k] += c22[k];

	return c;
}


//【畳込み（mod 998244353，長さ 16,777,216 以下）】O(9 (n + m) log(n + m))
/*
* a[0..n) と b[0..m) の mod 998244353 での畳込みを c[0..n+m-1) に格納し c を返す．
*
* 制約：n, m <= 16,777,216 = 2^24
*/
vm convolution_verylarge(const vm& a, const vm& b) {
	// verify : https://judge.yosupo.jp/problem/convolution_mod_large
	
	//【方法】
	// ACL の convolution() が結果の大きさ 2^23 以下までしか対応していないので，
	// 列を長さ 2^22 以下に分割し，カラツバ法っぽく畳込み結果を統合する．

	int n = sz(a), m = sz(b);
	int d = (max(n, m) + 3) / 4;

	// a を分割する．
	vm a0(d), a1(d), a2(d), a3(d);
	repi(i, 0, min(d, n) - 1)			a0[i] = a[i];
	repi(i, d, min(2 * d, n) - 1)		a1[i - d] = a[i];
	repi(i, 2 * d, min(3 * d, n) - 1)	a2[i - 2 * d] = a[i];
	repi(i, 3 * d, n - 1)				a3[i - 3 * d] = a[i];

	// b を分割する．
	vm b0(d), b1(d), b2(d), b3(d);
	repi(i, 0, min(d, m) - 1)			b0[i] = b[i];
	repi(i, d, min(2 * d, m) - 1)		b1[i - d] = b[i];
	repi(i, 2 * d, min(3 * d, m) - 1)	b2[i - 2 * d] = b[i];
	repi(i, 3 * d, m - 1)				b3[i - 3 * d] = b[i];

	// 列 x に列 y を加える．
	auto add = [&](vm& x, const vm& y, int l = 0) {
		rep(i, sz(y)) x[l + i] += y[i];
	};

	// 列 x から列 y を引く．
	auto sub = [&](vm& x, const vm& y, int l = 0) {
		rep(i, sz(y)) x[l + i] -= y[i];
	};

	// 列 x と列 y を加えた結果を返す．
	auto sum = [&](const vm& x, const vm& y) {
		vm z(d);
		rep(i, d) z[i] = x[i] + y[i];
		return z;
	};

	// hij = (fi + fj)(gi + gj) など
	vm h0 = convolution(a0, b0);
	vm h1 = convolution(a1, b1);
	vm h2 = convolution(a2, b2);
	vm h3 = convolution(a3, b3);
	vm h02 = convolution(sum(a0, a2), sum(b0, b2));
	vm h13 = convolution(sum(a1, a3), sum(b1, b3));
	add(a0, a1); add(b0, b1); add(a2, a3); add(b2, b3);
	vm h01 = convolution(a0, b0);
	vm h23 = convolution(a2, b2);
	add(a0, a2); add(b0, b2);
	vm h0123 = convolution(a0, b0);

	vm c01(4 * d);
	add(c01, h0);
	add(c01, h01, d); sub(c01, h0, d); sub(c01, h1, d);
	add(c01, h1, 2 * d);

	vm c23(4 * d);
	add(c23, h2);
	add(c23, h23, d); sub(c23, h2, d); sub(c23, h3, d);
	add(c23, h3, 2 * d);

	vm c0123(4 * d);
	add(c0123, h02);
	add(c0123, h0123, d); sub(c0123, h02, d); sub(c0123, h13, d);
	add(c0123, h13, 2 * d);

	vm c(8 * d);
	add(c, c01);
	add(c, c0123, 2 * d); sub(c, c01, 2 * d); sub(c, c23, 2 * d);
	add(c, c23, 4 * d);

	c.resize(n + m - 1);
	return c;
}


//【複数の数列の畳込み（mod 998244353）】O(n (log n)^2)
/*
* 数列の集合 a の要素を全て畳込んだ結果（長さは n）を返す．
*/
vm multi_convoluion(vvm a) {
	// verify : https://atcoder.jp/contests/abl/tasks/abl_f

	int m = sz(a);

	// (要素数, 数列の番号) の組を要素数昇順に記録する．
	priority_queue_rev<pii> q;
	rep(i, m) q.push({ sz(a[i]), i });

	while (sz(q) >= 2) {
		int ni, i, nj, j;
		tie(ni, i) = q.top(); q.pop();
		tie(nj, j) = q.top(); q.pop();

		a[i] = convolution(a[i], a[j]);
		q.push({ ni + nj - 1, i });
	}

	return a[q.top().second];
}


//【二次元畳込み】O(h1 w1 h2 w2)
/*
* a[0..h1)[0..w1) と b[0..h2)[0..w2) を畳み込んだ二次元配列 c[0..h1+h2-1)[0..w1+w2-1) を返す．
*/
template <class T>
vector<vector<T>> naive_convolution_2D(const vector<vector<T>>& a, const vector<vector<T>>& b) {
	int h1 = sz(a), w1 = sz(a[0]), h2 = sz(b), w2 = sz(b[0]);

	// c[i][j] = Σs Σt a[s][t] b[i-s][j-t]  (∀i∈[0..h1+h2-1), j∈[0..w1+w2-1))
	vector<vector<T>> c(h1 + h2 - 1, vector<T>(w1 + w2 - 1));
	rep(i, h1 + h2 - 1) rep(j, w1 + w2 - 1) {
		repi(s, max(i - (h2 - 1), 0), min(i, h1 - 1)) repi(t, max(j - (w2 - 1), 0), min(j, w1 - 1)) {
			c[i][j] += a[s][t] * b[i - s][j - t];
		}
	}

	return c;
}


//【二次元畳込み（mod 998244353）】O((h1 + h2)(w1 + w2) log(h1 + h2) log(w1 + w2))
/*
* a[0..h1)[0..w1) と b[0..h2)[0..w2) を畳み込んだ二次元配列 c[0..h1+h2-1)[0..w1+w2-1) を返す．
*/
vvm convolution_2D(vvm a, vvm b) {
	// verify : https://atcoder.jp/contests/abc260/tasks/abc260_g

	int h1 = sz(a), w1 = sz(a[0]), h2 = sz(b), w2 = sz(b[0]);

	int h = 1 << (msb(h1 + h2 - 2) + 1);
	int w = 1 << (msb(w1 + w2 - 2) + 1);

	a.resize(h); b.resize(h);
	rep(i, h) { a[i].resize(w); b[i].resize(w); }

	// a, b の行方向の ntt
	rep(i, h1) internal::butterfly(a[i]);
	rep(i, h2) internal::butterfly(b[i]);

	// aT, bT : a, b の転置
	vvm aT(w, vm(h)), bT(w, vm(h));
	rep(i, h) rep(j, w) { aT[j][i] = a[i][j]; bT[j][i] = b[i][j]; }

	// aT, bT の行方向の ntt
	rep(j, w) internal::butterfly(aT[j]);
	rep(j, w) internal::butterfly(bT[j]);

	// cT = aT : aT, bT の各点積
	rep(j, w) rep(i, h) aT[j][i] *= bT[j][i];

	// cT の行方向の intt
	rep(j, w) internal::butterfly_inv(aT[j]);

	// c : cT の転置
	vvm c(h1 + h2 - 1, vm(w));
	rep(i, h1 + h2 - 1) rep(j, w) c[i][j] = aT[j][i];

	// c の行方向の intt，縮小，定数倍
	mint inv = mint(h * w).inv();
	rep(i, h1 + h2 - 1) {
		internal::butterfly_inv(c[i]);
		c[i].resize(w1 + w2 - 1);
		rep(j, w1 + w2 - 1) c[i][j] *= inv;
	}

	return c;
}


//【添字 xor での畳込み】
/*
*【対称差畳込み】を利用すればよい．
*/


//【添字 and での畳込み】
/*
*【積集合畳込み】を利用すればよい．
*/


//【添字 or での畳込み】
/*
*【和集合畳込み】を利用すればよい．
*/


//【添字 or での畳込み（and = 0 制約付き）】
/*
*【非交和畳込み】を利用すればよい．
*/


//【添字 min での畳込み】
/*
* min_zeta(a) : O(n)
*   A[j] = Σ_(i >= j) a[i] なる A に上書きする．
*  （上位ゼータ変換，上からの累積和）
*
* min_mobius(A) : O(n)
*   A[j] = Σ_(i >= j) a[i] なる a に上書きする．
*  （上位メビウス変換，下からの差分）
* 
*  min_convolution(a, b) : O(n)
*   c[k] = Σ_(min(i, j) = k) a[i] b[j] なる c を返す．
*/
template <typename T> void min_zeta(vector<T>& f) {
	// 具体例：
	//	A[0] = a[0] + a[1] + a[2] + a[3] + a[4] + a[5] + a[6] + a[7] + ...
	//	A[1] =        a[1] + a[2] + a[3] + a[4] + a[5] + a[6] + a[7] + ...
	//	A[2] =             + a[2] + a[3] + a[4] + a[5] + a[6] + a[7] + ...
	//	A[3] =                    + a[3] + a[4] + a[5] + a[6] + a[7] + ...
	//	A[4] =                           + a[4] + a[5] + a[6] + a[7] + ...
	//	A[5] =                                  + a[5] + a[6] + a[7] + ...
	//	A[6] =                                         + a[6] + a[7] + ...
	//	A[7] =                                                + a[7] + ...

	int n = sz(f);
	repir(i, n - 2, 0) f[i] += f[i + 1];
}
template <typename T> void min_mobius(vector<T>& f) {
	// 具体例：
	//	a[0] = A[0] - A[1]
	//	a[1] =        A[1] - A[2]
	//	a[2] =               A[2] - A[3]
	//	a[3] =                      A[3] - A[4]
	//	a[4] =                             A[4] - A[5]
	//	a[5] =                                    A[5] - A[6]
	//	a[6] =                                           A[6] - A[7]
	//	a[7] =                                                  A[7] - A[8]

	int n = sz(f);
	repi(i, 0, n - 2) f[i] -= f[i + 1];
}
template <typename T> vector<T> min_convolution(vector<T> a, vector<T> b) {
	// 参考 : https://qiita.com/convexineq/items/afc84dfb9ee4ec4a67d5

	int n = sz(a);

	min_zeta(a); min_zeta(b);
	rep(i, n) a[i] *= b[i];
	min_mobius(a);

	return a;
}


//【添字 max での畳込み】
/*
* max_zeta(a) : O(n)
*   A[j] = Σ_(i <= j) a[i] なる A に上書きする．
*  （下位ゼータ変換，下からの累積和）
*
* max_mobius(A) : O(n)
*   A[j] = Σ_(i <= j) a[i] なる a に上書きする．
*  （下位メビウス変換，上からの差分）
*
* max_convolution(a, b) : O(n)
*   c[k] = Σ_(max(i, j) = k) a[i] b[j] なる c を返す．
*/
template <typename T> void max_zeta(vector<T>& f) {
	// 具体例：
	//	A[0] = a[0]
	//	A[1] = a[0] + a[1]
	//	A[2] = a[0] + a[1] + a[2]
	//	A[3] = a[0] + a[1] + a[2] + a[3]
	//	A[4] = a[0] + a[1] + a[2] + a[3] + a[4]
	//	A[5] = a[0] + a[1] + a[2] + a[3] + a[4] + a[5]
	//	A[6] = a[0] + a[1] + a[2] + a[3] + a[4] + a[5] + a[6]
	//	A[7] = a[0] + a[1] + a[2] + a[3] + a[4] + a[5] + a[6] + a[7]

	int n = sz(f);
	repi(i, 1, n - 1) f[i] += f[i - 1];
}
template <typename T> void max_mobius(vector<T>& f) {
	// 具体例：
	//	a[0] =   A[0]
	//	a[1] = - A[0] + A[1]
	//	a[2] =        - A[1] + A[2]
	//	a[3] =               - A[2] + A[3]
	//	a[4] =                      - A[3] + A[4]
	//	a[5] =                             - A[4] + A[5]
	//	a[6] =                                    - A[5] + A[6]
	//	a[7] =                                           - A[6] + A[7]

	int n = sz(f);
	repir(i, n - 1, 1) f[i] -= f[i - 1];
}
template <typename T> vector<T> max_convolution(vector<T> a, vector<T> b) {
	// 参考 : https://qiita.com/convexineq/items/afc84dfb9ee4ec4a67d5

	int n = sz(a);

	max_zeta(a); max_zeta(b);
	rep(i, n) a[i] *= b[i];
	max_mobius(a);

	return a;
}


//【数論変換（mod 998244353）】
/*
* NTT(int n) : O(n)
*	長さ n 以下の数列を扱えるよう初期化を行う．
*
* ntt(vm a, vm& A) : O(n log n)
*	a[0..n) に対し mod 998244353 で数論変換を行った結果を A[0..n) に格納する．
*	制約 : n は 2 の冪乗
*
* intt(vm A, vm& a) : O(n log n)
*	A[0..n) に対し mod 998244353 で逆数論変換を行った結果を a[0..n) に格納する．
*	制約 : n は 2 の冪乗
*
* vm convolution(vm a, vm b) : O((n + m) log(n + m))
*	a[0..n) と b[0..m) の畳込みを返す．
*
* vm cyclic_convolution(vm a, vm b) : O(n log n)
*	a[0..n) と b[0..n) の巡回畳込みを返す．
*	制約 : n は 2 の冪乗
*
* vm cyclic_convolution_power(vm a, ll d) : O(n log n + n log d)
*	a[0..n) を d 個巡回畳込みした結果を返す．
*	制約 : n は 2 の冪乗
*/
struct NTT {
	// 参考 : https://qiita.com/Sen_comp/items/9401382df736e51564c1

	using mint = modint998244353;
	using vm = vector<mint>;

	// N : 扱える数列の長さの上限（N = 2^M）
	int N, M;

	// w : 1 の原始 2^M 乗根，w_pow[i] : w^i，w_pow[N-i] : w^(-i)
	mint w; vm w_pow;

	// 長さ n 以下の数列を扱えるよう初期化を行う．
	NTT(int n) {
		// verify : https://judge.yosupo.jp/problem/convolution_mod

		// n 以上の最小の 2 冪 2^M を求める．
		M = msb(n - 1) + 1;
		N = 1 << M;

		// 長さ 2^M の数列を扱うためには，1 の原始 2^M 乗根が必要．
		// 998244353 = 2^23 * 119 + 1 なので，原始根 3 の 119 * 2^(23-M) 乗を計算することで求まる．
		w = mint(3).pow(119LL << (23 - M));

		// w の累乗を前計算しておく．
		w_pow.resize(N + 1);
		w_pow[0] = 1;
		repi(i, 1, N) w_pow[i] = w_pow[i - 1] * w;
	}

	// x を (y, z) に分割する
	void butterfly(const vm& x, vm& y, vm& z) {
		int n = sz(x) / 2, m = msb(n) + 1;
		y.resize(n); z.resize(n);

		rep(i, n) {
			y[i] = x[i] + x[i + n];
			z[i] = (x[i] - x[i + n]) * w_pow[(ll)i << (M - m)];
		}
	}

	// x を (y, z) に分割する（逆変換用）
	void butterfly_inv(const vm& x, vm& y, vm& z) {
		int n = sz(x) / 2, m = msb(n) + 1;
		y.resize(n); z.resize(n);

		rep(i, n) {
			y[i] = x[i] + x[i + n];
			z[i] = (x[i] - x[i + n]) * w_pow[N - (i << (M - m))];
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
		// verify : https://judge.yosupo.jp/problem/convolution_mod
		
		Assert(sz(a) <= N);

		int n = sz(a);
		if (n == 1) {
			A = a;
			return;
		}

		vm b, c, B, C;
		butterfly(a, b, c);
		ntt(b, B); ntt(c, C);
		riffle(B, C, A);
	}

	// 長さが 2 冪の列 A に対し mod 998244353 で逆数論変換を行った結果を a に格納する．
	void intt(const vm& A, vm& a) {
		// verify : https://judge.yosupo.jp/problem/convolution_mod
		
		Assert(sz(A) <= N);

		intt_sub(A, a);

		// 定数倍の調整
		mint n_inv = mint(sz(A)).inv();
		rep(i, sz(A)) a[i] *= n_inv;
	}
	void intt_sub(const vm& A, vm& a) {
		int n = sz(A);
		if (n == 1) {
			a = A;
			return;
		}

		vm b, c, B, C;
		butterfly_inv(A, B, C);
		intt_sub(B, b); intt_sub(C, c);
		riffle(b, c, a);
	}

	// a と b の畳込みを返す．
	vm convolution(vm a, vm b) {
		// verify : https://judge.yosupo.jp/problem/convolution_mod

		Assert(sz(a) + sz(b) - 1 <= N);

		int n = sz(a), m = sz(b);

		int k = 1 << (msb(n + m - 2) + 1);
		a.resize(k); b.resize(k);

		vm A, B; ntt(a, A); ntt(b, B);
		rep(i, k) A[i] *= B[i];
		intt(A, a);

		a.resize(n + m - 1);

		return a;
	}

	// 長さが 2 冪の列 a と b の巡回畳込みを返す．
	vm cyclic_convolution(vm a, vm b) {
		Assert(sz(a) == sz(b) && sz(a) <= N);

		int n = sz(a);

		vm A, B; ntt(a, A); ntt(b, B);
		rep(i, n) A[i] *= B[i];
		intt(A, a);

		return a;
	}

	// 長さが 2 冪の列 a を d 個巡回畳込みした結果を返す．
	vm cyclic_convolution_power(vm a, ll d) {
		Assert(sz(a) <= N);

		int n = sz(a);

		vm A; ntt(a, A);
		rep(i, n) A[i] = A[i].pow(d);
		intt(A, a);

		return a;
	}
};


//【高速フーリエ変換】
/*
* FFT<D>(int n) : O(n)
*	長さ n 以下の数列を扱えるよう初期化を行う．
*	制約 : D は double または long double（誤差には注意！）
*
* fft(vD a, vcD& A) : O(n log n)
*	実数列 a[0..n) に対し高速フーリエ変換を行った結果を A[0..n) に格納する（cD = complex<D>）
*	制約 : n は 2 の冪乗
*
* ifft(vcD A, vD& a) : O(n log n)
*	複素数列 A[0..n) に対し逆高速フーリエ変換を行った結果を a[0..n) に格納する．
*	制約 : n は 2 の冪乗
*
* vD convolution(vD a, vD b) : O((n + m) log(n + m))
*	実数列 a[0..n) と b[0..m) の畳込みを返す．
*
* vi convolution(vi a, vi b, int mod = 1e9+7) : O(3 (n + m) log(n + m))
*	整数列 a[0..n) と b[0..m) の mod を法とした畳込みを返す．
*
* vD cyclic_convolution(vD a, vD b) : O(n log n)
*	実数列 a[0..n) と b[0..n) の巡回畳込みを返す．
*	制約 : n は 2 の冪乗
*/
template<class D = double>
struct FFT {
	using vD = vector<D>;
	using cD = complex<D>;
	using vcD = vector<cD>;

	// N : 扱える数列の長さの上限（N = 2^M）
	int N, M;

	// w : 1 の原始 2^M 乗根とする．
	// w_pow[i] : w^i，w_pow[N-i] : w^(-i)
	vcD w_pow;

	// 長さ n 以下の数列を扱えるよう初期化を行う．
	FFT(int n) {
		// verify : https://atcoder.jp/contests/atc001/tasks/fft_c

		// n 以上の最小の 2 冪 2^M を求める．
		M = msb(n - 1) + 1;
		N = 1 << M;

		// w の累乗を前計算しておく．
		w_pow.resize(N + 1); D pi = acos(D(-1));
		repi(i, 0, N) {
			D th = 2 * pi * i / N;
			w_pow[i] = cD(cos(th), sin(th));
		}
	}

	// x を (y, z) に分割する
	void butterfly(const vcD& x, vcD& y, vcD& z) {
		int n = sz(x) / 2, m = msb(n) + 1;
		y.resize(n); z.resize(n);

		rep(i, n) {
			y[i] = x[i] + x[i + n];
			z[i] = (x[i] - x[i + n]) * w_pow[(ll)i << (M - m)];
		}
	}

	// x を (y, z) に分割する（逆変換用）
	void butterfly_inv(const vcD& x, vcD& y, vcD& z) {
		int n = sz(x) / 2, m = msb(n) + 1;
		y.resize(n); z.resize(n);

		rep(i, n) {
			y[i] = x[i] + x[i + n];
			z[i] = (x[i] - x[i + n]) * w_pow[N - (i << (M - m))];
		}
	}

	// (y, z) を x に統合する
	void riffle(const vcD& y, const vcD& z, vcD& x) {
		int n = sz(y);
		x = vcD(2 * n);

		rep(i, n) {
			x[2 * i] = y[i];
			x[2 * i + 1] = z[i];
		}
	}

	// 長さが 2 冪の複素数列 a に対し高速フーリエ変換を行った結果を A に格納する．
	void fft_sub(const vcD& a, vcD& A) {
		int n = sz(a);
		if (n == 1) {
			A = a;
			return;
		}

		vcD b, c, B, C;
		butterfly(a, b, c);
		fft_sub(b, B); fft_sub(c, C);
		riffle(B, C, A);
	}

	// 長さが 2 冪の実数列 a に対し高速フーリエ変換を行った結果を A に格納する．
	void fft(const vD& a_, vcD& A) {
		// verify : https://atcoder.jp/contests/atc001/tasks/fft_c

		int n = sz(a_);
		Assert(n <= N);

		vcD a(n);
		rep(i, n) a[i] = cD(a_[i], 0);

		fft_sub(a, A);
	}

	// 長さが 2 冪の複素数列 a に対し逆高速フーリエ変換を行った結果を A に格納する．
	void intt_sub(const vcD& A, vcD& a) {
		int n = sz(A);
		if (n == 1) {
			a = A;
			return;
		}

		vcD b, c, B, C;
		butterfly_inv(A, B, C);
		intt_sub(B, b); intt_sub(C, c);
		riffle(b, c, a);
	}

	// 長さが 2 冪の実数列 A に対し逆高速フーリエ変換を行った結果を a に格納する．
	void ifft(const vcD& A, vD& a_) {
		// verify : https://atcoder.jp/contests/atc001/tasks/fft_c

		int n = sz(A);
		Assert(n <= N);

		vcD a;
		intt_sub(A, a);

		a_.resize(n);
		rep(i, n) a_[i] = a[i].real() / n; // 定数倍も調整しておく
	}

	// a と b の畳込みを返す．
	vD convolution(vD a, vD b) {
		// verify : https://atcoder.jp/contests/atc001/tasks/fft_c

		int n = sz(a), m = sz(b);
		Assert(n + m - 1 <= N);

		int k = 1 << (msb(n + m - 2) + 1);
		a.resize(k); b.resize(k);

		vcD A, B; fft(a, A); fft(b, B);
		rep(i, k) A[i] *= B[i];
		ifft(A, a);

		a.resize(n + m - 1);

		return a;
	}

	// a と b の畳込みを返す．
	vi convolution(const vi& a, const vi& b, int mod = (int)1e9 + 7) {
		// 参考 : https://maspypy.com/%E6%95%B0%E5%AD%A6%E3%83%BBnumpy-%E9%AB%98%E9%80%9F%E3%83%95%E3%83%BC%E3%83%AA%E3%82%A8%E5%A4%89%E6%8F%9Bfft%E3%81%AB%E3%82%88%E3%82%8B%E7%95%B3%E3%81%BF%E8%BE%BC%E3%81%BF
		// verify : https://judge.yosupo.jp/problem/convolution_mod_1000000007

		int n = sz(a), m = sz(b);
		Assert(n + m - 1 <= N);

		vD a0(n), a1(n), b0(m), b1(m); int pow2 = 1 << 15;
		rep(i, n) {
			a0[i] = a[i] % pow2;
			a1[i] = a[i] / pow2;
		}
		rep(i, m) {
			b0[i] = b[i] % pow2;
			b1[i] = b[i] / pow2;
		}

		vD c00_d = this->convolution(a0, b0);
		vD c11_d = this->convolution(a1, b1);
		rep(i, n) a0[i] += a1[i];
		rep(i, m) b0[i] += b1[i];
		vD c01_d = this->convolution(a0, b0);

		vl c00(n + m - 1), c11(n + m - 1), c01(n + m - 1);
		rep(i, n + m - 1) {
			c00[i] = (ll)(c00_d[i] + 0.5) % mod;
			c11[i] = (ll)(c11_d[i] + 0.5) % mod;
			c01[i] = ((ll)(c01_d[i] + 0.5) - c00[i] - c11[i] + 2 * mod) % mod;
		}

		vi c(n + m - 1);
		rep(i, n + m - 1) {
			c[i] = (int)((c00[i] + c01[i] * pow2 + c11[i] * pow2 * pow2) % mod);
		}

		return c;
	}

	// 長さが 2 冪の列 a と b の巡回畳込みを返す．
	vD cyclic_convolution(vD a, vD b) {
		Assert(sz(a) == sz(b) && sz(a) <= N);

		int n = sz(a);

		vcD A, B; fft(a, A); fft(b, B);
		rep(i, n) A[i] *= B[i];
		ifft(A, a);

		return a;
	}
};


//【数論変換とシフト】
/*
* 長さ 2^m の数列 a の数論変換対が A であるとき，
* b[i] = a[(i - 1) mod 2^m] の数論変換対は B[i] = ζ^i A[i] である．
* ここで ζ は数論変換に用いた 1 の原始 2^m 乗根である．
*/


