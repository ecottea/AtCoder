#pragma once
#include "header.h"
#include "構造(グラフ).h"
#include "FPS(mint).h"
// ■■■■■ 木 DP ■■■■■


//【配る木 DP】O(n)
/*
* 各 s∈[0..n) について，r を根とする根付き木 g の
* 根からのパス r→s についての問題の答えを格納したリストを返す．
*
* T apply(T x, int s, int t) :
*   根からのパス r→s についての答えが x のとき，
*   辺 s→t を追加した根からのパス r→t についての答えを返す．
*
* T root(int r) :
*   根からのパス r→r に対する問題の答えを返す．
*/
template <class T, T(*apply)(const T&, int, int), T(*root)(int)>
vector<T> tree_giveDP(const Graph& g, int r) {
	// verify : https://algo-method.com/tasks/529

	int n = sz(g);

	vector<T> dp(n);
	dp[r] = root(r);

	// s の各子 t について，根からのパス r→t についての答えを計算する．（p : s の親）
	function<void(int, int)> dfs = [&](int s, int p) {
		repe(t, g[s]) {
			if (t == p) continue;

			// 根からのパス r→t についての答えを計算する．
			dp[t] = apply(dp[s], s, t);

			// t から先の答えを計算する．
			dfs(t, s);
		}
	};
	dfs(r, -1);

	return dp;

	/* 雛形
	using T = int;
	T apply(const T& x, int s, int t) { return x + 1; }
	T root(int r) { return 0; }
	vector<T> solve_by_tree_giveDP(const Graph& g, int r) {
		return tree_giveDP<T, apply, root>(g, r);
	}
	*/
};


//【貰う木 DP（頂点マージ）】O(n)
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
vector<T> tree_getDP_vmerge(const Graph& g, int r) {
	// verify : https://atcoder.jp/contests/dp/tasks/dp_p

	//【注意】
	// s に apply で辺 p→s を追加した結果を dp[s] に入れ直すことはできないので，
	// その分無駄なコピーが発生し定数倍が 2 倍ほど悪くなる．
	// verify : https://atcoder.jp/contests/arc029/tasks/arc029_4
	
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
		return tree_getDP_vmerge<T, merge, e, leaf, apply>(g, r);
	}
	*/
};


