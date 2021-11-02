#pragma once
#include "header.h"
#include "構造(グラフ).h"
#include "グラフ最適化.h"
#include "最短路.h"
// ■■■■■ 木 ■■■■■



//【木の直径】O(|V| log|V|)
/*
* コスト付き木の直径の長さを返す．また直径の両端となる頂点の組を p に格納する．
*
* 利用：【単一始点最短路／ダイクストラ法】
*/
ll tree_diameter(const WGraph& g, pii& p) {
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

	// s を始点にして最遠の点 t を求めれば，|s t| が木の直径である．
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


//【直径とその中点】O(|V|)
/*
* 木の直径の両端点を ep = {s, t} に，経路 s → t の中点を ctr に格納し，直径の長さを返す．
* 中点が頂点 v のときは ctr = {v, v}，辺 es → et 上のときは ctr = {es, et} とする．
*
* 利用：【幅優先探索】
*/
int tree_diameter_and_midpoint(const Graph& g, pii& ep, pii& ctr) {
	int n = sz(g);

	// 頂点 0 から幅優先探索を行う．
	vi dist;
	breadth_first_search(g, 0, dist);

	// 頂点 0 からの距離が最も遠い点 ep0 を見つける．
	int d = 0, ep0;
	rep(i, n) {
		if (chmax(d, dist[i])) ep0 = i;
	}

	dist = vi(n, -1); // スタートからの最短距離を保持するテーブル
	dist[ep0] = 0;
	vi parent(n); // 直前に通ってきた頂点（経路復元用）
	parent[ep0] = -1;
	queue<int> que; // 次に探索する頂点を入れておくキュー
	que.push(ep0);

	// 頂点 ep0 から幅優先探索を行う．
	while (!que.empty()) {
		auto s = que.front();
		que.pop();

		for (auto t : g[s]) {
			if (dist[t] != -1) continue;

			dist[t] = dist[s] + 1;
			parent[t] = s;

			que.push(t);
		}
	}

	// 頂点 ep0 からの距離が最も遠い点 ep1 を見つける．
	d = 0; int ep1;
	rep(i, n) {
		if (chmax(d, dist[i])) ep1 = i;
	}

	// 直径の中点を得る．
	int c0, c1 = ep1;
	rep(i, d / 2) c1 = parent[c1];
	c0 = (d % 2 == 0 ? c1 : parent[c1]);

	// 結果の格納
	ep = { ep0, ep1 };
	ctr = { c0, c1 };

	return d;
}


//【木上のシュタイナー木】O(|V|)
/*
* 無向木 g の頂点集合 v を含む最小の木を st に構築し，その大きさを返す．
* また st の頂点 i が g のどの頂点と対応するかを id[i] に格納する．
*/
int steiner_tree(const Graph& g, const vi& v, Graph& st, vi& id) {
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


