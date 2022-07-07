#pragma once
#include "header.h"
// ■■■■■ 統計量 ■■■■■


//【中央値（生データ）】O(n log n)
/*
* a[0..n) の中央値の二倍を返す．
*/
template <class T> T doubled_median(vector<T> a) {
	// verify : https://atcoder.jp/contests/abc169/tasks/abc169_e

	int n = sz(a);
	sort(all(a));
	return a[(n - 1) / 2] + a[n / 2];
}


//【中央値（度数分布）】O(n log n)
/*
* a[0..n) が各 c[0..n) 個あるデータの中央値の二倍を返す．
*/
template <class T> T doubled_median(const vector<T>& a, const vl& c) {
	// verify : https://yukicoder.me/problems/no/1251

	int n = sz(a);

	vector<pair<T, ll>> ac(n);
	rep(i, n) ac[i] = { a[i], c[i] };
	sort(all(ac));

	// acc : 累積度数
	vl acc(n + 1);
	rep(i, n) acc[i + 1] = acc[i] + ac[i].second;
	Assert(acc[n] > 0);

	// 累積度数が総度数の半分になるところを探す．
	repi(i, 1, n) {
		if (2 * acc[i] > acc[n]) {
			return ac[i - 1].first * 2;
		}
		else if (2 * acc[i] == acc[n]) {
			return ac[i - 1].first + ac[i].first;
		}
	}

	return -1;
}


//【四分位数（生データ）】O(n log n)
/*
* a[0..n) の四分位数の二倍を返す．
*/
template <class T> tuple<T, T, T> doubled_quartile(vector<T> a) {
	// verify : https://algo-method.com/tasks/709

	int n = sz(a);
	sort(all(a));
	T dq1 = a[n / 2 / 2] + a[(n / 2 - 1) / 2];
	T dq2 = a[n / 2] + a[(n - 1) / 2];
	T dq3 = a[n - 1 - n / 2 / 2] + a[n - 1 - (n / 2 - 1) / 2];
	return make_tuple(dq1, dq2, dq3);
}


//【平均値の最大化】
/*
* 平均値の最大化では，答えで二分探索を行うことにすれば，
*	(P)：平均値を M 以上にできるか
* という問題を解くことに帰着する．
* 
* (P) は，各値から M を引く前処理を施せば，
*	(Q)：和を 0 以上にできるか
* という問題を解くことに帰着する．
*/


//【中央値の最大化】
/*
* 中央値（偶数個なら小さい方を採用）の最大化では，答えで二分探索を行うことにすれば，
*	(P)：中央値を M 以上にできるか
* という問題を解くことに帰着する．
*
* (P) は，各値を M 以上なら +1，M 未満なら -1 と置き換える前処理を施せば，
*	(Q)：和を 0 より大きくできるか
* という問題を解くことに帰着する．
*/


