#pragma once
#include "header.h"
#include "構造(木).h"
#include "フェニック木.h"
// ■■■■■ 木のクエリ処理 ■■■■■


//【オイラーツアー】
/*
* Euler_tour(Graph g, int rt) : O(n)
*	rt を根とする根付き木 g で初期化する．
*
* int lca(int s, int t) : O(log n)
*	頂点 s, t の最小共通祖先を返す．
*
* int dist(int s, int t) : O(log n)
*	頂点 s, t 間の距離を返す．
*
* int jump(int s, int t, int i) : O(log n)
*	頂点 s から t までのパスの i 番目（0-indexed）の頂点を返す（なければ -1）
*
* sort_by_DFS_order(vi& vs) : O(log |vs|)
*	頂点集合 vs を DFS 昇順にソートする．
*/
ll op_ET(ll a, ll b) { return min(a, b); }
ll e_ET() { return INFL; }
struct Euler_tour {
	int n;

	// in[s]  : rt からの DFS で最初に頂点 s を訪れた時刻（根なら 0）
	// out[s] : rt からの DFS で最後に頂点 s から離れた時刻（根なら 2n-1）
	// pos[t] : rt からの DFS で時刻 t に居た頂点の番号（長さ 2n-1）
	// dep[s] : 頂点 s の深さ
	vi in, out, pos, dep;

	// seg[t] : 時刻 t に居た頂点の (深さ, 番号)
	using SEG = segtree<ll, op_ET, e_ET>;
	SEG seg;

	void dfs(const Graph& g, int rt) {
		int time = 0;

		function<void(int, int)> rf = [&](int s, int p) {
			// s を最初に訪れた
			in[s] = time;
			pos[time] = s;
			time++;

			repe(t, g[s]) {
				if (t == p) continue;

				dep[t] = dep[s] + 1;
				rf(t, s);
				pos[time] = s;
				time++;
			}

			// s から最後に離れる
			out[s] = time;
		};

		// 根から順に探索する．
		rf(rt, -1);
	}

public:
	// rt を根とする根付き木 g で初期化する．
	Euler_tour(const Graph& g, int rt) : n(sz(g)), in(n), out(n), pos(2 * n - 1), dep(n) {
		// verify : https://judge.yosupo.jp/problem/lca

		dfs(g, rt);

		vl ini(2 * n - 1);
		rep(t, 2 * n - 1) ini[t] = ((ll)dep[pos[t]] << 32) | pos[t];
		seg = SEG(ini);
	}
	Euler_tour() : n(0) {}

	// 頂点 s, t の最小共通祖先を返す．
	int lca(int s, int t) const {
		// verify : https://judge.yosupo.jp/problem/lca

		// 初めて s または t に訪れたとき
		int l = min(in[s], in[t]);

		// 最後に s または t から離れたとき
		int r = max(out[s], out[t]);

		// その途中で訪れたことのある最も浅い頂点が最小共通祖先
		return (int)seg.prod(l, r);
	}

	// 頂点 s, t 間の距離を返す．
	int dist(int s, int t) const {
		// verify : https://yukicoder.me/problems/no/2337

		int p = lca(s, t);

		// 根からの距離（深さ）の和を求め，ダブっている分を引く．
		return dep[s] + dep[t] - 2 * dep[p];
	}

	// 頂点 s から t までのパスの i 番目（0-indexed）の頂点を返す（なければ -1）
	int jump(int s, int t, int i) const {
		// verify : https://judge.yosupo.jp/problem/jump_on_tree

		int p = lca(s, t);
		int ds = dep[s], dt = dep[t], dp = dep[p];
		int dist = ds + dt - 2 * dp;

		int res;

		if (i < 0 || i > dist) {
			res = -1;
		}
		else if (i <= ds - dp) {
			int j = seg.max_right(out[s] - 1, [&](ll tmp) { return (tmp >> 32) > ds - i; });
			res = pos[j];
		}
		else {
			int j = seg.min_left(in[t] + 1, [&](ll tmp) { return (tmp >> 32) >= dt - (dist - i); });
			res = pos[j];
		}

		return res;
	}

	// 頂点集合 vs を DFS 昇順にソートする．
	void sort_by_DFS_order(vi& vs) const {
		// verify : https://yukicoder.me/problems/no/2588

		sort(all(vs), [&](int s, int t) { return in[s] < in[t]; });
	}
};


//【オイラーツアー（重み付き）】
/*
* Euler_tour(WGraph g, int rt) : O(n)
*	rt を根とする重み付き根付き木 g で初期化する．
*
* int lca(int s, int t) : O(log n)
*	頂点 s, t の最小共通祖先を返す．
*
* int dist(int s, int t) : O(log n)
*	頂点 s, t 間の距離を返す．
*
* sort_by_DFS_order(vi& vs) : O(log |vs|)
*	頂点集合 vs を DFS 昇順にソートする．
*/
pli op_ETw(pli a, pli b) { return min(a, b); }
pli e_ETw() { return { INFL, -1 }; }
struct Euler_tour_weighted {
	int n;

	// in[s]  : rt からの DFS で最初に頂点 s を訪れた時刻（根なら 0）
	// out[s] : rt からの DFS で最後に頂点 s から離れた時刻（根なら 2n-1）
	// pos[t] : rt からの DFS で時刻 t に居た頂点の番号（長さ 2n-1）
	// dep[s] : 頂点 s の深さ（重み付き）
	vi in, out, pos; vl dep;

	// seg[t] : 時刻 t に居た頂点の (深さ, 番号)
	using SEG = segtree<pli, op_ETw, e_ETw>;
	SEG seg;

	void dfs(const WGraph& g, int rt) {
		int time = 0;

		function<void(int, int)> rf = [&](int s, int p) {
			// s を最初に訪れた
			in[s] = time;
			pos[time] = s;
			time++;

			repe(t, g[s]) {
				if (t == p) continue;

				dep[t] = dep[s] + t.cost;
				rf(t, s);
				pos[time] = s;
				time++;
			}

			// s から最後に離れる
			out[s] = time;
		};

		// 根から順に探索する．
		rf(rt, -1);
	}

public:
	// rt を根とす重み付き根付き木 g で初期化する．
	Euler_tour_weighted(const WGraph& g, int rt) : n(sz(g)), in(n), out(n), pos(2 * n - 1), dep(n) {
		dfs(g, rt);

		vector<pli> ini(2 * n - 1);
		rep(t, 2 * n - 1) ini[t] = { dep[pos[t]], pos[t] };
		seg = SEG(ini);
	}
	Euler_tour_weighted() : n(0) {}

	// 頂点 s, t の最小共通祖先を返す．
	int lca(int s, int t) const {
		// 初めて s または t に訪れたとき
		int l = min(in[s], in[t]);

		// 最後に s または t から離れたとき
		int r = max(out[s], out[t]);

		// その途中で訪れたことのある最も浅い頂点が最小共通祖先
		return seg.prod(l, r).second;
	}

	// 頂点 s, t 間の距離を返す．
	ll dist(int s, int t) const {
		int p = lca(s, t);

		// 根からの距離（深さ）の和を求め，ダブっている分を引く．
		return dep[s] + dep[t] - 2 * dep[p];
	}

	// 頂点集合 vs を DFS 昇順にソートする．
	void sort_by_DFS_order(vi& vs) const {
		sort(all(vs), [&](int s, int t) { return in[s] < in[t]; });
	}
};


//【[1点,1辺]加算／[部分木]総和クエリ（アーベル群）】
/*
* Subtree_sum_query<S, op, o, inv>(Graph g, int rt) : O(n)
*	rt を根とする根付き木 g と値 o() で初期化する．
*	要素はアーベル群 (S, op, o, inv) の元とする．
*
* add_vertex(int s, S val) : O(log n)
*	頂点 s に val を加算する．
*
* add_edge(int s, S val) : O(log n)
*	頂点 s を親とする辺に val を加算する（どの辺でも結果は変わらない）
*
* S sum_subtree(int s) : O(log n)
*	部分木 s の頂点と辺の値の総和を返す．
*
* 利用：【フェニック木（アーベル群）】
*/
template <class S, S(*op)(S, S), S(*o)(), S(*inv)(S)>
class Subtree_sum_query {
	int n;

	// in[s]  : 根からの DFS で s に最初に入った時刻
	// out[s] : 根からの DFS で s から最後に出た時刻
	vi in, out;

	// ft[t] : 時刻 t に居た頂点の値
	Fenwick_tree<S, op, o, inv> ft;

	// ユニークオイラーツアー
	void euler_tour(const Graph& g, int rt) {
		int time = 0;

		function<void(int, int)> rf = [&](int s, int p) {
			// s を最初に訪れた
			in[s] = time;
			time++;

			repe(t, g[s]) {
				if (t == p) continue;

				rf(t, s);
			}

			// s から最後に離れる
			out[s] = time;
		};

		// 根から順に探索する．
		rf(rt, -1);
	}

public:
	// rt を根とする根付き木 g と値 o() で初期化する．
	Subtree_sum_query(const Graph& g, int rt) : n(sz(g)), in(n), out(n), ft(n) {
		// verify : https://www.codechef.com/problems/TREEDEST

		euler_tour(g, rt);
	}

	// 頂点 s に val を加算する．
	void add_vertex(int s, S val) {
		// verify : https://www.codechef.com/problems/TREEDEST

		ft.add(in[s], val);
	}

	// 頂点 s を親とする辺のいずれかに val を加算する．
	void add_edge(int s, S val) {
		// 部分木については
		//		s を親とする辺を含む ⇔ s を含む
		// なので，代わりに頂点 s に val を加算する．
		add_vertex(s, val);
	}

	// 部分木 s の全ての頂点と辺の値の総和を返す．
	S sum_subtree(int s) {
		// verify : https://www.codechef.com/problems/TREEDEST

		return ft.sum(in[s], out[s]);
	}
};


//【[1点,1辺]加算／[根からのパス]総和クエリ（アーベル群）】
/*
* Path_sum_query<S, op, o, inv>(Graph g, int rt) : O(n)
*	rt を根とする根付き木 g と値 o() で初期化する．
*	要素はアーベル群 (S, op, o, inv) の元とする．
*
* add_vertex(int s, S val) : O(log n)
*	頂点 s に val を加算する．
*
* add_edge(int s, S val) : O(log n)
*	頂点 s を子とする辺に val を加算する．
*
* S sum_root_path(int s) : O(log n)
*	根 rt から頂点 s まで（両端含む）の頂点と辺の値の総和を返す．
*
* S get(int s) : O(log n)
*	頂点 s の値を返す．
*
* 利用：【フェニック木（アーベル群）】
*/
template <class S, S(*op)(S, S), S(*o)(), S(*inv)(S)>
class Path_sum_query {
	// 参考 : https://perogram.hateblo.jp/entry/2020/10/01/034136

	int n;

	// in[s]  : 根からの DFS で s に最初に入った時刻
	// out[s] : 根からの DFS で s から最後に出た時刻
	// p[s]   : s の親（根なら -1）
	vi in, out, p;

	// ft[t] : 時刻 t に居た頂点の値
	Fenwick_tree<S, op, o, inv> ft;

	// ユニークオイラーツアー
	void euler_tour(const Graph& g, int rt) {
		int time = 0;

		function<void(int)> rf = [&](int s) {
			// s を最初に訪れた
			in[s] = time;
			time++;

			repe(t, g[s]) {
				if (t == p[s]) continue;
				p[t] = s;

				rf(t);
				// add_edge(t, t.cost); // 重み付きグラフの場合
			}

			// s から最後に離れる
			out[s] = time;
		};

		// 根から順に探索する．
		p[rt] = -1;
		rf(rt);
	}

public:
	// rt を根とする根付き木 g と値 o() で初期化する．
	Path_sum_query(const Graph& g, int rt) : n(sz(g)), in(n), out(n), p(n), ft(n + 1) {
		// verify : https://atcoder.jp/contests/kupc2012/tasks/kupc2012_11

		euler_tour(g, rt);
	}

	// 頂点 s に val を加算する．
	void add_vertex(int s, S val) {
		// いもす法のように，部分木 s にいる間だけ val が累積和に寄与するようにする．
		ft.add(in[s], val);
		ft.add(out[s], inv(val));
	}

	// 頂点 s を子とする辺に val を加算する．
	void add_edge(int s, S val) {
		// verify : https://atcoder.jp/contests/kupc2012/tasks/kupc2012_11

		// 根からのパスについては
		//		s を子とする辺を通る ⇔ s を通る
		// なので，代わりに頂点 s に val を加算する．
		add_vertex(s, val);
	}

	// 根 r から s までの頂点と辺の値の総和を返す．
	S sum_root_path(int s) const {
		// verify : https://atcoder.jp/contests/kupc2012/tasks/kupc2012_11

		return ft.sum(0, in[s] + 1);
	}

	// 頂点 s の値を返す．
	S get(int s) const {
		S res = sum_root_path(s);
		if (p[s] != -1) res = op(res, inv(sum_root_path(p[s])));
		return res;
	}

	// 頂点 s の親を返す（なければ -1）
	int get_parent(int s) const {
		return p[s];
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Path_sum_query Q) {
		rep(s, Q.n) os << Q.get(s) << " ";
		return os;
	}
#endif
};


//【[部分木]頂点作用／[部分木]頂点総和クエリ（M-可換モノイド）】
/*
* Subtree_apply_sum_query<S, op, o, F, act, comp, id>(Graph g, int rt) : O(n)
*	rt を根とする根付き木 g と値 o() で初期化する．
*	要素は M-可換モノイド (S, op, o, F, act, comp, id) の元とする．
*
* Subtree_apply_sum_query<S, op, o, F, act, comp, id>(Graph g, int rt, vS v) : O(n)
*	rt を根とする根付き木 g と値 v[0..n) で初期化する．
*
* apply(int s, F f) : O(log n)
*	頂点 s の値に f を作用させる．
*
* apply_subtree(int s, F f) : O(log n)
*	部分木 s の全ての頂点の値に f を作用させる．
*
* S get(int s) : O(log n)
*	頂点 s の値を返す．
*
* S sum_subtree(int s) : O(log n)
*	部分木 s の全ての頂点の値の総和を返す．
*/
template <class S, S(*op)(S, S), S(*o)(), class F, S(*act)(F, S), F(*comp)(F, F), F(*id)()>
class Subtree_apply_sum_query {
	int n;

	// in[s]  : 根からの DFS で s に最初に入った時刻
	// out[s] : 根からの DFS で s から最後に出た時刻
	vi in, out;

	// seg[t] : 時刻 t に居た頂点の値
	using SEG = lazy_segtree<S, op, o, F, act, comp, id>;
	SEG seg;

	// ユニークオイラーツアー
	void euler_tour(const Graph& g, int rt) {
		int time = 0;

		function<void(int, int)> rf = [&](int s, int p) {
			// s を最初に訪れた
			in[s] = time;
			time++;

			repe(t, g[s]) {
				if (t == p) continue;

				rf(t, s);
			}

			// s から最後に離れる
			out[s] = time;
		};

		// 根から順に探索する．
		rf(rt, -1);
	}

public:
	// rt を根とする根付き木 g と値 o() で初期化する．
	Subtree_apply_sum_query(const Graph& g, int rt) : n(sz(g)), in(n), out(n), seg(n) {
		euler_tour(g, rt);
	}

	// rt を根とする根付き木 g と値 v[0..n) で初期化する．
	Subtree_apply_sum_query(const Graph& g, int rt, const vector<S>& v) : n(sz(g)), in(n), out(n) {
		// verify : https://www.codechef.com/problems/CHEFDIVISION

		euler_tour(g, rt);
		vector<S> ini(n);
		rep(s, n) ini[in[s]] = v[s];
		seg = SEG(ini);
	}
	Subtree_apply_sum_query() : n(0) {}

	// 頂点 s の値に f を作用させる．
	void apply(int s, F f) {
		seg.apply(in[s], f);
	}

	// 部分木 s の全ての頂点の値に f を作用させる．
	void apply_subtree(int s, F f) {
		// verify : https://www.codechef.com/problems/CHEFDIVISION

		seg.apply(in[s], out[s], f);
	}

	// 頂点 s の値を返す．
	S get(int s) {
		// verify : https://atcoder.jp/contests/abc138/tasks/abc138_d

		return seg.get(in[s]);
	}

	// 部分木 s の全ての頂点の値の総和を返す．
	S sum_subtree(int s) {
		// verify : https://www.codechef.com/problems/CHEFDIVISION

		return seg.prod(in[s], out[s]);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Subtree_apply_sum_query Q) {
		rep(i, Q.n) os << Q.get(i) << " ";
		return os;
	}
#endif
};


//【[2隣接]頂点作用／[2隣接]頂点総和クエリ（M-可換モノイド）】
/*
* Tree_neighbor_vertex_query<S, op, o, F, act, comp, id>(Graph g, int rt) : O(n)
*	rt を根とする根付き木 g と頂点の初期値 v[0..n) = o() で初期化する．
*	要素は M-可換モノイド (S, op, o, F, act, comp, id) の元とする．
*
* Tree_neighbor_vertex_query<S, op, o, F, act, comp, id>(Graph g, int rt, vS a) : O(n)
*	rt を根とする根付き木 g と頂点の初期値 v[0..n) = a[0..n) で初期化する．
*
* set(int s, S x) : O(log n)
*	v[s] = x とする．
*
* S get(int s) : O(log n)
*	v[s] を返す．
*
* S child_sum(int s) : O(log n)
*	頂点 s の子の値の総和を返す．
*
* S neighbor_sum(int s) : O(log n)
*	頂点 s からの距離が 1 以下である頂点の値の総和を返す．
*
* S neighbor2_sum(int s) : O(log n)
*	頂点 s からの距離が 2 以下である頂点の値の総和を返す．
*
* S all_sum() : O(1)
*	全頂点の値の総和を返す．
*
* apply(int s, F f) : O(log n)
*	v[s] に f を作用させる．
*
* child_apply(int s, F f) : O(log n)
*	頂点 s の子の値に f を作用させる．
*
* neighbor_apply(int s, F f) : O(log n)
*	頂点 s からの距離が 1 以下である頂点の値に f を作用させる．
*
* neighbor2_apply(int s, F f) : O(log n)
*	頂点 s からの距離が 2 以下である頂点の値に f を作用させる．
*
* sort_by_BFS_order(vi& vs) : O(log |vs|)
*	頂点集合 vs を BFS 昇順にソートする．
*/
template <class S, S(*op)(S, S), S(*o)(), class F, S(*act)(F, S), F(*comp)(F, F), F(*id)()>
class Tree_neighbor_vertex_query {
	// n : 頂点数
	int n;

