#ifndef HIDDEN_IN_VISUAL_STUDIO // 折りたたみ用

// 警告の抑制
#define _CRT_SECURE_NO_WARNINGS

// 使えるライブラリの読み込み
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
const ll INFL = (ll)2e18;	const int INF = (int)1e9;
const double EPS = 1e-10; // 許容誤差に応じて調整

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

// 汎用関数の定義
template <class T> inline ll pow(T n, int k) { ll v = 1; rep(i, k) v *= n; return v; }
template <class T> inline bool chmax(T& M, const T& x) { if (M < x) { M = x; return true; } return false; } // 最大値を更新（更新されたら true を返す）
template <class T> inline bool chmin(T& m, const T& x) { if (m > x) { m = x; return true; } return false; } // 最小値を更新（更新されたら true を返す）

// 入出力用の >>, << のオーバーロード
template <class T, class U> inline istream& operator>> (istream& is, pair<T, U>& p) { is >> p.first >> p.second; return is; }
template <class T, class U> inline ostream& operator<< (ostream& os, const pair<T, U>& p) { os << "(" << p.first << "," << p.second << ")"; return os; }
template <class T, class U, class V> inline istream& operator>> (istream& is, tuple<T, U, V>& t) { is >> get<0>(t) >> get<1>(t) >> get<2>(t); return is; }
template <class T, class U, class V> inline ostream& operator<< (ostream& os, const tuple<T, U, V>& t) { os << "(" << get<0>(t) << "," << get<1>(t) << "," << get<2>(t) << ")"; return os; }
template <class T, class U, class V, class W> inline istream& operator>> (istream& is, tuple<T, U, V, W>& t) { is >> get<0>(t) >> get<1>(t) >> get<2>(t) >> get<3>(t); return is; }
template <class T, class U, class V, class W> inline ostream& operator<< (ostream& os, const tuple<T, U, V, W>& t) { os << "(" << get<0>(t) << "," << get<1>(t) << "," << get<2>(t) << "," << get<3>(t) << ")"; return os; }
template <class T> inline istream& operator>> (istream& is, vector<T>& v) { repea(x, v) is >> x; return is; }
template <class T> inline ostream& operator<< (ostream& os, const vector<T>& v) { repe(x, v) os << x << " "; return os; }
template <class T> inline ostream& operator<< (ostream& os, const set<T>& s) { repe(x, s) os << x << " "; return os; }
template <class T> inline ostream& operator<< (ostream& os, const unordered_set<T>& s) { repe(x, s) os << x << " "; return os; }
template <class T, class U> inline ostream& operator<< (ostream& os, const map<T, U>& m) { repe(p, m) os << p << " "; return os; }
template <class T, class U> inline ostream& operator<< (ostream& os, const unordered_map<T, U>& m) { repe(p, m) os << p << " "; return os; }
template <class T> inline ostream& operator<< (ostream& os, stack<T> s) { while (!s.empty()) { os << s.top() << " "; s.pop(); } return os; }
template <class T> inline ostream& operator<< (ostream& os, queue<T> q) { while (!q.empty()) { os << q.front() << " "; q.pop(); } return os; }
template <class T> inline ostream& operator<< (ostream& os, deque<T> q) { while (!q.empty()) { os << q.front() << " "; q.pop_front(); } return os; }
template <class T> inline ostream& operator<< (ostream& os, priority_queue<T> q) { while (!q.empty()) { os << q.top() << " "; q.pop(); } return os; }

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
#define dumpel(a) { int i = 0; cout << "\033[1;36m"; repe(x, a) {cout << i++ << ": " << x << endl;} cout << "\033[0m"; }
#define input_from_file(f) ifstream isTMP(f); cin.rdbuf(isTMP.rdbuf());
#define output_to_file(f) ofstream osTMP(f); cout.rdbuf(osTMP.rdbuf());
// 提出用（GCC）
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


