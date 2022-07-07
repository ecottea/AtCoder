#ifndef HIDDEN_IN_VS // 折りたたみ用

// 警告の抑制
#define _CRT_SECURE_NO_WARNINGS

// ライブラリの読み込み
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
const double PI = acos(-1);
const vi DX = { 1, 0, -1, 0 }; // 4 近傍（下，右，上，左）
const vi DY = { 0, 1, 0, -1 };
int INF = 1001001001; ll INFL = 4004004004004004004LL;
double EPS = 1e-12; // 許容誤差に応じて調整

// 入出力高速化
struct fast_io { fast_io() { cin.tie(nullptr); ios::sync_with_stdio(false); cout << fixed << setprecision(18); } } fastIOtmp;

// 汎用マクロの定義
#define all(a) (a).begin(), (a).end()
#define sz(x) ((int)(x).size())
#define lbpos(a, x) (int)distance((a).begin(), std::lower_bound(all(a), x))
#define ubpos(a, x) (int)distance((a).begin(), std::upper_bound(all(a), x))
#define Yes(b) {cout << ((b) ? "Yes\n" : "No\n");}
#define YES(b) {cout << ((b) ? "YES\n" : "NO\n");}
#define rep(i, n) for(int i = 0, i##_len = int(n); i < i##_len; ++i) // 0 から n-1 まで昇順
#define repi(i, s, t) for(int i = int(s), i##_end = int(t); i <= i##_end; ++i) // s から t まで昇順
#define repir(i, s, t) for(int i = int(s), i##_end = int(t); i >= i##_end; --i) // s から t まで降順
#define repe(v, a) for(const auto& v : (a)) // a の全要素（変更不可能）
#define repea(v, a) for(auto& v : (a)) // a の全要素（変更可能）
#define repb(set, d) for(int set = 0; set < (1 << int(d)); ++set) // d ビット全探索（昇順）
#define repp(a) sort(all(a)); for(bool a##_perm = true; a##_perm; a##_perm = next_permutation(all(a))) // a の順列全て（昇順）
#define smod(n, m) ((((n) % (m)) + (m)) % (m)) // 非負mod
#define uniq(a) {sort(all(a)); (a).erase(unique(all(a)), (a).end());} // 重複除去
#define EXIT(a) {cout << (a) << endl; exit(0);} // 強制終了

// 汎用関数の定義
template <class T> inline ll pow(T n, int k) { ll v = 1; rep(i, k) v *= n; return v; }
template <class T> inline bool chmax(T& M, const T& x) { if (M < x) { M = x; return true; } return false; } // 最大値を更新（更新されたら true を返す）
template <class T> inline bool chmin(T& m, const T& x) { if (m > x) { m = x; return true; } return false; } // 最小値を更新（更新されたら true を返す）

// 演算子オーバーロード
template <class T, class U> inline istream& operator>>(istream& is, pair<T, U>& p) { is >> p.first >> p.second; return is; }
template <class T> inline istream& operator>>(istream& is, vector<T>& v) { repea(x, v) is >> x; return is; }
template <class T> inline vector<T>& operator--(vector<T>& v) { repea(x, v) --x; return v; }
template <class T> inline vector<T>& operator++(vector<T>& v) { repea(x, v) ++x; return v; }

// 手元環境（Visual Studio）
#ifdef _MSC_VER
#include "local.hpp"
// 提出用（gcc）
#else
inline int popcount(int n) { return __builtin_popcount(n); }
inline int popcount(ll n) { return __builtin_popcountll(n); }
inline int lsb(int n) { return n != 0 ? __builtin_ctz(n) : -1; }
inline int lsb(ll n) { return n != 0 ? __builtin_ctzll(n) : -1; }
inline int msb(int n) { return n != 0 ? (31 - __builtin_clz(n)) : -1; }
inline int msb(ll n) { return n != 0 ? (63 - __builtin_clzll(n)) : -1; }
#define gcd __gcd
#define dump(...)
#define dumpel(v)
#define input_from_file(f)
#define output_to_file(f)
#define Assert(b) { if (!(b)) while (1) cout << "OLE"; }
#endif

#endif // 折りたたみ用


//--------------AtCoder 専用--------------
#include <atcoder/all>
using namespace atcoder;

using mint = modint1000000007;
//using mint = modint998244353;
//using mint = modint; // mint::set_mod(m);

