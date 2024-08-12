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


//【グラフ → 隣接行列】O(n^2)
/*
* 有向グラフ g の 0-1 隣接行列 adj[0..n)[0..n) を返す．
*/
vvi adjacency_matrix(const Graph& g) {
	// verify : https://yukicoder.me/problems/no/2733

	int n = sz(g);

	vvi adj(n, vi(n));
	rep(s, n) repe(t, g[s]) adj[s][t] = 1;

	return adj;
}


//【グラフ → 隣接行列（重み付き）】O(n^2)
/*
* 重み付き有向グラフ g の重み付き隣接行列 adj[0..n)[0..n) を返す．
* 辺が無い頂点間の重みは，対角成分は diag，その他は non_diag とする．
*/
vvl adjacency_matrix(const WGraph& g, ll diag = 0, ll non_diag = INFL) {
	int n = sz(g);

	vvl adj(n, vl(n, non_diag));
	rep(i, n) adj[i][i] = diag;

	rep(s, n) repe(t, g[s]) adj[s][t] = t.cost;

	return adj;
}


//【隣接行列 → グラフ】O(n^2)
/*
* 隣接行列 e[0..n)[0..n) で辺の有無が表される有向グラフ g を返す．
*/
template <class T>
Graph from_adjacency_matrix(const vector<vector<T>>& e, T exist = 1) {
	// verify : https://mojacoder.app/users/take44444/contests/tbc001/tasks/5

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


//【逆グラフ】O(n + m)
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


//【逆グラフ（重み付き）】O(n + m)
/*
* 重み付き有向グラフ g の辺の向きを逆にしたグラフを返す．
*/
WGraph reverse_graph(const WGraph& g) {
	int n = sz(g);
	WGraph g_rev(n);

	rep(s, n) repe(e, g[s]) g_rev[e.to].emplace_back(s, e.cost);

	return g_rev;
}


//【補グラフ】O(n^2)
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


//【接続二部グラフ】
/*
* g の n 個の頂点に対応する頂点 [0..n) と m 本の辺に対応する頂点 [n..n+m) をもち，
* 頂点 i が辺 j の端点であるときに限り辺 i-(n+j) をもつ二部グラフ gc を返す．
* directed = true とすると有向辺 i→(n+j) にする．
*/
Graph incidence_bipartite_graph(const IGraph& g, bool directed = false) {
	// verify : https://atcoder.jp/contests/abc318/tasks/abc318_g

	int n = sz(g);

	int m = -1;
	rep(s, n) repe(t, g[s]) chmax(m, t.id);
	m++;

	Graph g2(n + m);
	rep(s, n) repe(t, g[s]) {
		g2[s].push_back(n + t.id);
		if (!directed) g2[n + t.id].push_back(s);
	}

	return g2;
}


//【誘導部分グラフ】O(n + m)
/*
* グラフ g について，頂点集合を vs とする誘導部分グラフを返す．
*/
Graph induced_subgraph(const Graph& g, const vi& vs) {
	// verify : https://atcoder.jp/contests/abc253/tasks/abc253_h

	int n = sz(g), n2 = sz(vs);

	// id[v] : g の頂点が誘導部分グラフの何番目の頂点に対応するか（無ければ -1）
	vi id(n, -1);
	rep(i, n2) id[vs[i]] = i;

	Graph g2(n2);

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


//【誘導部分グラフ（重み付き）】O(n + m)
/*
* 重み付きグラフ g について，頂点集合を vs とする誘導部分グラフを返す．
*/
WGraph induced_subgraph(const WGraph& g, const vi& vs) {
	// verify : https://leetcode.com/contest/biweekly-contest-119/problems/number-of-possible-sets-of-closing-branches/

	int n = sz(g), n2 = sz(vs);

	// id[v] : g の頂点が誘導部分グラフの何番目の頂点に対応するか（無ければ -1）
	vi id(n, -1);
	rep(i, n2) id[vs[i]] = i;

	WGraph g2(n2);

	// 選ばれていない頂点は無視しながら g2 に誘導部分グラフを構築する．
	rep(s, n) {
		if (id[s] == -1) continue;

		repe(t, g[s]) {
			if (id[t] == -1) continue;

			g2[id[s]].push_back({ id[t], t.cost });
		}
	}

	return g2;
}


//【誘導部分グラフ（参照付き）】O(n + m)
/*
* 重み付きグラフ g について，頂点集合を vs とする誘導部分グラフを返す．
*/
IGraph induced_subgraph(const IGraph& g, const vi& vs) {
	// verify : https://atcoder.jp/contests/abc336/tasks/abc336_g

	int n = sz(g), n2 = sz(vs);

	// v_id[v] : g の頂点が誘導部分グラフの何番目の頂点に対応するか（無ければ -1）
	vi v_id(n, -1);
	rep(i, n2) v_id[vs[i]] = i;

	IGraph g2(n2);

	// 選ばれていない頂点は無視しながら g2 に誘導部分グラフを構築する．
	rep(s, n) {
		if (v_id[s] == -1) continue;

		repe(t, g[s]) {
			if (v_id[t] == -1) continue;

			g2[v_id[s]].push_back({ v_id[s], v_id[t], t.id, t.dir });
		}
	}

	return g2;
}


//【頂点の除去】
/*
* 有向グラフ g から頂点の集合 V' とそれに接続する辺を除去したグラフは，
* V\V' による g の誘導部分グラフに等しい．
* 
* verify : https://onlinejudge.u-aizu.ac.jp/problems/2347
*/


//【辺の除去】O(n + m)
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
	for (auto [s, t] : e_el) el[s].insert(t);

	rep(s, n) repe(t, g[s]) {
		if (el[s].count(t)) continue;

		g2[s].push_back(t);
	}

	return g2;
}


//【頂点の縮約】O(n + m)
/*
* 有向グラフ g とその頂点の分割 p について，成分 p[i] を 1 つの頂点 i として縮約したグラフを返す．
* 自己ループや多重辺が生じた場合は除去され，結果は単純グラフとなる．
* not_simple = true とすると自己ループや多重辺の除去を行わない．
*/
Graph vertex_contraction(const Graph& g, const vvi& p, bool not_simple = false) {
	// verify : https://atcoder.jp/contests/arc030/tasks/arc030_3

	int n = sz(g), m = sz(p);

	// id[v] : 頂点 v の属する成分
	vi id(n);
	rep(i, m) repe(v, p[i]) id[v] = i;

	if (!not_simple) {
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
	else {
		Graph gc(m);
		rep(s, n) repe(t, g[s]) gc[id[s]].push_back(id[t]);

		return gc;
	}
}


//【頂点の縮約（重み付き）】O(n + m)
/*
* 重み付き有向グラフ g とその頂点の分割 p について，成分 p[i] を 1 つの頂点 i として縮約したグラフを返す．
* 自己ループや多重辺の除去は行わない．
*/
WGraph vertex_contraction(const WGraph& g, const vvi& p) {
	int n = sz(g), m = sz(p);

	// id[v] : 頂点 v の属する成分
	vi id(n);
	rep(i, m) repe(v, p[i]) id[v] = i;

	WGraph gc(m);
	rep(s, n) repe(t, g[s]) gc[id[s]].push_back({ id[t], t.cost });

	return gc;
}


//【強連結成分の頂点を縮約 → DAG】
/*
* 有向グラフ g の強連結成分を成す頂点集合全てを縮約すると DAG が得られる．
* 
* verify : https://atcoder.jp/contests/abc245/tasks/abc245_f
*/


//【トーナメントグラフの SCC の頂点を縮約 → パス】
/*
* トーナメントグラフ g の強連結成分を成す頂点集合全てを縮約するとパスグラフが得られる．
* 
* verify : https://atcoder.jp/contests/arc163/tasks/arc163_d
*/


//【二重辺連結成分の頂点を縮約 → 森】
/*
* 無向グラフ g の二重辺連結成分を成す頂点集合を縮約すると森が得られる．
* 
* verify : https://atcoder.jp/contests/tenka1-2015-quala/tasks/tenka1_2015_qualA_d
*/


//【接続二部グラフの二重頂点連結成分の辺に対する頂点を縮約 → 森】
/*
* 無向グラフ g の辺を二重頂点連結成分を成す辺集合に分割して E = 凵_k E_k とする．
* g の接続二部グラフに対し，各 E_k に対応する頂点集合を縮約すると森が得られる．
* これは block-cut tree に似た特性をもつ．
*
* verify : https://atcoder.jp/contests/abc318/tasks/abc318_g
*/


//【辺の縮約】O(n + m)
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


//【橋以外の辺を縮約 → 森】
/*
* 無向グラフ g の橋以外の辺全てを縮約すると森が得られる．
* 
* verify : https://atcoder.jp/contests/tenka1-2015-quala/tasks/tenka1_2015_qualA_d
*/


//【左右の区間へ辺を張れるグラフ】
/*
* Cointerval_Graph(int n) : O(n)
*	有向グラフ g を n 頂点 0 辺で初期化する．
*
* add_edge(int s, int t) : O(1)
*	頂点 s から頂点 t に辺を張る．
*
* set_vertices(vi vs) : O(K)
*	注目頂点集合を vs[0..K) に設定する．
*
* add_edge_to_left(int s, int j) : O(1)
*	頂点 s から区間 vs[0..j] に辺を張る．
*
* add_edge_to_right(int s, int j) : O(1)
*	頂点 s から区間 vs[j..K) に辺を張る．
*
* add_edge_to_without(int s, int j) : O(1)
*	頂点 s から 2 区間 vs[0..j) ∪ vs(j..K) に辺を張る．
*
* Graph& get() : O(n + ΣK + Q)（Q : add_edge 系を呼び出した回数）
*	g への参照を返す．
*/
class Cointerval_Graph {
	int n;		// 頂点数
	int L, R;

public:
	Graph g;	// 有向グラフ

	// 有向グラフ g を n 頂点 0 辺で初期化する．
	Cointerval_Graph(int n) : n(n), L(0), R(0), g(n) {
		// verify : https://atcoder.jp/contests/abc210/tasks/abc210_f
	}
	Cointerval_Graph() : n(0), L(0), R(0) {}

	// 頂点 s から頂点 t に辺を張る．
	void add_edge(int s, int t) {
		g[s].emplace_back(t);
	}

	// 注目頂点集合を vs[0..K) に設定する．
	void set_vertices(const vi& vs) {
		// verify : https://atcoder.jp/contests/abc210/tasks/abc210_f

		int K = sz(vs);

		// 左右移動用レーンの分の頂点を新たに作成する．
		L = n; R = n + K; n = R + K;
		g.resize(n);

		// 左移動用レーンに左方向への辺を張る．
		repi(k, 1, K - 1) g[L + k].emplace_back(L + (k - 1));

		// 右移動用レーンに右方向への辺を張る．
		repi(k, 0, K - 2) g[R + k].emplace_back(R + (k + 1));

		// 左右移動用レーンから元の頂点への辺を張る
		repi(k, 0, K - 1) {
			g[L + k].emplace_back(vs[k]);
			g[R + k].emplace_back(vs[k]);
		}
	}

	// 頂点 s から区間 vs[0..j] に辺を張る．
	void add_edge_to_left(int s, int j) {
		g[s].emplace_back(L + j);
	}

	// 頂点 s から区間 vs[j..K) に辺を張る．
	void add_edge_to_right(int s, int j) {
		g[s].emplace_back(R + j);
	}

	// 頂点 s から 2 区間 vs[0..j) ∪ vs(j..K) に辺を張る．
	void add_edge_to_without(int s, int j) {
		// verify : https://atcoder.jp/contests/abc210/tasks/abc210_f

		if (j > 0) g[s].emplace_back(L + (j - 1));
		if (R + j < n - 1) g[s].emplace_back(R + (j + 1));
	}

	// グラフへの参照を返す．
	Graph& get() {
		// verify : https://atcoder.jp/contests/abc210/tasks/abc210_f

		return g;
	}
};


//【左右の区間へ辺を張れるグラフ（重み付き）】
/*
* Cointerval_WGraph(int n) : O(n)
*	重み付き有向グラフ g を n 頂点 0 辺で初期化する．
*
* add_edge(int s, int t, ll c) : O(1)
*	頂点 s から頂点 t に重み c の辺を張る．
*
* set_vertices(vi vs) : O(K)
*	注目頂点集合を vs[0..K) に設定する．
*
* add_edge_to_left(int s, int j, ll c) : O(1)
*	頂点 s から区間 vs[0..j] に重み c の辺を張る．
*
* add_edge_to_right(int s, int j, ll c) : O(1)
*	頂点 s から区間 vs[j..K) に重み c の辺を張る．
*
* add_edge_to_without(int s, int j, ll c) : O(1)
*	頂点 s から 2 区間 vs[0..j) ∪ vs(j..K) に重み c の辺を張る．
*
* WGraph& get() : O(n + ΣK + Q)（Q : add_edge 系を呼び出した回数）
*	g への参照を返す．
*/
class Cointerval_WGraph {
	int n;		// 頂点数
	int L, R;

public:
	WGraph g;	// 重み付き有向グラフ

	// 重み付き有向グラフ g を n 頂点 0 辺で初期化する．
	Cointerval_WGraph(int n) : n(n), L(0), R(0), g(n) {
		// verify : https://atcoder.jp/contests/abc210/tasks/abc210_f
	}
	Cointerval_WGraph() : n(0), L(0), R(0) {}

	// 頂点 s から頂点 t に重み c の辺を張る．
	void add_edge(int s, int t, ll c) {
		g[s].emplace_back(t, c);
	}

	// 注目頂点集合を vs[0..K) に設定する．
	void set_vertices(const vi& vs) {
		// verify : https://atcoder.jp/contests/abc210/tasks/abc210_f

		int K = sz(vs);

		// 左右移動用レーンの分の頂点を新たに作成する．
		L = n; R = n + K; n = R + K;
		g.resize(n);

		// 左移動用レーンに左方向へのコスト 0 の辺を張る．
		repi(k, 1, K - 1) g[L + k].emplace_back(L + (k - 1), 0);

		// 右移動用レーンに右方向へのコスト 0 の辺を張る．
		repi(k, 0, K - 2) g[R + k].emplace_back(R + (k + 1), 0);

		// 左右移動用レーンから元の頂点へのコスト 0 の辺を張る
		repi(k, 0, K - 1) {
			g[L + k].emplace_back(vs[k], 0);
			g[R + k].emplace_back(vs[k], 0);
		}
	}

	// 頂点 s から区間 vs[0..j] に重み c の辺を張る．
	void add_edge_to_left(int s, int j, ll c) {
		g[s].emplace_back(L + j, c);
	}

	// 頂点 s から区間 vs[j..K) に重み c の辺を張る．
	void add_edge_to_right(int s, int j, ll c) {
		g[s].emplace_back(R + j, c);
	}

	// 頂点 s から 2 区間 vs[0..j) ∪ vs(j..K) に重み c の辺を張る．
	void add_edge_to_without(int s, int j, ll c) {
		// verify : https://atcoder.jp/contests/abc210/tasks/abc210_f

		if (j > 0) g[s].emplace_back(L + (j - 1), c);
		if (R + j < n - 1) g[s].emplace_back(R + (j + 1), c);
	}

	// グラフへの参照を返す．
	WGraph& get() {
		// verify : https://atcoder.jp/contests/abc210/tasks/abc210_f

		return g;
	}
};


//【区間と辺を張れるグラフ】
/*
* Interval_Graph(int n) : O(n)
*	有向グラフ g を n 頂点 0 辺で初期化する．
*
* add_edge(int s, int t) : O(1)
*	頂点 s から頂点 t に辺を張る．
*
* set_vertices(vi vs) : O(K)
*	注目頂点集合を vs[0..K) に設定する．
*
* set_vertices_to_all() : O(n)
*	注目頂点集合を [0..n) に設定する．
*
* add_edge_to_interval(int s, int l, int r) : O(log K)
*	頂点 s から区間 vs[l..r) に辺を張る．
*
* add_edge_from_interval(int l, int r, int t) : O(log K)
*	区間 vs[l..r) から頂点 t に辺を張る．
*/
class Interval_Graph {
	int n; // 頂点数
	int It = 0, If = 0;
	vi vs;

public:
	Graph g; // 有向グラフ

	// 有向グラフ g を n 頂点 0 辺で初期化する．
	Interval_Graph(int n) : n(n), g(n) {
		// verify : https://atcoder.jp/contests/abc210/tasks/abc210_f
	}
	Interval_Graph() : n(0) {}

	// 頂点 s から頂点 t に辺を張る．
	void add_edge(int s, int t) {
		// verify : https://codeforces.com/contest/1903/problem/F

		g[s].emplace_back(t);
	}

	// 注目頂点集合を vs[0..K) に設定する．
	void set_vertices(const vi& vs_) {
		// verify : https://atcoder.jp/contests/abc210/tasks/abc210_f

		vs = vs_;
		int K = sz(vs);

		// 区間へ辺を張るための完全二分木用の頂点を新たに作成する（不要ならコメントアウト可）
		It = n - 1; n += K - 1;
		g.resize(n);
		repi(i, 1, K - 1) {
			// 左の区間への辺を張る．
			int l = 2 * i;
			g[It + i].emplace_back(l < K ? It + l : vs[l - K]);

			// 右の区間への辺を張る．
			int r = 2 * i + 1;
			g[It + i].emplace_back(r < K ? It + r : vs[r - K]);
		}

		// 区間から辺を張るための完全二分木用の頂点を新たに作成する（不要ならコメントアウト可）
		If = n - 1; n += K - 1;
		g.resize(n);
		repi(i, 1, K - 1) {
			// 左の区間からの辺を張る．
			int l = 2 * i;
			g[l < K ? If + l : vs[l - K]].emplace_back(If + i);

			// 右の区間からの辺を張る．
			int r = 2 * i + 1;
			g[r < K ? If + r : vs[r - K]].emplace_back(If + i);
		}
	}

	// 注目頂点集合を [0..n) に設定する．
	void set_vertices_to_all() {
		vi vs(n);
		iota(all(vs), 0);

		set_vertices(vs);
	}

	// 頂点 s から区間 vs[l..r) に辺を張る．
	void add_edge_to_interval(int s, int l, int r) {
		// verify : https://atcoder.jp/contests/abc210/tasks/abc210_f

		int K = sz(vs);
		chmax(l, 0); chmin(r, K);
		l += K; r += K;

		while (l < r) {
			if (l & 1) {
				g[s].emplace_back(l < K ? It + l : vs[l - K]);
				l++;
			}
			if (r & 1) {
				g[s].emplace_back(r - 1 < K ? It + r - 1 : vs[r - 1 - K]);
			}
			l >>= 1; r >>= 1;
		}
	}

	// 区間 vs[l..r) から頂点 s に辺を張る．
	void add_edge_from_interval(int l, int r, int t) {
		// verify : https://codeforces.com/contest/1904/problem/F

		int K = sz(vs);
		chmax(l, 0); chmin(r, K);
		l += K; r += K;

		while (l < r) {
			if (l & 1) {
				g[l < K ? If + l : vs[l - K]].emplace_back(t);
				l++;
			}
			if (r & 1) {
				g[r - 1 < K ? If + r - 1 : vs[r - 1 - K]].emplace_back(t);
			}
			l >>= 1; r >>= 1;
		}
	}
};


//【区間へ辺を張れるグラフ（重み付き）】
/*
* Interval_WGraph(int n) : O(n)
*	重み付き有向グラフ g を n 頂点 0 辺で初期化する．
*
* add_edge(int s, int t, ll c) : O(1)
*	頂点 s から頂点 t に重み c の辺を張る．
*
* set_vertices(vi vs) : O(K)
*	注目頂点集合を vs[0..K) に設定する．
*
* set_vertices_to_all() : O(n)
*	注目頂点集合を [0..n) に設定する．
*
* add_edge_to_interval(int s, int l, int r, ll c) : O(log K)
*	頂点 s から区間 vs[l..r) に重み c の辺を張る．
*
* WGraph& get() : O(n + ΣK + Q log K)（Q : add_edge 系を呼び出した回数）
*	g への参照を返す．
*/
class Interval_WGraph {
	int n0, n;	// 頂点数
	int I;
	vi vs;

public:
	WGraph g;	// 重み付き有向グラフ

	// 重み付き有向グラフ g を n 頂点 0 辺で初期化する．
	Interval_WGraph(int n) : n0(n), n(n), I(0), g(n) {
		// verify : https://atcoder.jp/contests/abc210/tasks/abc210_f
	}
	Interval_WGraph() : n0(0), n(0), I(0) {}

	// 頂点 s から頂点 t に重み c の辺を張る．
	void add_edge(int s, int t, ll c) {
		Assert(s >= 0 && s < n0 && t >= 0 && t < n0);

		g[s].emplace_back(t, c);
	}

	// 注目頂点集合を vs[0..K) に設定する．
	void set_vertices(const vi& vs_) {
		// verify : https://atcoder.jp/contests/abc210/tasks/abc210_f

		vs = vs_;
		int K = sz(vs);

		// 完全二分木用の頂点を新たに作成する．
		I = n - 1; n += K - 1;
		g.resize(n);

		repi(i, 1, K - 1) {
			// 左の区間への重み 0 の辺を張る．
			int l = 2 * i;
			g[I + i].emplace_back(l < K ? I + l : vs[l - K], 0);

			// 右の区間への重み 0 の辺を張る．
			int r = 2 * i + 1;
			g[I + i].emplace_back(r < K ? I + r : vs[r - K], 0);
		}
	}

	// 注目頂点集合を [0..n) に設定する．
	void set_vertices_to_all() {
		vi vs(n0);
		iota(all(vs), 0);

		set_vertices(vs);
	}

	// 頂点 s から区間 vs[l..r) に重み c の辺を張る．
	void add_edge_to_interval(int s, int l, int r, ll c) {
		// verify : https://atcoder.jp/contests/abc210/tasks/abc210_f

		Assert(s >= 0 && s < n0);

		int K = sz(vs);
		chmax(l, 0); chmin(l, K);
		if (l >= r) return;

		l += K; r += K;

		while (l < r) {
			if (l & 1) {
				g[s].emplace_back(l < K ? I + l : vs[l - K], c);
				l++;
			}
			if (r & 1) {
				g[s].emplace_back(r - 1 < K ? I + r - 1 : vs[r - 1 - K], c);
			}
			l >>= 1; r >>= 1;
		}
	}

	// グラフへの参照を返す．
	WGraph& get() {
		// verify : https://atcoder.jp/contests/abc210/tasks/abc210_f

		return g;
	}
};


//【大きい頂点への移動】
/*
* 頂点 V = s[0..n) ∪ t[0..n) をもち，O(n^2) 個の有向辺
*		s[i] → t[j] (i ≦ j)
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
*		s[i] → t[j] : コスト 0  (i ≦ j)
*		s[i] → t[j] : コスト c  (i > j)
* をもつ重み付き有向二部グラフ g を考える．
*
* 同じく V を頂点にもつ重み付き有向グラフ g' を，O(n) 個の有向辺
*		s[i] → s[i + 1] : コスト 0 (i < n - 1)
*		t[i] → t[i + 1] : コスト 0 (i < n - 1)
*		s[n - 1] → s[0] : コスト c
* をもつよう構成しても，s[i] から t[j] への最小移動コストは g と変わらない．
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


//【無向 functional graph の分解】O(n)（未 verify）
/*
* 辺 i-f[i] をもつ n 頂点 n 辺の無向グラフを連結成分に分解し，連結成分の頂点のリストを ccs とする．
* また各連結成分について，辺 v-f[v] を取り除くと連結成分が木になるような v のリストを es とする．
* これらの組 {ccs, es} を返す．
*/
pair<vvi, vi> functional_graph_decomposition(const vi& f) {
	int n = sz(f);

	vvi ccs; vi es; int pt = 0;

	// id[s] : 頂点 s が何番目の連結成分に属しているか
	vi id(n, -1);

	rep(st, n) {
		// st が分類済の頂点なら何もしない．
		if (id[st] != -1) continue;

		vi cs;

		int s = st;
		cs.emplace_back(s);
		id[s] = pt;

		while (1) {
			int t = f[s];

			if (id[t] == pt) {
				ccs.emplace_back(cs);
				es.emplace_back(s);
				pt++;
				break;
			}
			else if (id[t] != -1) {
				int i = id[t];
				repe(v, cs) {
					ccs[i].emplace_back(v);
					id[v] = i;
				}
				break;
			}

			cs.emplace_back(t);
			id[t] = pt;
			s = t;
		}
	}

	return { ccs, es };
}


