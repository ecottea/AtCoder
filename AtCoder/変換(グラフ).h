#pragma once
#include "header.h"
#include "構造(グラフ).h"
// ■■■■■ グラフの変換 ■■■■■


//【グリッド → グラフ】O(h w)
/*
* h 行 w 列のグリッドから 4 近傍を連結としたグラフ g を構築する．
* 壁マスは wall，空きマスはその他とする．
* i 行目の j 列目にあるマス (i, j) はグラフ頂点 i * w + j に対応する．
*/
template <class T>
void grid_to_graph(const vector<vector<T>>& c, Graph& g, T wall = '#') {
	int h = sz(c), w = sz(c[0]);

	g = Graph(h * w);
	rep(x, h) rep(y, w) {
		// 空きマスでなかったら辺は追加しない．
		if (c[x][y] == wall) continue;

		// 今考えている近傍それぞれについて
		rep(k, 4) {
			// 4 近傍のマスの座標
			int nx = x + DX[k];
			int ny = y + DY[k];

			// 範囲外だったり空きマスでなかったら辺は追加しない．
			if (nx < 0 || nx >= h || ny < 0 || ny >= w || c[nx][ny] == wall) continue;

			// 近傍に空きマスがあったら辺を追加する．
			g[x * w + y].push_back(nx * w + ny);
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


//【隣接行列 → グラフ】O(|V|^2)
/*
* 隣接行列 e[0..n)[0..n) で辺 i→j の存在が e[i][j]=ex で表されるとし，
* 対応する有向グラフを g に格納する．
*/
template <class T> void construct_graph(const vector<vector<T>>& e, T ex, Graph& g) {
	int n = sz(e);
	g = Graph(n);
	rep(i, n) rep(j, n) if (e[i][j] == ex) g[i].push_back(j);
}


//【逆グラフ】O(|V| + |E|)
/*
* 有向グラフ g の辺の向きを逆にしたグラフを g_rev に格納する．
*/
void reverse_graph(const Graph& g, Graph& g_rev) {
	// verify : https://atcoder.jp/contests/nikkei2019-qual/tasks/nikkei2019_qual_d

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


//【誘導部分グラフ】O(|V| + |E|)
/*
* グラフ g について，頂点集合を vs とする誘導部分グラフを g2 に格納する．
*/
template <class G> void induced_subgraph(const G& g, const vi& vs, G& g2) {
	// verify : https://atcoder.jp/contests/abc253/tasks/abc253_h

	int n = sz(g), n2 = sz(vs);

	vi id(n, -1);
	rep(i, n2) id[vs[i]] = i;

	g2 = G(n2);

	rep(s, n) {
		if (id[s] == -1) continue;

		repe(t, g[s]) {
			if (id[t] == -1) continue;

			g2[id[s]].push_back(id[t]);
		}
	}
}


//【頂点の除去】O(|V| + |E|)
/*
* グラフ g から頂点の集合 v_el とそれに接続する辺を除去したグラフを g2 に格納し，頂点数を返す．
* また g2 の頂点 i が g のどの頂点と対応するかを prv[i] に格納する．
*/
int eliminate_vertex(const Graph& g, const vi& v_el, Graph& g2, vi* prv = nullptr) {
	// verify : https://onlinejudge.u-aizu.ac.jp/problems/2347

	int n = sz(g);
	if (prv != nullptr) prv->clear();

	// ids[v] : g の頂点 v が g2 の何番目の頂点になるか（消去されるなら -1）
	vi ids(n); int id = 0;
	repe(v, v_el) ids[v] = -1;

	rep(v, n) {
		if (ids[v] == -1) continue;

		ids[v] = id++;
		if (prv != nullptr) prv->emplace_back(v);
	}

	if (id == 0) {
		g2.clear();
		return 0;
	}

	g2 = Graph(id);

	rep(s, n) {
		if (ids[s] == -1) continue;

		repe(t, g[s]) {
			if (ids[t] == -1) continue;

			g2[ids[s]].emplace_back(ids[t]);
		}
	}

	return id;
}


//【辺の除去】O(|V| + |E|)
/*
* グラフ g から辺の集合 e_el を除去したグラフを g2 に格納する．
* 辺 e∈es は始点 s と終点 t の順序対 (s, t) で表す．
*/
template <class G> void eliminate_edge(const G& g, const vector<pii>& e_el, G& g2) {
	// verify : https://atcoder.jp/contests/agc032/tasks/agc032_c

	int n = sz(g);
	g2 = G(n);

	vector<unordered_set<int>> el(n);
	repe(e, e_el) {
		int s, t;
		tie(s, t) = e;

		el[s].insert(t);
	}

	rep(s, n) {
		repe(t, g[s]) {
			if (el[s].count(t)) continue;

			g2[s].push_back(t);
		}
	}
}


//【頂点の縮約】O(|V| + |E|)
/*
* グラフ g とその頂点の分割 p について，成分 p[i] を 1 つの頂点 i として縮約したグラフを gc に格納する．
* 自己ループや多重辺が生じた場合は除去され，gc は単純グラフとなる．
*
* 特に強連結成分についての縮約を行えば DAG が得られる．
*/
void vertex_contraction(const Graph& g, const vvi& p, Graph& gc) {
	// verify : https://atcoder.jp/contests/arc030/tasks/arc030_3

	int n = sz(g), m = sz(p);

	// id[v] : 頂点 v の属する成分
	vi id(n);
	rep(i, m) repe(v, p[i]) id[v] = i;

	// 多重辺や自己ループを防ぐため一旦辺の集合を unordered_set でもつ．
	vector<unordered_set<int>> gc_set(m);
	rep(s, n) {
		repe(t, g[s]) gc_set[id[s]].insert(id[t]);
		gc_set[id[s]].erase(id[s]);
	}

	// 結果の格納
	gc = Graph(m);
	rep(s, m) repe(t, gc_set[s]) gc[s].push_back(t);
}


//【頂点の縮約と強連結成分】
/*
* 有向グラフ g の強連結成分を成す頂点集合を縮約すると DAG が得られる．
* 
* verify : https://atcoder.jp/contests/abc245/tasks/abc245_f
*/


//【頂点の縮約と二辺連結成分】
/*
* 無向グラフ g の二辺連結成分を成す頂点集合を縮約すると森が得られる．
* 
* verify : https://atcoder.jp/contests/tenka1-2015-quala/tasks/tenka1_2015_qualA_d
*/


//【辺の縮約】O(|V| + |E|)
/*
* グラフ g とその辺の集合 es について，es に含まれる辺を全て縮約したグラフを gc に格納する．
* また gc の頂点 i に対応する g の頂点の集合を vs[i] に格納する．
* 自己ループや多重辺が生じた場合は除去され，gc は単純グラフとなる．
*/
void edge_contraction(const Graph& g, const vector<pii>& es, Graph& gc, vvi* vs = nullptr) {
	// verify : https://atcoder.jp/contests/tenka1-2015-quala/tasks/tenka1_2015_qualA_d

	int n = sz(g);

	dsu d(n);
	repe(e, es) d.merge(e.first, e.second);

	if (vs == nullptr) vs = new(vvi);
	*vs = d.groups();
	int m = sz(*vs);

	// ids[v] : g の頂点 v の縮約後 gc におけるの頂点番号
	vi id(n);
	rep(i, m) repe(s, (*vs)[i]) id[s] = i;

	// 多重辺や自己ループを防ぐため一旦辺の集合を unordered_set でもつ．
	vector<unordered_set<int>> gc_set(m);
	rep(s, n) {
		repe(t, g[s]) gc_set[id[s]].insert(id[t]);
		gc_set[id[s]].erase(id[s]);
	}

	// 結果の格納
	gc = Graph(m);
	rep(s, m) repe(t, gc_set[s]) gc[s].push_back(t);
}


//【辺の縮約と橋】
/*
* 無向グラフ g の橋以外の辺全てを縮約すると森が得られる．
* 
* verify : https://atcoder.jp/contests/tenka1-2015-quala/tasks/tenka1_2015_qualA_d
*/


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


