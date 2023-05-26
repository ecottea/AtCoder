#pragma once
#include "header.h"
#include "数え上げ(区間).h"
#include "前処理(文字列).h"
// ■■■■■ スコア和（区間） ■■■■■


//【区間の min の総和】O(n log n)
/*
* Σi<j min( a[i..j) ) の値を返す．
*
* 利用：【区間の数え上げ（最小値指定）】
*/
template <class T>
T interval_min_sum(const vector<T>& a) {
	// verify : https://atcoder.jp/contests/agc005/tasks/agc005_b

	auto cnt = count_min_intervals(a);

	T res = T(0);
	repe(p, cnt) res += p.first * p.second;

	return res;
}


//【区間の OR の総和】O(n log A)
/*
* Σi<j OR a[i..j) の値を返す．
*
* 利用：【ランレングス符号】
*/
ll interval_OR_sum(const vi& a) {
	// verify : https://yukicoder.me/problems/no/2300

	int n = sz(a);

	ll res = 0;

	// 第 b ビットからの寄与を計算する．
	rep(b, 31) {
		// seq[i] : a[i] の第 b ビット
		vi seq(n);
		rep(i, n) seq[i] = get(a[i], b);

		// ビット列をランレングス符号化する．
		auto rle = run_length_encodeing(seq);

		// 全ての区間の個数を加算する．
		ll cnt = (ll)n * (n + 1) / 2;

		for (auto [v, c] : rle) {
			// 0 のみからなる区間の個数を減算する．
			if (v == 0) cnt -= (ll)c * (c + 1) / 2;
		}

		// ビット位置の分だけ平行移動したものが実際の寄与になる．
		res += cnt << b;
	}

	return res;
}


