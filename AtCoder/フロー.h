#pragma once
#include "header.h"
#include "構造(グラフ).h"
// ■■■■■ フロー ■■■■■


//【最小費用流（流量が自由）】
/*
* 流量を自由としてコストの最小化を行いたい場合，始点から終点まで容量 ∞，コスト 0 の辺を張れば良い．
*
* verify : https://onlinejudge.u-aizu.ac.jp/problems/2293
*/


//【最小費用流（負コスト可，DAG）】
/*
* Negative_mcf_graph(int n) : O(1)
*	n 頂点で初期化する．
*
* add_edge(int s, int t, ll cap, ll cost) : O(1)
*	s から t へ容量 cap，コスト cost の辺を追加する．
*
* pll flow(int ST, int GL, ll f_lim = INFL) : O(F (n + m) log n)（F:流量，m:辺の数）
*	ST から GL まで flow_limit まで流せるだけ流したときの {流量, 最小コスト} を返す．
*	制約：閉路は存在しない
*/
struct Negative_mcf_graph_DAG {
	// 参考 : https://ikatakos.com/pot/programming_algorithm/graph_theory/minimum_cost_flow

	// n : 頂点数
	int n;

	// 辺
	struct Edge {
		int to;
		ll cap, cost;

		Edge(int to_, ll cap_, ll cost_) : to(to_), cap(cap_), cost(cost_) {}

#ifdef _MSC_VER
		friend ostream& operator<<(ostream& os, const Edge& e) {
			os << "(to:" << e.to << ", cap:" << e.cap << ", cost:" << e.cost << ")";
			return os;
		}
#endif
	};

	// 元のグラフ（負辺あり）
	vector<vector<Edge>> g;

	// pot[s] : 頂点 s のポテンシャル
	vl pot;

	// n 頂点で初期化する．
	Negative_mcf_graph_DAG(int n_) : n(n_), g(n), pot(n, INFL) {}

	// s から t へ容量 cap，コスト cost の辺を追加する．
	void add_edge(int s, int t, ll cap, ll cost) {
		// verify : https://onlinejudge.u-aizu.ac.jp/problems/2266

		g[s].emplace_back(t, cap, cost);
	}

	// DAG 上の DP で GL までの距離を求め，その -1 倍をポテンシャルとする．
	void DAG_DP(int GL) {
		pot[GL] = 0;

		vb seen(n);
		seen[GL] = true;

		function<ll(int)> dfs = [&](int s) {
			if (seen[s]) return pot[s];
			seen[s] = true;

			repe(e, g[s]) chmin(pot[s], dfs(e.to) + e.cost);

			return pot[s];
		};

		// 各頂点 s についての情報を計算する．
		rep(s, n) if (!seen[s]) dfs(s);

		rep(s, n) pot[s] *= -1;
	}

	// ST から GL まで flow_limit まで流せるだけ流したときの {流量, 最小コスト} を返す．
	pll flow(int ST, int GL, ll f_lim = INFL) {
		// verify : https://onlinejudge.u-aizu.ac.jp/problems/2266

		// DAG 上の DP で v から GL までの距離を求め，その -1 倍をポテンシャル pot[v] とする．
		DAG_DP(GL);

		// g_pos : 辺 s→t のコストが (元の辺のコスト) - (pot[t] - pot[s]) >= 0 であるようなグラフ
		mcf_graph<ll, ll> g_pos(n);
		rep(s, n) repe(e, g[s]) {
			g_pos.add_edge(s, e.to, e.cap, e.cost - (pot[e.to] - pot[s]));
		}

		// g_pos の最小費用流を求める．
		ll cap, cost;
		tie(cap, cost) = g_pos.flow(ST, GL, f_lim);

		// 実際のコストは (流量) * (pot[GL] - pot[ST]) を加えたものになる．
		cost += cap * (pot[GL] - pot[ST]);

		return make_pair(cap, cost);
	}
};


//【最小費用流（負コスト可，負コスト閉路なし）】
/*
* Negative_mcf_graph(int n) : O(1)
*	n 頂点で初期化する．
*
* add_edge(int s, int t, ll cap, ll cost) : O(1)
*	s から t へ容量 cap，コスト cost の辺を追加する．
*
* pll flow(int ST, int GL, ll f_lim = INFL) : O(n m + F (n + m) log n)（F:流量，m:辺の数）
*	ST から GL まで flow_limit まで流せるだけ流したときの {流量, 最小コスト} を返す．
*	制約：負コストの閉路は存在しない
*/
struct Negative_mcf_graph {
	// 参考 : https://ikatakos.com/pot/programming_algorithm/graph_theory/minimum_cost_flow

	// n : 頂点数
	int n;

	// 辺
	struct Edge {
		int to;
		ll cap, cost;

		Edge(int to_, ll cap_, ll cost_) : to(to_), cap(cap_), cost(cost_) {}
	};

