#pragma once
#include "header.h"
#include "構造(グラフ).h"
// ■■■■■ グラフ上の最短路問題 ■■■■■


//【幅優先探索】O(|V| + |E|)
/*
* グラフ g に対し，st から各頂点への最短距離（到達不能なら INF）を格納したリストを返す．
*/
template <class G>
vi breadth_first_search(const G& g, int st) {
	// verify : https://algo-method.com/tasks/414

	int n = sz(g);

	vi dist(n, INF); // スタートからの最短距離を保持するテーブル : O(n)
	dist[st] = 0;

	queue<int> que; // 次に探索する頂点を入れておくキュー
	que.push(st);

	while (!que.empty()) {
		// 未探索の頂点を 1 つ得る．
		auto s = que.front(); que.pop();

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

	return dist;
}


//【幅優先探索（距離上限指定）】O((max deg(v))^D)
/*
* グラフ g に対し始点を st として距離 D 以下の範囲の幅優先探索を行い，
* st から到達可能な各頂点への最短距離を格納したリストを返す．
*/
unordered_map<int, int> BFS_ub(const Graph& g, int st, int D) {
	// verify : https://atcoder.jp/contests/abc254/tasks/abc254_e

	int n = sz(g);

	// dist[v] : st から v までの最短距離
	//（vi dist(n) とすると距離 D 以下でない頂点の分までテーブルを確保してしまい無駄がある）
	unordered_map<int, int> dist;
	dist[st] = 0;
	if (D == 0) return;

	queue<int> q; // 次に探索する頂点を入れておくキュー
	q.push(st);

	while (!q.empty()) {
		// 未探索の頂点 s を 1 つ得る．
		auto s = q.front(); q.pop();

		repe(t, g[s]) {
			// 探索済みの頂点なら何もしない．
			if (dist.count(t)) continue;

			// スタートからの最短距離を確定する．
			dist[t] = dist[s] + 1;

			// 未探索の頂点として t を追加する．
			if (dist[t] < D) q.push(t);
		}
	}

	return dist;
}


//【幅優先探索（動的）】O(|V| + |E|)（遅い）
/*
* st から到達可能な各頂点への最短距離を格納したリストを返す．
* nxt(s) は s の次に訪れることのできる頂点のリストを返す．
*/
template <class T>
unordered_map<T, int> dynamic_BFS(T st, const function<vector<T>(T)>& nxt) {
	// verify : https://atcoder.jp/contests/abc241/tasks/abc241_f

	unordered_map<T, int> dist; // st からの最短距離を保持するテーブル
	dist[st] = 0;

	queue<T> que; // 次に探索する頂点を入れておくキュー
	que.push(st);

	while (!que.empty()) {
		// 未探索の頂点 s を得る．
		auto s = que.front(); que.pop();

		repe(t, nxt(s)) {
			// t が発見済みの頂点なら何もしない．
			if (dist.count(t)) continue;

			// スタートからの最短距離を確定する．
			dist[t] = dist[s] + 1;

			// 未探索の頂点として t を追加する．
			que.push(t);
		}
	}

	return dist;

	/* nxt の定義の雛形
	using T = ll;
	function<vector<T>(T)> nxt = [&](T s) {
		vector<T> res;

		return res;
	};
	*/
}


//【01-BFS】O(|V| + |E|)
/*
* 辺のコストが 0, 1 の二値に限られる重み付きグラフ g に対し，
* st から各頂点への最短距離（到達不能なら INF）を格納したリストを返す．
*/
vi binary_BFS(const WGraph& g, int st) {
	// verify : https://atcoder.jp/contests/arc005/tasks/arc005_3

	int n = sz(g);

	vi dist(n, INF); // スタートからの最短距離を保持するテーブル
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

	return dist;
}


//【単一始点最短路】O(|V| + |E| log|V|)
/*
* 非負の重み付きグラフ g に対し
* st から各頂点への最短距離（到達不能なら INFL）を格納したリストを返す．
*/
vl dijkstra(const WGraph& g, int st) {
	// 参考 : https://snuke.hatenablog.com/entry/2021/02/22/102734
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_1_A

	int n = sz(g);
	vl dist(n, INFL); // スタートからの最短距離
	dist[st] = 0;

	// 組 (スタートからの距離, 頂点番号) を入れる優先度付きキュー
	priority_queue_rev<pli> q;
	q.push({ 0, st });

	while (!q.empty()) {
		ll c; int s;
		tie(c, s) = q.top(); q.pop();

		// すでにより短い距離に更新されていたなら何もしない（忘れると O(|V|^2)）
		if (dist[s] < c) continue;
		
		repe(e, g[s]) {
			// より短い距離で辿り着けるなら距離を更新し，その先も探索する．
			if (dist[s] + e.cost < dist[e.to]) {
				dist[e.to] = dist[s] + e.cost;
				q.push({ dist[e.to], e.to });
			}
		}
	}

	return dist;
}


//【単一始点最短路（動的）】O(|V| + |E| log|V|)（遅い）
/*
* st から到達可能な各頂点への最短距離を格納したリストを返す．
* nxt(s) は s の次に訪れることのできる {頂点, 移動コスト} の組のリストを返す．
*/
template <class T>
unordered_map<T, ll> dynamic_dijkstra(T st, const function<vector<pair<T, ll>>(T)>& nxt) {
	// verify : https://atcoder.jp/contests/abc297/tasks/abc297_e

	unordered_map<T, ll> dist; // st からの最短距離を保持するテーブル
	dist[st] = 0;

	// 組 (スタートからの距離, 頂点番号) を入れる優先度付きキュー
	priority_queue_rev<pair<ll, T>> q;
	q.emplace(0, st);

	while (!q.empty()) {
		auto [dist_s, s] = q.top(); q.pop();

		// すでにより短い距離に更新されていたなら何もしない（忘れると O(|V|^2)）		
		if (dist[s] < dist_s) continue;

		for (auto [t, cost_st] : nxt(s)) {
			// より短い距離で辿り着けるなら距離を更新し，その先も探索する．
			auto it_t = dist.find(t);
			if (it_t == dist.end()) {
				dist[t] = dist_s + cost_st;
				q.emplace(dist_s + cost_st, t);
			}
			else if (dist_s + cost_st < it_t->second) {
				it_t->second = dist_s + cost_st;
				q.emplace(dist_s + cost_st, t);
			}
		}
	}

	return dist;

	/* nxt の定義の雛形
	using T = ll;
	function<vector<pair<T, ll>>(T)> nxt = [&](T s) {
		vector<pair<T, ll>> res;

		return res;
	};
	*/
}


//【ポテンシャル付きダイクストラ法】O(|V| + |E| log|V|)
/*
* 負閉路のない重み付きグラフ g に対し，実行可能ポテンシャル u[0..n) を与え，
* st から各頂点への最短距離を格納したリストを返す．
*
* 条件：g[s][t].cost >= u[t] - u[s]
*/
vl dijkstra_potential(const WGraph& g, const vl& u, int st) {
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
	// となるのでどちらにせよだめ．
	// 参考：https://theory-and-me.hatenablog.com/entry/2019/09/08/182442

	int n = sz(g);
	vl dist(n, INFL); // スタートからの最短距離
	dist[st] = 0;

	// 組 (スタートからの距離, 頂点番号) を入れる優先度付きキュー
	priority_queue_rev<pli> q;
	q.push({ 0, st });

	while (!q.empty()) {
		ll c; int s;
		tie(c, s) = q.top(); q.pop();

		// すでにより短い距離に更新されていたなら何もしない．
		if (dist[s] < c) continue;
		
		repe(e, g[s]) {
			// r : 経路依存のコスト
			ll r = e.cost - (u[e.to] - u[s]);

			// より少ないコストで辿り着けるなら距離を更新し，その先も探索する．
			if (dist[s] + r < dist[e.to]) {
				dist[e.to] = dist[s] + r;
				q.push({ dist[e.to], e.to });
			}
		}
	}

	// 場所依存のコスト Δu を加算する．
	rep(i, n) dist[i] += u[i] - u[st];

	return dist;
}


//【ペナルティ付きダイクストラ法】
/*
* これまでの距離に依存してコストが追加でかかってくる状況でも
* それを加味すればダイクストラ法を使うことができる．
* 
* verify : https://atcoder.jp/contests/abc192/tasks/abc192_e
*/


//【最短路木】O(|V| + |E| log|V|)
/*
* 非負の重み付きグラフ g に対し，始点 st を根とする有向最短路木を返す．
*/
WGraph dijkstra_tree(const WGraph& g, int st) {
	// verify : https://atcoder.jp/contests/abc252/tasks/abc252_e

	int n = sz(g);
	WGraph gt(n);
	vl dist(n, INFL); // スタートからの最短距離
	dist[st] = 0;

	// 組 (スタートからの距離, 頂点番号) を入れる優先度付きキュー
	priority_queue_rev<pli> q;
	q.push({ 0, st });

	// 終点 → (直前の頂点, 直前の辺)
	vector<pair<int, WEdge>> t_to_se(n);

	while (!q.empty()) {
		ll c; int s;
		tie(c, s) = q.top(); q.pop();

		// すでにより短い距離に更新されていたなら何もしない．
		if (dist[s] < c) continue;

		repe(e, g[s]) {
			// より短い距離で辿り着けるなら距離を更新し，その先も探索する．
			if (dist[s] + e.cost < dist[e.to]) {
				dist[e.to] = dist[s] + e.cost;
				t_to_se[e.to] = { s, e };
				q.push({ dist[e.to], e.to });
			}
		}
	}

	// 全ての頂点 t について，直前に通った辺を集めて最短路木を構築する．
	rep(t, n) {
		if (t == st) continue;

		int s; WEdge e;
		tie(s, e) = t_to_se[t];

		gt[s].push_back(e);
	}

	return gt;
}


//【単一始点最短路（負コスト可）】O(|V| |E|)
/*
* 重み付きグラフ g（負のコストも可）に対し，
* st から各頂点への最短距離を格納したリストを返す．
* もし st から到達可能な負のコストをもつ閉路があれば空リストを返す．
*/
vl bellman_ford(const WGraph& g, int st) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_1_B

	//【補足】
	// min-plus 半環上のスパース行列とベクトルの積の反復とも思える．

	int n = sz(g);
	vl dist(n, INFL); // スタートからの最短距離を保持するテーブル
	dist[st] = 0;

	rep(i, n) {
		bool updated = false;

		// 全ての辺についての操作
		rep(s, n) {
			repe(e, g[s]) {
				// もし (始点への距離) + (辺のコスト) < (終点への距離) なら (終点への距離) を更新する．
				// INFL からは何を引いても INFL になるようにしているので，
				// st から到達可能な負閉路しか検出しない．
				if (dist[s] != INFL && dist[s] + e.cost < dist[e.to]) {
					dist[e.to] = dist[s] + e.cost;
					updated = true;
				}
			}
		}

		// もし距離の更新が起こらなければ最短距離確定
		if (!updated) return dist;
	}

	// もし全ての辺についての操作を |V| 回繰り返しても距離の更新があったなら，
	// st から到達可能な負の閉路を持っている．
	return vl();
}


