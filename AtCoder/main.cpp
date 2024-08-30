#ifndef HIDDEN_IN_VS // 折りたたみ用

// 警告の抑制
#define _CRT_SECURE_NO_WARNINGS

// ライブラリの読み込み
#include <bits/stdc++.h>
using namespace std;

// 型名の短縮
using ll = long long; using ull = unsigned long long; // -2^63 ～ 2^63 = 9e18（int は -2^31 ～ 2^31 = 2e9）
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
int DX[4] = { 1, 0, -1, 0 }; // 4 近傍（下，右，上，左）
int DY[4] = { 0, 1, 0, -1 };
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
#define repis(i, set) for(int i = lsb(set), bset##i = set; i < 32; bset##i -= 1 << i, i = lsb(bset##i)) // set の全要素（昇順）
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

//using mint = modint1000000007;
using mint = modint998244353;
//using mint = static_modint<999999937>;
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
inline int lsb(int n) { return n != 0 ? __builtin_ctz(n) : 32; }
inline int lsb(ll n) { return n != 0 ? __builtin_ctzll(n) : 64; }
template <size_t N> inline int lsb(const bitset<N>& b) { return b._Find_first(); }
inline int msb(int n) { return n != 0 ? (31 - __builtin_clz(n)) : -1; }
inline int msb(ll n) { return n != 0 ? (63 - __builtin_clzll(n)) : -1; }
#define dump(...)
#define dumpel(v)
#define dump_list(v)
#define dump_mat(v)
#define input_from_file(f)
#define output_to_file(f)
#define Assert(b) { if (!(b)) { vc MLE(1<<30); EXIT(MLE.back()); } } // RE の代わりに MLE を出す
#endif


//【ディリクレ畳込みの累積和（素朴）】O(n)
/*
* 数論関数 a[1..n] と b[1..n] のディリクレ畳込みを c とし，Σc[1..n] を返す．
*/
template <class T>
T naive_dirichlet_convolution_acc(const vector<T>& a, const vector<T>& b) {
	// 参考 : https://maspypy.com/dirichlet-%e7%a9%8d%e3%81%a8%e3%80%81%e6%95%b0%e8%ab%96%e9%96%a2%e6%95%b0%e3%81%ae%e7%b4%af%e7%a9%8d%e5%92%8c

	//【方法】
	// a[1..n] の累積和を A[i] = Σa[1..i] などとおく．求めたい C[n] は
	//		C[n]
	//		= Σ_k∈[1..n] Σ_(i×j=k) a[i] b[j]
	//		= Σ_(i×j≦n) a[i] b[j]
	// と表される．i ≦ √n の範囲からの寄与は
	//		Σ_(i≦√n) Σ_(j≦n/i) a[i] b[j] = Σ_i≦√n a[i] B[n/i]
	// となる．i > √n の範囲からの寄与は，j ≦ √n であることに注意すると，
	//		Σ_(j≦√n) Σ_(√n<i≦n/j) a[i] b[j] = Σ_j≦√n b[j] (A[n/j] - A[√n])
	// となる．

	int n = sz(a) - 1, m = (int)(sqrt(n) + 0.01);

	// A[i] = Σa[1..i], B[i] = Σb[1..i]
	vector<T> A(n + 1), B(n + 1);

	// 累積和を求めるのに O(n) 時間かかってしまう．
	repi(i, 1, n) {
		A[i] = A[i - 1] + a[i];
		B[i] = B[i - 1] + b[i];
	}

	// 寄与の足し合わせは平方分割しているので O(√n) 時間で済む．
	T res = 0;
	repi(i, 1, m) res += a[i] * B[n / i];
	repi(j, 1, m) res += b[j] * (A[n / j] - A[m]);

	return res;
}


//【添字整数商 vector】
/*
* v[1], v[2], ..., v[nl], v[N/nh], ..., v[N/2], v[N/1] にのみアクセスできる疎な vector
* 
* Vector_floor<T>(ll N) : O(1)
*	nl = √N とし，v[N/d] にアクセスできるよう初期化する．
* 
* Vector_floor<T>(ll N, int nl) : O(1)
*	v[N/d] にアクセスできるよう初期化する．
* 
* T [ll i] : O(1)
*	v[i] にアクセスする．
* 
* T get_l(int i) : O(1)
*	v[i] を返す．
*
* set_l(int i, T x) : O(1)
*	v[i] = x とする．
* 
* T get_h(int d) : O(1)
*	v[N/d] を返す．
*
* set_h(int d, T x) : O(1)
*	v[N/d] = x とする．
*/
template <class T>
class Vector_floor {
	// v : v[1], v[2], ..., v[nl], v[N/nh], ..., v[N/2], v[N/1] を並べたリスト
	vector<T> v;
	int nlh;

public:
	ll N;
	int nl, nh;

	// nl = √N とし，v[N/d] にアクセスできるよう初期化する．
	Vector_floor(ll N) : N(N) {
		nl = (int)(sqrt(N) + 1e-9);
		nh = (int)((N + nl - 1) / nl);
		nlh = nl + nh;
		v.resize(nlh);
	}

