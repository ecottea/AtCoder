#pragma once
#include "header.h"
#include "数え上げ(区間).h"
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


//【積スコアの和 → 数え上げ（積の和典型）】
/*
* ある区間分割のスコアが，各区間のスコアの積で与えられるとする．
* 各区間のスコアがその区間についての何らかの数え上げ問題の答えとみなせるならば，
* 積スコアの和を求める問題を数え上げの問題に帰着でき，遷移を細かくできることが期待される．
*
* 区間長の d 乗 → 区間に区別のできる d 個の玉を配置する（重複可）
* verify : https://atcoder.jp/contests/agc013/tasks/agc013_e
*/


//【連の長さに応じたスコア】
/*
* 連の長さに応じたスコアが得られる場合，連の長さが 1 増加したときの
* スコアの増加量に読み替えることができれば，連の長さではなく
* 直前に選んだ要素のみを状態にもった DP ができる．
*
* verify : https://atcoder.jp/contests/arc010/tasks/arc010_3
*/


//【区間最大値の積の和】O(n)
/*
* a[0..n) を区間 B_1, ..., B_k に分ける方法 2^(n-1) 通り全てについての
* Πi=[1..k] max(B_i) の値の総和を返す．
* 
*（スタックで高速化した DP）
*/
mint max_product_sum(const vi& a) {
	int n = sz(a);

	//【方法】
	// dp[i] を a[0..i) に対する答えとする．
	// dp[i+1] は，i を右端とする区間の左端 j で場合分けすることにより，漸化式
	//		dp[i+1] = Σj=[0..i] dp[j] max(a[j..i])
	// を用いて求めることができる．
	//
	// j0 < i を a[j0] >= a[i] を満たす最大のもの（なければ -1）と定めると，
	//		max(a[j..i]) = max(a[j..i))	(j <= j0 のとき)
	//		max(a[j..i]) = a[i]			(j > j0 のとき)
	// となる．これを踏まえると，漸化式は
	//		dp[i+1] = dp[i] - Σj=(j0..i] dp[j] max(a[j..i)) + a[i] Σj=(j0..i] dp[j]
	// と書き直せる．
	//
	// この更新を高速に行うため，max(a[j..i]) の値で j の値を分類し，
	// それぞれの j の範囲について dp の和を保持しておくスタックを用いる．

	// dp[i] : a[0..i) に対する答え
	vm dp(n + 1);
	dp[0] = 1;

	// st : max(a[j..i]) のユニークな値とその範囲の dp の和の組を j について昇順に保持する
	stack<pair<int, mint>> st;

	rep(i, n) {
		dp[i + 1] = (i == 0 ? 0 : dp[i]);

		// sum : max(a[j..i]) = a[i] となる j についての dp[j] の和
		mint sum = dp[i];

		// スタックトップの値が a[i] 以上になるまでスタックを掘っていく．
		while (!st.empty() && st.top().first < a[i]) {
			int v; mint s;
			tie(v, s) = st.top(); st.pop();

			dp[i + 1] -= v * s;
			sum += s;
		}
		st.push({ a[i], sum });

		dp[i + 1] += a[i] * sum;
	}

	return dp[n];
}


//【区間レンジの積の和】O(n)
/*
* a[0..n) を区間 B_1, ..., B_k に分ける方法 2^(n-1) 通り全てについての
* Πi=[1..k] (max(B_i) - min(B_i)) の値の総和を返す．
* 
*（スタックで高速化した DP）
*/
mint range_product_sum(const vi& a) {
	// verify : https://atcoder.jp/contests/abc234/tasks/abc234_g

	//【方法】
	//【区間最大値の積の和】と同様．

	int n = sz(a);

	// dp[i] : a[0..i) に対する答え
	vm dp(n + 1);
	dp[0] = 1;

	stack<pair<int, mint>> st_max;
	stack<pair<int, mint>> st_min;

	mint res_max = 0, res_min = 0;

	rep(i, n) {
		mint sum = dp[i];
		while (!st_max.empty() && st_max.top().first < a[i]) {
			int v; mint s;
			tie(v, s) = st_max.top(); st_max.pop();

			res_max -= v * s;
			sum += s;
		}
		st_max.push({ a[i], sum });
		res_max += a[i] * sum;

		sum = dp[i];
		while (!st_min.empty() && st_min.top().first > a[i]) {
			int v; mint s;
			tie(v, s) = st_min.top(); st_min.pop();

			res_min -= v * s;
			sum += s;
		}
		st_min.push({ a[i], sum });
		res_min += a[i] * sum;

		dp[i + 1] = res_max - res_min;
	}

	return dp[n];
}


