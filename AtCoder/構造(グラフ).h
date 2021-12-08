#pragma once
#include "header.h"
#include "ビット全探索.h"
// ■■■■■ グラフ（構造） ■■■■■


//【グラフ】
/*
* Graph g
* g[v] : 頂点 v から行ける頂点を並べたリスト
*/
using Graph = vvi;


//【グラフの入力】O(|V| + |E|)
/*
* 始点 終点の組からなる入力を受け取り，n 頂点 m 辺のグラフを構成する．
*
* n : グラフの頂点の数
* m : グラフの辺の数
* g : ここにグラフを構築して返す
* directed : 有向グラフなら true
* one_indexed : 入力が 1-indexed で与えられるなら true
*/
void read_graph(int n, int m, Graph& g,
	bool directed = false, bool one_indexed = true) {
	g = Graph(n);
	rep(i, m) {
		int a, b;
		cin >> a >> b;

		if (one_indexed) { a--; b--; }

		g[a].push_back(b);
		if (!directed) g[b].push_back(a);
	}
}


//【コスト付きグラフの辺】
/*
* to : 行き先の頂点番号
* cost : 辺のコスト
*/
struct Edge {
	// 参考：https://nyaannyaan.github.io/library/graph/graph-template.hpp

	int to; // 行き先の頂点番号
	ll cost; // 辺のコスト

	// コストなしグラフで呼ばれたとき用
	operator int() const { return to; }

	// デバッグ出力
	friend ostream& operator<<(ostream& os, const Edge& e) {
		os << '(' << e.to << ',' << e.cost << ')';
		return os;
	}
};


//【コスト付きグラフ】
/*
* WGraph g
* g[v] : 頂点 v から出る辺を並べたリスト
*/
using WGraph = vector<vector<Edge>>;


//【コスト付きグラフの入力】O(|V| + |E|)
/*
* 始点 終点 コストの組からなる入力を受け取り，n 頂点 m 辺のコスト付きグラフを構成する．
*
* n : グラフの頂点の数
* m : グラフの辺の数
* g : ここにグラフを構築して返す
* directed : 有向グラフなら true
* one_indexed : 入力が 1-indexed で与えられるなら true
*/
void read_graph(int n, int m, WGraph& g,
	bool directed = false, bool one_indexed = true) {
	g = WGraph(n);
	rep(i, m) {
		int a, b; ll c;
		cin >> a >> b >> c;

		if (one_indexed) { a--; b--; }

		g[a].push_back({ b, c });
		if (!directed) g[b].push_back({ a, c });
	}
}


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


