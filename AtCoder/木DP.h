#pragma once
#include "header.h"
#include "構造(グラフ).h"
// ■■■■■ 木 DP ■■■■■


//【貰う木 DP】O(n)
/*
* r を根とする根付き木 g の部分木 s についての問題の答えを dp[s] に格納する．
*
* void merge(T& x, T y) :
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
*   辺 p-s を追加した部分木 p についての答えを返す．
*/
template <class T, void(*merge)(T&, const T&), T(*e)(), T(*leaf)(int), T(*apply)(const T&, int, int)>
void tree_getDP(const Graph& g, int r, vector<T>& dp) {
	// verify : https://atcoder.jp/contests/dp/tasks/dp_p

	int n = sz(g);
	dp.resize(n, e());

	// 部分木 s についての答えを計算する．（p : s の親）
	function<void(int, int)> dfs = [&](int s, int p) {
		// s が葉か
		bool is_leef = true;

		repe(t, g[s]) {
			if (t == p) continue;
			is_leef = false;

			// 部分木 t についての答えを計算する．
			dfs(t, s);

			// 部分木 t に対して辺 s-t を接続した場合の部分木 s についての答えを得る．
			T sub = apply(dp[t], s, t);

			// それを部分木 s の暫定の答えとマージして答えを更新していく．
			merge(dp[s], sub);
		}

		// s が葉の場合は自明な答えを代入しておく．
		if (is_leef) dp[s] = leaf(s);
	};

	dfs(0, -1);

	/* 雛形
	using T = int;
	void merge(T& x, const T& y) { chmax(x, y); }
	T e() { return 0; }
	T leaf(int s) { return 0; }
	T apply(const T& x, int p, int s) { return x + 1; }
	void solve_by_tree_getDP(const Graph& g, int r, vector<T>& dp) {
		tree_getDP<T, merge, e, leaf, apply>(g, r, dp);
	}
	*/
};


//【貰う木 DP（コスト付き）】O(n)
/*
* r を根とするコスト付き根付き木 g の部分木 s についての問題の答えを dp[s] に格納する．
*
* T merge(T x, T y) :
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
*   コスト c の辺 p-s を追加した部分木 p についての答えを返す．
*/
template <class T, void(*merge)(T&, const T&), T(*e)(), T(*leaf)(int), T(*apply)(const T&, int, int, ll)>
void tree_getDP(const WGraph& g, int r, vector<T>& dp) {
	// verify : https://yukicoder.me/problems/no/417

	int n = sz(g);
	dp.resize(n, e());

	// 部分木 s についての答えを計算する．（p : s の親）
	function<void(int, int)> dfs = [&](int s, int p) {
		// s が葉か
		bool is_leef = true;

		repe(t, g[s]) {
			if (t == p) continue;
			is_leef = false;

			// 部分木 t についての答えを計算する．
			dfs(t, s);

			// 部分木 t に対して辺 s-t を接続した場合の部分木 s についての答えを得て，
			// それを部分木 s の暫定の答えとマージして答えを更新していく．
			merge(dp[s], apply(dp[t], s, t, t.cost));
		}

		// s が葉の場合は自明な答えを代入しておく．
		if (is_leef) dp[s] = leaf(s);
	};

	dfs(0, -1);

	/* 雛形
	using T = ll;
	void merge(T& x, const T& y) { chmax(x, y); }
	T e() { return 0; }
	T leaf(int s) { return 0; }
	T apply(const T& x, int p, int s, ll c) { return x + c; }
	void solve_by_tree_getDP(const WGraph& g, int r, vector<T>& dp) {
		tree_getDP<T, merge, e, leaf, apply>(g, r, dp);
	}
	*/
};


//【木の高さ】O(n)
/*
* r を根とする木 g の頂点 i の高さを h[i] に格納する．
*
* 利用：【貰う木 DP】
*/
// verify : https://mojacoder.app/users/milkcoffee/contests/milkcoffee-contest-001/tasks/4
using T_hot = int;
void merge_hot(T_hot& x, const T_hot& y) { chmax(x, y); }
T_hot e_hot() { return 0; }
T_hot leaf_hot(int s) { return 0; }
T_hot apply_hot(const T_hot& x, int s, int t) { return x + 1; }
void height_of_tree(const Graph& g, int r, vector<T_hot>& h) {
	tree_getDP<T_hot, merge_hot, e_hot, leaf_hot, apply_hot>(g, r, h);
}


