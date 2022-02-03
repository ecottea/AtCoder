#pragma once
#include "header.h"
#include "探索.h"
// ■■■■■ 計算 ■■■■■


//【累乗根】O(n log a)
/*
* 非負の数 a の n 乗根（a^(1/n)）の切り捨て値を返す．
*
* 利用：【めぐる式二分探索】
*/
ll root(ll a, int n) {
	if (a <= 1 || n == 1) return a;
	
	// x^k を返す．ただし a を超えた場合は a + 1 を返す．
	auto pow = [&](ll x, int k) {
		ll v = 1;
		rep(i, k) {
			if (v > a / x) return a + 1;
			v *= x;
		}
		return v;
	};

	// x^n <= a かを返す．
	function<bool(ll)> okQ = [&](ll x) {
		return pow(x, n) <= a;
	};

	ll res = meguru_search(1LL, a + 1, okQ);

	return res;
}


//【商列挙】O(√n)
/*
* i=[1..n] に対し，n/i の商が q となる i の範囲が [i1..i2) であることを
* {q, {i1, i2}} として q について降順に qr に格納する．
*/
void quotient_range(ll n, vector<pair<ll, pll>>& qr) {
	// verify : https://atcoder.jp/contests/abc230/tasks/abc230_e

	//【方法】
	// n/i の商が q となるような i の範囲を考える．条件を i について整理すると
	//		q = floor(n / i)
	//		⇔ q <= n / i < q + 1
	//		⇔ i q <= n < i(q + 1)
	//		⇔ n / (q + 1) < i <= n / q
	// となる．
	//
	// この幅が 1 以下であれば，q に対応する i は高々 1 個である．その条件は
	//		n / q - n / (q + 1) <= 1
	//		⇔ (q + 1)n - q n <= q(q + 1)
	//		⇔ n <= q(q + 1)
	// である．条件をやや弱めて
	//		n <= q^2
	//		⇔ √n <= q
	// としてもオーダーに影響はない．

	ll m = (ll)(sqrt(n) + EPS);

	// q に対応する i が高々 1 個の部分は i ごとに愚直に考える．
	for (int i = 1; n / i > m; i++) {
		qr.push_back({ n / i, {i, i + 1} });
	}

	// そうでない部分は q ごとにまとめて考える．
	repir(q, m, 1) {
		ll i0 = n / (q + 1) + 1;
		ll i1 = n / q + 1;
		qr.push_back({ q, {i0, i1} });
	}
}


//【フロイドの循環検出法】O(nc + c)
/*
* a[i+1] = f(a[i]), a[0] = a0 なる数列について，a[0] から始まる非周期列の長さ nc と
* a[nc] から始まる周期列の長さ c の組 {nc, c} を返す．
*/
template <class T> pii floyds_cycle_finding(function<T(T)>& f, T a0) {
	// 参考 : https://ja.wikipedia.org/wiki/%E3%83%95%E3%83%AD%E3%82%A4%E3%83%89%E3%81%AE%E5%BE%AA%E7%92%B0%E6%A4%9C%E5%87%BA%E6%B3%95

	T x = a0, y = a0;
	int m = 0;
	do {
		x = f(x);
		y = f(f(y));
		m++;
	} while (x != y);

	x = a0;
	int nc = 0;
	while (x != y) {
		x = f(x);
		y = f(y);
		nc++;
	}

	int c = 0;
	do {
		x = f(x);
		y = f(f(y));
		c++;
	} while (x != y);

	return make_pair(nc, c);
}


//【一次不等式の評価】O(1)
/*
* [不]等式 a * b op c が成り立つかを返す．（a * b がオーバーフローしても良い）
* 比較演算子 op は，">", ">=", "=", "<=", "<" のいずれかとする．
*/
bool ineq(ll a, ll b, string op_, ll c) {
	int op = 0;
	if (op_ == ">") op = -2;
	else if (op_ == ">=") op = -1;
	else if (op_ == "<=") op = 1;
	else if (op_ == "<") op = 2;

	// 等号付き不等式に統一する．
	if (op == -2) { c++; op = -1; }
	if (op == 2) { c--; op = 1; }

	// a = 0 または b = 0 の場合，左辺は 0 になるので容易に判断できる．
	if (a == 0 || b == 0) {
		if (op == -1) return 0 >= c;
		if (op == 0) return 0 == c;
		if (op == 1) return 0 <= c;
	}

	// a > 0 かつ b > 0となるように同値変形する．
	if (a < 0) { a *= -1; c *= -1; op *= -1; }
	if (b < 0) { b *= -1; c *= -1; op *= -1; }

	// (左辺) > 0 なので，0 >= c の場合は符号だけで判断できる．
	// op が ">=" なら不等式は成り立ち，さもなくば成り立たない．
	if (c <= 0) return op == -1;

	// 以降は a, b, c > 0 として考えて良い．

	// op が "=" の場合
	if (op == 0) {
		//	    a * b = c
		//	 ⇔ b = c / a
		//	 ⇔ a | c かつ b = floor(c / a)
		return c % a == 0 && c / a == b;
	}
	// op が "<=" の場合
	else if (op == 1) {
		//	    a * b <= c
		//	 ⇔ b <= c / a
		//	 ⇔ b <= floor(c / a)
		return b <= c / a;
	}
	// op が ">=" の場合
	else {
		//	    a * b >= c
		//	 ⇔ b >= c / a
		//	 ⇔ b >= ceiling(c / a)
		return b >= (c + a - 1) / a;
	}
}

