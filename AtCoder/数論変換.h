#pragma once
#include "header.h"
#include "二項係数.h"
// ■■■■■ 数論変換（mod 998244353），添字和畳込み ■■■■■


//【巡回畳込みと多項式の積】
/*
* F_p 上の数列 a[0..2^n), b[0..2^n) の巡回畳込みは，
* 剰余環 F_p[z]/(1 - z^(2^n)) における多項式の積と等価である．
*/


//【畳込み（mod 998244353，長さ 4,194,304 以下）】O((n + m) log(n + m))
/*
* ACL の vm convolution(vm a, vm b) を利用すればよい．
* 
* 制約：n + m - 1 ≦ 8,388,608 = 2^23
*/


//【畳込み（mod 998244353，長さ 8,388,608 以下）】O((n + m) log(n + m))
/*
* a[0..n) と b[0..m) の畳込み（長さは n+m-1）を返す．
*
* 制約：n, m ≦ 8,388,608 = 2^23
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

	int n = sz(a), m = sz(b);
	int L = 1 << max(msb(max(n, m) - 1), 0);

	// a を分割する．
	vm a0(2 * L), a1(2 * L);
	repi(i, 0, min(L, n) - 1)		a0[i] = a[i];
	repi(i, L, min(2 * L, n) - 1)	a1[i - L] = a[i];

	// b を分割する．
	vm b0(2 * L), b1(2 * L);
	repi(i, 0, min(L, m) - 1)		b0[i] = b[i];
	repi(i, L, min(2 * L, m) - 1)	b1[i - L] = b[i];

	// NTT
	internal::butterfly(a0);
	internal::butterfly(a1);
	internal::butterfly(b0);
	internal::butterfly(b1);

	// hi = ai bi
	vm h0(2 * L), h1(2 * L);

	// h01 = (a0 + a1)(b0 + b1)
	vm h01(2 * L);

	rep(i, 2 * L) {
		h0[i] = a0[i] * b0[i];
		h1[i] = a1[i] * b1[i];
		h01[i] = (a0[i] + a1[i]) * (b0[i] + b1[i]);
	}

	// INTT
	internal::butterfly_inv(h0);
	internal::butterfly_inv(h1);
	internal::butterfly_inv(h01);

	// 列 x に列 y を加える．
	auto add = [&](vm& x, const vm& y, int l) { rep(i, sz(y)) x[l + i] += y[i]; };

	// 列 x から列 y を引く．
	auto sub = [&](vm& x, const vm& y, int l) { rep(i, sz(y)) x[l + i] -= y[i]; };

	vm c(4 * L);
	add(c, h0, 0);
	add(c, h01, L); sub(c, h0, L); sub(c, h1, L);
	add(c, h1, 2 * L);

	c.resize(n + m - 1);

	mint inv = mint(2 * L).inv();
	rep(i, n + m - 1) c[i] *= inv;

	return c;
}


//【畳込み（mod 998244353，長さ 16,777,216 以下）】O((n + m) log(n + m))
/*
* a[0..n) と b[0..m) の畳込み（長さは n+m-1）を返す．
*
* 制約：n, m ≦ 16,777,216 = 2^24
*/
vm convolution_verylarge(const vm& a, const vm& b) {
	// verify : https://judge.yosupo.jp/problem/convolution_mod_large

	//【方法】
	// ACL の convolution() が結果の大きさ 2^23 以下までしか対応していないので，
	// 列を長さ 2^22 以下に分割し，カラツバ法っぽく畳込み結果を統合する．

	int n = sz(a), m = sz(b);
	int L = 1 << max(msb(max(n, m) - 1) - 1, 0);

	// a を分割する．
	vm a0(2 * L), a1(2 * L), a2(2 * L), a3(2 * L);
	repi(i, 0, min(L, n) - 1)			a0[i] = a[i];
	repi(i, L, min(2 * L, n) - 1)		a1[i - L] = a[i];
	repi(i, 2 * L, min(3 * L, n) - 1)	a2[i - 2 * L] = a[i];
	repi(i, 3 * L, n - 1)				a3[i - 3 * L] = a[i];

	// b を分割する．
	vm b0(2 * L), b1(2 * L), b2(2 * L), b3(2 * L);
	repi(i, 0, min(L, m) - 1)			b0[i] = b[i];
	repi(i, L, min(2 * L, m) - 1)		b1[i - L] = b[i];
	repi(i, 2 * L, min(3 * L, m) - 1)	b2[i - 2 * L] = b[i];
	repi(i, 3 * L, m - 1)				b3[i - 3 * L] = b[i];

	// NTT
	internal::butterfly(a0);
	internal::butterfly(a1);
	internal::butterfly(a2);
	internal::butterfly(a3);
	internal::butterfly(b0);
	internal::butterfly(b1);
	internal::butterfly(b2);
	internal::butterfly(b3);

	// hi = ai bi
	vm h0(2 * L), h1(2 * L), h2(2 * L), h3(2 * L);

	// hij = (ai + aj)(bi + bj)
	vm h02(2 * L), h13(2 * L), h01(2 * L), h23(2 * L);

	// h0123 = (a0 + a1 + a2 + a3)(b0 + b1 + b2 + b3)
	vm h0123(2 * L);

	rep(i, 2 * L) {
		h0[i] = a0[i] * b0[i];
		h1[i] = a1[i] * b1[i];
		h2[i] = a2[i] * b2[i];
		h3[i] = a3[i] * b3[i];
		h02[i] = (a0[i] + a2[i]) * (b0[i] + b2[i]);
		h13[i] = (a1[i] + a3[i]) * (b1[i] + b3[i]);
		h01[i] = (a0[i] + a1[i]) * (b0[i] + b1[i]);
		h23[i] = (a2[i] + a3[i]) * (b2[i] + b3[i]);
		h0123[i] = (a0[i] + a1[i] + a2[i] + a3[i]) * (b0[i] + b1[i] + b2[i] + b3[i]);
	}

	// INTT
	internal::butterfly_inv(h0);
	internal::butterfly_inv(h1);
	internal::butterfly_inv(h2);
	internal::butterfly_inv(h3);
	internal::butterfly_inv(h02);
	internal::butterfly_inv(h13);
	internal::butterfly_inv(h01);
	internal::butterfly_inv(h23);
	internal::butterfly_inv(h0123);

	// 列 x に列 y を加える．
	auto add = [&](vm& x, const vm& y, int l) { rep(i, sz(y)) x[l + i] += y[i]; };

	// 列 x から列 y を引く．
	auto sub = [&](vm& x, const vm& y, int l) { rep(i, sz(y)) x[l + i] -= y[i]; };

	vm c01(4 * L);
	add(c01, h0, 0);
	add(c01, h01, L); sub(c01, h0, L); sub(c01, h1, L);
	add(c01, h1, 2 * L);

	vm c23(4 * L);
	add(c23, h2, 0);
	add(c23, h23, L); sub(c23, h2, L); sub(c23, h3, L);
	add(c23, h3, 2 * L);

	vm c(8 * L);
	add(c, c01, 0);
	add(c, h02, 2 * L);
	add(c, h0123, 3 * L); sub(c, h02, 3 * L); sub(c, h13, 3 * L);
	add(c, h13, 4 * L);
	sub(c, c01, 2 * L); sub(c, c23, 2 * L);
	add(c, c23, 4 * L);

	c.resize(n + m - 1);

	mint inv = mint(2 * L).inv();
	rep(i, n + m - 1) c[i] *= inv;

	return c;
}