	// rt : 根
	int rt;

	// in[s] : 根からの BFS で頂点 s をなぞった時刻（根なら 0）
	vi in;

	// in_min[s] : 頂点 s の子 t の in[t] の最小値（葉なら n）
	// in_max[s] : 頂点 s の子 t の in[t] の最大値（葉なら -1）
	vi in_min, in_max;

	// in_min2[s] : 頂点 s の子 t の in_min[t] の最小値（葉なら n）
	// in_max2[s] : 頂点 s の子 t の in_max[t] の最大値（葉なら -1）
	vi in_min2, in_max2;

	// p[s] : 頂点 s の親
	vi p;

	// v[s] : 頂点 s の値
	lazy_segtree<S, op, o, F, act, comp, id> v;

	void bfs_euler_tour(const Graph& g, int rt) {
		// rt から BFS し in[s], p[s] を定める．
		queue<int> q; q.push(rt);
		in[rt] = 0;

		int now = 1;
		while (!q.empty()) {
			auto s = q.front(); q.pop();

			repe(t, g[s]) {
				if (in[t] != -1) continue;

				in[t] = now++;
				p[t] = s;
				q.push(t);
			}
		}

		// in_min[s], in_max[s] を定める．
		rep(s, n) repe(t, g[s]) {
			if (p[s] == t) continue;

			chmin(in_min[s], in[t]);
			chmax(in_max[s], in[t]);
		}

		// in_min2[s], in_max2[s] を定める．
		rep(s, n) repe(t, g[s]) {
			if (p[s] == t) continue;

			chmin(in_min2[s], in_min[t]);
			chmax(in_max2[s], in_max[t]);
		}
	}

public:
	// rt を根とする根付き木 g と頂点の初期値 o() で初期化する．
	Tree_neighbor_vertex_query(const Graph& g, int rt)
		: n(sz(g)), rt(rt), in(n, -1), in_min(n, n), in_max(n, -1), in_min2(n, n), in_max2(n, -1), p(n, -1), v(n)
	{
		bfs_euler_tour(g, rt);
	}

	// rt を根とする根付き木 g と頂点の初期値 a[0..n) で初期化する．
	Tree_neighbor_vertex_query(const Graph& g, int rt, const vector<S>& a)
		: n(sz(g)), rt(rt), in(n, -1), in_min(n, n), in_max(n, -1), in_min2(n, n), in_max2(n, -1), p(n, -1)
	{
		// verify : https://yukicoder.me/problems/no/899

		bfs_euler_tour(g, rt);

		vector<S> ini(n);
		rep(s, n) ini[in[s]] = a[s];
		v = lazy_segtree<S, op, o, F, act, comp, id>(ini);
	}
	Tree_neighbor_vertex_query() : n(0), rt(-1) {}

	// v[s] = x とする．
	void set(int s, S x) {
		Assert(0 <= s && s < n);

		v.set(in[s], x);
	}

	// v[s] を返す．
	S get(int s) {
		// verify : https://atcoder.jp/contests/arc148/tasks/arc148_c

		Assert(0 <= s && s < n);

		return v.get(in[s]);
	}

	// 頂点 s の子の値の総和を返す．
	S child_sum(int s) {
		Assert(0 <= s && s < n);

		// s が葉の場合は o() を返す．
		if (in_max[s] == -1) return o();

		return v.prod(in_min[s], in_max[s] + 1);
	}

	// 頂点 s からの距離が 1 以下である頂点の値の総和を返す．
	S neighbor_sum(int s) {
		Assert(0 <= s && s < n);

		S res = v.get(in[s]);
		if (p[s] != -1) res = op(res, v.get(in[p[s]]));
		res = op(res, child_sum(s));

		return res;
	}

	// 頂点 s からの距離が 2 以下である頂点の値の総和を返す．
	S neighbor2_sum(int s) {
		// verify : https://yukicoder.me/problems/no/899

		Assert(0 <= s && s < n);

		// 根方向に距離 [0..2] 進んだ頂点からの寄与
		S res = (p[s] != -1 ? neighbor_sum(p[s]) : v.get(in[s]));

		// 葉方向に距離 1 進んだ頂点からの寄与
		res = op(res, child_sum(s));

		// 葉方向に距離 2 進んだ頂点からの寄与
		if (in_max2[s] != -1) res = op(res, v.prod(in_min2[s], in_max2[s] + 1));

		return res;
	}

	// 全頂点の値の総和を返す．
	S all_sum() {
		// verify : https://atcoder.jp/contests/arc148/tasks/arc148_c

		return v.all_prod();
	}

	// v[s] に f を作用させる．
	void apply(int s, F f) {
		// verify : https://atcoder.jp/contests/arc148/tasks/arc148_c

		Assert(0 <= s && s < n);

		v.apply(in[s], f);
	}

	// 頂点 s の子の値に f を作用させる．
	void child_apply(int s, F f) {
		Assert(0 <= s && s < n);

		// s が葉の場合は何もしない．
		if (in_max[s] == -1) return;

		v.apply(in_min[s], in_max[s] + 1, f);
	}

	// 頂点 s からの距離が 1 以下である頂点の値に f を作用させる．
	void neighbor_apply(int s, F f) {
		Assert(0 <= s && s < n);

		v.apply(in[s], f);
		if (p[s] != -1) v.apply(in[p[s]], f);
		child_apply(s, f);
	}

	// 頂点 s からの距離が 2 以下である頂点の値に f を作用させる．
	void neighbor2_apply(int s, F f) {
		// verify : https://yukicoder.me/problems/no/899

		Assert(0 <= s && s < n);

		// 根方向に距離 [0..2] 進んだ頂点への作用
		if (p[s] != -1) neighbor_apply(p[s], f);
		else v.apply(in[s], f);

		// 葉方向に距離 1 進んだ頂点への作用
		child_apply(s, f);

		// 葉方向に距離 2 進んだ頂点への作用
		if (in_max2[s] != -1) v.apply(in_min2[s], in_max2[s] + 1, f);
	}

	// 全頂点の値に f を作用させる．
	void all_apply(F f) {
		// verify : https://atcoder.jp/contests/arc148/tasks/arc148_c

		v.apply(0, n, f);
	}

	// 頂点集合 vs を BFS 昇順にソートする．
	void sort_by_BFS_order(vi& vs) {
		// verify : https://atcoder.jp/contests/arc148/tasks/arc148_c

		sort(all(vs), [&](int s, int t) {return in[s] < in[t]; });
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Tree_neighbor_vertex_query TNVQ) {
		os << TNVQ.v;
		return os;
	}
#endif
};


//【[隣接,部分木]頂点作用／[隣接,部分木]頂点総和クエリ（M-可換モノイド）】
/*
* Tree_neighbor_subtree_vertex_query<S, op, o, F, act, comp, id>(Graph g, int rt) : O(n)
*	rt を根とする根付き木 g と頂点値 v[0..n) = o() で初期化する．
*	要素は M-可換モノイド (S, op, o, F, act, comp, id) の元とする．
*
* Tree_neighbor_subtree_vertex_query<S, op, o, F, act, comp, id>(Graph g, int rt, vS a) : O(n)
*	rt を根とする根付き木 g と頂点値 v[0..n) = a[0..n) で初期化する．
*
* set(int s, S x) : O(log n)
*	v[s] = x とする．
*
* S get(int s) : O(log n)
*	v[s] を返す．
*
* S child_sum(int s) : O(log n)
*	頂点 s の子の値の総和を返す．
*
* S neighbor_sum(int s) : O(log n)
*	頂点 s からの距離が 1 以下である頂点の値の総和を返す．
*
* S all_sum() : O(1)
*	全頂点の値の総和を返す．
*
* apply(int s, F f) : O(log n)
*	v[s] に f を作用させる．
*
* child_apply(int s, F f) : O(log n)
*	頂点 s の子の値に f を作用させる．
*
* neighbor_apply(int s, F f) : O(log n)
*	頂点 s からの距離が 1 以下である頂点の値に f を作用させる．
*
* sort_by_BFS_like_DFS_order(vi& vs) : O(log |vs|)
*	頂点集合 vs を BFS 風 DFS 昇順にソートする．
*/
template <class S, S(*op)(S, S), S(*o)(), class F, S(*act)(F, S), F(*comp)(F, F), F(*id)()>
class Tree_neighbor_subtree_vertex_query {
	// n : 頂点数
	int n;

	// rt : 根
	int rt;

	// in[s] : 根からの BFS 風 DFS で頂点 s をなぞった時刻（根なら 0）
	vi in;

	// in_min[s] : 頂点 s の子 t の in[t] の最小値（葉なら -1）
	// in_max[s] : 頂点 s の子 t の in[t] の最大値（葉なら -1）
	// in_max_all[s] : 頂点 s の子孫 t の in[t] の最大値（葉なら -1）
	vi in_min, in_max, in_max_all;

	// p[s] : 頂点 s の親
	vi p;

	// v[s] : 頂点 s の値
	lazy_segtree<S, op, o, F, act, comp, id> v;

	void bfs_like_dfs_euler_tour(const Graph& g, int rt) {
		// rt から BFS 風 DFS する．
		int time = 0;
		in[rt] = time++;

		function<void(int)> dfs = [&](int s) {
			// まず BFS のように s の子をなぞる．
			in_min[s] = time;
			repe(t, g[s]) {
				if (t == p[s]) continue;
				p[t] = s;
				in[t] = time++;
			}

			// s が葉の場合は何もしない．
			if (time == in_min[s]) {
				in_min[s] = -1;
				return;
			}
			in_max[s] = time - 1;

			// その後で DFS する．
			repe(t, g[s]) {
				if (t == p[s]) continue;
				dfs(t);
			}
			in_max_all[s] = time - 1;
		};
		dfs(rt);
	}

public:
	// rt を根とする根付き木 g と頂点の初期値 o() で初期化する．
	Tree_neighbor_subtree_vertex_query(const Graph& g, int rt)
		: n(sz(g)), rt(rt), in(n, -1), in_min(n, n), in_max(n, -1), in_max_all(n, -1), p(n, -1), v(n)
	{
		bfs_like_dfs_euler_tour(g, rt);
	}

	// rt を根とする根付き木 g と頂点の初期値 a[0..n) で初期化する．
	Tree_neighbor_subtree_vertex_query(const Graph& g, int rt, const vector<S>& a)
		: n(sz(g)), rt(rt), in(n, -1), in_min(n, n), in_max(n, -1), in_max_all(n, -1), p(n, -1)
	{
		// verify : https://yukicoder.me/problems/no/2340

		bfs_like_dfs_euler_tour(g, rt);

		vector<S> ini(n);
		rep(s, n) ini[in[s]] = a[s];
		v = lazy_segtree<S, op, o, F, act, comp, id>(ini);
	}
	Tree_neighbor_subtree_vertex_query() : n(0), rt(-1) {}

	// v[s] = x とする．
	void set(int s, S x) {
		Assert(0 <= s && s < n);

		v.set(in[s], x);
	}

	// v[s] を返す．
	S get(int s) {
		// verify : https://yukicoder.me/problems/no/2340

		Assert(0 <= s && s < n);

		return v.get(in[s]);
	}

	// 頂点 s の子の値の総和を返す．
	S child_sum(int s) {
		Assert(0 <= s && s < n);

		// s が葉の場合は o() を返す．
		if (in_max[s] == -1) return o();

		return v.prod(in_min[s], in_max[s] + 1);
	}

	// 頂点 s からの距離が 1 以下である頂点の値の総和を返す．
	S neighbor_sum(int s) {
		Assert(0 <= s && s < n);

		S res = v.get(in[s]);
		if (p[s] != -1) res = op(res, v.get(in[p[s]]));
		res = op(res, child_sum(s));

		return res;
	}

	// 部分木 s の値の総和を返す．
	S subtree_sum(int s) {
		Assert(0 <= s && s < n);

		S res = v.get(in[s]);
		if (in_min[s] != -1) res = op(res, v.prod(in_min[s], in_max_all[s] + 1));

		return res;
	}

	// 全頂点の値の総和を返す．
	S all_sum() {
		// verify : https://yukicoder.me/problems/no/2618

		return v.all_prod();
	}

	// v[s] に f を作用させる．
	void apply(int s, F f) {
		Assert(0 <= s && s < n);

		v.apply(in[s], f);
	}

	// 頂点 s の子の値に f を作用させる．
	void child_apply(int s, F f) {
		Assert(0 <= s && s < n);

		// s が葉の場合は何もしない．
		if (in_max[s] == -1) return;

		v.apply(in_min[s], in_max[s] + 1, f);
	}

	// 頂点 s からの距離が 1 以下である頂点の値に f を作用させる．
	void neighbor_apply(int s, F f) {
		// verify : https://yukicoder.me/problems/no/2340

		Assert(0 <= s && s < n);

		v.apply(in[s], f);
		if (p[s] != -1) v.apply(in[p[s]], f);
		child_apply(s, f);
	}

	// 部分木 s の値に f を作用させる．
	void subtree_apply(int s, F f) {
		// verify : https://yukicoder.me/problems/no/2340

		Assert(0 <= s && s < n);

		v.apply(in[s], f);
		if (in_min[s] != -1) v.apply(in_min[s], in_max_all[s] + 1, f);
	}

	// 全頂点の値に f を作用させる．
	void all_apply(F f) {
		v.apply(0, n, f);
	}

	// 頂点集合 vs を BFS 風 DFS 昇順にソートする．
	void sort_by_BFS_like_DFS_order(vi& vs) {
		sort(all(vs), [&](int s, int t) {return in[s] < in[t]; });
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Tree_neighbor_subtree_vertex_query TNSVQ) {
		os << TNSVQ.v;
		return os;
	}
#endif
};


//【[部分木,パス]頂点加算／[部分木,パス]頂点総和（Z-加群）】
/*
* Vertex_add_sum_query<S, op, o, inv, mul>(Graph g, int rt) : O(n)
*	rt を根とする根付き木 g と頂点値 v[0..n) = o() で初期化する．
*	要素は Z-加群 (S, op, o, inv, mul) の元とする．
*
* Vertex_add_sum_query<S, op, o, inv, mul>(Graph g, int rt, vS a) : O(n)
*	rt を根とする根付き木 g と頂点値 v[0..n) = a[0..n) で初期化する．
*
* set(int s, S x) : O(log n)
*	v[s] = x とする．
*
* S get(int s) : O(log n)
*	v[s] を返す．
*
* S sum_subtree(int s) : O(log n)
*	部分木 s の頂点の値の総和を返す．
*
* S sum_path(int s, int t) : O((log n)^2)
*	パス s→t 上の頂点（両端含む）の値の総和を返す．
*
* add(int s, S x) : O(log n)
*	v[s] += x とする．
*
* add_subtree(int v, S x) : O(log n)
*	部分木 s の頂点の値に x を加算する．
*
* add_path(int s, int t, S x) : O((log n)^2)
*	パス s→t 上の頂点（両端含む）の値に x を加算する．
*
* 利用：【区間加算フェニック木（Z-加群）】
*/
template <class S, S(*op)(S, S), S(*o)(), S(*inv)(S), S(*mul)(ll, S)>
class Vertex_add_sum_query {
	// 参考：https://qiita.com/Pro_ktmr/items/4e1e051ea0561772afa3

	int n;

	// in[s]  : 根からの DFS で頂点 s に最初に入った時刻
	// out[s] : 根からの DFS で頂点 s から最後に出た時刻
	// top[s] : 頂点 s を含む heavy path の最も浅い頂点
	// wgt[s] : 頂点 s の重さ（部分木 s のもつ辺の数）
	// p[s] : 頂点 s の親
	vi in, out, top, wgt, p;

	// v[i] :  時刻 t に居た頂点の値
	using RASQ = Fenwick_tree_range_add<S, op, o, inv, mul>;
	RASQ v;

	// 各頂点の重さと親を求めるための DFS を行う．
	void dfs1(const Graph& g, int rt) {
		function<void(int)> rf = [&](int s) {
			repe(t, g[s]) {
				if (t == p[s]) continue;
				p[t] = s;
				rf(t);
				wgt[s] += wgt[t] + 1;
			}
		};
		p[rt] = -1;
		rf(rt);
	};

	// 最も重い子を優先して DFS を行う．
	void dfs2(const Graph& g, int rt) {
		int time = 0;

		function<void(int, int)> rf = [&](int s, int tp) {
			in[s] = time;
			top[s] = tp;
			time++;

			// 重さ最大の頂点を得る．
			int w_max = -INF, t_max = -1;
			repe(t, g[s]) {
				if (t == p[s]) continue;
				if (chmax(w_max, wgt[t])) t_max = t;
			}

			// 重さ最大の頂点を優先的になぞる．
			if (t_max != -1) rf(t_max, tp);

			// 残りの頂点をなぞる．
			repe(t, g[s]) {
				if (t == p[s] || t == t_max) continue;
				rf(t, t);
			}

			// s から最後に離れる
			out[s] = time;
		};
		rf(rt, rt);
	}

public:
	// rt を根とする根付き木 g と頂点値 v[0..n) = o で初期化する．
	Vertex_add_sum_query(const Graph& g, int rt) : n(sz(g)), in(n), out(n), top(n), wgt(n), p(n), v(n) {
		// verify : https://www.codechef.com/problems/HEALTHYTREE
		dfs1(g, rt);
		dfs2(g, rt);
	}