	// v[N/d] にアクセスできるよう初期化する．
	Vector_floor(ll N, int nl) : N(N), nl(nl) {
		nh = (int)((N + nl - 1) / nl);
		nlh = nl + nh;
		v.resize(nlh);
	}

	// v[i] にアクセスする．
	inline T const& operator[](ll i) const {
		return i <= nl ? v[i - 1] : v[nlh - N / i];
	}
	inline T& operator[](ll i) {
		return i <= nl ? v[i - 1] : v[nlh - N / i];
	}

	// v[i] を返す．
	T get_l(int i) const {
		return v[i - 1];
	}

	// v[i] = x とする．
	void set_l(int i, T x) {
		return v[i - 1] = x;
	}

	// v[N/d] を返す．
	T get_h(int d) const {
		return N / d <= nl ? v[N / d - 1] : v[nlh - d];
	}

	// v[N/d] = x とする．
	void set_h(int d, T x) {
		return (N / d <= nl ? v[N / d - 1] : v[nlh - d]) = x;
	}
};


//【ディリクレ畳込みの総和】O(√N)
/*
* 数論関数 a と b のディリクレ畳込みを c とし，Σc[1..N] を返す．
* A, B はそれぞれ a, b の累積和であり，nl ≧ √N を満たすとする．
*/
template <class T>
T dirichlet_convolution_sum(const Vector_floor<T>& A, const Vector_floor<T>& B) {
	// 参考 : https://maspypy.com/dirichlet-%e7%a9%8d%e3%81%a8%e3%80%81%e6%95%b0%e8%ab%96%e9%96%a2%e6%95%b0%e3%81%ae%e7%b4%af%e7%a9%8d%e5%92%8c

	//【方法】
	// m=√N，a[1..N] の累積和を A[i] = Σa[1..i] などとおく．求めたい C[N] は
	//		C[N]
	//		= Σ_k∈[1..N] Σ_(i×j=k) a[i] b[j]
	//		= Σ_(i×j≦N) a[i] b[j]
	// と表される．i ≦ m の範囲からの寄与は
	//		Σ_(i≦m) Σ_(j≦N/i) a[i] b[j] = Σ_(i≦m) a[i] B[N/i]
	// となる．i > m の範囲からの寄与は，j ≦ m であることに注意すると，
	//		Σ_(j≦m) Σ_(m<i≦N/j) a[i] b[j] = Σ_(j≦m) b[j] (A[N/j] - A[m])
	// となる．

	int nl = A.nl;

	T c_sum = 0;

	c_sum += A.get_l(1) * B.get_h(1);
	repi(i, 2, nl) c_sum += (A.get_l(i) - A.get_l(i - 1)) * B.get_h(i);

	c_sum += B.get_l(1) * (A.get_h(1) - A.get_l(nl));
	repi(j, 2, nl) c_sum += (B.get_l(j) - B.get_l(j - 1)) * (A.get_h(j) - A.get_l(nl));

	return c_sum;
}


template <class T>
T solve(const vector<T>& a, const vector<T>& b) {
	int N = sz(a) - 1;

	vector<T> A(N + 1), B(N + 1);
	repi(i, 1, N) {
		A[i] = A[i - 1] + a[i];
		B[i] = B[i - 1] + b[i];
	}

	Vector_floor<T> A2(N), B2(N);
	int nl = A2.nl, nh = A2.nh;

	repi(i, 1, nl) {
		A2[i] = A[i];
		B2[i] = B[i];
	}

	repi(d, 1, nh) {
		A2[N / d] = A[N / d];
		B2[N / d] = B[N / d];
	}

	return dirichlet_convolution_sum(A2, B2);
}


void bug_find() {
	// 合わない入力例を見つける．

	mt19937_64 mt;
	mt.seed((int)time(NULL));
	uniform_int_distribution<ll> rnd(0LL, 1LL << 62);

	mute_dump = true;

	rep(hoge, 1000) {
		int n = rnd(mt) % 1000 + 1;
		vl a(n + 1), b(n + 1);
		repi(i, 1, n) {
			a[i] = rnd(mt) % 100 - 50;
			b[i] = rnd(mt) % 100 - 50;
		}

		auto res_naive = naive_dirichlet_convolution_acc(a, b);
		auto res_solve = solve(a, b);

		if (res_naive != res_solve) {
			cout << "----------error!----------" << endl;
			cout << "input:" << endl;
			cout << a << endl;
			cout << b << endl;
			cout << "results:" << endl;
			cout << res_naive << endl;
			cout << res_solve << endl;
			cout << "--------------------------" << endl;
		}
	}

	mute_dump = false;
}


int main() {
//	input_from_file("input.txt");
//	output_to_file("output.txt");

	bug_find(); exit(0);

	vl a{ 0, 3 };
	vl b{ 0, 4 };

	cout << naive_dirichlet_convolution_acc(a, b) << endl;

	dump("--------");

	cout << solve(a, b) << endl;
}