//【畳込み（複数，mod 998244353）】O(n (log n)^2)
/*
* 数列の集合 a の要素を全て畳込んだ結果（長さは n）を返す．
*/
vm multi_convoluion(vvm a) {
	// verify : https://atcoder.jp/contests/abl/tasks/abl_f

	int m = sz(a);
	if (m == 0) return vm{ 1 };

	// (要素数, 数列の番号) の組を要素数昇順に記録する．
	priority_queue_rev<pii> q;
	rep(i, m) {
		if (a[i].empty()) return vm();
		q.push({ sz(a[i]), i });
	}

	// 積のコストが小さい順に掛けていく（マージテク）
	while (sz(q) >= 2) {
		auto [ni, i] = q.top(); q.pop();
		auto [nj, j] = q.top(); q.pop();

		a[i] = convolution(a[i], a[j]);
		q.push({ ni + nj - 1, i });
	}

	return a[q.top().second];
}


//【自己畳込み（mod 998244353）】O(n log n log k)
/*
* a[0..n) を k 個畳み込んで切り詰めた数列 c[0..n) を返す．
*/
vm self_convolution(const vm& a, ll k) {
	int n = sz(a);
	if (n == 0) return vm();

	vm res(n);
	res[0] = 1;

	// 繰り返し二乗法
	vm pow2(a);
	while (k > 0) {
		if (k & 1) {
			res = convolution(res, pow2);
			res.resize(n);
		}

		pow2 = convolution(pow2, pow2);
		pow2.resize(n);

		k /= 2;
	}

	return res;
}