//【形式的冪級数（mint）】
/*
* FPS() : O(1)
*	零多項式 f = 0 で初期化する．
*
* FPS(c0) : O(1)
*	定数多項式 f = c0 で初期化する．
*
* FPS(c0, d) : O(d)
*	d 次未満の項をもつ定数多項式 f = c0 で初期化する．
*
* FPS(c) : O(|c|)
*	f(x) = c[0] + c[1] x + ... + c[n - 1] x^(n-1) で初期化する．
*
* c + f, f + c : O(1)	f + g : O(n)
* f - c : O(1)			c - f, f - g, -f : O(n)
* c * f, f * c : O(n)	f * g : O(n^2)		f * g_sp : O(n k)（k : g の項数）
* f / c : O(n)			f / g : O(n^2)		f / g_sp : O(n k)（k : g の項数）
*	形式的冪級数としての和，差，積，商の結果を返す．
*	g_sp はスパース多項式であり，{次数, 係数} の次数昇順の組の vector で表す．
*	制約 : 商では g(0) ≠ 0
*
* f.inv(d) : O(n^2)
*	1 / f mod x^d を返す．
*	制約 : f(0) ≠ 0
*
* f.quotient(g) : O(n^2)
* f.reminder(g) : O(n^2)
* f.quotient_remainder(g) : O(n^2)
*	多項式としての f を g で割った商，余り，商と余りの組を返す．
*
* f.pow(k, d) : O(n^2)
*	f(x)^k mod x^d を返す．
*
* f.deg(), f.size() : O(1)
*	多項式 f の次数[+1]を返す．
*
* monomial(d) : O(d)
*	単項式 x^d を返す．
*
* f.assign(v) : O(n)
*	多項式 f の不定元 x に v を代入した値を返す．
*
* f.resize() : O(n)
*	不要な高次の項を削る．
*
* f.resize(d) : O(1)
*	mod x^d をとる．
*
* f >> d, f << d : O(n)
*	係数列を d だけ右[左]シフトした多項式を返す．
*  （右シフトは x^d の乗算，左シフトは x^d で割った商と等価）
*
* power_mod(f, d, g) : O(m^2 log d)　（m = deg g）
*	f(x)^d % g(x) を返す．
*
* derivative(f) : O(n)
*	f'(x) を返す．
*
* integral(f) : O(n)
*	∫ f(x) dx を返す．（定数項は 0 とする）
*
* log(f, d) : O(n^2)
*	log f(x) mod x^d を返す．
*	制約 : f(0) = 1
*
* exp(f, d) : O(n^2)
*	exp f(x) mod x^d を返す．
*	制約 : f(0) = 0;
*/
struct FPS {
	using vm = vector<mint>;
	using SFPS = vector<pair<int, mint>>;

	int n; // 係数の個数（次数 + 1）
	vm c; // 係数列

	// コンストラクタ（0，定数，係数列で初期化）
	FPS() : n(0) {}
	FPS(const mint& c0) : n(1), c({ c0 }) {}
	FPS(const int& c0) : n(1), c({ mint(c0) }) {}
	FPS(const mint& c0, int d) : n(d), c(n) { c[0] = c0; }
	FPS(const int& c0, int d) : n(d), c(n) { c[0] = c0; }
	FPS(const vm& c_) : n(sz(c_)), c(c_) {}
	FPS(const vi& c_) : n(sz(c_)), c(n) { rep(i, n) c[i] = c_[i]; }

	// 代入
	FPS(const FPS& f) = default;
	FPS& operator=(const FPS& f) = default;
	FPS& operator=(const mint& c0) { n = 1; c = { c0 }; return *this; }

	// アクセス
	mint const& operator[](int i) const { return c[i]; }
	mint& operator[](int i) { return c[i]; }

	// 次数
	int deg() const { return n - 1; }
	int size() const { return n; }

	// 加算
	FPS& operator+=(const FPS& g) {
		if (n >= g.n) rep(i, g.n) c[i] += g.c[i];
		else {
			rep(i, n) c[i] += g.c[i];
			repi(i, n, g.n - 1)	c.push_back(g.c[i]);
			n = g.n;
		}
		return *this;
	}
	FPS operator+(const FPS& g) const { return FPS(*this) += g; }

