#pragma once
#include "header.h"
#include "構造(グラフ).h"
// ■■■■■ 木 DP ■■■■■


//【貰う木 DP】O(n)
/*
* 各 s∈[0..n) について，r を根とする根付き木 g の
* 部分木 s についての問題の答えを格納したリストを返す．
*
* void merge(T& x, T y, int s) :
*   根 s のみを共有する部分木 2 つに対する答えがそれぞれ x, y のとき，
*   x 側に y 側をマージして部分木 s についての答えを上書きする．
*
* T e() :
*   merge() の単位元を返す．
*
* T leaf(int s) :
*   葉 s のみからなる部分木についての答えを返す．
*
* T apply(T x, int p, int s) :
*   部分木 s についての暫定の答えが x のとき，
*   辺 p→s を追加した部分木 p についての答えを返す．
*/
template <class T, void(*merge)(T&, const T&, int), T(*e)(), T(*leaf)(int), T(*apply)(const T&, int, int)>
vector<T> tree_getDP(const Graph& g, int r) {
	// verify : https://atcoder.jp/contests/dp/tasks/dp_p

	int n = sz(g);
	vector<T> dp(n, e());

	// 部分木 s についての答えを計算する．（p : s の親）
	function<void(int, int)> dfs = [&](int s, int p) {
		// s が葉か
		bool is_leef = true;

		repe(t, g[s]) {
			if (t == p) continue;
			is_leef = false;

			// 部分木 t についての答えを計算する．
			dfs(t, s);

			// 部分木 t に対して辺 s→t を接続した場合の部分木 s についての答えを得る．
			T sub = apply(dp[t], s, t);

			// それを部分木 s の暫定の答えとマージして答えを更新していく．
			merge(dp[s], sub, s);
		}

		// s が葉の場合は自明な答えを代入しておく．
		if (is_leef) dp[s] = leaf(s);
	};
	dfs(r, -1);

	return dp;

	/* 雛形
	using T = int;
	void merge(T& x, const T& y, int s) { chmax(x, y); }
	T e() { return 0; }
	T leaf(int s) { return 0; }
	T apply(const T& x, int p, int s) { return x + 1; }
	vector<T> solve_by_tree_getDP(const Graph& g, int r) {
		return tree_getDP<T, merge, e, leaf, apply>(g, r);
	}
	*/
};


//【貰う木 DP（森経由）】O(n)
/*
* 各 s∈[0..n) について，r を根とする根付き木 g の
* 部分木 s についての問題の答えを格納したリストを返す．
*
* void merge(T& x, T y) :
*   ある部分森に対する答えが x, ある部分木に対する答えが y のとき，
*   これらをマージした部分森についての答えを x に上書きする．
*
* T e() :
*   merge() の単位元（空の森に対する答え）を返す．
*
* T leaf(int s) :
*   葉 s のみからなる部分木についての答えを返す．
*
* void apply(T& x, int s) :
*   ある部分森についての答えが x のとき，共通の根 s を追加した部分木についての答えを x に上書きする．
*/
template <class T, void(*merge)(T&, const T&), T(*e)(), T(*leaf)(int), void(*apply)(T&, int)>
vector<T> tree_getDP_forest(const Graph& g, int r) {
	int n = sz(g);

	// 空の森に対する答えで初期化する．
	vector<T> dp(n, e());

	// 部分木 s についての答えを計算する．（p : s の親）
	function<void(int, int)> dfs = [&](int s, int p) {
		// s が葉か
		bool is_leef = true;

		repe(t, g[s]) {
			if (t == p) continue;
			is_leef = false;

			// 部分木 t についての答えを計算する．
			dfs(t, s);

			// 部分木 t を森に加え答えを更新する．
			merge(dp[s], dp[t]);
		}

		// s が葉の場合は葉専用の答えを代入する．
		if (is_leef) dp[s] = leaf(s);
		// そうでないときは根 s を森に追加し答えを更新する．
		else apply(dp[s], s);
	};
	dfs(r, -1);

	return dp;

	/* 雛形
	using T = int;
	void merge(T& x, const T& y) { chmax(x, y); }
	T e() { return -INF; }
	T leaf(int s) { return 0; }
	void apply(T& x, int s) { x++; }
	vector<T> solve_by_tree_getDP(const Graph& g, int r) {
		return tree_getDP_forest<T, merge, e, leaf, apply>(g, r);
	}
	*/
};