//【貰う木 DP（頂点マージ，コスト付き）】O(n)
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
vector<T> tree_getDP_vmerge(const WGraph& g, int r) {
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
		return tree_getDP_vmerge<T, merge, e, leaf, apply>(g, r);
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
	// verify : https://atcoder.jp/contests/dp/tasks/dp_p

	//【注意】
	// apply において辺が一度に複数本増えるので，更新に子の個数が必要なとき困る．
	// 例えば木の重さ（辺の本数）を求めるには情報不足になる．
	//
	// merge 対象には根が複数個あるので，根の状態で場合分けする遷移で困る．
	// 例えば (少なくとも 1 つは P, 全て P でない) のように状態を持つ必要がある．
	// verify : https://atcoder.jp/contests/dp/tasks/dp_p

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


//【貰う木 DP（一括）】O(n)
/*
* 各 s∈[0..n) について，r を根とする根付き木 g の
* 部分木 s についての問題の答えを格納したリストを返す．
*
* void merge(T& x, int s, T y, int t) :
*   s を根とする部分木に対する答えが x, t を根とする部分木に対する答えが y のとき，
*   辺 s→t を追加してこれらをマージした部分木についての答えを x に上書きする．
*
* T leaf(int s) :
*   葉 s のみからなる部分木についての答えを返す．
*/
template <class T, void(*merge)(T&, int s, const T&, int t), T(*leaf)(int)>
vector<T> tree_getDP_once(const Graph& g, int r) {
	// verify : https://atcoder.jp/contests/dp/tasks/dp_p

	//【注意】
	// merge が x, y について対称ではないので遷移を考える量が 2 倍になる．

	int n = sz(g);
	vector<T> dp(n);

	// 部分木 s についての答えを計算する．（p : s の親）
	function<void(int, int)> dfs = [&](int s, int p) {
		// s のみからなる部分木に対する答えで初期化する．
		dp[s] = leaf(s);

		repe(t, g[s]) {
			if (t == p) continue;

			// 部分木 t についての答えを計算する．
			dfs(t, s);

			// 辺 s→t を追加して部分木 t をマージして答えを更新する．
			merge(dp[s], s, dp[t], t);
		}
	};
	dfs(r, -1);

	return dp;

	/* 雛形
	using T = int;
	void merge(T& x, int s, const T& y, int t) { chmax(x, y + 1); }
	T leaf(int s) { return 0; }
	vector<T> solve_by_tree_getDP_once(const Graph& g, int r) {
		return tree_getDP_once<T, merge, leaf>(g, r);
	}
	*/
};


//【二乗の木 DP】
/*
* 各部分木 s が O(|s|) 個の状態をもつ場合，貰う木 DP を使うと全体計算量は O(n^2) になる．
* 各部分木 s が O(min(|s|, k)) 個の状態をもつ場合，貰う木 DP を使うと全体計算量は O(n k) になる．
* 
* 参考 : https://snuke.hatenablog.com/entry/2019/01/15/211812
* verify : https://atcoder.jp/contests/tdpc/tasks/tdpc_eel
*/


//【貰う木 DP（森経由，多項式，mod 998244353）】O(n (log n)^3)
/*
* 与えられた r を根とする根付き木に対し，r に対応する多項式を返す．
*
* 制約 :
* ある部分森に対応する多項式が f(z), ある部分木に対応する多項式が g(z) のとき，
* これらをマージした部分森に対応する多項式は積 f(z) g(z) である．
*
* MFPS leaf(int s) :
*   葉 s のみからなる部分木に対応する多項式を返す．
*
* pair<MFPS, MFPS> apply(int s) :
*   ある部分森に対応する多項式が f(z) で，これらに共通の根 s を追加した部分木に
*	対応する多項式が a(z) f(z) + b(z) のとき，組 {a(z), b(z)} を返す．
*
* 利用：【形式的冪級数（mod 998244353）】,【多項式の積の展開】,【多項式の累積積の和】
*/
template <MFPS(*leaf)(int), pair<MFPS, MFPS>(*apply)(int)>
MFPS tree_getDP_forest_MFPS(Graph g, int r) {
	// 参考 : https://atcoder.jp/contests/abc269/editorial/4838
	// verify : https://atcoder.jp/contests/abc269/tasks/abc269_h

	int n = sz(g);

	// 貰う木 DP で各部分木の重さを求め，重さ最大の頂点を最後になぞるよう順番を入れ替える．
	// ついでに親へ戻る辺を削除し有向木にする．
	function<int(int, int)> dfs_w = [&](int s, int p) {
		int ws = 0, w_max = -INF, tj_max = -1, tj_par = -1;
		rep(tj, sz(g[s])) {
			auto t = g[s][tj];
			if (t == p) {
				tj_par = tj;
				continue;
			}

			int wt = dfs_w(t, s);
			ws += wt + 1;
			if (chmax(w_max, wt)) tj_max = tj;
		}

		// 親へ戻る辺を削除する．
		if (tj_par != -1) {
			swap(g[s][tj_par], g[s].back());
			if (tj_max == sz(g[s]) - 1) tj_max = tj_par;
			g[s].pop_back();
		}

		// 重さ最大の頂点を最後になぞるよう順番を入れ替える．
		if (tj_max != -1) swap(g[s][tj_max], g[s].back());

		return ws;
	};
	dfs_w(r, -1);

	function<MFPS(int)> dfs_root;
	function<void(int, vector<MFPS>&, vector<MFPS>&)> dfs_path;

	// heavy path の根である s に対応する多項式を返す．
	dfs_root = [&](int s) {
		// s が葉の場合は専用の答えを返す．
		if (g[s].empty()) return leaf(s);

		// fh : s を根とする heavy path 上の頂点に対応する多項式を浅い順に並べたもの
		// coef : fh の累積積に掛かる係数を浅い順に並べたもの
		vector<MFPS> fh, coef;

		// light child に対応する多項式の積を計算する．
		auto [a, b] = apply(s);
		vector<MFPS> fl{ a };
		rep(tj, sz(g[s]) - 1) {
			auto t = g[s][tj];
			fl.emplace_back(dfs_root(t));
		}
		fh.emplace_back(expand(fl));
		coef.emplace_back(b);

		// heavy path 上の頂点に対応する多項式を fh, coef に格納する．
		dfs_path(g[s].back(), fh, coef);

		// 分割統治法を用いて heavy path 上の多項式をまとめる計算を一括で行う．
		return weighted_cumulative_product_sum(coef, fh);
	};

	// heavy path の根でない s に対応する多項式を格納する．
	//	fh : s が属する heavy path 上の頂点に対応する多項式を浅い順に並べたもの
	//	coef : fh の累積積に掛かる係数を浅い順に並べたもの
	dfs_path = [&](int s, vector<MFPS>& fh, vector<MFPS>& coef) {
		// s が葉の場合は専用の答えを格納する．
		if (g[s].empty()) {
			coef.emplace_back(leaf(s));
			return;
		}

		// light child に対応する多項式の積を計算する．
		auto [a, b] = apply(s);
		vector<MFPS> fl{ a };
		rep(tj, sz(g[s]) - 1) {
			auto t = g[s][tj];
			fl.emplace_back(dfs_root(t));
		}
		fh.emplace_back(expand(fl));
		coef.emplace_back(b);

		// heavy path 上の頂点に対応する多項式を fh, coef に格納する．
		dfs_path(g[s].back(), fh, coef);
	};

	return dfs_root(r);
};


//【全方位木 DP】O(n)
/*
* 与えられた木 g に対し，各 s∈[0..n) について，
* g の頂点 s を根と見たときの問題の答えを格納したリストを返す．
* また必要なら各 s∈[0..n) と s に隣接する各頂点 t（j 番目）について，
* s-t 間の辺を切断し t を根と見たときの問題の答えを sub[s][j] に格納する．
*
* T merge(T x, T y, int s) :
*   根 s のみを共有する部分木 2 つに対する答えがそれぞれ x, y のとき，
*   これらをマージした部分木について同じく s を根と見たときの答えを返す．
*
* T e() :
*   merge() の単位元を返す．
*
* T leaf(int s) :
*   単独のノード s のみからなる部分木について，s を根と見たときの答えを返す．
*
* T apply(T x, int p, int s) :
*   頂点 s を根とする部分木の暫定の答えが x のとき，
*   辺 p→s を追加して p を根と見たときの答えを返す．
*/
template <class T, T(*merge)(T, T, int), T(*e)(), T(*leaf)(int), T(*apply)(T, int, int)>
vector<T> rerooting(const Graph& g, vector<vector<T>>* sub = nullptr) {
	// verify : https://atcoder.jp/contests/abc149/tasks/abc149_f

	int n = sz(g);
	vector<T> res(n);

	// sub[s][i] : 頂点 s と接続する i 番目の頂点を t としたとき，
	//             s-t 間の辺を切断し，t を根と見たときの答え
	if (sub == nullptr) sub = new vector<vector<T>>;
	sub->resize(n);
	rep(s, n) (*sub)[s] = vector<T>(sz(g[s]), e());

	// p-s 間の辺を切断し，s を根と見たときの答えを計算する．
	//  p : 0 を根としたとき s の親
	//  si : s が p に接続する何番目の頂点か
	function<void(int, int, int)> dfs1 = [&](int s, int p, int si) {
		// is_leef : s が葉か
		bool is_leef = true;

		rep(ti, sz(g[s])) {
			int t = g[s][ti];
			if (t == p) continue;
			is_leef = false;

			// s-t 間の辺を切断し，t を根と見たときの答えを計算する．
			dfs1(t, s, ti);

			// 先の部分木に対して辺 s→t を接続した場合の答えを得て，
			// それを暫定の答えとマージして自身の答えを計算していく．
			if (p != -1) (*sub)[p][si] = merge((*sub)[p][si], apply((*sub)[s][ti], s, t), s);
		}

		// s が葉の場合は専用の答えを代入しておく．
		if (is_leef && p != -1) (*sub)[p][si] = leaf(s);
	};
	dfs1(0, -1, -1);

	// s を根と見たときの答えを計算する．
	//  p : 0 を根としたときの s の親
	//  val : s-p 間の辺を切断し，p を根と見たときの答え
	function<void(int, int, const T&)> dfs2 = [&](int s, int p, const T& val) {
		// ds : 根 s から出る各辺について，その辺だけを s に接続したときの答えのリスト
		vector<T> ds{ p != -1 ? apply(val, s, p) : e() };

		rep(ti, sz(g[s])) {
			int t = g[s][ti];
			if (t == p) {
				(*sub)[s][ti] = val;
				continue;
			}

			// s-t 間の辺を切断し，t を根と見たときの答えは計算し終えているので，
			// その部分木に対して辺 s→t を接続し s を根と見た場合の答えを得る．
			ds.push_back(apply((*sub)[s][ti], s, t));
		}
		int k = sz(ds);

		// acc_l[acc_r] : 根 s の左[右] からの辺を順に s に接続したときの答えのリスト
		vector<T> acc_l(k + 1, e()), acc_r(k + 1, e());

		rep(i, k) acc_l[i + 1] = merge(acc_l[i], ds[i], s);
		repir(i, k - 1, 0) acc_r[i] = merge(acc_r[i + 1], ds[i], s);

		// 根 s から出る全ての辺を s に接続したときの答えが求めるものである．
		res[s] = acc_l[k];

		int i = 1;
		rep(ti, sz(g[s])) {
			int t = g[s][ti];
			if (t == p) continue;

			// 根 s に辺 s→t 以外の全ての辺を接続したときの答え，
			// すなわち，辺 t-s を切断し，s を根と見たときの答えを再帰関数に渡す．
			dfs2(t, s, merge(acc_l[i], acc_r[i + 1], s));

			i++;
		}
	};
	dfs2(0, -1, e());

	return res;

	/* 雛形
	using T = int;
	T merge(T x, T y, int s) { return max(x, y); }
	T e() { return 0; }
	T leaf(int s) { return 0; }
	T apply(T x, int p, int s) { return x + 1; }
	vector<T> solve_by_rerooting(Graph& g, vector<vector<T>>* sub = nullptr) {
		return rerooting<T, merge, e, leaf, apply>(g, sub);
	}
	*/
};


//【全方位木 DP（コスト付き）】O(n)
/*
* 与えられたコスト付き木 g に対し，各 s∈[0..n) について，
* g の頂点 s を根と見たときの問題の答えを格納したリストを返す．*
* また必要なら各 s∈[0..n) と s に隣接する各頂点 t（j 番目）について，
* s-t 間の辺を切断し t を根と見たときの問題の答えを sub[s][j] に格納する．
*
* T merge(T x, T y, int s) :
*   根 s のみを共有する部分木 2 つに対する答えがそれぞれ x, y のとき，
*   これらをマージした部分木について同じく s を根と見たときの答えを返す．
*
* T e() :
*   merge() の単位元を返す．
*
* T leaf(int s) :
*   単独のノード s のみからなる部分木について，s を根と見たときの答えを返す．
*
* T apply(T x, int p, int s, ll c) :
*   頂点 s を根とする部分木の暫定の答えが x のとき，
*   コストが c の辺 p→s を追加して p を根と見たときの答えを返す．
*/
template <class T, T(*merge)(T, T, int), T(*e)(), T(*leaf)(int), T(*apply)(T, int, int, ll)>
vector<T> rerooting(const WGraph& g, vector<vector<T>>* sub = nullptr) {
	// verify : https://atcoder.jp/contests/abc149/tasks/abc149_f

	int n = sz(g);
	vector<T> res(n);

	// sub[s][i] : 頂点 s と接続する i 番目の頂点を t としたとき，
	//             s-t 間の辺を切断し，t を根と見たときの答え
	if (sub == nullptr) sub = new vector<vector<T>>;
	sub->resize(n);
	rep(s, n) (*sub)[s] = vector<T>(sz(g[s]), e());

	// p-s 間の辺を切断し，s を根と見たときの答えを計算する．
	//  p : 0 を根としたとき s の親
	//  si : s が p に接続する何番目の頂点か
	function<void(int, int, int)> dfs1 = [&](int s, int p, int si) {
		// is_leef : s が葉か
		bool is_leef = true;

		rep(ti, sz(g[s])) {
			const auto& t = g[s][ti];
			if (t == p) continue;
			is_leef = false;

			// s-t 間の辺を切断し，t を根と見たときの答えを計算する．
			dfs1(t, s, ti);

			// 先の部分木に対して辺 s→t を接続した場合の答えを得て，
			// それを暫定の答えとマージして自身の答えを計算していく．
			if (p != -1) (*sub)[p][si] = merge((*sub)[p][si], apply((*sub)[s][ti], s, t, t.cost), s);
		}

		// s が葉の場合は専用の答えを代入しておく．
		if (is_leef && p != -1) (*sub)[p][si] = leaf(s);
	};
	dfs1(0, -1, -1);

	// s を根と見たときの答えを計算する．
	//  p : 0 を根としたときの s の親
	//	c : s-p 間の辺のコスト
	//  val : s-p 間の辺を切断し，p を根と見たときの答え
	function<void(int, int, ll, const T&)> dfs2 = [&](int s, int p, ll c, const T& val) {
		// ds : 根 s から出る各辺について，その辺だけを s に接続したときの答えのリスト
		vector<T> ds{ p != -1 ? apply(val, s, p, c) : e() };

		rep(ti, sz(g[s])) {
			const auto& t = g[s][ti];
			if (t == p) {
				(*sub)[s][ti] = val;
				continue;
			}

			// s-t 間の辺を切断し，t を根と見たときの答えは計算し終えているので，
			// その部分木に対して辺 s→t を接続し s を根と見た場合の答えを得る．
			ds.push_back(apply((*sub)[s][ti], s, t, t.cost));
		}
		int k = sz(ds);

		// acc_l[acc_r] : 根 s の左[右] からの辺を順に s に接続したときの答えのリスト
		vector<T> acc_l(k + 1, e()), acc_r(k + 1, e());

		rep(i, k) acc_l[i + 1] = merge(acc_l[i], ds[i], s);
		repir(i, k - 1, 0) acc_r[i] = merge(acc_r[i + 1], ds[i], s);

		// 根 s から出る全ての辺を s に接続したときの答えが求めるものである．
		res[s] = acc_l[k];

		int i = 1;
		rep(ti, sz(g[s])) {
			const auto& t = g[s][ti];
			if (t == p) continue;

			// 根 s に辺 s→t 以外の全ての辺を接続したときの答え，
			// すなわち，辺 t-s を切断し，s を根と見たときの答えを再帰関数に渡す．
			dfs2(t, s, t.cost, merge(acc_l[i], acc_r[i + 1], s));

			i++;
		}
	};
	dfs2(0, -1, INFL, e());

	return res;

	/* 雛形
	using T = int;
	T merge(T x, T y, int s) { return max(x, y); }
	T e() { return 0; }
	T leaf(int s) { return 0; }
	T apply(T x, int p, int s, ll c) { return x + 1; }
	vector<T> solve_by_rerooting(WGraph& g, vector<vector<T>>* sub = nullptr) {
		return rerooting<T, merge, e, leaf, apply>(g, sub);
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