	// rt を根とする根付き木 g と頂点値 v[0..n) = a[0..n) で初期化する．
	Vertex_add_sum_query(const Graph& g, int rt, const vector<S>& a) : n(sz(g)), in(n), out(n), top(n), wgt(n), p(n) {
		// verify : https://judge.yosupo.jp/problem/vertex_add_path_sum

		dfs1(g, rt);
		dfs2(g, rt);

		vector<S> ini(n);
		rep(s, n) ini[in[s]] = a[s];
		v = RASQ(ini);
	}
	Vertex_add_sum_query() : n(0) {}

	// v[s] = x とする．
	void set(int s, S x) {
		v.set(in[s], x);
	}

	// v[s] を返す．
	S get(int s) const {
		// verify : https://www.codechef.com/problems/HEALTHYTREE
		
		return v.get(in[s]);
	}

	// 部分木 s の頂点の値の総和を返す．
	S sum_subtree(int s) const {
		// verify : https://judge.yosupo.jp/problem/vertex_add_subtree_sum

		return v.sum(in[s], out[s]);
	}

	// パス s→t 上の頂点（両端含む）の値の総和を返す．
	S sum_path(int s, int t) const {
		// verify : https://judge.yosupo.jp/problem/vertex_add_path_sum

		S res = o();

		// s と t が異なる連結成分に属している限りループを回す．
		while (top[s] != top[t]) {
			// s の方が浅い連結成分に属しているとする．
			if (in[top[s]] > in[top[t]]) swap(s, t);

			// t を含む連結成分は v で並んで配置されているので，
			// 最も浅い頂点 top[t] から t までの範囲の和を求める．
			res = op(res, v.sum(in[top[t]], in[t] + 1));

			// 一つ浅い連結成分に移動する．
			t = p[top[t]];
		}

		// ここまできたら s と t は同じ連結成分に属するので，
		// その間の頂点のみの和を res に加算する．
		if (in[s] > in[t]) swap(s, t);
		res = op(res, v.sum(in[s], in[t] + 1));

		return res;
	}

	// v[s] += x とする．
	void add(int s, S x) {
		// verify : https://judge.yosupo.jp/problem/vertex_add_path_sum

		v.add(in[s], x);
	}

	// 部分木 s の頂点の値に x を加算する．
	void add_subtree(int s, S x) {
		v.add(in[s], out[s], x);
	}

	// パス s→t 上の頂点（両端含む）の値に x を加算する．
	void add_path(int s, int t, S x) {
		// verify : https://www.codechef.com/problems/HEALTHYTREE
				
		// s と t が異なる連結成分に属している限りループを回す．
		while (top[s] != top[t]) {
			// s の方が浅い連結成分に属しているとする．
			if (in[top[s]] > in[top[t]]) swap(s, t);

			// t を含む連結成分は v で並んで配置されているので，
			// 最も浅い頂点 top[t] から t までの範囲に x を加算する．
			v.add(in[top[t]], in[t] + 1, x);

			// 一つ浅い連結成分に移動する．
			t = p[top[t]];
		}

		// ここまできたら s と t は同じ連結成分に属するので，
		// その間の頂点のみに対して x を加算する．
		if (in[s] > in[t]) swap(s, t);
		v.add(in[s], in[t] + 1, x);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Vertex_add_sum_query& q) {
		rep(s, q.n) os << q.get(s) << " ";
		return os;
	}
#endif
};


//【[部分木,パス]辺加算／[部分木,パス]辺総和（Z-加群）】
/*
* Edge_add_sum_query<S, op, o, inv, mul>(Graph g, int rt) : O(n)
*	rt を根とする根付き木 g と辺値 v[0..n) = o() で初期化する．
*	要素は Z-加群 (S, op, o, inv, mul) の元とする．
*
* Edge_add_sum_query<S, op, o, inv, mul>(Graph g, int rt, vS a) : O(n)
*	rt を根とする根付き木 g と辺値 v[0..n) = a[0..n) で初期化する．
*	辺値 v[s] は頂点 s に入る辺の値を表す（v[rt] は無視）
*
* set(int s, S x) : O(log n)
*	頂点 s に入る辺の値を x にする．
*
* S get(int s) : O(log n)
*	頂点 s に入る辺の値を返す．
*
* S sum_subtree(int s) : O(log n)
*	部分木 s の辺の値の総和を返す．
*
* S sum_path(int s, int t) : O((log n)^2)
*	パス s→t 上の辺の値の総和を返す．
*
* add(int s, S x) : O(log n)
*	頂点 s に入る辺の値に x を加算する．
*
* add_subtree(int v, S x) : O(log n)
*	部分木 s の辺の値に x を加算する．
*
* add_path(int s, int t, S x) : O((log n)^2)
*	パス s→t 上の辺の値に x を加算する．
*
* int get_parent(int s) : O(1)
*	s の親を返す．
* 
* 利用：【区間加算フェニック木（Z-加群）】
*/
template <class S, S(*op)(S, S), S(*o)(), S(*inv)(S), S(*mul)(ll, S)>
class Edge_add_sum_query {
	// 参考：https://qiita.com/Pro_ktmr/items/4e1e051ea0561772afa3

	int n;

	// in[s]  : 根からの DFS で頂点 s に最初に入った時刻
	// out[s] : 根からの DFS で頂点 s から最後に出た時刻
	// top[s] : 頂点 s を含む heavy path の最も浅い頂点
	// wgt[s] : 頂点 s の重さ（部分木 s のもつ辺の数）
	// p[s] : 頂点 s の親
	vi in, out, top, wgt, p;

	// v[i] :  時刻 t に居た頂点に入る辺の値
	using RASQ = Fenwick_tree_range_add<S, op, o, inv, mul>;
	RASQ v;

	// 各頂点の重さと親を求めるための DFS を行う．
	void dfs1(const Graph& g, int rt) {
		function<void(int)> rf = [&](int s) {
			repe(t, g[s]) {
				if (t == p[s]) continue;
				p[t] = s;
				rf(t);
				wgt[s] += wgt[t] + 1;
			}
			};
		p[rt] = -1;
		rf(rt);
	};

	// 最も重い子を優先して DFS を行う．
	void dfs2(const Graph& g, int rt) {
		int time = 0;

		function<void(int, int)> rf = [&](int s, int tp) {
			in[s] = time;
			top[s] = tp;
			time++;

			// 重さ最大の頂点を得る．
			int w_max = -INF, t_max = -1;
			repe(t, g[s]) {
				if (t == p[s]) continue;
				if (chmax(w_max, wgt[t])) t_max = t;
			}

			// 重さ最大の頂点を優先的になぞる．
			if (t_max != -1) rf(t_max, tp);

			// 残りの頂点をなぞる．
			repe(t, g[s]) {
				if (t == p[s] || t == t_max) continue;
				rf(t, t);
			}

			// s から最後に離れる
			out[s] = time;
			};
		rf(rt, rt);
	}

public:
	// rt を根とする根付き木 g と辺値 v[0..n) = o() で初期化する．
	Edge_add_sum_query(const Graph& g, int rt) : n(sz(g)), in(n), out(n), top(n), wgt(n), p(n), v(n) {
		// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_5_E

		dfs1(g, rt);
		dfs2(g, rt);

		// 重み付きグラフの場合
		//vector<S> ini(n);
		//rep(s, n) repe(t, g[s]) if (t != p[s]) ini[in[t.to]] = t.cost;
		//v = RASQ(ini);
	}

	// rt を根とする根付き木 g と辺値 v[0..n) = a[0..n) で初期化する．
	Edge_add_sum_query(const Graph& g, int rt, const vector<S>& a) : n(sz(g)), in(n), out(n), top(n), wgt(n), p(n) {
		dfs1(g, rt);
		dfs2(g, rt);

		vector<S> ini(n);
		rep(s, n) ini[in[s]] = a[s];
		v = RASQ(ini);
	}
	Edge_add_sum_query() : n(0) {}

	// 頂点 s に入る辺の値を x にする．
	void set(int s, S x) {
		v.set(in[s], x);
	}

	// 頂点 s に入る辺の値を返す．
	S get(int s) const {
		return v.get(in[s]);
	}

	// 部分木 s の辺の値の総和を返す．
	S sum_subtree(int s) const {
		return v.sum(in[s] + 1, out[s]);
	}

	// パス s→t 上の辺の値の総和を返す．
	S sum_path(int s, int t) const {
		// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_5_E

		S res = o();

		// s と t が異なる連結成分に属している限りループを回す．
		while (top[s] != top[t]) {
			// s の方が浅い連結成分に属しているとする．
			if (in[top[s]] > in[top[t]]) swap(s, t);

			// t を含む連結成分は v で並んで配置されているので，
			// 最も浅い頂点 top[t] から t までの範囲の和を求める．
			res = op(res, v.sum(in[top[t]], in[t] + 1));

			// 一つ浅い連結成分に移動する．
			t = p[top[t]];
		}

		// ここまできたら s と t は同じ連結成分に属するので，
		// その間の辺のみの和を res に加算する．
		if (in[s] > in[t]) swap(s, t);
		res = op(res, v.sum(in[s] + 1, in[t] + 1));

		return res;
	}

	// 頂点 s に入る辺に x を加算する．
	void add(int s, S x) {
		v.add(in[s], x);
	}

	// 部分木 s の辺の値に x を加算する．
	void add_subtree(int s, S x) {
		v.add(in[s] + 1, out[s], x);
	}

	// パス s→t 上の辺の値に x を加算する．
	void add_path(int s, int t, S x) {
		// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_5_E

		// s と t が異なる連結成分に属している限りループを回す．
		while (top[s] != top[t]) {
			// s の方が浅い連結成分に属しているとする．
			if (in[top[s]] > in[top[t]]) swap(s, t);

			// t を含む連結成分は v で並んで配置されているので，
			// 最も浅い頂点 top[t] から t までの範囲に x を加算する．
			v.add(in[top[t]], in[t] + 1, x);

			// 一つ浅い連結成分に移動する．
			t = p[top[t]];
		}

		// ここまできたら s と t は同じ連結成分に属するので，
		// その間の辺のみに対して x を加算する．
		if (in[s] > in[t]) swap(s, t);
		v.add(in[s] + 1, in[t] + 1, x);
	}

	// s の親を返す．
	int get_parent(int s) const {
		Assert(0 <= s && s < n);
		return p[s];
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Edge_add_sum_query& q) {
		rep(s, q.n) os << q.get(s) << " ";
		return os;
	}
#endif
};


//【[1点]頂点作用／[部分木,パス]頂点総積（モノイド）】
/*
* Vertex_prod_query<S, op, e>(Graph g, int rt) : O(n)
*	rt を根とする根付き木 g と頂点値 v[0..n) = e() で初期化する．
*	要素はモノイド (S, op, e) の元とする．
*
* Vertex_prod_query<S, op, e>(Graph g, int rt, vS a) : O(n)
*	rt を根とする根付き木 g と頂点値 v[0..n) = a[0..n) で初期化する．
*
* set(int s, S x) : O(log n)
*	v[s] = x とする．
*
* S get(int s) : O(log n)
*	v[s] を返す．
*
* S prod_path(int s, int t) : O((log n)^2)
*	op(s→t の順に並べた頂点の値) を返す．（両端含む）
*
* S prod_subtree(int s) : O(log n)
*	op(行きがけ順に並べた部分木 s の頂点の値) を返す．
*/
template <class S, S(*op)(S, S), S(*e)()>
class Vertex_prod_query {
	// 参考：https://qiita.com/Pro_ktmr/items/4e1e051ea0561772afa3

	int n;

	// in[s]  : 根からの DFS で頂点 s に最初に入った時刻
	// out[s] : 根からの DFS で頂点 s から最後に出た時刻
	// top[s] : 頂点 s を含む heavy path の最も浅い頂点
	// wgt[s] : 頂点 s の重さ（部分木 s のもつ辺の数）
	// p[s] : 頂点 s の親
	vi in, out, top, wgt, p;

	// v[i] :  時刻 t に居た頂点の値
	using SEG = segtree<S, op, e>;
	SEG v, vR;

	// 各頂点の重さと親を求めるための DFS を行う．
	void dfs1(const Graph& g, int rt) {
		function<void(int)> rf = [&](int s) {
			repe(t, g[s]) {
				if (t == p[s]) continue;
				p[t] = s;
				rf(t);
				wgt[s] += wgt[t] + 1;
			}
			};
		p[rt] = -1;
		rf(rt);
	};

	// 最も重い子を優先して DFS を行う．
	void dfs2(const Graph& g, int rt) {
		int time = 0;

		function<void(int, int)> rf = [&](int s, int tp) {
			in[s] = time;
			top[s] = tp;
			time++;

			// 重さ最大の頂点を得る．
			int w_max = -INF, t_max = -1;
			repe(t, g[s]) {
				if (t == p[s]) continue;
				if (chmax(w_max, wgt[t])) t_max = t;
			}

			// 重さ最大の頂点を優先的になぞる．
			if (t_max != -1) rf(t_max, tp);

			// 残りの頂点をなぞる．
			repe(t, g[s]) {
				if (t == p[s] || t == t_max) continue;
				rf(t, t);
			}

			// s から最後に離れる
			out[s] = time;
			};
		rf(rt, rt);
	}

public:
	// rt を根とする根付き木 g と頂点値 v[0..n) = e() で初期化する．
	Vertex_prod_query(const Graph& g, int rt) : n(sz(g)), in(n), out(n), top(n), wgt(n), p(n), v(n), vR(n) {
		dfs1(g, rt);
		dfs2(g, rt);
	}

	// rt を根とする根付き木 g と頂点値 v[0..n) = a[0..n) で初期化する．
	Vertex_prod_query(const Graph& g, int rt, const vector<S>& a) : n(sz(g)), in(n), out(n), top(n), wgt(n), p(n) {
		// verify : https://judge.yosupo.jp/problem/vertex_set_path_composite

		dfs1(g, rt);
		dfs2(g, rt);

		vector<S> ini(n);
		rep(s, n) ini[in[s]] = a[s];
		v = SEG(ini);

		reverse(all(ini));
		vR = SEG(ini);
	}
	Vertex_prod_query() : n(0) {}

	// 頂点 s の値を x にする．
	void set(int s, S x) {
		// verify : https://judge.yosupo.jp/problem/vertex_set_path_composite

		v.set(in[s], x);
		vR.set((n - 1) - in[s], x);
	}

	// 頂点 s の値を返す．
	S get(int s) const {
		return v.get(in[s]);
	}

	// op(行きがけ順に並べた v の部分木の頂点の値) を返す．
	S prod_subtree(int s) const {
		return v.prod(in[s], out[s]);
	}

	// 頂点 s から t までの頂点（両端含む）の値の積を返す．
	S prod_path(int s, int t) const {
		// verify : https://judge.yosupo.jp/problem/vertex_set_path_composite

		S res = e(), resR = e();

		// s と t が異なる連結成分に属している限りループを回す．
		while (top[s] != top[t]) {
			if (in[top[s]] < in[top[t]]) {
				res = op(v.prod(in[top[t]], in[t] + 1), res);
				t = p[top[t]];
			}
			else {
				resR = op(resR, vR.prod((n - 1) - in[s], (n - 1) - in[top[s]] + 1));
				s = p[top[s]];
			}
		}

		// ここまできたら s と t は同じ連結成分に属する．
		if (in[s] <= in[t]) {
			res = op(v.prod(in[s], in[t] + 1), res);
		}
		else {
			resR = op(resR, vR.prod((n - 1) - in[s], (n - 1) - in[t] + 1));
		}

		return op(resR, res);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Vertex_prod_query& q) {
		rep(s, q.n) os << q.get(s) << " ";
		return os;
	}
#endif
};


//【[1辺]辺作用／[部分木,パス]辺総積（モノイド）】
/*
* Edge_prod_query<S, op, e>(Graph g, int rt) : O(n)
*	rt を根とする根付き木 g と辺値 v[0..n) = e() で初期化する．
*	要素はモノイド (S, op, e) の元とする．
*
* Edge_prod_query<S, op, e>(Graph g, int rt, vS a) : O(n)
*	rt を根とする根付き木 g と辺値 v[0..n) = a[0..n) で初期化する．
*	辺値 v[s] は頂点 s に入る辺の値を表す（v[rt] は無視）
*
* set(int s, S x) : O(log n)
*	頂点 s に入る辺の値を x にする．
*
* S get(int s) : O(log n)
*	頂点 s に入る辺の値を返す．
*
* S prod_path(int s, int t) : O((log n)^2)
*	op(s→t の順に並べた辺の値) を返す．
*
* S prod_subtree(int s) : O(log n)
*	op(行きがけ順に並べた部分木 s の頂辺の値) を返す．
*/
template <class S, S(*op)(S, S), S(*e)()>
class Edge_prod_query {
	// 参考：https://qiita.com/Pro_ktmr/items/4e1e051ea0561772afa3

	int n;

	// in[s]  : 根からの DFS で頂点 s に最初に入った時刻
	// out[s] : 根からの DFS で頂点 s から最後に出た時刻
	// top[s] : 頂点 s を含む heavy path の最も浅い頂点
	// wgt[s] : 頂点 s の重さ（部分木 s のもつ辺の数）
	// p[s] : 頂点 s の親
	vi in, out, top, wgt, p;

	// v[i] :  時刻 t に居た頂点の値
	using SEG = Segtree<S, op, e>;
	SEG v, vR;

