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
// 提出用（gcc）
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

//using mint = modint1000000007;
using mint = modint998244353;
//using mint = modint; // mint::set_mod(m);

template <class S, S(*op)(S, S), S(*e)()>ostream& operator<<(ostream& os, segtree<S, op, e> seg) { int n = seg.max_right(0, [](S x) {return true; }); rep(i, n) os << seg.get(i) << " "; return os; }
template <class S, S(*op)(S, S), S(*e)(), class F, S(*mp)(F, S), F(*cp)(F, F), F(*id)()>ostream& operator<<(ostream& os, lazy_segtree<S, op, e, F, mp, cp, id> seg) { int n = seg.max_right(0, [](S x) {return true; }); rep(i, n) os << seg.get(i) << " "; return os; }
istream& operator>> (istream& is, mint& x) { ll x_; is >> x_; x = x_; return is; }
ostream& operator<< (ostream& os, const mint& x) { os << x.val(); return os; }
using vm = vector<mint>;	using vvm = vector<vm>;		using vvvm = vector<vvm>;
//----------------------------------------------


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
	operator int() const { return to; }
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

	// デバッグ出力
	friend ostream& operator<<(ostream& os, const WTNode& v) {
		os << "(par:" << v.parent << ", cld:" << v.child << ", dep:" << v.depth
			<< ", dist:" << v.dist << ", wgt:" << v.weight << ")";
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
	WTNode const& operator[](int i) const { return v[i]; }
	WTNode& operator[](int i) { return v[i]; }

	// 大きさ
	int size() const { return n; }

	// デバッグ出力
	friend ostream& operator<<(ostream& os, const WRTree& rt) {
		rep(i, rt.n) os << rt[i] << endl;
		return os;
	}
};


//【根付き木の HL 分解】O(|V|)
/*
* 根付き木 rt の HL 分解を行う．
*
* in[s] : 最重頂点優先で頂点 s を何番目になぞるか（根なら 0）
* out[s] : 最重頂点優先で頂点 s から出て次になぞる頂点が何番目か（根なら |V|）
* pos[i] : 最重頂点優先で i 番目になぞる頂点（長さ |V|）
* top[s] : 頂点 s を含む連結成分の最も浅い頂点
*/
template <class TREE>
void heavy_light_decomposition(TREE& rt, vi& in, vi& out, vi& pos, vi& top) {
	// 参考：https://qiita.com/Pro_ktmr/items/4e1e051ea0561772afa3

	int n = sz(rt);

	int time = 0;
	in = vi(n);
	out = vi(n);
	pos = vi(n);
	top = vi(n);

	// 再帰用の関数
	// s : 注目している頂点
	// p : s を含む連結成分の最も浅い頂点
	function<void(int, int)> rf = [&](int s, int p) {
		in[s] = time;
		pos[time++] = s;
		top[s] = p;

		// 重さ最大の頂点を得る．
		int w_max = -INF, v_max = -1;
		repe(t, rt[s].child) {
			if (chmax(w_max, rt.v[t].weight)) {
				v_max = t;
			}
		}

		// 重さ最大の頂点を優先的になぞる．
		if (v_max != -1) {
			rf(v_max, p);
		}

		// 残りの頂点をなぞる．
		repe(t, rt[s].child) {
			if (t == v_max) continue;

			rf(t, t);
		}

		// s から最後に離れる
		out[s] = time;
	};

	// 根から順に探索する．
	rf(rt.r, rt.r);
}


//【遅延評価フェニック木】
/*
* Lazy_fenwick_tree(n) : O(n)
*	要素数 n かつ初期値 e で初期化する．
*	要素は Z 加群 (S, op, e, inv) の元とする．
*	x ∈ S を k 個 op() したものを mul(x, k) とする．
*
* Lazy_fenwick_tree(a) : O(n)
*	配列 a で初期化する．
*
* set(i, x) : O(log n) // 遅いので apply が使えるならそちらを使うべき
*	v[i] = x とする．
*
* get(i) : O(log n)
*	v[i] を返す．
*
* prod(l, r) : O(log n)
*	op( v[l..r) ) を返す．空なら e() を返す．
*
* apply(i, x) : O(log n)
*	v[i] = op(v[i], x) とする．
*
* apply(l, r, x) : O(log n)
*	v[l..r) = op(v[l..r), x) とする．
*/
template <class S, S(*op)(S, S), S(*e)(), S(*inv)(S), S(*mul)(S, int)>
struct Lazy_fenwick_tree {
	// 参考：https://algo-logic.info/binary-indexed-tree/

	// ノードの個数（要素数 + 1）
	int n;

	// op( [1..i] ) を acc0[i] op mul(acc1[i], i) と分解する．
	// さらに accd[i] = op( rawd[1..i] ) と表されるような rawd を導入する．
	// v[d] : op( rawd[*..i] ) の値（i ： 1-indexed，v[d][0] は使わない）
	vector<vector<S>> v;

