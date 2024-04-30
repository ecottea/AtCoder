#ifndef HIDDEN_IN_VS // 折りたたみ用

// 警告の抑制
#define _CRT_SECURE_NO_WARNINGS

// ライブラリの読み込み
#include <bits/stdc++.h>
using namespace std;

// 型名の短縮
using ll = long long; using ull = unsigned long long; // -2^63 ～ 2^63 = 9 * 10^18（int は -2^31 ～ 2^31 = 2 * 10^9）
using pii = pair<int, int>;	using pll = pair<ll, ll>;	using pil = pair<int, ll>;	using pli = pair<ll, int>;
using vi = vector<int>;		using vvi = vector<vi>;		using vvvi = vector<vvi>;	using vvvvi = vector<vvvi>;
using vl = vector<ll>;		using vvl = vector<vl>;		using vvvl = vector<vvl>;	using vvvvl = vector<vvvl>;
using vb = vector<bool>;	using vvb = vector<vb>;		using vvvb = vector<vvb>;
using vc = vector<char>;	using vvc = vector<vc>;		using vvvc = vector<vvc>;
using vd = vector<double>;	using vvd = vector<vd>;		using vvvd = vector<vvd>;
template <class T> using priority_queue_rev = priority_queue<T, vector<T>, greater<T>>;
using Graph = vvi;

// 定数の定義
const double PI = acos(-1);
int DX[4] = {1, 0, -1, 0}; // 4 近傍（下，右，上，左）
int DY[4] = {0, 1, 0, -1};
int INF = 1001001001; ll INFL = 4004004003094073385LL; // (int)INFL = INF, (int)(-INFL) = -INF;

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
#define repb(set, d) for(int set = 0, set##_ub = 1 << int(d); set < set##_ub; ++set) // d ビット全探索（昇順）
#define repis(i, set) for(int i = lsb(set), bset##i = set; i >= 0; bset##i -= 1 << i, i = lsb(bset##i)) // set の全要素（昇順）
#define repp(a) sort(all(a)); for(bool a##_perm = true; a##_perm; a##_perm = next_permutation(all(a))) // a の順列全て（昇順）
#define uniq(a) {sort(all(a)); (a).erase(unique(all(a)), (a).end());} // 重複除去
#define EXIT(a) {cout << (a) << endl; exit(0);} // 強制終了
#define inQ(x, y, u, l, d, r) ((u) <= (x) && (l) <= (y) && (x) < (d) && (y) < (r)) // 半開矩形内判定

// 汎用関数の定義
template <class T> inline ll powi(T n, int k) { ll v = 1; rep(i, k) v *= n; return v; }
template <class T> inline bool chmax(T& M, const T& x) { if (M < x) { M = x; return true; } return false; } // 最大値を更新（更新されたら true を返す）
template <class T> inline bool chmin(T& m, const T& x) { if (m > x) { m = x; return true; } return false; } // 最小値を更新（更新されたら true を返す）
template <class T> inline T getb(T set, int i) { return (set >> i) & T(1); }
template <class T> inline T smod(T n, T m) { n %= m; if (n < 0) n += m; return n; } // 非負mod

// 演算子オーバーロード
template <class T, class U> inline istream& operator>>(istream& is, pair<T, U>& p) { is >> p.first >> p.second; return is; }
template <class T> inline istream& operator>>(istream& is, vector<T>& v) { repea(x, v) is >> x; return is; }
template <class T> inline vector<T>& operator--(vector<T>& v) { repea(x, v) --x; return v; }
template <class T> inline vector<T>& operator++(vector<T>& v) { repea(x, v) ++x; return v; }

#endif // 折りたたみ用


#if __has_include(<atcoder/all>)
#include <atcoder/all>
using namespace atcoder;

#ifdef _MSC_VER
#include "localACL.hpp"
#endif

using mint = modint1000000007;
//using mint = modint998244353;
//using mint = modint; // mint::set_mod(m);

namespace atcoder {
	inline istream& operator>>(istream& is, mint& x) { ll x_; is >> x_; x = x_; return is; }
	inline ostream& operator<<(ostream& os, const mint& x) { os << x.val(); return os; }
}
using vm = vector<mint>; using vvm = vector<vm>; using vvvm = vector<vvm>; using vvvvm = vector<vvvm>; using pim = pair<int, mint>;
#endif


