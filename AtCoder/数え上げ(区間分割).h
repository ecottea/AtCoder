#pragma once
#include "header.h"
// ■■■■■ 数え上げ（区間分割） ■■■■■


//【区間和をとる操作で得られる列の数え上げ】
/*
* a[0..n) を区間 B_1, ..., B_k に分割し，それぞれの総和をとることで長さ k の列を得る．
* この操作で得られる列が何通りあるかを返す．
*
* 利用：【部分列の数え上げ】
*/
template <class T>
mint count_interval_partition_sum(const vector<T>& a) {
	// verify : https://atcoder.jp/contests/abc230/tasks/abc230_f

	//【方法】
	// 累積和 s[0..n] (s[i] := Σa[0..i)) を導入すれば，
	// この操作は s[1..n-1] から 0 個以上の要素を間引くことと等価である．
	// よって s[1..n-1] の部分列の数え上げに帰着する．

	int n = sz(a);
	if (n <= 1) return 1;

	vl s(n - 1);
	s[0] = a[0];
	repi(i, 1, n - 2) s[i] = s[i - 1] + a[i];

	return count_subsequences(s);
}


