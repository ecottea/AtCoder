#pragma once


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
const vi DX = { 1, 0, -1, 0 }; // 4 近傍（下，右，上，左）
const vi DY = { 0, 1, 0, -1 };
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
template <class T> inline ll powi(T n, int k) { ll v = 1; rep(i, k) v *= n; return v; } // 非負整数乗
template <class T> inline bool chmax(T& M, const T& x) { if (M < x) { M = x; return true; } return false; } // 最大値を更新（更新されたら true を返す）
template <class T> inline bool chmin(T& m, const T& x) { if (m > x) { m = x; return true; } return false; } // 最小値を更新（更新されたら true を返す）
template <class T> inline T getb(T set, int i) { return (set >> i) & T(1); } // 第iビット
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


// C++ の便利な機能
/*
// 0 埋め出力（d : 桁数，res : 出力したい数値）
cout << setfill('0') << right << setw(d) << res << endl;
 
// string s_ を vc s に変換
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

// str が部分文字列として pat を含むかを返す：O(n m)
str.find(pat) != string::npos

// 集合の共通部分，和集合，差集合を得る
set_intersection(all(a), all(b), inserter(res, res.end()));
set_union(all(a), all(b), inserter(res, res.end()));
set_difference(all(a), all(b), inserter(res, res.end()));

// x に [l..r] 上の一様乱数を代入する
mt19937_64 mt((int)time(NULL));
uniform_int_distribution<ll> rnd(l, r);
ll x = rnd(mt);

// 配列 a をランダムにシャッフルする
mt19937_64 mt((int)time(NULL));
shuffle(all(a), mt);

// 型 T の最小値[最大値] を取得する
numeric_limits<T>::lowest();
numeric_limits<T>::max();（__int128 だと 0 になるので注意）

// vector のメモリ解放
a.resize(0);
a.shrink_to_fit();

// map を逆順で使う
map<S, T, greater<S>>

// 多倍長整数（入出力が O(n^2) なので注意！）
#include <boost/multiprecision/cpp_int.hpp>
using Bint = boost::multiprecision::cpp_int;
Bint gcd(const Bint& x, const Bint& y) { return boost::math::gcd(x, y); }
Bint lcm(const Bint& x, const Bint& y) { return boost::math::lcm(x, y); }
boost::swap ?
boost::move ?

// bitset で MSB 位置取得（gcc 限定）
bitset._Find_first();

// 時間計測して TLE 寸前に終了
auto start = chrono::system_clock::now();
auto now = chrono::system_clock::now();
auto msec = chrono::duration_cast<chrono::milliseconds>(now - start).count();
if (msec >= 1950) break;

// QCFium 法
#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
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


// マルチテストケース用の雛形
/* ----------------------------------------------------------------

void Main() {
	
}

int main() {
//	input_from_file("input.txt");
//	output_to_file("output.txt");

	int t = 1;
	cin >> t; // マルチテストケースの場合

	while (t--) {
		dump("------------------------------");
		Main();
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

void bug_find() {
#ifdef _MSC_VER
	// 合わない入力例を見つける．

	mute_dump = true;

	mt19937_64 mt;
	mt.seed((int)time(NULL));
	uniform_int_distribution<ll> rnd(0LL, 1LL << 60);

	rep(hoge, 100) {
		int n = rnd(mt) % 10 + 1;
		vi a(n);
		rep(i, n) a[i] = rnd(mt) % 100;
		
		auto res_naive = naive(n, a);
		auto res_solve = solve(n, a);

		if (res_naive != res_solve) {
			cout << "----------error!----------" << endl;
			cout << "input:" << endl;
			cout << n << endl;
			cout << a << endl;
			cout << "results:" << endl;
			cout << res_naive << endl;
			cout << res_solve << endl;
			cout << "--------------------------" << endl;
		}
	}

	mute_dump = false;
	exit(0);
#endif
}

--------------------------------------------------------------- */


// Mathematica での P-recursive 実験用
/* -----------------------------------------------------------------

Clear[c, nn, dpsub];
seq = { 愚直に計算した a[1..] } (* 添字が 1 始まりなのに注意！ *);
terms = 3 (* 何項間漸化式の存在を仮定するか *);
degree = 2 (* 係数多項式の次数を何次未満と仮定するか *);
eqs = Table[Sum[c[i, j] (nn - i)^j seq[[nn - i]], {i, 0, terms - 1}, {j, 0, degree - 1}] == 0, {nn, terms, Length@seq}];
fi = FindInstance[eqs, Flatten@Table[c[i, j], {i, 0, terms - 1}, {j, 0, degree - 1}], Integers, 2][[1]]
sol = Solve[Sum[c[i, j] (nn - i)^j dpsub[nn - i], {i, 0, terms - 1}, {j, 0, degree - 1}] == 0 /. fi, dpsub[nn]][[1]]
CForm@FullSimplify@sol[[1, 2]]

時間がかかりすぎるようなら，fi の 1 行を以下の 2 行に置き換える：

eqs = eqs~Join~{c[0, degree-1] == 1} (* 1 に固定する係数の位置は適宜調整する *);
fi = FindInstance[eqs, Flatten@Table[c[i, j], {i, 0, terms - 1}, {j, 0, degree - 1}]][[1]]

コピペ後の整形では以下の関数を利用できる：

auto dpsub = [&](const mint& x) { return dp[x.val()]; };
auto Power = [&](const mint& x, int n) { mint res = 1; rep(hoge, n) res *= x; return res; };

--------------------------------------------------------------- */


// mint を手元環境でだけ有理数表示したいとき用
/* -----------------------------------------------------------------

string mint_to_frac(mint x, int v_max = 31595) {
	repi(dnm, 1, v_max) {
		int num = (x * dnm).val();
		if (num == 0) {
			return "0";
		}
		if (num <= v_max) {
			if (dnm == 1) return to_string(num);
			return to_string(num) + "/" + to_string(dnm);
		}
		if (mint::mod() - num <= v_max) {
			if (dnm == 1) return "-" + to_string(mint::mod() - num);
			return "-" + to_string(mint::mod() - num) + "/" + to_string(dnm);
		}
	}

	return to_string(x.val());
}

namespace atcoder {
	inline istream& operator>>(istream& is, mint& x) { ll x_; is >> x_; x = x_; return is; }
#ifdef _MSC_VER
	inline ostream& operator<<(ostream& os, const mint& x) { os << mint_to_frac(x); return os; }
#else
	inline ostream& operator<<(ostream& os, const mint& x) { os << x.val(); return os; }
#endif	
}

-------------------------------------------------------------- - */


// インタラクティブ問題のデバッグ用の雛形
/* -----------------------------------------------------------------

struct Opponent {
	bool sub_mode;
	mt19937_64 mt;
	uniform_int_distribution<ll> rnd;

	// 必要なデータ構造

	// 初期化
	Opponent(bool sub_mode = true) : sub_mode(sub_mode) {
		mt.seed((int)time(NULL));
		rnd = uniform_int_distribution<ll>(0LL, 1LL << 62);

	}

	// ジャッジプログラムからの入力を受け取る．
	void in(int& x) {
		// 提出用
		if (sub_mode) {
			cin >> x;
		}
		// デバッグ用
		else {
			
		}
	}

	// ジャッジプログラムに出力を渡す．
	void out(int x) {
		cout << x << endl;

		// デバッグ用
		if (!sub_mode) {
				
		}
	}
};

--------------------------------------------------------------- */