	// 定数加算
	FPS& operator+=(const mint& sc) {
		if (n == 0) { n = 1; c = { sc }; }
		else { c[0] += sc; }
		return *this;
	}
	FPS operator+(const mint& sc) const { return FPS(*this) += sc; }
	friend FPS operator+(const mint& sc, const FPS& f) { return f + sc; }
	FPS& operator+=(const int& sc) { *this += mint(sc); return *this; }
	FPS operator+(const int& sc) const { return FPS(*this) += sc; }
	friend FPS operator+(const int& sc, const FPS& f) { return f + sc; }

	// 減算
	FPS& operator-=(const FPS& g) {
		if (n >= g.n) rep(i, g.n) c[i] -= g.c[i];
		else {
			rep(i, n) c[i] -= g.c[i];
			repi(i, n, g.n - 1) c.push_back(-g.c[i]);
			n = g.n;
		}
		return *this;
	}
	FPS operator-(const FPS& g) const { return FPS(*this) -= g; }

	// 定数減算
	FPS& operator-=(const mint& sc) { *this += -sc; return *this; }
	FPS operator-(const mint& sc) const { return FPS(*this) -= sc; }
	friend FPS operator-(const mint& sc, const FPS& f) { return -(f - sc); }
	FPS& operator-=(const int& sc) { *this += -sc; return *this; }
	FPS operator-(const int& sc) const { return FPS(*this) -= sc; }
	friend FPS operator-(const int& sc, const FPS& f) { return -(f - sc); }

	// 加法逆元
	FPS operator-() const { return FPS(*this) *= -1; }

	// 定数倍
	FPS& operator*=(const mint& sc) { rep(i, n) c[i] *= sc; return *this; }
	FPS operator*(const mint& sc) const { return FPS(*this) *= sc; }
	friend FPS operator*(const mint& sc, const FPS& f) { return f * sc; }
	FPS& operator*=(const int& sc) { *this *= mint(sc); return *this; }
	FPS operator*(const int& sc) const { return FPS(*this) *= sc; }
	friend FPS operator*(const int& sc, const FPS& f) { return f * sc; }

	// 右からの定数除算
	FPS& operator/=(const mint& sc) { *this *= sc.inv(); return *this; }
	FPS operator/(const mint& sc) const { return FPS(*this) /= sc; }
	FPS& operator/=(const int& sc) { *this /= mint(sc); return *this; }
	FPS operator/(const int& sc) const { return FPS(*this) /= sc; }

	// 積
	FPS& operator*=(const FPS& g) {
		int m = g.deg();
		resize(n + m);

		// 後ろからインライン配る DP
		repir(i, n - 1, 0) {
			// 上位項に係数倍して配っていく．
			repi(j, 1, m) {
				
				if (i + j >= n) break;

				c[(ll)i + j] += c[i] * g[j];
			}

			// 定数項は最後に配るか消去しないといけない．
			c[i] *= g[0];
		}

		return *this;
	}
	FPS operator*(const FPS& g) const { return FPS(*this) *= g; }

	// 除算
	FPS inv(int d) const {
		// 参考：https://nyaannyaan.github.io/library/fps/formal-power-series.hpp

		//【方法】
		// 1 / f mod x^d を求めることは，
		//		f g = 1 (mod x^d)
		// なる g を求めることである．
		// この d の部分を 1, 2, 4, ..., 2^i と倍々にして求めていく．
		//
		// d = 1 のときについては
		//		g = 1 / f[0] (mod x^1)
		// である．
		//
		// 次に，
		//		g = h (mod x^k)
		// が求まっているとして
		//		g mod x^(2 k)
		// を求める．最初の式を変形していくことで
		//		g - h = 0 (mod x^k)
		//		⇒ (g - h)^2 = 0 (mod x^(2 k))
		//		⇔ g^2 - 2 g h + h^2 = 0 (mod x^(2 k))
		//		⇒ f g^2 - 2 f g h + f h^2 = 0 (mod x^(2 k))
		//		⇔ g - 2 h + f h^2 = 0 (mod x^(2 k)) 　(f g = 1 (mod x^d) より)
		//		⇔ g = (2 - f h) h (mod x^(2 k))
		// を得る．
		//
		// この手順を d <= 2^i となる i まで繰り返し，d 次以上の項を削除すればよい．

		FPS g(c[0].inv());
		for (int k = 1; k < d; k *= 2) {
			g = (2 - *this * g) * g;
			g.resize(2 * k);
		}

		return g.resize(d);
	}
	FPS& operator/=(const FPS& g) { return *this *= g.inv(n); }
	FPS operator/(const FPS& g) const { return FPS(*this) /= g; }

