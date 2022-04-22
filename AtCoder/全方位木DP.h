#pragma once
#include "header.h"
#include "構造(グラフ).h"
#include "二項係数.h"
// ■■■■■ 全方位木 DP ■■■■■


//【全方位木 DP】O(n)
/*
* 木 g の頂点 s を根と見たときの問題の答えを res[s] に格納する．
*
* T merge(T x, T y) :
*   根 s のみを共有する部分木 2 つに対する答えがそれぞれ x, y のとき，
*   これらをマージした部分木について同じく s を根と見たときの答えを返す．
*
* T e() :
*   merge() の単位元を返す．
*
* T leaf(int s) :
*   単独のノード s のみからなる部分木について，s を根と見たときの答えを返す．
*
* T apply(T x, int s, int t) :
*   頂点 t を根とする部分木の暫定の答えが x のとき，
*   s, t を結ぶ辺を追加して s を根と見たときの答えを返す．
*/
template <class T, T(*merge)(T, T), T(*e)(), T(*leaf)(int), T(*apply)(T, int, int)>
void rerooting(const Graph& g, vector<T>& res) {
	// 参考 : https://atcoder.jp/contests/abc222/editorial/2749
	// verify : https://atcoder.jp/contests/dp/tasks/dp_v
	
	int n = sz(g);
	res.resize(n);

	// dp[s] : p-s 間の辺を切断し，s を根と見たときの答え
	vector<T> dp(n, e());

	// p-s 間の辺を切断し，s を根と見たときの答えを計算する．（p : s の親）
	function<void(int, int)> dfs1 = [&](int s, int p) {
		// s が葉か
		bool is_leef = true;

		repe(t, g[s]) {
			if (t == p) continue;
			is_leef = false;

			// s-t 間の辺を切断し，t を根と見たときの答えを計算する．
			dfs1(t, s);

			// 先の部分木に対して辺 s-t を接続した場合の答えを得て，
			// それを暫定の答えとマージして自身の答えを計算していく．
			dp[s] = merge(dp[s], apply(dp[t], s, t));
		}

		// s が葉の場合は自明な答えを代入しておく．
		if (is_leef) dp[s] = leaf(s);
	};

	// s を根と見たときの答えを計算する．（p : s の親）
	//  val : 根 s に辺 s-p だけを接続したときの答え
	function<void(int, int, const T&)> dfs2 = [&](int s, int p, const T& val) {
		// ds : 根 s から出る各辺について，その辺だけを s に接続したときの答えのリスト
		vector<T> ds{ val };

		repe(t, g[s]) {
			if (t == p) continue;

			// s-t 間の辺を切断し，t を根と見たときの答えは計算し終えているので，
			// その部分木に対して辺 s-t を接続し s を根と見た場合の答えを得る．
			ds.push_back(apply(dp[t], s, t));
		}
		int k = sz(ds);

		// acc_l[acc_r] : 根 s の左[右]からの辺を順に s に接続したときの答えのリスト
		vector<T> acc_l(k + 1, e()), acc_r(k + 1, e());

		rep(i, k) acc_l[i + 1] = merge(acc_l[i], ds[i]);
		repir(i, k - 1, 0) acc_r[i] = merge(acc_r[i + 1], ds[i]);

		// 根 s から出る全ての辺を s に接続したときの答えが求めるものである．
		res[s] = acc_l[k];

		int i = 1;
		repe(t, g[s]) {
			if (t == p) continue;

			// sub : 根 s に辺 s-t 以外の全ての辺を接続したときの答え
			T sub = merge(acc_l[i], acc_r[i + 1]);

			// 先の部分木に辺 t-s を接続して t を根と見た場合の答えを再帰関数に渡す．
			dfs2(t, s, apply(sub, t, s));

			i++;
		}
	};

	dfs1(0, -1);
	dfs2(0, -1, e());

	/* 雛形
	using T = int;
	T merge(T x, T y) { return max(x, y); }
	T e() { return 0; }
	T leaf(int s) { return 0; }
	T apply(T x, int s, int t) { return x + 1; }
	void solve_by_rerooting(Graph& g, vector<T>& res) {
		rerooting<T, merge, e, leaf, apply>(g, res);
	}
	*/
};


