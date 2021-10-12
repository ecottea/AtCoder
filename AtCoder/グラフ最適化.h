#pragma once
#include "header.h"
#include "構造(グラフ).h"
#include "マッチング.h"
#include "ビット全探索.h"
// ■■■■■ グラフ上の最適化問題 ■■■■■



//【最短パス】O(|E|)
/*
* グラフ g の始点 st から終点 gl までの最短パスの長さを返す．
* gl = st なら最短サイクルの長さを返す．到達不能なら INF を返す．
* 必要なら path に最短パス上の頂点の列を格納する．
*
*（幅優先探索）
*/
int shortest_path(const Graph& g, int st, int gl, vi* path = nullptr) {
	int n = sz(g);

	vi dist(n, INF); // st からの最短距離を保持するテーブル
	dist[st] = (st == gl ? INF : 0);

	vi p(n); // 1 つ手前の頂点を記録しておくテーブル（復元用）
	p[st] = -1;

	queue<int> que; // 次に探索する頂点を入れておくキュー
	que.push(st);

	while (!que.empty()) {
		// 未探索の頂点を 1 つ得る．
		auto s = que.front();
		que.pop();

		for (auto t : g[s]) {
			if (dist[t] != INF) {
				// 発見済みの頂点なので何もしない．
				continue;
			}

			// スタートからの最短距離を確定する．
			// 幅優先探索なので，最短だという保証がある．
			dist[t] = (s == st ? 0 : dist[s]) + 1;
			p[t] = s;

			// gl への最短距離が確定したなら終了．
			if (t == gl) {
				goto LOOP_END;
			}

			// 未探索の頂点として t を追加する．
			que.push(t);
		}
	}
LOOP_END:;

	// st から gl まで到達不能の場合
	int d = dist[gl];
	if (d == INF) {
		return INF;
	}

	// 必要なら経路復元を行う．
	if (path != nullptr) {
		*path = vi(d + 1);

		int t = gl, i = d;

		do {
			(*path)[i--] = t;
			t = p[t];
		} while (t != st);

		(*path)[0] = st;
	}

	return d;
}


//【単一始点最短路／ダイクストラ法】O(|E| log|V|)
/*
* 非負のコスト付きグラフ g に対し，始点を st として
* ダイクストラ法を用いて最小コスト経路問題を解き，
* st から各頂点 i への最小コストを cost[i] に格納する．
*/
void dijkstra(const WGraph& g, int st, vl& cost) {
	int n = sz(g);
	cost = vl(n, INFL); // スタートからの最小コストを保持するテーブル

	// 組 (スタートからのコスト, 頂点番号) を入れる優先度付きキューを用意する．
	// スタートからのコストがより小さいものを優先的に取り出す．
	priority_queue_rev<pli> que;
	que.push({ 0, st });

	while (!que.empty()) {
		ll c;
		int s;
		tie(c, s) = que.top();
		que.pop();

		// もし既に最小コストが求まっているなら何もしない．
		if (c >= cost[s]) {
			continue;
		}

		// 最小コストの決定
		// 優先度付きキューでコストの小さい順に取り出しており，
		// かつコストが非負より三角不等式が成立するので最短の保証がある．
		cost[s] = c;

		// そこから移動できるノードについての情報をキューに追加する．
		for (auto e : g[s]) {
			que.push({ c + e.cost, e.to });
		}
	}
}


