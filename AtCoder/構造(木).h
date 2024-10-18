#pragma once
#include "header.h"
#include "構造(グラフ).h"
// ■■■■■ 木（構造） ■■■■■


//【木の入力】O(n)
/*
* 親を並べた入力を受け取り，n 頂点の木を構成して返す．
*
* n : グラフの頂点の数
* directed : 有向グラフか（省略すれば false）
* zero_indexed : 入力が 0-indexed か（省略すれば false）
*/
Graph read_tree(int n, bool directed = false, bool zero_indexed = false) {
	// verify : https://judge.yosupo.jp/problem/vertex_add_subtree_sum

	Graph g(n);
	repi(i, 1, n - 1) {
		int p;
		cin >> p;

		if (!zero_indexed) p--;

		g[p].push_back(i);
		if (!directed) g[i].push_back(p);
	}

	return g;
}


//【木の出力】O(n + m)
/*
* 木を【木の入力】で受け取る入力と同じ形式で出力する．
*
* directed : 有向木か（省略すれば false）
* zero_indexed : 入力が 0-indexed か（省略すれば false）
*/
void write_Tree(const Graph& g, bool directed = false, bool zero_indexed = false) {
	int n = sz(g);

	cout << n << endl;
	rep(s, n) repe(t, g[s]) {
		if (!directed && s > t) continue;

		int u = s + (!zero_indexed), v = t + (!zero_indexed);
		cout << u << " " << v << " " << endl;
	}
}


//【木のランダム生成】O(?)
/*
* n 頂点のランダムな木を返す．
*/
Graph create_random_tree(int n) {
	Graph g(n);

	static mt19937_64 mt; static bool first_call = true;
	if (first_call) {
		mt = mt19937_64((int)time(NULL));
		first_call = false;
	}

	uniform_int_distribution<int> rnd(0, n - 1);

	dsu d(n);
	while (d.size(0) < n) {
		int u = rnd(mt), v = rnd(mt);
		if (d.same(u, v)) continue;

		g[u].emplace_back(v);
		g[v].emplace_back(u);
		d.merge(u, v);
	}

	return g;
}


//【重み付き木のランダム生成】O(?)
/*
* n 頂点でコストが [c_min..c_max] 内の一様乱数で与えられるランダムな重み付き木を返す．
*/
WGraph create_random_Wtree(int n, ll c_min, ll c_max) {
	WGraph g(n);

	static mt19937_64 mt; static bool first_call = true;
	if (first_call) {
		mt = mt19937_64((int)time(NULL));
		first_call = false;
	}

	vector<pii> es;
	rep(i, n) repi(j, i + 1, n - 1) es.emplace_back(i, j);
	shuffle(all(es), mt);

	uniform_int_distribution<ll> rnd(c_min, c_max);

	dsu d(n);
	repe(e, es) {
		int u, v;
		tie(u, v) = e;
		if (d.same(u, v)) continue;

		ll c = rnd(mt);
		g[u].emplace_back(v, c);
		g[v].emplace_back(u, c);
		d.merge(u, v);
	}

	return g;
}


//【根付き木のランダム生成】O(n)
/*
* 自身より小さい頂点を親とする n 頂点のランダムな有向根付き木を返す．（根は 0）
* child_limit には子の個数の最大値を与える．
*/
Graph create_random_rooted_tree(int n, int child_limit = INF) {
	// verify : https://atcoder.jp/contests/abc329/tasks/abc329_g

	Graph g(n);

	static mt19937_64 mt; static bool first_call = true;
	if (first_call) {
		mt = mt19937_64((int)time(NULL));
		first_call = false;
	}

	uniform_int_distribution<int> rnd(0, 1 << 30);

	vi ps{ 0 };

	repi(i, 1, n - 1) {
		int j = rnd(mt) % sz(ps);
		int p = ps[j];

		g[p].push_back(i);

		if (sz(g[p]) == child_limit) {
			swap(ps[j], ps.back());
			ps.pop_back();
		}

		ps.push_back(i);
	}

	return g;
}