//【全頂点対最短路（負コスト可）】O(|V|^3)
/*
* 重み付きグラフ g（負のコストも可）に対し，
* 頂点 i から頂点 j への最短距離を dist[i][j] に格納し dist を返す．
* もし g が負の閉路をもっていれば空リストを返す．
*/
vvl warshall_floyd(const WGraph& g) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_1_C

	//【補足】
	// min-plus 半環上の行列累乗とも思える．

	int n = sz(g);

	// dist[i][j] : 頂点 i から頂点 j までの最短距離
	vvl dist(n, vl(n, INFL));

	rep(s, n) dist[s][s] = 0;
	rep(s, n) repe(e, g[s]) chmin(dist[s][e.to], e.cost);

	// 途中で通っていいのが頂点 [0..k] の場合
	rep(k, n) {
		rep(i, n) rep(j, n) {
			// 通れない場合は加算や減算をしないようにして INFL を保つ．
			if (dist[i][k] == INFL || dist[k][j] == INFL) continue;

			// 新しく通れるようになった k を通る方が距離が小さければ更新
			//（一時配列に退避させず計算してしまっているので途中は間違った値
			// になっているが，より小さい値になるだけなので最後には合う．）
			chmin(dist[i][j], dist[i][k] + dist[k][j]);
		}
	}

	// 負の閉路を持っていれば空リストを返す．
	rep(i, n) if (dist[i][i] < 0) return vvl();
	return dist;
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


