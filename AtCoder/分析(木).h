#pragma once
#include "header.h"
#include "構造(グラフ).h"
#include "最短路.h"
// ■■■■■ 木の性質の分析 ■■■■■


//【木の直径】O(n)
/*
* 木の直径の長さを返す．また直径の両端となる頂点の組を p に格納する．
*
* 利用：【幅優先探索】
*/
int tree_diameter(const Graph& g, pii& p) {
	// verify : https://atcoder.jp/contests/arc022/tasks/arc022_3

	int n = sz(g);

	// 適当な頂点を始点にして最遠の点 s を求める．
	vi dist;
	breadth_first_search(g, 0, dist);

	int max_dist = -1;
	int s;
	rep(i, n) {
		if (chmax(max_dist, dist[i])) {
			s = i;
		}
	}

	// s を始点にして最遠の点 t を求めれば，s と t の距離が木の直径である．
	max_dist = -1;
	int t;
	breadth_first_search(g, s, dist);
	rep(i, n) {
		if (chmax(max_dist, dist[i])) {
			t = i;
		}
	}

	p = { s, t };
	return max_dist;
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


//【直径とその中点】O(n)
/*
* 木の直径の両端点を ep = {s, t} に，経路 s → t の中点を ctr に格納し，直径の長さを返す．
* 中点が頂点 v のときは ctr = {v, v}，辺 es → et 上のときは ctr = {es, et} とする．
*
* 利用：【幅優先探索】
*/
int tree_diameter_and_midpoint(const Graph& g, pii& ep, pii& ctr) {
	// verify : https://atcoder.jp/contests/abc221/tasks/abc221_f

	int n = sz(g);

	// 頂点 0 から幅優先探索を行う．
	vi dist;
	breadth_first_search(g, 0, dist);

	// 頂点 0 からの距離が最も遠い点 ep0 を見つける．
	int d = 0, ep0;
	rep(i, n) {
		if (dist[i] < INF && chmax(d, dist[i])) ep0 = i;
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

		repe(t, g[s]) {
			if (dist[t] != -1) continue;

			dist[t] = dist[s] + 1;
			parent[t] = s;

			que.push(t);
		}
	}

	// 頂点 ep0 からの距離が最も遠い点 ep1 を見つける．
	d = 0; int ep1 = -1;
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


//【木上のシュタイナー木】O(n)
/*
* 無向木 g の頂点集合 v を含む最小の木を st に構築し，その大きさを返す．
* また st の頂点 i が g のどの頂点と対応するかを id[i] に格納する．
*/
int steiner_tree(const Graph& g, const vi& v, Graph& st, vi& id) {
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