//【根付き木】
/*
* Rooted_tree() : O(1)
*	空で初期化する．
*
* Rooted_tree(Graph g, int r) : O(n)
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

#ifdef _MSC_VER
		friend ostream& operator<<(ostream& os, const Node& v) {
			os << "(p:" << v.parent << ", c:[";
			repe(s, v.child) os << s << " ";
			os << "], d:" << v.depth << ", w:" << v.weight << ", h:" << v.height << ")";
			return os;
		}
#endif
	};

	int n; // 頂点数
	int r; // 根
	vector<Node> v; // 頂点

	// コンストラクタ（空で初期化，木と根で初期化）
	Rooted_tree() : n(0), r(-1) {}
	Rooted_tree(const Graph& g, int r_) : n(sz(g)), r(r_), v(n) {
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

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Rooted_tree& rt) {
		rep(i, rt.n) os << rt[i] << endl;
		return os;
	}
#endif
};


//【根付き木の入力】O(n)
/*
* ([自身] 子の数 子のリスト) を並べた入力を受け取り，n 頂点の根付き木を構築して返す．
*
* zero_indexed : 入力が 0-indexed か（省略すれば false）
* shuffled : [自身] の指定があるか（省略すれば false）
*/
Rooted_tree read_rooted_tree(int n, bool zero_indexed = false, bool shuffled = false) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/all/ALDS1_7_A

	// is_root[v] : v は根か
	vb is_root(n, true);

	// 一旦無向グラフとして入力を受け取る
	Graph g(n);
	rep(i, n) {
		int id, k;

		if (shuffled) {
			cin >> id;
			if (!zero_indexed) id--;
		}
		else id = i;

		cin >> k;
		rep(j, k) {
			int c;
			cin >> c;
			if (!zero_indexed) c--;

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
	return Rooted_tree(g, r);
}


//【重み付き根付き木】
/*
* Weighted_rooted_tree() : O(1)
*	空で初期化する．
*
* Weighted_rooted_tree(WGraph g, int r) : O(n)
*	重み付き木 g を r を根とみなした重み付き根付き木として受け取る．
*/
struct Weighted_rooted_tree {
	struct Node {
		int parent = -1; // 親（なければ -1）
		vector<WEdge> child; // 子（なければ空リスト）
		int depth = -1; // 深さ（根からのパスの長さ）
		ll dist = -1; // 距離（根からのパスのコスト）
		int weight = -1; // 重さ（部分木のもつ辺の数）

#ifdef _MSC_VER
		friend ostream& operator<<(ostream& os, const Node& v) {
			os << "(par:" << v.parent << ", cld:[";
			repe(s, v.child) os << s << " ";
			os << "], dep:" << v.depth << ", dist:" << v.dist << ", wgt:" << v.weight << ")";
			return os;
		}
#endif
	};

	int n; // 頂点数
	int r; // 根
	vector<Node> v; // 頂点

	// コンストラクタ（初期化なし，重み付き木と根で初期化）
	Weighted_rooted_tree() : n(0), r(-1) {}
	Weighted_rooted_tree(const WGraph& g, int r_) : n(sz(g)), v(n), r(r_) {
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

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Weighted_rooted_tree& rt) {
		rep(i, rt.n) os << rt[i] << endl;
		return os;
	}
#endif
};


//【二分木】
/*
* Binary_Tree() : O(1)
*	空で初期化する．
*
* Binary_Tree(vi s, vi l, vi r) : O(n)
*	s[i] の左の子が l[i]，右の子が r[i] であるような二分木で初期化する．
*	存在しない場合は -1 を与える．
*/
struct Binary_Tree {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/all/ALDS1_7_B

	struct Node {
		int parent = -1; // 親（なければ -1）
		int left = -1; // 左の子（なければ -1）
		int right = -1; // 右の子（なければ -1）
		int depth = -1; // 深さ（根からのパスの長さ）
		int height = -1; // 高さ（最も遠い葉へのパスの長さ）
		int weight = -1; // 重さ（自身を根とする部分木の頂点の数）

#ifdef _MSC_VER
		// 出力
		friend ostream& operator<<(ostream& os, const Node& v) {
			os << "(p:" << v.parent << ", l:" << v.left << ", r:" << v.right <<
				", d:" << v.depth << ", h:" << v.height << ", w:" << v.weight << ')';
			return os;
		}
#endif
	};

	int n; // 頂点の数
	int root; // 根
	vector<Node> v; // 頂点

	// コンストラクタ（初期化なし，子の情報で初期化）
	Binary_Tree() : n(0), root(-1) {}
	Binary_Tree(const vi& s, const vi& l, const vi& r) : n(sz(s)), root(-1), v(n) {
		// 親子関係を設定する．
		rep(i, n) {
			v[s[i]].left = l[i];
			v[s[i]].right = r[i];
			if (l[i] != -1) v[l[i]].parent = s[i];
			if (r[i] != -1) v[r[i]].parent = s[i];
		}

		// 親が設定されていないノードが根である．
		rep(i, n) {
			if (v[i].parent == -1) {
				root = i;
				break;
			}
		}

		// 頂点の各種情報を決定する（s : 注目ノード，p : s の親）
		function<void(int)> dfs = [&](int s) {
			v[s].weight = 1;
			v[s].height = 0;

			int t = v[s].left;
			if (t != -1) {
				v[t].depth = v[s].depth + 1;
				dfs(t);
				v[s].weight += v[t].weight;
				chmax(v[s].height, v[t].height + 1);
			}

			t = v[s].right;
			if (t != -1) {
				v[t].depth = v[s].depth + 1;
				dfs(t);
				v[s].weight += v[t].weight;
				chmax(v[s].height, v[t].height + 1);
			}
		};

		// 根 root を始点として再帰関数を呼び出す．
		v[root].depth = 0;
		dfs(root);
	}

	// アクセス
	Node const& operator[](int i) const { return v[i]; }
	Node& operator[](int i) { return v[i]; }

	// 大きさ
	int size() const { return n; }

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Binary_Tree& rt) {
		rep(i, sz(rt)) os << rt[i] << endl;
		return os;
	}
#endif
};


//【二分木の入力】O(n)
/*
* (自身 左の子 右の子) を並べた入力を受け取り，n 頂点の二分木を構築し返す．
* 非存在を表す入力を nval に与える．
*
* zero_indexed : 入力が 0-indexed か（省略すれば false）
*/
Binary_Tree read_binary_tree(int n, bool zero_indexed = false, int nval = -1) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/all/ALDS1_7_B

	vi s(n), l(n), r(n);
	rep(i, n) {
		cin >> s[i] >> l[i] >> r[i];

		if (s[i] == nval) s[i] = -1;
		if (l[i] == nval) l[i] = -1;
		if (r[i] == nval) r[i] = -1;

		if (!zero_indexed) {
			if (s[i] != -1) s[i]--;
			if (l[i] != -1) l[i]--;
			if (r[i] != -1) r[i]--;
		}
	}

	return Binary_Tree(s, l, r);
}