	// コンストラクタ（初期化なし）
	Lazy_fenwick_tree() : n(0) {}

	// 要素数 n かつ初期値 e で初期化
	Lazy_fenwick_tree(int n_) : n(n_ + 1), v(2, vector<S>(n, e())) {}

	// 配列 a で初期化
	Lazy_fenwick_tree(const vector<S>& v_) : n(sz(v_) + 1), v(2, vector<S>(n, e())) {
		// 配列の値を仮登録する．
		rep(i, n - 1) v[0][i + 1LL] = v_[i];
		
		// 正しい値になるよう根に向かって累積 op() をとっていく．
		for (int pow2 = 1; 2 * pow2 < n; pow2 *= 2) {
			for (int i = 2 * pow2; i < n; i += 2 * pow2) {
				v[0][i] = op(v[0][i], v[0][(ll)i - pow2]);
			}
		}
	}

	// v[i] = x とする．（i : 0-indexed）
	void set(int i, S x) {
		// 差分を求める．
		S d = op(x, inv(get(i)));

		apply(i, d);
	}

	// v[i] を返す．（i : 0-indexed）
	S get(int i) const {
		return prod(i, i + 1);
	}

	// op( v[l..r) ) を返す．空なら e を返す．（l, r : 0-indexed）
	S prod(int l, int r) const {
		// 0-indexed での半開区間 [l, r) は，
		// 1-indexed での閉区間 [l + 1, r] に対応する．
		// よって閉区間 [1, r] の総和から閉区間 [1, l] の総和を引けば良い．
		return prod_sub(r) - prod_sub(l);
	}

	// op( v[1..r] ) を返す．空なら e を返す．（r : 1-indexed）
	S prod_sub(int r) const {
		return prod_sub(r, 0) + mul(prod_sub(r, 1), r);
	}

	// op( v[d][1..r] ) を返す．空なら e を返す．（r : 1-indexed）
	S prod_sub(int r, int d) const {
		S res = e();

		// 子に向かって累積 op() をとっていく．
		while (r > 0) {
			res = op(res, v[d][r]);

			// r の最下位ビットから 1 を減算することで次の位置を得る．
			r -= r & -r;
		}
		return res;
	}

	// v[i] = op(v[i], x) とする．（i : 0-indexed）
	void apply(int i, S x) {
		// i を 1-indexed に直す．
		i++;

		apply_sub(i, x, 0);
	}

	// v[l..r) = op(v[l..r), x) とする．（l, r : 0-indexed） 
	void apply(int l, int r, S x) {
		// 0-indexed での半開区間 [l, r) は，
		// 1-indexed での閉区間 [l + 1, r] に対応する．
		l++;

		// 区間の端の値を調整する．
		apply_sub(l, mul(inv(x), l - 1), 0);
		apply_sub(r + 1, mul(x, r), 0);
		apply_sub(l, x, 1);
		apply_sub(r + 1, inv(x), 1);
	}

	// v[d][i] = op(v[d][i], x) とする．（i : 1-indexed）
	void apply_sub(int i, S x, int d) {
		// 根に向かって値を op() していく．
		while (i < n) {
			v[d][i] = op(v[d][i], x);

			// i の最下位ビットに 1 を加算することで次の位置を得る．
			i += i & -i;
		}
	}

	// デバッグ出力
	friend ostream& operator<<(ostream& os, const Lazy_fenwick_tree& ft) {
		rep(i, ft.n - 1) {
			os << ft.get(i) << " ";
		}
		return os;
	}
};


//【区間加算／区間総和クエリ】
/*
* 利用：【遅延評価フェニック木】
*/
template <class T> T op8(T x, T y) { return x + y; }
template <class T> T e8() { return T(0); }
template <class T> T inv8(T x) { return -x; }
template <class T> T mul8(T f, int i) { return f * i; }
template <class T> using RASQ = Lazy_fenwick_tree<T, op8<T>, e8<T>, inv8<T>, mul8<T>>;


//【木の辺への加算／木の辺の総和クエリ】
/*
* Tree_edge_add_sum_query(rt) : O(|V|)
*	コスト付き根付き木 rt で初期化する．
*
* set(v, val) : O(log |V|)
*	頂点 v への v の親からの辺の値を val にする．
* 
* get(v) : O(log |V|)
*	頂点 v への v の親からの辺の値を返す．
*
* add(v, val) : O(log |V|)
*	頂点 v の部分木の辺に val を加算する．
*
* add(v1, v2, val) : O((log |V|)^2)
*	頂点 v1 から v2 までの辺に val を加算する．
*
* sum(v) : O(log |V|)
*	頂点 v の部分木の辺の値の和を返す．
*
* sum(v1, v2) : O((log |V|)^2)
*	頂点 v1 から v2 までの辺の値の和を返す．
*
* 利用：
*	【根付き木の HL 分解／オイラーツアー】
*	【区間加算／区間総和クエリ】
*/
struct Tree_edge_add_sum_query {
	// 参考：https://qiita.com/Pro_ktmr/items/4e1e051ea0561772afa3

