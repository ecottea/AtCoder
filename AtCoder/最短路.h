#pragma once
#include "header.h"
#include "構造(グラフ).h"
// ■■■■■ グラフ上の最適化問題 ■■■■■



//【最短パス】O(|V| + |E|)
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


//【コスト最小パス】O(|E| log|V|)
/*
* 非負のコスト付きグラフ g の始点 st から終点 gl までのコスト最小パスのコストを返す．
* gl = st ならコスト最小サイクルのコストを返す．到達不能なら INFL を返す．
* 必要なら path にコスト最小パス上の頂点の列を格納する．
*
*（ダイクストラ法）
*/
ll minimum_cost_path(const WGraph& g, int st, int gl, vi* path = nullptr) {
	int n = sz(g);

	vl cost(n, INFL); // st からの最小コストを保持するテーブル
	vi parent(n); // 1 つ手前の頂点を記録しておくテーブル（復元用）

	// 組 (スタートからのコスト, 頂点番号, 直前の頂点) を入れる優先度付きキューを用意する．
	// スタートからのコストがより小さいものを優先的に取り出す．
	priority_queue_rev<tuple<ll, int, int>> que;
	que.push({ 0, st, -1 });

	while (!que.empty()) {
		ll c; int s, p;
		tie(c, s, p) = que.top();
		que.pop();

		// もし既に最小コストが求まっているなら何もしない．
		if (c >= cost[s]) {
			continue;
		}

		// 最小コストの決定
		// 優先度付きキューでコストの小さい順に取り出しており，
		// かつコストが非負より三角不等式が成立するので最短の保証がある．
		cost[s] = (s == st && st == gl && p == -1) ? INFL : c;
		parent[s] = p;

		// ゴールに辿り着いたなら終了
		if (s == gl && !(st == gl && p == -1)) goto LOOP_END;

		// そこから移動できるノードについての情報をキューに追加する．
		for (auto e : g[s]) {
			que.push({ c + e.cost, e.to, s });
		}
	}
LOOP_END:;

	// st から gl まで到達不能の場合
	ll d = cost[gl];
	if (d == INFL) {
		return INFL;
	}

	// 必要なら経路復元を行う．
	if (path != nullptr) {
		path->clear();

		int t = gl;

		do {
			path->push_back(t);
			t = parent[t];
		} while (t != st);

		path->push_back(st);
		reverse(all(*path));
	}

	return d;
}


//【幅優先探索】O(|V| + |E|)
/*
* グラフ g に対し，始点を start として幅優先探索を行い，
* start から各頂点 i への最短経路長を dist[i] に格納する．
* i が start から到達不能な頂点の場合は dist[i] = -1 となる．
*/
void breadth_first_search(const Graph& g, int start, vi& dist) {
	int n = sz(g);
	dist = vi(n, -1); // スタートからの最短距離を保持するテーブル
	dist[start] = 0;
	queue<int> que; // 次に探索する頂点を入れておくキュー
	que.push(start);

	while (!que.empty()) {
		// 未探索の頂点を 1 つ得る．
		auto s = que.front();
		que.pop();

		for (auto t : g[s]) {
			if (dist[t] != -1) {
				// 発見済みの頂点なので何もしない．
				continue;
			}

			// スタートからの最短距離を確定する．
			// 幅優先探索なので，最短だという保証がある．
			dist[t] = dist[s] + 1;

			// 未探索の頂点として t を追加する．
			que.push(t);
		}
	}
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


//【単一始点最短路／ベルマン - フォード法】O(|E| |V|)
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


//【全頂点対最短路／ワーシャル - フロイド法】O(|V|^3)
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


//【最近傍探索】
/*
* 無向グラフ g とその頂点集合 v について，頂点 i と最も近い v の頂点の 1 つを nn[i] に，
* i と nn[i] との距離を dist[i] にそれぞれ格納する．（なければそれぞれ -1, INF）
*/
void nearest_neighbor(const Graph& g, const vi& v, vi& nn, vi& dist) {
	int n = sz(g);

	nn.resize(n, -1);
	dist.resize(n, INF);
	queue<int> q;

	repe(s, v) {
		q.push(s);
		nn[s] = s;
		dist[s] = 0;
	}

	while (!q.empty()) {
		int s = q.front();
		q.pop();

		repe(t, g[s]) {
			if (dist[t] != INF) continue;

			dist[t] = dist[s] + 1;
			nn[t] = nn[s];

			q.push(t);
		}
	}
}


