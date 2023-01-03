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
double EPS = 1e-12;

// 入出力高速化
struct fast_io { fast_io() { cin.tie(nullptr); ios::sync_with_stdio(false); cout << fixed << setprecision(18); } } fastIOtmp;

// 汎用マクロの定義
#define all(a) (a).begin(), (a).end()
#define sz(x) ((int)(x).size())
#define lbpos(a, x) (int)distance((a).begin(), std::lower_bound(all(a), x))
#define ubpos(a, x) (int)distance((a).begin(), std::upper_bound(all(a), x))
#define Yes(b) {cout << ((b) ? "Yes\n" : "No\n");}
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
#define dump_list(v)
#define dump_list2D(v)
#define input_from_file(f)
#define output_to_file(f)
#define Assert(b) { if (!(b)) while (1) cout << "OLE"; }
#endif

#endif // 折りたたみ用


////--------------AtCoder 専用--------------
//#include <atcoder/all>
//using namespace atcoder;
//
////using mint = modint1000000007;
//using mint = modint998244353;
////using mint = modint; // mint::set_mod(m);
//
//istream& operator>>(istream& is, mint& x) { ll x_; is >> x_; x = x_; return is; }
//ostream& operator<<(ostream& os, const mint& x) { os << x.val(); return os; }
//using vm = vector<mint>; using vvm = vector<vm>; using vvvm = vector<vvm>;
////----------------------------------------


//【木の入力】O(n)
/*
* 親を並べた入力を受け取り，n 頂点の木を構成して返す．
*
* n : グラフの頂点の数
* undirected : 無向グラフなら true
* one_indexed : 入力が 1-indexed で与えられるなら true
*/
Graph read_tree(int n, bool undirected = true, bool one_indexed = true) {
	// verify : https://atcoder.jp/contests/arc028/tasks/arc028_3

	Graph g(n);
	repi(i, 1, n - 1) {
		int p;
		cin >> p;

		if (one_indexed) p--;

		g[p].push_back(i);
		if (undirected) g[i].push_back(p);
	}

	return g;
}


//【配る木 DP】O(n)
/*
* 各 s∈[0..n) について，r を根とする根付き木 g の
* 根からのパス r→s についての問題の答えを格納したリストを返す．
*
* T apply(T x, int s, int t) :
*   根からのパス r→s についての答えが x のとき，
*   辺 s→t を追加した根からのパス r→t についての答えを返す．
*
* T root(int r) :
*   根からのパス r→r に対する問題の答えを返す．
*/
template <class T, T(*apply)(const T&, int, int), T(*root)(int)>
vector<T> tree_giveDP(const Graph& g, int r) {
	int n = sz(g);

	vector<T> dp(n);
	dp[r] = root(r);

	// s の各子 t について，根からのパス r→t についての答えを計算する．（p : s の親）
	function<void(int, int)> dfs = [&](int s, int p) {
		repe(t, g[s]) {
			if (t == p) continue;

			// 根からのパス r→t についての答えを計算する．
			dp[t] = apply(dp[s], s, t);

			// t から先の答えを計算する．
			dfs(t, s);
		}
	};
	dfs(r, -1);

	return dp;

	/* 雛形
	using T = int;
	T apply(const T& x, int s, int t) { return x + 1; }
	T root(int r) { return 0; }
	vector<T> solve_by_tree_giveDP(const Graph& g, int r) {
		return tree_giveDP<T, apply, root>(g, r);
	}
	*/
};


//【木の深さ】O(n)
/*
* 各 s∈[0..n) について，r を根とする木 g の頂点 s の深さを格納したリストを返す．
* s の深さとは，根から s までの辺の本数のことである．
*
* 利用：【配る木 DP】
*/
// verify : https://algo-method.com/tasks/529
using T = int;
T apply(const T& x, int s, int t) { return x + 1; }
T root(int r) { return 0; }
vector<T> solve_by_tree_giveDP(const Graph& g, int r) {
	return tree_giveDP<T, apply, root>(g, r);
}


int main() {
//	input_from_file("input.txt");
//	output_to_file("output.txt");

	int n;
	cin >> n;

	Graph g = read_tree(n, true, false);
	
	auto depth = solve_by_tree_giveDP(g, 0);

	rep(i, n) cout << depth[i] << endl;
}
