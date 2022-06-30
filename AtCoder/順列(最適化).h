#pragma once
#include "header.h"
// ■■■■■ 順列の最小化[最大化] ■■■■■


//【並べ替え不等式（積の和）】
/*
* 数列 a[0..n) と b[0..n) それぞれを自由に並べ替えてよいとき，
* Σi a[i] b[i] が最大になるのは a, b の大小順が同じときで，
* Σi a[i] b[i] が最小になるのは a, b の大小順が異なるときである．
*
* 参考：https://manabitimes.jp/math/632
*/


//【並べ替え不等式（積の max）】
/*
* 非負数列 a[0..n) と b[0..n) それぞれを自由に並べ替えてよいとき，
* max_i a[i] b[i] が最大になるのは a, b の大小順が同じときで，
* max_i a[i] b[i] が最小になるのは a, b の大小順が異なるときである．
*
* verify : https://atcoder.jp/contests/abc144/tasks/abc144_e
*/


//【隣接項の差の総和の最大化】O(n log n)
/*
* 与えられた数列 a[0..n) について，[0..n) の順列 p[0..n) のうち，
*	Σi∈[0..n) |a[p[i+1]] - a[p[i]]|  （ただし p[n] = p[0]）
* を最大にするものを p に格納する．またそのときの値を返す．
*/
ll maximize_difference_sum(const vl& a, vi* p = nullptr) {
	// verify : https://atcoder.jp/contests/agc025/tasks/agc025_c
	
	//【方法】
	// 数直線上のチェックポイント a[0..n) を一巡する最長経路を求める問題と考える．
	// 
	// a[0..n) を昇順ソートしておき，その間の n-1 個の区間を考える．
	// ある区間を通るのは，その区間の左右間を移動する必要のあるときに限るので，
	// 各区間を通る回数の上界は
	//		2, 4, 6, ..., n, ..., 6, 4, 2 （n が偶数のとき）
	//		2, 4, 6, ..., n-1, n-1, ..., 6, 4, 2 （n が奇数のとき）
	// となる．
	//
	// この上界は左半分と右半分を行き来するような経路で容易に達成可能である．

	int n = sz(a);
	if (p == nullptr) p = new vi;
	p->resize(n);

	vector<pli> ai(n);
	rep(i, n) ai[i] = { a[i], i };
	sort(all(ai));

	rep(i, (n + 1) / 2) (*p)[2 * i] = ai[i].second;
	rep(i, n / 2) (*p)[2 * i + 1] = ai[(n + 1) / 2 + i].second;

	ll res = 0;
	rep(i, n) res += abs(a[(*p)[smod(i + 1, n)]] - a[(*p)[i]]);

	return res;
}


//【累積和の最大値の最小化】O(n log n)
/*
* 与えられた非負整数 a[0..n), b[0..n) について，[0..n) の順列 p[0..n) のうち，
*	a[p[0]], -b[p[0]], a[p[1]], -b[p[1]], a[p[2]], -b[p[2]], ...
* の累積和の最大値 M を最小とするものを p に格納する．またそのときの M を返す．
*/
ll minimize_pair_accumulate(const vl& a, const vl& b, vi* p = nullptr) {
	// verify : https://atcoder.jp/contests/arc053/tasks/arc053_c

	//【方法】
	// a[i] - b[i] の符号により組 (a[i], b[i]) を 2 つに分ける．
	// 全体で見ると，負の組を先，正の組を後に並べるのが最善である．
	//
	// 負の組の中では，最大値を更新してしまいやすい a[i] の大きいものを後ろに回すのが最善．
	// 正の組の中では，順序を逆にして考えると，b[i] の大きいものを前に回すのが最善．

	int n = sz(a);
	if (p != nullptr) p->resize(n);

	vector<tuple<int, ll, int>> svi(n);
	rep(i, n) {
		if (a[i] - b[i] < 0) svi[i] = { -1, a[i], i };
		else svi[i] = { 1, -b[i], i };
	}
	sort(all(svi));

	ll res = 0, v = 0;
	rep(i, n) {
		int id = get<2>(svi[i]);

		if (p != nullptr) (*p)[i] = id;

		v += a[id];
		chmax(res, v);
		v -= b[id];
	}

	return res;
}


