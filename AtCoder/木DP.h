#pragma once
#include "header.h"
#include "構造(グラフ).h"
#include "FPS(mint).h"
// ■■■■■ 木 DP ■■■■■


//【貰う木 DP】O(n)
/*
* 与えられた r を根とする根付き木 g に対し，各頂点 s∈[0..n) について，
* 部分木 s に関する問題の答えを格納したリストを返す．
*
* T leaf(int s) :
*   葉 s のみからなる部分木についての答えを返す．
*
* T add_edge(T x, int p, int s) :
*   部分木 s についての暫定の答えが x のとき，
*   辺 p'→s を追加した部分木 p' についての答えを返す（記号 ' は仮の頂点を表す）
*
* void merge(T& x, T y, int s) :
*   仮の根 s' を共有する部分木 2 つに対する答えがそれぞれ x, y のとき，
*   x 側に y 側をマージして部分木 s' についての答えを x に上書きする．
*
* void add_vertex(T& x, int s) :
*	仮の根 s' をもつ部分木 s' に対する答えが x のとき，
*	根 s を追加した部分木 s についての答えを x に上書きする．
*/
template <class T, T(*leaf)(int), T(*add_edge)(const T&, int, int), void(*merge)(T&, const T&, int), void(*add_vertex)(T&, int)>
vector<T> tree_getDP_virtual(const Graph& g, int r) {
	// verify : https://atcoder.jp/contests/tdpc/tasks/tdpc_eel

	int n = sz(g);
	vector<T> dp(n);

	// 部分木 s についての答えを計算する．（p : s の親）
	function<void(int, int)> dfs = [&](int s, int p) {
		// is_leaf : s が葉か
		bool is_leaf = true;

		repe(t, g[s]) {
			if (t == p) continue;

			// 部分木 t についての答えを計算する．
			dfs(t, s);

			// 部分木 t に対して辺 s'→t を追加した場合の部分木 s' についての答えを得る．
			T sub = add_edge(dp[t], s, t);

			// それを部分木 s' の暫定の答えとマージして答えを更新していく．
			if (is_leaf) dp[s] = move(sub);
			else merge(dp[s], sub, s);

			is_leaf = false;
		}

		// s が葉の場合は専用の答えを代入しておく．
		if (is_leaf) dp[s] = leaf(s);
		// そうでない場合は根 s を追加する．
		else add_vertex(dp[s], s);
	};
	dfs(r, -1);

	return dp;

	/* 雛形
	struct T {
		int v;
	};
	T leaf(int s) {
		return T{ 1 };
	}
	T add_edge(const T& x, int p, int s) {
		return x;
	}
	void merge(T& x, const T& y, int s) {
		x.v += y.v;
	}
	void add_vertex(T& x, int s) {
		x.v += 1;
	}
	vector<T> solve_by_tree_getDP(const Graph& g, int r) {
		return tree_getDP_virtual<T, leaf, add_edge, merge, add_vertex>(g, r);
	}
	*/
};


//【貰う木 DP（重み付き）】O(n)
/*
* 与えられた r を根とする重み付き根付き木 g に対し，各頂点 s∈[0..n) について，
* 部分木 s に関する問題の答えを格納したリストを返す．
*
* T leaf(int s) :
*   葉 s のみからなる部分木についての答えを返す．
*
* T add_edge(T x, int p, int s, ll w) :
*   部分木 s についての暫定の答えが x のとき，
*   重み w の辺 p'→s を追加した部分木 p' についての答えを返す（記号 ' は仮の頂点を表す）
*
* void merge(T& x, T y, int s) :
*   仮の根 s' を共有する部分木 2 つに対する答えがそれぞれ x, y のとき，
*   x 側に y 側をマージして部分木 s' についての答えを x に上書きする．
*
* void add_vertex(T& x, int s) :
*	仮の根 s' をもつ部分木 s' に対する答えが x のとき，
*	根 s を追加した部分木 s についての答えを x に上書きする．
*/
template <class T, T(*leaf)(int), T(*add_edge)(const T&, int, int, ll), void(*merge)(T&, const T&, int), void(*add_vertex)(T&, int)>
vector<T> tree_getDP_virtual(const WGraph& g, int r) {
	// verify : https://yukicoder.me/problems/no/417

	int n = sz(g);
	vector<T> dp(n);

	// 部分木 s についての答えを計算する．（p : s の親）
	function<void(int, int)> dfs = [&](int s, int p) {
		// is_leaf : s が葉か
		bool is_leaf = true;

		repe(t, g[s]) {
			if (t == p) continue;

			// 部分木 t についての答えを計算する．
			dfs(t, s);

			// 部分木 t に対して辺 s'→t を追加した場合の部分木 s' についての答えを得る．
			T sub = add_edge(dp[t], s, t, t.cost);

			// それを部分木 s' の暫定の答えとマージして答えを更新していく．
			if (is_leaf) dp[s] = move(sub);
			else merge(dp[s], sub, s);

			is_leaf = false;
		}

		// s が葉の場合は専用の答えを代入しておく．
		if (is_leaf) dp[s] = leaf(s);
		// そうでない場合は根 s を追加する．
		else add_vertex(dp[s], s);
	};
	dfs(r, -1);

	return dp;

	/* 雛形
	struct T { ll v; };
	T leaf(int s) {
		return T{ 0 };
	}
	T add_edge(const T& x, int p, int s, ll w) {
		return T{ x.v + w };
	}
	void merge(T& x, const T& y, int s) {
		chmax(x.v, y.v);
	}
	void add_vertex(T& x, int s) {
		;
	}
	vector<T> solve_by_tree_getDP(const WGraph& g, int r) {
		return tree_getDP_virtual<T, leaf, add_edge, merge, add_vertex>(g, r);
	}
	*/
};


