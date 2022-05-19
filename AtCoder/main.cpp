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
const vi dx4 = { 1, 0, -1, 0 }; // 4 近傍（下，右，上，左）
const vi dy4 = { 0, 1, 0, -1 };
const vi dx8 = { 1, 1, 0, -1, -1, -1, 0, 1 }; // 8 近傍
const vi dy8 = { 0, 1, 1, 1, 0, -1, -1, -1 };
const int INF = 1001001001; const ll INFL = 4004004004004004004LL;
const double EPS = 1e-12; // 許容誤差に応じて調整

// 入出力高速化
struct fast_io { fast_io() { cin.tie(nullptr); ios::sync_with_stdio(false); cout << fixed << setprecision(15); } } fastIOtmp;

// 汎用マクロの定義
#define all(a) (a).begin(), (a).end()
#define sz(x) ((int)(x).size())
#define distance (int)distance
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
#define popcount (int)__builtin_popcount
#define popcountll (int)__builtin_popcountll
#define lsb __builtin_ctz
#define lsbll __builtin_ctzll
#define msb(n) (31 - __builtin_clz(n))
#define msbll(n) (63 - __builtin_clzll(n))
#define gcd __gcd
#define dump(...)
#define dumpel(v)
#define input_from_file(f)
#define output_to_file(f)
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
* 始点 終点の組からなる入力を受け取り，n 頂点 m 辺のグラフを構成する．
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


int main() {
//	input_from_file("input.txt");
//	output_to_file("output.txt");

	int n, m;
	cin >> n >> m;

	Graph g;
	read_graph(n, m, g);


}