	// 各頂点の重さと親を求めるための DFS を行う．
	void dfs1(const Graph& g, int rt) {
		function<void(int)> rf = [&](int s) {
			repe(t, g[s]) {
				if (t == p[s]) continue;
				p[t] = s;
				rf(t);
				wgt[s] += wgt[t] + 1;
			}
			};
		p[rt] = -1;
		rf(rt);
	};

	// 最も重い子を優先して DFS を行う．
	void dfs2(const Graph& g, int rt) {
		int time = 0;

		function<void(int, int)> rf = [&](int s, int tp) {
			in[s] = time;
			top[s] = tp;
			time++;

			// 重さ最大の頂点を得る．
			int w_max = -INF, t_max = -1;
			repe(t, g[s]) {
				if (t == p[s]) continue;
				if (chmax(w_max, wgt[t])) t_max = t;
			}

			// 重さ最大の頂点を優先的になぞる．
			if (t_max != -1) rf(t_max, tp);

			// 残りの頂点をなぞる．
			repe(t, g[s]) {
				if (t == p[s] || t == t_max) continue;
				rf(t, t);
			}

			// s から最後に離れる
			out[s] = time;
			};
		rf(rt, rt);
	}

public:
	// rt を根とする根付き木 g と辺値 v[0..n) = e() で初期化する．
	Edge_prod_query(const Graph& g, int rt) : n(sz(g)), in(n), out(n), top(n), wgt(n), p(n), v(n), vR(n) {
		// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_5_D

		dfs1(g, rt);
		dfs2(g, rt);

		// 重み付きグラフの場合
		//vector<S> ini(n);
		//rep(s, n) repe(t, g[s]) if (t != p[s]) ini[in[t.to]] = t.cost;
		//v = SEG(ini);
		//reverse(all(ini));
		//vR = SEG(ini);
	}

	// rt を根とする根付き木 g と辺値 v[0..n) = a[0..n) で初期化する．
	Edge_prod_query(const Graph& g, int rt, const vector<S>& a) : n(sz(g)), in(n), out(n), top(n), wgt(n), p(n) {
		dfs1(g, rt);
		dfs2(g, rt);

		vector<S> ini(n);
		rep(s, n) ini[in[s]] = a[s];
		v = SEG(ini);

		reverse(all(ini));
		vR = SEG(ini);
	}
	Edge_prod_query() : n(0) {}

	// 頂点 s に入る辺の値を x にする．
	void set(int s, S x) {
		// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_5_D

		v.set(in[s], x);
		vR.set((n - 1) - in[s], x);
	}

	// 頂点 s に入る辺の値を返す．
	S get(int s) const {
		// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_5_D

		return v.get(in[s]);
	}

	// op(行きがけ順に並べた v の部分木の辺の値) を返す．
	S prod_subtree(int s) const {
		return v.prod(in[s], out[s]);
	}

	// 頂点 s から t までの辺の値の積を返す．
	S prod_path(int s, int t) const {
		// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_5_D

		S res = e(), resR = e();

		// s と t が異なる連結成分に属している限りループを回す．
		while (top[s] != top[t]) {
			if (in[top[s]] < in[top[t]]) {
				res = op(v.prod(in[top[t]], in[t] + 1), res);
				t = p[top[t]];
			}
			else {
				resR = op(resR, vR.prod((n - 1) - in[s], (n - 1) - in[top[s]] + 1));
				s = p[top[s]];
			}
		}

		// ここまできたら s と t は同じ連結成分に属する．
		if (in[s] <= in[t]) {
			res = op(v.prod(in[s] + 1, in[t] + 1), res);
		}
		else {
			resR = op(resR, vR.prod((n - 1) - in[s], (n - 1) - in[t]));
		}

		return op(resR, res);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Edge_prod_query& q) {
		rep(s, q.n) os << q.get(s) << " ";
		return os;
	}
#endif
};


//【[部分木,パス]辺作用／[部分木,パス]辺総和（M-可換モノイド）】
/*
* Edge_apply_sum_query<S, op, o, F, act, comp, id>(Graph g, int rt) : O(n)
*	rt を根とする根付き木 g と辺値 v[0..n) = o() で初期化する．
*	要素は M-可換モノイド (S, op, o, F, act, comp, id) の元とする．
*
* Edge_apply_sum_query<S, op, o, F, act, comp, id>(Graph g, int rt, vS a) : O(n)
*	rt を根とする根付き木 g と辺値 v[0..n) = a[0..n) で初期化する．
*	辺値 v[s] は頂点 s に入る辺の値を表す（v[rt] は無視）
*
* set(int s, S x) : O(log n)
*	頂点 s に入る辺の値を x にする．
*
* S get(int s) : O(log n)
*	頂点 s に入る辺の値を返す．
*
* S sum_subtree(int s) : O(log n)
*	部分木 s の辺の値の総和を返す．
*
* S sum_path(int s, int t) : O((log n)^2)
*	パス s→t 上の辺の値の総和を返す．
*
* apply(int s, F f) : O(log n)
*	頂点 s に入る辺の値に f を作用させる．
*
* apply_subtree(int v, F f) : O(log n)
*	部分木 s の辺の値に f を作用させる．
*
* apply_path(int s, int t, F f) : O((log n)^2)
*	パス s→t 上の辺の値に f を作用させる．
*/
template <class S, S(*op)(S, S), S(*o)(), class F, S(*act)(F, S), F(*comp)(F, F), F(*id)()>
class Edge_apply_sum_query {
	// 参考：https://qiita.com/Pro_ktmr/items/4e1e051ea0561772afa3

	int n;

	// in[s]  : 根からの DFS で頂点 s に最初に入った時刻
	// out[s] : 根からの DFS で頂点 s から最後に出た時刻
	// top[s] : 頂点 s を含む heavy path の最も浅い頂点
	// wgt[s] : 頂点 s の重さ（部分木 s のもつ辺の数）
	// p[s] : 頂点 s の親
	vi in, out, top, wgt, p;

	// v[i] :  時刻 t に居た頂点に入る辺の値
	using SEG = lazy_segtree<S, op, o, F, act, comp, id>;
	SEG v;

	// 各頂点の重さと親を求めるための DFS を行う．
	void dfs1(const Graph& g, int rt) {
		function<void(int)> rf = [&](int s) {
			repe(t, g[s]) {
				if (t == p[s]) continue;
				p[t] = s;
				rf(t);
				wgt[s] += wgt[t] + 1;
			}
		};
		p[rt] = -1;
		rf(rt);
	};

	// 最も重い子を優先して DFS を行う．
	void dfs2(const Graph& g, int rt) {
		int time = 0;

		function<void(int, int)> rf = [&](int s, int tp) {
			in[s] = time;
			top[s] = tp;
			time++;

			// 重さ最大の頂点を得る．
			int w_max = -INF, t_max = -1;
			repe(t, g[s]) {
				if (t == p[s]) continue;
				if (chmax(w_max, wgt[t])) t_max = t;
			}

			// 重さ最大の頂点を優先的になぞる．
			if (t_max != -1) rf(t_max, tp);

			// 残りの頂点をなぞる．
			repe(t, g[s]) {
				if (t == p[s] || t == t_max) continue;
				rf(t, t);
			}

			// s から最後に離れる
			out[s] = time;
		};
		rf(rt, rt);
	}

public:
	// rt を根とする根付き木 g と辺値 v[0..n) = o() で初期化する．
	Edge_apply_sum_query(const Graph& g, int rt) : n(sz(g)), in(n), out(n), top(n), wgt(n), p(n), v(n) {
		dfs1(g, rt);
		dfs2(g, rt);

		// 重み付きグラフの場合
		//vector<S> ini(n);
		//rep(s, n) repe(t, g[s]) if (t != p[s]) ini[in[t.to]] = t.cost;
		//v = SEG(ini);
	}

	// rt を根とする根付き木 g と辺値 v[0..n) = a[0..n) で初期化する．
	Edge_apply_sum_query(const Graph& g, int rt, const vector<S>& a) : n(sz(g)), in(n), out(n), top(n), wgt(n), p(n) {
		dfs1(g, rt);
		dfs2(g, rt);

		vector<S> ini(n);
		rep(s, n) ini[in[s]] = a[s];
		v = SEG(ini);
	}
	Edge_apply_sum_query() : n(0) {}

	// 頂点 s に入る辺の値を x にする．
	void set(int s, S x) {
		v.set(in[s], x);
	}

	// 頂点 s に入る辺の値を返す．
	S get(int s) {
		return v.get(in[s]);
	}

	// 部分木 s の辺の値の総和を返す．
	S sum_subtree(int s) {
		return v.prod(in[s] + 1, out[s]);
	}

	// パス s→t 上の辺の値の総和を返す．
	S sum_path(int s, int t) {
		// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_5_E

		S res = o();

		// s と t が異なる連結成分に属している限りループを回す．
		while (top[s] != top[t]) {
			// s の方が浅い連結成分に属しているとする．
			if (in[top[s]] > in[top[t]]) swap(s, t);

			// t を含む連結成分は v で並んで配置されているので，
			// 最も浅い頂点 top[t] から t までの範囲の和を求める．
			res = op(res, v.prod(in[top[t]], in[t] + 1));

			// 一つ浅い連結成分に移動する．
			t = p[top[t]];
		}

		// ここまできたら s と t は同じ連結成分に属するので，
		// その間の辺のみの和を res に加算する．
		if (in[s] > in[t]) swap(s, t);
		res = op(res, v.prod(in[s] + 1, in[t] + 1));

		return res;
	}

	// 頂点 s に入る辺に f を作用させる．
	void apply(int s, F f) {
		v.apply(in[s], f);
	}

	// 部分木 s の辺の値に f を作用させる．
	void apply_subtree(int s, F f) {
		v.apply(in[s] + 1, out[s], f);
	}

	// パス s→t 上の辺の値に f を作用させる．
	void apply_path(int s, int t, F f) {
		// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_5_E

		// s と t が異なる連結成分に属している限りループを回す．
		while (top[s] != top[t]) {
			// s の方が浅い連結成分に属しているとする．
			if (in[top[s]] > in[top[t]]) swap(s, t);

			// t を含む連結成分は v で並んで配置されている．
			v.apply(in[top[t]], in[t] + 1, f);

			// 一つ浅い連結成分に移動する．
			t = p[top[t]];
		}

		// ここまできたら s と t は同じ連結成分に属する．
		if (in[s] > in[t]) swap(s, t);
		v.apply(in[s] + 1, in[t] + 1, f);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Edge_apply_sum_query& q) {
		rep(s, q.n) os << q.get(s) << " ";
		return os;
	}
#endif
};


//【[隣接,部分木,パス]頂点作用／[隣接,部分木,パス]頂点総和（M-可換モノイド）】
/*
* Vertex_Verious_apply_sum_query<S, op, o, F, act, comp, id>(Graph g, int rt) : O(n)
*	rt を根とする根付き木 g と頂点値 v[0..n) = o() で初期化する．
*	要素は M-可換モノイド (S, op, o, F, act, comp, id) の元とする．
*
* Vertex_Verious_apply_sum_query<S, op, o, F, act, comp, id>(Graph g, int rt, vS a) : O(n)
*	rt を根とする根付き木 g と頂点値 v[0..n) = a[0..n) で初期化する．
*
* set(int s, S x) : O(log n)
*	v[s] = x とする．
*
* S get(int s) : O(log n)
*	v[s] を返す．
*
* S sum_child(int s) : O(log n)
*	頂点 s の子の値の総和を返す．
*
* S sum_neighbor(int s) : O(log n)
*	頂点 s からの距離が 1 以下である頂点の値の総和を返す．
*
* S sum_subtree(int s) : O(log n)
*	部分木 s の頂点の値の総和を返す．
* 
* S sum_path(int s, int t) : O((log n)^2)
*	パス s-t 上の頂点（両端含む）の値の総和を返す．
*
* S sum_all() : O(1)
*	全頂点の値の総和を返す．
*
* apply(int s, F f) : O(log n)
*	v[s] に f を作用させる．
*
* apply_child(int s, F f) : O(log n)
*	頂点 s の子の値に f を作用させる．
*
* apply_neighbor(int s, F f) : O(log n)
*	頂点 s からの距離が 1 以下である頂点の値に f を作用させる．
*
* apply_subtree(int s, F f) : O(log n)
*	部分木 s の頂点の値に f を作用させる．
* 
* apply_path(int s, int t, F f) : O((log n)^2)
*	パス s-t 上の頂点（両端含む）の値に f を作用させる．
*
* apply_all(F f) : O(log n)
*	全頂点の値に f を作用させる．
*/
template <class S, S(*op)(S, S), S(*o)(), class F, S(*act)(F, S), F(*comp)(F, F), F(*id)()>
class Vertex_Verious_apply_sum_query {
	int n;

	// in[s]  : 根からの DFS で頂点 s に最初に入った時刻
	// out[s] : 根からの DFS で頂点 s から最後に出た時刻
	// top[s] : 頂点 s を含む heavy path の最も浅い頂点
	// wgt[s] : 頂点 s の重さ（部分木 s のもつ辺の数）
	// p[s] : 頂点 s の親
	// hch_in[s] : 頂点 s の heavy child の in
	// lch_inl[s] : 頂点 s の light childs の in の最小値
	// lch_inr[s] : 頂点 s の light childs の in の最大値 - 1
	vi in, out, top, wgt, p, hch_in, lch_inl, lch_inr;

	// seg[t] : 時刻 t に居た頂点の値
	using SEG = lazy_segtree<S, op, o, F, act, comp, id>;
	SEG v;

	// 各頂点の重さと親を求めるための DFS を行う．
	void dfs1(const Graph& g, int rt) {
		function<void(int)> rf = [&](int s) {
			repe(t, g[s]) {
				if (t == p[s]) continue;
				p[t] = s;
				rf(t);
				wgt[s] += wgt[t] + 1;
			}
		};
		p[rt] = -1;
		rf(rt);
	};

	// 最も重い子とその子孫 → 子 → 子孫 の優先順位で DFS を行う．
	void dfs2(const Graph& g, int rt) {
		int time = 1;
		function<void(int, int)> rf = [&](int s, int tp) {
			top[s] = tp;

			// 重さ最大の頂点を得る．
			int w_max = -INF, t_max = -1;
			repe(t, g[s]) {
				if (t == p[s]) continue;
				if (chmax(w_max, wgt[t])) t_max = t;
			}

			// s が葉の場合は何もしない．
			if (t_max == -1) {
				hch_in[s] = -1;
				lch_inl[s] = time;
				lch_inr[s] = time;
				out[s] = time;
				return;
			}

			// 重さ最大の頂点だけ先に DFS する．
			in[t_max] = time;
			hch_in[s] = time;
			time++;
			rf(t_max, tp);

			// その他の子を BFS する．
			lch_inl[s] = time;
			repe(t, g[s]) {
				if (t == p[s] || t == t_max) continue;
				in[t] = time;
				time++;
			}
			lch_inr[s] = time;

			// 残りの子孫を DFS する．
			repe(t, g[s]) {
				if (t == p[s] || t == t_max) continue;
				rf(t, t);
			}

			// s から最後に離れる
			out[s] = time;
		};
		rf(rt, rt);
	}

public:
	// rt を根とする根付き木 g と値 o() で初期化する．
	Vertex_Verious_apply_sum_query(const Graph& g, int rt) : n(sz(g)), in(n), out(n), top(n),
		wgt(n), p(n), hch_in(n), lch_inl(n), lch_inr(n), v(n)
	{
		dfs1(g, rt);
		dfs2(g, rt);
	}

	// rt を根とする根付き木 g と値 a[0..n) で初期化する．
	Vertex_Verious_apply_sum_query(const Graph& g, int rt, const vector<S>& a) : n(sz(g)), in(n), out(n), top(n),
		wgt(n), p(n), hch_in(n), lch_inl(n), lch_inr(n)
	{
		// verify : https://yukicoder.me/problems/no/2341

		dfs1(g, rt);
		dfs2(g, rt);
		vector<S> ini(n);
		rep(s, n) ini[in[s]] = a[s];
		v = SEG(ini);
	}
	Vertex_Verious_apply_sum_query() : n(0) {}

	// v[s] = x とする．
	void set(int s, S x) {
		Assert(0 <= s && s < n);

		v.set(in[s], x);
	}

	// v[s] を返す．
	S get(int s) {
		// verify : https://yukicoder.me/problems/no/2341

		Assert(0 <= s && s < n);

		return v.get(in[s]);
	}

	// 頂点 s の子の値の総和を返す．
	S sum_child(int s) {
		Assert(0 <= s && s < n);

		S res = o();
		if (hch_in[s] != -1) res = v.get(hch_in[s]);
		res = op(res, v.prod(lch_inl[s], lch_inr[s]));

		return res;
	}

	// 頂点 s からの距離が 1 以下である頂点の値の総和を返す．
	S sum_neighbor(int s) {
		Assert(0 <= s && s < n);

		S res = v.get(in[s]);
		if (p[s] != -1) res = op(res, v.get(in[p[s]]));
		res = op(res, sum_child(s));

		return res;
	}

	// 部分木 s の値の総和を返す．
	S sum_subtree(int s) {
		Assert(0 <= s && s < n);

		S res = v.get(in[s]);
		if (hch_in[s] != -1) res = op(res, v.prod(hch_in[s], out[s]));

		return res;
	}

	// パス s→t 上の頂点（両端含む）の総和を返す．
	S sum_path(int s, int t) {
		// verify : https://yukicoder.me/problems/10671

		Assert(0 <= s && s < n && 0 <= t && t < n);

		S res = o();

		// s と t が異なる heavy path に属している限りループを回す．
		while (top[s] != top[t]) {
			// s の方が浅い連結成分に属しているとする．
			if (in[top[s]] > in[top[t]]) swap(s, t);

			// heavy path の最も浅い頂点 top[t] から t までの範囲の和を加算する．
			res = op(res, v.get(in[top[t]]));
			if (t != top[t] && hch_in[top[t]] != -1) res = op(res, v.prod(hch_in[top[t]], in[t] + 1));

			// 一つ浅い連結成分に移動する．
			t = p[top[t]];
		}

		// ここまできたら s と t は同じ heavy path に属するのでその間の頂点の和を加算する．
		if (in[s] > in[t]) swap(s, t);
		if (top[s] == s) {
			res = op(res, v.get(in[s]));
			if (s != t && hch_in[s] != -1) res = op(res, v.prod(hch_in[s], in[t] + 1));
		}
		else {
			res = op(res, v.prod(in[s], in[t] + 1));
		}

		return res;
	}

