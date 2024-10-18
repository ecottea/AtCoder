#pragma once
#include "header.h"
// ■■■■■ 数え上げ（区間分割） ■■■■■


//【区間分割の数え上げ（和 S 禁止）】O(n)
/*
* 数列 a[0..n)（負値も可）を区間 B_1, ..., B_K に分割する方法のうち，
*	∀B_k, ΣB_k ≠ S
* を満たすものの個数を返す．
*
*（総和で高速化した貰う DP）
*/
template <class T>
mint count_interval_partition_avoidS(const vector<T>& a, T S) {
	// verify : https://atcoder.jp/contests/abc370/tasks/abc370_e

	//【方法】
	// a[0..n) の 累積和 acc[0..n] (acc[i] := Σa[0..i)) を導入する．
	// dp[i] を a[0..i) の区間分割の方法と定めると，i を右端とする半開区間の左端 j で場合分けして
	//		dp[i] = Σj∈[0..i),acc[j]≠acc[i]-S dp[j]
	// なる遷移式を得る．余事象を考えると
	//		dp[i] = Σdp[0..i) - Σj∈[0..i),acc[j]=acc[i]-S dp[j]
	// となるので，それぞれの項に対応する総和を記憶しておけば良い．

	int n = sz(a);

	vector<T> acc(n + 1);
	rep(i, n) acc[i + 1] = acc[i] + a[i];

	// dp_i : a[0..i) の区間分割の方法
	mint dp = 1, dp_sum = 1;

	unordered_map<T, mint> sub;
	sub[0] = 1;

	repi(i, 1, n) {
		dp = dp_sum;
		if (sub.count(acc[i] - S)) dp -= sub[acc[i] - S];

		sub[acc[i]] += dp;
		dp_sum += dp;
	}

	return dp;
}


//【区間和をとる操作で得られる列の数え上げ】O(n)
/*
* a[0..n)（負値も可）を区間 B_1, ..., B_k に分割し，それぞれの総和をとることで長さ k の列を得る．
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

	return count_unique_subsequences_Lpri_getDP(s);
}


