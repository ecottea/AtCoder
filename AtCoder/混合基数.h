#pragma once
#include "header.h"
// ■■■■■ 混合基数 ■■■■■


//【混合基数表示】
/*
* 最下位を 0 桁目とし，[0..n) 桁目の重みが a[0..n) で与えられる混合基数について，
* 値 val を混合基数表示したときの i 桁目の数字を d[i] に格納する．
* また b[0..n) は i 桁目の数字の取りうる範囲が [0..b[i]) であることを表す．
*
* 制約：a[0] = 1，a[i] は a[i+1] の真の約数
*/
void mixed_radix(const vl& a, ll val, vl& d, vl& b) {
	// verify : https://atcoder.jp/contests/abc231/tasks/abc231_e

	int n = sz(a);

	b.resize(n);
	rep(i, n - 1) b[i] = a[i + 1] / a[i];
	b[n - 1] = INFL;

	d.resize(n);
	rep(i, n) {
		d[i] = val % b[i];
		val /= b[i];
	}
}


//【混合基数，下から桁 DP，桁上げフラグ，スコア最小化】O(n)
/*
* 混合基数 b[0..n) で表現 num[0..n) をもつ数について，全ての整数 d についての
* d の数字和と num + d の数字和の合計の最小値を返す．
*/
ll minimize_pair_digit_sum(const vl& num, const vl& b) {
	// verify : https://atcoder.jp/contests/abc231/tasks/abc231_e

	int n = sz(num);

	// dp[i][f] : 以下の条件を満たす数の最小スコア：
	//	i : 下からの桁 d[0..i) まで決まっている．
	//	f : d[i+1] への桁上げがあるなら 1，さもなくば 0（桁上げフラグ）
	vvl dp(n + 1, vl(2, INFL));
	dp[0][0] = 0;

	// 下の桁から順に配る DP
	rep(i, n) {
		ll x = num[i];

		rep(f, 2) {
			// d の i 桁目を 0 にする場合
			chmin(dp[i + 1][0], dp[i][f] + (x + f));

			// num + d の i 桁目を 0 にする場合
			chmin(dp[i + 1][1], dp[i][f] + (b[i] - (x + f)));

			// この 2 つの場合以外はスコアを最小にすることはないので無視できる．
			// おつり問題でいうと，渡した硬貨がそのまま返ってくることに対応する．
		}
	}

	return dp[n][0];
}