	// 根付き木
	WRTree rt;

	// HL 分解の結果の記録用
	// in[s] : 最重頂点優先で頂点 s を何番目になぞるか（根なら 0）
	// out[s] : 最重頂点優先で頂点 s から出て次になぞる頂点が何番目か（根なら |V|）
	// pos[i] : 最重頂点優先で i 番目になぞる頂点（長さ |V|）
	// top[s] : 頂点 s を含む連結成分の最も浅い頂点
	vi in, out, pos, top;

	// 列 pos に対する区間加算／区間総和クエリを処理する．
	// rasq[i] : i 番目になぞる頂点に入る辺の値（rasq[0] は使わない）
	RASQ<ll> rasq;

	// コンストラクタ（根付き木で初期化）
	Tree_edge_add_sum_query(WRTree& rt_) : rt(rt_) {
		// rt を HL 分解する．
		heavy_light_decomposition(rt, in, out, pos, top);

		vl val(rt.n);
		rep(s, rt.n) {
			repe(e, rt[s].child) {
				val[in[e.to]] += e.cost;
			}
		}

		rasq = RASQ<ll>(val);
	}

	// 頂点 v への v の親からの辺の値を val にする．
	void set(int v, ll val) { rasq.set(in[v], val); }

	// 頂点 v への v の親からの辺の値を返す．
	ll get(int v) { return rasq.get(in[v]); }

	// 頂点 v の部分木の辺に val を加算する．
	void add(int v, ll val) { rasq.apply(in[v] + 1, out[v], val); }

	// 頂点 v1 から v2 までの辺に val を加算する．
	void add(int v1, int v2, ll val) {
		// v1 と v2 が異なる連結成分に属している限りループを回す．
		while (top[v1] != top[v2]) {
			// v1 の方が浅い連結成分に属しているとする．
			if (in[top[v1]] > in[top[v2]]) {
				swap(v1, v2);
			}

			// v2 を含む連結成分は pos で並んで配置されているので，
			// 最も浅い頂点 top[v2] から v2 までの範囲に val を加算する．
			rasq.apply(in[top[v2]], in[v2] + 1, val);

			// 一つ浅い連結成分に移動する．
			v2 = rt[top[v2]].parent;
		}

		// ここまできたら v1 と v2 は同じ連結成分に属するので，
		// その間の辺のみに対して val を加算する．
		if (in[v1] > in[v2]) {
			swap(v1, v2);
		}
		rasq.apply(in[v1] + 1, in[v2] + 1, val);
	}

	// 頂点 v の部分木の辺の値の和を返す．
	ll sum(int v) { return rasq.prod(in[v] + 1, out[v]); }

	// 頂点 v1 から v2 までの辺の値の和を返す．
	ll sum(int v1, int v2) {
		ll res = 0;

		// v1 と v2 が異なる連結成分に属している限りループを回す．
		while (top[v1] != top[v2]) {
			// v1 の方が浅い連結成分に属しているとする．
			if (in[top[v1]] > in[top[v2]]) {
				swap(v1, v2);
			}

			// v2 を含む連結成分は pos で並んで配置されているので，
			// 最も浅い頂点 top[v2] から v2 までの範囲の和を求める．
			res += rasq.prod(in[top[v2]], in[v2] + 1);

			// 一つ浅い連結成分に移動する．
			v2 = rt[top[v2]].parent;
		}

		// ここまできたら v1 と v2 は同じ連結成分に属するので，
		// その間の辺のみの和を res に加算する．
		if (in[v1] > in[v2]) {
			swap(v1, v2);
		}
		res += rasq.prod(in[v1] + 1, in[v2] + 1);

		return res;
	}

	// デバッグ出力
	friend ostream& operator<<(ostream& os, Tree_edge_add_sum_query q) {
		os << q.rt << q.in << endl << q.out << endl << q.pos << endl
			<< q.top << endl << q.rasq << endl;
		return os;
	}
};


int main() {
	cout << fixed << setprecision(15);
	//	input_from_file("input.txt");
	//	output_to_file("output.txt");

	int n;
	cin >> n;

	WGraph g(n);
	read_graph(n, n - 1, g, false, false);

	WRTree rt(g, 0);

	Tree_edge_add_sum_query tq(rt);

	int q;
	cin >> q;

	rep(i, q) {
		int type;
		cin >> type;

		if (type == 1) {
			int a; ll x;
			cin >> a >> x;

			tq.add(a, x);
		}
		else {
			int b;
			cin >> b;

			cout << tq.sum(0, b) << endl;
		}
	}

	dump(tq);
	dump(tq.sum(3, 2));
	dump(tq.sum(5));
	dump(tq.get(6));
	dump(tq.sum(1));
}