//【オンライン畳込み（mod 998244353）】
/*
* Online_convolution(int n) : O(n)
*	a[0..n) と b[0..n) の畳込み c[0..n) を計算できるよう初期化する．
*
* void set(mint a, mint b) : ならし O((log n)^2)
*	t 回目に呼び出すときは，a=a[t], b=b[t] を与える．
*
* mint [](int i) : O(1)
*	c[i] = Σj∈[0..i] a[j] b[i-j] を返す．
*	制約 : a[0..i], b[0..i] を指定済でなくてはならない．
*
* mint back() : O(1)
*	直前に決定された c[i] を返す．
*
* void update(int i, mint c) : O(1)
*	c[i] を強制的に c に書き換える．
*/
class Online_convolution {
	// 参考 : https://qiita.com/Kiri8128/items/1738d5403764a0e26b4c

	int n, t; // t : 次が何回目の呼び出しか
	vm as, bs, cs;

public:
	// 長さ n の数列同士の畳込みを行えるよう初期化する．
	Online_convolution(int n) : n(n), t(0), as(n), bs(n), cs(n) {
		// verify : https://atcoder.jp/contests/abc280/tasks/abc280_e
	}
	Online_convolution() : n(0), t(0) {}

	// t 回目に呼び出すときは，a=a[t], b=b[t] を与える．
	void set(mint a, mint b) {
		// verify : https://atcoder.jp/contests/abc280/tasks/abc280_e

		as[t] = a; bs[t] = b;

		int i1_max = lsb(t + 2), i2_max = i1_max;

		// 対角線上の正方形領域に対する処理を行う場合
		if (popcount(t + 2) == 1) { i1_max -= 2; i2_max -= 1; }

		// 2^i : 正方形の一辺の長さ（対角線より下）
		repi(i, 0, i1_max) {
			// cs_sub[0..j_max] まで計算する必要がある．
			int j_max = min((1 << (i + 1)) - 2, n - 1 - t);

			// len : 真に計算するべき正方形の一辺の長さ
			int len = min(1 << i, j_max + 1);

			// as[x_min..x_min+len) と bs[y_min..y_min+len) を畳み込む．
			int x_min = t + 1 - (1 << i);
			int y_min = (1 << i) - 1;

			vm as_sub, bs_sub;
			copy(as.begin() + x_min, as.begin() + (x_min + len), back_inserter(as_sub));
			copy(bs.begin() + y_min, bs.begin() + (y_min + len), back_inserter(bs_sub));

			vm cs_sub = convolution(as_sub, bs_sub);
			repi(j, 0, j_max) cs[t + j] += cs_sub[j];
		}

		// 2^i : 正方形の一辺の長さ（対角線以上）
		repi(i, 0, i2_max) {
			// cs_sub[0..j_max] まで計算する必要がある．
			int j_max = min((1 << (i + 1)) - 2, n - 1 - t);

			// len : 真に計算するべき正方形の一辺の長さ
			int len = min(1 << i, j_max + 1);

			// as[x_min..x_min+len) と bs[y_min..y_min+len) を畳み込む．
			int x_min = (1 << i) - 1;
			int y_min = t + 1 - (1 << i);

			vm as_sub, bs_sub;
			copy(as.begin() + x_min, as.begin() + (x_min + len), back_inserter(as_sub));
			copy(bs.begin() + y_min, bs.begin() + (y_min + len), back_inserter(bs_sub));

			vm cs_sub = convolution(as_sub, bs_sub);
			repi(j, 0, j_max) cs[t + j] += cs_sub[j];
		}

		t++;
	}

	// c[i] を返す．
	mint const& operator[](int i) const {
		// verify : https://atcoder.jp/contests/abc280/tasks/abc280_e

		Assert(i < t);

		return cs[i];
	}

	// 直前に決定された c[i] を返す．
	mint back() const {
		// verify : https://judge.yosupo.jp/problem/log_of_formal_power_series

		return cs[t - 1];
	}