//【全方位部分木 DP】O(n)
/*
* 木 g の頂点 s を根と見たときの問題の答えを res[s] に格納する．
* また頂点 s と接続する ti 番目の頂点を t としたとき，
* s-t 間の辺を切断し，t を根と見たときの答えを cut[s][ti] に格納する．
*
* T merge(T x, T y) :
*   根 s のみを共有する部分木 2 つに対する答えがそれぞれ x, y のとき，
*   これらをマージした部分木について同じく s を根と見たときの答えを返す．
*
* T e() :
*   merge() の単位元を返す．
*
* T leaf(int s) :
*   単独のノード s のみからなる部分木について，s を根と見たときの答えを返す．
*
* T apply(T x, int s, int t) :
*   頂点 t を根とする部分木の暫定の答えが x のとき，
*   s, t を結ぶ辺を追加して s を根と見たときの答えを返す．
*/
template <class T, T(*merge)(T, T), T(*e)(), T(*leaf)(int), T(*apply)(T, int, int)>
void rerooting(const Graph& g, vector<T>& res, vector<vector<T>>& cut) {
	// verify : https://atcoder.jp/contests/abc149/tasks/abc149_f

	int n = sz(g);
	res.resize(n);

	// cut[s][i] : 頂点 s と接続する i 番目の頂点を t としたとき，
	//             s-t 間の辺を切断し，t を根と見たときの答え
	cut.resize(n);
	rep(s, n) cut[s] = vector<T>(sz(g[s]), e());

	// p-s 間の辺を切断し，s を根と見たときの答えを計算する．
	//  p : s の親
	//  si : s が p に接続する何番目の頂点か
	function<void(int, int, int)> dfs1 = [&](int s, int p, int si) {
		// s が葉か
		bool is_leef = true;

		rep(ti, sz(g[s])) {
			int t = g[s][ti];
			if (t == p) continue;
			is_leef = false;

			// s-t 間の辺を切断し，t を根と見たときの答えを計算する．
			dfs1(t, s, ti);

			// 先の部分木に対して辺 s-t を接続した場合の答えを得て，
			// それを暫定の答えとマージして自身の答えを計算していく．
			if (p != -1) {
				cut[p][si] = merge(cut[p][si], apply(cut[s][ti], s, t));
			}
		}

		// s が葉の場合は自明な答えを代入しておく．
		if (is_leef && p != -1) cut[p][si] = leaf(s);
	};

	// s を根と見たときの答えを計算する．
	//  p : s の親
	//  val : s-p 間の辺を切断し，p を根と見たときの答え
	function<void(int, int, const T&)> dfs2 = [&](int s, int p, const T& val) {
		// ds : 根 s から出る各辺について，その辺だけを s に接続したときの答えのリスト
		vector<T> ds{ p != -1 ? apply(val, s, p) : e() };

		rep(ti, sz(g[s])) {
			int t = g[s][ti];
			if (t == p) {
				cut[s][ti] = val;
				continue;
			}

			// s-t 間の辺を切断し，t を根と見たときの答えは計算し終えているので，
			// その部分木に対して辺 s-t を接続し s を根と見た場合の答えを得る．
			ds.push_back(apply(cut[s][ti], s, t));
		}
		int k = sz(ds);

		// acc_l[acc_r] : 根 s の左[右]からの辺を順に s に接続したときの答えのリスト
		vector<T> acc_l(k + 1, e()), acc_r(k + 1, e());

		rep(i, k) acc_l[i + 1] = merge(acc_l[i], ds[i]);
		repir(i, k - 1, 0) acc_r[i] = merge(acc_r[i + 1], ds[i]);

		// 根 s から出る全ての辺を s に接続したときの答えが求めるものである．
		res[s] = acc_l[k];

		int i = 1;
		rep(ti, sz(g[s])) {
			int t = g[s][ti];
			if (t == p) continue;

			// 根 s に辺 s-t 以外の全ての辺を接続したときの答え，
			// すなわち，辺 t-s を切断し，s を根と見たときの答えを再帰関数に渡す．
			dfs2(t, s, merge(acc_l[i], acc_r[i + 1]));

			i++;
		}
	};

	dfs1(0, -1, -1);
	dfs2(0, -1, e());

	/* 雛形
	using T = int;
	T merge(T x, T y) { return max(x, y); }
	T e() { return 0; }
	T leaf(int s) { return 0; }
	T apply(T x, int s, int t) { return x + 1; }
	void solve_by_rerooting(Graph& g, vector<T>& res) {
		rerooting<T, merge, e, leaf, apply>(g, res);
	}
	*/
};


