#pragma once
#include "header.h"
// ■■■■■ グラフ（構造） ■■■■■


//【グラフ】
/*
* Graph g（Graph = vvi）
* g[v] : 頂点 v から行ける頂点を並べたリスト
*/


//【グラフの入力】O(n + m)
/*
* (始点, 終点) の組からなる入力を受け取り，n 頂点 m 辺のグラフを構築して返す．
*
* n : グラフの頂点の数
* m : グラフの辺の数（省略すれば n-1）
* undirected : 無向グラフか（省略すれば true）
* one_indexed : 入力が 1-indexed か（省略すれば true）
*/
Graph read_Graph(int n, int m = -1, bool undirected = true, bool one_indexed = true) {
	// verify : https://codeforces.com/contest/764/problem/C

	Graph g(n);
	if (m == -1) m = n - 1;

	rep(i, m) {
		int a, b;
		cin >> a >> b;

		if (one_indexed) { --a; --b; }

		g[a].push_back(b);
		if (undirected) g[b].push_back(a);
	}

	return g;
}


//【グラフの出力】O(n + m)
/*
* グラフを【グラフの入力】で受け取る入力と同じ形式で出力する．
*
* undirected : 無向グラフか（省略すれば true）
* one_indexed : 入力が 1-indexed か（省略すれば true）
*/
void write_Graph(const Graph& g, bool undirected = true, bool one_indexed = true) {
	// verify : https://www.codechef.com/problems/B_BRANCH

	int n = sz(g);

	// m : 辺の数
	int m = 0;
	rep(s, n) m += sz(g[s]);
	if (undirected) m /= 2;

	cout << n << " " << m << endl;
	rep(s, n) repe(t, g[s]) {
		if (undirected && s > t) continue;

		int u = s + one_indexed, v = t + one_indexed;
		cout << u << " " << v << " " << endl;
	}
}


//【重み付きグラフの辺】
/*
* to : 行き先の頂点番号
* cost : 辺の重み
*/
struct WEdge {
	// verify : https://judge.yosupo.jp/problem/shortest_path

	int to; // 行き先の頂点番号
	ll cost; // 辺の重み

	WEdge() : to(-1), cost(-INFL) {}
	WEdge(int to, ll cost) : to(to), cost(cost) {}

	// プレーングラフで呼ばれたとき用
	operator int() const { return to; }

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const WEdge& e) {
		os << '(' << e.to << ',' << e.cost << ')';
		return os;
	}
#endif
};


//【重み付きグラフ】
/*
* WGraph g
* g[v] : 頂点 v から出る辺を並べたリスト
*
* verify : https://judge.yosupo.jp/problem/shortest_path
*/
using WGraph = vector<vector<WEdge>>;


//【重み付きグラフの入力】O(n + m)
/*
* (始点, 終点, 重み) の組からなる入力を受け取り，n 頂点 m 辺の重み付きグラフを構築して返す．
*
* n : グラフの頂点の数
* m : グラフの辺の数（省略すれば n-1）
* undirected : 無向グラフか（省略すれば true）
* one_indexed : 入力が 1-indexed か（省略すれば true）
*/
WGraph read_WGraph(int n, int m = -1, bool undirected = true, bool one_indexed = true) {
	// verify : https://judge.yosupo.jp/problem/shortest_path

	WGraph g(n);
	if (m == -1) m = n - 1;

	rep(i, m) {
		int a, b; ll c;
		cin >> a >> b >> c;

		if (one_indexed) { --a; --b; }

		g[a].push_back({ b, c });
		if (undirected) g[b].push_back({ a, c });
	}

	return g;
}


