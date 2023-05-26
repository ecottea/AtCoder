#pragma once
#include "header.h"
#include "構造(グラフ).h"
// ■■■■■ フロー ■■■■■


//【最大流（最小流量制約付き）】
/*
* Mf_graph_lb(int n) : O(1)
*	n 頂点で初期化する．
*
* add_edge(int s, int t, ll cap) : O(1)
*	s から t へ容量 cap の辺を追加する．
*
* add_edge(int s, int t, ll lb, ll cap) : O(1)
*	s から t へ最小流量 lb，容量 cap の辺を追加する．
*
* ll flow(int s, int t) : O(n^2 m)（m : 辺の数）
*	s から t まで流せるだけ流したときの流量を返す（不可能なら -1）
*/
struct Mf_graph_lb {
	// 参考 : https://atcoder.jp/contests/abc285/editorial/5500

	int n; // 頂点の数
	mf_graph<ll> g; // グラフ
	vl div; // 湧き出し

	// n 頂点で初期化する．
	Mf_graph_lb(int n) : n(n), g(n + 2), div(n) {
		// verify : https://atcoder.jp/contests/abc285/tasks/abc285_g
	}

	// s から t へ容量 cap の辺を追加する．
	void add_edge(int s, int t, ll cap) {
		// verify : https://atcoder.jp/contests/abc285/tasks/abc285_g

		if (s == t || cap == 0) return;
		g.add_edge(s, t, cap);
	}

	// s から t へ最小流量 lb，容量 cap の辺を追加する．
	void add_edge(int s, int t, ll lb, ll cap) {
		// verify : https://atcoder.jp/contests/abc285/tasks/abc285_g

		chmax(lb, 0LL);
		Assert(lb <= cap);
		if (s == t || cap == 0) return;

		// lb だけ流したことにし，辺の容量はその分だけ差し引いておく．
		g.add_edge(s, t, cap - lb);

		// 辺の終点に lb 分だけの湧き出し，始点に lb 分だけの吸い込みを設定する．
		div[s] -= lb;
		div[t] += lb;
	}

	// s から t まで流せるだけ流したときの流量を返す（不可能なら -1）
	ll flow(int s, int t) {
		// verify : https://atcoder.jp/contests/abc285/tasks/abc285_g

		int ST = n, GL = n + 1;
		ll sum = 0;

		// 超始点から湧き出しへ，吸い込みから超終点へと辺を張る．
		rep(i, n) {
			if (div[i] > 0) {
				g.add_edge(ST, i, div[i]);
				sum += div[i];
			}
			else {
				g.add_edge(i, GL, -div[i]);
			}
		}

		ll f_SG = g.flow(ST, GL);
		ll f_St = g.flow(ST, t);
		ll f_sG = g.flow(s, GL);
		ll f_st = g.flow(s, t);
		dump(f_SG, f_St, f_sG, f_st);

		if (f_SG + f_St < sum) return -1;
		if (f_SG + f_sG < sum) return -1;
		return f_sG + f_st;
	}
};


//【最小費用流（負コスト可，DAG）】
/*
* Negative_mcf_graph(int n) : O(1)
*	n 頂点で初期化する．
*
* add_edge(int s, int t, ll cap, ll cost) : O(1)
*	s から t へ容量 cap，コスト cost の辺を追加する．
*
* pll flow(int ST, int GL, ll f_lim = INFL) : O(F (n + m) log n)（F:流量，m:辺の数）
*	ST から GL まで f_lim まで流せるだけ流したときの {流量, 最小コスト} を返す．
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

	// g : 元のグラフ（負辺あり）
	vector<vector<Edge>> g;

	// pot[s] : 頂点 s のポテンシャル
	vl pot;

	// g_pos : ポテンシャル分だけ下駄を履かせて非負の辺のみにしたグラフ
	mcf_graph<ll, ll> g_pos;

	// n 頂点で初期化する．
	Negative_mcf_graph_DAG(int n_) : n(n_), g(n), pot(n, INFL), g_pos(n) {
		// verify : https://atcoder.jp/contests/practice2/tasks/practice2_e
	}

	// s から t へ容量 cap，コスト cost の辺を追加する．
	void add_edge(int s, int t, ll cap, ll cost) {
		// verify : https://atcoder.jp/contests/practice2/tasks/practice2_e

		g[s].emplace_back(t, cap, cost);
	}

	// DAG 上の DP で GL までの距離を求め，その -1 倍をポテンシャルとする．: O(n + m)
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

	// ST から GL まで f_lim まで流せるだけ流したときの {流量, 最小コスト} を返す．
	pll flow(int ST, int GL, ll f_lim = INFL) {
		// verify : https://atcoder.jp/contests/practice2/tasks/practice2_e

		// DAG 上の DP で v から GL までの距離を求め，その -1 倍をポテンシャル pot[v] とする．
		DAG_DP(GL);

		// g_pos : 辺 s→t のコストが (元の辺のコスト) - (pot[t] - pot[s]) >= 0 であるようなグラフ
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

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Negative_mcf_graph_DAG& g) {
		rep(s, g.n) {
			os << s << ": ";
			repe(e, g.g[s]) os << e << " ";
			os << endl;
		}
		return os;
	}
#endif
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
*	ST から GL まで f_lim まで流せるだけ流したときの {流量, 最小コスト} を返す．
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


//【一般化最小費用流】
/*
* Generalized_min_cost_flow(int n) : O(1)
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
*	湧き出しから吸い込みまでなるだけ多くフローを流し，最大流量とそのときの最小費用の組を返す．
*	循環流には要注意．可能なら SCC して頂点を縮約し DAG にしてからの方が良い．
*
* add_profit 系を使うとその辺の容量分だけ流量が増え計算量が悪化するので注意．
*/
struct Generalized_min_cost_flow {
	// 参考 : https://snuke.hatenablog.com/entry/2017/06/07/115821