	// c[i] を強制的に c に変更する．
	void update(int i, mint c) {
		cs[i] = c;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Online_convolution& c) {
		os << "a: " << c.as << endl;
		os << "b: " << c.bs << endl;
		os << "c: " << c.cs;
		return os;
	}
#endif
};


//【オンライン畳込み（片側固定，mod 998244353）】
/*
* Semi_online_convolution(vm b) : O(n)
*	a[0..n) と固定された b[0..n) の畳込み c[0..n) を計算できるよう初期化する．
*
* void set(mint a) : ならし O((log n)^2)
*	t 回目に呼び出すときは，a=a[t] を与える．
*
* mint [](int i) : O(1)
*	c[i] = Σj∈[0..i] a[j] b[i-j] を返す．
*	制約 : a[0..i] を指定済でなくてはならない．
*
* mint back() : O(1)
*	直前に決定された c[i] を返す．
*
* void update(int i, mint c) : O(1)
*	c[i] を強制的に c に書き換える．
*/
class Semi_online_convolution {
	// 参考 : https://qiita.com/Kiri8128/items/1738d5403764a0e26b4c

	int n, t; // t : 次が何回目の呼び出しか
	vm as, cs; vvm bss;

public:
	// 長さ n の数列同士の畳込みを行えるよう初期化する．
	Semi_online_convolution(const vm& bs) : n(sz(bs)), t(0), as(n), cs(n), bss(msb(n) + 1) {
		// verify : https://atcoder.jp/contests/abc280/tasks/abc280_e

		// b[0], b[1] だけは例外的に bss[0] に格納しておく．
		int len = min(2, n);
		copy(bs.begin(), bs.begin() + len, back_inserter(bss[0]));

		// b[2..n) を幅 2^i の区間にあらかじめ分割しておく．
		repi(i, 1, msb(n)) {
			int y_min = 1 << i;
			int len = min(1 << i, n - y_min);
			copy(bs.begin() + y_min, bs.begin() + (y_min + len), back_inserter(bss[i]));
		}
	}
	Semi_online_convolution() : n(0), t(0) {}

	// t 回目に呼び出すときは，a=a[t] を与える．
	void set(mint a) {
		// verify : https://atcoder.jp/contests/abc280/tasks/abc280_e

		as[t] = a;

		// b[0], b[1] との積だけは例外処理
		cs[t] += as[t] * bss[0][0];
		if (t + 1 < n) cs[t + 1] += as[t] * bss[0][1];

		int i_max = lsb(t);

		// 2^i : 正方形の一辺の長さ
		repi(i, 1, i_max) {
			// cs_sub[0..j_max] まで計算する必要がある．
			int j_max = min((1 << (i + 1)) - 2, n - 1 - t);

			// len : 真に計算するべき正方形の一辺の長さ
			int len = min(1 << i, j_max + 1);

			// as[x_min..x_min+len) と bss[i] を畳み込む．
			int x_min = t - (1 << i);

			vm as_sub;
			copy(as.begin() + x_min, as.begin() + (x_min + len), back_inserter(as_sub));

			vm cs_sub = convolution(as_sub, bss[i]);
			repi(j, 0, j_max) cs[t + j] += cs_sub[j];
		}

		t++;
	}

	// c[i] を返す．
	mint const& operator[](int i) const {
		// verify : https://atcoder.jp/contests/abc280/tasks/abc280_e

		Assert(i < t);

		return cs[i];
	}

	// 直前に決定された c[i] を返す．
	mint back() const {
		// verify : https://atcoder.jp/contests/abc318/tasks/abc318_h

		return cs[t - 1];
	}

	// c[i] を強制的に c に変更する．
	void update(int i, mint c) {
		cs[i] = c;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Semi_online_convolution& c) {
		os << "a: " << c.as << endl;
		os << "c: " << c.cs;
		return os;
	}
#endif
};


//【オンライン指数関数（mod 998244353）】
/*
* Online_exp(int n, Factorial_mint* fm) : O(n)
*	exp(f(z)) を [z^n] まで計算できるよう初期化する．
*	制約 : fm は n! まで計算可能
*
* void set(mint a) : ならし O((log n)^2)
*	t 回目に呼び出すときは，a = [z^t]f(z) を与える．
*	制約 : 0 回目に呼び出すときは a = 0
*
* mint [](int i) : O(1)
*	[z^i] exp(f(z)) を返す．
*	制約 : [z^[0..i]] f(z) を指定済でなくてはならない．
*
* mint back() : O(1)
*	直前に決定された exp(f(z)) の係数を返す．
*
* 利用：【オンライン畳込み（mod 998244353）】
*/
class Online_exp {
	int t; // t : 次が何回目の呼び出しか
	vm as, bs;