//【重み付きグラフの出力】O(n + m)
/*
* 重み付きグラフを【重み付きグラフの入力】で受け取る入力と同じ形式で出力する．
*
* undirected : 無向グラフか（省略すれば true）
* one_indexed : 入力が 1-indexed か（省略すれば true）
*/
void write_WGraph(const WGraph& g, bool undirected = true, bool one_indexed = true) {
	// verify : https://atcoder.jp/contests/discovery2016-final/tasks/discovery_2016_final_d

	int n = sz(g);

	// m : 辺の数
	int m = 0;
	rep(s, n) m += sz(g[s]);
	if (undirected) m /= 2;

	cout << n << " " << m << endl;
	rep(s, n) repe(t, g[s]) {
		if (undirected && s > t) continue;

		int u = s + one_indexed, v = t + one_indexed;
		cout << u << " " << v << " " << t.cost << endl;
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
	IEdge(int to, int id) : to(to), id(id), dir(true) {}
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


//【参照付きグラフの入力】O(n + m)
/*
* (始点, 終点) の組からなる入力を受け取り，n 頂点 m 辺の参照付きグラフを構築して返す．
* また必要なら j 番目の辺が u→v であることを es[j] = {u, v} として格納する．
*
* n : グラフの頂点の数
* m : グラフの辺の数（省略すれば n-1）
* undirected : 無向グラフか（省略すれば true）
* one_indexed : 入力が 1-indexed か（省略すれば true）
*/
IGraph read_IGraph(int n, int m = -1, bool undirected = true, bool one_indexed = true, vector<pii>* es = nullptr) {
	// verify : https://judge.yosupo.jp/problem/cycle_detection_undirected

	IGraph g(n);
	if (m == -1) m = n - 1;
	if (es != nullptr) es->resize(m);

	rep(j, m) {
		int a, b;
		cin >> a >> b;

		if (one_indexed) { --a; --b; }

		g[a].push_back({ b, j, true });
		if (undirected) g[b].push_back({ a, j, false });

		if (es != nullptr) (*es)[j] = { a, b };
	}

	return g;
}


//【Functional Graph】
/*
* 任意の頂点の出次数が 1 であるような有向グラフを Functional Graph という．
* G が Functional Graph であるとき，G の各弱連結成分には閉路がただ 1 つ存在する．
* G 自身は弱連結とは限らないので注意！！！
*
* verify : https://atcoder.jp/contests/abc256/tasks/abc256_e
*/


//【グリッドグラフ上のハミルトンサイクルの構築】
/*
* h×w グリッドグラフ上のハミルトンサイクルをなす点列を構築し返す．
*/
vector<pii> create_grid_hamilton_cycle(int h, int w) {
	// verify : https://atcoder.jp/contests/arc118/tasks/arc118_d

	// 幅が 1 の場合は蛇腹が構築できない．
	if (h == 1 || w == 1) return vector<pii>();;

	// h, w ともに奇数だと頂点数が奇数個になるが，
	// グリッドグラフは二部グラフなのでハミルトンサイクルは存在し得ない．
	if (h % 2 == 1 && w % 2 == 1) return vector<pii>();

	vector<pii> res{ {0, 0} };

	// h が偶数のときは，横方向に往復する蛇腹状に構築できる．
	if (h % 2 == 0) {
		rep(i, h) {
			if (i % 2 == 0)	repi(j, 1, w - 1) res.emplace_back(i, j);
			else repir(j, w - 1, 1) res.emplace_back(i, j);
		}
		repir(i, h - 1, 1) res.emplace_back(i, 0);
	}
	// w が偶数のときは，縦方向に往復する蛇腹状に構築できる．
	else {
		rep(j, w) {
			if (j % 2 == 0)	repi(i, 1, h - 1) res.emplace_back(i, j);
			else repir(i, h - 1, 1) res.emplace_back(i, j);
		}
		repir(j, w - 1, 1) res.emplace_back(0, j);
	}

	return res;
}


//【無向グラフのランダム生成】O(n^2)
/*
* n 頂点で，辺の存在確率が p % であるランダムな無向グラフを返す．
* no_loop : 自己ループを禁止するか（デフォルトでは true）
*/
Graph create_random_undirected_Graph(int n, int p, bool no_loop = true) {
	Graph g(n);

	static mt19937_64 mt; static bool first_call = true;
	if (first_call) {
		mt = mt19937_64((int)time(NULL));
		first_call = false;
	}

	uniform_int_distribution<int> rnd(0, 99);

	rep(s, n) repi(t, s + 1, n - 1) {
		if (rnd(mt) >= p) continue;
		g[s].emplace_back(t);
		g[t].emplace_back(s);
	}
	if (!no_loop) {
		rep(s, n) {
			if (rnd(mt) >= p) continue;
			g[s].emplace_back(s);
		}
	}

	return g;
}


//【重み付き有向グラフのランダム生成】O(n^2)
/*
* n 頂点で，コストが [c_min..c_max] 内の一様乱数で与えられ，
* 辺の存在確率が p % であるランダムな重み付き有向グラフを返す．
* no_loop : 自己ループを禁止するか（デフォルトでは true）
*/
WGraph create_random_WGraph(int n, ll c_min, ll c_max, int p, bool no_loop = true) {
	// verify : https://atcoder.jp/contests/discovery2016-final/tasks/discovery_2016_final_d

	WGraph g(n);

	static mt19937_64 mt; static bool first_call = true;
	if (first_call) {
		mt = mt19937_64((int)time(NULL));
		first_call = false;
	}

	uniform_int_distribution<ll> rnd_cost(c_min, c_max);
	uniform_int_distribution<int> rnd_edge(0, 99);

	rep(s, n) rep(t, n) {
		if (no_loop && s == t) continue;
		if (rnd_edge(mt) >= p) continue;

		g[s].emplace_back(t, rnd_cost(mt));
	}

	return g;
}


