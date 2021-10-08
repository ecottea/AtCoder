#ifndef HIDDEN_IN_VISUAL_STUDIO // 折りたたみ用

// 警告の抑制
#define _CRT_SECURE_NO_WARNINGS

// 使えるライブラリの読み込み
#include <bits/stdc++.h>
using namespace std;

// 型名の短縮
using ll = long long; // -2^63 ～ 2^63 = 9 * 10^18（int は -2^31 ～ 2^31 = 2 * 10^9）
using pii = pair<int, int>;	using pll = pair<ll, ll>;	using pil = pair<int, ll>;	using pli = pair<ll, int>;
using vi = vector<int>;		using vvi = vector<vi>;		using vvvi = vector<vvi>;
using vl = vector<ll>;		using vvl = vector<vl>;		using vvvl = vector<vvl>;
using vb = vector<bool>;	using vvb = vector<vb>;		using vvvb = vector<vvb>;
using vc = vector<char>;	using vvc = vector<vc>;		using vvvc = vector<vvc>;
using vd = vector<double>;	using vvd = vector<vd>;		using vvvd = vector<vvd>;
template <class T> using priority_queue_rev = priority_queue<T, vector<T>, greater<T>>;
using Graph = vvi;

// 定数の定義
const double PI = 3.14159265359;
const double DEG = PI / 180.; // θ [deg] = θ * DEG [rad]
const vi dx4 = { 1, 0, -1, 0 }; // 4 近傍（下，右，上，左）
const vi dy4 = { 0, 1, 0, -1 };
const vi dx8 = { 1, 1, 0, -1, -1, -1, 0, 1 }; // 8 近傍
const vi dy8 = { 0, 1, 1, 1, 0, -1, -1, -1 };
const ll INFL = (ll)2e18;	const int INF = (int)1e9;
const double EPS = 1e-10; // 許容誤差に応じて調整

// 汎用マクロの定義
#define all(a) (a).begin(), (a).end()
#define sz(x) ((int)(x).size())
#define distance (int)distance
#define Yes(b) {cout << ((b) ? "Yes" : "No") << endl;}
#define rep(i, n) for(int i = 0, i##_len = int(n); i < i##_len; ++i) // 0 から n-1 まで昇順
#define repi(i, s, t) for(int i = int(s), i##_end = int(t); i <= i##_end; ++i) // s から t まで昇順
#define repir(i, s, t) for(int i = int(s), i##_end = int(t); i >= i##_end; --i) // s から t まで降順
#define repe(v, a) for(const auto& v : (a)) // a の全要素（変更不可能）
#define repea(v, a) for(auto& v : (a)) // a の全要素（変更可能）
#define repb(set, d) for(int set = 0; set < (1 << int(d)); ++set) // d ビット全探索（昇順）
#define repp(a) sort(all(a)); for(bool a##_perm = true; a##_perm; a##_perm = next_permutation(all(a))) // a の順列全て（昇順）
#define repit(it, a) for(auto it = (a).begin(); it != (a).end(); ++it) // イテレータを回す（昇順）
#define repitr(it, a) for(auto it = (a).rbegin(); it != (a).rend(); ++it) // イテレータを回す（降順）

// 汎用関数の定義
template <class T> inline ll pow(T n, int k) { ll v = 1; rep(i, k) v *= n; return v; }
template <class T> inline bool chmax(T& M, const T& x) { if (M < x) { M = x; return true; } return false; } // 最大値を更新（更新されたら true を返す）
template <class T> inline bool chmin(T& m, const T& x) { if (m > x) { m = x; return true; } return false; } // 最小値を更新（更新されたら true を返す）