//【二乗の木 DP】
/*
* 各部分木 s が O(|s|) 個の状態をもつ場合，貰う木 DP を使うと全体計算量は O(n^2) になる．
* 各部分木 s が O(min(|s|, K)) 個の状態をもつ場合，貰う木 DP を使うと全体計算量は O(n K) になる．
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
* これらを直和した部分森に対応する多項式は積 f(z) g(z) である．
*
* MFPS leaf(int s) :
*   葉 s のみからなる部分木に対応する多項式を返す．
*
* pair<MFPS, MFPS> apply(int s) :
*   ある部分森に対応する多項式が f(z) で，これらに共通の根 s を追加した部分木に
*	対応する多項式が a(z) f(z) + b(z) のとき，組 {a(z), b(z)} を返す．
*
* 利用：【多項式の積の展開】,【多項式の累積積の加重和】
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


//【重軽再帰木 DP】O(poly(n) K)
/*
* 各部分木 s が O(K) 個の状態をもつ場合，貰う木 DP を使うと全体計算量は O(n K^2) になってしまう．
* でもマージを行わなくて済むようトップダウンで DP を行うことで計算量を O(poly(n) K) にできる．
* 
* 参考 : https://qiita.com/tmaehara/items/4b2735e56843bad89949
* verify : https://atcoder.jp/contests/abc311/tasks/abc311_h
*/


