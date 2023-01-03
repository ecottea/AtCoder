#pragma once
#include "header.h"
#include "構造(グラフ).h"
#include "木DP.h"
// ■■■■■ 木（最適化） ■■■■■


//【部分木の最小コスト】O(n^2)
/*
* 頂点にコスト c[0..n) が与えられた木 g について，各 i∈[0..n] について，
* 頂点 r を含む大きさ i の部分木の最小コストを格納したリストを返す．
*
*（二乗の木 DP）
*
* 利用：【貰う木 DP（森経由）】
*/
// verify : https://atcoder.jp/contests/arc029/tasks/arc029_4
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
	c_mcs = c;
	return tree_getDP_forest<T_mcs, merge_mcs, e_mcs, leaf_mcs, apply_mcs>(g, r);
}

