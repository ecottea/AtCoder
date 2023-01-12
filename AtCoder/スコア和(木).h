#pragma once
#include "header.h"
#include "木DP.h"
// ■■■■■ 木（スコア和） ■■■■■


//【2 点間距離の和】O(n)
/*
* 木 g の全ての 2 点の組についての距離の総和を返す．
*
* 利用：【貰う木 DP（頂点マージ）】
*/
using T_ds = pll; // (距離への寄与の和, 頂点の数)
int n_ds;
void merge_ds(T_ds& x, const T_ds& y, int s) { x.first += y.first; x.second += y.second - 1; }
T_ds e_ds() { return { 0, 1 }; }
T_ds leaf_ds(int s) { return { 0, 1 }; }
T_ds apply_ds(const T_ds& x, int s, int t) {
	return { x.first + x.second * (n_ds - x.second), x.second + 1 };
}
ll distance_sum(const Graph& g) {
	// verify : https://atcoder.jp/contests/typical90/tasks/typical90_am

	n_ds = sz(g);
	auto dp = tree_getDP_vmerge<T_ds, merge_ds, e_ds, leaf_ds, apply_ds>(g, 0);
	return dp[0].first;
}


//【2 点間距離の和（部分木内）】O(n)
/*
* 与えられた r を根とする木 g に対し，各 s∈[0..n) について，
* 部分木 s の全ての 2 点の組についての距離の総和を格納したリストを返す．
*
* 利用：【貰う木 DP（頂点マージ）】
*/
using T_dss = tuple<ll, ll, ll>; // (距離の総和，根から子孫への距離の和, 頂点の数)
void merge_dss(T_dss& x, const T_dss& y, int s) {
	auto [ds_x, ds2_x, cnt_x] = x;
	auto [ds_y, ds2_y, cnt_y] = y;

	ll nds = ds_x + ds_y + ds2_x * (cnt_y - 1) + ds2_y * (cnt_x - 1);
	ll nds2 = ds2_x + ds2_y;
	ll ncnt = cnt_x + cnt_y - 1;

	x = { nds, nds2, ncnt };
}
T_dss e_dss() { return { 0, 0, 1 }; }
T_dss leaf_dss(int s) { return { 0, 0, 1 }; }
T_dss apply_dss(const T_dss& x, int s, int t) {
	auto [ds, ds2, cnt] = x;

	ll nds = ds + ds2 + cnt;
	ll nds2 = ds2 + cnt;
	ll ncnt = cnt + 1;

	return { nds, nds2, ncnt };
}
vl distance_sum_subtree(const Graph& g, int r) {
	// verify : https://atcoder.jp/contests/typical90/tasks/typical90_am

	int n = sz(g);
	vl res(n);

	auto dp = tree_getDP_vmerge<T_dss, merge_dss, e_dss, leaf_dss, apply_dss>(g, r);

	rep(s, n) res[s] = get<0>(dp[s]);
	return res;
}


//【2 点間距離の和（端点固定）】O(n)
/*
* 与えられた木 g に対し，各 s∈[0..n) について，
* Σt∈[0..n) (s-t 間の距離) を格納したリストを返す．
*
* 利用：【全方位木 DP】
*/
using T_ds = pll; // (根からの距離の和, 頂点の数)
T_ds merge_ds(T_ds x, T_ds y, int s) { return { x.first + y.first, x.second + y.second - 1 }; }
T_ds e_ds() { return { 0, 1 }; }
T_ds leaf_ds(int s) { return { 0, 1 }; }
T_ds apply_ds(T_ds x, int s, int t) { return { x.first + x.second, x.second + 1 }; }
vl distance_sum(Graph& g) {
	// verify : https://atcoder.jp/contests/abc220/tasks/abc220_f

	int n = sz(g);

	auto tmp = rerooting<T_ds, merge_ds, e_ds, leaf_ds, apply_ds>(g);

	vl res(n);
	rep(i, n) res[i] = tmp[i].first;

	return res;
}


//【重さの和 = 深さの和】
/*
* 任意の根付き木 g について，以下の等式が成り立つ：
*		Σv∈V weight(v) = Σv∈V depth(v)
* ここで，weight(v) は部分木 v の辺の数，depth(v) は根から v までの辺の数である．
* 
* 証明：各頂点に 0，各辺に 1 を書き込み，頂点と辺をまとめて根方向への累積和をとる．
* weight(v) は頂点 v に何本の辺から値が撒かれたかに等しく，
* depth(v) は辺 p→v から何個の頂点に値を撒いたかに等しい．
*/


