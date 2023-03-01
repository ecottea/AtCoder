#pragma once
#include "header.h"
#include "最短路.h"
#include "分析(グラフ).h"
#include "DAG.h"
// ■■■■■ フローの双対問題 ■■■■■


//【最短路問題の LP 定式化】
/*
* グラフ G = (V, E) について，
*	c[s][t] : 辺 s→t のコスト
*	f[s][t] : 辺 s→t を通るか
* とおくと，S から T までの最短路問題は，f[s][t] を変数とする
*	minimize	Σs→t c[s][t] f[s][t]
*	subject to	Σv→t f[v][t] - Σs→v f[s][v] = 0 （∀v ∈ V\{S, T}）
*				ΣS→t f[S][t] - Σs→S f[s][S] = 1
*				ΣT→t f[T][t] - Σs→T f[s][T] = -1
*				0 <= f[s][t] （∀s→t ∈ E）
* なる線形計画問題として定式化できる．
* 
* これは，最小費用流問題において辺の容量を全て ∞ とし，流量を 1 としたものに等価である．
*/


//【最短路問題の双対問題】
/*
*【最短路問題の LP 定式化】の双対をとると，p[v] を変数とする
*	maximize	-p[S] + p[T]
*	subject to	p[t] - p[s] <= c[s][t] （∀s→t ∈ E）
* なる線形計画問題（いわゆる牛ゲー）になる．
*/


//【牛ゲー】
/*
* Ushige(int n) : O(n)
*	n 変数で初期化する．
*
* set_ub(int a, int b, ll d) : O(1)
*	v[b] - v[a] <= d という制約を追加する．
*
* set_lb(int a, int b, ll d) : O(1)
*	v[b] - v[a] >= d という制約を追加する．
*
* vl maximize_diff(int a) : O(n m)（m : 制約の数）
*	各 b について v[b] - v[a] の最大値（無いなら INFL）を格納したリストを返す．
*	制約を満たすことが不可能なら空リストを返す．
* 
* 利用：【コスト付きグラフ】
*/
struct Ushige {
	int n;
	WGraph g;

	Ushige(int n_) : n(n_), g(n_) {
		// verify : https://onlinejudge.u-aizu.ac.jp/problems/0304
	}

	void set_ub(int a, int b, ll d) {
		// verify : https://onlinejudge.u-aizu.ac.jp/problems/0304

		// 差の上限に対応する重みを持つ辺を張る．
		g[a].push_back({ b, d });
	}

	void set_lb(int a, int b, ll d) {
		// verify : https://onlinejudge.u-aizu.ac.jp/problems/0304

		// 差の下限に対応する重みを持つ辺を張る．
		g[b].push_back({ a, -d });
	}

	vl maximize_diff(int a) {
		// verify : https://onlinejudge.u-aizu.ac.jp/problems/0304

		// a を始点とする最短経路問題をベルマンフォード法で解く．
		// b までの最短経路長がそのまま v[b] - v[a] の最大値になる．
		// ただし負の閉路を持っていた場合は制約を満たせない．
		
		vl cost(n, INFL); // スタートからの最小コストを保持するテーブル
		cost[a] = 0;

		rep(i, n) {
			bool updated = false;

			// 全ての辺についての操作
			rep(s, n) {
				repe(e, g[s]) {
					// INFL からの引き算も認めて計算しているので，
					// st から到達不可能な負閉路も含めて検出することに注意．
					if (cost[s] + e.cost < cost[e.to]) {
						cost[e.to] = cost[s] + e.cost;
						updated = true;
					}
				}
			}

			// もしコストの更新が起こらなければ最小コスト確定
			if (!updated) return cost;
		}

		// もし全ての辺についての操作を |V| 回繰り返してもコストの更新があったなら，
		// どこかに負の閉路を持っているので false を返す．
		return vl();
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Ushige& u) {
		rep(s, u.n) {
			repe(e, u.g[s]) {
				os << "v[" << e.to << "] - v[" << s << "] <= " << e.cost << endl;
			}
		}
		return os;
	}
#endif
};


//【牛ゲー（上界指定のみ）】
/*
* Ushige_ub_only(n) : O(1)
*	n 変数で初期化する．
*
* set_ub(a, b, d) : O(1)
*	v[b] - v[a] <= d という制約を追加する．（d >= 0）
*
* vl maximize_diff(a) : O(n + m log n)（m : 制約の数）
*	各 b について v[b] - v[a] の最大値（無いなら INFL）を格納したリストを返す．
*
* 利用：【単一始点最短路】
*/
struct Ushige_ub_only {
	int n;
	WGraph g;

