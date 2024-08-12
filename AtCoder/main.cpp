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
//using mint = static_modint<1234567891>;
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


void WA() {
	int n; ll m;
	cin >> n >> m;

	vl a(n), b(n);
	cin >> a >> b;

	rep(i, n) {
		if (a[i] < b[i]) {
			b[i] -= m;
		}
	}
	dump(a); dump(b);

	vvl dp(n, vl(2, INFL));

	rep(i, n) {
		if (i == 0) {
			chmin(dp[i][0], a[i] - b[i]);
			chmin(dp[i][1], (b[i] + m) - a[i]);
		}
		else {
			if ((a[i - 1] < a[i]) == (b[i - 1] < b[i])) {
				chmin(dp[i][0], dp[i - 1][0] + (a[i] - b[i]));
			}
			if ((a[i - 1] < a[i]) == (b[i - 1] + m < b[i])) {
				chmin(dp[i][0], dp[i - 1][1] + (a[i] - b[i]));
			}
			if ((a[i - 1] < a[i]) == (b[i - 1] < b[i] + m)) {
				chmin(dp[i][1], dp[i - 1][0] + ((b[i] + m) - a[i]));
			}
			if ((a[i - 1] < a[i]) == (b[i - 1] + m < b[i] + m)) {
				chmin(dp[i][1], dp[i - 1][1] + ((b[i] + m) - a[i]));
			}
		}
	}
	dumpel(dp);

	ll res = min(dp[n - 1][0], dp[n - 1][1]);
	if (res > INFL / 2) res = -1;

	EXIT(res);
}


int main() {
//	input_from_file("input.txt");
//	output_to_file("output.txt");

	int n; ll m;
	cin >> n >> m;

	vl a(n), b(n);
	cin >> a >> b;

	int K = 1;
	vvl dp(n, vl(2 * K + 1, INFL));

	rep(i, n) {
		if (i == 0) {
			repi(k, -K, K) {
				chmin(dp[i][k + K], abs(a[i] - (b[i] + m * k)));
			}
		}
		else {
			repi(k, -K, K) {
				repi(nk, -K, K) {
					if ((a[i - 1] < a[i]) == (b[i - 1] + m * k < b[i] + m * nk)) {
						chmin(dp[i][nk + K], dp[i - 1][k + K] + abs(a[i] - (b[i] + m * nk)));
					}
				}
			}
		}
	}
	dumpel(dp);

	ll res = INFL;
	repi(k, -K, K) chmin(res, dp[n - 1][k + K]);
	if (res > INFL / 2) res = -1;

	EXIT(res); // 小さすぎるので論外
}