//【全方位木可逆 DP】O(n)
/*
* 木 g の頂点 s を根と見たときの問題の答えを res[s] に格納する．
*
* merge(T& x, T y) :
*   根 s のみを共有する部分木 2 つに対する答えがそれぞれ x, y のとき，
*   x 側に y 側をマージして s を根と見たときの答えを上書きする．
*
* T erase(T x, T y) :
*   s を根とする答えが x である部分木から，それに含まれるような
*	答えが y である部分木を取り除いたときの s を根と見た答えを返す．
*
* T e() :
*   merge() の単位元を返す．
*
* T leaf(int s) :
*   単独のノード s のみからなる部分木について，s を根と見たときの答えを返す．
*
* T apply(T x, int s, int t) :
*   頂点 t を根とする部分木の暫定の答えが x のとき，
*   s, t を結ぶ辺を追加して s を根と見たときの答えを返す．
*/
template <class T, void(*merge)(T&, const T&), T(*erase)(const T&, const T&),
	T(*e)(), T(*leaf)(int), T(*apply)(const T&, int, int)>
	void rerooting(const Graph& g, vector<T>& res) {
	// verify : https://atcoder.jp/contests/abc220/tasks/abc220_f

	int n = sz(g);
	res = vector<T>(n, e());

	// dp[s] : p-s 間の辺を切断し，s を根と見たときの答え
	vector<T> dp(n, e());

	// p-s 間の辺を切断し，s を根と見たときの答えを計算する．（p : s の親）
	function<void(int, int)> dfs1 = [&](int s, int p) {
		// s が葉か
		bool is_leef = true;

		repe(t, g[s]) {
			if (t == p) continue;
			is_leef = false;

			// s-t 間の辺を切断し，t を根と見たときの答えを計算する．
			dfs1(t, s);

			// 先の部分木に対して辺 s-t を接続した場合の答えを得て，
			// それを暫定の答えとマージして自身の答えを計算していく．
			merge(dp[s], apply(dp[t], s, t));
		}

		// s が葉の場合は自明な答えを代入しておく．
		if (is_leef) dp[s] = leaf(s);
	};

	// s を根と見たときの答えを計算する．（p : s の親）
	//  val : 根 s に辺 s-p だけを接続したときの答え
	function<void(int, int, const T&)> dfs2 = [&](int s, int p, const T& val) {
		// ds : 根 s から出る各辺について，その辺だけを s に接続したときの答えのリスト
		vector<T> ds{ val };

		repe(t, g[s]) {
			if (t == p) continue;

			// s-t 間の辺を切断し，t を根と見たときの答えは計算し終えているので，
			// その部分木に対して辺 s-t を接続し s を根と見た場合の答えを得る．
			ds.push_back(apply(dp[t], s, t));
		}
		int k = sz(ds);

		// 根 s から出る全ての辺を s に接続したときの答え res[s] を得る．
		rep(i, k) merge(res[s], ds[i]);

		int i = 1;
		repe(t, g[s]) {
			if (t == p) continue;

			// sub : 根 s に辺 s-t 以外の全ての辺を接続したときの答えを復元する．
			T sub = erase(res[s], ds[i]);

			// 先の部分木に辺 t-s を接続して t を根と見た場合の答えを再帰関数に渡す．
			dfs2(t, s, apply(sub, t, s));

			i++;
		}
	};

	dfs1(0, -1);
	dfs2(0, -1, e());

	/* 雛形
	using T = int;
	void merge(T& x, T y) { return x + y - 1; }
	T erase(T x, T y) { return x - y + 1; }
	T e() { return 1; }
	T leaf(int s) { return 1; }
	T apply(T x, int s, int t) { return x + 1; }
	void solve_by_rerooting(Graph& g, vector<T>& res) {
		rerooting<T, merge, erase, e, leaf, apply>(g, res);
	}
	*/
};