//【単一始点最短路／ダイクストラ法（頂点コスト）】O(|E| log|V|)
/*
* 頂点に非負のコスト vc が与えられたグラフ g に対し，始点を st として
* ダイクストラ法を用いて最小コスト経路問題を解き，
* st から各頂点 i への最小コストを cost[i] に格納する．
*/
void dijkstra(const Graph& g, const vl& vc, int st, vl& cost) {
	int n = sz(g);
	cost = vl(n, INFL); // スタートからの最小コストを保持するテーブル

	// 組 (スタートからのコスト, 頂点番号) を入れる優先度付きキューを用意する．
	// スタートからのコストがより小さいものを優先的に取り出す．
	priority_queue_rev<pli> que;
	que.push({ vc[st], st });

	while (!que.empty()) {
		ll c;
		int s;
		tie(c, s) = que.top();
		que.pop();

		// もし既に最小コストが求まっているなら無視
		if (c >= cost[s]) {
			continue;
		}

		// 最小コストの決定
		// 優先度付きキューでコストの小さい順に取り出しているので最小の保証がある．
		cost[s] = c;

		// そこから移動できるノードについての情報をキューに追加する．
		for (auto t : g[s]) {
			que.push({ c + vc[t], t });
		}
	}
}


//【単一始点最短路／ベルマン・フォード法】O(|E| |V|)
/*
* コスト付きグラフ g（負のコストも可）に対し，始点を st として
* ベルマン・フォード法を用いて最小コスト経路問題を解き，
* st から各頂点 i への最小コストを cost[i] に格納する．
* もし負のコストをもつ閉路があれば false を返す．
*/
bool bellman_ford(const WGraph& g, int st, vl& cost) {
	int n = sz(g);
	cost = vl(n, INFL); // スタートからの最小コストを保持するテーブル
	cost[st] = 0;

	rep(i, n) {
		bool updated = false;

		// 全ての辺についての操作
		rep(s, n) {
			for (auto e : g[s]) {
				// もし (始点へのコスト) + (辺のコスト) < (終点へのコスト) なら
				// (終点へのコスト) を更新する．
				// INFL には何を足しても INFL になるようにしている．
				if (cost[s] != INFL && cost[s] + e.cost < cost[e.to]) {
					cost[e.to] = cost[s] + e.cost;
					updated = true;
				}
			}
		}

		// もしコストの更新が起こらなければ最小コスト確定
		if (!updated) {
			return true;
		}
	}

	// もし全ての辺についての操作を |V| 回繰り返してもコストの更新があったなら，
	// 負の閉路を持っているので false を返す．
	return false;
}


//【全頂点対最短路／ワーシャルフロイド法】O(|V|^3)
/*
* コスト付きグラフ g（負のコストも可）に対し，ワーシャルフロイド法を用いて
* 全頂点対 (i, j) に関する最小コスト経路問題を解き，結果を cost[i][j] に格納する．
* もし負の閉路をもっていれば false を返す．
*/
bool warshall_floyd(const WGraph& g, vvl& cost) {
	int n = (int)g.size();
	cost = vvl(n, vl(n, INFL)); // 頂点対の最小コストを保持するテーブル
	rep(i, n) {
		cost[i][i] = 0;
	}
	rep(s, n) {
		for (auto t : g[s]) {
			// 多重辺に対応するため chmin を用いている．
			chmin(cost[s][t.to], t.cost);
		}
	}

	rep(k, n) {
		// 途中で通っていいのが 0 から k までの頂点のとき
		rep(i, n) {
			rep(j, n) {
				// 新しく通れるようになった k を通る方がコストが小さければ更新
				// （一時配列に退避させず計算してしまっているので途中は間違った値
				// になっているが，より小さい値になるだけなので最後には合う．）
				cost[i][j] = min(cost[i][j], cost[i][k] + cost[k][j]);

				// ∞ からは何を引いても ∞ になっていて欲しい．
				if (cost[i][j] > INFL / 2) {
					cost[i][j] = INFL;
				}
			}
		}
	}

	// 負の閉路を持っていれば false を返す
	rep(i, n) {
		if (cost[i][i] < 0) {
			return false;
		}
	}
	return true;
}