//【最短パス（動的）】O(|V| + |E|)（遅い）
/*
* st から gl までの最短距離を返し（到達不能なら INF），path に最短パス上の頂点の列を格納する．
* nxt(s) は s の次に訪れることのできる頂点のリストを返す．
*/
template <class T>
int dynamic_shortest_path(T st, T gl, const function<vector<T>(T)>& nxt, vector<T>* path = nullptr) {
	// verify : https://atcoder.jp/contests/abc241/tasks/abc241_f

	unordered_map<T, int> dist; // st からの最短距離を保持するテーブル
	dist[st] = 0;

	unordered_map<T, T> p; // 1 つ手前の頂点を記録しておくテーブル（復元用）

	queue<T> que; // 次に探索する頂点を入れておくキュー
	que.push(st);

	while (!que.empty()) {
		// 未探索の頂点 s を得る．
		auto s = que.front(); que.pop();

		repe(t, nxt(s)) {
			// t が発見済みの頂点なら何もしない．
			if (dist.count(t)) continue;

			// スタートからの最短距離を確定する．
			dist[t] = dist[s] + 1;
			p[t] = s;

			// ゴールに到着したら終了．
			if (t == gl) break;

			// 未探索の頂点として t を追加する．
			que.push(t);
		}
	}

	// st から gl まで到達不能の場合
	if (!dist.count(gl)) return INF;
	int d = dist[gl];

	// 必要なら経路復元を行う．
	if (path != nullptr) {
		*path = vector<T>(d + 1);

		T t = gl; int i = d;

		while (t != st) {
			(*path)[i--] = t;
			t = p[t];
		}

		(*path)[0] = st;
	}

	// ゴールに到達不可能
	return d;

	/* nxt の定義の雛形
	using T = ll;
	function<vector<T>(T)> nxt = [&](T s) {
		vector<T> res;

		return res;
	};
	*/
}