//【全方位木 DP（コスト付き）】O(n)
/*
* コスト付き木 g の頂点 s を根と見たときの問題の答えを res[s] に格納する．
*
* Edge : 辺の型（int, WEdge, IEdge）
* T : 答えの型
*
* T merge(T x, T y) :
*   根 s のみを共有する部分木 2 つに対する答えがそれぞれ x, y のとき，
*   これらをマージした部分木について同じく s を根と見たときの答えを返す．
*
* T e() :
*   merge() の単位元を返す．
*
* T leaf(int s) :
*   単独のノード s のみからなる部分木について，s を根と見たときの答えを返す．
*
* T apply(T x, int s, int t, ll c) :
*   頂点 t を根とする部分木の暫定の答えが x のとき，
*   s, t を結ぶコスト c の辺を追加して s を根と見たときの答えを返す．
*/
template <class T, T(*merge)(T, T), T(*e)(), T(*leaf)(int), T(*apply)(T, int, int, ll)>
void rerooting(const WGraph& g, vector<T>& res) {
	// 参考 : https://atcoder.jp/contests/abc222/editorial/2749
	// verify : https://atcoder.jp/contests/abc222/tasks/abc222_f

	int n = sz(g);
	res.resize(n);

	// dp[s] : p-s 間の辺を切断し，s を根と見たときの答え
	vector<T> dp(n, e());

	// p-s 間の辺を切断し，s を根と見たときの答えを計算する．（p : s の親）
	function<void(int, int)> dfs1 = [&](int s, int p) {
		// s が葉か
		bool is_leef = true;

		repe(t, g[s]) {
			if (t == p) continue;
			is_leef = false;

			// s-t 間の辺を切断し，t を根と見たときの答えを計算する．
			dfs1(t, s);

			// 先の部分木に対して辺 s-t を接続した場合の答えを得て，
			// それを暫定の答えとマージして自身の答えを計算していく．
			dp[s] = merge(dp[s], apply(dp[t], s, t, t.cost));
		}

		// s が葉の場合は自明な答えを代入しておく．
		if (is_leef) dp[s] = leaf(s);
	};

	// s を根と見たときの答えを計算する．（p : s の親）
	//  val : 根 s に辺 s-p だけを接続したときの答え
	function<void(int, int, const T&)> dfs2 = [&](int s, int p, const T& val) {
		// ds : 根 s から出る各辺について，その辺だけを s に接続したときの答えのリスト
		vector<T> ds{ val };

		repe(t, g[s]) {
			if (t == p) continue;

			// s-t 間の辺を切断し，t を根と見たときの答えは計算し終えているので，
			// その部分木に対して辺 s-t を接続し s を根と見た場合の答えを得る．
			ds.push_back(apply(dp[t], s, t, t.cost));
		}
		int k = sz(ds);

		// acc_l[acc_r] : 根 s の左[右]からの辺を順に s に接続したときの答えのリスト
		vector<T> acc_l(k + 1, e()), acc_r(k + 1, e());

		rep(i, k) acc_l[i + 1] = merge(acc_l[i], ds[i]);
		repir(i, k - 1, 0) acc_r[i] = merge(acc_r[i + 1], ds[i]);

		// 根 s から出る全ての辺を s に接続したときの答えが求めるものである．
		res[s] = acc_l[k];

		int i = 1;
		repe(t, g[s]) {
			if (t == p) continue;

			// sub : 根 s に辺 s-t 以外の全ての辺を接続したときの答え
			T sub = merge(acc_l[i], acc_r[i + 1]);

			// 先の部分木に辺 t-s を接続して t を根と見た場合の答えを再帰関数に渡す．
			dfs2(t, s, apply(sub, t, s, t.cost));

			i++;
		}
	};

	dfs1(0, -1);
	dfs2(0, -1, e());

	/* 雛形
	using T = ll;
	T merge(T x, T y) { return max(x, y); }
	T e() { return 0; }
	T leaf(int s) { return 0; }
	T apply(T x, int s, int t, T c) { return x + c; }
	void solve_by_rerooting(WGraph& g, vector<T>& res) {
		rerooting<T, merge, e, leaf, apply>(g, res);
	}
	*/
};