	Ushige_ub_only() : n(0) {}
	Ushige_ub_only(int n_) : n(n_), g(n_) {}

	void set_ub(int a, int b, ll d) {
		Assert(d >= 0);

		// 差の上限に対応する重みを持つ辺を張る．
		g[a].push_back({ b, d });
	}

	vl maximize_diff(int a) {
		// a を始点とする最短経路問題を解く．
		// b までの最短経路長がそのまま v[b] - v[a] の最大値になる．
		return dijkstra(g, a);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Ushige_ub_only& u) {
		rep(s, u.n) {
			repe(e, u.g[s]) {
				os << "v[" << e.to << "] - v[" << s << "] <= " << e.cost << endl;
			}
		}
		return os;
	}
#endif
};


//【牛ゲー（01-上界指定のみ）】
/*
* Ushige_ub01_only(n) : O(1)
*	n 変数で初期化する．
*
* set_ub(a, b, d) : O(1)
*	v[b] - v[a] <= d という制約を追加する．（d ∈ {0, 1}）
*
* vi maximize_diff(a) : O(n + m)（m : 制約の数）
*	各 b について v[b] - v[a] の最大値（無いなら INFL）を格納したリストを返す．
*
* 利用：【01-BFS】
*/
struct Ushige_ub01_only {
	int n;
	WGraph g;

	Ushige_ub01_only(int n_) : n(n_), g(n_) {
		// verify : https://atcoder.jp/contests/agc056/tasks/agc056_c
	}
	Ushige_ub01_only() : n(0) {}

	void set_ub(int a, int b, int d) {
		// verify : https://atcoder.jp/contests/agc056/tasks/agc056_c

		Assert(d == 0 || d == 1);

		// 差の上限に対応する重みを持つ辺を張る．
		g[a].push_back({ b, d });
	}

	vi maximize_diff(int a) {
		// verify : https://atcoder.jp/contests/agc056/tasks/agc056_c

		// a を始点とする最短経路問題を解く．
		// b までの最短経路長がそのまま v[b] - v[a] の最大値になる．
		return binary_BFS(g, a);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Ushige_ub01_only& u) {
		rep(s, u.n) {
			repe(e, u.g[s]) {
				os << "v[" << e.to << "] - v[" << s << "] <= " << e.cost << endl;
			}
		}
		return os;
	}
#endif
};


//【牛ゲー（1-下界指定のみ）】
/*
* Ushige_lb1_only(int n) : O(n)
*	n 変数で初期化する．
*
* set_lb1(int a, int b) : O(1)
*	v[b] - v[a] >= 1 という制約を追加する．
*
* vi minimize_range() : O(n + m)（m : 制約の数）
*	min(v) = 0 で max(v) を最小とする v[i] の一例を返す（無ければ空リスト）
*
* 利用：【トポロジカルソート】,【最長パス】
*/
struct Ushige_lb1_only {
	int n;
	Graph g; // 辺の重みが -1 のグラフ

	Ushige_lb1_only(int n_) : n(n_), g(n_) {
		// verify : https://codeforces.com/contest/1635/problem/E
	}

	void set_lb1(int a, int b) {
		// verify : https://codeforces.com/contest/1635/problem/E

		// 差の下限に対応する重みを持つ辺を張る．
		g[b].push_back(a);
	}

	vi minimize_range() {
		// verify : https://codeforces.com/contest/1635/problem/E

		// 負閉路がどこかにあれば制約充足不可能
		if (topological_sort(g).empty()) return vi();

		// DAG が保証されたので各頂点からの最長パス長を求める．
		return longest_path(g);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Ushige_lb1_only& u) {
		rep(s, u.n) {
			repe(t, u.g[s]) {
				os << "v[" << t << "] - v[" << s << "] >= " << 1 << endl;
			}
		}
		return os;
	}
#endif
};


//【牛ゲー（DAG）】
/*
* Ushige_DAG(int n) : O(n)
*	n 変数で初期化する．
*
* set_ub(int a, int b, ll d) : O(1)
*	v[b] - v[a] <= d という制約を追加する．
*
* set_lb(int a, int b, ll d) : O(1)
*	v[b] - v[a] >= d という制約を追加する．
*
* vl maximize_diff(int a) : O(n + m)（m : 制約の数）
*	各 b について v[b] - v[a] の最大値（無いなら INFL）を格納したリストを返す．
*	制約：v[s] <= ... <= v[s] のような制約の循環は存在しない
*
* 利用：【コスト最小パス】
*/
struct Ushige_DAG {
	int n;
	WGraph g_rev; // 逆グラフ