//【最短サイクル】O(|V| + |E|)
/*
* 有向グラフ g の頂点 st を通る最短サイクルの長さを返す（存在しないなら INF）
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


//【最短パス（重み付きグラフ）】O(|V| + |E| log|V|)
/*
* 非負の重み付きグラフ g の始点 st から終点 gl までの最短パスの長さを返す．
* 到達不能なら INFL を返す．必要なら path に最短パス上の頂点の列を格納する．
*
*（ダイクストラ法）
*/
ll minimum_cost_path(const WGraph& g, int st, int gl, vi* path = nullptr) {
	// verify : https://judge.yosupo.jp/problem/shortest_path

	int n = sz(g);

	vl dist(n, INFL); // st からの最短距離
	dist[st] = 0;

	vi parent(n); // 1 つ手前の頂点（復元用）

	// 組 (スタートからの距離, 頂点番号) を入れる優先度付きキュー
	priority_queue_rev<pli> q;
	q.emplace(0, st);

	while (!q.empty()) {
		ll c; int s;
		tie(c, s) = q.top(); q.pop();

		// ゴールに辿り着いたなら終了
		if (s == gl) break;

		// すでにより短い距離に更新されていたなら何もしない．
		if (dist[s] < c) continue;

		repe(e, g[s]) {
			// より短い距離で辿り着けるなら距離を更新し，その先も探索する．
			if (dist[s] + e.cost < dist[e.to]) {
				dist[e.to] = dist[s] + e.cost;
				parent[e.to] = s;
				q.emplace(dist[e.to], e.to);
			}
		}
	}

	// st から gl まで到達不能の場合
	ll d = dist[gl];
	if (d == INFL) return INFL;

	// 必要なら経路復元を行う．
	if (path != nullptr) {
		path->clear();

		int t = gl;
		while (t != st) {
			path->emplace_back(t);
			t = parent[t];
		}

		path->emplace_back(st);
		reverse(all(*path));
	}

	return d;
}