	Online_convolution OC;
	Factorial_mint* fm;

public:
	// exp(f(z)) を [z^n] まで計算できるよう初期化する．
	Online_exp(int n, Factorial_mint* fm) : t(0), as(n + 1), bs(n + 1), OC(n), fm(fm) {
		// verify : https://judge.yosupo.jp/problem/exp_of_formal_power_series

		bs[0] = 1;
	}
	Online_exp() : t(0) {}

	// t 回目に呼び出すときは，a = [z^t]f(z) を与える．
	void set(mint a) {
		// verify : https://judge.yosupo.jp/problem/exp_of_formal_power_series

		if (t == 0) {
			Assert(a == 0);
			t++;
			return;
		}

		OC.set(t * a, bs[t - 1]);
		as[t] = a;
		bs[t] = OC.back() * fm->inv(t);
		t++;
	}

	// [z^i] exp(f(z)) を返す．
	mint const& operator[](int i) const {
		Assert(i < t);

		return bs[i];
	}

	// 直前に決定された exp(f(z)) の係数を返す．
	mint back() const {
		// verify : https://judge.yosupo.jp/problem/exp_of_formal_power_series

		return bs[t - 1];
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Online_exp& OE) {
		os << "a: " << OE.as << endl;
		os << "b: " << OE.bs;
		return os;
	}
#endif
};


//【オンライン対数関数（mod 998244353）】
/*
* Online_exp(int n, Factorial_mint* fm) : O(n)
*	log(f(z)) を [z^n] まで計算できるよう初期化する．
*	制約 : fm は n! まで計算可能
*
* void set(mint a) : ならし O((log n)^2)
*	t 回目に呼び出すときは，a = [z^t]f(z) を与える．
*	制約 : 0 回目に呼び出すときは a = 1
*
* mint [](int i) : O(1)
*	[z^i] log(f(z)) を返す．
*	制約 : [z^[0..i]] f(z) を指定済でなくてはならない．
*
* mint back() : O(1)
*	直前に決定された log(f(z)) の係数を返す．
*
* 利用：【オンライン畳込み（mod 998244353）】
*/
class Online_log {
	int t; // t : 次が何回目の呼び出しか
	vm as, bs;

	Online_convolution OC;
	Factorial_mint* fm;

public:
	// log(f(z)) を [z^n] まで計算できるよう初期化する．
	Online_log(int n, Factorial_mint* fm) : t(0), as(n + 1), bs(n + 1), OC(n), fm(fm) {
		// verify : https://judge.yosupo.jp/problem/log_of_formal_power_series

		as[0] = 1;
	}
	Online_log() : t(0) {}

	// t 回目に呼び出すときは，a = [z^t]f(z) を与える．
	void set(mint a) {
		// verify : https://judge.yosupo.jp/problem/log_of_formal_power_series

		if (t == 0) {
			Assert(a == 1);
			t++;
			return;
		}

		if (t == 1) {
			as[1] = bs[1] = a;
			t++;
			return;
		}

		as[t] = a;
		OC.set((t - 1) * bs[t - 1], as[t - 1]);
		bs[t] = a - OC.back() * fm->inv(t);
		t++;
	}

	// [z^i] log(f(z)) を返す．
	mint const& operator[](int i) const {
		Assert(i < t);

		return bs[i];
	}

	// 直前に決定された log(f(z)) の係数を返す．
	mint back() const {
		// verify : https://judge.yosupo.jp/problem/log_of_formal_power_series

		return bs[t - 1];
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Online_log& O) {
		os << "a: " << O.as << endl;
		os << "b: " << O.bs;
		return os;
	}
#endif
};