// 入出力用の >>, << のオーバーロード
template <class T, class U> inline istream& operator>> (istream& is, pair<T, U>& p) { is >> p.first >> p.second; return is; }
template <class T, class U> inline ostream& operator<< (ostream& os, const pair<T, U>& p) { os << "(" << p.first << "," << p.second << ")"; return os; }
template <class T, class U, class V> inline istream& operator>> (istream& is, tuple<T, U, V>& t) { is >> get<0>(t) >> get<1>(t) >> get<2>(t); return is; }
template <class T, class U, class V> inline ostream& operator<< (ostream& os, const tuple<T, U, V>& t) { os << "(" << get<0>(t) << "," << get<1>(t) << "," << get<2>(t) << ")"; return os; }
template <class T, class U, class V, class W> inline istream& operator>> (istream& is, tuple<T, U, V, W>& t) { is >> get<0>(t) >> get<1>(t) >> get<2>(t) >> get<3>(t); return is; }
template <class T, class U, class V, class W> inline ostream& operator<< (ostream& os, const tuple<T, U, V, W>& t) { os << "(" << get<0>(t) << "," << get<1>(t) << "," << get<2>(t) << "," << get<3>(t) << ")"; return os; }
template <class T> inline istream& operator>> (istream& is, vector<T>& v) { repea(x, v) is >> x; return is; }
template <class T> inline ostream& operator<< (ostream& os, const vector<T>& v) { repe(x, v) os << x << " "; return os; }
template <class T> inline ostream& operator<< (ostream& os, const set<T>& s) { repe(x, s) os << x << " "; return os; }
template <class T> inline ostream& operator<< (ostream& os, const unordered_set<T>& s) { repe(x, s) os << x << " "; return os; }
template <class T, class U> inline ostream& operator<< (ostream& os, const map<T, U>& m) { repe(p, m) os << p << " "; return os; }
template <class T, class U> inline ostream& operator<< (ostream& os, const unordered_map<T, U>& m) { repe(p, m) os << p << " "; return os; }
template <class T> inline ostream& operator<< (ostream& os, stack<T> s) { while (!s.empty()) { os << s.top() << " "; s.pop(); } return os; }
template <class T> inline ostream& operator<< (ostream& os, queue<T> q) { while (!q.empty()) { os << q.front() << " "; q.pop(); } return os; }
template <class T> inline ostream& operator<< (ostream& os, deque<T> q) { while (!q.empty()) { os << q.front() << " "; q.pop_front(); } return os; }
template <class T> inline ostream& operator<< (ostream& os, priority_queue<T> q) { while (!q.empty()) { os << q.top() << " "; q.pop(); } return os; }

// 手元環境（Visual Studio）
#ifdef _MSC_VER
#define popcount (int)__popcnt // 全ビット中の 1 の個数
#define popcountll (int)__popcnt64
inline int lsb(unsigned int n) { unsigned long i; _BitScanForward(&i, n); return i; } // 最下位ビットの位置（0-indexed）
inline int lsbll(unsigned long long n) { unsigned long i; _BitScanForward64(&i, n); return i; }
inline int msb(unsigned int n) { unsigned long i; _BitScanReverse(&i, n); return i; } // 最上位ビットの位置（0-indexed）
inline int msbll(unsigned long long n) { unsigned long i; _BitScanReverse64(&i, n); return i; }
template <class T> T gcd(T a, T b) { return b ? gcd(b, a % b) : a; }
#define dump(x) cout << "\033[1;36m" << (x) << "\033[0m" << endl;
#define dumps(x) cout << "\033[1;36m" << (x) << "\033[0m ";
#define dumpel(a) { int i = 0; cout << "\033[1;36m"; repe(x, a) {cout << i++ << ": " << x << endl;} cout << "\033[0m"; }
#define input_from_file(f) ifstream isTMP(f); cin.rdbuf(isTMP.rdbuf());
#define output_to_file(f) ofstream osTMP(f); cout.rdbuf(osTMP.rdbuf());
// 提出用（GCC）
#else
#define popcount (int)__builtin_popcount
#define popcountll (int)__builtin_popcountll
#define lsb __builtin_ctz
#define lsbll __builtin_ctzll
#define msb(n) (31 - __builtin_clz(n))
#define msbll(n) (63 - __builtin_clzll(n))
#define gcd __gcd
#define dump(x)
#define dumps(x)
#define dumpel(v)
#define input_from_file(f)
#define output_to_file(f)
#endif

#endif // 折りたたみ用


//-----------------AtCoder 専用-----------------
#include <atcoder/all>
using namespace atcoder;

using mint = modint1000000007;
//using mint = modint998244353;
//using mint = modint; // mint::set_mod(m);

template <class S, S(*op)(S, S), S(*e)()>ostream& operator<<(ostream& os, segtree<S, op, e> seg) { int n = seg.max_right(0, [](S x) {return true; }); rep(i, n) os << seg.get(i) << " "; return os; }
template <class S, S(*op)(S, S), S(*e)(), class F, S(*mp)(F, S), F(*cp)(F, F), F(*id)()>ostream& operator<<(ostream& os, lazy_segtree<S, op, e, F, mp, cp, id> seg) { int n = seg.max_right(0, [](S x) {return true; }); rep(i, n) os << seg.get(i) << " "; return os; }
istream& operator>> (istream& is, mint& x) { ll x_; is >> x_; x = x_; return is; }
ostream& operator<< (ostream& os, const mint& x) { os << x.val(); return os; }
using vm = vector<mint>;	using vvm = vector<vm>;		using vvvm = vector<vvm>;
//----------------------------------------------


