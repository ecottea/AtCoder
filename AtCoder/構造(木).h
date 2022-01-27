#pragma once
#include "header.h"
#include "構造(グラフ).h"
// ■■■■■ 木（構造） ■■■■■


//【木の入力】O(n)
/*
* 親を並べた入力を受け取り，n 頂点の木を構成する．
*
* n : グラフの頂点の数
* g : ここにグラフを構築して返す
* directed : 有向グラフなら true
* one_indexed : 入力が 1-indexed で与えられるなら true
*/
void read_tree(int n, Graph& g, bool directed = false, bool one_indexed = true) {
	// verify : https://algo-method.com/tasks/525

	g = Graph(n);
	repi(i, 1, n - 1) {
		int p;
		cin >> p;

		if (one_indexed) p--;

		g[p].push_back(i);
		if (!directed) g[i].push_back(p);
	}
}


//【根付き木】
/*
* Rooted_tree() : O(1)
*	空で初期化する．
*
* Rooted_tree(g, r) : O(n)
*	木 g を r を根とみなした根付き木として受け取る．
*/
struct Rooted_tree {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/all/ALDS1_7_A

	struct Node {
		int parent = -1; // 親（なければ -1）
		vi child; // 子（なければ空リスト）
		int depth = -1; // 深さ（根からのパスの長さ）
		int& dist = depth; // 深さを距離ともみなす（パスのコストを 1 とみなす）
		int weight = -1; // 重さ（部分木のもつ辺の数）
		int height = -1; // 高さ（最も遠い葉までの距離）

		// デバッグ出力
		friend ostream& operator<<(ostream& os, const Node& v) {
			os << "(p:" << v.parent << ", c:[" << v.child << "], d:" << v.depth
				<< ", w:" << v.weight << ", h:" << v.height << ")";
			return os;
		}
	};

	int n; // 頂点数
	int r; // 根
	vector<Node> v; // 頂点

	// コンストラクタ（空で初期化，木と根で初期化）
	Rooted_tree() : n(0), r(-1) {}
	Rooted_tree(Graph& g, int r_) : n(sz(g)), r(r_), v(n) {
		// s : 注目ノード，p : s の親
		function<void(int, int)> dfs = [&](int s, int p) {
			v[s].parent = p;
			v[s].child.clear();
			v[s].weight = 0;
			v[s].height = 0;

			repe(t, g[s]) {
				if (t == p) continue;

				v[t].depth = v[s].depth + 1;

				dfs(t, s);

				v[s].child.push_back(t);
				v[s].weight += v[t].weight + 1;
				chmax(v[s].height, v[t].height + 1);
			}
		};

		// 根 r を始点として再帰関数を呼び出す．
		v[r].depth = 0;
		dfs(r, -1);
	}

	// アクセス
	Node const& operator[](int i) const { return v[i]; }
	Node& operator[](int i) { return v[i]; }

	// 大きさ
	int size() const { return n; }

	// デバッグ出力
	friend ostream& operator<<(ostream& os, const Rooted_tree& rt) {
		rep(i, rt.n) os << rt[i] << endl;
		return os;
	}
};


//【根付き木の入力】O(n)
/*
* ([自身] 子の数 子のリスト) を並べた入力を受け取り，n 頂点の根付き木 rt を構築する．
*
* one_indexed : 入力が 1-indexed で与えられるなら true
* shuffled : [自身] の指定があるなら true
*/
void read_rooted_tree(int n, Rooted_tree& rt, bool one_indexed = true, bool shuffled = false) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/all/ALDS1_7_A

	// is_root[v] : v は根か
	vb is_root(n, true);

	// 一旦無向グラフとして入力を受け取る
	Graph g(n);
	rep(i, n) {
		int id, k;

		if (shuffled) {
			cin >> id;
			if (one_indexed) id--;
		}
		else id = i;

		cin >> k;
		rep(j, k) {
			int c;
			cin >> c;
			if (one_indexed) c--;

			g[id].push_back(c);
			g[c].push_back(id);

			is_root[c] = false;
		}
	}

	// 根の決定
	int r = -1;
	rep(i, n) {
		if (is_root[i]) {
			r = i;
			break;
		}
	}

	// 根付き木の構築
	rt = Rooted_tree(g, r);
}


//【コスト付き根付き木】
/*
* Weighted_rooted_tree() : O(1)
*	空で初期化する．
* 
* Weighted_rooted_tree(g, r) : O(n)
*	コスト付き木 g を r を根とみなしたコスト付き根付き木として受け取る．
*/
struct Weighted_rooted_tree {
	struct Node {
		int parent = -1; // 親（なければ -1）
		vector<WEdge> child; // 子（なければ空リスト）
		int depth = -1; // 深さ（根からのパスの長さ）
		ll dist = -1; // 距離（根からのパスのコスト）
		int weight = -1; // 重さ（部分木のもつ辺の数）

		// デバッグ出力
		friend ostream& operator<<(ostream& os, const Node& v) {
			os << "(par:" << v.parent << ", cld:[" << v.child << "], dep:" << v.depth
				<< ", dist:" << v.dist << ", wgt:" << v.weight << ")";
			return os;
		}
	};

	int n; // 頂点数
	int r; // 根
	vector<Node> v; // 頂点

	// コンストラクタ（初期化なし，コスト付き木と根で初期化）
	Weighted_rooted_tree() : n(0), r(-1) {}
	Weighted_rooted_tree(WGraph& g, int r_) : n(sz(g)), v(n), r(r_) {
		// 再帰用の関数
		// s : 注目ノード，p : s の親
		function<void(int, int, ll)> dfs = [&](int s, int p, ll d) {
			v[s].parent = p;
			v[s].dist = d;
			v[s].child.clear();
			v[s].weight = 0;

			repe(t, g[s]) {
				if (t == p) continue;

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

	// アクセス
	Node const& operator[](int i) const { return v[i]; }
	Node& operator[](int i) { return v[i]; }

	// 大きさ
	int size() const { return n; }

	// デバッグ出力
	friend ostream& operator<<(ostream& os, const Weighted_rooted_tree& rt) {
		rep(i, rt.n) os << rt[i] << endl;
		return os;
	}
};