//【最小全域森／クラスカル法】O(|E| log|V|)
/*
* クラスカル法でコスト付き無向グラフ g の最小全域森を求める．
* 最小全域森は msf に構成し，各最小全域木の代表元を mst に格納する．
* また戻り値として最小コストを返す．
*/
ll kruskal(const WGraph& g, WGraph& msf, vi& mst) {
	// 参考 : https://ja.wikipedia.org/wiki/%E3%82%AF%E3%83%A9%E3%82%B9%E3%82%AB%E3%83%AB%E6%B3%95

	int n = sz(g);
	msf = WGraph(n);

	// 辺を集めてコスト昇順にソートする．
	vector<tuple<ll, int, int>> edges;
	rep(i, n) {
		repe(v, g[i]) {
			edges.push_back({ v.cost, i, v.to });
		}
	}
	sort(all(edges));

	ll cost = 0; // 最小コスト
	dsu d(n + 1); // 連結判定用
	rep(i, sz(edges)) {
		// もし辺の両端が既に連結なら繋がない．
		int s = get<1>(edges[i]);
		int t = get<2>(edges[i]);
		if (d.same(s, t)) {
			continue;
		}

		// そうでないならコスト最小の辺なのでそれで繋ぐ．
		ll c = get<0>(edges[i]);
		msf[s].push_back({ t, c });
		cost += c;
		d.merge(s, t);
	}

	// 連結成分のそれぞれが最小全域木なので，その代表元を記録．
	mst = vi();
	repe(t, d.groups()) {
		mst.push_back(t[0]);
	}

	return cost;
}


//【最小全域木／プリム法】O(|E| log|V|)
/*
* コスト付きグラフ g の頂点 r を含む連結成分の最小全域木を mst に格納する．
* また戻り値として最小コストを返す．
*/
ll prim(const WGraph& g, int r, WGraph& mst) {
	// 参考 : https://ja.wikipedia.org/wiki/%E3%83%97%E3%83%AA%E3%83%A0%E6%B3%95

	int n = sz(g);
	mst = WGraph(n);
	ll res = 0;

	// selected[v] : 頂点 v を既に選んだかどうか
	vb selected(n);
	selected[r] = true;

	// 選んだ頂点から出ている辺をコスト昇順に記録しておくための優先度付きキュー．
	using E = tuple<ll, int, int>;
	priority_queue<E, vector<E>, greater<E>> q;
	repe(e, g[r]) {
		q.push({ e.cost, r, e.to });
	}

	while (!q.empty()) {
		ll c;
		int s, t;
		tie(c, s, t) = q.top();
		q.pop();

		// 既に選んだ頂点への辺なら何もしない．
		if (selected[t]) {
			continue;
		}

		// 最小全域木に辺を追加し，頂点を選んだことを記録しておく．
		mst[s].push_back({ t, c });
		res += c;
		selected[t] = true;

		// 調べるべき辺を追加する．
		repe(e, g[t]) {
			q.push({ e.cost, t, e.to });
		}
	}

	return res;
}


//【最長パス】O(|V| + |E|)
/*
* 有向非巡回グラフ g の最長パスの長さを返す．
* 
*（DAG 上の DP）
*/
int longest_path(const Graph& g) {
	int n = sz(g);

	// dp[s] : 頂点 s からの最長パスの長さ
	vi dp(n);
	vb seen(n);

	function<int(int)> dfs = [&](int s) {
		// s の情報を計算済だったらすぐに返す．
		if (seen[s]) return dp[s];
		seen[s] = true;

		// s から行ける頂点 t の情報を元に s の情報を計算する．
		dp[s] = 0;
		repe(t, g[s]) {
			chmax(dp[s], dfs(t) + 1);
		}
		return dp[s];
	};

	// 各頂点 s についての情報を計算する．
	int res = 0;
	rep(s, n) {
		chmax(res, dfs(s));
	}

	return res;
}


