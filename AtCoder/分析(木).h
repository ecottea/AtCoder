#pragma once
#include "header.h"
#include "構造(グラフ).h"
#include "最短路.h"
// ■■■■■ 木の性質の分析 ■■■■■


//【木の直径】O(n)
/*
* 木 g の直径の長さ d を返し，直径の端点間を結ぶパスを path[0..d] に格納する．
* 直径の端点は path[0], path[d] である．
* 直径の中点[辺上なら辺の両端点] は path[d/2], path[(d+1)/2] である．
*/
int tree_diameter(const Graph& g, vi* path = nullptr) {
	// verify : https://atcoder.jp/contests/typical90/tasks/typical90_c
	// verify : https://atcoder.jp/contests/abc221/tasks/abc221_f

	int n = sz(g);

	// 頂点 0 から幅優先探索を行う．
	vi dist(n, INF); // 頂点 0 からの最短距離：O(n)
	dist[0] = 0;
	queue<int> q; // 次に探索する頂点を入れておくキュー
	q.push(0);

	while (!q.empty()) {
		// 未探索の頂点を 1 つ得る．
		auto s = q.front(); q.pop();

		repe(t, g[s]) {
			// 発見済みの頂点なら何もしない．
			if (dist[t] != INF) continue;

			// 頂点 0 からの最短距離を確定する．
			dist[t] = dist[s] + 1;

			// 未探索の頂点として t を追加する．
			q.push(t);
		}
	}

	// 頂点 0 からの距離が最も遠い点 v1 を見つける．
	int d = -INF, v1 = 0;
	rep(i, n) if (chmax(d, dist[i])) v1 = i;

	// 頂点 v1 から幅優先探索を行う．
	dist.assign(n, INF); // v1 からの最短距離：O(n)
	dist[v1] = 0;
	vi parent(n); // 直前に通ってきた頂点（経路復元用）
	parent[v1] = -1;
	q.push(v1); // 次に探索する頂点を入れておくキュー

	while (!q.empty()) {
		auto s = q.front(); q.pop();

		repe(t, g[s]) {
			// 発見済みの頂点なら何もしない．
			if (dist[t] != INF) continue;

			// v からの最短距離を確定する．
			dist[t] = dist[s] + 1;
			parent[t] = s;

			q.push(t);
		}
	}

	// 頂点 v1 からの距離が最も遠い点 v2 を見つける．
	d = -INF; int v2 = v1;
	rep(i, n) if (chmax(d, dist[i])) v2 = i;

	// v1 から v2 への経路を復元する．
	if (path != nullptr) {
		path->resize(d + 1);

		int v = v2;
		(*path)[d] = v;
		repir(i, d - 1, 0) (*path)[i] = parent[(*path)[i + 1]];
	}

	return d;
}


//【コスト付き木の直径】O(n log n)
/*
* コスト付き木の直径の長さを返す．また直径の両端となる頂点の組を p に格納する．
*
* 利用：【単一始点最短路／ダイクストラ法】
*/
ll tree_diameter(const WGraph& g, pii& p) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_5_A

	int n = sz(g);

	// 適当な頂点を始点にして最遠の点 s を求める．
	vl dist;
	dijkstra(g, 0, dist);

	ll max_dist = -1;
	int s;
	rep(i, n) {
		if (chmax(max_dist, dist[i])) {
			s = i;
		}
	}

	// s を始点にして最遠の点 t を求めれば，s と t の距離が木の直径である．
	max_dist = -1;
	int t;
	dijkstra(g, s, dist);
	rep(i, n) {
		if (chmax(max_dist, dist[i])) {
			t = i;
		}
	}

	p = { s, t };
	return max_dist;
}


