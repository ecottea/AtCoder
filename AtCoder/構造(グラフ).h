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
* directed : 有向グラフか（省略すれば false）
* zero_indexed : 入力が 0-indexed か（省略すれば false）
*/
Graph read_Graph(int n, int m = -1, bool directed = false, bool zero_indexed = false) {
	// verify : https://atcoder.jp/contests/tessoku-book/tasks/tessoku_book_bi

	Graph g(n);
	if (m == -1) m = n - 1;

	rep(j, m) {
		int a, b;
		cin >> a >> b;
		if (!zero_indexed) { --a; --b; }

		g[a].push_back(b);
		if (!directed && a != b) g[b].push_back(a);
	}

	return g;
}


//【グラフの出力】O(n + m)
/*
* グラフを【グラフの入力】で受け取る入力と同じ形式で出力する．
*
* directed : 有向グラフか（省略すれば false）
* zero_indexed : 入力が 0-indexed か（省略すれば false）
*/
void write_Graph(const Graph& g, bool directed = false, bool zero_indexed = false) {
	// verify : https://www.codechef.com/problems/B_BRANCH

	int n = sz(g);

	// m : 辺の数
	int m = 0;
	rep(s, n) m += sz(g[s]);
	if (!directed) m /= 2;

	cout << n << " " << m << endl;
	rep(s, n) repe(t, g[s]) {
		if (!directed && s > t) continue;

		int u = s + (!zero_indexed), v = t + (!zero_indexed);
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
		os << "(" << e.to << "," << e.cost << ")";
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
* directed : 有向グラフか（省略すれば false）
* zero_indexed : 入力が 0-indexed か（省略すれば false）
*/
WGraph read_WGraph(int n, int m = -1, bool directed = false, bool zero_indexed = false) {
	// verify : https://judge.yosupo.jp/problem/shortest_path

	WGraph g(n);
	if (m == -1) m = n - 1;

	rep(j, m) {
		int u, v; ll c;
		cin >> u >> v >> c;

		if (!zero_indexed) { --u; --v; }

		g[u].push_back({ v, c });
		if (!directed && u != v) g[v].push_back({ u, c });
	}

	return g;
}


//【重み付きグラフの出力】O(n + m)
/*
* 重み付きグラフを【重み付きグラフの入力】で受け取る入力と同じ形式で出力する．
*
* directed : 有向グラフか（省略すれば false）
* zero_indexed : 入力が 0-indexed か（省略すれば false）
*/
void write_WGraph(const WGraph& g, bool directed = false, bool zero_indexed = false) {
	// verify : https://atcoder.jp/contests/discovery2016-final/tasks/discovery_2016_final_d

	int n = sz(g);

	// m : 辺の数
	int m = 0;
	rep(s, n) m += sz(g[s]);
	if (!directed) m /= 2;

	cout << n << " " << m << endl;
	rep(s, n) repe(t, g[s]) {
		if (!directed && s > t) continue;

		int u = s + (!zero_indexed), v = t + (!zero_indexed);
		cout << u << " " << v << " " << t.cost << endl;
	}
}


//【参照付きグラフの辺】
/*
* int from : 始点
* int to : 終点
* int id : 辺番号
* bool dir : 順方向か
*/
struct IEdge {
	// verify : https://judge.yosupo.jp/problem/cycle_detection_undirected

	int from; // 始点
	int to; // 終点
	int id; // 辺番号
	bool dir; // 順方向か

	IEdge() : from(-1), to(-1), id(-1), dir(true) {}
	IEdge(int from, int to, int id, bool dir = true) : from(from), to(to), id(id), dir(dir) {}

	// プレーングラフで呼ばれたとき用
	operator int() const { return to; }

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const IEdge& e) {
		os << '(' << e.from << "→" << e.to << ',' << "id:" << e.id << ',' << (e.dir ? "fwd" : "rev") << ')';
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
* directed : 有向グラフか（省略すれば false）
* zero_indexed : 入力が 0-indexed か（省略すれば false）
*/
IGraph read_IGraph(int n, int m = -1, bool directed = false, bool zero_indexed = false, vector<pii>* es = nullptr) {
	// verify : https://judge.yosupo.jp/problem/cycle_detection_undirected

	IGraph g(n);
	if (m == -1) m = n - 1;
	if (es != nullptr) es->resize(m);

	rep(j, m) {
		int a, b;
		cin >> a >> b;

		if (!zero_indexed) { --a; --b; }

		g[a].push_back({ a, b, j, true });
		if (!directed && a != b) g[b].push_back({ b, a, j, false });

		if (es != nullptr) (*es)[j] = { a, b };
	}

	return g;
}


//【参照付きグラフの出力】O(n + m)
/*
* グラフを【グラフの入力】で受け取る入力と同じ形式で出力する．
*
* zero_indexed : 入力が 0-indexed か（省略すれば false）
*/
void write_Graph(const IGraph& g, bool zero_indexed = true) {
	int n = sz(g);

	// m : 辺の数
	int m = -1;
	rep(s, n) repe(t, g[s]) chmax(m, t.id);
	m++;

	vi u(m), v(m);
	rep(s, n) repe(t, g[s]) {
		if (t.dir) {
			u[t.id] = s + (!zero_indexed);
			v[t.id] = t + (!zero_indexed);
		}
	}

	cout << n << " " << m << endl;
	rep(j, m) cout << u[j] << " " << v[j] << endl;
}


//【Functional Graph】
/*
* 任意の頂点の出次数が 1 であるような有向グラフを Functional Graph という．
* G が Functional Graph であるとき，G の各弱連結成分には閉路がただ 1 つ存在する．
* G 自身は弱連結とは限らないので注意！
*
* verify : https://atcoder.jp/contests/abc256/tasks/abc256_e
*/


//【無向グラフのランダム生成】O(n^2)
/*
* n 頂点で，辺の存在確率が p % であるランダムな無向グラフを返す．
* self_loop : 自己ループを許可するか（デフォルトでは false）
*/
Graph create_random_undirected_Graph(int n, int p, bool self_loop = false) {
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
	if (self_loop) {
		rep(s, n) {
			if (rnd(mt) >= p) continue;
			g[s].emplace_back(s);
		}
	}

	return g;
}


//【有向グラフのランダム生成】O(n^2)
/*
* n 頂点で，辺の存在確率が p % であるランダムな有向グラフを返す．
* self_loop : 自己ループを許可するか（デフォルトでは false）
*/
Graph create_random_Graph(int n, int p, bool self_loop = false) {
	static mt19937_64 mt; static bool first_call = true;
	if (first_call) {
		mt = mt19937_64((int)time(NULL));
		first_call = false;
	}
	uniform_int_distribution<int> rnd_edge(0, 99);

	Graph g(n);
	rep(s, n) rep(t, n) {
		if (!self_loop && s == t) continue;
		if (rnd_edge(mt) >= p) continue;

		g[s].emplace_back(t);
	}

	return g;
}


//【重み付き有向グラフのランダム生成】O(n^2)
/*
* n 頂点で，コストが [c_min..c_max] 内の一様乱数で与えられ，
* 辺の存在確率が p % であるランダムな重み付き有向グラフを返す．
* self_loop : 自己ループを許可するか（デフォルトでは false）
*/
WGraph create_random_WGraph(int n, ll c_min, ll c_max, int p, bool self_loop = false) {
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
		if (!self_loop && s == t) continue;
		if (rnd_edge(mt) >= p) continue;

		g[s].emplace_back(t, rnd_cost(mt));
	}

	return g;
}


//【参照 & 重み付きグラフの辺】
/*
* int from : 始点
* int to : 終点
* ll cost : 重み
* int id : 辺番号
* bool dir : 順方向か
*/
struct IWEdge {
	int from; // 始点
	int to; // 終点
	ll cost; // 重み
	int id; // 辺番号
	bool dir; // 順方向か

	IWEdge() : from(-1), to(-1), cost(0), id(-1), dir(true) {}
	IWEdge(int from, int to, ll cost, int id, bool dir = true) : from(from), to(to), cost(cost), id(id), dir(dir) {}

	// プレーングラフで呼ばれたとき用
	operator int() const { return to; }

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const IWEdge& e) {
		os << '(' << e.from << "→" << e.to << ',' << "cost:" << e.cost
			<< "id:" << e.id << ',' << (e.dir ? "fwd" : "rev") << ')';
		return os;
	}
#endif
};


//【参照 & 重み付きグラフ】
/*
* IWGraph g
* g[v] : 頂点 v から出る辺を並べたリスト
*/
using IWGraph = vector<vector<IWEdge>>;


//【参照 & 重み付きグラフの入力】O(n + m)
/*
* (始点, 終点, 重み) の組からなる入力を受け取り，n 頂点 m 辺の参照 & 重み付きグラフを構築して返す．
* また必要なら j 番目の辺が u→v で重み c であることを es[j] = {u, v, c} として格納する．
*
* n : グラフの頂点の数
* m : グラフの辺の数（省略すれば n-1）
* directed : 有向グラフか（省略すれば false）
* zero_indexed : 入力が 0-indexed か（省略すれば false）
*/
IWGraph read_IGraph(int n, int m = -1, bool directed = false, bool zero_indexed = false, vector<tuple<int, int, ll>>* es = nullptr) {
	IWGraph g(n);
	if (m == -1) m = n - 1;
	if (es != nullptr) es->resize(m);

	rep(j, m) {
		int a, b; ll c;
		cin >> a >> b >> c;

		if (!zero_indexed) { --a; --b; }

		g[a].push_back({ a, b, c, j, true });
		if (!directed && a != b) g[b].push_back({ b, a, c, j, false });

		if (es != nullptr) (*es)[j] = { a, b, c };
	}

	return g;
}