#ifdef _MSC_VER // 手元環境（Visual Studio）
#include "local.hpp"
#else // 提出用（gcc）
inline int popcount(int n) { return __builtin_popcount(n); }
inline int popcount(ll n) { return __builtin_popcountll(n); }
inline int lsb(int n) { return n != 0 ? __builtin_ctz(n) : -1; }
inline int lsb(ll n) { return n != 0 ? __builtin_ctzll(n) : -1; }
template <size_t N> inline int lsb(const bitset<N>& b) { return b._Find_first(); }
inline int msb(int n) { return n != 0 ? (31 - __builtin_clz(n)) : -1; }
inline int msb(ll n) { return n != 0 ? (63 - __builtin_clzll(n)) : -1; }
#define dump(...)
#define dumpel(v)
#define dump_list(v)
#define dump_mat(v)
#define input_from_file(f)
#define output_to_file(f)
#define Assert(b) { if (!(b)) { string s; while (1) s += "MLE";} } // メモリ爆食いするが MLE ではなく TLE が出る．
#endif


//【階乗など（法が大きな素数）】
/*
* Factorial_mint(int N) : O(n)
*	N まで計算可能として初期化する．
*
* mint fact(int n) : O(1)
*	n! を返す．
*
* mint fact_inv(int n) : O(1)
*	1/n! を返す（n が負なら 0 を返す）
*
* mint inv(int n) : O(1)
*	1/n を返す．
*
* mint perm(int n, int r) : O(1)
*	順列の数 nPr を返す．
*
* mint bin(int n, int r) : O(1)
*	二項係数 nCr を返す．
*
* mint bin_inv(int n, int r) : O(1)
*	二項係数の逆数 1/nCr を返す．
*
* mint mul(vi rs) : O(|rs|)
*	多項係数 nC[rs] を返す．（n = Σrs）
*
* mint hom(int n, int r) : O(1)
*	重複組合せの数 nHr = n+r-1Cr を返す（0H0 = 1 とする）
*
* mint neg_bin(int n, int r) : O(1)
*	負の二項係数 nCr = (-1)^r -n+r-1Cr を返す（n ≦ 0, r ≧ 0）
*/
class Factorial_mint {
	int n_max;

	// 階乗と階乗の逆数の値を保持するテーブル
	vm fac, fac_inv;

public:
	// n! までの階乗とその逆数を前計算しておく．O(n)
	Factorial_mint(int n) : n_max(n), fac(n + 1), fac_inv(n + 1) {
		// verify : https://atcoder.jp/contests/dwacon6th-prelims/tasks/dwacon6th_prelims_b

		fac[0] = 1;
		repi(i, 1, n) fac[i] = fac[i - 1] * i;

		fac_inv[n] = fac[n].inv();
		repir(i, n - 1, 0) fac_inv[i] = fac_inv[i + 1] * (i + 1);
	}
	Factorial_mint() : n_max(0) {} // ダミー

	// n! を返す．
	mint fact(int n) const {
		// verify : https://atcoder.jp/contests/dwacon6th-prelims/tasks/dwacon6th_prelims_b

		Assert(0 <= n && n <= n_max);
		return fac[n];
	}

	// 1/n! を返す（n が負なら 0 を返す）
	mint fact_inv(int n) const {
		// verify : https://atcoder.jp/contests/abc289/tasks/abc289_h

		Assert(n <= n_max);
		if (n < 0) return 0;
		return fac_inv[n];
	}

	// 1/n を返す．
	mint inv(int n) const {
		// verify : https://atcoder.jp/contests/exawizards2019/tasks/exawizards2019_d

		Assert(0 < n && n <= n_max);
		return fac[n - 1] * fac_inv[n];
	}

	// 順列の数 nPr を返す．
	mint perm(int n, int r) const {
		// verify : https://atcoder.jp/contests/abc172/tasks/abc172_e

		Assert(n <= n_max);

		if (r < 0 || n - r < 0) return 0;
		return fac[n] * fac_inv[n - r];
	}

	// 二項係数 nCr を返す．
	mint bin(int n, int r) const {
		// verify : https://judge.yosupo.jp/problem/binomial_coefficient_prime_mod

		Assert(n <= n_max);
		if (r < 0 || n - r < 0) return 0;
		return fac[n] * fac_inv[r] * fac_inv[n - r];
	}

	// 二項係数の逆数 1/nCr を返す．
	mint bin_inv(int n, int r) const {
		// verify : https://www.codechef.com/problems/RANDCOLORING

		Assert(n <= n_max);
		Assert(r >= 0 || n - r >= 0);
		return fac_inv[n] * fac[r] * fac[n - r];
	}

	// 多項係数 nC[rs] を返す．
	mint mul(const vi& rs) const {
		// verify : https://yukicoder.me/problems/no/2141

		if (*min_element(all(rs)) < 0) return 0;
		int n = accumulate(all(rs), 0);
		Assert(n <= n_max);

		mint res = fac[n];
		repe(r, rs) res *= fac_inv[r];

		return res;
	}

