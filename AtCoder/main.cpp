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
const int INF = 1001001001; const ll INFL = 4004004004004004004LL;
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
template <class T> inline ostream& operator<< (ostream& os, const set<T>& s) { repe(x, s) os << x << " "; return os; }
template <class T> inline ostream& operator<< (ostream& os, const set<T, greater<T>>& s) { repe(x, s) os << x << " "; return os; }
template <class T> inline ostream& operator<< (ostream& os, const unordered_set<T>& s) { repe(x, s) os << x << " "; return os; }
template <class T, class U> inline ostream& operator<< (ostream& os, const map<T, U>& m) { repe(p, m) os << p << " "; return os; }
template <class T, class U> inline ostream& operator<< (ostream& os, const unordered_map<T, U>& m) { repe(p, m) os << p << " "; return os; }
template <class T> inline ostream& operator<< (ostream& os, stack<T> s) { while (!s.empty()) { os << s.top() << " "; s.pop(); } return os; }
template <class T> inline ostream& operator<< (ostream& os, queue<T> q) { while (!q.empty()) { os << q.front() << " "; q.pop(); } return os; }
template <class T> inline ostream& operator<< (ostream& os, deque<T> q) { while (!q.empty()) { os << q.front() << " "; q.pop_front(); } return os; }
template <class T> inline ostream& operator<< (ostream& os, priority_queue<T> q) { while (!q.empty()) { os << q.top() << " "; q.pop(); } return os; }
template <class T> inline ostream& operator<< (ostream& os, priority_queue_rev<T> q) { while (!q.empty()) { os << q.top() << " "; q.pop(); } return os; }
template <class T> inline vector<T>& operator--(vector<T>& v) { rep(i, sz(v)) --v[i]; return v; }

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
#define dumpel(a) { int i = -1; cout << "\033[1;36m"; repe(x, a) {cout << ++i << ": " << x << endl;} cout << "\033[0m"; }
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


//-----------------AtCoder 専用-----------------
#include <atcoder/all>
using namespace atcoder;

using mint = modint1000000007;
//using mint = modint998244353;
//using mint = modint; // mint::set_mod(m);

template <class S, S(*op)(S, S), S(*e)()>ostream& operator<<(ostream& os, segtree<S, op, e> seg) { int n = seg.max_right(0, [](S x) {return true; }); rep(i, n) os << seg.get(i) << " "; return os; }
template <class S, S(*op)(S, S), S(*e)(), class F, S(*mp)(F, S), F(*cp)(F, F), F(*id)()>ostream& operator<<(ostream& os, lazy_segtree<S, op, e, F, mp, cp, id> seg) { int n = seg.max_right(0, [](S x) {return true; }); rep(i, n) os << seg.get(i) << " "; return os; }
istream& operator>> (istream& is, mint& x) { ll x_; is >> x_; x = x_; return is; }
ostream& operator<< (ostream& os, const mint& x) { os << x.val(); return os; }
using vm = vector<mint>;	using vvm = vector<vm>;		using vvvm = vector<vvm>;
//----------------------------------------------


//【素因数分解／試し割り法】O(√n)
/*
* n を素因数分解した結果を pps に格納する．
*
* pps[p] = d : n に素因数 p が d 個含まれていることを表す．
*/
void factor_integer(ll n, map<ll, int>& pps) {
	// verify : https://algo-method.com/tasks/457

	pps.clear();

	for (ll i = 2; i * i <= n; i++) {
		int d = 0;
		while (n % i == 0) {
			d++;
			n /= i;
		}
		if (d > 0) pps[i] = d;
	}
	if (n > 1) pps[n] = 1;
}


//【互いに素な数の個数】O(√a + 2^m)（m : a の素因数の種類数）
/*
* l 以上 r 以下の整数のうち、a と互いに素な数の個数を返す．
*
*（状態系包除原理）
*
* 利用：【素因数分解】
*/
ll count_coprime(ll a, ll l, ll r) {
	// a と互いに素 ⇔ a の各素因数で割り切れない，なので a を素因数分解する．
	map<ll, int> pps;
	factor_integer(a, pps);

	// a の素因数だけのリスト p を作る．（個数は使わない）
	vl p;
	repe(s, pps) p.push_back(s.first);
	int m = sz(p);

	// 包除原理を用いて数え上げる．
	// 例えば，6 と互いに素な数の個数は，
	//		1 の倍数を全て数え，そこから 2 の倍数の個数を引き，
	//		さらに 3 の倍数の個数を引き，引きすぎた 6 の倍数の個数を足す
	// ことにより数えることができる．
	ll res = 0;
	rep(bit, 1 << m) {
		// mul の倍数を考える．
		ll mul = 1;

		// mul が何個の素因数の積か．
		int ones = 0;

		rep(i, m) {
			if (bit & (1 << i)) {
				mul *= p[i];
				ones++;
			}
		}

		// 素因数の個数の偶奇で加減を切り替えつつ個数を数えていく．
		res += ((ones % 2) ? -1 : 1) * (r / mul - (l - 1) / mul);
	}
	return res;
}


