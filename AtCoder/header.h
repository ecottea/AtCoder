#pragma once


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
const int INF = 1001001001; const ll INFL = 4004004004004004004LL;
const double EPS = 1e-12; // 許容誤差に応じて調整

// 入出力高速化
struct fast_io { fast_io() { cin.tie(nullptr); ios::sync_with_stdio(false); cout << fixed << setprecision(15); } } fastIOtmp;

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



// C++ の便利な機能
/*
// 0 埋め出力（d : 桁数，res : 出力したい数値）
cout << setfill('0') << right << setw(d) << res << endl;
 
// string s_ を vector<char> s に変換
vc s(all(s_));

// vc s_ を string s に変換
string s(all(s_));

// string s を int n [ll n] に変換
int n = stoi(s); [ ll n = stoll(s); ]

// string s から s[i..i+k) を取得
string sub = s.substr(i, k);

// ll n を二進法表示の string s に変換
string s = bitset<63>(n).to_string();

// 空白も含め一行を文字列 str として読み込み
string str;
getline(cin, str);

// 空白区切りで文字列 str から s に読み込み
stringstream ss{ str };
string s;
while (getline(ss, s, ' ')) {}

// 集合の共通部分，和集合を得る
set_intersection(all(a), all(b), inserter(res, res.end()));
set_union(all(a), all(b), inserter(res, res.end()));

// x に [l..r] 上の一様乱数を代入する
mt19937_64 mt((int)time(NULL));
uniform_int_distribution<ll> rnd(l, r);
ll x = rnd(mt);

// 配列 a をランダムにシャッフルする
mt19937_64 mt((int)time(NULL));
shuffle(all(a), mt);

// 型 T の最小値[最大値] を取得する．
numeric_limits<T>::min(); numeric_limits<T>::max();

// 時間計測して TLE 寸前に終了
auto start = chrono::system_clock::now();
auto now = chrono::system_clock::now();
auto msec = chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
if (msec >= 1950) break;
*/


// Visual Studio の便利な機能
/*
Ctrl + K → Ctrl + F：一括インデント調整
Ctrl + R → Ctrl + R：リネーム
Ctrl + M → Ctrl + L：一括折りたたみ／展開
Ctrl + M → Ctrl + M：折りたたみ／展開（ドラッグ後ならその部分のみ一括）

AtCoder → プロパティ → 構成プロパティ → C/C++ → 詳細設定 → 指定の警告を無効にする
に 26451 を追加することで，配列の添字内での加算などへの警告を抑制できる．

コード -1073741571 のエラーはスタックオーバーフロー．AtCoder 上での実行なら大丈夫．
*/


// マルチテストケース 用の雛形
/* ----------------------------------------------------------------

void solve() {
	
}

int main() {
//	input_from_file("input.txt");
//	output_to_file("output.txt");

	int t;
	cin >> t; // マルチテストケースの場合
//	t = 1; // シングルテストケースの場合

	while (t--) {
		dump("------------------------------");
		solve();
	}
}

------------------------------------------------------------------ */


// 実験用の雛形
/* -----------------------------------------------------------------

int naive(int n, const vi& a) {
	// 愚直コード

	int res = 0;

	return res;
}

int solve(int n, const vi& a) {
	// 提出用コード

	int res = 0;

	return res;
}

void zikken() {
	// 合わない入力例を見つける．

	mt19937_64 mt;
	mt.seed((int)time(NULL));
	uniform_int_distribution<ll> rnd(0LL, 1LL << 62);

#ifdef _MSC_VER
	mute_dump = true;
#endif

	rep(hoge, 100) {
		int n = 100;
		vi a(n);
		rep(i, n) {
			a[i] = rnd(mt) % 3;
		}

		auto res_naive = naive(n, a);
		auto res_solve = solve(n, a);

#ifdef _MSC_VER
		if (res_naive != res_solve) {
			cout << "----------error!----------" << endl;
			cout << "input:" << endl;
			cout << a << endl;
			cout << "results: << endl;
			cout << res_naive << endl;
			cout << res_solve << endl;
			cout << "--------------------------" << endl;
		}
#endif
	}

#ifdef _MSC_VER
	mute_dump = false;
#endif
}

--------------------------------------------------------------- */