//【コスト最大パス（頂点コスト）】O(|V| + |E|)
/*
* 頂点コスト w の与えられた有向非巡回グラフ g のパス（長さ 0 も可）で，
* パスに属する頂点のコストの和の最大値を返す．
*
*（DAG 上の DP）
*/
ll highest_cost_path(const Graph& g, const vl& w) {
	int n = sz(g);

	// dp[s] : 頂点 s からの最大コスト
	vl dp(n);
	vb seen(n);

	function<ll(int)> dfs = [&](int s) {
		// s の情報を計算済だったらすぐに返す．
		if (seen[s]) return dp[s];
		seen[s] = true;

		// s から行ける頂点 t の情報を元に s の情報を計算する．
		dp[s] = 0;
		repe(t, g[s]) {
			chmax(dp[s], dfs(t));
		}
		dp[s] += w[s];

		return dp[s];
	};

	// 各頂点 s についての情報を計算する．
	ll res = 0;
	rep(s, n) {
		chmax(res, dfs(s));
	}

	return res;
}


//【コスト最大パス（頂点コスト）】O(|V| + |E|)
/*
* 頂点コスト w の与えられた有向非巡回グラフ g の r からのパス（長さ 0 も可）で，
* パスに属する頂点のコストの和を最大とするパスの頂点列を path に格納する．
* またそのパスのコストを返す．
*
*（DAG 上の DP）
*/
ll highest_cost_path(const Graph& g, const vl& w, int r, vi* path = nullptr) {
	int n = sz(g);

	// dp[s] : 頂点 s からの最大コスト
	vl dp(n);
	vb seen(n);
	vi next(n, -1);

	function<ll(int)> dfs = [&](int s) {
		// s の情報を計算済だったらすぐに返す．
		if (seen[s]) return dp[s];
		seen[s] = true;

		// s から行ける頂点 t の情報を元に s の情報を計算する．
		dp[s] = 0;
		repe(t, g[s]) {
			if (chmax(dp[s], dfs(t))) {
				next[s] = t;
			}
		}
		dp[s] += w[s];

		return dp[s];
	};

	// r から探索
	ll res = dfs(r);

	// DP 復元
	if (path != nullptr) {
		path->clear();

		for (int s = r; s != -1; s = next[s]) {
			path->push_back(s);
		}
	}

	return res;
}


//【コスト最大パスの組（頂点コスト）】O(|V|^3)
/*
* 頂点コスト w の与えられた有向非巡回グラフ g（トポロジカルソート済）のパスの組で，
* いずれかのパスに属している頂点のコストの和の最大値を返す．
*
*（DAG 上の二次元 DP）
*
* 利用：【幅優先探索】
*/
ll highest_cost_twinpath(const Graph& g_, const vl& w_) {
	// 参考 : https://suikaba.hatenablog.com/entry/2017/08/26/172626

	int n = sz(g_);

	// 全頂点への有向辺をもちコストが 0 の頂点 0 を追加する．
	Graph g(n + 1LL);
	repi(t, 1, n) {
		g[0].push_back(t);
	}
	rep(s, n) {
		repe(t, g_[s]) {
			g[s + 1LL].push_back(t + 1);
		}
	}

	vl w(n + 1LL);
	w[0] = 0;
	rep(s, n) {
		w[s + 1LL] = w_[s];
	}

	n++;

	// downQ[s][t] : パス s → t が存在するか（いくつかの頂点を飛び越えて移動できるか）
	vvb downQ(n, vb(n));
	rep(s, n) {
		vi dist;
		breadth_first_search(g, s, dist);

		rep(t, n) {
			downQ[s][t] = (dist[t] >= 0);
		}
	}

	// dp[s1][s2] : 頂点 s1 < s2 からのパスの組の最大コスト
	vvl dp(n, vl(n));
	vvb seen(n, vb(n));

	function<ll(int, int)> dfs = [&](int s1, int s2) {
		// (s1, s2) の情報を計算済だったらすぐに返す．
		if (seen[s1][s2]) return dp[s1][s2];
		seen[s1][s2] = true;

		dp[s1][s2] = w[s1] + w[s2];

		// s2 から行ける頂点 t2 の情報を元に (s1, s2) の情報を計算する．
		repe(t2, g[s2]) {
			chmax(dp[s1][s2], dfs(s1, t2) + w[s2]);
		}

		// s1 から行ける頂点 t1 の情報を元に (s1, s2) の情報を計算する．
		// ただし s1 からは s2 を飛び越えるような移動しか認めないこととする．
		repi(t1, s2 + 1, n - 1) {
			if (downQ[s1][t1]) {
				chmax(dp[s1][s2], dfs(s2, t1) + w[s1]);
			}
		}

		return dp[s1][s2];
	};

	dfs(0, 0);

	ll res = 0;
	rep(s2, n) {
		rep(s1, s2) {
			chmax(res, dp[s1][s2]);
		}
	}
	return res;
}


