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


//【グラフの入力】O(|E|)
/*
* 入力を受け取り n 頂点 m 辺のグラフを構成する．
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

		if (one_indexed) {
			a--;
			b--;
		}

		g[a].push_back(b);
		if (!directed) {
			g[b].push_back(a);
		}
	}
}


//【根付き木のノード】
/*
* parent : 親の頂点（なければ -1）
* child : 子のリスト（なければ空リスト）
* depth : 深さ（根からのパスの長さ）
* weight : 重さ（部分木のもつ辺の数）
*/
struct TNode {
	int parent = -1; // 親（なければ -1）
	vi child; // 子（なければ空リスト）
	int depth = -1; // 深さ（根からのパスの長さ）
	int weight = -1; // 重さ（部分木のもつ辺の数）

	// 出力
	friend ostream& operator<<(ostream& os, const TNode& v) {
		os << "(p:" << v.parent << ", c:" << v.child << ", d:" << v.depth
			<< ", w:" << v.weight << ")";
		return os;
	}
};


//【根付き木】
/*
* rt[i] : 根付き木の i 番目のノードの情報
* r : 根の頂点番号
*
* RTree(g, r) : O(|V|)
*	木 g を r を根とみなした根付き木として受け取る．
*/
struct RTree {
	int n;
	vector<TNode> v;
	int r;


	// コンストラクタ（木と根で初期化）
	RTree(Graph& g, int r_) : n(sz(g)), v(n), r(r_) {
		// 再帰用の関数
		// s : 注目ノード，p : s の親
		function<void(int, int)> dfs = [&](int s, int p) {
			v[s].parent = p;
			v[s].child.clear();
			v[s].weight = 0;

			repe(t, g[s]) {
				if (t == p) {
					continue;
				}

				v[t].depth = v[s].depth + 1;

				dfs(t, s);

				v[s].child.push_back(t);
				v[s].weight += v[t].weight + 1;
			}
		};

		// 根 r を始点として再帰関数を呼び出す．
		v[r].depth = 0;
		dfs(r, -1);
	}
};


//【コスト付きグラフの辺】
/*
* to : 行き先の頂点番号
* cost : 辺のコスト
*/
struct Edge {
	// 参考：https://nyaannyaan.github.io/library/graph/graph-template.hpp

	int to; // 行き先の頂点番号
	ll cost; // 辺のコスト

	// 出力
	friend ostream& operator<<(ostream& os, const Edge& e) {
		os << '(' << e.to << ',' << e.cost << ')';
		return os;
	}

	// コストなしグラフで呼ばれたとき用
	operator int() const {
		return to;
	}
};


//【コスト付きグラフ】
/*
* WGraph g
* g[v] : 頂点 v から出る辺を並べたリスト
*/
using WGraph = vector<vector<Edge>>;


//【コスト付きグラフの入力】O(|E|)
/*
* 入力を受け取り n 頂点 m 辺のコスト付きグラフを構成する．
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
		int a, b;
		ll c;
		cin >> a >> b >> c;

		if (one_indexed) {
			a--;
			b--;
		}

		g[a].push_back({ b, c });
		if (!directed) {
			g[b].push_back({ a, c });
		}
	}
}


//【コスト付き根付き木のノード】
/*
* parent : 親の頂点（なければ -1）
* child : 子への辺のリスト（なければ空リスト）
* depth : 深さ（根からのパスの長さ）
* dist : 根からの距離（根からのパスのコスト）
* weight : 重さ（部分木のもつ辺の数）
*/
struct WTNode {
	int parent = -1;
	vector<Edge> child;
	int depth = -1;
	ll dist = -1;
	int weight = -1;

	// 出力
	friend ostream& operator<<(ostream& os, const WTNode& v) {
		os << "(p:" << v.parent << ", c:" << v.child << ", d:" << v.depth
			<< ", d:" << v.dist << ", w:" << v.weight << ")";
		return os;
	}
};


//【コスト付き根付き木】
/*
* rt[i] : 根付き木の i 番目のノードの情報
* r : 根の頂点番号
*
* WRTree(g, r) : O(|V|)
*	コスト付き木 g を r を根とみなしたコスト付き根付き木として受け取る．
*/
struct WRTree {
	int n;
	vector<WTNode> v;
	int r;


	// コンストラクタ（木と根で初期化）
	WRTree(WGraph& g, int r_) : n(sz(g)), v(n), r(r_) {
		// 再帰用の関数
		// s : 注目ノード，p : s の親
		function<void(int, int, ll)> dfs = [&](int s, int p, ll d) {
			v[s].parent = p;
			v[s].dist = d;
			v[s].child.clear();
			v[s].weight = 0;

			repe(t, g[s]) {
				if (t == p) {
					continue;
				}

				v[t].depth = v[s].depth + 1;
				dfs(t, s, d + t.cost);

				v[s].child.push_back(t);
				v[s].weight += v[t].weight + 1;
			}
		};

		// 根 r を始点として再帰関数を呼び出す．
		v[r].depth = 0;
		dfs(r, -1, 0);
	}
};


//【辺のコストの取得】
/*
* コスト付きグラフの辺なら辺のコストを返す．
* さもなくば INFL を返す．
*/
inline ll get_cost(const Edge& e) {
	return e.cost;
}
inline ll get_cost(const int& t) {
	return INFL;
}