//【貰う木 DP（コスト付き）】O(n)
/*
* 各 s∈[0..n) について，r を根とするコスト付き根付き木 g の
* 部分木 s についての問題の答えを格納したリストを返す．
*
* T merge(T x, T y, int s) :
*   根 s のみを共有する部分木 2 つに対する答えがそれぞれ x, y のとき，
*   x 側に y 側をマージして部分木 s についての答えを上書きする．
*
* T e() :
*   merge() の単位元を返す．
*
* T leaf(int s) :
*   葉 s のみからなる部分木についての答えを返す．
*
* T apply(T x, int p, int s, ll c) :
*   部分木 s についての暫定の答えが x のとき，
*   コスト c の辺 p→s を追加した部分木 p についての答えを返す．
*/
template <class T, void(*merge)(T&, const T&, int), T(*e)(), T(*leaf)(int), T(*apply)(const T&, int, int, ll)>
vector<T> tree_getDP(const WGraph& g, int r) {
	// verify : https://yukicoder.me/problems/no/417

	int n = sz(g);
	vector<T> dp(n, e());

	// 部分木 s についての答えを計算する．（p : s の親）
	function<void(int, int)> dfs = [&](int s, int p) {
		// s が葉か
		bool is_leef = true;

		repe(t, g[s]) {
			if (t == p) continue;
			is_leef = false;

			// 部分木 t についての答えを計算する．
			dfs(t, s);

			// 部分木 t に対して辺 s→t を接続した場合の部分木 s についての答えを得て，
			// それを部分木 s の暫定の答えとマージして答えを更新していく．
			merge(dp[s], apply(dp[t], s, t, t.cost), s);
		}

		// s が葉の場合は自明な答えを代入しておく．
		if (is_leef) dp[s] = leaf(s);
	};
	dfs(r, -1);

	return dp;

	/* 雛形
	using T = ll;
	void merge(T& x, const T& y, int s) { chmax(x, y); }
	T e() { return 0; }
	T leaf(int s) { return 0; }
	T apply(const T& x, int p, int s, ll c) { return x + c; }
	vector<T> solve_by_tree_getDP(const WGraph& g, int r) {
		return tree_getDP<T, merge, e, leaf, apply>(g, r);
	}
	*/
};


//【木の高さ】O(n)
/*
* 各 s∈[0..n) について，r を根とする木 g の頂点 s の高さを格納したリストを返す．
* s の高さとは，s から部分木 s の葉までの辺の本数の最大値のことである．
*
* 利用：【貰う木 DP（森経由）】
*/
// verify : https://algo-method.com/tasks/528
using T_hot = int;
void merge_hot(T_hot& x, const T_hot& y) { chmax(x, y); }
T_hot e_hot() { return -INF; }
T_hot leaf_hot(int s) { return 0; }
void apply_hot(T_hot& x, int s) { x++; }
vector<T_hot> height_of_tree(const Graph& g, int r) {
	return tree_getDP_forest<T_hot, merge_hot, e_hot, leaf_hot, apply_hot>(g, r);
}


//【木の高さ（コスト付き）】O(n)
/*
* 各 s∈[0..n) について，r を根とするコスト付き木 g の頂点 s の高さを格納したリストを返す．
* s の高さとは，s から部分木 s の葉までの辺のコストの和の最大値のことである．
*
* 利用：【貰う木 DP（コスト付き）】
*/
using T_hoct = ll;
void merge_hoct(T_hoct& x, const T_hoct& y, int s) { chmax(x, y); }
T_hoct e_hoct() { return 0; }
T_hoct leaf_hoct(int s) { return 0; }
T_hoct apply_hoct(const T_hoct& x, int s, int t, ll c) { return x + c; }
vector<T_hoct> height_of_weighted_tree(const WGraph& g, int r) {
	return tree_getDP<T_hoct, merge_hoct, e_hoct, leaf_hoct, apply_hoct>(g, r);
}


