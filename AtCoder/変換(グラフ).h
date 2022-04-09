#pragma once
#include "header.h"
#include "構造(グラフ).h"
// ■■■■■ グラフの変換 ■■■■■


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


//【壁情報 → グラフ】O(h w)
/*
* マス (i, j) と (i + 1, j) の間の壁の有無が wx[0..h-1)[0..w) で，
* マス (i, j) と (i, j + 1) の間の壁の有無が wy[0..h)[0..w-1) で表されたグリッドにおいて，
* 4 近傍を連結としたグラフ g を構築する（壁があることは wall で表す）
* マス (i, j) はグラフ頂点 i * w + j に対応する．
*/
template <class T>
void wall_to_graph(const vector<vector<T>>& wx, const vector<vector<T>>& wy, Graph& g, T wall = '1') {
	// verify : https://atcoder.jp/contests/abc168/tasks/abc168_f

	int h = sz(wy), w = sz(wx[0]);
	g = Graph(h * w);

	rep(x, h) {
		rep(y, w) {
			if (x > 0 && wx[x - 1][y] != wall) g[x * w + y].push_back((x - 1) * w + y);
			if (x < h - 1 && wx[x][y] != wall) g[x * w + y].push_back((x + 1) * w + y);
			if (y > 0 && wy[x][y - 1] != wall) g[x * w + y].push_back(x * w + (y - 1));
			if (y < w - 1 && wy[x][y] != wall) g[x * w + y].push_back(x * w + (y + 1));
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
	// verify : https://atcoder.jp/contests/abc187/tasks/abc187_f

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


//【大きい頂点への移動】
/*
* 頂点 V = s[0..n) ∪ t[0..n) をもち，O(n^2) 個の有向辺
*		s[i] → t[j] (i <= j)
* をもつ有向二部グラフ g を考える．
* 
* 同じく V を頂点にもつ有向グラフ g' を，O(n) 個の有向辺
*		s[i] → s[i + 1] (i < n - 1)
*		t[i] → t[i + 1] (i < n - 1)
*		s[i] → t[i]     (i < n)
* をもつよう構成しても，s[i] から t[j] への移動可能性は g と変わらない．
*/


//【大きい頂点への移動コスト】
/*
* 頂点 V = s[0..n) ∪ t[0..n) をもち，O(n^2) 個の有向辺
*		s[i] → t[j] : コスト 0  (i <= j)
*		s[i] → t[j] : コスト c  (i > j)
* をもつコスト付き有向二部グラフ g を考える．
*
* 同じく V を頂点にもつコスト付き有向グラフ g' を，O(n) 個の有向辺
*		s[i] → s[i + 1] : コスト 0 (i < n - 1)
*		t[i] → t[i + 1] : コスト 0 (i < n - 1)
*		s[n - 1] → s[0] : コスト c
* をもつよう構成しても，s[i] から t[j] への最小移動コストは g と変わらない．
*/


//【以外の頂点への移動】
/*
* 頂点 V = s[0..n) ∪ t[0..n) をもち，O(n^2) 個の有向辺
*		s[i] → t[j] (i != j)
* をもつ有向二部グラフ g を考える．
* 
* 頂点 V' = V ∪ tl[0..n) ∪ tr[0..n) をもつ有向グラフ g' を，O(n) 個の有向辺
*		s[i] → tl[i - 1], tr[i + 1]
*		tl[i] → tl[i - 1], t[i]
*		tr[i] → tr[i + 1], t[i]
* をもつよう構成しても，s[i] から t[j] への移動可能性は g と変わらない．
*
* verify : https://atcoder.jp/contests/abc210/tasks/abc210_f
*/


//【和が k 以下の頂点への移動】
/*
* 頂点 V = s[0..n) ∪ t[0..n) をもち，O(n^2) 個の有向辺
*		s[i] → t[j] (i + j <= k)
* をもつ有向二部グラフ g を考える．
*
* 同じく V を頂点にもつ有向グラフ g' を，O(n) 個の有向辺
*		s[i] → s[i + 1] (i < n - 1)
*		t[i + 1] → t[i] (i < n - 1)
*		s[i] → t[k - i] (i < k)
* をもつよう構成しても，s[i] から t[j] への移動可能性は g と変わらない．
* 
* verify : https://atcoder.jp/contests/abc232/tasks/abc232_g
*/


//【区間内の頂点全てへの移動】
/*
* 全ての辺を直接張ってしまうと O(n^2) 本必要になるが，
* セグメント木のように 2 冪個ずつ頂点をまとめれば O(n log n) 本で済む．
* 
* verify : https://atcoder.jp/contests/abc210/tasks/abc210_f
*/