	// 全頂点の値の総和を返す．
	S sum_all() {
		return v.all_prod();
	}

	// v[s] に f を作用させる．
	void apply(int s, F f) {
		Assert(0 <= s && s < n);

		v.apply(in[s], f);
	}

	// 頂点 s の子の値に f を作用させる．
	void apply_child(int s, F f) {
		Assert(0 <= s && s < n);

		if (hch_in[s] != -1) v.apply(hch_in[s], f);
		v.apply(lch_inl[s], lch_inr[s], f);
	}

	// 頂点 s からの距離が 1 以下である頂点の値に f を作用させる．
	void apply_neighbor(int s, F f) {
		// verify : https://yukicoder.me/problems/no/2341

		Assert(0 <= s && s < n);

		v.apply(in[s], f);
		if (p[s] != -1) v.apply(in[p[s]], f);
		apply_child(s, f);
	}

	// 部分木 s の値に f を作用させる．
	void apply_subtree(int s, F f) {
		// verify : https://yukicoder.me/problems/no/2341

		Assert(0 <= s && s < n);

		v.apply(in[s], f);
		if (hch_in[s] != -1) v.apply(hch_in[s], out[s], f);
	}

	// パス s→t 上の頂点（両端含む）の値に f を作用させる．
	void apply_path(int s, int t, F f) {
		// verify : https://yukicoder.me/problems/no/2341

		Assert(0 <= s && s < n && 0 <= t && t < n);

		// s と t が異なる heavy path に属している限りループを回す．
		while (top[s] != top[t]) {
			// s の方が浅い連結成分に属しているとする．
			if (in[top[s]] > in[top[t]]) swap(s, t);

			// heavy path の最も浅い頂点 top[t] から t までに f を作用させる．
			v.apply(in[top[t]], f);
			if (t != top[t] && hch_in[top[t]] != -1) v.apply(hch_in[top[t]], in[t] + 1, f);

			// 一つ浅い連結成分に移動する．
			t = p[top[t]];
		}

		// ここまできたら s と t は同じ heavy path に属するのでその間の頂点に f を作用させる．
		if (in[s] > in[t]) swap(s, t);
		if (top[s] == s) {
			v.apply(in[s], f);
			if (s != t && hch_in[s] != -1) v.apply(hch_in[s], in[t] + 1, f);
		}
		else {
			v.apply(in[s], in[t] + 1, f);
		}
	}

	// 全頂点の値に f を作用させる．
	void apply_all(F f) {
		v.apply(0, n, f);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Vertex_Verious_apply_sum_query Q) {
		rep(i, Q.n) os << Q.get(i) << " ";
		return os;
	}
#endif
};


//【[隣接,部分木,パス]頂点作用／[隣接,部分木,パス]頂点総積（M-モノイド）】
/*
* Verious_apply_sum_query<S, op, e, F, act, comp, id>(Graph g, int rt) : O(n)
*	rt を根とする根付き木 g と頂点値 v[0..n) = e で初期化する．
*	要素は M-モノイド (S, op, e, F, act, comp, id) の元とする．
*
* Verious_apply_sum_query<S, op, e, F, act, comp, id>(Graph g, int rt, vS a) : O(n)
*	rt を根とする根付き木 g と頂点値 v[0..n) = a[0..n) で初期化する．
*
* set(int s, S x) : O(log n)
*	v[s] = x とする．
*
* S get(int s) : O(log n)
*	v[s] を返す．
*
* S prod_child(int s) : O(log n)
*	頂点 s の子の値の総積を返す．
*
* S prod_neighbor(int s) : O(log n)
*	頂点 s からの距離が 1 以下である頂点の値の総積を返す．
*
* S prod_subtree(int s) : O(log n)
*	部分木 s の頂点の値の総積を返す．
*
* S prod_path(int s, int t) : O((log n)^2)
*	パス s→t 上の頂点（両端含む）のこの順での総積を返す．
*
* S prod_all() : O(1)
*	全頂点の値の総積を返す．
*
* apply(int s, F f) : O(log n)
*	v[s] に f を作用させる．
*
* apply_child(int s, F f) : O(log n)
*	頂点 s の子の値に f を作用させる．
*
* apply_neighbor(int s, F f) : O(log n)
*	頂点 s からの距離が 1 以下である頂点の値に f を作用させる．
*
* apply_subtree(int s, F f) : O(log n)
*	部分木 s の頂点の値に f を作用させる．
*
* apply_path(int s, int t, F f) : O((log n)^2)
*	パス s→t 上の頂点（両端含む）の値に f を作用させる．
*
* apply_all(F f) : O(log n)
*	全頂点の値に f を作用させる．
*/
template <class S, S(*op)(S, S), S(*e)(), class F, S(*act)(F, S), F(*comp)(F, F), F(*id)()>
class Vertex_Verious_apply_prod_query {
	int n;

	// in[s]  : 根からの DFS で頂点 s に最初に入った時刻
	// out[s] : 根からの DFS で頂点 s から最後に出た時刻
	// top[s] : 頂点 s を含む heavy path の最も浅い頂点
	// wgt[s] : 頂点 s の重さ（部分木 s のもつ辺の数）
	// p[s] : 頂点 s の親
	// hch_in[s] : 頂点 s の heavy child の in
	// lch_inl[s] : 頂点 s の light childs の in の最小値
	// lch_inr[s] : 頂点 s の light childs の in の最大値 - 1
	vi in, out, top, wgt, p, hch_in, lch_inl, lch_inr;

	// seg[t] : 時刻 t に居た頂点の値
	using SEG = lazy_segtree<S, op, e, F, act, comp, id>;
	SEG seg, seg_rev;

	// 各頂点の重さと親を求めるための DFS を行う．
	void dfs1(const Graph& g, int rt) {
		function<void(int)> rf = [&](int s) {
			repe(t, g[s]) {
				if (t == p[s]) continue;
				p[t] = s;
				rf(t);
				wgt[s] += wgt[t] + 1;
			}
		};
		p[rt] = -1;
		rf(rt);
	};

	// 最も重い子とその子孫 → 子 → 子孫 の優先順位で DFS を行う．
	void dfs2(const Graph& g, int rt) {
		int time = 1;
		function<void(int, int)> rf = [&](int s, int tp) {
			top[s] = tp;

			// 重さ最大の頂点を得る．
			int w_max = -INF, t_max = -1;
			repe(t, g[s]) {
				if (t == p[s]) continue;
				if (chmax(w_max, wgt[t])) t_max = t;
			}

			// s が葉の場合は何もしない．
			if (t_max == -1) {
				hch_in[s] = -1;
				lch_inl[s] = time;
				lch_inr[s] = time;
				out[s] = time;
				return;
			}

			// 重さ最大の頂点だけ先に DFS する．
			in[t_max] = time;
			hch_in[s] = time;
			time++;
			rf(t_max, tp);

			// その他の子を BFS する．
			lch_inl[s] = time;
			repe(t, g[s]) {
				if (t == p[s] || t == t_max) continue;
				in[t] = time;
				time++;
			}
			lch_inr[s] = time;

			// 残りの子孫を DFS する．
			repe(t, g[s]) {
				if (t == p[s] || t == t_max) continue;
				rf(t, t);
			}

			// s から最後に離れる
			out[s] = time;
		};
		rf(rt, rt);
	}

public:
	// rt を根とする根付き木 g と値 o で初期化する．
	Vertex_Verious_apply_prod_query(const Graph& g, int rt) : n(sz(g)), in(n), out(n), top(n),
		wgt(n), p(n), hch_in(n), lch_inl(n), lch_inr(n), seg(n), seg_rev(n)
	{
		dfs1(g, rt);
		dfs2(g, rt);
	}

	// rt を根とする根付き木 g と値 a[0..n) で初期化する．
	Vertex_Verious_apply_prod_query(const Graph& g, int rt, const vector<S>& a) : n(sz(g)), in(n), out(n), top(n),
		wgt(n), p(n), hch_in(n), lch_inl(n), lch_inr(n)
	{
		// verify : https://yukicoder.me/problems/no/2341

		dfs1(g, rt);
		dfs2(g, rt);
		vector<S> ini(n);
		rep(s, n) ini[in[s]] = a[s];
		seg = SEG(ini);
		reverse(all(ini));
		seg_rev = SEG(ini);
	}
	Vertex_Verious_apply_prod_query() : n(0) {}

	// v[s] = x とする．
	void set(int s, S x) {
		Assert(0 <= s && s < n);

		seg.set(in[s], x);
		seg_rev.set((n - 1) - in[s], x);
	}

	// v[s] を返す．
	S get(int s) {
		// verify : https://yukicoder.me/problems/no/2341

		Assert(0 <= s && s < n);

		return seg.get(in[s]);
	}

	// 頂点 s の子の値の総積を返す．
	S prod_child(int s) {
		Assert(0 <= s && s < n);

		S res = e();
		if (hch_in[s] != -1) res = seg.get(hch_in[s]);
		res = op(res, seg.prod(lch_inl[s], lch_inr[s]));

		return res;
	}

	// 頂点 s からの距離が 1 以下である頂点の値の総積を返す．
	S prod_neighbor(int s) {
		Assert(0 <= s && s < n);

		S res = seg.get(in[s]);
		if (p[s] != -1) res = op(res, seg.get(in[p[s]]));
		res = op(res, prod_child(s));

		return res;
	}

	// 部分木 s の値の総積を返す．
	S prod_subtree(int s) {
		// verify : https://judge.yosupo.jp/problem/vertex_add_subtree_sum

		Assert(0 <= s && s < n);

		S res = seg.get(in[s]);
		if (hch_in[s] != -1) res = op(res, seg.prod(hch_in[s], out[s]));

		return res;
	}

	// パス s→t 上の頂点（両端含む）のこの順での総積を返す．
	S prod_path(int s, int t) {
		// verify : https://judge.yosupo.jp/problem/vertex_set_path_composite

		Assert(0 <= s && s < n && 0 <= t && t < n);

		S res = e(), res_rev = e();

		// s と t が異なる heavy path に属している限りループを回す．
		while (top[s] != top[t]) {
			if (in[top[s]] < in[top[t]]) {
				// heavy path の最も浅い頂点 top[t] から t までの範囲の積を乗算する．
				if (t != top[t] && hch_in[top[t]] != -1) {
					res = op(seg.prod(hch_in[top[t]], in[t] + 1), res);
				}
				res = op(seg.get(in[top[t]]), res);
				t = p[top[t]];
			}
			else {
				// heavy path の最も浅い頂点 top[s] から s までの範囲の積を乗算する．
				if (s != top[s] && hch_in[top[s]] != -1) {
					res_rev = op(res_rev, seg_rev.prod((n - 1) - in[s], (n - 1) - hch_in[top[s]] + 1));
				}
				res_rev = op(res_rev, seg_rev.get((n - 1) - in[top[s]]));
				s = p[top[s]];
			}
		}

		// ここまできたら s と t は同じ heavy path に属するのでその間の頂点の積を乗算する．
		if (in[s] <= in[t]) {
			if (top[s] == s) {
				if (s != t && hch_in[s] != -1) {
					res = op(seg.prod(hch_in[s], in[t] + 1), res);
				}
				res = op(seg.get(in[s]), res);
			}
			else {
				res = op(seg.prod(in[s], in[t] + 1), res);
			}
		}
		else {
			if (top[t] == t) {
				if (hch_in[t] != -1) {
					res_rev = op(res_rev, seg_rev.prod((n - 1) - in[s], (n - 1) - hch_in[t] + 1));
				}
				res_rev = op(res_rev, seg_rev.get((n - 1) - in[t]));
			}
			else {
				res_rev = op(res_rev, seg_rev.prod((n - 1) - in[s], (n - 1) - in[t] + 1));
			}
		}

		return op(res_rev, res);
	}

	// 全頂点の値の総積を返す．
	S prod_all() {
		return seg.all_prod();
	}

	// v[s] に f を作用させる．
	void apply(int s, F f) {
		// verify : https://judge.yosupo.jp/problem/vertex_add_subtree_sum

		Assert(0 <= s && s < n);

		seg.apply(in[s], f);
		seg_rev.apply((n - 1) - in[s], f);
	}

	// 頂点 s の子の値に f を作用させる．
	void apply_child(int s, F f) {
		Assert(0 <= s && s < n);

		if (hch_in[s] != -1) {
			seg.apply(hch_in[s], f);
			seg_rev.apply((n - 1) - hch_in[s], f);
		}
		seg.apply(lch_inl[s], lch_inr[s], f);
		seg_rev.apply(n - lch_inr[s], n - lch_inl[s], f);
	}

	// 頂点 s からの距離が 1 以下である頂点の値に f を作用させる．
	void apply_neighbor(int s, F f) {
		// verify : https://yukicoder.me/problems/no/2341

		Assert(0 <= s && s < n);

		seg.apply(in[s], f);
		seg_rev.apply((n - 1) - in[s], f);
		if (p[s] != -1) {
			seg.apply(in[p[s]], f);
			seg_rev.apply((n - 1) - in[p[s]], f);
		}
		apply_child(s, f);
	}

	// 部分木 s の値に f を作用させる．
	void apply_subtree(int s, F f) {
		// verify : https://yukicoder.me/problems/no/2341

		Assert(0 <= s && s < n);

		seg.apply(in[s], f);
		seg_rev.apply((n - 1) - in[s], f);
		if (hch_in[s] != -1) {
			seg.apply(hch_in[s], out[s], f);
			seg_rev.apply(n - out[s], n - hch_in[s], f);
		}
	}

	// パス s→t 上の頂点（両端含む）の値に f を作用させる．
	void apply_path(int s, int t, F f) {
		// verify : https://yukicoder.me/problems/no/2341

		Assert(0 <= s && s < n && 0 <= t && t < n);

		// s と t が異なる heavy path に属している限りループを回す．
		while (top[s] != top[t]) {
			// s の方が浅い連結成分に属しているとする．
			if (in[top[s]] > in[top[t]]) swap(s, t);

			// heavy path の最も浅い頂点 top[t] から t までに f を作用させる．
			seg.apply(in[top[t]], f);
			seg_rev.apply((n - 1) - in[top[t]], f);
			if (t != top[t] && hch_in[top[t]] != -1) {
				seg.apply(hch_in[top[t]], in[t] + 1, f);
				seg_rev.apply((n - 1) - in[t], (n - 1) - hch_in[top[t]] + 1, f);
			}

			// 一つ浅い連結成分に移動する．
			t = p[top[t]];
		}

		// ここまできたら s と t は同じ heavy path に属するのでその間の頂点に f を作用させる．
		if (in[s] > in[t]) swap(s, t);
		if (top[s] == s) {
			seg.apply(in[s], f);
			seg_rev.apply((n - 1) - in[s], f);
			if (s != t && hch_in[s] != -1) {
				seg.apply(hch_in[s], in[t] + 1, f);
				seg_rev.apply((n - 1) - in[t], (n - 1) - hch_in[s] + 1, f);
			}
		}
		else {
			seg.apply(in[s], in[t] + 1, f);
			seg_rev.apply((n - 1) - in[t], (n - 1) - in[s] + 1, f);
		}
	}

	// 全頂点の値に f を作用させる．
	void apply_all(F f) {
		seg.apply(0, n, f);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Vertex_Verious_apply_prod_query G) {
		rep(i, G.n) os << G.get(i) << " ";
		return os;
	}
#endif
};