	// n : 頂点の数
	int n;
	mcf_graph<ll, ll> g;

	// precost : 前払いしているコスト
	ll precost = 0;

	// div[i] : 頂点 i からの湧き出し（負値なら吸い込み）
	vl div;

	// n 頂点で初期化
	Generalized_min_cost_flow(int n_) : n(n_), g(n + 2), div(n) {}
	Generalized_min_cost_flow() : n(0), precost(0) {}

	// 頂点 s に湧き出し量 cap を加算する（cap が負値なら吸い込み）．
	void add_source(int s, ll cap) {
		// verify : https://atcoder.jp/contests/tdpc/tasks/tdpc_graph

		div[s] += cap;
	}

	// 頂点 s から頂点 t まで，流量上限が cap で非負コストが cost である辺を張る．
	void add_cost_edge(int s, int t, ll cap, ll cost) {
		// verify : https://atcoder.jp/contests/abc231/tasks/abc231_h

		Assert(cost >= 0);

		// 普通に辺を張れば良い．
		g.add_edge(s, t, cap, cost);
	}

	// 頂点 s から頂点 t まで，流量上限が cap で非負利得が prof である辺を張る．
	void add_profit_edge(int s, int t, ll cap, ll prof) {
		// verify : https://atcoder.jp/contests/tdpc/tasks/tdpc_graph

		Assert(prof >= 0);

		// 予めフローを最大まで流しておき，そこを逆流することに正のコストをかければよい．
		precost -= cap * prof;
		div[s] -= cap;
		div[t] += cap;
		g.add_edge(t, s, cap, prof);
	}

	// 頂点 s から頂点 t まで，流量下限[上限] が cap_min[ cap_max ] で非負コストが cost である辺を張る．
	void add_cost_edge(int s, int t, ll cap_min, ll cap_max, ll cost) {
		// verify : https://atcoder.jp/contests/abc231/submissions/me

		if (cap_min > cap_max) return;
		Assert(cost >= 0);

		// 予めフローを cap_min だけ流しておけばいい．
		precost += cap_min * cost;
		div[s] -= cap_min;
		div[t] += cap_min;
		g.add_edge(s, t, cap_max - cap_min, cost);
	}

	// 頂点 s から頂点 t まで，流量下限[上限] が cap_min[ cap_max ] で非負利得が prof である辺を張る．
	void add_profit_edge(int s, int t, ll cap_min, ll cap_max, ll prof) {
		if (cap_min > cap_max) return;
		Assert(prof >= 0);

		// 予めフローを最大まで流しておき，そこを逆流することに非負のコストをかければいい．
		precost -= cap_max * prof;
		div[s] -= cap_max;
		div[t] += cap_max;
		g.add_edge(t, s, cap_max - cap_min, prof);
	}

	// 湧き出しから吸い込みまでなるだけ多くフローを流し，最大流量とそのときの最大利得の組を返す．
	pll flow() {
		// verify : https://atcoder.jp/contests/tdpc/tasks/tdpc_graph

		// ST : 始点（湧き出しへ），GL : 終点（吸い込みから）
		int ST = n, GL = n + 1;

		// 湧き出しへは始点から，吸い込みからは終点へと辺を繋ぐ．
		rep(i, n) {
			if (div[i] > 0) g.add_edge(ST, i, div[i], 0);
			else if (div[i] < 0) g.add_edge(i, GL, -div[i], 0);
		}

		// 最小費用流を求める．
		ll cap, cost;
		tie(cap, cost) = g.flow(ST, GL);

		return make_pair(cap, precost + cost);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Generalized_min_cost_flow& g) {
		os << "precost: " << g.precost << endl;
		os << "div: " << g.div << endl;
		os << "graph:" << endl << g.g;
		return os;
	}
#endif
};


//【流量が自由】
/*
* 流量を自由としてコストの最小化を行いたい場合，始点から終点まで容量 ∞，コスト 0 の辺を張れば良い．
*
* verify : https://atcoder.jp/contests/practice2/tasks/practice2_e
*/