//【全方位木 DP】O(n)
/*
* 与えられた木 g に対し，各頂点 s∈[0..n) について，
* s を根と見たときの問題の答えを格納したリストを返す．
* また必要なら各 s∈[0..n) と s に隣接する各頂点 t（j 番目）について，
* s-t 間の辺を切断し t を根と見たときの問題の答えを sub[s][j] に格納する．
*
* T leaf(int s) :
*   葉 s のみからなる部分木について，s を根と見たときの答えを返す．
*
* T add_edge(T x, int p, int s) :
*   頂点 s を根とする部分木の暫定の答えが x のとき，
*   辺 p'→s を追加して p' を仮の根と見たときの答えを返す（記号 ' は仮の頂点を表す）
*
* T merge(T x, T y, int s) :
*   仮の根 s' のみを共有する部分木 2 つに対する答えがそれぞれ x, y のとき，
*   これらをマージした部分木について同じく s' を仮の根と見たときの答えを返す．
*
* T add_vertex(T x, int s) :
*	仮の根 s' をもつ部分木 s' に対する答えが x のとき，
*	根 s を追加した部分木 s についての答えを返す．
*/
template <class T, T(*leaf)(int), T(*add_edge)(const T&, int, int), T(*merge)(const T&, const T&, int), T(*add_vertex)(const T&, int)>
vector<T> rerooting(const Graph& g, vector<vector<T>>* sub = nullptr) {
	// 参考 : https://atcoder.jp/contests/abc222/editorial/2749
	// verify : https://atcoder.jp/contests/abc149/tasks/abc149_f

	int n = sz(g);
	vector<T> res(n);

	// sub[s][j] : 
	// 頂点 s と接続する j 番目の頂点を t としたとき，s-t 間の辺を切断し，t を根と見たときの答え
	if (sub == nullptr) sub = new vector<vector<T>>;
	sub->resize(n);
	rep(s, n) (*sub)[s] = vector<T>(sz(g[s]));

	// 大きさ 1 の木に対する例外処理
	if (n == 1) return vector<T>{ leaf(0) };

	// p-s 間の辺を切断し，s を根と見たときの答えを計算する．
	//  p : 0 を根としたときの s の親
	//  sj : s が p に接続する何番目の頂点か
	function<void(int, int, int)> dfs1 = [&](int s, int p, int sj) {
		// 頂点 0 については後で計算するので計算不要．
		if (p == -1) {
			rep(tj, sz(g[s])) dfs1(g[s][tj], s, tj);;
			return;
		}

		// is_leaf : s が葉か
		bool is_leaf = true;

		rep(tj, sz(g[s])) {
			int t = g[s][tj];
			if (t == p) continue;

			// s-t 間の辺を切断し，t を根と見たときの答えを計算する．
			dfs1(t, s, tj);

			// 先の部分木に対して辺 s'→t を追加した場合の部分木 s' についての答えを得る．
			T val = add_edge((*sub)[s][tj], s, t);

			// それを部分木 s' の暫定の答えとマージして答えを計算していく．
			if (is_leaf) (*sub)[p][sj] = move(val);
			else (*sub)[p][sj] = merge((*sub)[p][sj], val, s);

			is_leaf = false;
		}

		// s が葉の場合は専用の答えを代入しておく．
		if (is_leaf) (*sub)[p][sj] = leaf(s);
		// そうでない場合は根 s を追加する．
		else (*sub)[p][sj] = add_vertex((*sub)[p][sj], s);
	};
	dfs1(0, -1, -1);

	// s を根と見たときの答えを計算する．
	//  p : 0 を根としたときの s の親
	//  val : s-p 間の辺を切断し，p を根と見たときの答え
	function<void(int, int, const T&)> dfs2 = [&](int s, int p, const T& val) {
		// K : 根 s から出る辺の数
		int K = sz(g[s]);

		// ds[j] : 仮の根 s' から出る j 番目の辺だけを s' に接続したときの答え
		vector<T> ds(K);

		rep(tj, K) {
			const auto& t = g[s][tj];
			if (t == p) {
				(*sub)[s][tj] = val;
				ds[tj] = add_edge(val, s, p);
				continue;
			}

			// s'-t 間の辺を切断し，t を根と見たときの答えは計算し終えているので，
			// その部分木に対して辺 s'→t を接続し s' を仮の根と見た場合の答えを得る．
			ds[tj] = add_edge((*sub)[s][tj], s, t);
		}

		// acc_l[j] : 仮の根 s' の [0..j] 番目の辺を s' に接続したときの答え
		vector<T> acc_l(K);
		acc_l[0] = ds[0];
		repi(tj, 1, K - 1) acc_l[tj] = merge(acc_l[tj - 1], ds[tj], s);

		// acc_r[j] : 仮の根 s' の [j..K) 番目の辺を s' に接続したときの答え
		vector<T> acc_r(K);
		acc_r[K - 1] = ds[K - 1];
		repir(tj, K - 2, 0) acc_r[tj] = merge(acc_r[tj + 1], ds[tj], s);

		// 仮の根 s' から出る全ての辺を s' に接続し，根 s を追加したときの答えが求めるものである．
		res[s] = add_vertex(acc_l[K - 1], s);

		rep(tj, K) {
			const auto& t = g[s][tj];
			if (t == p) continue;

			// 仮の根 s' に辺 s'→t 以外の全ての辺を接続し，根 s を追加したときの答え，
			// すなわち，辺 t-s を切断し，s を根と見たときの答えを再帰関数に渡す．
			if (K == 1) dfs2(t, s, leaf(s));
			else if (tj == 0) dfs2(t, s, add_vertex(acc_r[1], s));
			else if (tj == K - 1) dfs2(t, s, add_vertex(acc_l[K - 2], s));
			else dfs2(t, s, add_vertex(merge(acc_l[tj - 1], acc_r[tj + 1], s), s));
		}
	};
	dfs2(0, -1, leaf(0)); // 第 3 引数はダミー

	return res;

	/* 雛形
	struct T {
		int v;
	};
	T leaf(int s) {
		return { 1 };
	}
	T add_edge(const T& x, int p, int s) {
		return { x.v };
	}
	T merge(const T& x, const T& y, int s) {
		return { x.v + y.v };
	}
	T add_vertex(const T& x, int s) {
		return { x.v + 1 };
	}
	vector<T> solve_by_tree_getDP(const Graph& g) {
		return rerooting<T, leaf, add_edge, merge, add_vertex>(g);
	}
	*/
};