//【木の高さ（コスト付き）】O(n)
/*
* r を根とするコスト付き木 g の頂点 i の高さを h[i] に格納する．
*
* 利用：【貰う木 DP（コスト付き）】
*/
using T_hoct = ll;
void merge_hoct(T_hoct& x, const T_hoct& y) { chmax(x, y); }
T_hoct e_hoct() { return 0; }
T_hoct leaf_hoct(int s) { return 0; }
T_hoct apply_hoct(const T_hoct& x, int s, int t, ll c) { return x + c; }
void height_of_weighted_tree(const WGraph& g, int r, vector<T_hoct>& h) {
	tree_getDP<T_hoct, merge_hoct, e_hoct, leaf_hoct, apply_hoct>(g, r, h);
}


//【独立集合の数え上げ】O(n)
/*
* 木 g の独立集合（辺を共有しない頂点の集合）の個数を返す．
*
* 利用：【貰う木 DP】
*/
using T_cis = pair<mint, mint>; // (根が黒, 根が白)
void merge_cis(T_cis& x, const T_cis& y) { x.first *= y.first; x.second *= y.second; }
T_cis e_cis() { return { 1, 1 }; }
T_cis leaf_cis(int s) { return { 1, 1 }; }
T_cis apply_cis(const T_cis& x, int s, int t) { return { x.second, x.first + x.second }; }
mint count_independent_set(const Graph& g) {
	// verify : https://atcoder.jp/contests/dp/tasks/dp_p

	vector<T_cis> dp;
	tree_getDP<T_cis, merge_cis, e_cis, leaf_cis, apply_cis>(g, 0, dp);
	return dp[0].first + dp[0].second;
}


//【最大独立集合】O(n)
/*
* 木 g の最大独立集合（辺を共有しない最大の頂点集合）の大きさを返す．
*
* 利用：【貰う木 DP】
*/
using T_mis = pii; // (根が存在, 根を削除)
void merge_mis(T_mis& x, const T_mis& y) { x.first += y.first - 1; x.second += y.second; }
T_mis e_mis() { return { 1, 0 }; }
T_mis leaf_mis(int s) { return { 1, 0 }; }
T_mis apply_mis(const T_mis& x, int s, int t) { return { max(x.first, x.second + 1), max(x.first, x.second) }; }
int maximum_independent_set(const Graph& g) {
	// verify : https://yukicoder.me/problems/no/763

	vector<T_mis> dp;
	tree_getDP<T_mis, merge_mis, e_mis, leaf_mis, apply_mis>(g, 0, dp);
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
void merge_ds(T_ds& x, const T_ds& y) { x.first += y.first; x.second += y.second - 1; }
T_ds e_ds() { return { 0, 1 }; }
T_ds leaf_ds(int s) { return { 0, 1 }; }
T_ds apply_ds(const T_ds& x, int s, int t) {
	return { x.first + x.second * (n_ds - x.second), x.second + 1 };
}
ll distance_sum(const Graph& g) {
	// verify : https://atcoder.jp/contests/typical90/tasks/typical90_am

	n_ds = sz(g);
	vector<T_ds> dp;
	tree_getDP<T_ds, merge_ds, e_ds, leaf_ds, apply_ds>(g, 0, dp);
	return dp[0].first;
}


//【2 点間距離の和（部分木）】O(n)
/*
* 木 g について，部分木 s の全ての 2 点の組についての距離の総和を ds[s] に格納する．
*
* 利用：【貰う木 DP】
*/
using T_dss = tuple<ll, ll, ll>; // (距離の総和，根から子孫への距離の和, 頂点の数)
void merge_dss(T_dss& x, const T_dss& y) {
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
void distance_sum_subtree(const Graph& g, vl& ds) {
	// verify : https://atcoder.jp/contests/abc248/tasks/abc248_g

	int n = sz(g);
	ds.resize(n);

	vector<T_dss> dp;
	tree_getDP<T_dss, merge_dss, e_dss, leaf_dss, apply_dss>(g, 0, dp);
	dumpel(dp);

	rep(s, n) ds[s] = get<0>(dp[s]);
}


