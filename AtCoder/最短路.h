#pragma once
#include "header.h"
#include "構造(グラフ).h"
// ■■■■■ グラフ上の最適化問題 ■■■■■


//【幅優先探索】O(|V| + |E|)
/*
* グラフ g に対し，始点 st から各頂点 i への最短経路長を dist[i] に格納する．
* i が st から到達不能な頂点の場合は dist[i] = INF となる．
*/
void breadth_first_search(const Graph& g, int st, vi& dist) {
	// verify : https://algo-method.com/tasks/414

	int n = sz(g);
	dist = vi(n, INF); // スタートからの最短距離を保持するテーブル
	dist[st] = 0;
	queue<int> que; // 次に探索する頂点を入れておくキュー
	que.push(st);

	while (!que.empty()) {
		// 未探索の頂点を 1 つ得る．
		auto s = que.front();
		que.pop();

		repe(t, g[s]) {
			// 発見済みの頂点なら何もしない．
			if (dist[t] != INF) continue;

			// スタートからの最短距離を確定する．
			// 幅優先探索なので，最短だという保証がある．
			dist[t] = dist[s] + 1;

			// 未探索の頂点として t を追加する．
			que.push(t);
		}
	}
}


//【01-BFS】O(|V| + |E|)
/*
* 辺のコストが 0, 1 の二値に限られるコスト付きグラフ g に対し，
* st から各頂点 i への最短経路長を dist[i] に格納する．
* i が st から到達不能な頂点の場合は dist[i] = INF となる．
*/
void binary_bfs(const WGraph& g, int st, vi& dist) {
	// verify : https://atcoder.jp/contests/arc005/tasks/arc005_3

	int n = sz(g);

	dist = vi(n, INF); // スタートからの最短距離を保持するテーブル
	dist[st] = 0;

	deque<int> q; // 次に探索する頂点を入れておくデック
	q.push_back(st);

	while (!q.empty()) {
		// 未探索の頂点 s を 1 つ得る．
		auto s = q.front(); q.pop_front();

		repe(e, g[s]) {
			// 辺 e を経由して t まで行くときの最短距離
			int d = dist[s] + (int)e.cost;

			// 暫定の最短距離が更新できるか
			if (d < dist[e.to]) {
				dist[e.to] = d;

				// 未探索の頂点として t を追加する．
				// ただし t までの辺のコストが 0 ならば最優先で探索する．
				if (e.cost == 0) q.push_front(e.to);
				else q.push_back(e.to);
			}
		}
	}
}


//【単一始点最短路／ダイクストラ法】O(|V| + |E| log|V|)
/*
* 非負のコスト付きグラフ g に対し，始点 st から各頂点 i への最小コストを cost[i] に格納する．
*/
void dijkstra(const WGraph& g, int st, vl& cost) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_1_A

	int n = sz(g);
	cost = vl(n, INFL); // スタートからの最小コストを保持するテーブル

	// 組 (スタートからのコスト, 頂点番号) を入れる優先度付きキューを用意する．
	// スタートからのコストがより小さいものを優先的に取り出す．
	priority_queue_rev<pli> que;
	que.push({ 0, st });

	while (!que.empty()) {
		ll c; int s;
		tie(c, s) = que.top(); que.pop();

		// もし既に最小コストが求まっているなら何もしない．
		if (c >= cost[s]) continue;

		// 最小コストの決定
		// 優先度付きキューでコストの小さい順に取り出しており，
		// かつコストが非負より三角不等式が成立するので最短の保証がある．
		cost[s] = c;

		// そこから移動できるノードについての情報をキューに追加する．
		repe(e, g[s]) que.push({ c + e.cost, e.to });
	}
}


//【単一始点最短路／ダイクストラ法（頂点コスト）】O(|V| + |E| log|V|)
/*
* 頂点に非負のコスト vc が与えられたグラフ g に対し，
* 始点 st から各頂点 i への最小コストを cost[i] に格納する．
*/
void dijkstra(const Graph& g, const vl& vc, int st, vl& cost) {
	int n = sz(g);
	cost = vl(n, INFL); // スタートからの最小コストを保持するテーブル

	// 組 (スタートからのコスト, 頂点番号) を入れる優先度付きキューを用意する．
	// スタートからのコストがより小さいものを優先的に取り出す．
	priority_queue_rev<pli> que;
	que.push({ vc[st], st });

	while (!que.empty()) {
		ll c; int s;
		tie(c, s) = que.top(); que.pop();

		// もし既に最小コストが求まっているなら無視
		if (c >= cost[s]) continue;

		// 最小コストの決定
		// 優先度付きキューでコストの小さい順に取り出しているので最小の保証がある．
		cost[s] = c;

		// そこから移動できるノードについての情報をキューに追加する．
		repe(t, g[s]) que.push({ c + vc[t], t });
	}
}