//【全方位木 DP（重み付き）】O(n)
/*
* 与えられた重み付き木 g に対し，各頂点 s∈[0..n) について，
* s を根と見たときの問題の答えを格納したリストを返す．
* また必要なら各 s∈[0..n) と s に隣接する各頂点 t（j 番目）について，
* s-t 間の辺を切断し t を根と見たときの問題の答えを sub[s][j] に格納する．
*
* T leaf(int s) :
*   葉 s のみからなる部分木について，s を根と見たときの答えを返す．
*
* T add_edge(T x, int p, int s, ll w) :
*   頂点 s を根とする部分木の暫定の答えが x のとき，
*   重み w の辺 p'→s を追加して p' を仮の根と見たときの答えを返す（記号 ' は仮の頂点を表す）
*
* T merge(T x, T y, int s) :
*   仮の根 s' のみを共有する部分木 2 つに対する答えがそれぞれ x, y のとき，
*   これらをマージした部分木について同じく s' を仮の根と見たときの答えを返す．
*
* T add_vertex(T x, int s) :
*	仮の根 s' をもつ部分木 s' に対する答えが x のとき，
*	根 s を追加した部分木 s についての答えを返す．
*/
template <class T, T(*leaf)(int), T(*add_edge)(const T&, int, int, ll), T(*merge)(const T&, const T&, int), T(*add_vertex)(const T&, int)>
vector<T> rerooting(const WGraph& g, vector<vector<T>>* sub = nullptr) {
	// 参考 : https://atcoder.jp/contests/abc222/editorial/2749
	// verify : https://judge.yosupo.jp/problem/tree_path_composite_sum

	int n = sz(g);
	vector<T> res(n);

	// sub[s][j] : 
	// 頂点 s と接続する j 番目の頂点を t としたとき，s-t 間の辺を切断し，t を根と見たときの答え
	if (sub == nullptr) sub = new vector<vector<T>>;
	sub->resize(n);
	rep(s, n) (*sub)[s] = vector<T>(sz(g[s]));

	// 大きさ 1 の木に対する例外処理
	if (n == 1) return vector<T>{ leaf(0) };

	// p-s 間の辺を切断し，s を根と見たときの答えを計算する．
	//  p : 0 を根としたときの s の親
	//  sj : s が p に接続する何番目の頂点か
	function<void(int, int, int)> dfs1 = [&](int s, int p, int sj) {
		// 頂点 0 については後で計算するので計算不要．
		if (p == -1) {
			rep(tj, sz(g[s])) dfs1(g[s][tj], s, tj);;
			return;
		}

		// is_leaf : s が葉か
		bool is_leaf = true;

		rep(tj, sz(g[s])) {
			auto t = g[s][tj];
			if (t == p) continue;

			// s-t 間の辺を切断し，t を根と見たときの答えを計算する．
			dfs1(t, s, tj);

			// 先の部分木に対して辺 s'→t を追加した場合の部分木 s' についての答えを得る．
			T val = add_edge((*sub)[s][tj], s, t, t.cost);

			// それを部分木 s' の暫定の答えとマージして答えを計算していく．
			if (is_leaf) (*sub)[p][sj] = move(val);
			else (*sub)[p][sj] = merge((*sub)[p][sj], val, s);

			is_leaf = false;
		}

		// s が葉の場合は専用の答えを代入しておく．
		if (is_leaf) (*sub)[p][sj] = leaf(s);
		// そうでない場合は根 s を追加する．
		else (*sub)[p][sj] = add_vertex((*sub)[p][sj], s);
	};
	dfs1(0, -1, -1);

	// s を根と見たときの答えを計算する．
	//  p : 0 を根としたときの s の親
	//	w : s-p 間の辺の重み
	//  val : s-p 間の辺を切断し，p を根と見たときの答え
	function<void(int, int, ll, const T&)> dfs2 = [&](int s, int p, ll w, const T& val) {
		// K : 根 s から出る辺の数
		int K = sz(g[s]);

		// ds[j] : 仮の根 s' から出る j 番目の辺だけを s' に接続したときの答え
		vector<T> ds(K);

		rep(tj, K) {
			auto t = g[s][tj];
			if (t == p) {
				(*sub)[s][tj] = val;
				ds[tj] = add_edge(val, s, p, w);
				continue;
			}

			// s'-t 間の辺を切断し，t を根と見たときの答えは計算し終えているので，
			// その部分木に対して辺 s'→t を接続し s' を仮の根と見た場合の答えを得る．
			ds[tj] = add_edge((*sub)[s][tj], s, t, t.cost);
		}

		// acc_l[j] : 仮の根 s' の [0..j] 番目の辺を s' に接続したときの答え
		vector<T> acc_l(K);
		acc_l[0] = ds[0];
		repi(tj, 1, K - 1) acc_l[tj] = merge(acc_l[tj - 1], ds[tj], s);

		// acc_r[j] : 仮の根 s' の [j..K) 番目の辺を s' に接続したときの答え
		vector<T> acc_r(K);
		acc_r[K - 1] = ds[K - 1];
		repir(tj, K - 2, 0) acc_r[tj] = merge(acc_r[tj + 1], ds[tj], s);

		// 仮の根 s' から出る全ての辺を s' に接続し，根 s を追加したときの答えが求めるものである．
		res[s] = add_vertex(acc_l[K - 1], s);

		rep(tj, K) {
			auto t = g[s][tj];
			if (t == p) continue;

			// 仮の根 s' に辺 s'→t 以外の全ての辺を接続し，根 s を追加したときの答え，
			// すなわち，辺 t-s を切断し，s を根と見たときの答えを再帰関数に渡す．
			if (K == 1) dfs2(t, s, t.cost, leaf(s));
			else if (tj == 0) dfs2(t, s, t.cost, add_vertex(acc_r[1], s));
			else if (tj == K - 1) dfs2(t, s, t.cost, add_vertex(acc_l[K - 2], s));
			else dfs2(t, s, t.cost, add_vertex(merge(acc_l[tj - 1], acc_r[tj + 1], s), s));
		}
	};
	dfs2(0, -1, INFL, leaf(0)); // 第 3, 4 引数はダミー

	return res;

	/* 雛形
	struct T {
		ll v;
	};
	T leaf(int s) {
		return T{ 0 };
	}
	T add_edge(const T& x, int p, int s, ll w) {
		return T{ x.v + w };
	}
	T merge(const T& x, const T& y, int s) {
		return T{ x.v + y.v };
	}
	T add_vertex(const T& x, int s) {
		return T{ x.v };
	}
	vector<T> solve_by_tree_getDP(const WGraph& g) {
		return rerooting<T, leaf, add_edge, merge, add_vertex>(g);
	}
	*/
};


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
	// verify : https://atcoder.jp/contests/abc309/tasks/abc309_e

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
	T apply(const T& x, int s, int t) {
		return x + 1;
	}
	T root(int r) {
		return 0;
	}
	vector<T> solve_by_tree_giveDP(const Graph& g, int r) {
		return tree_giveDP<T, apply, root>(g, r);
	}
	*/
};