	// 余り付き除算
	FPS quotient(const FPS& g) const {
		// 参考 : https://nyaannyaan.github.io/library/fps/formal-power-series.hpp

		//【方法】
		// f(x) = g(x) q(x) + r(x) となる q(x) を求める．
		// f の次数は n - 1, g の次数は m - 1 とする．(n >= m)
		// 従って q の次数は n - m，r の次数は m - 2 となる．
		// 
		// f^R で f の係数列を逆順にした多項式を表す．すなわち
		//		f^R(x) := f(1/x) x^(n-1)
		// である．他の多項式も同様とする．
		//
		// 最初の式で x → 1/x と置き換えると，
		//		f(1/x) = g(1/x) q(1/x) + r(1/x)
		//		⇔ f(1/x) x^(n-1) = g(1/x) q(1/x) x^(n-1) + r(1/x) x^(n-1)
		//		⇔ f(1/x) x^(n-1) = g(1/x) x^(m-1) q(1/x) x^(n-m) + r(1/x) x^(m-2) x^(n-m+1)
		//		⇔ f^R(x) = g^R(x) q^R(x) + r^R(x) x^(n-m+1)
		//		⇒ f^R(x) = g^R(x) q^R(x) (mod x^(n-m+1))
		// 	    ⇒ q^R(x) = f^R(x) / g^R(x)  (mod x^(n-m+1))
		// を得る．
		// 	   
		// これで q を mod x^(n-m+1) で正しく求めることができることになるが，
		// q の次数は n - m であったから，q 自身を正しく求めることができた．

		if (n < g.n) return FPS();
		return ((this->rev() / g.rev()).resize(n - g.n + 1)).rev();
	}
	FPS reminder(const FPS& g) const { return (*this - this->quotient(g) * g).resize(g.n - 1); }
	pair<FPS, FPS> quotient_remainder(const FPS& g) const {
		pair<FPS, FPS> res;
		res.first = this->quotient(g);
		res.second = (*this - res.first * g).resize(g.n - 1);
		return res;
	}

	// スパース積
	FPS& operator*=(const SFPS& g) {
		// g の定数項だけ例外処理
		auto it0 = g.begin();
		mint g0 = 0;
		if (it0->first == 0) {
			g0 = it0->second;
			it0++;
		}

		// 後ろからインライン配る DP
		repir(i, n - 1, 0) {
			// 上位項に係数倍して配っていく．
			for (auto it = it0; it != g.end(); it++) {
				int j; mint gj;
				tie(j, gj) = *it;

				if (i + j >= n) break;

				c[(ll)i + j] += c[i] * gj;
			}

			// 定数項は最後に配るか消去しないといけない．
			c[i] *= g0;
		}

		return *this;
	}
	FPS operator*(const SFPS& g) const { return FPS(*this) *= g; }

	// スパース商
	FPS& operator/=(const SFPS& g) {
		// g の定数項だけ例外処理
		auto it0 = g.begin();
		assert(it0->first == 0 && it0->second != 0);
		mint g0_inv = it0->second.inv();
		it0++;

		// 前からインライン配る DP（後ろに累積効果あり）
		rep(i, n) {

			// 定数項は最初に配らないといけない．
			c[i] *= g0_inv;

			// 上位項に係数倍して配っていく．
			for (auto it = it0; it != g.end(); it++) {
				int j; mint gj;
				tie(j, gj) = *it;

				if (i + j >= n) break;

				c[(ll)i + j] -= c[i] * gj;
			}
		}

		return *this;
	}
	FPS operator/(const SFPS& g) const { return FPS(*this) /= g; }

