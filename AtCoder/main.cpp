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
#define rep(i, n) for(int i = 0, i##_len = ll(n); i < i##_len; ++i) // 0 から n-1 まで昇順
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


//【任意文字列の列挙】O(n |cs|^n)
/*
* 文字集合 cs の要素からなる長さ n の文字列全てを strs に格納する．
*/
void all_strings(int n, const vc& cs, vector<string>& strs) {
	strs.clear();
	string s;

	// l : 長さ
	function<void(int)> rf = [&](int l) {
		// 長さが n の場合は記録
		if (l == n) {
			strs.push_back(s);
			return;
		}

		// c : s[l]
		repe(c, cs) {
			s.push_back(c);
			rf(l + 1);
			s.pop_back();
		}
	};

	rf(0);
}


// O(n 2^n)
int TLE(const string& s, int k) {
	int n = sz(s);
	int c_min = INF;
	
	repb(set, n - 1) {
		vi cnt(k);

		bool ok = true;
		cnt[s[0] - 'a']++;

		rep(i, n - 1) {
			if (set & (1 << i)) {
				int odd = 0;
				rep(j, k) if (cnt[j] % 2 == 1) odd++;

				if (odd >= 2) {
					ok = false;
					break;
				}

				cnt.assign(k, 0);
			}
			cnt[s[i + 1] - 'a']++;
		}
		int odd = 0;
		rep(j, k) if (cnt[j] % 2 == 1) odd++;
		if (odd >= 2) ok = false;

		if (!ok) continue;

		chmin(c_min, popcount(set) + 1);
	}

	return c_min;
}


void zikken() {
	int n = 5, k = 3;

	vc cs(k);
	iota(all(cs), 'a');

	vector<string> strs;
	all_strings(n, cs, strs);

	int c_max = -INF; string s_max;

	repe(s, strs) {
		int c_min = TLE(s, k);

		if (chmax(c_max, c_min)) s_max = s;
	}

	dump(c_max, s_max);
}
/*
文字が 2 種類の場合：
1 a
2 ab
1 aaa
2 aaab
1 aaaaa
2 aaaaab
1 aaaaaaa

文字が 3 種類の場合：
1 a
2 ab
3 abc
4 abca
3 aaabc
4 aaabca
3 aaaaabc
4 aaaaabca

文字が 4 種類の場合：
1 a
2 ab
3 abc
4 abca
5 abcad
6 abcadb
7 abcadba
6 aaabcadb
7 aaabcadba

文字が 5 種類の場合：
1 a
2 ab
3 abc
4 abca
5 abcad
6 abcadb
7 abcadba
8 abcadbae

文字が 6 種類の場合の一例：
13 fabfcafdbfecf
16 fabfcafdbfecfabf
*/

int main() {
//	input_from_file("input.txt");
//	output_to_file("output.txt");

//	zikken(); return 0;

	int c = TLE("eabecaedbecbe", 5);
	dump(c);
}