//【ボテンシャル付きダイクストラ法】O(|V| + |E| log|V|)
/*
* 負閉路のないコスト付きグラフ g に対し，実行可能ポテンシャル u を与え，
* 始点 st から各頂点 i への最小コストを cost[i] に格納する．
*
* 条件：g[s][t].cost >= u[t] - u[s]
*/
void dijkstra_potential(const WGraph& g, const vl& u, int st, vl& cost) {
	// verify : https://atcoder.jp/contests/abc237/tasks/abc237_e

	//【方法】
	// g の各頂点 s にポテンシャル u[s] を導入し，辺 s→t のコスト c[s][t] が
	//		c[s][t] = (u[t] - u[s]) + r[s][t]  (r[s][t] >= 0)
	// と表されるとする．
	//（場所依存のコスト Δu と経路依存のコスト r に分けるイメージ）
	// 
	// 任意の経路 s→...→t について，u からの寄与は途中によらず u[t] - u[s] で一定である．
	//（ベクトル解析の rot grad = 0 を思い出す．勾配場中の移動コストは経路に依存しない．）
	// よって残る r からの寄与を最小化すればよいが，r は非負なので通常のダイクストラ法でよい．
	//
	// なお，負のコストの辺がある場合に通常のダイクストラ法を使うと，
	//		負の閉路に行ける → 無限ループ
	//		負の閉路に行けない → 正しい答えは出るが，最悪計算量 O(2^|V|)
	// となるのでだめ．参考：https://theory-and-me.hatenablog.com/entry/2019/09/08/182442

	int n = sz(g);
	cost = vl(n, INFL);

	priority_queue_rev<pli> que;
	que.push({ 0, st });

	while (!que.empty()) {
		ll c; int s;
		tie(c, s) = que.top(); que.pop();

		if (c >= cost[s]) continue;

		cost[s] = c;

		repe(e, g[s]) {
			ll r = e.cost - (u[e.to] - u[s]);
			que.push({ c + r, e.to });
		}
	}

	rep(i, n) {
		cost[i] += u[i] - u[st];
	}
}


//【単一始点最短路／ベルマン－フォード法】O(|E| |V|)
/*
* コスト付きグラフ g（負のコストも可）に対し，始点を st として
* ベルマン・フォード法を用いて最小コスト経路問題を解き，
* st から各頂点 i への最小コストを cost[i] に格納する．
* もし st から到達可能な負のコストをもつ閉路があれば false を返す．
*/
bool bellman_ford(const WGraph& g, int st, vl& cost) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_1_B

	int n = sz(g);
	cost = vl(n, INFL); // スタートからの最小コストを保持するテーブル
	cost[st] = 0;

	rep(i, n) {
		bool updated = false;

		// 全ての辺についての操作
		rep(s, n) {
			repe(e, g[s]) {
				// もし (始点へのコスト) + (辺のコスト) < (終点へのコスト) なら
				// (終点へのコスト) を更新する．
				// INFL からは何を引いても INFL になるようにしているので，
				// st から到達可能な負閉路しか検出しない．
				if (cost[s] != INFL && cost[s] + e.cost < cost[e.to]) {
					cost[e.to] = cost[s] + e.cost;
					updated = true;
				}
			}
		}

		// もしコストの更新が起こらなければ最小コスト確定
		if (!updated) return true;
	}

	// もし全ての辺についての操作を |V| 回繰り返してもコストの更新があったなら，
	// st から到達可能な負の閉路を持っているので false を返す．
	return false;
}