// x を固定して y を数えることを繰り返す．
// x を 1 個ずつ素因数分解してるので TLE しそうだが 1900ms くらいでギリギリ AC する．
void AC1() {
	ll l, r;
	cin >> l >> r;

	ll res = 0;
	repi(x, l, r) {
		res += (r - x) - (count_coprime(x, x + 1, r));
		if (x > 1) {
			res -= r / x - 1;
		}
	}
	res *= 2;

	cout << res << endl;
}


//【一括素因数分解】O(n log(log n))
/*
* n 以下の自然数 i の素因数分解を pps[i] に格納する．（pps[0] は使わない）
*
*（エラトステネスの篩）
*/
void factor_integer_all(int n, vector<map<int, int>>& pps) {
	pps = vector<map<int, int>>(n + 1);

	// 順に素数で割っていった残りの値を記録しておくためのテーブル
	vi a(n + 1);
	iota(all(a), 0);

	int p = 2;

	// √n 以下の p の処理（continue されない場合は p は素数）
	for (; p * p <= n; p++) {
		if (a[p] == 1) continue;

		for (int i = p; i <= n; i += p) {
			while (a[i] % p == 0) {
				pps[i][p]++;
				a[i] /= p;
			}
		}
	}

	// √n より大きい p の処理（この p は素数とは限らないので注意）
	for (; p <= n; p++) {
		if (a[p] != 1) pps[p][a[p]]++;
	}
}


//【添字非倍数の和】O(2^n n O(mf))
/*
* 互いに異なる素数 ps[0..n) のいずれの倍数でもない数の集合を S とし，Σi∈S f(i) を返す．
* ただし mf(d) := Σd|i f(i) とする．
* 
* ps を数 a の素因数のリストとすれば，i は a と互いに素な数を走査する．
*
*（状態系包除原理）
*/
template <class T> T nonmultiple_sum(vl& ps, function<T(ll)>& mf) {
	int n = sz(ps);

	T res = 0;
	
	// 包除原理を用いて計算する．
	// 例えば ps[0..2) = (2, 3) のとき，
	//		Σi∈S f(i) = Σ1|i f(i) - Σ2|i f(i) - Σ3|i f(i) + Σ6|i f(i)
	// となる．
	repb(set, n) {
		ll d = 1; // d の倍数を考える．
		int sign = 1; // sign : 符号因子

		rep(i, n) {
			if (set & (1 << i)) {
				// オーバーフロー対策
				if (d > INFL / ps[i]) goto NEXT_LOOP;

				d *= ps[i];
				sign *= -1;
			}
		}

		res += sign * mf(d);

	NEXT_LOOP:;
	}

	return res;
}


// AC1() と考え方は同じだが，素因数分解を一括で行うことにした．
// 実行時間は 1000ms ほどまで改善された．
void AC2() {
	int l, r;
	cin >> l >> r;

	vector<map<int, int>> pps;
	factor_integer_all(r, pps);

	ll res = 0;
	repi(x, l, r) {
		vl ps;
		repe(pp, pps[x]) ps.push_back(pp.first);
		function<ll(ll)> mf = [&](ll d) { return r / d - x / d; };
		res += (r - x) - nonmultiple_sum(ps, mf);
		if (x > 1) {
			res -= r / x - 1;
		}
	}
	res *= 2;

	cout << res << endl;
}


int main() {
//	input_from_file("input.txt");
//	output_to_file("output.txt");

	int l, r;
	cin >> l >> r;

	vector<map<int, int>> pps;
	factor_integer_all(r, pps);

	ll res = 0;
	repi(x, l, r) {
		vl ps;
		repe(pp, pps[x]) ps.push_back(pp.first);
		function<ll(ll)> mf = [&](ll d) { return r / d - x / d; };
		res += (r - x) - nonmultiple_sum(ps, mf);
		if (x > 1) {
			res -= r / x - 1;
		}
	}
	res *= 2;

	cout << res << endl;
}
