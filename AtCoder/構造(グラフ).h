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
struct WEdge {
	int to; // 行き先の頂点番号
	ll cost; // 辺のコスト

	// コストなしグラフで呼ばれたとき用
	operator int() const { return to; }

	// デバッグ出力
	friend ostream& operator<<(ostream& os, const WEdge& e) {
		os << '(' << e.to << ',' << e.cost << ')';
		return os;
	}
};


//【コスト付きグラフ】
/*
* WGraph g
* g[v] : 頂点 v から出る辺を並べたリスト
*/
using WGraph = vector<vector<WEdge>>;


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


//【コスト付きグラフの入力（コストは別）】O(|V| + |E|)
/*
* 始点 終点 コストの組からなる入力を受け取り，n 頂点 m 辺のグラフを構成する．
* 辺へのコストの割り当ては別で記録する．
*
* n : グラフの頂点の数
* m : グラフの辺の数
* g : ここにグラフを構築して返す
* c : 辺 (s, t) のコストを c[s][t] に格納する
* directed : 有向グラフなら true
* one_indexed : 入力が 1-indexed で与えられるなら true
*/
void read_graph(int n, int m, Graph& g, vector<unordered_map<int, ll>>& c,
	bool directed = false, bool one_indexed = true) {
	g = Graph(n);
	c = vector<unordered_map<int, ll>>(n);
	rep(i, m) {
		int a, b; ll x;
		cin >> a >> b >> x;

		if (one_indexed) { a--; b--; }

		g[a].push_back(b);
		c[a][b] = x;
		if (!directed) {
			g[b].push_back(a);
			c[b][a] = x;
		}
	}
}


//【参照付きグラフの辺】
/*
* to : 行き先の頂点番号
* id : 辺の番号
*/
struct IEdge {
	int to; // 行き先の頂点番号
	int id; // 辺の番号

	// コストなしグラフで呼ばれたとき用
	operator int() const { return to; }

	// デバッグ出力
	friend ostream& operator<<(ostream& os, const IEdge& e) {
		os << '(' << e.to << ',' << e.id << ')';
		return os;
	}
};


//【参照付きグラフ】
/*
* IGraph g
* g[v] : 頂点 v から出る辺を並べたリスト
*/
using IGraph = vector<vector<IEdge>>;


//【参照付きグラフの入力】O(|V| + |E|)
/*
* 始点 終点 の組からなる入力を受け取り，n 頂点 m 辺の参照付きグラフを構成する．
*
* n : グラフの頂点の数
* m : グラフの辺の数
* g : ここにグラフを構築して返す
* directed : 有向グラフなら true
* one_indexed : 入力が 1-indexed で与えられるなら true
*/
void read_graph(int n, int m, IGraph& g,
	bool directed = false, bool one_indexed = true) {
	g = IGraph(n);
	rep(i, m) {
		int a, b;
		cin >> a >> b;

		if (one_indexed) { a--; b--; }

		g[a].push_back({ b, i });
		if (!directed) g[b].push_back({ a, i });
	}
}


