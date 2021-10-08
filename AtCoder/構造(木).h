#pragma once
#include "header.h"
#include "構造(グラフ).h"
// ■■■■■ 木（構造） ■■■■■



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


