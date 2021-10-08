#pragma once
#include "header.h"
#include "構造(グラフ).h"
#include "グラフ最適化.h"
// ■■■■■ 木 ■■■■■



//【木のオイラーツアー】O(|V|)
/*
* 頂点 r を始点とする木 g のオイラーツアーを求める．
*
* in[v] : 最初に頂点 v を訪れた時刻（r なら 0）
* out[v] : 最後に頂点 v から離れた時刻（r なら 2 |V| - 1）
* pos[t] : 時刻 t に訪れた頂点の番号（長さ 2 |V| - 1）
*/
template <class G>
void euler_tour(G& g, int r, vi& in, vi& out, vi& pos) {
	// 参考 : https://qiita.com/recuraki/items/72e37eb9be9f71bc623a

	int n = sz(g);

	int time = 0;
	in = vi(n);
	out = vi(n);
	pos = vi(2 * n - 1);

	// 再帰用の関数
	function<void(int, int)> rf = [&](int s, int p) {
		// s を最初に訪れた
		in[s] = time;
		pos[time++] = s;

		for (auto t : g[s]) {
			// 親には戻らない．
			if (t == p) {
				continue;
			}

			rf(t, s);
			pos[time++] = s;
		}

		// s から最後に離れる
		out[s] = time;
	};

	// 根から順に探索する．
	rf(r, -1);
}


//【木の直径】O(|E| log|V|)
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


//【直径とその中点】O(n)
/*
* 木の直径の両端点を ep = {s, t} に，経路 s → t の中点を ctr に格納する．
* 中点が頂点 v のときは ctr = {v, v}，辺 es → et 上のときは ctr = {es, et} とする．
* また直径の長さを返す．
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


