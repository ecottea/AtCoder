#pragma once
#include "header.h"
// ■■■■■ 数え上げ（列） ■■■■■


//【部分列の数え上げ】O(n + max(a))
/*
* 列 a[0..n) の部分列の個数を返す．空列も a の部分列とみなす．
*
*（総和で高速化した部分列 DP）
*/
template <class T> mint count_subseq_inline(const vector<T>& a) {
	// verify : https://judge.yosupo.jp/problem/number_of_subsequences

	int n = sz(a);
	int m = *max_element(all(a)) + 1;

	// dp_i[j] : a[0..i) の空でない部分列で，最後が j であるものの個数．
	//	ただし同じ部分列については選択する位置の組が辞書順最大になるもののみを認める．
	//	この制約を設けることにより同じ部分列を重複して数えてしまわないようにする．
	vm dp(m);

	// sum_i : a[0..i) の空でない部分列の個数
	mint sum = 0;

	rep(i, n) {
		int j = a[i];
		mint old = dp[j];

		// a[i] = j を選ぶと，最後が j である列が今までの列の個数 + 1 になる．
		// 今までの列で最後が j であるものについて a[i] = j を選ばないことは禁止されている．
		dp[j] = sum + 1;

		// 総和の差分更新
		sum = sum - old + dp[j];
	}

	// 空列の分を加算
	return sum + 1;
}


//【指定部分列の数え上げ】O(n m)
/*
* s[0..n) の部分列のうち t[0..m) に一致するものの個数を返す．
*
*（耳 DP）
*/
template <class T> mint count_subsequences(const vector<T>& s, const vector<T>& t) {
	// verify : https://atcoder.jp/contests/typical90/tasks/typical90_h

	int n = sz(s), m = sz(t);

	// dp[i][j] : s[0..i) が部分列として t[0..j) を何個含むか
	vvm dp(n + 1, vm(m + 1));
	dp[0][0] = 1;

	// 配る DP
	rep(i, n) {
		repi(j, 0, m) {
			// s[i] を採用しない場合
			dp[i + 1][j] += dp[i][j];

			// s[i] を t[j] に一致する文字として採用する場合
			if (j < m && s[i] == t[j]) {
				dp[i + 1][j + 1] += dp[i][j];
			}
		}
	}

	return dp[n][m];
}


//【共通部分列の数え上げ】O(n m)
/*
* s[0..n) の部分列と t[0..m) の部分列の組のうち両者が一致するものの個数を返す．
*/
template <class T> mint count_common_subsequences(const vector<T>& s, const vector<T>& t) {
	// verify : https://atcoder.jp/contests/abc130/tasks/abc130_e

	int n = sz(s), m = sz(t);

	// dp[i][j] : s[0..i) の部分列と t[0..j) の部分列の組のうち両者が一致するものの個数
	vvm dp(n + 1, vm(m + 1));
	dp[0][0] = 1;

	// 配る DP
	repi(i, 0, n) {
		repi(j, 0, m) {
			// s[i] を採用しない場合
			if (i < n) {
				dp[i + 1][j] += dp[i][j];
			}

			// t[j] を採用しない場合
			if (j < m) {
				dp[i][j + 1] += dp[i][j];
			}

			// s[i], t[j] を共に採用しない場合を足しすぎたので引いておく
			if (i < n && j < m) {
				dp[i + 1][j + 1] -= dp[i][j];
			}

			// s[i] と t[j] を共通部分列の文字として採用する場合
			if (i < n && j < m && s[i] == t[j]) {
				dp[i + 1][j + 1] += dp[i][j];
			}
		}
	}

	return dp[n][m];
}


//【貪欲増加部分列長が指定された列の数え上げ】
/*
* 各要素が [0..k) の元である列 a[0..n) で，貪欲増加部分列の長さが k であるものの個数は
* 第二種スターリング数 s(n, k) に等しい．
* 
* 証明：各要素が [0..k) の元である列 a[0..n) で，貪欲増加部分列の長さが k であるものをとる．
* 各 j∈[0..k) について a[i]=j なる i の集合 a^(-1)(j)⊂[0..n) をとれば，
* [0..n) のちょうど k 個の集合への分割と 1:1 に対応する．
* 
* 例えば n=4, k=3 のときの [0,1,2,0] は，集合の分割 {0,3}∪{1}∪{2} と対応する．
*/


