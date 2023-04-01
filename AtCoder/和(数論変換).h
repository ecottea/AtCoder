#pragma once
#include "header.h"
// ■■■■■ 数論変換（mod 998244353），添字和畳込み ■■■■■


//【巡回畳込みと多項式の積】
/*
* F_p 上の数列 a[0..2^n), b[0..2^n) の巡回畳込みは，
* 剰余環 F_p[z]/(z^(2^n)-1) における多項式の積と等価である．
*/


//【畳込み（mod 998244353，長さ 4,194,304 以下）】O((n + m) log(n + m))
/*
* ACL の vm convolution(vm a, vm b) を利用すればよい．
* 
* 制約：n + m - 1 <= 8388608 = 2^23
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
	//
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

	// カラツバ法っぽく計算する．
	vm c11 = convolution(a1, b1);
	vm c22 = convolution(a2, b2);

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

	// 積のコストが小さい順に掛けていく（マージテク）
	while (sz(q) >= 2) {
		auto [ni, i] = q.top(); q.pop();
		auto [nj, j] = q.top(); q.pop();

		a[i] = convolution(a[i], a[j]);
		q.push({ ni + nj - 1, i });
	}

	return a[q.top().second];
}


//【二次元畳込み（mod 998244353）】O((ha + hb) (wa + wb) (log(ha + hb) + log(wa + wb)))
/*
* a[0..ha)[0..wa) と b[0..hb)[0..wb) の二次元畳込みを返す．
*/
vvm convolution_2D(vvm a, vvm b) {
	int ha = sz(a), wa = sz(a[0]);
	int hb = sz(b), wb = sz(b[0]);

	// 高さと幅を 2 冪に拡張しておく．
	int H = 1 << (msb(ha + hb - 2) + 1);
	int W = 1 << (msb(wa + wb - 2) + 1);
	a.resize(H); b.resize(H);
	rep(i, H) { a[i].resize(W); b[i].resize(W); }

	// 行方向の ntt
	rep(i, H) { internal::butterfly(a[i]); internal::butterfly(b[i]); }

	// 転置
	vvm aT(W, vm(H)), bT(W, vm(H));
	rep(i, H) rep(j, W) { aT[j][i] = a[i][j]; bT[j][i] = b[i][j]; }

	// 列方向の ntt
	rep(j, W) { internal::butterfly(aT[j]); internal::butterfly(bT[j]); }

	// 各点積
	rep(j, W) rep(i, H) aT[j][i] *= bT[j][i];

	// 列方向の intt
	rep(j, W) internal::butterfly_inv(aT[j]);

	// 転置
	rep(i, H) rep(j, W) a[i][j] = aT[j][i];

	// 行方向の intt
	rep(i, H) internal::butterfly_inv(a[i]);

	// 不要な部分の削除
	a.resize(ha + hb - 1);
	rep(i, ha + hb - 1) a[i].resize(wa + wb - 1);

	// 定数倍の調整
	mint inv = mint(H * W).inv();
	rep(i, ha + hb - 1) rep(j, wa + wb - 1) a[i][j] *= inv;

	return a;
}


//【数論変換とシフト】
/*
* 数列 a[0..2^m) の数論変換対が A[0..2^m) であるとき，
* b[i] = a[(i - 1) mod 2^m] の数論変換対は B[i] = ω^i A[i] である．
* ここで ω は数論変換に用いた 1 の原始 2^m 乗根である．
*/


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