//【上三角畳込み（mod 998244353）】O(n (log n)^2)
/*
* a[0..n) と b[0..n) の上三角畳込み c を
*	c[k] = Σ_(i+j=k,i<j) a[i] b[j]（strict = false なら i≦j）
* で定義し，c[0..2n-1) を返す．
*
*（分割統治法）
*/
vm triangle_convolution(const vm& a, const vm& b, bool strict = true) {
	//【方法】
	// 上三角部分を矩形に分割し，各矩形について通常の畳込みを用いれば良い．

	//【注意】
	// 上三角畳込みは結合法則を満たさない．

	int n = sz(a);

	vm c(2 * n - 1);

	// l ≦ i < j < r からの寄与を計算する．
	function<void(int, int)> rf = [&](int l, int r) {
		if (r - l == 1) {
			if (!strict) c[2 * l] += a[l] * b[l];
			return;
		}

		int m = (l + r) / 2;

		vm a2(a.begin() + l, a.begin() + m);
		vm b2(b.begin() + m, b.begin() + r);
		vm c2 = convolution(a2, b2);
		rep(i, sz(c2)) c[l + m + i] += c2[i];

		rf(l, m);
		rf(m, r);
	};
	rf(0, n);

	return c;
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

	// 行方向の NTT
	rep(i, H) { internal::butterfly(a[i]); internal::butterfly(b[i]); }

	// 転置
	vvm aT(W, vm(H)), bT(W, vm(H));
	rep(i, H) rep(j, W) { aT[j][i] = a[i][j]; bT[j][i] = b[i][j]; }

	// 列方向の NTT
	rep(j, W) { internal::butterfly(aT[j]); internal::butterfly(bT[j]); }

	// 各点積
	rep(j, W) rep(i, H) aT[j][i] *= bT[j][i];

	// 列方向の INTT
	rep(j, W) internal::butterfly_inv(aT[j]);

	// 転置
	rep(i, H) rep(j, W) a[i][j] = aT[j][i];

	// 行方向の INTT
	rep(i, H) internal::butterfly_inv(a[i]);

	// 不要な部分の削除
	a.resize(ha + hb - 1);
	rep(i, ha + hb - 1) a[i].resize(wa + wb - 1);

	// 定数倍の調整
	mint inv = mint(H * W).inv();
	rep(i, ha + hb - 1) rep(j, wa + wb - 1) a[i][j] *= inv;

	return a;
}


//【フィルタリング（mod 998244353）】O(n log n)
/*
* a[0..n) にフィルタ b[0..m) をかけた結果 c[0..n-m+1) を返す．c[i] は以下の式で表される：
*		c[i] = Σj∈[0..m) a[i+j] b[j]
*/
vm filtering(const vm& a, vm b) {
	// verify : https://yukicoder.me/problems/no/2330

	//【方法】
	// b[0..m) を左右反転すると
	//		c[i] = Σj∈[0..m) a[i+j] b[m-1-j]
	// となる．さらに j ← m-1-j とすると
	//		c[i] = Σj∈[0..m) a[i+m-1-j] b[j]
	// となる．よって畳込みを用いて
	//		c[i] = (a＊b)[i+m-1]
	// と計算すれば良い．

	int n = sz(a), m = sz(b);
	if (n < m) return vm();

	reverse(all(b));
	vm conv = convolution(a, b);

	vm c(n - m + 1);
	rep(i, n - m + 1) c[i] = conv[i + m - 1];

	return c;
}


//【畳込み（法が任意）】O((n + m) log(n + m))
/*
* a と b の MOD を法とした畳込みを返す．
*
* 制約：n + m - 1 ≦ 8,388,608 = 2^23，コンパイラが gcc
*/
vi convolution_arbitrary_mod(const vi& a, const vi& b, int MOD = (int)1e9 + 7) {
	// verify : https://judge.yosupo.jp/problem/convolution_mod_1000000007

	int n = sz(a), m = sz(b);
	if (n == 0 || m == 0) return vi();

	constexpr int MOD1 = 998244353;
	constexpr int MOD2 = 897581057;
	constexpr int MOD3 = 880803841;

	constexpr __int128 MOD23 = 790592842614439937;
	constexpr __int128 MOD13 = 879257460378959873;
	constexpr __int128 MOD12 = 896005221510021121;

	constexpr __int128 MOD123 = __int128(MOD1) * MOD2 * MOD3; // 789204840662082423367925761

	constexpr int MOD23_inv = 41593599;
	constexpr int MOD13_inv = 635786105;
	constexpr int MOD12_inv = 220201354;

	using mint1 = static_modint<MOD1>;
	using mint2 = static_modint<MOD2>;
	using mint3 = static_modint<MOD3>;

	vector<mint1> a1(n), b1(m);
	vector<mint2> a2(n), b2(m);
	vector<mint3> a3(n), b3(m);

	rep(i, n) {
		a1[i] = a[i];
		a2[i] = a[i];
		a3[i] = a[i];
	}
	rep(j, m) {
		b1[j] = b[j];
		b2[j] = b[j];
		b3[j] = b[j];
	}

	auto c1 = convolution(a1, b1);
	auto c2 = convolution(a2, b2);
	auto c3 = convolution(a3, b3);

	vi res(n + m - 1);

	rep(k, n + m - 1) {
		__int128 val1 = c1[k].val() * MOD23 * MOD23_inv;
		__int128 val2 = c2[k].val() * MOD13 * MOD13_inv;
		__int128 val3 = c3[k].val() * MOD12 * MOD12_inv;

		res[k] = (int)((val1 + val2 + val3) % MOD123 % MOD);
	}

	return res;
}


