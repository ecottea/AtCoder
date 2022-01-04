#pragma once
#include "header.h"
#include "構造(グラフ).h"
// ■■■■■ グラフの変形 ■■■■■


//【グリッド → グラフ】O(h w)
/*
* h 行 w 列のグリッドから nb 近傍を連結としたグラフ g を構築する．
* 壁マスは wall，空きマスはその他とする．
* i 行目の j 列目にあるマス (i, j) はグラフ頂点 i * w + j に対応する．
*/
template <class T>
void grid_to_graph(const vector<vector<T>>& c, Graph& g, T wall = '#', int nb = 4) {
	int h = sz(c), w = sz(c[0]);

	const vi& dx = (nb == 4 ? dx4 : dx8);
	const vi& dy = (nb == 4 ? dy4 : dy8);

	g = Graph(h * w);
	rep(x, h) {
		rep(y, w) {
			// 空きマスでなかったら辺は追加しない．
			if (c[x][y] == wall) continue;

			// 今考えている近傍それぞれについて
			rep(k, nb) {
				// 近傍のマスの座標
				int nx = x + dx[k];
				int ny = y + dy[k];

				// 範囲外だったり空きマスでなかったら辺は追加しない．
				if (nx < 0 || nx >= h || ny < 0 || ny >= w || c[nx][ny] == wall) {
					continue;
				}

				// 近傍に空きマスがあったら辺を追加する．
				g[x * w + y].push_back(nx * w + ny);
			}
		}
	}
}


//【逆グラフ】O(|V| + |E|)
/*
* 有向グラフ g の辺の向きを逆にしたグラフを g_rev に格納する．
*/
void reverse_graph(const Graph& g, Graph& g_rev) {
	int n = sz(g);
	g_rev = Graph(n);

	rep(s, n) {
		repe(t, g[s]) {
			g_rev[t].push_back(s);
		}
	}
}


//【逆グラフ（コスト付き）】O(|V| + |E|)
/*
* コスト付き有向グラフ g の辺の向きを逆にしたグラフを g_rev に格納する．
*/
void reverse_graph(const WGraph& g, WGraph& g_rev) {
	int n = sz(g);
	g_rev = WGraph(n);

	rep(s, n) {
		repe(e, g[s]) {
			g_rev[e.to].push_back({ s, e.cost });
		}
	}
}


//【補グラフ】O(|V|^2)
/*
* 無向グラフ g の補グラフ（単純，自己ループなし）を cg に格納する
*/
void complement_graph(const Graph& g, Graph& cg) {
	int n = sz(g);
	cg = Graph(n);

	rep(s, n) {
		vb e(n, true);
		e[s] = false;

		repe(t, g[s]) e[t] = false;

		rep(t, n) {
			if (e[t]) cg[s].push_back(t);
		}
	}
}


//【不要な頂点の除去】O(|V| + |E|)
/*
* グラフ g の総次数 0 の頂点を除去したグラフを gs に構成する．
* また gs の頂点数を返す．
*/
int shrink_graph(const Graph& g, Graph& gs) {
	// verify : https://atcoder.jp/contests/jsc2021/tasks/jsc2021_g

	int n = sz(g);

	// used[s] : 頂点 s の総次数が 1 以上か
	vb used(n);
	rep(s, n) {
		repe(t, g[s]) {
			used[s] = used[t] = true;
		}
	}

	// is[s] : 頂点 s の新しいインデックス
	vi is(n); int ns = 0;
	rep(s, n) {
		if (used[s]) {
			is[s] = ns++;
		}
	}

	// 無駄な頂点を除去したグラフを gs に構成する
	gs = Graph(ns);
	rep(s, n) {
		int ss = is[s];
		repe(t, g[s]) {
			int ts = is[t];

			gs[ss].push_back(ts);
		}
	}

	return ns;
}


