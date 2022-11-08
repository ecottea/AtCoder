#pragma once
#include "header.h"
// ■■■■■ グラフ（構造） ■■■■■


//【グラフ】
/*
* Graph g（Graph = vvi）
* g[v] : 頂点 v から行ける頂点を並べたリスト
*/


//【グラフの入力】O(|V| + |E|)
/*
* 始点 終点の組からなる入力を受け取り，n 頂点 m 辺のグラフを構築する．
*
* n : グラフの頂点の数
* m : グラフの辺の数
* g : ここにグラフを構築して返す
* undirected : 無向グラフなら true
* one_indexed : 入力が 1-indexed で与えられるなら true
*/
void read_graph(int n, int m, Graph& g,	bool undirected = true, bool one_indexed = true) {
	g = Graph(n);
	rep(i, m) {
		int a, b;
		cin >> a >> b;

		if (one_indexed) { a--; b--; }

		g[a].push_back(b);
		if (undirected) g[b].push_back(a);
	}
}


//【コスト付きグラフの辺】
/*
* to : 行き先の頂点番号
* cost : 辺のコスト
*/
struct WEdge {
	// verify : https://judge.yosupo.jp/problem/shortest_path

	int to; // 行き先の頂点番号
	ll cost; // 辺のコスト

	WEdge() : to(-1), cost(-INFL) {}
	WEdge(int to_, ll cost_) : to(to_), cost(cost_) {}

	// プレーングラフで呼ばれたとき用
	operator int() const { return to; }

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const WEdge& e) {
		os << '(' << e.to << ',' << e.cost << ')';
		return os;
	}
#endif
};


//【コスト付きグラフ】
/*
* WGraph g
* g[v] : 頂点 v から出る辺を並べたリスト
* 
* verify : https://judge.yosupo.jp/problem/shortest_path
*/ 
using WGraph = vector<vector<WEdge>>;


//【コスト付きグラフの入力】O(|V| + |E|)
/*
* 始点 終点 コストの組からなる入力を受け取り，n 頂点 m 辺のコスト付きグラフを構築する．
*
* n : グラフの頂点の数
* m : グラフの辺の数
* g : ここにグラフを構築して返す
* undirected : 無向グラフなら true
* one_indexed : 入力が 1-indexed で与えられるなら true
*/
void read_graph(int n, int m, WGraph& g, bool undirected = true, bool one_indexed = true) {
	// verify : https://judge.yosupo.jp/problem/shortest_path
	
	g = WGraph(n);
	rep(i, m) {
		int a, b; ll c;
		cin >> a >> b >> c;

		if (one_indexed) { a--; b--; }

		g[a].push_back({ b, c });
		if (undirected) g[b].push_back({ a, c });
	}
}


//【参照付きグラフの辺】
/*
* to : 行き先の頂点番号
* id : 辺の番号
*/
struct IEdge {
	// verify : https://judge.yosupo.jp/problem/cycle_detection_undirected

	int to; // 行き先の頂点番号
	int id; // 辺の番号
	bool dir; // 順方向か

	IEdge() : to(-1), id(-1), dir(true) {}
	IEdge(int to, int id, bool dir) : to(to), id(id), dir(dir) {}

	// プレーングラフで呼ばれたとき用
	operator int() const { return to; }

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const IEdge& e) {
		os << '(' << "to:" << e.to << ',' << "id:" << e.id << ',' << (e.dir ? "fwd" : "rev") << ')';
		return os;
	}
#endif
};


//【参照付きグラフ】
/*
* IGraph g
* g[v] : 頂点 v から出る辺を並べたリスト
*
* verify : https://judge.yosupo.jp/problem/cycle_detection_undirected
*/
using IGraph = vector<vector<IEdge>>;


//【参照付きグラフの入力】O(|V| + |E|)
/*
* 始点 終点 の組からなる入力を受け取り，n 頂点 m 辺の参照付きグラフを構築する．
*
* n : グラフの頂点の数
* m : グラフの辺の数
* g : ここにグラフを構築して返す
* undirected : 無向グラフなら true
* one_indexed : 入力が 1-indexed で与えられるなら true
*/
void read_graph(int n, int m, IGraph& g, bool undirected = true, bool one_indexed = true) {
	// verify : https://judge.yosupo.jp/problem/cycle_detection_undirected

	g = IGraph(n);
	rep(i, m) {
		int a, b;
		cin >> a >> b;

		if (one_indexed) { a--; b--; }

		g[a].push_back({ b, i, true });
		if (undirected) g[b].push_back({ a, i, false });
	}
}