//【最短サイクル（重み付きグラフ）】O(|V| + |E| log|V|)
/*
* 非負の重み付き有向グラフ g の頂点 st を通る最短サイクルの長さを返す．
* 存在しないなら INFL を返す．必要なら path に最短サイクル上の頂点の列を格納する．
*
*（ダイクストラ法）
*/
ll minimum_cost_cycle(const WGraph& g, int st, vi* path = nullptr) {
	int n = sz(g);

	vl cost(n, INFL); // st からの最短距離を保持するテーブル
	vi parent(n); // 1 つ手前の頂点を記録しておくテーブル（復元用）

	// 組 (スタートからの距離, 頂点番号, 直前の頂点) を入れる優先度付きキューを用意する．
	// スタートからの距離がより小さいものを優先的に取り出す．
	priority_queue_rev<tuple<ll, int, int>> que;
	repe(e, g[st]) {
		que.push({ e.cost, e.to, st });
	}

	while (!que.empty()) {
		ll c; int s, p;
		tie(c, s, p) = que.top(); que.pop();

		// もし既に最短距離が求まっているなら何もしない．
		if (c >= cost[s]) continue;

		// 最短距離の決定
		// 優先度付きキューで距離の小さい順に取り出しており，
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


//【最短パス（参照付きグラフ）】O(|V| + |E|)
/*
* グラフ g の始点 st から終点 gl までの最短パスの長さを返す（到達不能なら INF）
* また path に最短パス上の辺番号の列を格納する．
*
*（幅優先探索）
*/
int shortest_path(const IGraph& g, int st, int gl, vi& path) {
	// verify : https://atcoder.jp/contests/abc218/tasks/abc218_f

	int n = sz(g);

	vi dist(n, INF); // st からの最短距離を保持するテーブル
	dist[st] = 0;

	vi p(n); // 1 つ手前の頂点を記録しておくテーブル（復元用）
	p[st] = -1;

	vi e(n); // 直前に通った辺番号を記録しておくテーブル（復元用）
	e[st] = -1;

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
			e[t] = t.id;

			// 未探索の頂点として t を追加する．
			que.push(t);
		}
	}

	// st から gl まで到達不能の場合
	int d = dist[gl];
	if (d == INF) return INF;

	// 経路復元を行う．
	path = vi(d);

	int t = gl, i = d - 1;

	while (t != st) {
		path[i--] = e[t];
		t = p[t];
	}

	return d;
}


//【最短パス（負コスト可）】O(|E| |V|)
/*
* 重み付きグラフ g（負のコストも可）の始点 st から終点 gl までの最短パスの長さを返す．
* 到達不能なら INFL，距離に下限がなければ -INFL を返す．
* 必要なら path に最短パス上の頂点の列を格納する．
*
*（ベルマン－フォード法）
*/
ll minimum_cost_path_nc(const WGraph& g, int st, int gl, vi* path = nullptr) {
	// verify : https://atcoder.jp/contests/abc137/tasks/abc137_e

	int n = sz(g);

	vl cost(n, INFL); cost[st] = 0; // st からの最短距離
	vi parent(n, -1); // そこまでの最短パスにおいて直前に通る頂点（復元用）
	vb updated(n); // 距離の更新があったか
	bool updated_any = false;

	// ベルマン－フォード法で各頂点までの最短距離を求める．
	rep(i, n) {
		updated = vb(n);
		updated_any = false;

		// 全ての辺についての操作
		rep(s, n) {
			repe(e, g[s]) {
				// st から未到達の頂点の先は一旦無視する．
				if (cost[s] == INFL) continue;

				// 距離の更新
				if (cost[s] + e.cost < cost[e.to]) {
					cost[e.to] = cost[s] + e.cost;
					parent[e.to] = s;
					updated_any = updated[e.to] = true;
				}
			}
		}

		// もし距離の更新が起こらなければ全体の最短距離が確定したことになる．
		if (!updated_any) break;
	}

	// gl に到達不能の場合
	if (cost[gl] == INFL) return INFL;

	// st から到達可能な負閉路が存在した場合
	if (updated_any) {
		// 距離の更新のあった全頂点を始点にして BFS を行う．
		queue<int> q;
		rep(s, n) {
			if (updated[s]) q.push(s);
		}

		vb seen(n);
		while (!q.empty()) {
			auto s = q.front(); q.pop();

			if (seen[s]) continue;
			seen[s] = true;

			// 距離の更新のあった頂点から gl に到達可能なら下限なしが確定する．
			if (s == gl) return -INFL;

			repe(t, g[s]) q.push(t);
		}
	}

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

	return cost[gl];
}