	// 係数反転
	FPS rev() const { FPS h = *this; reverse(all(h.c)); return h; }

	// 不要な高次項の除去
	FPS& resize() {
		// 最高次の係数が非 0 になるまで削る．
		while (n > 0 && c[n - 1LL] == 0) {
			c.pop_back();
			n--;
		}
		return *this;
	}

	// 高次項の除去
	FPS& resize(int d) {
		// x^d 以上の項を除去する．
		n = d;
		c.resize(d);
		return *this;
	}

	// 不定元への代入
	mint assign(const mint& x) const {
		mint val = 0;
		repir(i, n - 1, 0) val = val * x + c[i];
		return val;
	}

	// 係数のシフト
	FPS& operator>>=(int d) {
		n += d;
		c.insert(c.begin(), d, 0);
		return *this;
	}
	FPS& operator<<=(int d) {
		n -= d;
		if (n <= 0) { c.clear(); n = 0; }
		else c.erase(c.begin(), c.begin() + d);
		return *this;
	}
	FPS operator>>(int d) const { return FPS(*this) >>= d; }
	FPS operator<<(int d) const { return FPS(*this) <<= d; }

	// 累乗の剰余
	friend FPS power_mod(const FPS& f, ll d, const FPS& g) {
		FPS res(1), pow2(f);
		while (d > 0) {
			if (d & 1) res = (res * pow2).reminder(g);
			pow2 = (pow2 * pow2).reminder(g);
			d /= 2;
		}
		return res;
	}

	// 微分
	friend FPS derivative(const FPS& f) {
		FPS res;
		repi(i, 1, f.n - 1) res.c.push_back(f[i] * i);
		res.n = sz(res.c);
		return res;
	}

	// 不定積分
	friend FPS integral(const FPS& f) {
		FPS res(0);
		repi(i, 0, f.n - 1) res.c.push_back(f[i] / (i + 1));
		res.n = sz(res.c);
		return res;
	}

	// 対数関数
	friend FPS log(const FPS& f, int d) {
		// 参考 : https://qiita.com/hotman78/items/f0e6d2265badd84d429a

		return integral((derivative(f) * f.inv(d - 1)).resize(d - 1));
	}

	// 指数関数
	friend FPS exp(const FPS& f, int d) {
		// 参考 : https://qiita.com/hotman78/items/f0e6d2265badd84d429a

		//【方法】
		// g(x) = exp(f(x)) とおき，方程式
		//		log g(x) = f(x)
		// に対してニュートン法を用いる．
		// 
		// f(0) = 0 なので，mod x^1 では
		//		log(1) ≡ f(x) mod x^1
		// が成り立つ．
		//
		// mod x^k で
		//		log h(x) ≡ f(x) mod x^k
		// が成り立っていると仮定すると，ニュートン法より
		//		g = h - (log h - f) / (log h)'
		//   ⇔ g = h (f + 1 - log h)
		// と置くと
		//		log g(x) ≡ f(x) mod x^(2 k)
		// が成り立つ．
		//
		// これを繰り返せば所望の g が求まる．

		// ニュートン法で log g = f なる g を見つける．
		FPS g(1);
		for (int k = 1; k < d; k *= 2) {
			g = g * (f + 1 - log(g, 2 * k));
			g.resize(2 * k);
		}

		return g;
	}