//【幅優先探索】O(|E|)
/*
* グラフ g に対し，始点を start として幅優先探索を行い，
* start から各頂点 i への最短経路長を dist[i] に格納する．
* i が start から到達不能な頂点の場合は dist[i] = -1 となる．
*/
void breadth_first_search(const Graph& g, int start, vi& dist) {
	int n = sz(g);
	dist = vi(n, -1); // スタートからの最短距離を保持するテーブル
	dist[start] = 0;
	queue<int> que; // 次に探索する頂点を入れておくキュー
	que.push(start);

	while (!que.empty()) {
		// 未探索の頂点を 1 つ得る．
		auto s = que.front();
		que.pop();

		for (auto t : g[s]) {
			if (dist[t] != -1) {
				// 発見済みの頂点なので何もしない．
				continue;
			}

			// スタートからの最短距離を確定する．
			// 幅優先探索なので，最短だという保証がある．
			dist[t] = dist[s] + 1;

			// 未探索の頂点として t を追加する．
			que.push(t);
		}
	}
}


//【強連結成分分解】O(|V| + |E|)
/*
* 有向グラフ g を強連結成分分解し，トポロジカルソートされた結果を scc に返す．
* scc[i] は i 番目の強連結成分の頂点からなるリストである．
*/
void strongly_connected_component_decomposition(Graph& g, vvi& scc) {
	// 参考 : https://hkawabata.github.io/technical-note/note/Algorithm/graph/scc.html

	int n = sz(g);

	// 辺の向きを逆にしたグラフを作成
	Graph g_rev(n);
	rep(s, n) {
		repe(t, g[s]) {
			g_rev[t].push_back(s);
		}
	}

	// 各頂点の状態（0:未探索，1:順探索済かつ未逆探索，2:逆探索済）
	vi status(n, 0);


	// step1: まず順探索（深さ優先）を行い，結果をスタックに格納する．

	// 深さ優先の順探索で見つかった順に頂点を記録するスタック
	stack<int> stk;

	// 順探索用の再帰関数
	function<void(int)> trace = [&](int s) {
		// 状態を順探索済かつ未逆探索（1）にする．
		status[s] = 1;

		repe(t, g[s]) {
			// 未探索の頂点を探索しにいく．
			if (status[t] == 0) {
				trace(t);
			}
		}

		// 先の探索が済んだら自身を記録する（深さ優先探索）
		stk.push(s);
	};

	rep(i, n) {
		// 未探索の頂点を見つけたら探索する．
		if (status[i] == 0) {
			trace(i);
		}
	}


	// step2: 次に逆探索を行い，強連結成分を確定する．

	// 逆探索用の再帰関数
	function<void(int)> trace_rev = [&](int s) {
		// 状態を逆探索済（2）にする．
		status[s] = 2;

		repe(t, g_rev[s]) {
			// 未逆探索の頂点を探索しにいく．
			if (status[t] == 1) {
				trace_rev(t);
			}
		}

		// 先の探索が済んだら自身を強連結成分の一員として記録する．
		scc.rbegin()->push_back(s);
	};

	while (!stk.empty()) {
		auto v = stk.top();
		stk.pop();

		// 新しい強連結成分を見つけたらそれをなぞりに行く．
		if (status[v] == 1) {
			scc.push_back(vi());
			trace_rev(v);
		}
	}
}


//【頂点の縮約】O(|V| + |E| log |V|)
/*
* グラフ g とその頂点の分割 p について，成分 p[i] を 1 つの頂点 i として
* 縮約したグラフを gc に格納する．
*/
void vertex_contraction(const Graph& g, const vvi& p, Graph& gc) {
	int n = sz(g);
	int m = sz(p);

	// id[v] : 頂点 v の属する成分
	vi id(n);
	rep(i, m) {
		repe(v, p[i]) {
			id[v] = i;
		}
	}

	// 多重辺や自己ループを防ぐため一旦辺の集合を set でもつ．
	vector<set<int>> gc_set(m);
	rep(s, n) {
		repe(t, g[s]) {
			gc_set[id[s]].insert(id[t]);
		}
		gc_set[id[s]].erase(id[s]);
	}

	// 結果の格納
	gc = Graph(m);
	rep(s, m) {
		repe(t, gc_set[s]) {
			gc[s].push_back(t);
		}
	}
}