	Ushige_DAG(int n_) : n(n_), g_rev(n_) {}

	void set_ub(int a, int b, ll d) {
		// 差の上限に対応する重みを持つ辺を張る．
		g_rev[b].push_back({ a, d });
	}

	void set_lb(int a, int b, ll d) {
		// 差の下限に対応する重みを持つ辺を張る．
		g_rev[a].push_back({ b, -d });
	}

	vl maximize_diff(int a) {
		// a を始点とする最短経路問題を解く代わりに，a を終点とする逆グラフの最短経路問題を解く．
		// b までの最短経路長がそのまま v[b] - v[a] の最大値になる．
		return lowest_cost_path(g_rev, a);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Ushige_DAG& u) {
		rep(s, u.n) {
			repe(e, u.g_rev[s]) {
				os << "v[" << s << "] - v[" << e.to << "] <= " << e.cost << endl;
			}
		}
		return os;
	}
#endif
};


//【最大流問題の LP 定式化】
/*
* グラフ G = (V, E) について，
*	u[s][t] : 辺 s→t の容量
*	f[s][t] : 辺 s→t のフロー
* とおくと，最大流問題は，F, f[s][t] を変数とする
*	maximize	F
*	subject to	Σv→t f[v][t] - Σs→v f[s][v] = 0 （∀v ∈ V\{S, T}）
*				ΣS→t f[S][t] - Σs→S f[s][S] = F
*				ΣT→t f[T][t] - Σs→T f[s][T] = -F
*				0 <= f[s][t] <= u[s][t] （∀s→t ∈ E）
* なる線形計画問題として定式化できる．
*/


//【最大流問題の双対問題】
/*
*【最大流問題の LP 定式化】の双対をとると，y[s][t], p[v] を変数とする
*	minimize	Σs→t u[s][t] y[s][t]
*	subject to	y[s][t] + p[t] - p[s] >= 0 （∀s→t ∈ E）
*				p[S] - p[T] >= 1
*				y[s][t] >= 0 （∀s→t ∈ E）
* なる線形計画問題になる．
* 
* 結局 p[v] には {0, 1} を割り当てるのが最善となり，最小カット問題と等価である．
*/


//【最小費用流問題の LP 定式化】
/*
* グラフ G = (V, E) について，
*	c[s][t] : 辺 s→t のコスト
*	u[s][t] : 辺 s→t の容量
*	F : 始点 S から終点 T までの流量
*	f[s][t] : 辺 s→t のフロー
* とおくと，最小費用流問題は，f[s][t] を変数とする
*	minimize	Σs→t c[s][t] f[s][t]
*	subject to	Σv→t f[v][t] - Σs→v f[s][v] = 0 （∀v ∈ V\{S, T}）
*				ΣS→t f[S][t] - Σs→S f[s][S] = F
*				ΣT→t f[T][t] - Σs→T f[s][T] = -F
*				0 <= f[s][t] <= u[s][t] （∀s→t ∈ E）
* なる線形計画問題として定式化できる．
*/


//【最小費用流問題の双対問題】
/*
*【最小費用流問題の LP 定式化】の双対をとると，y[s][t], p[v] を変数とする
*	maximize	-Σs→t u[s][t] y[s][t] - F p[S] + F p[T]
*	subject to	-y[s][t] + p[t] - p[s] <= c[s][t] （∀s→t ∈ E）
*				y[s][t] >= 0 （∀s→t ∈ E）
* なる線形計画問題になる．
*/


//【一般化最小費用流問題の LP 定式化】
/*
* グラフ G = (V, E) について，
*	c[e] : 辺 e のコスト
*	u[e] : 辺 e の容量
*	d[v] : 頂点 v からの湧き出し（負なら吸い込み，総和 0）
*	out[v] : 頂点 v から出る辺の集合
*	in[v] : 頂点 v に入ってくる辺の集合
*	f[e] : 辺 e のフロー
* とおくと，一般化最小費用流問題は，f[e] を変数とする
*	minimize	Σe c[e] f[e]
*	subject to	Σe∈out[v] f[e] - Σe∈in[v] f[e] = g[v] （∀v ∈ V）
*				0 <= f[e] <= u[e] （∀e ∈ E）
* なる線形計画問題として定式化できる．
*/


