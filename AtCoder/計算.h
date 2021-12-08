#pragma once
#include "header.h"
#include "探索.h"
// ■■■■■ 計算 ■■■■■


//【累乗根】O(n log a)
/*
* 非負の数 a の n 乗根の切り捨て値を返す．
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