	// 重複組合せの数 nHr = n+r-1Cr を返す（0H0 = 1 とする）
	mint hom(int n, int r) {
		// verify : https://mojacoder.app/users/riantkb/problems/toj_ex_2

		if (n == 0) return (int)(r == 0);
		Assert(n + r - 1 <= n_max);
		if (r < 0 || n - 1 < 0) return 0;
		return fac[n + r - 1] * fac_inv[r] * fac_inv[n - 1];
	}

	// 負の二項係数 nCr を返す（n ≦ 0, r ≧ 0）
	mint neg_bin(int n, int r) {
		// verify : https://atcoder.jp/contests/abc345/tasks/abc345_g

		if (n == 0) return (int)(r == 0);
		Assert(-n + r - 1 <= n_max);
		if (r < 0 || -n - 1 < 0) return 0;
		return (r & 1 ? -1 : 1) * fac[-n + r - 1] * fac_inv[r] * fac_inv[-n - 1];
	}
};


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


//【全方位木 DP】O(n)
/*
* 与えられた木 g に対し，各頂点 s∈[0..n) について，
* s を根と見たときの問題の答えを格納したリストを返す．
* また必要なら各 s∈[0..n) と s に隣接する各頂点 t（j 番目）について，
* s-t 間の辺を切断し t を根と見たときの問題の答えを sub[s][j] に格納する．
*
* T leaf(int s) :
*   葉 s のみからなる部分木について，s を根と見たときの答えを返す．
*
* T add_edge(T x, int p, int s) :
*   頂点 s を根とする部分木の暫定の答えが x のとき，
*   辺 p'→s を追加して p' を仮の根と見たときの答えを返す（記号 ' は仮の頂点を表す）
*
* T merge(T x, T y, int s) :
*   仮の根 s' のみを共有する部分木 2 つに対する答えがそれぞれ x, y のとき，
*   これらをマージした部分木について同じく s' を仮の根と見たときの答えを返す．
*
* T add_vertex(T x, int s) :
*	仮の根 s' をもつ部分木 s' に対する答えが x のとき，
*	根 s を追加した部分木 s についての答えを返す．
*/
template <class T, T(*leaf)(int), T(*add_edge)(const T&, int, int), T(*merge)(const T&, const T&, int), T(*add_vertex)(const T&, int)>
vector<T> rerooting(const Graph& g, vector<vector<T>>* sub = nullptr) {
	// 参考 : https://atcoder.jp/contests/abc222/editorial/2749
	// verify : https://atcoder.jp/contests/abc149/tasks/abc149_f

	int n = sz(g);
	vector<T> res(n);

	// sub[s][j] : 
	// 頂点 s と接続する j 番目の頂点を t としたとき，s-t 間の辺を切断し，t を根と見たときの答え
	if (sub == nullptr) sub = new vector<vector<T>>;
	sub->resize(n);
	rep(s, n) (*sub)[s] = vector<T>(sz(g[s]));

	// 大きさ 1 の木に対する例外処理
	if (n == 1) return vector<T>{ leaf(0) };

	// p-s 間の辺を切断し，s を根と見たときの答えを計算する．
	//  p : 0 を根としたときの s の親
	//  sj : s が p に接続する何番目の頂点か
	function<void(int, int, int)> dfs1 = [&](int s, int p, int sj) {
		// 頂点 0 については後で計算するので計算不要．
		if (p == -1) {
			rep(tj, sz(g[s])) dfs1(g[s][tj], s, tj);;
			return;
		}

		// is_leaf : s が葉か
		bool is_leaf = true;

		rep(tj, sz(g[s])) {
			int t = g[s][tj];
			if (t == p) continue;

			// s-t 間の辺を切断し，t を根と見たときの答えを計算する．
			dfs1(t, s, tj);

			// 先の部分木に対して辺 s'→t を追加した場合の部分木 s' についての答えを得る．
			T val = add_edge((*sub)[s][tj], s, t);

			// それを部分木 s' の暫定の答えとマージして答えを計算していく．
			if (is_leaf) (*sub)[p][sj] = move(val);
			else (*sub)[p][sj] = merge((*sub)[p][sj], val, s);

			is_leaf = false;
		}

		// s が葉の場合は専用の答えを代入しておく．
		if (is_leaf) (*sub)[p][sj] = leaf(s);
		// そうでない場合は根 s を追加する．
		else (*sub)[p][sj] = add_vertex((*sub)[p][sj], s);
	};
	dfs1(0, -1, -1);

	// s を根と見たときの答えを計算する．
	//  p : 0 を根としたときの s の親
	//  val : s-p 間の辺を切断し，p を根と見たときの答え
	function<void(int, int, const T&)> dfs2 = [&](int s, int p, const T& val) {
		// K : 根 s から出る辺の数
		int K = sz(g[s]);

		// ds[j] : 仮の根 s' から出る j 番目の辺だけを s' に接続したときの答え
		vector<T> ds(K);

		rep(tj, K) {
			const auto& t = g[s][tj];
			if (t == p) {
				(*sub)[s][tj] = val;
				ds[tj] = add_edge(val, s, p);
				continue;
			}

			// s'-t 間の辺を切断し，t を根と見たときの答えは計算し終えているので，
			// その部分木に対して辺 s'→t を接続し s' を仮の根と見た場合の答えを得る．
			ds[tj] = add_edge((*sub)[s][tj], s, t);
		}

		// acc_l[j] : 仮の根 s' の [0..j] 番目の辺を s' に接続したときの答え
		vector<T> acc_l(K);
		acc_l[0] = ds[0];
		repi(tj, 1, K - 1) acc_l[tj] = merge(acc_l[tj - 1], ds[tj], s);

		// acc_r[j] : 仮の根 s' の [j..K) 番目の辺を s' に接続したときの答え
		vector<T> acc_r(K);
		acc_r[K - 1] = ds[K - 1];
		repir(tj, K - 2, 0) acc_r[tj] = merge(acc_r[tj + 1], ds[tj], s);

		// 仮の根 s' から出る全ての辺を s' に接続し，根 s を追加したときの答えが求めるものである．
		res[s] = add_vertex(acc_l[K - 1], s);

		rep(tj, K) {
			const auto& t = g[s][tj];
			if (t == p) continue;

			// 仮の根 s' に辺 s'→t 以外の全ての辺を接続し，根 s を追加したときの答え，
			// すなわち，辺 t-s を切断し，s を根と見たときの答えを再帰関数に渡す．
			if (K == 1) dfs2(t, s, leaf(s));
			else if (tj == 0) dfs2(t, s, add_vertex(acc_r[1], s));
			else if (tj == K - 1) dfs2(t, s, add_vertex(acc_l[K - 2], s));
			else dfs2(t, s, add_vertex(merge(acc_l[tj - 1], acc_r[tj + 1], s), s));
		}
	};
	dfs2(0, -1, leaf(0)); // 第 3 引数はダミー

	return res;

	/* 雛形
	struct T {
		int v;
	};
	T leaf(int s) {
		return { 1 };
	}
	T add_edge(const T& x, int p, int s) {
		return { x.v };
	}
	T merge(const T& x, const T& y, int s) {
		return { x.v + y.v };
	}
	T add_vertex(const T& x, int s) {
		return { x.v + 1 };
	}
	vector<T> solve_by_tree_getDP(const Graph& g) {
		return rerooting<T, leaf, add_edge, merge, add_vertex>(g);
	}
	*/
};