	// 元のグラフ（負辺あり）
	vector<vector<Edge>> g;

	// pot[s] : 頂点 s のポテンシャル
	vl pot;

	// n 頂点で初期化する．
	Negative_mcf_graph(int n_) : n(n_), g(n), pot(n, INFL) {}

	// s から t へ容量 cap，コスト cost の辺を追加する．
	void add_edge(int s, int t, ll cap, ll cost) {
		// verify : https://atcoder.jp/contests/code-festival-2014-china-open/tasks/code_festival_china_h

		g[s].emplace_back(t, cap, cost);
	}

	// ベルマンフォード法で ST からの距離を求め，それをポテンシャルとする．
	bool bellman_ford(int ST) {
		pot[ST] = 0;

		rep(i, n) {
			bool updated = false;

			// 全ての辺についての操作
			rep(s, n) repe(e, g[s]) {
				// もし (始点への距離) + (辺のコスト) < (終点への距離) なら (終点への距離) を更新する．
				// INFL からは何を引いても INFL になるようにしているので，ST から到達可能な負閉路しか検出しない．
				if (pot[s] != INFL && pot[s] + e.cost < pot[e.to]) {
					pot[e.to] = pot[s] + e.cost;
					updated = true;
				}
			}

			// もし距離の更新が起こらなければ最短距離確定
			if (!updated) return true;
		}

		// もし全ての辺についての操作を n 回繰り返しても距離の更新があったなら，
		// ST から到達可能な負の閉路を持っているので false を返す．
		return false;
	}

	// ST から GL まで flow_limit まで流せるだけ流したときの {流量, 最小コスト} を返す．
	pll flow(int ST, int GL, ll f_lim = INFL) {
		// verify : https://atcoder.jp/contests/code-festival-2014-china-open/tasks/code_festival_china_h

		// ベルマンフォード法で ST から v までの距離を求め，それをポテンシャル pot[v] とする．
		bool non_neg_cyc = bellman_ford(ST);
		Assert(non_neg_cyc);

		// g_pos : 辺 s→t のコストが (元の辺のコスト) - (pot[t] - pot[s]) >= 0 であるようなグラフ
		mcf_graph<ll, ll> g_pos(n);
		rep(s, n) repe(e, g[s]) {
			g_pos.add_edge(s, e.to, e.cap, e.cost - (pot[e.to] - pot[s]));
		}

		// g_pos の最小費用流を求める．
		ll cap, cost;
		tie(cap, cost) = g_pos.flow(ST, GL, f_lim);

		// 実際のコストは (流量) * pot[GL] を加えたものになる．
		cost += cap * pot[GL];

		return make_pair(cap, cost);
	}
};


//【一般化最大利得流】
/*
* Generalized_max_profit_flow(int n) : O(1)
*	n 頂点で初期化する．
*
* add_source(int s, ll cap) : O(1)
*	頂点 s に湧き出し量 cap を加算する（cap が負値なら吸い込み）
*	制約：湧き出し量の総和は 0
*
* add_cost_edge(int s, int t, ll cap, ll cost) : O(1)
*	頂点 s から頂点 t まで，流量上限が cap で非負コストが cost である辺を張る．
*
* add_profit_edge(int s, int t, ll cap, ll prof) : O(1)
*	頂点 s から頂点 t まで，流量上限が cap で非負利得が prof である辺を張る．
*
* add_cost_edge(int s, int t, ll cap_min, ll cap_max, ll cost) : O(1)
*	頂点 s から頂点 t まで，流量下限[上限] が cap_min[ cap_max ] で非負コストが cost である辺を張る．
*
* add_profit_edge(int s, int t, ll cap_min, ll cap_max, ll prof) : O(1)
*	頂点 s から頂点 t まで，流量下限[上限] が cap_min[ cap_max ] で非負利得が prof である辺を張る．
*
* pll flow() : O(F (n + m) log n)（F : 流量，m : 辺の数）
*	湧き出しから吸い込みまでなるだけ多くフローを流し，最大流量とそのときの最大利得の組を返す．
*	循環流には要注意．可能なら SCC して頂点を縮約し DAG にしてからの方が良い．
*
* add_profit 系を使うとその辺の容量分だけ流量が増え計算量が悪化するので注意．
*/
struct Generalized_max_profit_flow {
	// 参考 : https://snuke.hatenablog.com/entry/2017/06/07/115821

	// n : 頂点の数
	int n;
	mcf_graph<ll, ll> g;

	// profit : 前借りしている利得
	ll profit = 0;

	// div[i] : 頂点 i からの湧き出し（負値なら吸い込み）
	vl div;

	// n 頂点で初期化
	Generalized_max_profit_flow(int n_) : n(n_), g(n + 2), div(n) {}

