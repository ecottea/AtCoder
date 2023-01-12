#pragma once
#include "header.h"
#include "構造(グラフ).h"
#include "木DP.h"
// ■■■■■ 木（最適化） ■■■■■


//【最大独立集合】O(n)
/*
* 木 g の最大独立集合（辺を共有しない最大の頂点集合）の大きさを返す．
*
* 利用：【貰う木 DP（頂点マージ）】
*/
using T_mis = pii; // (根を選択，根を非選択)
void merge_mis(T_mis& x, const T_mis& y, int s) { x.first += y.first - 1; x.second += y.second; }
T_mis e_mis() { return { 1, 0 }; }
T_mis leaf_mis(int s) { return { 1, 0 }; }
T_mis apply_mis(const T_mis& x, int s, int t) { return { x.second + 1, max(x.first, x.second) }; }
int maximum_independent_set(const Graph& g) {
	// verify : https://yukicoder.me/problems/no/763

	auto dp = tree_getDP_vmerge<T_mis, merge_mis, e_mis, leaf_mis, apply_mis>(g, 0);
	return max(dp[0].first, dp[0].second);
}


//【部分木の最小コスト（大きさ毎）】O(n^2)
/*
* 与えられた頂点がコスト c[0..n) をもつ r を根とする根付き木 g に対し，
* 各 s∈[0..n) および各 i∈[0..|s|]（|s| は部分木 s の大きさ）について，
* 部分木 s の大きさ i の部分木の最小コストを格納した二次元リストを返す．
*
*（二乗の木 DP）
*
* 利用：【貰う木 DP（森経由）】
*/
vl c_mcs; // 頂点コスト
using T_mcs = vl;
void merge_mcs(T_mcs& x, const T_mcs& y) {
	int ns = sz(x), nt = sz(y);

	T_mcs nx(ns + nt - 1, INFL);
	rep(i, ns) rep(j, nt) chmin(nx[i + j], x[i] + y[j]);
	x = move(nx);
}
T_mcs e_mcs() { return T_mcs{ 0 }; }
T_mcs leaf_mcs(int s) { return T_mcs{ 0, c_mcs[s] }; }
void apply_mcs(T_mcs& x, int s) {
	int ns = sz(x);
	x.resize(ns + 1);
	repir(i, ns, 1) x[i] = x[i - 1] + c_mcs[s];
	x[0] = 0;
}
vector<T_mcs> minimum_cost_subtree(const Graph& g, const vl& c, int r) {
	// verify : https://atcoder.jp/contests/arc029/tasks/arc029_4
	
	c_mcs = c;
	return tree_getDP_forest<T_mcs, merge_mcs, e_mcs, leaf_mcs, apply_mcs>(g, r);
}