//【全頂点対最短路／ワーシャル－フロイド法】O(|V|^3)
/*
* コスト付きグラフ g（負のコストも可）に対し，ワーシャルフロイド法を用いて
* 全頂点対 (i, j) に関する最小コスト経路問題を解き，結果を cost[i][j] に格納する．
* もし負の閉路をもっていれば false を返す．
*/
bool warshall_floyd(const WGraph& g, vvl& cost) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_1_C

	int n = sz(g);
	cost = vvl(n, vl(n, INFL)); // 頂点対の最小コストを保持するテーブル
	rep(s, n) cost[s][s] = 0;
	rep(s, n) {
		repe(e, g[s]) {
			// 多重辺に対応するため chmin を用いている．
			chmin(cost[s][e.to], e.cost);
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
				if (cost[i][j] > INFL / 2) cost[i][j] = INFL;
			}
		}
	}

	// 負の閉路を持っていれば false を返す
	rep(i, n) {
		if (cost[i][i] < 0) return false;
	}
	return true;
}


//【最短パス】O(|V| + |E|)
/*
* グラフ g の始点 st から終点 gl までの最短パスの長さを返す（到達不能なら INF）
* 必要なら path に最短パス上の頂点の列を格納する．
*
*（幅優先探索）
*/
int shortest_path(const Graph& g, int st, int gl, vi* path = nullptr) {
	// verify : https://atcoder.jp/contests/abc233/tasks/abc233_f

	int n = sz(g);

	vi dist(n, INF); // st からの最短距離を保持するテーブル
	dist[st] = 0;

	vi p(n); // 1 つ手前の頂点を記録しておくテーブル（復元用）
	p[st] = -1;

	queue<int> que; // 次に探索する頂点を入れておくキュー
	que.push(st);

	while (!que.empty()) {
		auto s = que.front(); que.pop();

		if (s == gl) break;

		repe(t, g[s]) {
			// 発見済みの頂点なら何もしない．
			if (dist[t] != INF) continue;

			// スタートからの最短距離を確定する．
			dist[t] = dist[s] + 1;
			p[t] = s;

			// 未探索の頂点として t を追加する．
			que.push(t);
		}
	}

	// st から gl まで到達不能の場合
	int d = dist[gl];
	if (d == INF) return INF;

	// 必要なら経路復元を行う．
	if (path != nullptr) {
		*path = vi(d + 1);

		int t = gl, i = d;

		while (t != st) {
			(*path)[i--] = t;
			t = p[t];
		}

		(*path)[0] = st;
	}

	return d;
}


//【最短サイクル】O(|V| + |E|)
/*
* グラフ g の頂点 st を通る最短サイクルの長さを返す．（存在しないなら INF）
* 必要なら path に最短サイクル上の頂点の列を格納する．
*
*（幅優先探索）
*/
int shortest_cycle(const Graph& g, int st, vi* path = nullptr) {
	// verify : https://atcoder.jp/contests/abc142/tasks/abc142_f

	int n = sz(g);

	vi dist(n, INF); // st からの最短距離を保持するテーブル

	vi p(n); // 1 つ手前の頂点を記録しておくテーブル（復元用）
	p[st] = -1;

	queue<int> que; // 次に探索する頂点を入れておくキュー
	repe(t, g[st]) {
		que.push(t);
		dist[t] = 1;
		p[t] = st;
	}

	while (!que.empty()) {
		// 未探索の頂点を 1 つ得る．
		auto s = que.front(); que.pop();

		if (s == st) break;

		repe(t, g[s]) {
			// 発見済みの頂点なら何もしない．
			if (dist[t] != INF) continue;

			// スタートからの最短距離を確定する．
			dist[t] = dist[s] + 1;
			p[t] = s;

			// 未探索の頂点として t を追加する．
			que.push(t);
		}
	}

	// st から st まで到達不能の場合
	int d = dist[st];
	if (d == INF) return INF;

	// 必要なら経路復元を行う．
	if (path != nullptr) {
		*path = vi(d + 1);

		int t = st, i = d;

		do {
			(*path)[i--] = t;
			t = p[t];
		} while (t != st);

		(*path)[0] = st;
	}

	return d;
}


