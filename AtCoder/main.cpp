#ifndef HIDDEN_IN_VISUAL_STUDIO // 折りたたみ用

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
const double PI = 3.14159265359;
const double DEG = PI / 180.; // θ [deg] = θ * DEG [rad]
const vi dx4 = { 1, 0, -1, 0 }; // 4 近傍（下，右，上，左）
const vi dy4 = { 0, 1, 0, -1 };
const vi dx8 = { 1, 1, 0, -1, -1, -1, 0, 1 }; // 8 近傍
const vi dy8 = { 0, 1, 1, 1, 0, -1, -1, -1 };
const int INF = 1001001001; const ll INFL = 2002002002002002002LL;
const double EPS = 1e-10; // 許容誤差に応じて調整

// 入出力高速化
struct fast_io { fast_io() { cin.tie(nullptr); ios::sync_with_stdio(false); cout << fixed << setprecision(15); } } fastIOtmp;

// 汎用マクロの定義
#define all(a) (a).begin(), (a).end()
#define sz(x) ((int)(x).size())
#define distance (int)distance
#define Yes(b) {cout << ((b) ? "Yes" : "No") << endl;}
#define rep(i, n) for(int i = 0, i##_len = int(n); i < i##_len; ++i) // 0 から n-1 まで昇順
#define repi(i, s, t) for(int i = int(s), i##_end = int(t); i <= i##_end; ++i) // s から t まで昇順
#define repir(i, s, t) for(int i = int(s), i##_end = int(t); i >= i##_end; --i) // s から t まで降順
#define repe(v, a) for(const auto& v : (a)) // a の全要素（変更不可能）
#define repea(v, a) for(auto& v : (a)) // a の全要素（変更可能）
#define repb(set, d) for(int set = 0; set < (1 << int(d)); ++set) // d ビット全探索（昇順）
#define repp(a) sort(all(a)); for(bool a##_perm = true; a##_perm; a##_perm = next_permutation(all(a))) // a の順列全て（昇順）
#define repit(it, a) for(auto it = (a).begin(); it != (a).end(); ++it) // イテレータを回す（昇順）
#define repitr(it, a) for(auto it = (a).rbegin(); it != (a).rend(); ++it) // イテレータを回す（降順）
#define smod(n, m) ((((n) % (m)) + (m)) % (m)) // 非負mod
#define uniq(a) {sort(all(a)); (a).erase(unique(all(a)), (a).end());} // 重複除去

// 汎用関数の定義
template <class T> inline ll pow(T n, int k) { ll v = 1; rep(i, k) v *= n; return v; }
template <class T> inline bool chmax(T& M, const T& x) { if (M < x) { M = x; return true; } return false; } // 最大値を更新（更新されたら true を返す）
template <class T> inline bool chmin(T& m, const T& x) { if (m > x) { m = x; return true; } return false; } // 最小値を更新（更新されたら true を返す）

// 演算子オーバーロード
template <class T, class U> inline istream& operator>> (istream& is, pair<T, U>& p) { is >> p.first >> p.second; return is; }
template <class T, class U> inline ostream& operator<< (ostream& os, const pair<T, U>& p) { os << "(" << p.first << "," << p.second << ")"; return os; }
template <class T, class U, class V> inline istream& operator>> (istream& is, tuple<T, U, V>& t) { is >> get<0>(t) >> get<1>(t) >> get<2>(t); return is; }
template <class T, class U, class V> inline ostream& operator<< (ostream& os, const tuple<T, U, V>& t) { os << "(" << get<0>(t) << "," << get<1>(t) << "," << get<2>(t) << ")"; return os; }
template <class T, class U, class V, class W> inline istream& operator>> (istream& is, tuple<T, U, V, W>& t) { is >> get<0>(t) >> get<1>(t) >> get<2>(t) >> get<3>(t); return is; }
template <class T, class U, class V, class W> inline ostream& operator<< (ostream& os, const tuple<T, U, V, W>& t) { os << "(" << get<0>(t) << "," << get<1>(t) << "," << get<2>(t) << "," << get<3>(t) << ")"; return os; }
template <class T> inline istream& operator>> (istream& is, vector<T>& v) { repea(x, v) is >> x; return is; }
template <class T> inline ostream& operator<< (ostream& os, const vector<T>& v) { repe(x, v) os << x << " "; return os; }
template <class T> inline ostream& operator<< (ostream& os, const list<T>& v) { repe(x, v) os << x << " "; return os; }
template <class T> inline ostream& operator<< (ostream& os, const set<T>& s) { repe(x, s) os << x << " "; return os; }
template <class T> inline ostream& operator<< (ostream& os, const set<T, greater<T>>& s) { repe(x, s) os << x << " "; return os; }
template <class T> inline ostream& operator<< (ostream& os, const unordered_set<T>& s) { repe(x, s) os << x << " "; return os; }
template <class T> inline ostream& operator<< (ostream& os, const multiset<T>& s) { repe(x, s) os << x << " "; return os; }
template <class T, class U> inline ostream& operator<< (ostream& os, const map<T, U>& m) { repe(p, m) os << p << " "; return os; }
template <class T, class U> inline ostream& operator<< (ostream& os, const unordered_map<T, U>& m) { repe(p, m) os << p << " "; return os; }
template <class T> inline ostream& operator<< (ostream& os, stack<T> s) { while (!s.empty()) { os << s.top() << " "; s.pop(); } return os; }
template <class T> inline ostream& operator<< (ostream& os, queue<T> q) { while (!q.empty()) { os << q.front() << " "; q.pop(); } return os; }
template <class T> inline ostream& operator<< (ostream& os, deque<T> q) { while (!q.empty()) { os << q.front() << " "; q.pop_front(); } return os; }
template <class T> inline ostream& operator<< (ostream& os, priority_queue<T> q) { while (!q.empty()) { os << q.top() << " "; q.pop(); } return os; }
template <class T> inline ostream& operator<< (ostream& os, priority_queue_rev<T> q) { while (!q.empty()) { os << q.top() << " "; q.pop(); } return os; }
template <class T> inline vector<T>& operator--(vector<T>& v) { rep(_i_, sz(v)) --v[_i_]; return v; }
template <class T> inline vector<T>& operator++(vector<T>& v) { rep(_i_, sz(v)) ++v[_i_]; return v; }

