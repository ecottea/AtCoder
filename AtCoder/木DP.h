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
vector<T> tree_getDP(const Graph& g, int r) {
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
	#ifdef _MSC_VER
		friend ostream& operator<<(ostream& os, const T& x) {
			os << '(' << x.v << ')';
			return os;
		}
	#endif
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
	auto dp = tree_getDP<T, leaf, add_edge, merge, add_vertex>(g, r);
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
vector<T> tree_getDP(const WGraph& g, int r) {
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
	struct T {
		ll v;
	#ifdef _MSC_VER
		friend ostream& operator<<(ostream& os, const T& x) {
			os << '(' << x.v << ')';
			return os;
		}
	#endif
	};
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
	auto dp = tree_getDP<T, leaf, add_edge, merge, add_vertex>(g, r);
	*/
};


//【二乗の木 DP】
/*
* 部分木 s, t のマージに |s| |t| の計算量がかかるとする．
* 各部分木 s が O(|s|) 個の状態をもつ場合，貰う木 DP を使うと全体計算量は O(n^2) になる．
* 各部分木 s が O(min(|s|, K)) 個の状態をもつ場合，貰う木 DP を使うと全体計算量は O(n K) になる．
* 
* 参考 : https://snuke.hatenablog.com/entry/2019/01/15/211812
* verify : https://atcoder.jp/contests/tdpc/tasks/tdpc_eel
*/


//【貰う木 DP（多項式，mod 998244353）】O(n (log n)^3)
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
MFPS tree_getDP_MFPS(Graph g, int r) {
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
	#ifdef _MSC_VER
		friend ostream& operator<<(ostream& os, const T& x) {
			os << '(' << x.v << ')';
			return os;
		}
	#endif
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
	auto dp = rerooting<T, leaf, add_edge, merge, add_vertex>(g);
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
	#ifdef _MSC_VER
		friend ostream& operator<<(ostream& os, const T& x) {
			os << '(' << x.v << ')';
			return os;
		}
	#endif
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
	auto dp = rerooting<T, leaf, add_edge, merge, add_vertex>(g);
	*/
};


//【static top tree】
/*
* Static_top_tree<F, get_fnc, rake, comp>(Graph g, int r) : O(n (log n)^2)
*	r を根とする根付き木 g で初期化する．
*
* set(int s) : O((log n)^2)
*	頂点 s の情報の更新を反映する．
*
* F get() : O(1)
*	根付き木全体に対応する関数を返す．
*
* なおテンプレート引数が表す関数は以下の通りとする：
*
* F get_fnc(int s) :
*   生きた葉 s とその仮の親 p'（開頂点）のみからなる開部分木に対応する関数 f を返す．
*	開部分木 t に対応する関数が g であるとき，t を s に結合した開部分木の関数が f o g になるようにする．
*
* F rake(F f, F g) :
*   開部分木 s, t に対応する関数が f, g のとき，開部分木の開根を重ねた開部分木に対する関数を返す．
*	その際 g の生きた葉は死に関数としての能力を失うので 2 変数関数になるわけではない．
*
* F comp(F f, F g) :
*	合成関数 f o g を返す．
*/
template <class F, F(*get_fnc)(int), F(*rake)(const F&, const F&), F(*comp)(const F&, const F&)>
class Static_top_tree {
	// 参考 : https://atcoder.jp/contests/abc351/editorial/9868

	struct Node {
		// tp : ノードのタイプ（R:rake, C:comp, f:leaf）
		char tp = '?';

		// id : heavy path ならその根，light child ならその長男
		int id = -1;

		// pp : 親ノードへのポインタ，lp[rp] : 左[右]の子ノードへのポインタ
		Node* pp = nullptr, * lp = nullptr, * rp = nullptr;

		// f : 関数
		F f;

		//// 参考 : https://qiita.com/tubo28/items/f058582e457f6870a800
		//static inline int node_count = 0;
		//// 静的に確保した配列から返す
		//void* operator new(std::size_t) {
		//	constexpr int MAX_N = (int)2e5 * 2 + 10; // 2 倍居る
		//	static Node pool[MAX_N];
		//	return pool + node_count++;
		//}
	};

	// root : 根（根付き木全体に対応する）
	Node* root;

	// st[s] : 頂点 s の変更があったとき，どのノードから更新を始めればいいか
	vector<Node*> st;

public:
	// rt を根とする根付き木 g で初期化する．
	Static_top_tree(const Graph& g, int rt) {
		// verify : https://judge.yosupo.jp/problem/point_set_tree_path_composite_sum_fixed_root

		int n = sz(g);

		// j_max[s] : s の重さ最大の部分木が何番目か
		vi j_max(n, -1);

		// 部分木の重さを調べる．
		function<int(int, int)> dfs_wgt = [&](int s, int p) {
			int ws = 0; int wt_max = -INF;

			rep(j, sz(g[s])) {
				auto t = g[s][j];
				if (t == p) continue;

				int wt = dfs_wgt(t, s);
				ws += wt + 1;
				if (chmax(wt_max, wt)) j_max[s] = j;
			}
			return ws;
		};
		dfs_wgt(rt, -1);

		// hp[s] : 根を s とする heavy path を成す頂点の列（深さ降順）
		vvi hp(n);

		// lc[s] : 頂点 s の兄弟である light child のリスト（自身を lc[s][0] に含む）
		vvi lc(n);

		// HL 分解を行う．
		function<void(int, int, int)> dfs_hld = [&](int s, int p, int r) {
			hp[r].push_back(s);

			int ht = -1;

			if (j_max[s] != -1) {
				ht = g[s][j_max[s]];
				lc[ht].push_back(ht);

				dfs_hld(ht, s, r);
			}

			rep(j, sz(g[s])) {
				int t = g[s][j];
				if (t == p || j == j_max[s]) continue;

				lc[ht].push_back(t);

				dfs_hld(t, s, t);
			}
		};
		dfs_hld(rt, -1, rt);

		root = new Node{ 'C', rt };
		st.resize(n);

		// トップダウンに二分木を構築する．
		// [l..r) : heavy path, light child 共にどの範囲を見ているか
		function<void(Node*, int, int)> dfs_btree = [&](Node* p, int l, int r) {
			// 仮ラベルが 'C'
			if (p->tp == 'C') {
				// ラベルが 'C'
				if (r - l >= 2) {
					int m = (l + r) / 2;

					p->lp = new Node{ 'C', p->id, p };
					dfs_btree(p->lp, l, m);

					p->rp = new Node{ 'C', p->id, p };
					dfs_btree(p->rp, m, r);

					p->f = comp(p->lp->f, p->rp->f);
				}
				// ラベルが 'R' か 'f'
				else {
					p->id = hp[p->id][l]; // light child の長男の頂点番号
					l = 0;
					r = sz(lc[p->id]);

					if (r <= 1) {
						p->tp = 'f';
						p->f = get_fnc(p->id);
						st[p->id] = p;
					}
					else {
						p->tp = 'R';
						dfs_btree(p, l, r);
					}
				}
			}
			// 仮ラベルが 'R'
			else {
				// ラベルが 'R'
				if (r - l >= 2) {
					int m = (l + r) / 2;

					p->lp = new Node{ 'R', p->id, p };
					dfs_btree(p->lp, l, m);

					p->rp = new Node{ 'R', p->id, p };
					dfs_btree(p->rp, m, r);

					p->f = rake(p->lp->f, p->rp->f);
				}
				// ラベルが 'C' か 'f'
				else {
					p->id = lc[p->id][l]; // heavy path の根の頂点番号
					l = 0;
					r = sz(hp[p->id]);

					if (r <= 1) {
						p->tp = 'f';
						p->f = get_fnc(p->id);
						st[p->id] = p;
					}
					else {
						p->tp = 'C';
						dfs_btree(p, l, r);
					}
				}
			}
		};
		dfs_btree(root, 0, sz(hp[rt]));
	}
	Static_top_tree() {}

	// 頂点 s の情報の更新を反映する．
	void set(int v) {
		// verify : https://judge.yosupo.jp/problem/point_set_tree_path_composite_sum_fixed_root

		Node* p = st[v];

		// ボトムアップに必要な箇所のみ更新する．
		while (p) {
			if (p->tp == 'R') {
				p->f = rake(p->lp->f, p->rp->f);
			}
			else if (p->tp == 'C') {
				p->f = comp(p->lp->f, p->rp->f);
			}
			else if (p->tp == 'f') {
				p->f = get_fnc(p->id);
			}

			p = p->pp;
		}
	}

	// 根付き木全体の関数を返す．
	F get() {
		// verify : https://judge.yosupo.jp/problem/point_set_tree_path_composite_sum_fixed_root

		return root->f;
	}

	/* 雛形
	struct F {
	mint a, b;
	};
	F get_fnc(int i) {
		return F{ 1, a[i] };
	}
	F rake(const F& f, const F& g) {
		return F{ f.a * g.b, f.b * g.b };
	}
	F comp(const F& f, const F& g) {
		return F{ f.a * g.a, f.a * g.b + f.b };
	}
	Static_top_tree<F, get_fnc, rake, comp> G(g, 0);
	*/
}; 


//【static top tree（二属性）】
/*
* Static_top_tree_2atr<S, F, get_val, get_fnc, merge, add_vtx, comp, act>(Graph g, int r) : O(n (log n)^2)
*	r を根とする根付き木 g で初期化する．
*
* set(int s) : O((log n)^2)
*	頂点 s の情報の更新を反映する．
*
* S get() : O(1)
*	根付き木全体の値を返す．
*
* なおテンプレート引数が表す関数は以下の通りとする：
*
* S get_val(int s) :
*   葉 s とその開親 p' のみからなる開部分木 p' についての値を返す．
*
* F get_fnc(int s) :
*   節点 s とその開親 p' のみからなる欠損開部分木（子が 1 つ欠けた開部分木）について，
*	関数 : (欠けた開部分木の値 → 開部分木 p' の値) を返す．
*
* S merge(S x, S y) :
*   開根を共有する開部分木 2 つの値がそれぞれ x, y のとき，それらをマージした開部分木の値を返す．
*
* F add_vtx(S x, int s) :
*	開根 s' をもつ開部分木 s' の値が x のとき，根 s とその開親 p' を追加した欠損開部分木 p' についての
*	関数 : (欠けた開部分木の値 → 開部分木 p' の値) を返す．
*
* F comp(F f, F g) :
*	合成関数 f o g を返す．
*
* S act(F f, S x) :
*	f(x) を返す．
*/
template <class S, class F, S(*get_val)(int), F(*get_fnc)(int), S(*merge)(const S&, const S&), F(*add_vtx)(const S&, int), F(*comp)(const F&, const F&), S(*act)(const F&, const S&)>
class Static_top_tree_2atr {
	struct Node {
		// tp : ノードのタイプ
		//	A:act, C:comp, V:add_vtx, M:merge, f:get_fnc, x:get_val
		char tp = '?';

		// id : heavy path ならその根，light child ならその親
		//	ただし二分木の構築後は不要になるので，使い回して参照すべき頂点番号を表す．
		int id = -1;

		// [l..r) : heavy path, light child 共にどの範囲を見ているか
		int l = -1, r = -1;

		// pp : 親ノードへのポインタ，lp[rp] : 左[右]の子ノードへのポインタ
		//	ただし子が 1 つの場合は lp のみを使用する．
		Node* pp = nullptr, * lp = nullptr, * rp = nullptr;

		// f : 関数
		F f;

		// x : 値
		S x;
	};

	// root : 根（根付き木全体に対応する）
	Node* root;

	// st[s] : 頂点 s の変更があったとき，どのノードから更新を始めればいいか
	vector<Node*> st;

public:
	// r を根とする根付き木 g で初期化する．
	Static_top_tree_2atr(const Graph& g, int rt) {
		int n = sz(g);

		// j_max[s] : s の重さ最大の部分木が何番目か
		vi j_max(n, -1);

		// 部分木の重さを調べる．
		function<int(int, int)> dfs_wgt = [&](int s, int p) {
			int ws = 0; int wt_max = -INF;

			rep(j, sz(g[s])) {
				auto t = g[s][j];
				if (t == p) continue;

				int wt = dfs_wgt(t, s);
				ws += wt + 1;
				if (chmax(wt_max, wt)) j_max[s] = j;
			}
			return ws;
		};
		dfs_wgt(rt, -1);

		// hp[s] : 根を s とする heavy path を成す頂点の列（深さ降順）
		vvi hp(n);

		// lc[s] : 頂点 s の light child のリスト
		vvi lc(n);

		// HL 分解を行う．
		function<void(int, int, int)> dfs_hld = [&](int s, int p, int r) {
			hp[r].push_back(s);

			if (j_max[s] != -1) {
				int t = g[s][j_max[s]];
				dfs_hld(t, s, r);
			}

			rep(j, sz(g[s])) {
				int t = g[s][j];
				if (t == p || j == j_max[s]) continue;

				lc[s].push_back(t);

				dfs_hld(t, s, t);
			}
		};
		dfs_hld(rt, -1, rt);

		root = new Node{ 'A', rt, 0, sz(hp[0]) };
		st.resize(n);

		// トップダウンに二分木を構築する．
		function<void(Node*)> dfs_btree = [&](Node* p) {
			if (p->tp == 'A' || p->tp == 'C') {
				if (p->r - p->l > 1) {
					int m = (p->l + p->r) / 2;

					p->lp = new Node{ 'C', p->id, p->l, m, p };
					dfs_btree(p->lp);

					p->rp = new Node{ p->tp, p->id, m, p->r, p };
					dfs_btree(p->rp);

					if (p->tp == 'A') p->x = act(p->lp->f, p->rp->x);
					else p->f = comp(p->lp->f, p->rp->f);
				}
				else {
					p->id = hp[p->id][p->l]; // 使い回して頂点番号を入れておく
					st[p->id] = p;
					int r = sz(lc[p->id]);
					if (r > 0) {
						p->tp = 'V';
						p->lp = new Node{ 'M', p->id, 0, r, p };
						dfs_btree(p->lp);

						p->f = add_vtx(p->lp->x, p->id);
					}
					else {
						if (p->tp == 'A') {
							p->tp = 'x';
							p->x = get_val(p->id);
						}
						else {
							p->tp = 'f';
							p->f = get_fnc(p->id);
						}
					}
				}
			}
			else if (p->tp == 'M') {
				if (p->r - p->l > 1) {
					int m = (p->l + p->r) / 2;

					p->lp = new Node{ 'M', p->id, p->l, m, p };
					dfs_btree(p->lp);

					p->rp = new Node{ 'M', p->id, m, p->r, p };
					dfs_btree(p->rp);

					p->x = merge(p->lp->x, p->rp->x);
				}
				else {
					p->tp = 'A';
					p->id = lc[p->id][p->l];
					p->l = 0;
					p->r = sz(hp[p->id]);
					dfs_btree(p);
				}
			}
		};
		dfs_btree(root);
	}

	// 頂点 s の情報の更新を反映する．
	void set(int v) {
		Node* p = st[v];

		// ボトムアップに必要な箇所のみ更新する．
		while (p) {
			if (p->tp == 'A') {
				p->x = act(p->lp->f, p->rp->x);
			}
			else if (p->tp == 'C') {
				p->f = comp(p->lp->f, p->rp->f);
			}
			else if (p->tp == 'V') {
				p->f = add_vtx(p->lp->x, p->id);
			}
			else if (p->tp == 'M') {
				p->x = merge(p->lp->x, p->rp->x);
			}
			else if (p->tp == 'x') {
				p->x = get_val(v);
			}
			else if (p->tp == 'f') {
				p->f = get_fnc(v);
			}

			p = p->pp;
		}
	}

	// 根付き木全体の値を返す．
	S get() {
		return root->x;
	}

	/* 雛形
	using S = mint;
	struct F {
		mint a, b;
	};
	S get_val(int v) {
		return a[v];
	}
	F get_fnc(int v) {
		return { 1, a[v] };
	}
	S merge(const S& x, const S& y) {
		return x * y;
	}
	F add_vtx(const S& x, int v) {
		return { x, a[v] };
	}
	F comp(const F& f, const F& g) {
		return { f.a * g.a, f.a * g.b + f.b };
	}
	S act(const F& f, const S& x) {
		return f.a * x + f.b;
	}
	Static_top_tree_2atr<S, F, get_val, get_fnc, merge, add_vtx, comp, act> G(g, 0);
	*/
};


//【static top tree（全方位）】
/*
* Static_top_tree_rerooting<H, L, H2, L2, ...略>(Graph g) : O(n (log n)^2)
*	木 g で初期化する．
*
* update(int s) : O((log n)^2)
*	頂点 s の情報の更新を反映する．
*
* H2 get(int r) : O((log n)^2)
*	r を根とした根付き木全体に対応する 2 変数関数を返す．
*
* テンプレート引数が表す関数の意味の説明は多すぎるのでマニュアルへ．
*/
template <class H, class L, class H2, class L2, H(*get_H)(int), H(*L_to_H)(const L&, int), L(*H_to_L)(const H&), H2(*H_to_H2)(const H&), H2(*L2_to_H2)(const L2&, int), L2(*H2_to_L2)(const H2&), H(*comp_H)(const H&, const H&), L(*comp_L)(const L&, const L&), H2(*comp_H2u)(const H2&, const H&), H2(*comp_H2d)(const H2&, const H&), L2(*comp_L2)(const L2&, const L&)>
class Static_top_tree_rerooting {
	struct Node {
		// tp : ノードのタイプ
		//	H: heavy path, L: light child
		char tp = '?';

		// id : heavy path ならその根，light child ならその親
		//	ただし二分木の構築後は不要になるので，使い回して参照すべき頂点番号を表す．
		int id = -1;

		// [l..r) : heavy path, light child 共にどの範囲を見ているか
		int l = -1, r = -1;

		// pp : 親ノードへのポインタ，lp[rp] : 左[右]の子ノードへのポインタ
		//	ただし子が 1 つの場合は lp のみを使用する．
		Node* pp = nullptr, * lp = nullptr, * rp = nullptr;

		// f : H 上の 1 変数関数
		H f, fR;

		// g : L 上の 1 変数関数
		L g;
	};

	// root : static top tree の根（根付き木全体に対応する）
	Node* root;

	// st[s] : 頂点 s の変更があったとき，どのノードから更新を始めればいいか
	vector<Node*> st;

public:
	// 木 g で初期化する．
	Static_top_tree_rerooting(const Graph& g) {
		// verify : https://judge.yosupo.jp/problem/point_set_tree_path_composite_sum

		int n = sz(g);

		// j_max[s] : s の重さ最大の部分木が何番目か
		vi j_max(n, -1);

		// 部分木の重さを調べる．
		function<int(int, int)> dfs_wgt = [&](int s, int p) {
			int ws = 0; int wt_max = -INF;

			rep(j, sz(g[s])) {
				auto t = g[s][j];
				if (t == p) continue;

				int wt = dfs_wgt(t, s);
				ws += wt + 1;
				if (chmax(wt_max, wt)) j_max[s] = j;
			}
			return ws;
		};
		dfs_wgt(0, -1);

		// hp[s] : 根を s とする heavy path を成す頂点の列（深さ降順）
		vvi hp(n);

		// lc[s] : 頂点 s の light child のリスト
		vvi lc(n);

		// HL 分解を行う．
		function<void(int, int, int)> dfs_hld = [&](int s, int p, int r) {
			hp[r].push_back(s);

			if (j_max[s] != -1) {
				int t = g[s][j_max[s]];
				dfs_hld(t, s, r);
			}

			rep(j, sz(g[s])) {
				int t = g[s][j];
				if (t == p || j == j_max[s]) continue;

				lc[s].push_back(t);

				dfs_hld(t, s, t);
			}
		};
		dfs_hld(0, -1, 0);

		root = new Node{ 'H', 0, 0, sz(hp[0]) };
		st.resize(n);

		// トップダウンに二分木を構築する．
		function<void(Node*)> dfs_btree = [&](Node* p) {
			if (p->tp == 'H') {
				if (p->r - p->l > 1) {
					int m = (p->l + p->r) / 2;

					p->lp = new Node{ 'H', p->id, p->l, m, p };
					dfs_btree(p->lp);

					p->rp = new Node{ 'H', p->id, m, p->r, p };
					dfs_btree(p->rp);

					p->f = comp_H(p->lp->f, p->rp->f);
					p->fR = comp_H(p->rp->fR, p->lp->fR);
				}
				else {
					p->id = hp[p->id][p->l]; // 使い回して頂点番号を入れておく
					st[p->id] = p;

					int r = sz(lc[p->id]);
					if (r > 0) {
						p->lp = new Node{ 'L', p->id, 0, r, p };
						dfs_btree(p->lp);

						p->f = p->fR = L_to_H(p->lp->g, p->id);
					}
					else {
						p->f = p->fR = get_H(p->id);
					}
				}
			}
			else if (p->tp == 'L') {
				if (p->r - p->l > 1) {
					int m = (p->l + p->r) / 2;

					p->lp = new Node{ 'L', p->id, p->l, m, p };
					dfs_btree(p->lp);

					p->rp = new Node{ 'L', p->id, m, p->r, p };
					dfs_btree(p->rp);

					p->g = comp_L(p->lp->g, p->rp->g);
				}
				else {
					p->id = lc[p->id][p->l]; // 使い回して頂点番号を入れておく
					int r = sz(hp[p->id]);

					p->lp = new Node{ 'H', p->id, 0, r, p };
					dfs_btree(p->lp);

					p->g = H_to_L(p->lp->f); // この向きでダメなときはどうせ再計算している
				}
			}
		};
		dfs_btree(root);
	}

	// 頂点 s の情報の更新を反映する．
	void update(int s) {
		// verify : https://judge.yosupo.jp/problem/point_set_tree_path_composite_sum

		Node* p = st[s];

		// ボトムアップに必要な箇所のみ更新する．
		while (p) {
			if (p->tp == 'H') {
				// 子が居ない場合
				if (!p->lp) {
					p->f = p->fR = get_H(p->id);
				}
				// 子が L 1 つの場合
				else if (!p->rp) {
					p->f = p->fR = L_to_H(p->lp->g, p->id);
				}
				// 子が H 2 つの場合
				else {
					p->f = comp_H(p->lp->f, p->rp->f);
					p->fR = comp_H(p->rp->fR, p->lp->fR);
				}
			}
			else if (p->tp == 'L') {
				// 子が H 1 つの場合
				if (!p->rp) {
					p->g = H_to_L(p->lp->f); // この向きでダメなときはどうせ再計算している
				}
				// 子が L 2 つの場合
				else {
					p->g = comp_L(p->lp->g, p->rp->g);
				}
			}

			p = p->pp;
		}
	}

	// rt を根とみたときの根付き木全体の値を返す．
	H2 get(int rt) {
		// verify : https://judge.yosupo.jp/problem/point_set_tree_path_composite_sum

		Node* p = st[rt];

		H2 f2 = H_to_H2(p->f);
		L2 g2;

		// ボトムアップに計算する．
		while (true) {
			Node* pp = p->pp;
			if (!pp) break;

			if (pp->tp == 'H') {
				if (p->tp == 'L') {
					f2 = L2_to_H2(g2, pp->id);
				}
				else if (pp->lp == p) {
					f2 = comp_H2d(f2, pp->rp->f);
				}
				else if (pp->rp == p) {
					f2 = comp_H2u(f2, pp->lp->fR);
				}
			}
			else {
				if (p->tp == 'H') {
					g2 = H2_to_L2(f2);
				}
				else if (pp->lp == p) {
					g2 = comp_L2(g2, pp->rp->g);
				}
				else if (pp->rp == p) {
					g2 = comp_L2(g2, pp->lp->g);
				}
			}

			p = pp;
		}

		return f2;
	}

	/* 雛形
	vm A, B, C; int n;
	struct H {
		mint a, b, v, c;
	#ifdef _MSC_VER
		friend ostream& operator<<(ostream& os, const H& x) {
			os << '(' << x.a << "," << x.b << "," << x.v << "," << x.c << ')';
			return os;
		}
	#endif
	};
	struct L {
		mint v, c;
	#ifdef _MSC_VER
		friend ostream& operator<<(ostream& os, const L& x) {
			os << '(' << x.v << "," << x.c << ')';
			return os;
		}
	#endif
	};
	struct H2 {
		mint al, bl, ah, bh, v, c;
	#ifdef _MSC_VER
		friend ostream& operator<<(ostream& os, const H2& x) {
			os << '(' << x.al << "," << x.bl << "," << x.ah << "," << x.bh << "," << x.v << "," << x.c << ')';
			return os;
		}
	#endif
	};
	struct L2 {
		mint a, b, vl, cl, vh, ch;
	#ifdef _MSC_VER
		friend ostream& operator<<(ostream& os, const L2& x) {
			os << '(' << x.a << "," << x.b << "," << x.vl << "," << x.cl << "," << x.vh << "," << x.ch << ')';
			return os;
		}
	#endif
	};
	H get_H(int i) {
		H h;

		if (i < n) {
			h.a = 1;
			h.b = 0;
			h.v = A[i];
			h.c = 1;
		}
		else {
			i -= n;
			h.a = B[i];
			h.b = C[i];
			h.v = 0;
			h.c = 0;
		}

		return h;
	}
	H L_to_H(const L& f, int i) {
		H h;

		if (i < n) {
			h.a = 1;
			h.b = 0;
			h.v = 1 * (f.v + A[i]) + 0 * (f.c + 1);
			h.c = f.c + 1;
		}
		else {
			i -= n;
			h.a = B[i];
			h.b = C[i];
			h.v = B[i] * (f.v + 0) + C[i] * (f.c + 1);
			h.c = f.c + 0;
		}

		return h;
	}
	L H_to_L(const H& f) {
		L h;

		h.v = f.v;
		h.c = f.c;

		return h;
	}
	H2 H_to_H2(const H& f) {
		H2 h;

		h.al = h.ah = f.a;
		h.bl = h.bh = f.b;
		h.v = f.v;
		h.c = f.c;

		return h;
	}
	H2 L2_to_H2(const L2& f, int i) {
		H2 h;

		if (i < n) {
			h.al = h.ah = f.a * 1;
			h.bl = h.bh = f.a * 0 + f.b;
			h.v = (h.al * f.vl + h.bl * f.cl) + (f.a * A[i] + f.b) + f.vh;
			h.c = 1 + f.cl + f.ch;
		}
		else {
			i -= n;
			h.al = h.ah = f.a * B[i];
			h.bl = h.bh = f.a * C[i] + f.b;
			h.v = (h.al * f.vl + h.bl * f.cl) + (f.a * 0 + f.b) + f.vh;
			h.c = 0 + f.cl + f.ch;
		}

		return h;
	}
	L2 H2_to_L2(const H2& f) {
		L2 h;

		h.a = f.ah;
		h.b = f.bh;
		h.vh = f.v;
		h.ch = f.c;
		h.vl = 0;
		h.cl = 0;

		return h;
	}
	H comp_H(const H& f, const H& g) {
		H h;

		h.a = f.a * g.a;
		h.b = f.a * g.b + f.b;
		h.v = f.a * g.v + f.b * g.c + f.v;
		h.c = f.c + g.c;

		return h;
	}
	L comp_L(const L& f, const L& g) {
		L h;

		h.v = f.v + g.v;
		h.c = f.c + g.c;

		return h;
	}
	H2 comp_H2u(const H2& f, const H& g) {
		H2 h;

		h.ah = f.ah * g.a;
		h.bh = f.ah * g.b + f.bh;
		h.al = f.al;
		h.bl = f.bl;
		h.v = f.ah * g.v + f.bh * g.c + f.v;
		h.c = f.c + g.c;

		return h;
	}
	H2 comp_H2d(const H2& f, const H& g) {
		H2 h;

		h.al = f.al * g.a;
		h.bl = f.al * g.b + f.bl;
		h.ah = f.ah;
		h.bh = f.bh;
		h.v = f.al * g.v + f.bl * g.c + f.v;
		h.c = f.c + g.c;

		return h;
	}
	L2 comp_L2(const L2& f, const L& g) {
		L2 h;

		h.a = f.a;
		h.b = f.b;
		h.vh = f.vh;
		h.ch = f.ch;
		h.vl = f.vl + g.v;
		h.cl = f.cl + g.c;

		return h;
	}
	Static_top_tree_rerooting<H, L, H2, L2, get_H, L_to_H, H_to_L, H_to_H2, L2_to_H2, H2_to_L2, comp_H, comp_L, comp_H2u, comp_H2d, comp_L2> G(g);
	*/
};


//【動的全方位木 DP（直径短，次数小）】
/*
* Dynamic_rerooting<T, leaf, add_edge, merge, add_vertex>(Graph g, int r = 0) : O(n)
*	r を根とする木 g で初期化する．
*
* set_root(int r) : O(diam deg)（diam : 木の直径，deg : 頂点の最大次数）
*	根を r に変更する．
*
* update(int v) : O(diam deg)
*	頂点 v の情報の更新を反映する．
*
* T get(int s) : O(1)
*	頂点 s における木 DP の結果を返す．
*
* なおテンプレート引数が表す関数は以下の通りとする：
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
class Dynamic_rerooting {
	int n; // 頂点数
	Graph g; // 木
	int r; // 根

	vector<T> dp; // 木 DP の値

	vi dep; // 初期化時の r を根としたときの深さ
	vi par; // 初期化時の r を根としたときの親

	// 再帰的に木全体の情報を計算する．
	void build(int s, int p) {
		// is_leaf : s が葉か
		bool is_leaf = true;

		repe(t, g[s]) {
			if (t == p) continue;

			// 部分木 t についての情報を計算する．
			build(t, s);
			dep[t] = dep[s] + 1;
			par[t] = s;

			// 部分木 t に対して辺 s'→t を追加した場合の部分木 s' についての情報を得る．
			T sub = add_edge(dp[t], s, t);

			// それを部分木 s' の暫定の情報とマージして情報を更新していく．
			if (is_leaf) dp[s] = move(sub);
			else merge(dp[s], sub, s);

			is_leaf = false;
		}

		// s が葉の場合は専用の情報を代入しておく．
		if (is_leaf) dp[s] = leaf(s);
		// そうでない場合は根 s を追加する．
		else add_vertex(dp[s], s);
	}

	// s から t までのパスを path に格納する．
	void get_path(int s, int t, deque<int>& path) {
		if (dep[s] > dep[t]) {
			get_path(par[s], t, path);
			path.push_front(s);
		}
		else if (dep[s] < dep[t]) {
			get_path(s, par[t], path);
			path.push_back(t);
		}
		else if (s != t) {
			get_path(par[s], par[t], path);
			path.push_front(s);
			path.push_back(t);
		}
		else {
			path.push_front(s);
		}
	}

	// ST から GL（根）までの情報を更新する．
	void update(int ST, int GL) {
		deque<int> path;
		get_path(ST, GL, path);
		int L = sz(path);

		rep(l, L) {
			int s = path[l];
			int p = (l < L - 1 ? path[l + 1] : -1);

			// is_leaf : s が葉か
			bool is_leaf = true;

			repe(t, g[s]) {
				if (t == p) continue;

				// 部分木 t についての情報は全て計算済

				// 部分木 t に対して辺 s'→t を追加した場合の部分木 s' についての情報を得る．
				T sub = add_edge(dp[t], s, t);

				// それを部分木 s' の暫定の情報とマージして情報を更新していく．
				if (is_leaf) dp[s] = move(sub);
				else merge(dp[s], sub, s);

				is_leaf = false;
			}

			// s が葉の場合は専用の情報を代入しておく．
			if (is_leaf) dp[s] = leaf(s);
			// そうでない場合は根 s を追加する．
			else add_vertex(dp[s], s);
		}
	}

public:
	// r を根とする木 g で初期化する．
	Dynamic_rerooting(const Graph& g, int r = 0) : n(sz(g)), g(g), r(r), dp(n), dep(n), par(n, -1) {
		// verify : https://atcoder.jp/contests/tdpc/tasks/tdpc_tree

		build(r, -1);
	}
	Dynamic_rerooting() : n(0), r(0) {}

	// 根を r に変更する．
	void set_root(int r) {
		// verify : https://atcoder.jp/contests/tdpc/tasks/tdpc_tree
		
		update(this->r, r);
		this->r = r;
	}

	// 頂点 v の情報の更新を反映する．
	void update(int v) {
		update(v, r);
	}

	// 頂点 s における木 DP の結果を返す．
	T get(int s) {
		// verify : https://atcoder.jp/contests/tdpc/tasks/tdpc_tree
		
		return dp[s];
	}

	/* 雛形
	vl wgt;
	struct T {
		ll v;
	#ifdef _MSC_VER
		friend ostream& operator<<(ostream& os, const T& x) {
			os << '(' << x.v << ')';
			return os;
		}
	#endif
	};
	T leaf(int s) {
		return T{ wgt[s] };
	}
	T add_edge(const T& x, int p, int s) {
		return x;
	}
	void merge(T& x, const T& y, int s) {
		x.v += y.v;
	}
	void add_vertex(T& x, int s) {
		x.v += wgt[s];
	}
	Dynamic_rerooting<T, leaf, add_edge, merge, add_vertex> G(g);
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

