#pragma once
#include "header.h"
#include "二項係数.h"
#include "数論(一括).h"
// ■■■■■ 包除原理 ■■■■■


//【指定バウンディングボックスをもつ点配置の数え上げ】O(1)
/*
* バウンディングボックスが [0..h)×[0..w) になるような n 個の格子点の配置の数を返す．
* 最大 (h * w)! まで計算可能な fm を引数に与えること．
*
*（状態系包除原理）
*
* 利用：【階乗と二項係数（mint利用）】
*/
mint count_points_in_BB(int n, int h, int w, Factorial_mint& fm) {
	// verify : https://atcoder.jp/contests/abc003/tasks/abc003_4

	mint res = 0;

	// すべての場合
	res += fm.binomial(h * w, n);

	// 少なくとも 1 個の辺が条件を満たしていない場合
	res -= 2 * fm.binomial((h - 1) * w, n); // 上または下
	res -= 2 * fm.binomial(h * (w - 1), n); // 左または右

	// 少なくとも 2 個の辺が条件を満たしていない場合
	res += fm.binomial((h - 2) * w, n); // 上下
	res += fm.binomial(h * (w - 2), n); // 左右
	res += 4 * fm.binomial((h - 1) * (w - 1), n); //（上または下）かつ（左または右）

	// 少なくとも 3 個の辺が条件を満たしていない場合
	res -= 2 * fm.binomial((h - 2) * (w - 1), n); // 上下かつ（左または右）
	res -= 2 * fm.binomial((h - 1) * (w - 2), n); //（上または下）かつ左右

	// 全ての辺が条件を満たしていない場合
	if (h >= 2 && w >= 2) res += fm.binomial((h - 2) * (w - 2), n); // 上下かつ左右

	return res;
}


//【互いに素な数の個数】O(√a + 2^m)（m : a の素因数の種類数）
/*
* l 以上 r 以下の整数のうち、a と互いに素な数の個数を返す．
* 
*（状態系包除原理）
*
* 利用：【素因数分解】
*/
ll count_coprime(ll a, ll l, ll r) {
	// a と互いに素 ⇔ a の各素因数で割り切れない，なので a を素因数分解する．
	map<ll, int> pps;
	factor_integer(a, pps);

	// a の素因数だけのリスト p を作る．（個数は使わない）
	vl p;
	repe(s, pps) p.push_back(s.first);
	int m = sz(p);

	// 包除原理を用いて数え上げる．
	// 例えば，6 と互いに素な数の個数は，
	//		1 の倍数を全て数え，そこから 2 の倍数の個数を引き，
	//		さらに 3 の倍数の個数を引き，引きすぎた 6 の倍数の個数を足す
	// ことにより数えることができる．
	ll res = 0;
	rep(bit, 1 << m) {
		// mul の倍数を考える．
		ll mul = 1;

		// mul が何個の素因数の積か．
		int ones = 0;

		rep(i, m) {
			if (bit & (1 << i)) {
				mul *= p[i];
				ones++;
			}
		}

		// 素因数の個数の偶奇で加減を切り替えつつ個数を数えていく．
		res += ((ones % 2) ? -1 : 1) * (r / mul - (l - 1) / mul);
	}
	return res;
}