//【全方位部分木 DP（コスト付き）】O(n)
/*
* コスト付き木 g の頂点 s を根と見たときの問題の答えを res[s] に格納する．
* また頂点 s と接続する ti 番目の頂点を t としたとき，
* s-t 間の辺を切断し，t を根と見たときの答えを cut[s][ti] に格納する．
*
* T merge(T x, T y) :
*   根 s のみを共有する部分木 2 つに対する答えがそれぞれ x, y のとき，
*   これらをマージした部分木について同じく s を根と見たときの答えを返す．
*
* T e() :
*   merge() の単位元を返す．
*
* T leaf(int s) :
*   単独のノード s のみからなる部分木について，s を根と見たときの答えを返す．
*
* T apply(T x, int s, int t, ll c) :
*   頂点 t を根とする部分木の暫定の答えが x のとき，
*   s, t を結ぶコスト c の辺を追加して s を根と見たときの答えを返す．
*/
template <class T, T(*merge)(T, T), T(*e)(), T(*leaf)(int), T(*apply)(T, int, int, ll)>
void rerooting(const WGraph& g, vector<T>& res, vector<vector<T>>& cut) {
	int n = sz(g);
	res.resize(n);

	// cut[s][i] : 頂点 s と接続する i 番目の頂点を t としたとき，
	//             s-t 間の辺を切断し，t を根と見たときの答え
	cut.resize(n);
	rep(s, n) cut[s] = vector<T>(sz(g[s]), e());

	// p-s 間の辺を切断し，s を根と見たときの答えを計算する．
	//  p : s の親
	//  si : s が p に接続する何番目の頂点か
	function<void(int, int, int)> dfs1 = [&](int s, int p, int si) {
		// s が葉か
		bool is_leef = true;

		rep(ti, sz(g[s])) {
			auto t = g[s][ti];
			if (t == p) continue;
			is_leef = false;

			// s-t 間の辺を切断し，t を根と見たときの答えを計算する．
			dfs1(t, s, ti);

			// 先の部分木に対して辺 s-t を接続した場合の答えを得て，
			// それを暫定の答えとマージして自身の答えを計算していく．
			if (p != -1) {
				cut[p][si] = merge(cut[p][si], apply(cut[s][ti], s, t, t.cost));
			}
		}

		// s が葉の場合は自明な答えを代入しておく．
		if (is_leef && p != -1) cut[p][si] = leaf(s);
	};

	// s を根と見たときの答えを計算する．
	//  p : s の親
	//  c : s-p 間の辺のコスト
	//  val : s-p 間の辺を切断し，p を根と見たときの答え
	function<void(int, int, ll, const T&)> dfs2 = [&](int s, int p, ll c, const T& val) {
		// ds : 根 s から出る各辺について，その辺だけを s に接続したときの答えのリスト
		vector<T> ds{ p != -1 ? apply(val, s, p, c) : e() };

		rep(ti, sz(g[s])) {
			auto t = g[s][ti];
			if (t == p) {
				cut[s][ti] = val;
				continue;
			}

			// s-t 間の辺を切断し，t を根と見たときの答えは計算し終えているので，
			// その部分木に対して辺 s-t を接続し s を根と見た場合の答えを得る．
			ds.push_back(apply(cut[s][ti], s, t, t.cost));
		}
		int k = sz(ds);

		// acc_l[acc_r] : 根 s の左[右]からの辺を順に s に接続したときの答えのリスト
		vector<T> acc_l(k + 1, e()), acc_r(k + 1, e());

		rep(i, k) acc_l[i + 1] = merge(acc_l[i], ds[i]);
		repir(i, k - 1, 0) acc_r[i] = merge(acc_r[i + 1], ds[i]);

		// 根 s から出る全ての辺を s に接続したときの答えが求めるものである．
		res[s] = acc_l[k];

		int i = 1;
		rep(ti, sz(g[s])) {
			auto t = g[s][ti];
			if (t == p) continue;

			// 根 s に辺 s-t 以外の全ての辺を接続したときの答え，
			// すなわち，辺 t-s を切断し，s を根と見たときの答えを再帰関数に渡す．
			dfs2(t, s, t.cost, merge(acc_l[i], acc_r[i + 1]));

			i++;
		}
	};

	dfs1(0, -1, -1);
	dfs2(0, -1, INFL, e());

	/* 雛形
	using T = ll;
	T merge(T x, T y) { return max(x, y); }
	T e() { return 0; }
	T leaf(int s) { return 0; }
	T apply(T x, int s, int t, T c) { return x + c; }
	void solve_by_rerooting(WGraph& g, vector<T>& res) {
		rerooting<T, merge, e, leaf, apply>(g, res);
	}
	*/
};


