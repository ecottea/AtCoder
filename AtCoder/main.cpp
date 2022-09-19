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
#define input_from_file(f)
#define output_to_file(f)
#define Assert(b) { if (!(b)) while (1) cout << "OLE"; }
#endif

#endif // 折りたたみ用


//--------------AtCoder 専用--------------
#include <atcoder/all>
using namespace atcoder;

//using mint = modint1000000007;
using mint = modint998244353;
//using mint = modint; // mint::set_mod(m);

istream& operator>>(istream& is, mint& x) { ll x_; is >> x_; x = x_; return is; }
ostream& operator<<(ostream& os, const mint& x) { os << x.val(); return os; }
using vm = vector<mint>; using vvm = vector<vm>; using vvvm = vector<vvm>;
//----------------------------------------


void naive(int n, const vi& a) {
	vector<pii> la(n);
	rep(i, n) la[i] = { a[0] / gcd(a[0], a[i]) * a[i], a[i] };

	repi(i, 1, n - 2) {
//		rep(j, n) cerr << la[j].second << (j < n - 1 ? " " : "\n");
		sort(la.begin() + i, la.end());
		repi(j, i + 1, n - 1) {
			la[j].first = la[i].second / gcd(la[i].second, la[j].second) * la[j].second;
		}
	}

	rep(i, n) cout << la[i].second << (i < n - 1 ? " " : "\n");
}


void zikken() {
	int n = 30;

	vi a(n);
	rep(i, n) a[i] = i + 1;
	swap(a[0], a[29]);

	mt19937_64 mt((int)time(NULL));
//	shuffle(all(a), mt);
	dump(a);

	naive(n, a);
	
	exit(0);
}
/*
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30
1 2 4 8 16 3 6 12 24 9 18 27 5 10 20 15 30 25 7 14 28 21 11 22 13 26 17 19 23 29

3 17 8 7 10 16 13 4 6 9 4 3 5 6 14 8 9 11 12 11 5 13 16 14 15 15 10 7 17 12
3 3 6 6 4 4 8 8 16 16 12 12 9 9 5 5 10 10 15 15 7 7 14 14 11 11 13 13 17 17

5 7 28 16 18 4 19 10 27 8 22 15 14 12 11 2 20 17 1 9 25 6 29 23 21 30 24 26 3 13
5 1 2 4 8 16 3 6 12 24 9 18 27 15 10 20 30 25 7 14 28 21 11 22 13 26 17 19 23 29

1 2 3 4 30 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29
1 2 4 8 16 3 6 12 24 9 18 27 15 10 20 30 25 7 14 28 21 11 22 13 26 17 19 23 29

30 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 1
30 1 2 4 8 16 3 6 12 24 9 18 27 5 10 20 15 25 7 14 28 21 11 22 13 26 17 19 23 29
*/


int main() {
//	input_from_file("input.txt");
//	output_to_file("output.txt");
	
	zikken();

	int n;
	cin >> n;

	vi a(n);
	cin >> a;

	naive(n, a);	
}
