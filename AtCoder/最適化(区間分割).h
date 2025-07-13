#pragma once
#include "header.h"
// ■■■■■ 最適化（区間分割） ■■■■■


//【区間◯◯の和の最小化】
/*
* 完全 DAG 上の最短路問題とみなすことができる．
*/


//【区間和の最小値の最大化（非負，区間数指定）】
/*
* 判定問題
*	和が x 以上の K 個の区間に分割できるか？
* は貪欲に解けるので，答えで二分探索すれば良い．
* 
* verify : https://atcoder.jp/contests/typical90/tasks/typical90_a
*/


//【和が x 以上の区間数の最大化（負値可）】O(n log n)
/*
* 数列 a[0..n)（負値も可）に対して
*	a[l..r) のスコア：Σa[l..r) ≧ x ? 1 : -INF
*	a[0..n) の区間分割のスコア：各区間スコアの和
* と定め，2^(n-1) 通り全ての区間分割をわたるスコアの最大値を返す．
* 
* 利用：【max 可換モノイド】
*/
template <class T>
int maximize_greater_count(const vector<T>& a, T x) {
	// verify : https://algo-method.com/tasks/1119

	//【方法】
	// 素朴な貰う DP を累積和 acc[i] := Σa[0..i) を用いて高速化すると，
	//		repi(r, 1, n) rep(l, r) if (acc[r] - acc[l] ≧ x) chmax(dp[r], dp[l] + 1);
	// なる遷移式を得る．これは
	//		repi(r, 1, n) dp[r] = max(dp[l] | acc[l] ≦ acc[r] - x) + 1
	// と書き直せるので，acc 昇順に DP テーブルを並べておけば，max-セグ木で高速化できる．

	int n = sz(a);

	// acc[i] : Σa[0..i)
	vector<T> acc(n + 1);
	rep(i, n) acc[i + 1] = acc[i] + a[i];

	vector<pair<T, int>> acc_i(n + 1);
	repi(i, 0, n) acc_i[i] = { acc[i], i };
	sort(all(acc_i));

	vi p(n + 1);
	repi(i, 0, n) tie(acc[i], p[i]) = acc_i[i];
	
	// acc[q[i]] : Σa[0..i)
	vi q(n + 1);
	repi(i, 0, n) q[p[i]] = i;

	// dp[q[i]] : a[0..i) まででの和が x 以上の区間への分割の最大個数
	segtree<Max_monoid> dp(n + 1);
	dp.set(q[0], 0);

	repi(r, 1, n) {
		int j = ubpos(acc, acc[q[r]] - x);
		int cnt = dp.prod(0, j);
		dp.set(q[r], cnt + 1);
	}

	return dp.get(q[n]);
}


//【区間最大値の和の最小化（負値可）】O(n log n)
/*
* 数列 a[0..n)（負値可）に対して
*	a[l..r) のコスト：max a[l..r)
*	a[0..n) の区間分割のコスト：各区間コストの和
* と定め，2^(n-1) 通り全ての区間分割をわたるコストの最小値を返す．
*
* 利用：【最近傍要素】，【min 可換モノイド】
*/
template <class T>
T minimize_max_sum(const vector<T>& a) {
	// verify : https://www.codechef.com/problems/PARTITION

	//【方法】
	// dp[i] を a[0..i) についての答えとする（ただし dp[0] = 0）と，遷移式は
	//		dp[i+1] = Min_j∈[0..i] (dp[j] + max a[j..i])
	// となる．
	// 
	// a[i] より大きい直前の要素を a[l-1] とする（なければ l=0）．
	// 
	// j∈[l..i] の範囲では max a[j..i] = a[i] となるので，これらからの寄与は
	//		chmin(dp[i+1], min dp[l..i] + a[i])
	// として dp の区間 min をとることでまとめられる．
	// 
	// l > 0 のとき，j∈[0..l) の範囲では max a[j..i] = max a[j..i-1] となるので，
	// これらからの寄与は dp[i] と共通している．dp[i] のみに存在する因子は
	//		Min_j∈[l..i-1] (dp[j] + max a[j..i-1])
	//		= Min_j∈[l..i-1] dp[j] + a[i] （l の定め方より）
	//		= min dp[l..i-1] + a[i]
	//		≧ min dp[l..i] + a[i]
	// より無視できる．

	int n = sz(a);

	Jump_on_array NE(a);
	NE.set_prev_greater();

	Segtree<Min_monoid> dp(n + 1);
	dp.set(0, 0);

	rep(i, n) {
		int l = NE.prev_greater(i) + 1;
		T val = dp.prod(l, i + 1) + a[i];
		if (l > 0) chmin(val, dp.get(l));
		dp.set(i + 1, val);
	}

	return dp.get(n);
}


//【区間レンジの和の最大化（負値可）】O(n)
/*
* 数列 a[0..n)（負値可）に対して
*	a[l..r) のスコア：max a[l..r) - min a[l..r)
*	a[0..n) の区間分割のスコア：各区間スコアの和
* と定め，2^(n-1) 通り全ての区間分割をわたるスコアの最大値を返す．
* 
*（耳 DP）
*/
template <class T>
T maximize_range_sum(const vector<T>& a) {
	// 参考 : https://drken1215.hatenablog.com/entry/2023/09/04/214120
	// verify : https://atcoder.jp/contests/codequeen2023-final-open/tasks/codequeen2023_final_e

	//【方法】
	// 区間 [l..r) を固定したとき，a[l..r) のレンジはどんな差よりも大きいかまたは等しい．すなわち
	//		max a[l..r) - min a[l..r) = max_l≦i,j<r (a[i] - a[j])
	// が成り立つ．
	// 右辺は ±1 を丁度 1 つずつ含む {0,±1} の列の集合 S のある要素との内積とみなせる．
	// 最大値をまとめてとることにすれば，元の問題は a[0..n) と S* との内積の最大化とみなせる．

	int n = sz(a);

	// dp[i][j] : a[0..i) で，次に選ぶのが 0:+1, 1:-1, 2:任意 である場合のスコアの最大値
	vector<vector<T>> dp(n + 1, vector<T>(3, -(T)INFL));
	dp[0][2] = 0;

	rep(i, n) {
		chmax(dp[i + 1][0], dp[i][0]);
		chmax(dp[i + 1][2], dp[i][0] + a[i]);

		chmax(dp[i + 1][1], dp[i][1]);
		chmax(dp[i + 1][2], dp[i][1] - a[i]);

		chmax(dp[i + 1][0], dp[i][2] - a[i]);
		chmax(dp[i + 1][1], dp[i][2] + a[i]);
		chmax(dp[i + 1][2], dp[i][2]);
	}
	dumpel(dp);

	return dp[n][2];
}