//【配る木 DP（重み付き）】O(n)
/*
* 各 s∈[0..n) について，r を根とする重み付き根付き木 g の
* 根からのパス r→s についての問題の答えを格納したリストを返す．
*
* T apply(T x, int s, int t, ll c) :
*   根からのパス r→s についての答えが x のとき，
*   コスト c の辺 s→t を追加した根からのパス r→t についての答えを返す．
*
* T root(int r) :
*   根からのパス r→r に対する問題の答えを返す．
*/
template <class T, T(*apply)(const T&, int, int, ll), T(*root)(int)>
vector<T> tree_giveDP(const WGraph& g, int r) {
	int n = sz(g);

	vector<T> dp(n);
	dp[r] = root(r);

	// s の各子 t について，根からのパス r→t についての答えを計算する．（p : s の親）
	function<void(int, int)> dfs = [&](int s, int p) {
		repe(t, g[s]) {
			if (t == p) continue;

			// 根からのパス r→t についての答えを計算する．
			dp[t] = apply(dp[s], s, t, t.cost);

			// t から先の答えを計算する．
			dfs(t, s);
		}
	};
	dfs(r, -1);

	return dp;

	/* 雛形
	using T = ll;
	T apply(const T& x, int s, int t, ll c) {
		return x + c;
	}
	T root(int r) {
		return 0;
	}
	vector<T> solve_by_tree_giveDP(const WGraph& g, int r) {
		return tree_giveDP<T, apply, root>(g, r);
	}
	*/
};