//【DSU on Tree】O(n log n β + Q)
/*
* 根を rt とする木 g について，各頂点を根とする部分木に対するクエリを一括で処理する．
*
* insert(int s) : O(β)
*	部分木に頂点 s を追加し，データ構造を更新する．
*
* erase(int s) : O(β)
*	部分木から頂点 s を削除し，データ構造を更新する．
*
* get_sol(int s) : 計 O(Q)
*	部分木 s に対し，データ構造を参照して解を求める．
*/
template <class F_INS, class F_ERS, class F_SOL>
void dsu_on_tree(const Graph& g, int rt, const F_INS& insert, const F_ERS& erase, const F_SOL& get_sol) {
	// 参考 : https://nyaannyaan.github.io/library/tree/dsu-on-tree.hpp.html

	int n = sz(g);

	// hc[s] : 頂点 s の重さ最大の子
	vi hc(n, -1);

	// 各頂点の重さ最大の子を求める．
	function<int(int, int)> dfs_wgt = [&](int s, int p) {
		int ws = 0; int wt_max = -1;
		repe(t, g[s]) {
			if (t == p) continue;
			int wt = dfs_wgt(t, s);
			if (chmax(wt_max, wt)) hc[s] = t;
			ws += wt + 1;
		}
		return ws;
	};
	dfs_wgt(rt, -1);

	// 部分木 s 内の頂点を全て追加する．
	function<void(int, int)> dfs_insert = [&](int s, int p) {
		insert(s);
		repe(t, g[s]) {
			if (t == p) continue;
			dfs_insert(t, s);
		}
	};

	// 部分木 s 内の頂点を全て削除する．
	function<void(int, int)> dfs_erase = [&](int s, int p) {
		erase(s);
		repe(t, g[s]) {
			if (t == p) continue;
			dfs_erase(t, s);
		}
	};

	// 部分木 s 内の頂点を全て追加し，部分木 s に対するクエリに答える．
	// 必ず頂点集合が空の状態で呼ばれ，keep = false なら頂点集合を空に戻して関数を抜ける．
	function<void(int, int, bool)> dfs = [&](int s, int p, bool keep) {
		// light edge の先の部分木全てについての計算を行う．
		// 計算後は頂点集合は，空である．
		repe(t, g[s]) {
			if (t == p || t == hc[s]) continue;
			dfs(t, s, false);
		}

		// heavy edge の先の部分木についての計算を行う．
		// 計算後の頂点集合は，部分木 hc[s] 内の頂点全てである．
		if (hc[s] != -1) dfs(hc[s], s, true);

		// light edge の先の頂点全てと s 自身を追加する．
		// 計算後の頂点集合は，部分木 s 内の頂点全てである．
		repe(t, g[s]) {
			if (t == p || t == hc[s]) continue;
			dfs_insert(t, s);
		}
		insert(s);

		// 部分木 s に対するクエリに答える．
		get_sol(s);

		// keep フラグが false なら部分木 s 内の頂点を全て削除し，頂点集合を空にする．
		if (!keep) dfs_erase(s, p);
	};
	dfs(rt, -1, true);

	/* 雛形
	// freq[c] : 色 c の頂点の個数
	vi freq(n + 1);
	int freq_max = 0;

	// sum[f] : f 個ある色の色番号の和
	vl sum(n + 1);
	sum[0] = (ll)n * (n + 1) / 2;

	auto insert_vtx = [&](int s) {
		int col = c[s];
		int f = freq[col], nf = f + 1;

		sum[f] -= col;
		sum[nf] += col;

		freq[col] = nf;
		chmax(freq_max, nf);
	};

	auto erase_vtx = [&](int s) {
		int col = c[s];
		int f = freq[col], nf = f - 1;

		sum[f] -= col;
		sum[nf] += col;

		freq[col] = nf;
		if (sum[freq_max] == 0) freq_max = nf;
	};

	vl res(n);
	auto get_sol = [&](int s) {
		res[s] = sum[freq_max];
	};

	dsu_on_tree(g, 0, insert_vtx, erase_vtx, get_sol);
	*/
}


//【Mo's algorithm（パスクエリ，辺）】O(n√q α + q log q)
/*
* 与えられた n 頂点の重み付き無向木 g に対し，各 j∈[0..q) について，
* u[j] と v[j] を結ぶパスクエリに対する解を順に格納したリストを返す．
*
* 制約：任意箇所の辺の追加[削除] が O(α) で可能
*/
template <class S>
vector<S> mos_algorithm_path_edge(const WGraph& g, const vi& u, const vi& v) {
	// 参考 : https://ei1333.hateblo.jp/entry/2017/09/11/211011

	int n = sz(g), q = sz(u);
	int sqrt_q = (int)(sqrt(q) + 1e-12);
	int width = max((n + sqrt_q - 1) / sqrt_q, 1);
	vector<S> res(q);

	// 便宜上 g を 0 を根とする根付き木とみなす．
	//	in[s] : オイラーツアーで最初に頂点 s を訪れた時刻（根なら 0）
	//	w[t] : t 番目に通った辺の重み
	//	c[t] : t 番目に通った辺の両端の頂点のうち子の方の番号（辺の識別用）
	vi in(n); vl w(2 * (n - 1)); vi c(2 * (n - 1));

	// 0 を根とする根付き木とみなしてオイラーツアーを行う．
	int time = 0;
	function<void(int, int)> rf = [&](int s, int p) {
		in[s] = time;
		time++;

		repe(t, g[s]) {
			if (t == p) continue;

			w[time - 1] = t.cost, c[time - 1] = t;
			rf(t, s);
			w[time - 1] = t.cost, c[time - 1] = t;
			time++;
		}
	};
	rf(0, -1);

	// クエリを左端の位置するブロックについて昇順に，
	// 次いで右端を偶数番目のブロックは昇順，奇数番目のブロックは降順でソートする．
	vi l(q), r(q);
	vector<tuple<int, int, int>> lb_sr_j(q);
	rep(j, q) {
		l[j] = in[u[j]];
		r[j] = in[v[j]];
		if (l[j] > r[j]) swap(l[j], r[j]);

		int b = l[j] / width;
		lb_sr_j[j] = { b, (b % 2 == 0 ? 1 : -1) * r[j], j };
	}
	sort(all(lb_sr_j));

	// ----------------------- ここを実装する ------------------------

	// 必要なデータ構造を用意する．
	vl freq((int)2e5 + 10);
	S sol = 0;

	// 区間に要素 w を追加し，データ構造を更新する．
	auto insert = [&](ll w) {
		if (freq[w] == 0) sol++;
		freq[w]++;
	};

	// 区間から要素 w を削除し，データ構造を更新する．
	auto erase = [&](ll w) {
		freq[w]--;
		if (freq[w] == 0) sol--;
	};

	// クエリ j に対し，データ構造を参照して解を求める．
	auto get_sol = [&](int j) {
		return sol;
	};

	// --------------------------------------------------------------

	// exist[i] : 子 i をもつ辺がデータ構造に含まれているか
	vi exist(n);

	// 辺の有無を切り替える．
	auto flip = [&](int t) {
		if (exist[c[t]]) erase(w[t]);
		else insert(w[t]);

		exist[c[t]] ^= 1;
	};

	// lpt[rpt] : 半開区間の左[右] 端の位置
	int lpt = 0, rpt = 0;

	// クエリを順に処理していく．
	rep(tmp, q) {
		int j = get<2>(lb_sr_j[tmp]);

		// 区間を広げる．
		while (lpt > l[j]) flip(--lpt);
		while (rpt < r[j]) flip(rpt++);

		// 区間を狭める．
		while (lpt < l[j]) flip(lpt++);
		while (rpt > r[j]) flip(--rpt);

		// クエリ j に対する解を得る．
		res[j] = get_sol(j);
	}

	return res;
}


//【Mo's algorithm（パスクエリ，頂点）】O(n√q α + q log q)
/*
* 頂点重み a[0..n) が与えられた無向木 g に対し，各 j∈[0..q) について，
* u[j] と v[j] を結ぶパスクエリに対する解を順に格納したリストを返す．
*
* 制約：任意箇所の頂点の追加[削除] が O(α) で可能
*/
pli op_moLCA(pli a, pli b) { return min(a, b); }
pli e_moLCA() { return { INFL, -1 }; }
template <class F1, class F2, class F3>
void mos_algorithm_on_tree_vtx(const Graph& g, const vi& u, const vi& v, const F1& insert, const F2& erase, const F3& get_sol) {
	// 参考 : https://ei1333.hateblo.jp/entry/2017/09/11/211011
	// verify : https://www.spoj.com/problems/COT2/

	int n = sz(g), q = sz(u);
	int sqrt_q = max((int)sqrt(q), 1);
	int width = max((n + sqrt_q - 1) / sqrt_q, 1);

	// 便宜上 g を 0 を根とする根付き木とみなす．
	//	in[s] : オイラーツアーで最初に頂点 s を訪れた時刻（根なら 0）
	//	out[s] : オイラーツアーで最後に頂点 s から離れた時刻（根なら 2n-1）
	//	pos[t] : オイラーツアーで時刻 t に訪れていた頂点の番号
	//	dep[s] : 頂点 s の深さ
	//	c[t] : t 番目に通った辺の両端の頂点のうち子の方の番号
	vi in(n), out(n), pos(2 * n - 1), dep(n), c(2 * (n - 1));

	// 0 を根とする根付き木とみなしてオイラーツアーを行う．
	int time = 0;
	function<void(int, int)> rf = [&](int s, int p) {
		in[s] = time;
		pos[time++] = s;

		repe(t, g[s]) {
			if (t == p) continue;

			c[time - 1] = t;

			dep[t] = dep[s] + 1;
			rf(t, s);

			c[time - 1] = t;

			pos[time++] = s;
		}

		out[s] = time;
	};
	rf(0, -1);

	// seg : 深さに関する RmQ を処理するためのセグ木
	vector<pli> ini(2 * n - 1);
	rep(t, 2 * n - 1) ini[t] = { dep[pos[t]], pos[t] };
	segtree<pli, op_moLCA, e_moLCA> seg(ini);

	// 頂点 s, t の LCA を返す．
	auto get_lca = [&](int s, int t) {
		// 初めて s または t に訪れたとき
		int left = min(in[s], in[t]);

		// 最後に s または t から離れたとき
		int right = max(out[s], out[t]);

		// その途中で訪れたことのある最も浅い頂点が LCA
		return seg.prod(left, right).second;
	};

	// クエリを左端の位置するブロックについて昇順に，
	// 次いで右端を偶数番目のブロックは昇順，奇数番目のブロックは降順でソートする．
	vi l(q), r(q);
	vector<tuple<int, int, int>> lb_sr_j(q);
	rep(j, q) {
		l[j] = in[u[j]];
		r[j] = in[v[j]];
		if (l[j] > r[j]) swap(l[j], r[j]);

		int b = l[j] / width;
		lb_sr_j[j] = { b, (b % 2 == 0 ? 1 : -1) * r[j], j };
	}
	sort(all(lb_sr_j));


	// exist[i] : 頂点 i がデータ構造に含まれているか
	vi exist(n);

	// 頂点の有無を切り替える．
	auto flip = [&](int t) {
		if (exist[c[t]]) erase(c[t]);
		else insert(c[t]);

		exist[c[t]] ^= 1;
	};

	// lpt[rpt] : 半開区間の左[右] 端の位置
	int lpt = 0, rpt = 0;

	// クエリを順に処理していく．
	rep(tmp, q) {
		int j = get<2>(lb_sr_j[tmp]);

		// 区間を広げる．
		while (lpt > l[j]) flip(--lpt);
		while (rpt < r[j]) flip(rpt++);

		// 区間を狭める．
		while (lpt < l[j]) flip(lpt++);
		while (rpt > r[j]) flip(--rpt);

		// パスの両端点の LCA を追加する．
		int lca = get_lca(u[j], v[j]);
		insert(lca);

		// クエリ j に対する解を得る．
		get_sol(j);

		// パスの両端点の LCA を削除する．
		erase(lca);
	}

	/* 雛形
	// 必要なデータ構造を用意する．
	vi freq(n);
	int sol = 0;

	// 区間に要素 w を追加し，データ構造を更新する．
	auto insert = [&](int i) {
		if (freq[w[i]] == 0) sol++;
		freq[w[i]]++;
	};

	// 区間から要素 w を削除し，データ構造を更新する．
	auto erase = [&](int i) {
		freq[w[i]]--;
		if (freq[w[i]] == 0) sol--;
	};

	// クエリ j に対し，データ構造を参照して解を求める．
	vi res(q);
	auto get_sol = [&](int j) {
		res[j] = sol;
	};

	mos_algorithm_on_tree_vtx(g, u, v, insert, erase, get_sol);
	*/
}


//【Link-Cut Tree（可換モノイド）】
/*
* Link_cut_tree<S, op, o>(int n) : O(n)
*	値 o() をもった n 頂点で初期化する．
*	要素は可換モノイド (S, op, o) の元とする．
* 
* Link_cut_tree<S, op, o>(vS a) : O(n log n)
*	値 a[0..n) をもった n 頂点で初期化する．
* 
* cut(int i) : ならし O(log n)
*	頂点 i とその親との間の辺を切断する．
*	制約：i は根でない．
* 
* cut(int s, int t) : ならし O(log n)
*	辺 s-t を切断する（辺がなければ何もしない）
* 
* link(int s, int t) : ならし O(log n)
*	頂点 s と根 t に対し，親から子への辺 s→t を繋ぐ．
*	制約：t は根である．
* 
* set_root(int rt) : ならし O(log n)
*	頂点 rt を根にする．
* 
* S sum(int i) : ならし O(log n)
*	根から頂点 i まで（両端含む）の頂点の値の総和を返す．
* 
* set(int i, S x) : ならし O(log n)
*	頂点 i の値を x に変更する．
* 
* add(int i, S x) : ならし O(log n)
*	頂点 i の値に x を加える．
* 
* bool connectedQ(int s, int t) : ならし O(log n)
*	頂点 s, t が連結かを返す．
*/
template <class S, S(*op)(S, S), S(*o)()>
class Link_cut_tree {
	// 参考 : https://www.slideshare.net/iwiwi/2-12188845
	// 参考 : https://ei1333.github.io/library/structure/lct/link-cut-tree.hpp.html

	// splay 木（平衡二分探索木）のノード
	// key は Link-Cut 木のノードの深さとし，左ほど浅く右ほど深いものとする．
	struct Node {
		S val; // 頂点の値
		S acc; // 部分木のノードの総和
		bool rev; // 部分木が反転されているか
		Node* p; // 親へのポインタ（heavy path 内部または外部）
		Node* l, * r; // 左右の子へのポインタ

		Node(S val = o()) : val(val), acc(val), rev(false), p(nullptr), l(nullptr), r(nullptr) {}

		// 自身が根かを返す．
		bool rootQ() {
			// 親が設定されていなければもちろん根である．
			// そうでなくてもそれが heavy path 外部の親であれば自身は根である．
			return !p || (p->l != this && p->r != this);
		}

		// 自身の情報を子の情報をもとにして更新する．
		void pushup() {
			acc = op(l ? l->acc : o(), op(val, r ? r->acc : o()));
		}

		void pushdown() {
			// 反転フラグが立っていたら実際に反転しフラグを折る．
			if (rev) {
				rev = false;
				swap(l, r);

				// 自分の子については反転フラグを flip しておくだけにする．
				if (l) l->rev = !l->rev;
				if (r) r->rev = !r->rev;
			}

			// 自身の情報（acc）を子の情報をもとにして更新する．
			pushup();
		}

		// 右回転する．
		void rotR() {
			Node* my_p = p, * my_pp = my_p->p;

			// 自分の親と自分の右の子（あれば）を繋ぐ．
			if (my_p->l = r) r->p = my_p;
			my_p->pushup();

			// 自身と自分の親の親子関係を逆転させる．
			r = my_p; my_p->p = this;
			pushup();

			// 自身と自分の親の親（あれば）を繋ぐ．
			if (p = my_pp) {
				if (my_pp->l == my_p) my_pp->l = this;
				if (my_pp->r == my_p) my_pp->r = this;
			}
		}

		// 左回転する．
		void rotL() {
			Node* my_p = p, * my_pp = my_p->p;

			// 自分の親と自分の左の子（あれば）を繋ぐ．
			if (my_p->r = l) l->p = my_p;
			my_p->pushup();

			// 自身と自分の親の親子関係を逆転させる．
			l = my_p; my_p->p = this;
			pushup();

			// 自身と自分の親の親（あれば）を繋ぐ．
			if (p = my_pp) {
				if (my_pp->l == my_p) my_pp->l = this;
				if (my_pp->r == my_p) my_pp->r = this;
			}
		}

		// 自身を heavy path 内の根に持ってくる（スプレー操作）
		void splay() {
			pushdown();

			// 自身が根でないかぎり操作を続ける．
			while (!rootQ()) {
				Node* my_p = p;

				// 自分の親が根である場合
				if (my_p->rootQ()) {
					my_p->pushdown();
					pushdown();

					// 適切な回転操作を行う．
					if (my_p->l == this) rotR();
					else rotL();
				}
				// 自分の親が根でない場合
				else {
					Node* my_pp = my_p->p;
					my_pp->pushdown();
					my_p->pushdown();
					pushdown();

					// 自分の親の親から自分まで同方向に枝が伸びている場合は zig-zig ステップ，
					// さもなくば zig-zag ステップを実行する．
					if (my_pp->l == my_p) {
						// zig-zig ステップ
						if (my_p->l == this) my_p->rotR(), rotR();
						// zig-zag ステップ
						else rotL(), rotR();
					}
					else {
						// zig-zig ステップ
						if (my_p->r == this) my_p->rotL(), rotL();
						// zig-zag ステップ
						else rotR(), rotL();
					}
				}
			}
		}
	};

	int n; // 頂点数
	vector<Node> vs; // 頂点のリスト

	// 木の根から頂点 v までを 1 つの heavy path にし，v を heavy path 内の根とする．
	void expose(Node* v) {
		auto v0(v);

		// rt : 作成途中の heavy path の根
		Node* rt = nullptr;

		while (v) {
			// v をいまの heavy path 内の根にもってくる．
			v->splay();

			// v の右の子を切り離し，作成途中の heavy path を繋ぐ．
			v->r = rt;
			rt = v;
			v->pushup();

			// 1 つ上の heavy path に移動する．
			v = v->p;
		}

		// v を構築した heavy path 内の根とする．
		v0->splay();
	}

	// 頂点 v とその親との間の辺を切断する．
	void cut(Node* v) {
		// 木の根から v までの heavy path を繋ぐ．
		expose(v);

		// v の親は v より 1 つ浅いので v の左の子孫になっている．
		Node* l = v->l;

		// v とその親の間の辺を切断する．
		v->l = nullptr;
		l->p = nullptr;

		v->pushup();
	}

	// 頂点 v, vp を繋ぎ，v の親を vp とする（v は根であること）
	void link(Node* v, Node* vp) {
		// それぞれの木の根から v, vp までの heavy path を繋ぐ．
		expose(v);
		expose(vp);

		// vp と v の間の辺を繋ぐ（v の方が深いので v を vp の右の子とする）
		v->p = vp;
		vp->r = v;

		vp->pushup();
	}

	// 頂点 v を木の根にする．
	void evert(Node* v) {
		expose(v);
		v->rev = !v->rev;
	}

public:
	// 値 o() をもった n 頂点で初期化する．
	Link_cut_tree(int n) : n(n), vs(n) { }

	// 値 a[0..n) をもった n 頂点で初期化する．
	Link_cut_tree(const vector<S>& a) : n(sz(a)), vs(n) {
		// verify : https://judge.yosupo.jp/problem/dynamic_tree_vertex_add_path_sum

		rep(i, n) vs[i].val = vs[i].acc = a[i];
	}

