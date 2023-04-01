#pragma once
#include "header.h"
#include "構造(グラフ).h"
// ■■■■■ グラフの変換 ■■■■■


//【グリッド → グラフ】O(h w)
/*
* h 行 w 列のグリッドから 4 近傍を連結としたグラフ g を返す．
* 壁マスは wall，空きマスはその他とする．
* i 行目の j 列目にあるマス (i, j) はグラフ頂点 i * w + j に対応する．
*/
template <class T>
Graph grid_to_graph(const vector<vector<T>>& c, T wall = '#') {
	int h = sz(c), w = sz(c[0]);

	Graph g(h * w);
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

	return g;
}


//【壁情報 → グラフ】O(h w)
/*
* マス (i, j) と (i + 1, j) の間の壁の有無が wx[0..h-1)[0..w) で，
* マス (i, j) と (i, j + 1) の間の壁の有無が wy[0..h)[0..w-1) で表されたグリッドにおいて，
* 4 近傍を連結としたグラフ g を返す（壁があることは wall で表す）
* マス (i, j) はグラフ頂点 i * w + j に対応する．
*/
template <class T>
Graph wall_to_graph(const vector<vector<T>>& wx, const vector<vector<T>>& wy, T wall = '1') {
	// verify : https://atcoder.jp/contests/abc168/tasks/abc168_f

	int h = sz(wy), w = sz(wx[0]);
	Graph g(h * w);

	rep(x, h) rep(y, w) {
		if (x > 0 && wx[x - 1][y] != wall) g[x * w + y].push_back((x - 1) * w + y);
		if (x < h - 1 && wx[x][y] != wall) g[x * w + y].push_back((x + 1) * w + y);
		if (y > 0 && wy[x][y - 1] != wall) g[x * w + y].push_back(x * w + (y - 1));
		if (y < w - 1 && wy[x][y] != wall) g[x * w + y].push_back(x * w + (y + 1));
	}

	return g;
}


//【隣接行列 → グラフ】O(|V|^2)
/*
* 隣接行列 e[0..n)[0..n) で辺の有無が表される有向グラフ g を返す．
*/
template <class T>
Graph construct_graph(const vector<vector<T>>& e, T exist = 1) {
	int n = sz(e);
	Graph g(n);
	rep(i, n) rep(j, n) if (e[i][j] == exist) g[i].push_back(j);

	return g;
}


//【始点[終点] の任意化】
/*
* 始点を任意にしたい場合，超頂点 ST から各頂点へ重み 0 の辺を張り，ST を始点とすればよい．
* verify : https://atcoder.jp/contests/tdpc/tasks/tdpc_graph
*
* 終点を任意にしたい場合，超頂点 GL へ各頂点へ重み 0 の辺を張り，GL を終点とすればよい．
*/


//【逆グラフ】O(|V| + |E|)
/*
* 有向グラフ g の辺の向きを逆にしたグラフを返す．
*/
Graph reverse_graph(const Graph& g) {
	// verify : https://atcoder.jp/contests/nikkei2019-qual/tasks/nikkei2019_qual_d

	int n = sz(g);
	Graph g_rev(n);

	rep(s, n) repe(t, g[s]) g_rev[t].push_back(s);

	return g_rev;
}


//【逆グラフ（コスト付き）】O(|V| + |E|)
/*
* コスト付き有向グラフ g の辺の向きを逆にしたグラフを返す．
*/
WGraph reverse_graph(const WGraph& g) {
	int n = sz(g);
	WGraph g_rev(n);

	rep(s, n) repe(e, g[s]) g_rev[e.to].emplace_back(s, e.cost);

	return g_rev;
}