	// 累乗
	FPS pow(ll k, int d) const {
		// 参考 : https://qiita.com/hotman78/items/f0e6d2265badd84d429a

		// 最低次の項を見つける．
		int i0 = 0;
		while (i0 < n && c[i0] == 0) i0++;

		// f = 0 なら f^k = 0 である．
		if (i0 == n) return FPS(0, d);

		// 最低次の項の係数を記録する．
		mint c0 = c[i0];

		// 定数項が 1 になるようシフトかつ定数除算した多項式を得る．
		FPS fs = (*this << i0) / c0;
		ll ds = d - k * i0;

		// 最終的に k * i0 次以上の項しか残らないことに注意し，0 になるケースを処理する．
		if (ds <= 0) return FPS(0, d);

		// f^k = exp(k log f(x)) を用いて f^k を計算する．
		FPS gs = exp(mint(k) * log(fs, (int)ds), (int)ds);

		// シフトと定数除算した分を元に戻す．
		FPS g = (gs * c0.pow(k)) >> ((int)k * i0);

		return g;
	}

	// デバッグ出力
	friend ostream& operator<<(ostream& os, const FPS& f) {
		if (f.n == 0) os << 0;
		else {
			rep(i, f.n) {
				os << f.c[i] << "x^" << i;
				if (i < f.n - 1) os << " + ";
			}
		}
		return os;
	}
};


//【展開係数／ボスタン－森法】O(n log n log d)
/*
* 有理式 f(x) / g(x) を形式的冪級数に展開したときの x^d の係数を返す．
*
* 制約 : deg f < deg g,
*/
mint coef(const FPS& f, const FPS& g, ll d) {
	// 参考 : http://q.c.titech.ac.jp/docs/progs/polynomial_division.html

	//【方法】
	// 分母分子に g(-x) を掛けることにより
	//		f(x) / g(x) = f(x) g(-x) / g(x) g(-x)
	// を得る．ここで g(x) g(-x) は偶多項式なので
	//		g(x) g(-x) = e(x^2)
	// と表すことができる．
	// 
	// 分子について
	//		f(x) g(-x) = E(x^2) + x O(x^2)
	// というように偶多項式部分と奇多項式部分に分けると，d が偶数のときは
	//		[x^d] f(x) g(-x) / g(x) g(-x)
	//		= [x^d] E(x^2) / e(x^2)
	//		= [x^(d/2)] E(x) / e(x)
	// となり，d が奇数のときは
	//		[x^d] f(x) g(-x) / g(x) g(-x)
	//		= [x^d] x O(x^2) / e(x^2)
	//		= [x^((d-1)/2)] O(x) / e(x)
	// となる．
	//
	// これを繰り返せば d を半分ずつに減らしていくことができる．

	// d = 0 のときは定数項を返す．
	if (d == 0) {
		return f[0] / g[0];
	}

	// f2(x) = f(x) g(-x), g2(x) = g(x) g(-x) を求める．
	FPS f2, g2 = g;
	rep(i, g2.n) {
		if (i % 2) {
			g2.c[i] *= -1;
		}
	}
	f2 = f * g2;
	g2 *= g;

	// f3(x) = E(x) or O(x), g3(x) = e(x) を求める．
	FPS f3, g3;
	if (d % 2 == 0) {
		for (int i = 0; 2 * i < f2.n; i++) {
			f3.c.push_back(f2.c[2 * i]);
		}
	}
	else {
		for (int i = 0; 2 * i + 1 < f2.n; i++) {
			f3.c.push_back(f2.c[2 * i + 1]);
		}
	}
	f3.n = sz(f3.c);
	rep(i, g.n) {
		g3.c.push_back(g2.c[2 * i]);
	}
	g3.n = sz(g3.c);

	// d を半分にして再帰を回す．
	return coef(f3, g3, d / 2);
}


int main() {
	cout << fixed << setprecision(12);
//	input_from_file("input.txt");
//	output_to_file("output.txt");

	int k, n;
	cin >> k >> n;

	FPS f(0, k + 1);
	f[1] = 1;
	repi(i, 3, k) f[i] = -(i - 2);
	dump(f);

	FPS g(0, k + 1);
	g[0] = 1;
	repi(i, 1, k) g[i] = -1;
	dump(g);

	cout << coef(f, g, n) << endl;
}