// 手元環境（Visual Studio）
#ifdef _MSC_VER
#define popcount (int)__popcnt // 全ビット中の 1 の個数
#define popcountll (int)__popcnt64
inline int lsb(unsigned int n) { unsigned long i; _BitScanForward(&i, n); return i; } // 最下位ビットの位置（0-indexed）
inline int lsbll(unsigned long long n) { unsigned long i; _BitScanForward64(&i, n); return i; }
inline int msb(unsigned int n) { unsigned long i; _BitScanReverse(&i, n); return i; } // 最上位ビットの位置（0-indexed）
inline int msbll(unsigned long long n) { unsigned long i; _BitScanReverse64(&i, n); return i; }
template <class T> T gcd(T a, T b) { return b ? gcd(b, a % b) : a; }
#define dump(x) cout << "\033[1;36m" << (x) << "\033[0m" << endl;
#define dumps(x) cout << "\033[1;36m" << (x) << "\033[0m ";
#define dumpel(a) { int _i_ = -1; cout << "\033[1;36m"; repe(x, a) {cout << ++_i_ << ": " << x << endl;} cout << "\033[0m"; }
#define input_from_file(f) ifstream isTMP(f); cin.rdbuf(isTMP.rdbuf());
#define output_to_file(f) ofstream osTMP(f); cout.rdbuf(osTMP.rdbuf());
// 提出用（gcc）
#else
#define popcount (int)__builtin_popcount
#define popcountll (int)__builtin_popcountll
#define lsb __builtin_ctz
#define lsbll __builtin_ctzll
#define msb(n) (31 - __builtin_clz(n))
#define msbll(n) (63 - __builtin_clzll(n))
#define gcd __gcd
#define dump(x)
#define dumps(x)
#define dumpel(v)
#define input_from_file(f)
#define output_to_file(f)
#endif

#endif // 折りたたみ用


////-----------------AtCoder 専用-----------------
//#include <atcoder/all>
//using namespace atcoder;
//
////using mint = modint1000000007;
//using mint = modint998244353;
////using mint = modint; // mint::set_mod(m);
//
//istream& operator>> (istream& is, mint& x) { ll x_; is >> x_; x = x_; return is; }
//ostream& operator<< (ostream& os, const mint& x) { os << x.val(); return os; }
//using vm = vector<mint>;	using vvm = vector<vm>;		using vvvm = vector<vvm>;
//
//template <class S, S(*op)(S, S), S(*e)()>ostream& operator<<(ostream& os, segtree<S, op, e> seg) { int n = seg.max_right(0, [](S x) {return true; }); rep(i, n) os << seg.get(i) << " "; return os; }
//template <class S, S(*op)(S, S), S(*e)(), class F, S(*mp)(F, S), F(*cp)(F, F), F(*id)()>ostream& operator<<(ostream& os, lazy_segtree<S, op, e, F, mp, cp, id> seg) { int n = seg.max_right(0, [](S x) {return true; }); rep(i, n) os << seg.get(i) << " "; return os; }
//ostream& operator<<(ostream& os, dsu d) { repe(g, d.groups()) { repe(v, g) { os << v << " "; } os << endl; } return os; };
////----------------------------------------------


void solve() {
	int n;
	cin >> n;

	
}

int main() {
	//	input_from_file("input.txt");
	//	output_to_file("output.txt");

	int t;
	cin >> t;

	while (t--) {
		dump("------------------------------");
		solve();
	}
}
