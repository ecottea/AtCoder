#pragma once
#include "header.h"
#include "構造(グラフ).h"
#include "木DP.h"
// ■■■■■ 木（最適化） ■■■■■


//【最大独立集合】O(n)
/*
* 木 g の最大独立集合（辺を共有しない最大の頂点集合）の大きさを返す．
*
* 利用：【貰う木 DP】
*/
struct T_mis {
	int v1; // 根を選択
	int v0; // 根を非選択
};
T_mis leaf_mis(int s) {
	return T_mis{ 1, 0 };
}
T_mis add_edge_mis(const T_mis& x, int p, int s) {
	return T_mis{ x.v0, max(x.v1, x.v0) };
}
void merge_mis(T_mis& x, const T_mis& y, int s) {
	x.v0 += y.v0;
	x.v1 += y.v1;
}
void add_vertex_mis(T_mis& x, int s) {
	x.v1++;
}
int maximum_independent_set(const Graph& g) {
	// verify : https://yukicoder.me/problems/no/763

	auto dp = tree_getDP<T_mis, leaf_mis, add_edge_mis, merge_mis, add_vertex_mis>(g, 0);
	return max(dp[0].v1, dp[0].v0);
}


//【部分木の最小コスト（大きさ毎）】O(n^2)
/*
* 与えられた頂点がコスト c[0..n) をもつ r を根とする根付き木 g に対し，
* 各 s∈[0..n) および各 i∈[0..|s|]（|s| は部分木 s の大きさ）について，
* 部分木 s の大きさ i の部分木の最小コストを格納した二次元リストを返す．
*
*（二乗の木 DP）
*
* 利用：【貰う木 DP】
*/
vl c_mcs; // 頂点コスト
using T_mcs = vl;
T_mcs leaf_mcs(int s) {
	return T_mcs{ 0, c_mcs[s] };
}
T_mcs add_edge_mcs(const T_mcs& x, int p, int s) {
	return x;
}
void merge_mcs(T_mcs& x, const T_mcs& y, int s) {
	int ns = sz(x), nt = sz(y);

	T_mcs nx(ns + nt - 1, INFL);
	rep(i, ns) rep(j, nt) chmin(nx[i + j], x[i] + y[j]);
	x = move(nx);
}
void add_vertex_mcs(T_mcs& x, int s) {
	int ns = sz(x);
	x.resize(ns + 1);
	repir(i, ns, 1) x[i] = x[i - 1] + c_mcs[s];
	x[0] = 0;
}
vector<T_mcs> minimum_cost_subtree(const Graph& g, const vl& c, int r) {
	// verify : https://atcoder.jp/contests/arc029/tasks/arc029_4

	c_mcs = c;
	return tree_getDP<T_mcs, leaf_mcs, add_edge_mcs, merge_mcs, add_vertex_mcs>(g, r);
}