//【流量 f まで利得 p】
/*
* 流量 f までは利得 p が得られ，それより多く流しても利得が得られない辺を張りたい場合，
* 流量 f で利得 p の辺と，流量 ∞ で利得 0 の辺を組にして張れば良い．
*
* verify : https://atcoder.jp/contests/tdpc/tasks/tdpc_graph
*/


//【双対問題】
/*
* フロー双対.h へ
*/


//【最大流問題】O(maxflow (n + m))
/*
* 重み付き有向グラフ g の始点 s から終点 t までの最大フローの大きさを返す．
*/
ll ford_fullkerson(const WGraph& g, int s, int t) {
	// 参考：https://algo-logic.info/ford-fullkerson/
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_6_A

	int n = sz(g);

	// 残余ネットワークを作り初期化する．
	// また更新のために逆向きの辺の番号を記録しておく．
	WGraph res(n); vvi rev(n);
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


//【最小費用流問題】
/*
* Ford_fullkerson(int n) : O(n)
*	n 頂点で初期化する．
*
* add_edge(int s, int t, ll cap, ll cost) : O(1)
*	s から t への容量 cap，コスト cost の辺を追加する．
*
* pll flow(int ST, int GL, ll f_lim = INFL) : O(maxflow (n + m) log n)
*	ST から GL まで f_lim まで流せるだけ流したときの {流量, 最小コスト} を返す．
*/
class Ford_fullkerson {
	// 参考 : https://ikatakos.com/pot/programming_algorithm/graph_theory/minimum_cost_flow

	struct Edge {
		int from; // どの頂点から出る辺か
		int to; // どの頂点に行く辺か
		ll cap; // 空き容量
		ll cost; // コスト
		Edge* rev; // 逆辺

		Edge(int from, int to, ll cap, ll cost, Edge* rev = nullptr) :
			from(from), to(to), cap(cap), cost(cost), rev(rev) {}

#ifdef _MSC_VER
		friend ostream& operator<<(ostream& os, const Edge& v) {
			os << "(" << v.from << "→" << v.to << ":" << v.cap << "/" << v.cost << ")";
			return os;
		}
#endif
	};

	int n;
	vector<vector<Edge*>> g;

public:
	Ford_fullkerson(int n) : n(n), g(n) {
		// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/6/GRL_6_B
	}

	void add_edge(int s, int t, ll cap, ll cost) {
		// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/6/GRL_6_B

		g[s].push_back(new Edge(s, t, cap, cost));
		g[t].push_back(new Edge(t, s, 0, -cost));
		g[s].back()->rev = g[t].back();
		g[t].back()->rev = g[s].back();
	}

	// ST から GL まで最小費用流を 1 回流し，{流量, コスト} の組を返す．
	pll dijkstra(int ST, int GL) {
		vl cost(n, INFL); // ST からの最小コスト
		cost[ST] = 0;

		vl flow(n, INFL); // ST から流せる流量

		vector<Edge*> p(n); // 直前に通ってきた辺

		// 組 (ST からのコスト, 頂点番号) を入れる優先度付きキュー
		priority_queue_rev<pli> q;
		q.push({ 0, ST });

		while (!q.empty()) {
			auto [c, s] = q.top(); q.pop();

			// GL までのパスを見つけたら終了．
			if (s == GL) break;

			// すでにより小さいコストに更新されていたなら何もしない（忘れると O(n^2)）
			if (cost[s] < c) continue;

			repe(e, g[s]) {
				// 空き容量がなければフローを流さない．
				if (e->cap == 0) continue;

				// コストが更新できないならフローを流さない．
				if (cost[s] + e->cost >= cost[e->to]) continue;

				// コストを更新し，その先も探索する．
				cost[e->to] = cost[s] + e->cost;
				flow[e->to] = min(flow[s], e->cap);
				p[e->to] = e;
				q.push({ cost[e->to], e->to });
			}
		}

		// フローが流せない場合は終了．
		if (cost[GL] == INFL) return { 0, 0 };

		// フローを流し，空き容量を更新する．
		int s = GL;
		while (s != ST) {
			p[s]->cap -= flow[GL];
			p[s]->rev->cap += flow[GL];

			s = p[s]->from;
		}

		return { flow[GL], cost[GL] };
	}

	pll flow(int ST, int GL, ll flow_limit = INFL) {
		// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/6/GRL_6_B

		ll flow_sum = 0, cost_sum = 0;

		while (true) {
			// 始点 s からフローを流す．
			auto [f, c] = dijkstra(ST, GL);

			// フローが流れなかったら最大まで流し切ったので終了．
			if (f == 0) break;

			// フローが上限を超えて流せるなら打ち切り．
			if (flow_sum + f >= flow_limit) {
				f = flow_limit - flow_sum;
				flow_sum += f;
				cost_sum += c * f;
				break;
			}

			// フローが流れたら流量とコストを更新する．
			flow_sum += f;
			cost_sum += c * f;
		}

		return { flow_sum, cost_sum };
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Ford_fullkerson& g) {
		rep(s, g.n) {
			os << s << ": ";
			repe(e, g.g[s]) os << *e << " ";
			os << endl;
		}
		return os;
	}
#endif
};