//【貰う木 DP（頂点マージ）】O(n)（互換用）
/*
* 各 s∈[0..n) について，r を根とする根付き木 g の
* 部分木 s についての問題の答えを格納したリストを返す．
*
* void merge(T& x, T y, int s) :
*   根 s のみを共有する部分木 2 つに対する答えがそれぞれ x, y のとき，
*   x 側に y 側をマージして部分木 s についての答えを上書きする．
*
* T leaf(int s) :
*   葉 s のみからなる部分木についての答えを返す．
*
* T apply(T x, int p, int s) :
*   部分木 s についての暫定の答えが x のとき，
*   辺 p→s を追加した部分木 p についての答えを返す．
*/
template <class T, void(*merge)(T&, const T&, int), T(*leaf)(int), T(*apply)(const T&, int, int)>
vector<T> tree_getDP_vmerge(const Graph& g, int r) {
	// verify : https://atcoder.jp/contests/dp/tasks/dp_p

	int n = sz(g);
	vector<T> dp(n);

	// 部分木 s についての答えを計算する．（p : s の親）
	function<void(int, int)> dfs = [&](int s, int p) {
		// is_leaf : s が葉か
		bool is_leaf = true;

		repe(t, g[s]) {
			if (t == p) continue;

			// 部分木 t についての答えを計算する．
			dfs(t, s);

			// 部分木 t に対して辺 s→t を接続した場合の部分木 s についての答えを得る．
			T sub = apply(dp[t], s, t);

			// それを部分木 s の暫定の答えとマージして答えを更新していく．
			if (is_leaf) dp[s] = move(sub);
			else merge(dp[s], sub, s);

			is_leaf = false;
		}

		// s が葉の場合は専用の答えを代入しておく．
		if (is_leaf) dp[s] = leaf(s);
	};
	dfs(r, -1);

	return dp;

	/* 雛形
	using T = int;
	void merge(T& x, const T& y, int s) {
		chmax(x, y);
	}
	T leaf(int s) {
		return 0;
	}
	T apply(const T& x, int p, int s) {
		return x + 1;
	}
	vector<T> solve_by_tree_getDP(const Graph& g, int r) {
		return tree_getDP_vmerge<T, merge, leaf, apply>(g, r);
	}
	*/
};