	// 頂点 i とその親との間の辺を切断する．
	void cut(int i) {
		// verify : https://judge.yosupo.jp/problem/incremental_minimum_spanning_forest

		cut(&vs[i]);
	}

	// 辺 s-t を切断する（なければ何もしない）
	void cut(int s, int t) {
		// verify : https://judge.yosupo.jp/problem/dynamic_tree_vertex_add_path_sum

		if (s == t) return;
		expose(&vs[s]), expose(&vs[t]);
		if (!vs[s].p) return;

		if (vs[t].l == &vs[s]) {
			Node* l = vs[t].l;
			vs[t].l = nullptr;
			l->p = nullptr;
			vs[t].pushup();
		}
		else {
			expose(&vs[s]);
			Node* l = vs[s].l;
			vs[s].l = nullptr;
			l->p = nullptr;
			vs[s].pushup();
		}
	}

	// 頂点 s と根 t に対し，親から子への辺 s→t を繋ぐ．
	void link(int s, int t) {
		// verify : https://judge.yosupo.jp/problem/dynamic_tree_vertex_add_path_sum

		link(&vs[t], &vs[s]);
	}

	// 頂点 rt を根にする．
	void set_root(int rt) {
		// verify : https://judge.yosupo.jp/problem/dynamic_tree_vertex_add_path_sum

		evert(&vs[rt]);
	}

	// 根から頂点 i まで（両端含む）の頂点の値の総和を返す．
	S sum(int i) {
		// verify : https://judge.yosupo.jp/problem/dynamic_tree_vertex_add_path_sum

		expose(&vs[i]);
		return vs[i].acc;
	}

	// 頂点 i の値を x に変更する．
	void set(int i, S x) {
		expose(&vs[i]);
		vs[i].val = x;
		vs[i].pushup();
	}

	// 頂点 i の値に x を加える．
	void add(int i, S x) {
		// verify : https://judge.yosupo.jp/problem/dynamic_tree_vertex_add_path_sum

		expose(&vs[i]);
		vs[i].val = op(x, vs[i].val);
		vs[i].pushup();
	}

	// 頂点 s, t が連結かを返す．
	bool connectedQ(int s, int t) {
		// verify : https://atcoder.jp/contests/abc350/tasks/abc350_g

		if (s == t) return true;
		expose(&vs[s]), expose(&vs[t]);
		return vs[s].p;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Link_cut_tree LCT) {
		// verify : https://atcoder.jp/contests/abc355/tasks/abc355_f
		rep(i, LCT.n) {
			os << LCT.vs[i].val << " ";
		}
		return os;
	}
#endif
};


//【Link-Cut Tree（モノイド）】
/*
* Link_cut_tree<S, op, e>(int n) : O(n)
*	値 e() をもった n 頂点で初期化する．
*	要素はモノイド (S, op, e) の元とする．
*
* Link_cut_tree<S, op, o>(vS a) : O(n log n)
*	値 a[0..n) をもった n 頂点で初期化する．
*
* cut(int i) : ならし O(log n)
*	頂点 i とその親との間の辺を切断する．
*	制約：i は根でない．
*
* cut(int s, int t) : ならし O(log n)
*	辺 s-t を切断する（辺がなければ何もしない）
*
* link(int s, int t) : ならし O(log n)
*	頂点 s と根 t に対し，親から子への辺 s→t を繋ぐ．
*	制約：t は根である．
*
* set_root(int rt) : ならし O(log n)
*	頂点 rt を根にする．
*
* S prod(int i) : ならし O(log n)
*	根から頂点 i まで（両端含む）の頂点の値のこの順の総積を返す．
*
* set(int i, S x) : ならし O(log n)
*	頂点 i の値を x に変更する．
*
* apply_left(int i, S x) : ならし O(log n)
*	頂点 i の値に x を左から乗ずる．
*
* bool connectedQ(int s, int t) : ならし O(log n)
*	頂点 s, t が連結かを返す．
*/
template <class S, S(*op)(S, S), S(*e)()>
class Link_cut_tree {
	// 参考 : https://www.slideshare.net/iwiwi/2-12188845
	// 参考 : https://ei1333.github.io/library/structure/lct/link-cut-tree.hpp.html

	// splay 木（平衡二分探索木）のノード
	// key は Link-Cut 木のノードの深さとし，左ほど浅く右ほど深いものとする．
	struct Node {
		S val; // 頂点の値
		S acc; // 部分木のノードの総積
		S accR; // 部分木のノードの総積（逆順）
		bool rev; // 部分木が反転されているか
		Node* p; // 親へのポインタ（heavy path 内部または外部）
		Node* l, * r; // 左右の子へのポインタ

		Node(S val = e()) : val(val), acc(val), accR(val), rev(false), p(nullptr), l(nullptr), r(nullptr) {}

		// 自身が根かを返す．
		bool rootQ() {
			// 親が設定されていなければもちろん根である．
			// そうでなくてもそれが heavy path 外部の親であれば自身は根である．
			return !p || (p->l != this && p->r != this);
		}

		void toggle() {
			swap(acc, accR);
			swap(l, r);
			rev = !rev;
		}

		// 自身の情報を子の情報をもとにして更新する．
		void pushup() {
			acc = op(l ? l->acc : e(), op(val, r ? r->acc : e()));
			accR = op(r ? r->accR : e(), op(val, l ? l->accR : e()));
		}

		void pushdown() {
			if (rev) {
				if (l) l->toggle();
				if (r) r->toggle();
				rev = false;
			}

			pushup();
		}

		// 右回転する．
		void rotR() {
			Node* my_p = p, * my_pp = my_p->p;

			// 自分の親と自分の右の子（あれば）を繋ぐ．
			if (my_p->l = r) r->p = my_p;

			// 自身と自分の親の親子関係を逆転させる．
			r = my_p; my_p->p = this;

			my_p->pushup();
			pushup();

			// 自身と自分の親の親（あれば）を繋ぐ．
			if (p = my_pp) {
				if (my_pp->l == my_p) my_pp->l = this;
				if (my_pp->r == my_p) my_pp->r = this;

				my_pp->pushup();
			}
		}

		// 左回転する．
		void rotL() {
			Node* my_p = p, * my_pp = my_p->p;

			// 自分の親と自分の左の子（あれば）を繋ぐ．
			if (my_p->r = l) l->p = my_p;

			// 自身と自分の親の親子関係を逆転させる．
			l = my_p; my_p->p = this;

			my_p->pushup();
			pushup();

			// 自身と自分の親の親（あれば）を繋ぐ．
			if (p = my_pp) {
				if (my_pp->l == my_p) my_pp->l = this;
				if (my_pp->r == my_p) my_pp->r = this;

				my_pp->pushup();
			}
		}

		// 自身を heavy path 内の根に持ってくる（スプレー操作）
		void splay() {
			pushdown();

			// 自身が根でないかぎり操作を続ける．
			while (!rootQ()) {
				Node* my_p = p;

				// 自分の親が根である場合
				if (my_p->rootQ()) {
					my_p->pushdown();
					pushdown();

					// 適切な回転操作を行う．
					if (my_p->l == this) rotR();
					else rotL();
				}
				// 自分の親が根でない場合
				else {
					Node* my_pp = my_p->p;
					my_pp->pushdown();
					my_p->pushdown();
					pushdown();

					// 自分の親の親から自分まで同方向に枝が伸びている場合は zig-zig ステップ，
					// さもなくば zig-zag ステップを実行する．
					if (my_pp->l == my_p) {
						// zig-zig ステップ
						if (my_p->l == this) my_p->rotR(), rotR();
						// zig-zag ステップ
						else rotL(), rotR();
					}
					else {
						// zig-zig ステップ
						if (my_p->r == this) my_p->rotL(), rotL();
						// zig-zag ステップ
						else rotR(), rotL();
					}
				}
			}
		}
	};

	int n; // 頂点数
	vector<Node> vs; // 頂点のリスト

	// 木の根から頂点 v までを 1 つの heavy path にし，v を heavy path を表す Splay 木の根とする．
	void expose(Node* v) {
		auto v0(v);

		// rt : 作成途中の heavy path の根
		Node* rt = nullptr;

		while (v) {
			// v を heavy path を表す Splay 木の根にもってくる．
			v->splay();

			// v の右の子を切り離し，作成途中の heavy path を繋ぐ．
			v->r = rt;
			rt = v;
			v->pushup();

			// 1 つ上の heavy path に移動する．
			v = v->p;
		}

		// v を構築した heavy path 内の根とする．
		v0->splay();
	}

	// 頂点 v とその親との間の辺を切断する．
	void cut(Node* v) {
		// 木の根から v までの heavy path を繋ぐ．
		expose(v);

		// v の親は v より 1 つ浅いので v の左の子孫になっている．
		Node* l = v->l;

		// v とその親の間の辺を切断する．
		v->l = nullptr;
		l->p = nullptr;

		v->pushup();
	}

	// 頂点 v, vp を繋ぎ，v の親を vp とする（v は根であること）
	void link(Node* v, Node* vp) {
		// それぞれの木の根から v, vp までの heavy path を繋ぐ．
		expose(v);
		expose(vp);

		// vp と v の間の辺を繋ぐ（v の方が深いので v を vp の右の子とする）
		v->p = vp;
		vp->r = v;

		vp->pushup();
	}

	// 頂点 v を木の根にする．
	void evert(Node* v) {
		expose(v);
		v->toggle();
		v->pushdown();
	}

public:
	// 値 e() をもった n 頂点で初期化する．
	Link_cut_tree(int n) : n(n), vs(n) { }

	// 値 a[0..n) をもった n 頂点で初期化する．
	Link_cut_tree(const vector<S>& a) : n(sz(a)), vs(n) {
		// verify : https://judge.yosupo.jp/problem/dynamic_tree_vertex_set_path_composite

		rep(i, n) vs[i].val = vs[i].acc = vs[i].accR = a[i];
	}

	// 頂点 i とその親との間の辺を切断する．
	void cut(int i) {
		cut(&vs[i]);
	}

	// 辺 s-t を切断する（なければ何もしない）
	void cut(int s, int t) {
		// verify : https://judge.yosupo.jp/problem/dynamic_tree_vertex_set_path_composite

		if (s == t) return;
		expose(&vs[s]), expose(&vs[t]);
		if (!vs[s].p) return;

		if (vs[t].l == &vs[s]) {
			Node* l = vs[t].l;
			vs[t].l = nullptr;
			l->p = nullptr;
			vs[t].pushup();
		}
		else {
			expose(&vs[s]);
			Node* l = vs[s].l;
			vs[s].l = nullptr;
			l->p = nullptr;
			vs[s].pushup();
		}
	}

	// 頂点 s と根 t に対し，親から子への辺 s→t を繋ぐ．
	void link(int s, int t) {
		// verify : https://judge.yosupo.jp/problem/dynamic_tree_vertex_set_path_composite

		link(&vs[t], &vs[s]);
	}

	// 頂点 rt を根にする．
	void set_root(int rt) {
		// verify : https://judge.yosupo.jp/problem/dynamic_tree_vertex_set_path_composite

		evert(&vs[rt]);
	}

	// 根から頂点 i まで（両端含む）の頂点の値のこの順の総積を返す．
	S prod(int i) {
		// verify : https://judge.yosupo.jp/problem/dynamic_tree_vertex_set_path_composite

		expose(&vs[i]);
		return vs[i].acc;
	}

	// 頂点 i の値を x に変更する．
	void set(int i, S x) {
		// verify : https://judge.yosupo.jp/problem/dynamic_tree_vertex_set_path_composite

		expose(&vs[i]);
		vs[i].val = x;
		vs[i].pushup();
	}

	// 頂点 i の値に x を左から乗ずる．
	void apply_left(int i, S x) {
		expose(&vs[i]);
		vs[i].val = op(x, vs[i].val);
		vs[i].pushup();
	}

	// 頂点 s, t が連結かを返す．
	bool connectedQ(int s, int t) {
		if (s == t) return true;
		expose(&vs[s]), expose(&vs[t]);
		return vs[s].p;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Link_cut_tree LCT) {
		rep(i, LCT.n) {
			os << LCT.vs[i].val << " ";
		}
		return os;
	}
#endif
};


//【Euler Tour Tree（可換モノイド）】
/*
* Euler_tour_tree<S, op, o>(int n) : O(n)
*	値 o() をもった n 頂点で初期化する．
*	要素は可換モノイド (S, op, o) の元とする．
*
* Euler_tour_tree<S, op, o>(vS a) : O(n log n)
*	値 a[0..n) をもった n 頂点で初期化する．
*
* cut(int i) : ならし O(log n)
*	頂点 i とその親との間の辺を切断する．
*
* cut(int s, int t) : ならし O(log n)
*	辺 s→t を切断する．
*	制約：s は根である．
*
* link(int s, int t) : ならし O(log n)
*	根 s, t に対し，親から子への辺 s→t を繋ぐ．
*	制約：s, t は根である．
*
* set_root(int rt) : ならし O(log n)
*	頂点 rt を根にする．
*
* S sum(int i) : ならし O(log n)
*	部分木 i の頂点の値の総和を返す．
*
* set(int i, S x) : ならし O(log n)
*	頂点 i の値を x に変更する．
*
* add(int i, S x) : ならし O(log n)
*	頂点 i の値に x を加える．
*
* bool connectedQ(int s, int t) : ならし O(log n)
*	頂点 s, t が連結かを返す．
*/
template <class S, S(*op)(S, S), S(*o)()>
class Euler_tour_tree {
	// 参考 : https://qiita.com/hotman78/items/78cd3aa50b05a57738d4

	// splay 木（平衡二分探索木）のノード
	// key はオイラーツアー順とし，左ほど先で右ほど後とする．
	struct Node {
		int s, t; // 辺 s→t に対応するノードであることを表す（デバッグ用）
		S val; // 頂点の値
		S acc; // 部分木のノードの総和
		Node* p; // 親へのポインタ
		Node* l, * r; // 左右の子へのポインタ

		Node(int s, int t, S val = o()) : s(s), t(t), val(val), acc(val), p(nullptr), l(nullptr), r(nullptr) {}

		// 自身の情報を子の情報をもとにして更新する．
		void pushup() {
			acc = op(l ? l->acc : o(), op(val, r ? r->acc : o()));
		}

		// 右回転する．
		void rotR() {
			Node* my_p = p, * my_pp = my_p->p;

			// 自分の親と自分の右の子（あれば）を繋ぐ．
			if (my_p->l = r) r->p = my_p;
			my_p->pushup();

			// 自身と自分の親の親子関係を逆転させる．
			r = my_p; my_p->p = this;
			pushup();

			// 自身と自分の親の親（あれば）を繋ぐ．
			if (p = my_pp) {
				if (my_pp->l == my_p) my_pp->l = this;
				if (my_pp->r == my_p) my_pp->r = this;
			}
		}

		// 左回転する．
		void rotL() {
			Node* my_p = p, * my_pp = my_p->p;

			// 自分の親と自分の左の子（あれば）を繋ぐ．
			if (my_p->r = l) l->p = my_p;
			my_p->pushup();

			// 自身と自分の親の親子関係を逆転させる．
			l = my_p; my_p->p = this;
			pushup();

			// 自身と自分の親の親（あれば）を繋ぐ．
			if (p = my_pp) {
				if (my_pp->l == my_p) my_pp->l = this;
				if (my_pp->r == my_p) my_pp->r = this;
			}
		}

		// 自身を splay 木内の根に持ってくる（スプレー操作）
		void splay() {
			// 自身が根でないかぎり操作を続ける．
			while (p) {
				Node* my_p = p;

				// 自分の親が根である場合
				if (!my_p->p) {
					// 適切な回転操作を行う．
					if (my_p->l == this) rotR();
					else rotL();
				}
				// 自分の親が根でない場合
				else {
					Node* my_pp = my_p->p;

					// 自分の親の親から自分まで同方向に枝が伸びている場合は zig-zig ステップ，
					// さもなくば zig-zag ステップを実行する．
					if (my_pp->l == my_p) {
						// zig-zig ステップ
						if (my_p->l == this) my_p->rotR(), rotR();
						// zig-zag ステップ
						else rotL(), rotR();
					}
					else {
						// zig-zig ステップ
						if (my_p->r == this) my_p->rotL(), rotL();
						// zig-zag ステップ
						else rotR(), rotL();
					}
				}
			}
		}
	};

	int n; // 頂点数
	vector<unordered_map<int, Node*>> vs; // 辺へのポインタのリスト

	// 辺 s→t へのポインタを返す．
	Node* get_node(int s, int t) {
		auto it = vs[s].find(t);
		if (it == vs[s].end()) return vs[s][t] = new Node(s, t);
		else return it->second;
	}

public:
	// 値 o() をもった n 頂点で初期化する．
	Euler_tour_tree(int n) : n(n), vs(n) {
		rep(i, n) vs[i][i] = get_node(i, i);
	}

	// 値 a[0..n) をもった n 頂点で初期化する．
	Euler_tour_tree(const vector<S>& a) : n(sz(a)), vs(n) {
		// verify : https://judge.yosupo.jp/problem/dynamic_tree_vertex_add_subtree_sum

		rep(i, n) {
			Node* v = get_node(i, i);
			v->val = v->acc = a[i];
			vs[i][i] = v;
		}
	}

	// 辺 s→t を切断する（s は根であること）
	void cut(int s, int t) {
		// verify : https://judge.yosupo.jp/problem/dynamic_tree_vertex_add_subtree_sum

		if (s == t) return;

		Node* st = get_node(s, t);
		st->splay();
		if (st->l) st->l->p = nullptr;
		if (st->r) st->r->p = nullptr;

		Node* ts = get_node(t, s);
		ts->splay();
		if (ts->l) ts->l->p = nullptr;
		if (ts->r) ts->r->p = nullptr;

		if (st->l && ts->r) {
			Node* stl_last = st->l;
			while (stl_last->r) stl_last = stl_last->r;
			stl_last->splay();
			stl_last->r = ts->r;
			ts->r->p = stl_last;
			stl_last->pushup();
		}

		delete st; vs[s].erase(t);
		delete ts; vs[t].erase(s);
	}