//【一般化最小費用流問題の双対問題】
/*
*【一般化最小費用流問題の LP 定式化】の双対をとると，y[e], p[v] を変数とする
*	maximize	-Σe u[e] y[e] - Σv d[v] p[v]
*	subject to	-y[e] + p[t] - p[s] <= c[e] （∀e = s→t ∈ E）
*				y[e] >= 0 （∀e ∈ E）
* なる線形計画問題になる．
*/


//【一般化最小費用流問題の LP 定式化（最小流量付き）】
/*
* グラフ G = (V, E) について，
*	c[e] : 辺 e のコスト
*	l[e] : 辺 e の最小流量
*	u[e] : 辺 e の最大流量（容量）
*	d[v] : 頂点 v からの湧き出し（負なら吸い込み，総和 0）
*	out[v] : 頂点 v から出る辺の集合
*	in[v] : 頂点 v に入ってくる辺の集合
*	f[e] : 辺 e のフロー
* とおくと，一般化最小費用流問題は，f[e] を変数とする
*	minimize	Σe c[e] f[e]
*	subject to	Σe∈out[v] f[e] - Σe∈in[v] f[e] = g[v] （∀v ∈ V）
*				l[e] <= f[e] <= u[e] （∀e ∈ E）
* なる線形計画問題として定式化できる．
*/


//【一般化最小費用流問題の双対問題（最小流量付き）】
/*
*【一般化最小費用流問題の LP 定式化（最小流量付き）】の双対をとると，
* x[e], y[e], p[v] を変数とする
*	maximize	Σe l[e] x[e] - Σe u[e] y[e] - Σv d[v] p[v]
*	subject to	x[e] - y[e] + p[t] - p[s] <= c[e] （∀e = s→t ∈ E）
*				x[e] >= 0 （∀e ∈ E）
*				y[e] >= 0 （∀e ∈ E）
* なる線形計画問題になる．
*/


//【一般化最小費用流問題の双対問題】
/*
* Generalized_dual_mcf(int n) : O(1)
*	変数 p[0..n) に関する最小化問題
*		minimize	Σv d[v] p[v] + ΣsΣt u[s][t] max(0, p[t] - p[s] - c[s][t])
*	を解くよう初期化する．
*
* add_const(ll c) : O(1)
*   目的関数に定数 c を加算する．
*
* add_linear(int v, ll d) : O(1)
*	目的関数に d p[v] を加算する．
*	制約：d[v] の総和は 0
*
* add_dif_relu(int s, int t, ll u, ll c) : O(1)
*	目的関数に u max(0, p[t] - p[s] - c) を加算する．
*   制約：u >= 0
*
* add_dif_abs(int s, int t, ll u, ll c) : O(1)
*	目的関数に u |p[t] - p[s] - c| を加算する．
*   制約：u >= 0
*
* add_dif_ub(int s, int t, ll ub) : O(1)
*	制約条件に p[t] - p[s] <= ub を追加する．
*	注意：ub < 0 だと流量が INF 増加して計算量がやばいかもしんない．
*
* ll solve() : O(F (n + m) log n)（F : 係数の絶対値の和，m : ReLU 項の数）
*	最小化問題を解き，目的関数値を返す．
*/
class Generalized_dual_mcf {
	// 参考 : https://www.slideshare.net/wata_orz/ss-91375739