//【全方位木 DP】O(n)（互換用）
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
* T leaf(int s) :
*   木 g の葉 s のみからなる部分木について，s を根と見たときの答えを返す．
*
* T apply(T x, int p, int s) :
*   頂点 s を根とする部分木の暫定の答えが x のとき，
*   辺 p→s を追加して p を根と見たときの答えを返す．
*/
template <class T, T(*merge)(T, T, int), T(*leaf)(int), T(*apply)(T, int, int)>
vector<T> rerooting(const Graph& g, vector<vector<T>>* sub = nullptr) {
	// 参考 : https://atcoder.jp/contests/abc222/editorial/2749

	int n = sz(g);
	vector<T> res(n);

	// sub[s][i] : 頂点 s と接続する i 番目の頂点を t としたとき，
	//             s-t 間の辺を切断し，t を根と見たときの答え
	if (sub == nullptr) sub = new vector<vector<T>>;
	sub->resize(n);
	rep(s, n) (*sub)[s] = vector<T>(sz(g[s]));

	// 大きさ 1 の木に対する例外処理
	if (n == 1) return vector<T>{ leaf(0) };

	// p-s 間の辺を切断し，s を根と見たときの答えを計算する．
	//  p : 0 を根としたときの s の親
	//  si : s が p に接続する何番目の頂点か
	function<void(int, int, int)> dfs1 = [&](int s, int p, int si) {
		// is_leaf : s が葉か
		bool is_leaf = true;

		rep(ti, sz(g[s])) {
			auto t = g[s][ti];
			if (t == p) continue;

			// s-t 間の辺を切断し，t を根と見たときの答えを計算する．
			dfs1(t, s, ti);

			// 先の部分木に対して辺 s→t を接続した場合の答えを得る．
			T val = apply((*sub)[s][ti], s, t);

			// それを暫定の答えとマージして自身の答えを計算していく．
			if (p != -1) {
				if (is_leaf) (*sub)[p][si] = move(val);
				else (*sub)[p][si] = merge((*sub)[p][si], val, s);
			}

			is_leaf = false;
		}

		// s が葉の場合は専用の答えを代入しておく．
		if (is_leaf && p != -1) (*sub)[p][si] = leaf(s);
	};
	dfs1(0, -1, -1);

	// s を根と見たときの答えを計算する．
	//  p : 0 を根としたときの s の親
	//  val : s-p 間の辺を切断し，p を根と見たときの答え
	function<void(int, int, const T&)> dfs2 = [&](int s, int p, const T& val) {
		// K : 根 s から出る辺の数
		int K = sz(g[s]);

		// ds[i] : 根 s から出る i 番目の辺だけを s に接続したときの答え
		vector<T> ds(K);

		rep(ti, K) {
			auto t = g[s][ti];
			if (t == p) {
				(*sub)[s][ti] = val;
				ds[ti] = apply(val, s, p);
				continue;
			}

			// s-t 間の辺を切断し，t を根と見たときの答えは計算し終えているので，
			// その部分木に対して辺 s→t を接続し s を根と見た場合の答えを得る．
			ds[ti] = apply((*sub)[s][ti], s, t);
		}

		// acc_l[i] : 根 s の [0..i] 番目の辺を s に接続したときの答え
		vector<T> acc_l(K);
		acc_l[0] = ds[0];
		repi(i, 1, K - 1) acc_l[i] = merge(acc_l[i - 1], ds[i], s);

		// acc_r[i] : 根 s の [i..K) 番目の辺を s に接続したときの答え
		vector<T> acc_r(K);
		acc_r[K - 1] = ds[K - 1];
		repir(i, K - 2, 0) acc_r[i] = merge(acc_r[i + 1], ds[i], s);

		// 根 s から出る全ての辺を s に接続したときの答えが求めるものである．
		res[s] = acc_l[K - 1];

		rep(ti, K) {
			const auto& t = g[s][ti];
			if (t == p) continue;

			// 根 s に辺 s→t 以外の全ての辺を接続したときの答え，
			// すなわち，辺 t-s を切断し，s を根と見たときの答えを再帰関数に渡す．
			if (K == 1) dfs2(t, s, leaf(s));
			else if (ti == 0) dfs2(t, s, acc_r[1]);
			else if (ti == K - 1) dfs2(t, s, acc_l[K - 2]);
			else dfs2(t, s, merge(acc_l[ti - 1], acc_r[ti + 1], s));
		}
	};
	dfs2(0, -1, T()); // 後ろ 1 つの引数はダミー

	return res;

	/* 雛形
	using T = int;
	T merge(T x, T y, int s) {
		return max(x, y);
	}
	T leaf(int s) {
		return 0;
	}
	T apply(T x, int p, int s) {
		return x + 1;
	}
	vector<T> solve_by_rerooting(const Graph& g, vector<vector<T>>* sub = nullptr) {
		return rerooting<T, merge, leaf, apply>(g, sub);
	}
	*/
};