//【補グラフ】O(|V|^2)
/*
* 無向グラフ g の補グラフ（単純，自己ループなし）を返す．
*/
Graph complement_graph(const Graph& g) {
	// verify : https://atcoder.jp/contests/abc187/tasks/abc187_f

	int n = sz(g);
	Graph cg(n);

	rep(s, n) {
		vb e(n, true); // 多重辺はなし（辺の有無だけを考える）
		e[s] = false; // 自己ループはなし

		repe(t, g[s]) e[t] = false;

		rep(t, n) if (e[t]) cg[s].push_back(t);
	}

	return cg;
}


//【誘導部分グラフ】O(|V| + |E|)
/*
* グラフ g について，頂点集合を vs とする誘導部分グラフを返す．
*/
template <class G>
G induced_subgraph(const G& g, const vi& vs) {
	// verify : https://atcoder.jp/contests/abc253/tasks/abc253_h

	int n = sz(g), n2 = sz(vs);

	// id[v] : g の頂点が誘導部分グラフの何番目の頂点に対応するか（無ければ -1）
	vi id(n, -1);
	rep(i, n2) id[vs[i]] = i;

	G g2(n2);

	// 選ばれていない頂点は無視しながら g2 に誘導部分グラフを構築する．
	rep(s, n) {
		if (id[s] == -1) continue;

		repe(t, g[s]) {
			if (id[t] == -1) continue;

			g2[id[s]].push_back(id[t]);
		}
	}

	return g2;
}


//【頂点の除去】
/*
* グラフ g から頂点の集合 V' とそれに接続する辺を除去したグラフは，
* V\V' による g の誘導部分グラフに等しい．
* 
* verify : https://onlinejudge.u-aizu.ac.jp/problems/2347
*/


//【辺の除去】O(|V| + |E|)
/*
* 有向グラフ g から辺の集合 e_el を除去した有向グラフを返す．
* 辺 e∈e_el は始点 s と終点 t の順序対 (s, t) で表す．
*/
template <class G>
G eliminate_edge(const G& g, const vector<pii>& e_el) {
	// verify : https://atcoder.jp/contests/agc032/tasks/agc032_c

	int n = sz(g);
	G g2(n);

	vector<unordered_set<int>> el(n);
	repe(e, e_el) {
		int s, t;
		tie(s, t) = e;

		el[s].insert(t);
	}

	rep(s, n) repe(t, g[s]) {
		if (el[s].count(t)) continue;

		g2[s].push_back(t);
	}

	return g2;
}


//【頂点の縮約】O(|V| + |E|)
/*
* グラフ g とその頂点の分割 p について，成分 p[i] を 1 つの頂点 i として縮約したグラフを返す．
* 自己ループや多重辺が生じた場合は除去され，結果は単純グラフとなる．
*/
Graph vertex_contraction(const Graph& g, const vvi& p) {
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

	// 結果の構築
	Graph gc(m);
	rep(s, m) repe(t, gc_set[s]) gc[s].push_back(t);

	return gc;
}


//【頂点の縮約と強連結成分】
/*
* 有向グラフ g の強連結成分を成す頂点集合を縮約すると DAG が得られる．
* 
* verify : https://atcoder.jp/contests/abc245/tasks/abc245_f
*/


//【頂点の縮約と二重辺連結成分】
/*
* 無向グラフ g の二重辺連結成分を成す頂点集合を縮約すると森が得られる．
* 
* verify : https://atcoder.jp/contests/tenka1-2015-quala/tasks/tenka1_2015_qualA_d
*/


//【辺の縮約】O(|V| + |E|)
/*
* グラフ g とその辺の集合 es について，es に含まれる辺を全て縮約したグラフ gc を返す．
* また gc の頂点 i に対応する g の頂点の集合を vs[i] に格納する．
* 自己ループや多重辺が生じた場合は除去され，gc は単純グラフとなる．
*/
Graph edge_contraction(const Graph& g, const vector<pii>& es, vvi* vs = nullptr) {
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
	Graph gc(m);
	rep(s, m) repe(t, gc_set[s]) gc[s].push_back(t);

	return gc;
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