	//【方法】
	// グラフ G = (V, E) について，
	//		c[s][t] : 辺 s→t のコスト
	//		u[s][t] : 辺 s→t の容量
	//		d[v] : 頂点 v からの湧き出し（負なら吸い込み，総和 0）
	//		out[v] : 頂点 v から出る辺の集合
	//		in[v] : 頂点 v に入ってくる辺の集合
	//		f[s][t] : 辺 s→t のフロー
	// とおくと，一般化最小費用流問題は，f[s][t] を変数とする
	//		minimize	Σe c[e] f[e]
	//		subject to	Σe∈out[v] f[e] - Σe∈in[v] f[e] = g[v] （∀v ∈ V）
	//					0 <= f[e] <= u[e] （∀e ∈ E）
	// なる線形計画問題として定式化できる．
	//
	// これの双対を考えると，y[s][t], p[v] を変数とする
	//		maximize	-ΣsΣt u[s][t] y[s][t] - Σv d[v] p[v]
	//		subject to	-y[s][t] + p[t] - p[s] <= c[s][t] （∀s→t ∈ E）
	//					y[s][t] >= 0 （∀s→t ∈ E）
	// なる線形計画問題になる．
	//
	// y[s][t] に課された不等式制約は
	//		y[s][t] >= p[t] - p[s] - c[s][t]
	//		y[s][t] >= 0
	// であり，u[s][t] >= 0 より目的関数の最大化のためには y[s][t] は小さくすべきなので，
	//		y[s][t] = max(0, p[t] - p[s] - c[s][t])
	// とするのが最善である．これを踏まえた上で目的関数を -1 倍すれば，p[v] を変数とする
	//		minimize	Σv d[v] p[v] + ΣsΣt u[e] max(0, p[t] - p[s] - c[e])
	// なる最適化問題の解は，元の問題の解の -1 倍に一致する．

	// n : 変数の数
	int n;
	mcf_graph<ll, ll> g; // 双対な最小費用流問題を解くためのグラフ

	// adv_cost : 前払いしているコスト
	ll adv_cost = 0;

	// div[i] : 頂点 i からの湧き出し（負値なら吸い込み）
	vl div;

public:
	// n 変数で初期化
	Generalized_dual_mcf(int n_) : n(n_), g(n + 2), div(n) {}

	// 目的関数に定数 c を加算する．
	void add_const(ll c) {
		// verify : https://onlinejudge.u-aizu.ac.jp/problems/2230

		// 前払いしているコストを c 減らしておく．
		adv_cost -= c;
	}

	// 目的関数に d p[v] を加算する．
	void add_linear(int v, ll d) {
		// verify : https://onlinejudge.u-aizu.ac.jp/problems/2230

		// 頂点 v に湧き出し量 d を加算する．
		div[v] += d;
	}

	// 目的関数に u max(0, p[t] - p[s] - c) を加算する．
	void add_dif_relu(int s, int t, ll u, ll c) {
		// s から t に流用 u でコスト c の辺を張りたい．

		// コストが非負の場合
		if (c >= 0) {
			// 普通に張ればいい．
			g.add_edge(s, t, u, c);
		}
		// コストが負の場合
		else {
			// 予めフローを最大まで流しておき，そこを逆流することに正のコストをかければよい．
			adv_cost += u * c;
			div[s] -= u;
			div[t] += u;
			g.add_edge(t, s, u, -c);
		}
	}

	// 目的関数に u |p[t] - p[s] - c| を加算する．
	void add_dif_abs(int s, int t, ll u, ll c) {
		// 一般に，絶対値は ReLU を用いて
		//		|x| = max(0, x) + max(0, -x)
		// と分解できる．よって目的関数に
		//		u max(0, p[t] - p[s] - c) + u max(0, p[s] - p[t] + c)
		// を加算すればよい．

		add_dif_relu(s, t, u, c);
		add_dif_relu(t, s, u, -c);
	}

	// 制約条件に p[t] - p[s] <= ub を追加する．
	void add_dif_ub(int s, int t, ll ub) {
		// verify : https://onlinejudge.u-aizu.ac.jp/problems/2230

		// p[t] - p[s] - ub > 0 となっている場合に目的関数値を ∞ 増加させればいい．
		// そのためには，目的関数に
		//		∞ max(0, p[t] - p[s] - ub)
		// を加算すればよい．

		add_dif_relu(s, t, INF, ub);
	}

	// 最小化問題を解き，目的関数値を返す．
	ll solve() {
		// verify : https://onlinejudge.u-aizu.ac.jp/problems/2230

		// ST : 始点（湧き出しへ），GL : 終点（吸い込みから）
		int ST = n, GL = n + 1;

		// 湧き出しへは始点から，吸い込みから終点へと辺を繋ぐ．
		rep(i, n) {
			if (div[i] > 0) g.add_edge(ST, i, div[i], 0);
			else if (div[i] < 0) g.add_edge(i, GL, -div[i], 0);
		}

		// 最小費用流を求める．
		ll cap, cost;
		tie(cap, cost) = g.flow(ST, GL);

		return -(adv_cost + cost);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Generalized_dual_mcf& g) {
		os << "cost: " << g.adv_cost << endl;
		os << "div: " << g.div << endl;
		os << "graph:" << endl << g.g;
		return os;
	}
#endif
};