//【全方位木 DP（重み付き）】O(n)（互換用）
/*
* 与えられた重み付き木 g に対し，各 s∈[0..n) について，
* g の頂点 s を根と見たときの問題の答えを格納したリストを返す．
* また必要なら各 s∈[0..n) と s に隣接する各頂点 t（j 番目）について，
* s-t 間の辺を切断し t を根と見たときの問題の答えを sub[s][j] に格納する．
*
* T merge(T x, T y, int s) :
*   根 s のみを共有する部分木 2 つに対する答えがそれぞれ x, y のとき，
*   これらをマージした部分木について同じく s を根と見たときの答えを返す．
*
* T leaf(int s) :
*   木 g の葉 s のみからなる部分木について，s を根と見たときの答えを返す．
*
* T apply(T x, int p, int s, ll c) :
*   頂点 s を根とする部分木の暫定の答えが x のとき，
*   コストが c の辺 p→s を追加して p を根と見たときの答えを返す．
*/
template <class T, T(*merge)(T, T, int), T(*leaf)(int), T(*apply)(T, int, int, ll)>
vector<T> rerooting(const WGraph& g, vector<vector<T>>* sub = nullptr) {
	// verify : https://judge.yosupo.jp/problem/tree_path_composite_sum

	int n = sz(g);
	vector<T> res(n);

	// sub[s][i] : 頂点 s と接続する i 番目の頂点を t としたとき，
	//             s-t 間の辺を切断し，t を根と見たときの答え
	if (sub == nullptr) sub = new vector<vector<T>>;
	sub->resize(n);
	rep(s, n) (*sub)[s] = vector<T>(sz(g[s]));

	// 大きさ 1 の木に対する例外処理
	if (n == 1) return vector<T>{ leaf(0) };

	// p-s 間の辺を切断し，s を根と見たときの答えを計算する．
	//  p : 0 を根としたときの s の親
	//  si : s が p に接続する何番目の頂点か
	function<void(int, int, int)> dfs1 = [&](int s, int p, int si) {
		// is_leaf : s が葉か
		bool is_leaf = true;

		rep(ti, sz(g[s])) {
			const auto& t = g[s][ti];
			if (t == p) continue;

			// s-t 間の辺を切断し，t を根と見たときの答えを計算する．
			dfs1(t, s, ti);

			// 先の部分木に対して辺 s→t を接続した場合の答えを得る．
			T val = apply((*sub)[s][ti], s, t, t.cost);

			// それを暫定の答えとマージして自身の答えを計算していく．
			if (p != -1) {
				if (is_leaf) (*sub)[p][si] = move(val);
				else (*sub)[p][si] = merge((*sub)[p][si], val, s);
			}

			is_leaf = false;
		}

		// s が葉の場合は専用の答えを代入しておく．
		if (is_leaf && p != -1) (*sub)[p][si] = leaf(s);
	};
	dfs1(0, -1, -1);

	// s を根と見たときの答えを計算する．
	//  p : 0 を根としたときの s の親
	//	c : s-p 間の辺のコスト
	//  val : s-p 間の辺を切断し，p を根と見たときの答え
	function<void(int, int, ll, const T&)> dfs2 = [&](int s, int p, ll c, const T& val) {
		// K : 根 s から出る辺の数
		int K = sz(g[s]);

		// ds[i] : 根 s から出る i 番目の辺だけを s に接続したときの答え
		vector<T> ds(K);

		rep(ti, K) {
			const auto& t = g[s][ti];
			if (t == p) {
				(*sub)[s][ti] = val;
				ds[ti] = apply(val, s, p, c);
				continue;
			}

			// s-t 間の辺を切断し，t を根と見たときの答えは計算し終えているので，
			// その部分木に対して辺 s→t を接続し s を根と見た場合の答えを得る．
			ds[ti] = apply((*sub)[s][ti], s, t, t.cost);
		}

		// acc_l[i] : 根 s の [0..i] 番目の辺を s に接続したときの答え
		vector<T> acc_l(K);
		acc_l[0] = ds[0];
		repi(i, 1, K - 1) acc_l[i] = merge(acc_l[i - 1], ds[i], s);

		// acc_r[i] : 根 s の [i..K) 番目の辺を s に接続したときの答え
		vector<T> acc_r(K);
		acc_r[K - 1] = ds[K - 1];
		repir(i, K - 2, 0) acc_r[i] = merge(acc_r[i + 1], ds[i], s);

		// 根 s から出る全ての辺を s に接続したときの答えが求めるものである．
		res[s] = acc_l[K - 1];

		rep(ti, K) {
			const auto& t = g[s][ti];
			if (t == p) continue;

			// 根 s に辺 s→t 以外の全ての辺を接続したときの答え，
			// すなわち，辺 t-s を切断し，s を根と見たときの答えを再帰関数に渡す．
			if (K == 1) dfs2(t, s, t.cost, leaf(s));
			else if (ti == 0) dfs2(t, s, t.cost, acc_r[1]);
			else if (ti == K - 1) dfs2(t, s, t.cost, acc_l[K - 2]);
			else dfs2(t, s, t.cost, merge(acc_l[ti - 1], acc_r[ti + 1], s));
		}
	};
	dfs2(0, -1, INFL, T()); // 後ろ 2 つの引数はダミー

	return res;

	/* 雛形
	using T = int;
	T merge(T x, T y, int s) {
		return max(x, y);
	}
	T leaf(int s) {
		return 0;
	}
	T apply(T x, int p, int s, ll c) {
		return x + c;
	}
	vector<T> solve_by_rerooting(const WGraph& g, vector<vector<T>>* sub = nullptr) {
		return rerooting<T, merge, leaf, apply>(g, sub);
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