	// 頂点 s に湧き出し量 cap を加算する（cap が負値なら吸い込み）．
	void add_source(int s, ll cap) {
		// verify : https://atcoder.jp/contests/tdpc/tasks/tdpc_graph

		div[s] += cap;
	}

	// 頂点 s から頂点 t まで，流量上限が cap で非負コストが cost である辺を張る．
	void add_cost_edge(int s, int t, ll cap, ll cost) {
		Assert(cost >= 0);

		// 普通に辺を張れば良い．
		g.add_edge(s, t, cap, cost);
	}

	// 頂点 s から頂点 t まで，流量上限が cap で非負利得が prof である辺を張る．
	void add_profit_edge(int s, int t, ll cap, ll prof) {
		// verify : https://atcoder.jp/contests/tdpc/tasks/tdpc_graph

		Assert(prof >= 0);

		// 予めフローを最大まで流しておき，そこを逆流することに正のコストをかければよい．
		profit += cap * prof;
		div[s] -= cap;
		div[t] += cap;
		g.add_edge(t, s, cap, prof);
	}

	// 頂点 s から頂点 t まで，流量下限[上限] が cap_min[ cap_max ] で非負コストが cost である辺を張る．
	void add_cost_edge(int s, int t, ll cap_min, ll cap_max, ll cost) {
		// verify : https://atcoder.jp/contests/jag2013summer-day4/tasks/icpc2013summer_day4_i

		if (cap_min > cap_max) return;
		Assert(cost >= 0);

		// 予めフローを cap_min だけ流しておけばいい．
		profit -= cap_min * cost;
		div[s] -= cap_min;
		div[t] += cap_min;
		g.add_edge(s, t, cap_max - cap_min, cost);
	}

	// 頂点 s から頂点 t まで，流量下限[上限] が cap_min[ cap_max ] で非負利得が prof である辺を張る．
	void add_profit_edge(int s, int t, ll cap_min, ll cap_max, ll prof) {
		if (cap_min > cap_max) return;
		Assert(prof >= 0);

		// 予めフローを最大まで流しておき，そこを逆流することに非負のコストをかければいい．
		profit += cap_max * prof;
		div[s] -= cap_max;
		div[t] += cap_max;
		g.add_edge(t, s, cap_max - cap_min, prof);
	}

	// 湧き出しから吸い込みまでなるだけ多くフローを流し，最大流量とそのときの最大利得の組を返す．
	pll flow() {
		// verify : https://atcoder.jp/contests/tdpc/tasks/tdpc_graph

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

		return make_pair(cap, profit - cost);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Generalized_max_profit_flow& g) {
		os << "profit: " << g.profit << endl;
		os << "div: " << g.div << endl;
		os << "graph:" << endl << g.g;
		return os;
	}
#endif
};


//【最大流問題】O(|E| maxflow)
/*
* コスト付き有向グラフ g の始点 s から終点 t までの最大フローの大きさを返す．
*/
ll ford_fullkerson(const WGraph& g, int s, int t) {
	// 参考：https://algo-logic.info/ford-fullkerson/
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_6_A

	int n = sz(g);

	// 残余ネットワークを作り初期化する．
	// また更新のために逆向きの辺の番号を記録しておく．
	WGraph res(n);
	vvi rev(n);
	rep(i, n) repe(e, g[i]) {
		// 順方向は g と同じ，逆方向は 0 で初期化．
		res[i].push_back(e);
		res[e.to].push_back({ i, 0 });

		// e = res[i][j] の逆向きの辺は res[e.to][rev[i][j]]
		rev[i].push_back(sz(res[e.to]) - 1);
		rev[e.to].push_back(sz(res[i]) - 1);
	}

	vb seen(n);
	function<ll(int, ll)> dfs = [&](int v, ll f) {
		// 終点 t まで流せたら流量を返す．
		if (v == t) return f;

		// 頂点 v に訪れたことを記録する．
		seen[v] = true;

		// v から出ている残余ネットワークの各辺 e について
		rep(j, sz(res[v])) {
			auto& e = res[v][j];

			// e の先の頂点に既に訪れていたり，e に空きが無いなら何もしない．
			if (seen[e.to] || e.cost == 0) continue;

			// e にフローを流す．
			ll f2 = dfs(e.to, min(f, e.cost));

			// もしフローが流れたら残余ネットワークを更新し流量を返す．
			if (f2 > 0) {
				e.cost -= f2;
				res[e.to][rev[v][j]].cost += f2;
				return f2;
			}
		}

		// ここまでくるのはフローを流せなかったとき
		return 0LL;
	};

	ll mf = 0;
	while (true) {
		// どの頂点にも訪れていない状態にする．
		seen.assign(n, false);

		// 始点 s からフローを流す．
		ll f = dfs(s, INFL);

		// フローが流れなかったら最大まで流し切ったので終了．
		if (f == 0) break;

		// フローが流れたら流量を更新する．
		mf += f;
	}

	return mf;
}