	// 根 s, t に対し，親から子への辺 s→t を繋ぐ．
	void link(int s, int t) {
		// verify : https://judge.yosupo.jp/problem/dynamic_tree_vertex_add_subtree_sum

		Node* ts = get_node(t, s);
		Node* tt = get_node(t, t);
		tt->splay();
		ts->l = tt;
		tt->p = ts;
		ts->pushup();

		Node* st = get_node(s, t);
		tt->splay();
		tt->l = st;
		st->p = tt;
		tt->pushup();

		Node* ss = get_node(s, s);
		ss->splay();
		st->splay();
		st->l = ss;
		ss->p = st;
		st->pushup();
	}

	// 頂点 rt を根にする．
	void set_root(int rt) {
		// verify : https://judge.yosupo.jp/problem/dynamic_tree_vertex_add_subtree_sum

		Node* rr = get_node(rt, rt);
		rr->splay();
		Node* rr_l = rr->l;
		if (!rr_l) return;
		rr_l->p = nullptr;
		rr->l = nullptr;
		rr->pushup();

		Node* rr_last = rr;
		while (rr_last->r) rr_last = rr_last->r;
		rr_last->splay();
		rr_last->r = rr_l;
		rr_l->p = rr_last;
		rr_last->pushup();
	}

	// 部分木 i の頂点の値の総和を返す．
	S sum(int i) {
		// verify : https://judge.yosupo.jp/problem/dynamic_tree_vertex_add_subtree_sum

		Node* ii = get_node(i, i);
		ii->splay();
		return ii->acc;
	}

	// 頂点 i の値を x に変更する．
	void set(int i, S x) {
		Node* ii = get_node(i, i);
		ii->splay();
		ii->val = x;
		ii->pushup();
	}

	// 頂点 i の値に x を加える．
	void add(int i, S x) {
		// verify : https://judge.yosupo.jp/problem/dynamic_tree_vertex_add_subtree_sum

		Node* ii = get_node(i, i);
		ii->splay();
		ii->val = op(x, ii->val);
		ii->pushup();
	}

	// 頂点 s, t が連結かを返す．
	bool connectedQ(int s, int t) {
		if (s == t) return true;
		Node* ss = get_node(s, s);
		Node* tt = get_node(t, t);
		ss->splay();
		tt->splay();
		return ss->p;
	}

	// 辺 i→i を含む木の構造を表示する．
	void dump_tree(int i) {
		Node* root = get_node(i, i);
		while (root->p) root = root->p;

		function<void(Node*)> rf = [&](Node* v) {
			if (!v) return;

			rf(v->l);

			cerr << "(" << v->s << "→" << v->t << ")" << " : ";
			cerr << "p=";
			if (v->p) cerr << "(" << v->p->s << "→" << v->p->t << ")";
			else cerr << "(null)";
			cerr << " l=";
			if (v->l) cerr << "(" << v->l->s << "→" << v->l->t << ")";
			else cerr << "(null)";
			cerr << " r=";
			if (v->r) cerr << "(" << v->r->s << "→" << v->r->t << ")";
			else cerr << "(null)";
			cerr << " acc=" << v->acc;
			cerr << endl;

			rf(v->r);
		};
		rf(root);
		cerr << endl;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Euler_tour_tree ETT) {
		os << "val:";
		rep(i, ETT.n) os << " " << ETT.vs[i][i]->val;
		os << endl;
		os << "graph:" << endl;
		rep(i, ETT.n) {
			os << i << ":";
			repe(tmp, ETT.vs[i]) os << " " << tmp.first;
			os << endl;
		}
		return os;
	}
#endif
};


//【Euler Tour Tree（M-可換モノイド）】
/*
* Euler_tour_tree<S, op, o, F, act, comp, id>(int n) : O(n)
*	値 o() をもった n 頂点で初期化する．
*	要素は M-可換モノイド (S, op, o, F, act, comp, id) の元とする．
*
* Euler_tour_tree<S, op, o, F, act, comp, id>(vS a) : O(n log n)
*	値 a[0..n) をもった n 頂点で初期化する．
*
* cut(int i) : ならし O(log n)
*	頂点 i とその親との間の辺を切断する．
*
* cut(int s, int t) : ならし O(log n)
*	辺 s→t を切断する．
*	制約：s は根である．
*
* link(int s, int t) : ならし O(log n)
*	根 s, t に対し，親から子への辺 s→t を繋ぐ．
*	制約：s, t は根である．
*
* set_root(int rt) : ならし O(log n)
*	頂点 rt を根にする．
*
* S sum(int i) : ならし O(log n)
*	部分木 i の頂点の値の総和を返す．
*
* set(int i, S x) : ならし O(log n)
*	頂点 i の値を x に変更する．
*
* add(int i, S x) : ならし O(log n)
*	頂点 i の値に x を加える．
*
* apply(int i, F f) : ならし O(log n)
*	部分木 i の頂点の値に f を作用させる．
*
* bool connectedQ(int s, int t) : ならし O(log n)
*	頂点 s, t が連結かを返す．
*/
template <class S, S(*op)(S, S), S(*o)(), class F, S(*act)(F, S), F(*comp)(F, F), F(*id)()>
class Euler_tour_tree {
	// 参考 : https://qiita.com/hotman78/items/78cd3aa50b05a57738d4

	// splay 木（平衡二分探索木）のノード
	// key はオイラーツアー順とし，左ほど先で右ほど後とする．
	struct Node {
		int s, t; // 辺 s→t に対応するノードであることを表す（デバッグ用）
		S val; // 頂点の値
		S acc; // 部分木のノードの総和
		F lazy; // 部分木への遅延作用
		Node* p; // 親へのポインタ
		Node* l, * r; // 左右の子へのポインタ

		Node(int s, int t, S val = o()) : s(s), t(t), val(val), acc(val), lazy(id()), p(nullptr), l(nullptr), r(nullptr) {}

		// 自身の情報を子の情報をもとにして更新する．
		void pushup() {
			acc = op(l ? l->acc : o(), op(val, r ? r->acc : o()));
		}

		// 自身の遅延評価を適用し，子に遅延評価を設定する．
		void pushdown() {
			// 遅延させている作用があれば実際に作用させ遅延作用をなくす．
			if (lazy != id()) {
				// 自分の子については作用を遅延させておくだけにする（ただし acc は更新する）
				if (l) {
					l->lazy = comp(lazy, l->lazy);
					l->acc = act(lazy, l->acc);
				}
				if (r) {
					r->lazy = comp(lazy, r->lazy);
					r->acc = act(lazy, r->acc);
				}

				val = act(lazy, val);
				lazy = id();
			}

			// 自身の情報（acc）を子の情報をもとにして更新する．
			pushup();
		}

		// 右回転する．
		void rotR() {
			Node* my_p = p, * my_pp = my_p->p;

			// 自分の親と自分の右の子（あれば）を繋ぐ．
			if (my_p->l = r) r->p = my_p;
			my_p->pushup();

			// 自身と自分の親の親子関係を逆転させる．
			r = my_p; my_p->p = this;
			pushup();

			// 自身と自分の親の親（あれば）を繋ぐ．
			if (p = my_pp) {
				if (my_pp->l == my_p) my_pp->l = this;
				if (my_pp->r == my_p) my_pp->r = this;
			}
		}

		// 左回転する．
		void rotL() {
			Node* my_p = p, * my_pp = my_p->p;

			// 自分の親と自分の左の子（あれば）を繋ぐ．
			if (my_p->r = l) l->p = my_p;
			my_p->pushup();

			// 自身と自分の親の親子関係を逆転させる．
			l = my_p; my_p->p = this;
			pushup();

			// 自身と自分の親の親（あれば）を繋ぐ．
			if (p = my_pp) {
				if (my_pp->l == my_p) my_pp->l = this;
				if (my_pp->r == my_p) my_pp->r = this;
			}
		}

		// 自身を splay 木内の根に持ってくる（スプレー操作）
		void splay() {
			// 根から順に lazy を作用させる．
			stack<Node*> stk; Node* tmp = this;
			while (tmp->p) {
				stk.push(tmp);
				tmp = tmp->p;
			}
			stk.push(tmp);

			while (!stk.empty()) {
				stk.top()->pushdown();
				stk.pop();
			}

			// 自身が根でないかぎり操作を続ける．
			while (p) {
				Node* my_p = p;

				// 自分の親が根である場合
				if (!my_p->p) {
					// 適切な回転操作を行う．
					if (my_p->l == this) rotR();
					else rotL();
				}
				// 自分の親が根でない場合
				else {
					Node* my_pp = my_p->p;

					// 自分の親の親から自分まで同方向に枝が伸びている場合は zig-zig ステップ，
					// さもなくば zig-zag ステップを実行する．
					if (my_pp->l == my_p) {
						// zig-zig ステップ
						if (my_p->l == this) my_p->rotR(), rotR();
						// zig-zag ステップ
						else rotL(), rotR();
					}
					else {
						// zig-zig ステップ
						if (my_p->r == this) my_p->rotL(), rotL();
						// zig-zag ステップ
						else rotR(), rotL();
					}
				}
			}
		}
	};

	int n; // 頂点数
	vector<unordered_map<int, Node*>> vs; // 辺へのポインタのリスト

	// 辺 s→t へのポインタを返す．
	Node* get_node(int s, int t) {
		auto it = vs[s].find(t);
		if (it == vs[s].end()) return vs[s][t] = new Node(s, t);
		else return it->second;
	}

public:
	// 値 o() をもった n 頂点で初期化する．
	Euler_tour_tree(int n) : n(n), vs(n) {
		rep(i, n) vs[i][i] = get_node(i, i);
	}

	// 値 a[0..n) をもった n 頂点で初期化する．
	Euler_tour_tree(const vector<S>& a) : n(sz(a)), vs(n) {
		// verify : https://judge.yosupo.jp/problem/dynamic_tree_subtree_add_subtree_sum

		rep(i, n) {
			Node* v = get_node(i, i);
			v->val = v->acc = a[i];
			vs[i][i] = v;
		}
	}

	// 辺 s→t を切断する（s は根であること）
	void cut(int s, int t) {
		// verify : https://judge.yosupo.jp/problem/dynamic_tree_subtree_add_subtree_sum

		if (s == t) return;

		Node* st = get_node(s, t);
		st->splay();
		if (st->l) st->l->p = nullptr;
		if (st->r) st->r->p = nullptr;

		Node* ts = get_node(t, s);
		ts->splay();
		if (ts->l) ts->l->p = nullptr;
		if (ts->r) ts->r->p = nullptr;

		if (st->l && ts->r) {
			Node* stl_last = st->l;
			while (stl_last->r) stl_last = stl_last->r;
			stl_last->splay();
			stl_last->r = ts->r;
			ts->r->p = stl_last;
			stl_last->pushup();
		}

		delete st; vs[s].erase(t);
		delete ts; vs[t].erase(s);
	}

	// 根 s, t に対し，親から子への辺 s→t を繋ぐ．
	void link(int s, int t) {
		// verify : https://judge.yosupo.jp/problem/dynamic_tree_subtree_add_subtree_sum

		Node* ts = get_node(t, s);
		Node* tt = get_node(t, t);
		tt->splay();
		ts->l = tt;
		tt->p = ts;
		ts->pushup();

		Node* st = get_node(s, t);
		tt->splay();
		tt->l = st;
		st->p = tt;
		tt->pushup();

		Node* ss = get_node(s, s);
		ss->splay();
		st->splay();
		st->l = ss;
		ss->p = st;
		st->pushup();
	}

	// 頂点 rt を根にする．
	void set_root(int rt) {
		// verify : https://judge.yosupo.jp/problem/dynamic_tree_subtree_add_subtree_sum

		Node* rr = get_node(rt, rt);
		rr->splay();
		Node* rr_l = rr->l;
		if (!rr_l) return;
		rr_l->p = nullptr;
		rr->l = nullptr;
		rr->pushup();

		Node* rr_last = rr;
		while (rr_last->r) rr_last = rr_last->r;
		rr_last->splay();
		rr_last->r = rr_l;
		rr_l->p = rr_last;
		rr_last->pushup();
	}

	// 部分木 i の頂点の値の総和を返す．
	S sum(int i) {
		// verify : https://judge.yosupo.jp/problem/dynamic_tree_subtree_add_subtree_sum

		Node* ii = get_node(i, i);
		ii->splay();
		return ii->acc;
	}

	// 頂点 i の値を x に変更する．
	void set(int i, S x) {
		Node* ii = get_node(i, i);
		ii->splay();
		ii->val = x;
		ii->pushup();
	}

	// 頂点 i の値に x を加える．
	void add(int i, S x) {
		Node* ii = get_node(i, i);
		ii->splay();
		ii->val = op(x, ii->val);
		ii->pushup();
	}

	// 部分木 i の頂点の値に f を作用させる．
	void apply(int i, F f) {
		// verify : https://judge.yosupo.jp/problem/dynamic_tree_subtree_add_subtree_sum

		Node* ii = get_node(i, i);
		ii->splay();
		ii->lazy = f;
	}

	// 頂点 s, t が連結かを返す．
	bool connectedQ(int s, int t) {
		if (s == t) return true;
		Node* ss = get_node(s, s);
		Node* tt = get_node(t, t);
		ss->splay();
		tt->splay();
		return ss->p;
	}

	// 辺 i→i を含む木の構造を表示する．
	void dump_tree(int i) {
		Node* root = get_node(i, i);
		while (root->p) root = root->p;

		function<void(Node*)> rf = [&](Node* v) {
			if (!v) return;

			rf(v->l);

			cerr << "(" << v->s << "→" << v->t << ")" << " : ";
			cerr << "p=";
			if (v->p) cerr << "(" << v->p->s << "→" << v->p->t << ")";
			else cerr << "(null)";
			cerr << " l=";
			if (v->l) cerr << "(" << v->l->s << "→" << v->l->t << ")";
			else cerr << "(null)";
			cerr << " r=";
			if (v->r) cerr << "(" << v->r->s << "→" << v->r->t << ")";
			else cerr << "(null)";
			cerr << " acc=" << v->acc;
			cerr << " lazy=" << v->lazy;
			cerr << endl;

			rf(v->r);
		};
		rf(root);
		cerr << endl;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Euler_tour_tree ETT) {
		//		os << "val:";
		//		rep(i, ETT.n) os << " " << ETT.vs[i][i]->val;
		//		os << endl;
		os << "graph:" << endl;
		rep(i, ETT.n) {
			os << i << ":";
			repe(tmp, ETT.vs[i]) os << " " << tmp.first;
			os << endl;
		}
		return os;
	}
#endif
};


//【Vertex Get Contour Prod（可換モノイド作用）】
/*
* 参考 : https://noshi91.hatenablog.com/entry/2022/03/27/042143
* 参考 : https://github.com/yosupo06/library-checker-problems/blob/master/tree/vertex_get_range_contour_add_on_tree/sol/correct.cpp
*/



//【根付き木のオイラーツアー】O(n)
/*
* n 頂点の根付き木 rt のオイラーツアーを求める．
*
* in[s] : DFS で最初に頂点 s を訪れた時刻（根なら 0）
* out[s] : DFS で最後に頂点 s から離れた時刻（根なら 2n-1）
* pos[t] : DFS で時刻 t に訪れていた頂点の番号（長さ 2n-1）
*/
template <class TREE>
void euler_tour(const TREE& rt, vi& in, vi& out, vi& pos) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_5_C

	int n = sz(rt);

	int time = 0;
	in.resize(n);
	out.resize(n);
	pos.resize(2 * n - 1);

	// 再帰用の関数
	function<void(int)> rf = [&](int s) {
		// s を最初に訪れた
		in[s] = time;
		pos[time++] = s;

		repe(t, rt[s].child) {
			rf(t);
			pos[time++] = s;
		}

		// s から最後に離れる
		out[s] = time;
	};

	// 根から順に探索する．
	rf(rt.r);
}


//【根付き木の HL 分解】O(n)
/*
* 根付き木 rt の HL 分解を行う．
*
* in[s] : 最重頂点優先で頂点 s を何番目になぞるか（根なら 0）
* out[s] : 最重頂点優先で頂点 s から出て次になぞる頂点が何番目か（根なら n）
* pos[i] : 最重頂点優先で i 番目になぞる頂点（長さ n）
* top[s] : 頂点 s を含む連結成分の最も浅い頂点
*/
template <class TREE>
void heavy_light_decomposition(const TREE& rt, vi& in, vi& out, vi& pos, vi& top) {
	// 参考 : https://qiita.com/Pro_ktmr/items/4e1e051ea0561772afa3
	// verify : https://judge.yosupo.jp/problem/vertex_add_path_sum

	int n = sz(rt);

	int time = 0;
	in.resize(n); out.resize(n); pos.resize(n); top.resize(n);

	// 再帰用の関数
	// s : 注目している頂点
	// p : s を含む連結成分の最も浅い頂点
	function<void(int, int)> rf = [&](int s, int p) {
		in[s] = time;
		pos[time] = s;
		top[s] = p;
		time++;

		// 重さ最大の頂点を得る．
		int w_max = -INF, v_max = -1;
		repe(t, rt[s].child) if (chmax(w_max, rt.v[t].weight)) v_max = t;

		// 重さ最大の頂点を優先的になぞる．
		if (v_max != -1) rf(v_max, p);

		// 残りの頂点をなぞる．
		repe(t, rt[s].child) {
			if (t == v_max) continue;

			rf(t, t);
		}

		// s から最後に離れる
		out[s] = time;
	};

	// 根から順に探索する．
	rf(rt.r, rt.r);
}