//【木の高さ】O(n)
/*
* 木 g の頂点 i を根にしたときの高さ（最も遠い葉までの距離）を h[i] に格納する．
*
* 利用：【全方位木 DP】
*/
using T_hut = int;
T_hut merge_hut(T_hut x, T_hut y) { return max(x, y); }
T_hut e_hut() { return 0; }
T_hut leaf_hut(int s) { return 0; }
T_hut apply_hut(T_hut x, int s, int t) { return x + 1; }
void height_of_undirected_tree(Graph& g, vi& h) {
	rerooting<T_hut, merge_hut, e_hut, leaf_hut, apply_hut>(g, h);
}


//【木の高さ（コスト付き）】O(n)
/*
* コスト付き木 g の頂点 i を根にしたときの高さ（最も遠い葉までのコスト）を h[i] に格納する．
*
*（全方位木 DP）
*/
using T_hutc = ll;
T_hutc merge_hutc(T_hutc x, T_hutc y) { return max(x, y); }
T_hutc e_hutc() { return 0; }
T_hutc leaf_hutc(int s) { return 0; }
T_hutc apply_hutc(T_hutc x, int s, int t, T_hutc c) { return x + c; }
void height_of_undirected_tree(WGraph& g, vl& h) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_5_B

	rerooting<T_hutc, merge_hutc, e_hutc, leaf_hutc, apply_hutc>(g, h);
}


//【部分木の数え上げ】O(n)
/*
* 木 g の部分木のうち頂点 i を含むものの個数を cnt[i] に格納する．
*
* 利用：【全方位木 DP】
*/
using T_st = mint;
T_st merge_st(T_st x, T_st y) { return x * y; }
T_st e_st() { return 1; }
T_st leaf_st(int s) { return 1; }
T_st apply_st(T_st x, int s, int t) { return x + 1; }
void count_subtree(Graph& g, vm& cnt) {
	// verify : https://atcoder.jp/contests/dp/tasks/dp_v

	rerooting<T_st, merge_st, e_st, leaf_st, apply_st>(g, cnt);
}