//【独立集合の数え上げ】O(n)
/*
* 木 g の独立集合（辺を共有しない頂点の集合）の個数を返す．
*
* 利用：【貰う木 DP】
*/
using T_cis = pair<mint, mint>; // (根を選択，根を非選択)
void merge_cis(T_cis& x, const T_cis& y, int s) { x.first *= y.first; x.second *= y.second; }
T_cis e_cis() { return { 1, 1 }; }
T_cis leaf_cis(int s) { return { 1, 1 }; }
T_cis apply_cis(const T_cis& x, int s, int t) { return { x.second, x.first + x.second }; }
mint count_independent_set(const Graph& g) {
	// verify : https://atcoder.jp/contests/dp/tasks/dp_p

	auto dp = tree_getDP<T_cis, merge_cis, e_cis, leaf_cis, apply_cis>(g, 0);
	return dp[0].first + dp[0].second;
}


//【最大独立集合】O(n)
/*
* 木 g の最大独立集合（辺を共有しない最大の頂点集合）の大きさを返す．
*
* 利用：【貰う木 DP】
*/
using T_mis = pii; // (根を選択，根を非選択)
void merge_mis(T_mis& x, const T_mis& y, int s) { x.first += y.first - 1; x.second += y.second; }
T_mis e_mis() { return { 1, 0 }; }
T_mis leaf_mis(int s) { return { 1, 0 }; }
T_mis apply_mis(const T_mis& x, int s, int t) { return { x.second + 1, max(x.first, x.second) }; }
int maximum_independent_set(const Graph& g) {
	// verify : https://yukicoder.me/problems/no/763

	auto dp = tree_getDP<T_mis, merge_mis, e_mis, leaf_mis, apply_mis>(g, 0);
	return max(dp[0].first, dp[0].second);
}


//【2 点間距離の和】O(n)
/*
* 木 g の全ての 2 点の組についての距離の総和を返す．
*
* 利用：【貰う木 DP】
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
	auto dp = tree_getDP<T_ds, merge_ds, e_ds, leaf_ds, apply_ds>(g, 0);
	return dp[0].first;
}


//【2 点間距離の和（部分木）】O(n)
/*
* 各 s∈[0..n) について，r を根とする木 g について
* 部分木 s の全ての 2 点の組についての距離の総和を格納したリストを返す．
*
* 利用：【貰う木 DP】
*/
using T_dss = tuple<ll, ll, ll>; // (距離の総和，根から子孫への距離の和, 頂点の数)
void merge_dss(T_dss& x, const T_dss& y, int s) {
	ll ds_x, ds2_x, cnt_x, ds_y, ds2_y, cnt_y;
	tie(ds_x, ds2_x, cnt_x) = x;
	tie(ds_y, ds2_y, cnt_y) = y;

	x = { ds_x + ds_y + ds2_x * (cnt_y - 1) + ds2_y * (cnt_x - 1), ds2_x + ds2_y, cnt_x + cnt_y - 1 };
}
T_dss e_dss() { return { 0, 0, 1 }; }
T_dss leaf_dss(int s) { return { 0, 0, 1 }; }
T_dss apply_dss(const T_dss& x, int s, int t) {
	ll ds, ds2, cnt;
	tie(ds, ds2, cnt) = x;

	return { ds + ds2 + cnt, ds2 + cnt, cnt + 1 };
}
vl distance_sum_subtree(const Graph& g, int r) {
	// verify : https://atcoder.jp/contests/abc248/tasks/abc248_g

	int n = sz(g);
	vl res(n);

	auto dp = tree_getDP<T_dss, merge_dss, e_dss, leaf_dss, apply_dss>(g, r);
	dumpel(dp);

	rep(s, n) res[s] = get<0>(dp[s]);
	return res;
}