//【最近傍探索】O(|V| + |E|)
/*
* 無向グラフ g とその頂点集合 vs について，頂点 i と最も近い vs の頂点の 1 つを nn[i] に，
* i と nn[i] との距離を dist[i] にそれぞれ格納する（なければそれぞれ -1, INF）
* 
*（幅優先探索）
*/
void nearest_neighbor(const Graph& g, const vi& vs, vi& nn, vi& dist) {
	// verify : https://atcoder.jp/contests/agc024/tasks/agc024_d

	int n = sz(g);

	nn = vi(n, -1);
	dist = vi(n, INF);
	queue<int> q;

	repe(s, vs) {
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


//【最近傍探索（重み付きグラフ）】O(|V| + |E| log|V|)
/*
* 重み付き無向グラフ g とその頂点集合 vs について，
* 頂点 i と最も近い vs の頂点の 1 つを nn[i] に，
* i と nn[i] との距離を dist[i] にそれぞれ格納する（なければそれぞれ -1, INFL）
*
*（ダイクストラ法）
*/
void nearest_neighbor(const WGraph& g, const vi& vs, vi& nn, vl& dist) {
	// verify : https://atcoder.jp/contests/joi2012ho/tasks/joi2012ho5

	int n = sz(g);

	nn = vi(n, -1);
	dist = vl(n, INFL);
	priority_queue_rev<pli> q;

	repe(s, vs) {
		q.push({ 0, s });
		nn[s] = s;
		dist[s] = 0;
	}

	while (!q.empty()) {
		int s; ll c;
		tie(c, s) = q.top(); q.pop();

		if (dist[s] < c) continue;

		repe(e, g[s]) {
			if (c + e.cost >= dist[e.to]) continue;

			dist[e.to] = c + e.cost;
			nn[e.to] = nn[s];

			q.push({ dist[e.to], e.to });
		}
	}
}


//【k-近傍探索】O(|V| + k |E| log|V|)
/*
* 重み付き無向グラフ g とその頂点集合 vs について，
* 頂点 i と近い方から k 個の vs の頂点を knn[i] に，
* i と knn[i] との距離を dist[i] にそれぞれ格納する（なければそれぞれ -1, INFL）
*
*（ダイクストラ法）
*/
void k_nearest_neighbor(const WGraph& g, const vi& vs, int k, vvi& knn, vvl& dist) {
	// verify : https://atcoder.jp/contests/abc245/tasks/abc245_g

	int n = sz(g);

	knn = vvi(n, vi(k, -1));
	dist = vvl(n, vl(k, INFL));

	// iv_to_cost[i][v] : v から i までの最短距離
	vector<unordered_map<int, ll>> iv_to_cost(n);

	// q ∋ {c, i, vi} : vi から i への距離が c であることを記録する．
	priority_queue_rev<tuple<ll, int, int>> q;
	repe(v, vs) q.push({ 0LL, v, v });

	while (!q.empty()) {
		ll c; int s, v;
		tie(c, s, v) = q.top(); q.pop();

		// k 近傍を調べ終わっていたら何もしない．
		if (sz(iv_to_cost[s]) == k) continue;

		// 既に v からの最短距離が求まっているなら何もしない．
		if (iv_to_cost[s].count(v)) continue;

		// 最短距離の更新
		iv_to_cost[s][v] = c;

		// s の先を調べる
		repe(e, g[s]) q.push({ c + e.cost, e.to, v });
	}

	// 結果の格納
	rep(i, n) {
		int j = 0;
		repe(p, iv_to_cost[i]) {
			knn[i][j] = p.first;
			dist[i][j] = p.second;
			j++;
		}
	}
}