istream& operator>>(istream& is, mint& x) { ll x_; is >> x_; x = x_; return is; }
ostream& operator<<(ostream& os, const mint& x) { os << x.val(); return os; }
using vm = vector<mint>; using vvm = vector<vm>; using vvvm = vector<vvm>;
//----------------------------------------


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
void read_graph(int n, int m, Graph& g, bool undirected = true, bool one_indexed = true) {
	g = Graph(n);
	rep(i, m) {
		int a, b;
		cin >> a >> b;

		if (one_indexed) { a--; b--; }

		g[a].push_back(b);
		if (undirected) g[b].push_back(a);
	}
}


//【隣接作用／一点参照クエリ（可換モノイド）】
/*
* Neighbor_add_query<S, op, o>(Graph g) : O(|V| + |E|)
*	グラフ g と初期値 o() で初期化する．
*	要素は可換モノイド (S, op, o) の元とする．
*
* Neighbor_add_query<S, op, o>(Graph g, vS v) : O(|V| + |E|)
*	グラフ g と初期値 v[0..n) で初期化する．
*
* add(int i, S x) : O(1)
*	v[i] += x とする．
* 
* S get(int i) : O(√|E|)
*	v[i] を返す．
*
* spread(int s, S x) : O(√|E|)
*	s の隣接頂点 t 全てに対して v[t] += x とする．
*/
template <class S, S(*op)(S, S), S(*o)()>
class Neighbor_add_query {
	using vS = vector<S>;

	int n; // 頂点数
	Graph g; // グラフ

	int th; // 頂点の次数が大きいかどうかの閾値
	Graph gl; // 隣接する次数が大きい頂点だけを記録したグラフ

	vS v; // 各頂点の値
	vS lazy; // 各頂点から撒く値

public:
	// グラフ g と単位元 o() で初期化
	Neighbor_add_query(const Graph& g_) : n(sz(g_)), g(g_), gl(n), v(n, o()), lazy(n, o()) {
		// m : g の辺の数
		int m = 0;
		rep(s, n) m += sz(g[s]);

		// 頂点の次数が大きいか小さいかの閾値 th を決定
		th = (int)(sqrt(m) + 0.001);

		// 隣接する大きい頂点だけを記録したグラフ gl を作成
		rep(s, n) repe(t, g[s]) if (sz(g[t]) > th) gl[s].push_back(t);
	}

	// グラフ g と配列 a[0..n) で初期化
	Neighbor_add_query(const Graph& g, const vector<S>& v_) : Neighbor_add_query(g) { v = v_; }
	Neighbor_add_query() {}

	// v[i] += x とする．
	void add(int i, S x) {
		v[i] = op(v[i], x);
	}

	// v[i] を返す．
	S get(int i) {
		// i の次数が大きい場合は v[i] に正しい値が入っている．
		if (sz(g[i]) > th) return v[i];

		// i の次数が小さい場合は周りから撒かれた値の和を計算する．
		S res = o();
		repe(j, g[i]) res = op(lazy[j], res);

		return op(v[i], res);
	}

	// s の隣接頂点 t 全てに対して v[t] += x とする．
	void spread(int s, S x) {
		// 隣接する次数が大きい頂点の値の更新
		repe(t, gl[s]) v[t] = op(x, v[t]);

		// 隣接頂点に数を撒いたことを記録
		lazy[s] = op(x, lazy[s]);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Neighbor_add_query npq) {
		rep(i, npq.n) os << npq.get(i) << " ";
		return os;
	}
#endif
};


//【max モノイド】
using S003 = pii;
S003 op003(S003 a, S003 b) { return max(a, b); }
S003 e003() { return { -INF, -INF }; }
#define Max_monoid S003, op003, e003


int main() {
//	input_from_file("input.txt");
//	output_to_file("output.txt");

	int n, m, q;
	cin >> n >> m >> q;

	Graph g;
	read_graph(n, m, g);

	vector<S003> ini(n);
	rep(i, n) ini[i] = { -1, i + 1 };
	Neighbor_add_query<Max_monoid> npq(g, ini);

	rep(t, q) {
		int x;
		cin >> x;
		x--;

		int v = npq.get(x).second;
		npq.spread(x, { t, v });
	}

	rep(i, n) cout << npq.get(i).second << " ";
}