//【コスト最小パス】O(|V| + |E| log|V|)
/*
* 非負のコスト付きグラフ g の始点 st から終点 gl までのコスト最小パスのコストを返す．
* 到達不能なら INFL を返す．必要なら path にコスト最小パス上の頂点の列を格納する．
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
		tie(c, s, p) = que.top(); que.pop();

		// もし既に最小コストが求まっているなら何もしない．
		if (c >= cost[s]) continue;

		// 最小コストの決定
		cost[s] = c;
		parent[s] = p;

		// ゴールに辿り着いたなら終了
		if (s == gl) break;

		// そこから移動できるノードについての情報をキューに追加する．
		repe(e, g[s]) {
			que.push({ c + e.cost, e.to, s });
		}
	}

	// st から gl まで到達不能の場合
	ll d = cost[gl];
	if (d == INFL) return INFL;

	// 必要なら経路復元を行う．
	if (path != nullptr) {
		path->clear();

		int t = gl;
		while (t != st) {
			path->push_back(t);
			t = parent[t];
		}

		path->push_back(st);
		reverse(all(*path));
	}

	return d;
}


//【コスト最小サイクル】O(|V| + |E| log|V|)
/*
* 非負のコスト付きグラフ g の頂点 st を通るコスト最小サイクルのコストを返す．
* 存在しないなら INFL を返す．必要なら path にコスト最小サイクル上の頂点の列を格納する．
*
*（ダイクストラ法）
*/
ll minimum_cost_cycle(const WGraph& g, int st, vi* path = nullptr) {
	int n = sz(g);

	vl cost(n, INFL); // st からの最小コストを保持するテーブル
	vi parent(n); // 1 つ手前の頂点を記録しておくテーブル（復元用）

	// 組 (スタートからのコスト, 頂点番号, 直前の頂点) を入れる優先度付きキューを用意する．
	// スタートからのコストがより小さいものを優先的に取り出す．
	priority_queue_rev<tuple<ll, int, int>> que;
	repe(e, g[st]) {
		que.push({ e.cost, e.to, st });
	}

	while (!que.empty()) {
		ll c; int s, p;
		tie(c, s, p) = que.top(); que.pop();

		// もし既に最小コストが求まっているなら何もしない．
		if (c >= cost[s]) continue;

		// 最小コストの決定
		// 優先度付きキューでコストの小さい順に取り出しており，
		// かつコストが非負より三角不等式が成立するので最短の保証がある．
		cost[s] = c;
		parent[s] = p;

		// st に戻ってきたら終了
		if (s == st) break;

		// そこから移動できるノードについての情報をキューに追加する．
		repe(e, g[s]) {
			que.push({ c + e.cost, e.to, s });
		}
	}

	// st から st まで到達不能の場合
	ll d = cost[st];
	if (d == INFL) return INFL;

	// 必要なら経路復元を行う．
	if (path != nullptr) {
		path->clear();

		int t = st;
		do {
			path->push_back(t);
			t = parent[t];
		} while (t != st);

		path->push_back(st);
		reverse(all(*path));
	}

	return d;
}


//【最近傍探索】O(|V| + |E|)
/*
* 無向グラフ g とその頂点集合 v について，頂点 i と最も近い v の頂点の 1 つを nn[i] に，
* i と nn[i] との距離を dist[i] にそれぞれ格納する．（なければそれぞれ -1, INF）
* 
*（幅優先探索）
*/
void nearest_neighbor(const Graph& g, const vi& v, vi& nn, vi& dist) {
	int n = sz(g);

	nn = vi(n, -1);
	dist = vi(n, INF);
	queue<int> q;

	repe(s, v) {
		q.push(s);
		nn[s] = s;
		dist[s] = 0;
	}

	while (!q.empty()) {
		int s = q.front(); q.pop();

		repe(t, g[s]) {
			if (dist[t] != INF) continue;

			dist[t] = dist[s] + 1;
			nn[t] = nn[s];

			q.push(t);
		}
	}
}


//【最近傍探索】O(|V| + |E| log|V|)
/*
* コスト付き無向グラフ g とその頂点集合 v について，
* 頂点 i と最も近い v の頂点の 1 つを nn[i] に，
* i と nn[i] との距離を cost[i] にそれぞれ格納する．（なければそれぞれ -1, INFL）
*
*（ダイクストラ法）
*/
void nearest_neighbor(const WGraph& g, const vi& v, vi& nn, vl& cost) {
	// verify : https://atcoder.jp/contests/joi2012ho/tasks/joi2012ho5
	
	int n = sz(g);

	nn = vi(n, -1);
	cost = vl(n, INFL);
	priority_queue_rev<pli> q;

	repe(s, v) {
		q.push({ 0LL, s });
		nn[s] = s;
		cost[s] = 0LL;
	}

	while (!q.empty()) {
		int s; ll c;
		tie(c, s) = q.top(); q.pop();

		repe(e, g[s]) {
			if (c + e.cost >= cost[e.to]) continue;

			cost[e.to] = c + e.cost;
			nn[e.to] = nn[s];

			q.push({ cost[e.to], e.to });
		}
	}
}


