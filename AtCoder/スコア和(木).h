#pragma once
#include "header.h"
#include "木DP.h"
// ■■■■■ 木（スコア和） ■■■■■


//【2 点間距離の和】O(n)
/*
* 木 g の全ての 2 点の組についての距離の総和を返す．
*/
ll distance_sum(const Graph& g) {
	// verify : https://atcoder.jp/contests/typical90/tasks/typical90_am

	int n = sz(g);
	if (n == 0) return 0;

	ll res = 0;

	// g を頂点 0 を根とする根付き木とみなしたときの部分木 s の頂点数を返す．
	function<int(int, int)> dfs = [&](int s, int p) {
		int w = 1;
		repe(t, g[s]) {
			if (t == p) continue;

			auto w2 = dfs(t, s);

			// 辺 s-t の寄与は (s 側の頂点数)×(t 側の頂点数)
			res += (ll)(n - w2) * w2;

			w += w2;
		}
		return w;
	};
	dfs(0, -1);

	return res;
}


//【2 点間距離の和（重み付き）】O(n)
/*
* 木 g の全ての 2 点の組についての距離の総和を返す．
*/
template <class T>
T distance_sum(const WGraph& g) {
	// verify : https://yukicoder.me/problems/no/1207

	int n = sz(g);
	if (n == 0) return T(0);

	T res = 0;

	// g を頂点 0 を根とする根付き木とみなしたときの部分木 s の頂点数を返す．
	function<int(int, int)> dfs = [&](int s, int p) {
		int w = 1;
		repe(t, g[s]) {
			if (t == p) continue;

			auto w2 = dfs(t, s);

			// 辺 s-t の寄与は (s 側の頂点数)×(t 側の頂点数)
			res += (T)t.cost * (n - w2) * w2;

			w += w2;
		}
		return w;
	};
	dfs(0, -1);

	return res;
}


//【2 点間距離の和（2 色）】O(n)
/*
* 木 g の col[s]=col[t]=1 である全ての 2 点の組 (s,t) についての距離の総和を返す．
*/
ll distance_sum(const Graph& g, const vi& col) {
	// verify : https://atcoder.jp/contests/abc359/tasks/abc359_g

	int n = sz(g);
	if (n == 0) return 0;

	int col_sum = 0;
	rep(s, n) col_sum += (col[s] == 1);

	ll res = 0;

	// g を頂点 0 を根とする根付き木とみなしたときの部分木 s の頂点数を返す．
	function<int(int, int)> dfs = [&](int s, int p) {
		int w = col[s];
		repe(t, g[s]) {
			if (t == p) continue;

			auto w2 = dfs(t, s);

			// 辺 s-t の寄与は (s 側の頂点数)×(t 側の頂点数)
			res += (ll)(col_sum - w2) * w2;

			w += w2;
		}
		return w;
	};
	dfs(0, -1);

	return res;
}


//【2 点間距離の和（色付き）】O(n log n)
/*
* 色毎に木を座標圧縮する，1/3 重心分解するなどすれば良い．
* 
* verify : https://atcoder.jp/contests/abc359/tasks/abc359_g
*/


//【2 点間距離の和（部分木）】O(n)
/*
* 与えられた r を根とする木 g に対し，各 s∈[0..n) について，
* 部分木 s の全ての 2 点の組についての距離の総和を格納したリストを返す．
*
* 利用：【貰う木 DP】
*/
struct T_dss {
	ll s; // 距離の総和
	ll r; // 根から子孫への距離の和
	ll c; // 頂点の数
};
T_dss leaf_dss(int s) {
	return { 0, 0, 1 };
}
T_dss add_edge_dss(const T_dss& x, int p, int s) {
	return T_dss{ x.s + x.r + x.c, x.r + x.c, x.c };
}
void merge_dss(T_dss& x, const T_dss& y, int s) {
	x.s += y.s + x.r * y.c + y.r * x.c;
	x.r += y.r;
	x.c += y.c;
}
void add_vertex_dss(T_dss& x, int s) {
	x.c += 1;
}
vl distance_sum_subtree(const Graph& g, int r) {
	// verify : https://atcoder.jp/contests/typical90/tasks/typical90_am

	int n = sz(g);

	vl res(n);

	auto dp = tree_getDP<T_dss, leaf_dss, add_edge_dss, merge_dss, add_vertex_dss>(g, r);
	rep(s, n) res[s] = dp[s].s;

	return res;
}


//【2 点間距離の和（端点固定）】O(n)
/*
* 与えられた木 g に対し，各 s∈[0..n) について，
* Σt∈[0..n) (s-t 間の距離) を格納したリストを返す．
*
* 利用：【全方位木 DP】
*/
struct T_dse {
	ll r; // 根からの距離の和
	ll c; // 頂点の数
};
T_dse leaf_dse(int s) {
	return { 0, 1 };
}
T_dse add_edge_dse(const T_dse& x, int p, int s) {
	return T_dse{ x.r + x.c, x.c };
}
T_dse merge_dse(const T_dse& x, const T_dse& y, int s) {
	return T_dse{ x.r + y.r, x.c + y.c };
}
T_dse add_vertex_dse(const T_dse& x, int s) {
	return T_dse{ x.r, x.c + 1 };
}
vl distance_sum_endpoint(const Graph& g) {
	// verify : https://atcoder.jp/contests/abc220/tasks/abc220_f

	int n = sz(g);

	vl res(n);

	auto dp = rerooting<T_dse, leaf_dse, add_edge_dse, merge_dse, add_vertex_dse>(g);
	rep(s, n) res[s] = dp[s].r;

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