//【最大クリーク問題】O(2^√(2|E|) |V|)
/*
* 無向グラフ g の最大クリークの大きさを返す．
* 
* 利用：【部分集合の全探索】
*/
int maximum_clique(const Graph& g) {
	// 参考：https://www.slideshare.net/wata_orz/ss-12131479

	int n = sz(g);

	// 隣接行列 adj，各頂点の次数 deg，総次数 deg_sum，
	// 最小次数 deg_min，次数最小頂点の番号 i_min を得る．
	vvb adj(n, vb(n));
	vi deg(n);
	int deg_sum = 0, deg_min = INF, i_min = -1;
	rep(s, n) {
		repe(t, g[s]) {
			adj[s][t] = true;
			deg[s]++;
		}
		deg_sum += deg[s];

		if (chmin(deg_min, deg[s])) {
			i_min = s;
		}
	}

	// 考慮すべき頂点のリスト
	vi v(n);
	rep(i, n) {
		v[i] = i;
	}

	// 素朴な方法で最大クリークを求める O(2^n n)
	function<int()> naive = [&]() {
		int res = 0;

		// 全ての部分集合 set について
		repb(set, n) {
			bool sum = 0;

			// n 頂点それぞれについて
			rep(i, n) {
				// set に選んでいないなら無関係
				if (!(set & (1 << i))) {
					continue;
				}

				// i 番目以降の頂点について
				repi(j, i + 1, n - 1) {
					// set に選んでいないなら無関係 
					if (!(set & (1 << j))) {
						continue;
					}

					// 辺 (v[i], v[j]) がなければクリークでない．
					if (!adj[v[i]][v[j]]) {
						goto LOOP_END;
					}
				}
			}

			// クリークが見つかったので大きさを更新する．
			chmax(res, popcount(set));

		LOOP_END:;
		}
		return res;
	};

	int res = 1;
	while (n > 0) {
		// 辺に対して頂点が十分少ないなら素朴な方法で構わない．
		if (deg_min * deg_min <= deg_sum) {
			return max(res, naive());
		}

		// 次数最小の頂点 v[i_min] の隣接点の番号の集合を得る．
		// 同時に v[i_min] に出入りする辺を削除したことにし，各頂点の次数 deg を更新する．
		int set = 0;
		rep(i, n) {
			if (adj[v[i_min]][v[i]]) {
				set += 1 << i;

				deg[v[i]]--;
			}
		}

		// まず v[i_min] を含む最大クリークの大きさ res を求める．
		// v[i_min] の隣接点の部分集合 sub すべてについて
		repbs(sub, set) {
			rep(i, n) {
				// sub に選んでいないなら無関係
				if (!(sub & (1 << i))) {
					continue;
				}

				repi(j, i + 1, n - 1) {
					// sub に選んでいないなら無関係
					if (!(sub & (1 << j))) {
						continue;
					}

					// sub がクリークでなければ何もしない．
					if (!adj[v[i]][v[j]]) {
						goto LOOP_END;
					}
				}
			}

			// sub がクリークなら v[i_min] と合わせてもクリークとなる．
			chmax(res, popcount(sub) + 1);

		LOOP_END:;
		}

		// v[i_min] を含む最大クリークの大きさは求まったので，
		// 以降は v[i_min] を含まないクリークだけを考えれば良い．
		// 頂点 v[i_min] と v[n-1] を交換して n を減らすことで v[i_min] を除去する．
		swap(v[i_min], v[n - 1]);
		n--;

		// 総次数 deg_sum，最小次数 deg_min，次数最小頂点の番号 i_min を得る．
		deg_sum = 0;
		deg_min = INF;
		rep(i, n) {
			if (chmin(deg_min, deg[v[i]])) {
				i_min = i;
			}
			deg_sum += deg[v[i]];
		}
	}

	return res;
}