//【畳込み（法が任意，mint）】O((n + m) log(n + m))
/*
* a と b の MOD を法とした畳込みを返す．
*
* 制約：n + m - 1 ≦ 8,388,608 = 2^23，コンパイラが gcc
*/
vm convolution_arbitrary_mod(const vm& a, const vm& b) {
	// verify : https://judge.yosupo.jp/problem/convolution_mod_1000000007

	int n = sz(a), m = sz(b);
	if (n == 0 || m == 0) return vm();

	const int MOD = mint::mod();

	constexpr int MOD1 = 998244353;
	constexpr int MOD2 = 897581057;
	constexpr int MOD3 = 880803841;

	constexpr __int128 MOD23 = 790592842614439937;
	constexpr __int128 MOD13 = 879257460378959873;
	constexpr __int128 MOD12 = 896005221510021121;

	constexpr __int128 MOD123 = __int128(MOD1) * MOD2 * MOD3; // 789204840662082423367925761

	constexpr int MOD23_inv = 41593599;
	constexpr int MOD13_inv = 635786105;
	constexpr int MOD12_inv = 220201354;

	using mint1 = static_modint<MOD1>;
	using mint2 = static_modint<MOD2>;
	using mint3 = static_modint<MOD3>;

	vector<mint1> a1(n), b1(m);
	vector<mint2> a2(n), b2(m);
	vector<mint3> a3(n), b3(m);

	rep(i, n) {
		a1[i] = a[i].val();
		a2[i] = a[i].val();
		a3[i] = a[i].val();
	}
	rep(j, m) {
		b1[j] = b[j].val();
		b2[j] = b[j].val();
		b3[j] = b[j].val();
	}

	auto c1 = convolution(a1, b1);
	auto c2 = convolution(a2, b2);
	auto c3 = convolution(a3, b3);

	vm res(n + m - 1);

	rep(k, n + m - 1) {
		__int128 val1 = c1[k].val() * MOD23 * MOD23_inv;
		__int128 val2 = c2[k].val() * MOD13 * MOD13_inv;
		__int128 val3 = c3[k].val() * MOD12 * MOD12_inv;

		res[k] = (val1 + val2 + val3) % MOD123;
	}

	return res;
}


//【畳込み（法が任意）】O((n + m) log(n + m))（遅い）
/*
* a と b の mod を法とした畳込みを返す．
*/
vi convolution_arbitrary_mod_sep(const vi& a, const vi& b, int mod = (int)1e9 + 7) {
	// verify : https://judge.yosupo.jp/problem/convolution_mod_1000000007

	int n = sz(a), m = sz(b);
	if (n == 0 || m == 0) return vi();

	vl a0(n), a1(n), b0(m), b1(m); const int pow2 = 1 << 15;
	rep(i, n) {
		int ai = smod(a[i], mod);
		a0[i] = ai % pow2;
		a1[i] = ai / pow2;
	}
	rep(i, m) {
		int bi = smod(b[i], mod);
		b0[i] = bi % pow2;
		b1[i] = bi / pow2;
	}

	vl c00 = convolution_ll(a0, b0);
	vl c11 = convolution_ll(a1, b1);
	rep(i, n) a0[i] += a1[i];
	rep(i, m) b0[i] += b1[i];
	vl c01 = convolution_ll(a0, b0);
	rep(i, n + m - 1) {
		c00[i] %= mod;
		c11[i] %= mod;
		c01[i] = (c01[i] - c00[i] - c11[i] + 2LL * mod) % mod;
	}

	vi c(n + m - 1);
	rep(i, n + m - 1) {
		c[i] = (int)((c00[i] + c01[i] * pow2 + c11[i] * pow2 * pow2) % mod);
	}

	return c;
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