//【ヒープの数え上げ】O(n)
/*
* 与えられた木 g の頂点に対する数 [0..n) の割り当て方のうち，
* 各 s∈[0..n) を根とみなしたときヒープをなすものの数を格納したリストを返す．
*
* 制約：fm は n! まで計算可能
*
* 利用：【全方位木 DP】
*/
struct T_ch {
	mint v; // ヒープの数
	int c; // 辺の数
};
Factorial_mint const* fm_ch;
T_ch leaf_ch(int s) {
	return { 1, 0 }; 
}
T_ch add_edge_ch(const T_ch& x, int p, int s) {
	return { x.v, x.c + 1 };
}
T_ch merge_ch(const T_ch& x, const T_ch& y, int s) {
	// 左右それぞれで大小順が変わらない限り，独立に数の再割り当てができる．
	return { x.v * y.v * fm_ch->bin(x.c + y.c, x.c), x.c + y.c };
}
T_ch add_vertex_ch(const T_ch& x, int s) {
	// 根に割り当てられる数は 0 しかなく，他については全体に 1 加算される．
	return x;
}
vm count_heap(const Graph& g, const Factorial_mint& fm) {
	// verify : https://atcoder.jp/contests/abc160/tasks/abc160_f

	int n = sz(g);
	fm_ch = &fm;

	auto dp = rerooting<T_ch, leaf_ch, add_edge_ch, merge_ch, add_vertex_ch>(g);

	vm res(n);
	rep(i, n) res[i] = dp[i].v;

	return res;
}


int main() {
//	input_from_file("input.txt");
//	output_to_file("output.txt");

	int n;
	cin >> n;

	auto g = read_Graph(n);

	Factorial_mint fm(n);
	auto cnt = count_heap(g, fm);

	mint res = 0;
	rep(i, n) res += cnt[i];
	res /= 2; // 最初に描く辺の両端の頂点のどちらを根とみなすかの自由度で割る

	cout << res << endl;
}