//【最大流問題／フォード－ファルカーソンのアルゴリズム】O(|E| maxflow)
/*
* コスト付き有向グラフ g の始点 s から終点 t までの最大フローの大きさを返す．
*/
ll ford_fullkerson(const WGraph& g, int s, int t) {
	// 参考：https://algo-logic.info/ford-fullkerson/

	int n = sz(g);

	// 残余ネットワークを作り初期化する．
	// また更新のために逆向きの辺の番号を記録しておく．
	WGraph res(n);
	vvi rev(n);
	rep(i, n) {
		repe(e, g[i]) {
			// 順方向は g と同じ，逆方向は 0 で初期化．
			res[i].push_back(e);
			res[e.to].push_back({ i, 0 });

			// e = res[i][j] の逆向きの辺は res[e.to][rev[i][j]]
			rev[i].push_back(sz(res[e.to]) - 1);
			rev[e.to].push_back(sz(res[i]) - 1);
		}
	}

	vb seen(n);
	function<ll(int, ll)> dfs = [&](int v, ll f) {
		// 終点 t まで流せたら流量を返す．
		if (v == t) {
			return f;
		}

		// 頂点 v に訪れたことを記録する．
		seen[v] = true;

		// v から出ている残余ネットワークの各辺 e について
		rep(j, sz(res[v])) {
			auto& e = res[v][j];

			// e の先の頂点に既に訪れていたり，e に空きが無いなら何もしない．
			if (seen[e.to] || e.cost == 0) {
				continue;
			}

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

		// フローが流れなかったら最大まで流し切ったので結果を返す．
		if (f == 0) {
			return mf;
		}

		// フローが流れたら流量を更新する．
		mf += f;
	}

	// ここにはこない．
	return 0LL;
}


//【中国人郵便配達問題】O(2^|V| |V|)
/*
* コスト付き連結無向グラフの全辺を通る閉路の最小コストを返す．
*
* 利用：【最小コスト完全マッチング】
*/
ll chinese_postman_problem(const WGraph& g) {
	// 参考：https://perogram.hateblo.jp/entry/2020/09/30/101602

	int n = sz(g);

	// 全頂点対の最短経路長を求めておく．
	vvl cost;
	warshall_floyd(g, cost);

	// 次数が奇数の頂点を求める．
	vi vodd;
	rep(i, n) {
		if (sz(g[i]) % 2 == 1) {
			vodd.push_back(i);
		}
	}
	int nodd = sz(vodd);

	// 次数が奇数の頂点だけを集めたコスト付きグラフの隣接行列を作る．
	// 辺のコストは元のグラフ g での最短経路長とする．
	vvl adj(nodd, vl(nodd, INFL));
	rep(i, nodd) {
		rep(j, nodd) {
			adj[i][j] = cost[vodd[i]][vodd[j]];
		}
	}

	// g の全ての辺のコストの総和を求める．
	ll res = 0;
	rep(s, n) {
		repe(e, g[s]) {
			res += e.cost;
		}
	}
	res /= 2;

	// オイラー閉路を作るために必要な最小のコスト分を加算する．
	res += minimum_cost_matching(adj);

	return res;
}