//【コスト最大パス（頂点コスト）】O(|V| + |E|)
/*
* 頂点コスト w の与えられた有向非巡回グラフ g のパス（長さ 0 も可）で，
* パスに属する頂点のコストの和の最大値を返す．
*
*（DAG 上の DP）
*/
ll highest_cost_path(const Graph& g, const vl& w) {
	int n = sz(g);

	// dp[s] : 頂点 s からの最大コスト
	vl dp(n);
	vb seen(n);

	function<ll(int)> dfs = [&](int s) {
		// s の情報を計算済だったらすぐに返す．
		if (seen[s]) return dp[s];
		seen[s] = true;

		// s から行ける頂点 t の情報を元に s の情報を計算する．
		dp[s] = 0;
		repe(t, g[s]) {
			chmax(dp[s], dfs(t));
		}
		dp[s] += w[s];

		return dp[s];
	};

	// 各頂点 s についての情報を計算する．
	ll res = 0;
	rep(s, n) {
		chmax(res, dfs(s));
	}

	return res;
}


//【コスト最大パス（頂点コスト）】O(|V| + |E|)
/*
* 頂点コスト w の与えられた有向非巡回グラフ g の r からのパス（長さ 0 も可）で，
* パスに属する頂点のコストの和を最大とするパスの頂点列を path に格納する．
* またそのパスのコストを返す．
*
*（DAG 上の DP）
*/
ll highest_cost_path(const Graph& g, const vl& w, int r, vi* path = nullptr) {
	int n = sz(g);

	// dp[s] : 頂点 s からの最大コスト
	vl dp(n);
	vb seen(n);
	vi next(n, -1);

	function<ll(int)> dfs = [&](int s) {
		// s の情報を計算済だったらすぐに返す．
		if (seen[s]) return dp[s];
		seen[s] = true;

		// s から行ける頂点 t の情報を元に s の情報を計算する．
		dp[s] = 0;
		repe(t, g[s]) {
			if (chmax(dp[s], dfs(t))) {
				next[s] = t;
			}
		}
		dp[s] += w[s];

		return dp[s];
	};

	// r から探索
	ll res = dfs(r);

	// DP 復元
	if (path != nullptr) {
		path->clear();

		for (int s = r; s != -1; s = next[s]) {
			path->push_back(s);
		}
	}

	return res;
}


//【コスト最大パスの組（頂点コスト）】O(|V| + |E|)
/*
* 頂点コスト w の与えられた有向非巡回グラフ g のパスの組で，
* いずれかのパスに属している頂点のコストの和の最大値を返す．
*
*（グラフ上の DP）
* 
* 利用：【コスト最大パス（頂点コスト）】
*/
ll highest_cost_twinpath(const Graph& g, const vl& w) {
	int n = sz(g);

	ll res = 0;

	rep(s, n) {
		vi path;
		ll sum = highest_cost_path(g, w, s, &path);
		dump("------------------------");
		dump(s);
		dump(path);
		dump(sum);
		
		vl nw = w;
		repe(v, path) {
			nw[v] = 0;
		}

		sum += highest_cost_path(g, nw);
		dump(sum);

		chmax(res, sum);
	}

	return res;
}


int main() {
	cout << fixed << setprecision(12);
	input_from_file("input.txt");

	int n;
	cin >> n;

	Graph g(n);
	rep(s, n) {
		rep(t, n) {
			int e;
			cin >> e;

			if (e) g[s].push_back(t);
//			if (e) g[t].push_back(s); // 逆グラフ入力用
		}
	}
	dumpel(g);

	////テストケース作成用
	//int n = 10;
	//Graph g(n);
	//srand(time(0));
	//rep(s, n) {
	//	rep(t, n) {
	//		int e = !(rand() % 13);
	//		cout << e << " ";
	//		if (e) g[s].push_back(t);
	//	}
	//	cout << endl;
	//}

	vvi scc;
	strongly_connected_component_decomposition(g, scc);
	dumpel(scc);

	Graph gc;
	vertex_contraction(g, scc, gc);
	dumpel(gc);

	int m = sz(gc);
	vl w(m);
	rep(i, m) w[i] = sz(scc[i]);
	dump(w);

	ll res = highest_cost_twinpath(gc, w);

	//Graph gc_rev(m);
	//rep(s, m) {
	//	repe(t, gc[s]) {
	//		gc_rev[t].push_back(s);
	//	}
	//}
	//ll res2 = highest_cost_twinpath(gc_rev, w);
	//dumps(res); dump(res2);

	cout << res << endl;
}