//【最大マッチングの共通部分】O(n)
/*
* 頂点 i が木 g の全ての最大マッチングに含まれるかを b[i] に格納する．
*
* 利用：【全方位木 DP】
*/
using T_mmi = bool; // 根を必ず使うか
T_mmi merge_mmi(T_mmi x, T_mmi y) { return x || y; }
T_mmi e_mmi() { return false; }
T_mmi leaf_mmi(int s) { return false; }
T_mmi apply_mmi(T_mmi x, int s, int t) { return !x; }
void maximum_matching_intersection(Graph& g, vb& res) {
	// verify : https://atcoder.jp/contests/abc223/tasks/abc223_g

	rerooting<T_mmi, merge_mmi, e_mmi, leaf_mmi, apply_mmi>(g, res);
}


//【2 点間距離の和（端点固定）】O(n)
/*
* Σt=[0..n) (s-t 間の距離) を dist[s] に格納する．
*
* 利用：【全方位木可逆 DP】
*/
using T_ds = pll; // (根からの距離の和, 頂点の数)
void merge_ds(T_ds& x, const T_ds& y) { x.first += y.first; x.second += y.second - 1; }
T_ds erase_ds(const T_ds& x, const T_ds& y) { return { x.first - y.first, x.second - y.second + 1 }; }
T_ds e_ds() { return { 0, 1 }; }
T_ds leaf_ds(int s) { return { 0, 1 }; }
T_ds apply_ds(const T_ds& x, int s, int t) { return { x.first + x.second, x.second + 1 }; }
void distance_sum(Graph& g, vl& dist) {
	// verify : https://atcoder.jp/contests/abc220/tasks/abc220_f

	int n = sz(g);
	dist.resize(n);

	vector<pll> tmp;
	rerooting<T_ds, merge_ds, erase_ds, e_ds, leaf_ds, apply_ds>(g, tmp);

	rep(i, n) dist[i] = tmp[i].first;
}


//【木の構築方法の数え上げ】O(n)
/*
* 頂点 s から始めて連結性を保ったまま辺の追加を行い木 g を構築する方法の数を cnt[s] に格納する．
*
* 利用：【全方位木 DP】，【階乗など（法が大きな素数）】
*/
using T_cctc = pair<mint, int>; // (構築方法の数, 辺の数)
Factorial_mint fm_cctc;
T_cctc merge_cctc(T_cctc x, T_cctc y) {
	mint cnt = x.first * y.first * fm_cctc.binomial(x.second + y.second, x.second);
	return { cnt, x.second + y.second };
}
T_cctc e_cctc() { return { 1, 0 }; }
T_cctc leaf_cctc(int s) { return { 1, 0 }; }
T_cctc apply_cctc(T_cctc x, int s, int t) { return { x.first, x.second + 1 }; }
void count_continuous_tree_construction(Graph& g, vm& cnt) {
	// verify : https://atcoder.jp/contests/abc160/tasks/abc160_f

	int n = sz(g);
	cnt.resize(n);
	fm_cctc = Factorial_mint(n);

	vector<T_cctc> tmp;
	rerooting<T_cctc, merge_cctc, e_cctc, leaf_cctc, apply_cctc>(g, tmp);

	rep(i, n) cnt[i] = tmp[i].first;
}


//【部分木の大きさ】O(n)
/*
* 木 g において頂点 s と接続する ti 番目の頂点を t としたとき，
* s-t 間の辺を切断し，t を根と見たときの部分木の頂点数を cnt[s][ti] に格納する．
*
* 利用：【全方位部分木 DP】
*/
using T_ss = int;
T_ss merge_ss(T_ss x, T_ss y) { return x + y - 1; }
T_ss e_ss() { return 1; }
T_ss leaf_ss(int s) { return 1; }
T_ss apply_ss(T_ss x, int s, int t) { return x + 1; }
void subtree_size(Graph& g, vvi& cnt) {
	// verify : https://atcoder.jp/contests/abc149/tasks/abc149_f

	vi dummy;
	rerooting<T_ss, merge_ss, e_ss, leaf_ss, apply_ss>(g, dummy, cnt);
}