//【木上のシュタイナー木】O(n)
/*
* 無向木 g の頂点集合 v を含む最小の木を st に構築し，その大きさを返す．
* また st の頂点 i が g のどの頂点と対応するかを id[i] に格納する．
*/
int steiner_tree(const Graph& g, const vi& v, Graph& st, vi& id) {
	// verify : https://atcoder.jp/contests/arc030/tasks/arc030_2

	if (v.empty()) {
		st.clear();
		id.clear();
		return 0;
	}

	int n = sz(g);

	vb sel(n); // v に属するか
	repe(s, v) sel[s] = true;

	vi deg(n); // 次数
	queue<int> q_deg1; // 次数 1 の頂点を入れておくキュー

	rep(s, n) {
		deg[s] = sz(g[s]);
		if (!sel[s] && deg[s] == 1) q_deg1.push(s);
	}

	// 次数 1 で v に属さない頂点を次々に除去していく．
	int m = n;
	while (!q_deg1.empty()) {
		int s = q_deg1.front();
		q_deg1.pop();

		deg[s] = -1; // st から除外の意味とする
		m--;

		repe(t, g[s]) {
			if (deg[t] > 0) deg[t]--;
			if (!sel[t] && deg[t] == 1) q_deg1.push(t);
		}
	}

	st.resize(m);
	id.resize(m);
	vi id_inv(n);
	queue<int> q_st;

	q_st.push(v[0]);
	id[0] = v[0];
	id_inv[v[0]] = 0;
	int i = 1;

	// 幅優先探索で st の頂点をなぞりつつ新たな木を構築する．
	while (!q_st.empty()) {
		int s = q_st.front();
		q_st.pop();

		repe(t, g[s]) {
			if (deg[t] >= 0) {
				id[i] = t;
				id_inv[t] = i;

				st[id_inv[s]].push_back(i);
				st[i].push_back(id_inv[s]);

				q_st.push(t);
				i++;
			}
		}

		deg[s] = -2; // 探索終了の意味とする
	}

	return m;
}


//【コスト付き木上のシュタイナー木】O(n)
/*
* コスト付き無向木 g の頂点集合 v を含む最小の木を st に構築し，その合計コストを返す．
* また st の頂点 i が g のどの頂点と対応するかを id[i] に格納する．
*/
ll steiner_tree(const WGraph& g, const vi& v, WGraph& st, vi& id) {
	int n = sz(g);

	vb sel(n); // v に属するか
	repe(s, v) sel[s] = true;

	vi deg(n); // 次数
	queue<int> q_deg1; // 次数 1 の頂点を入れておくキュー

	rep(s, n) {
		deg[s] = sz(g[s]);
		if (!sel[s] && deg[s] == 1) q_deg1.push(s);
	}

	// 次数 1 で v に属さない頂点を次々に除去していく．
	int m = n;
	while (!q_deg1.empty()) {
		int s = q_deg1.front();
		q_deg1.pop();

		deg[s] = -1; // st から除外の意味とする
		m--;

		repe(t, g[s]) {
			if (deg[t] > 0) deg[t]--;
			if (!sel[t] && deg[t] == 1) q_deg1.push(t);
		}
	}

	st.resize(m);
	id.resize(m);
	vi id_inv(n);
	queue<int> q_st;

	q_st.push(v[0]);
	id[0] = v[0];
	id_inv[v[0]] = 0;
	int i = 1;

	ll cost = 0;

	// 幅優先探索で st の頂点をなぞりつつ新たな木を構築する．
	while (!q_st.empty()) {
		int s = q_st.front();
		q_st.pop();

		repe(t, g[s]) {
			if (deg[t] >= 0) {
				id[i] = t;
				id_inv[t] = i;

				st[id_inv[s]].push_back({ i, t.cost });
				st[i].push_back({ id_inv[s], t.cost });
				cost += t.cost;

				q_st.push(t);
				i++;
			}
		}

		deg[s] = -2; // 探索終了の意味とする
	}

	return cost;
}


//【葉の削除回数】O(n)
/*
* 木 g に対し葉の削除を繰り返したとき何回目に頂点 i が削除されるかを lv[i] に格納する．
*
*（葉からの幅優先探索）
*/
void leaf_remove_level(const Graph& g, vi& lv) {
	int n = sz(g);
	lv = vi(n);

	// 木が 1 頂点のみで次数 1 の頂点が存在しない場合の例外処理
	if (n == 1) {
		lv[0] = 0;
		return;
	}

	// 次数を求めておく．
	vi degree(n);
	rep(i, n) {
		repe(t, g[i]) {
			degree[t]++;
		}
	}

	// 次数が 1 の頂点から順に取り除いていく．
	queue<pii> q;
	rep(i, n) {
		if (degree[i] == 1) {
			q.push({ i, 0 });
		}
	}

	while (!q.empty()) {
		int s, d;
		tie(s, d) = q.front();
		q.pop();

		lv[s] = d;

		repe(t, g[s]) {
			// 頂点 s を取り除き，t の次数を更新する．
			degree[t]--;

			// 新たに次数 1 の頂点が生まれたらキューに追加する．
			if (degree[t] == 1) {
				q.push({ t, d + 1 });
			}
		}
	}
}


