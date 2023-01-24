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
#define dump_list2D(v)
#define input_from_file(f)
#define output_to_file(f)
#define Assert(b) { if (!(b)) while (1) cout << "OLE"; }
#endif

#endif // 折りたたみ用


#if __has_include(<atcoder/all>)
#include <atcoder/all>
using namespace atcoder;

using mint = modint1000000007;
//using mint = modint998244353;
//using mint = modint; // mint::set_mod(m);
//using mint = static_modint<1234567891>;

istream& operator>>(istream& is, mint& x) { ll x_; is >> x_; x = x_; return is; }
ostream& operator<<(ostream& os, const mint& x) { os << x.val(); return os; }
using vm = vector<mint>; using vvm = vector<vm>; using vvvm = vector<vvm>;
#endif


//【畳込み（法が任意）】O((n + m) log (n + m))
/*
* a と b の mod を法とした畳込みを返す．
*/
vm convolution_arbitrary_mod(const vm& a, const vm& b) {
	int n = sz(a), m = sz(b);
	int mod = mint::mod();

	vl a0(n), a1(n), b0(m), b1(m); const int pow2 = 1 << 15;
	rep(i, n) {
		int ai = a[i].val();
		a0[i] = ai % pow2;
		a1[i] = ai / pow2;
	}
	rep(i, m) {
		int bi = b[i].val();
		b0[i] = bi % pow2;
		b1[i] = bi / pow2;
	}

	vl c00 = convolution_ll(a0, b0);
	vl c11 = convolution_ll(a1, b1);
	rep(i, n) a0[i] += a1[i];
	rep(i, m) b0[i] += b1[i];
	vl c01 = convolution_ll(a0, b0);
	rep(i, n + m - 1) {
		c00[i] %= mod;
		c11[i] %= mod;
		c01[i] = (c01[i] - c00[i] - c11[i] + 2LL * mod) % mod;
	}

	vm c(n + m - 1);
	rep(i, n + m - 1) {
		c[i] = c00[i] + c01[i] * pow2 + c11[i] * pow2 * pow2;
	}

	return c;
}


//【形式的冪級数】
/*
* MFPS() : O(1)
*	零多項式 f = 0 で初期化する．
*
* MFPS(mint c0) : O(1)
*	定数多項式 f = c0 で初期化する．
*
* MFPS(mint c0, int n) : O(n)
*	n 次未満の項をもつ定数多項式 f = c0 で初期化する．
*
* MFPS(vm c) : O(n)
*	f(x) = c[0] + c[1] x + ... + c[n - 1] x^(n-1) で初期化する．
* 
* set_conv(vm(*CONV)(const vm&, const vm&)) : O(1)
*	畳込み用の関数を CONV に設定する．
*
* c + f, f + c : O(1)	f + g : O(n)
* f - c : O(1)			c - f, f - g, -f : O(n)
* c * f, f * c : O(n)	f * g : O(n log n)		f * g_sp : O(n k)（k : g の項数）
* f / c : O(n)			f / g : O(n log n)		f / g_sp : O(n k)（k : g の項数）
*	形式的冪級数としての和，差，積，商の結果を返す．
*	g_sp はスパース多項式であり，{次数, 係数} の次数昇順の組の vector で表す．
*	制約 : 商では g(0) != 0
*
* MFPS f.inv(int d) : O(n log n)
*	1 / f mod x^d を返す．
*	制約 : f(0) != 0
*
* MFPS f.quotient(MFPS g) : O(n log n)
* MFPS f.reminder(MFPS g) : O(n log n)
* pair<MFPS, MFPS> f.quotient_remainder(MFPS g) : O(n log n)
*	多項式としての f を g で割った商，余り，商と余りの組を返す．
*	制約 : g の最高次の係数は 0 でない
*
* int f.deg(), int f.size() : O(1)
*	多項式 f の次数[項数]を返す．
*
* MFPS::monomial(int d) : O(d)
*	単項式 x^d を返す．
*
* mint f.assign(mint c) : O(n)
*	多項式 f の不定元 x に c を代入した値を返す．
*
* f.resize(int d) : O(1)
*	mod x^d をとる．
*
* f.resize() : O(n)
*	不要な高次の項を削る．
*
* f >> d, f << d : O(n)
*	係数列を d だけ右[左]シフトした多項式を返す．
*  （右シフトは x^d の乗算，左シフトは x^d で割った商と等価）
*
* MFPS power_mod(MFPS f, ll d, MFPS g) : O(m log m log d)　（m = deg g）
*	f(x)^d mod g(x) を返す．
*/
struct MFPS {
	using SMFPS = vector<pair<int, mint>>;

	int n; // 係数の個数（次数 + 1）
	vm c; // 係数列
	inline static vm(*CONV)(const vm&, const vm&) = convolution; // 畳込み用の関数

	// コンストラクタ（0，定数，係数列で初期化）
	MFPS() : n(0) {}
	MFPS(const mint& c0) : n(1), c({ c0 }) {}
	MFPS(const int& c0) : n(1), c({ mint(c0) }) {}
	MFPS(const mint& c0, int d) : n(d), c(n) { c[0] = c0; }
	MFPS(const int& c0, int d) : n(d), c(n) { c[0] = c0; }
	MFPS(const vm& c_) : n(sz(c_)), c(c_) {}
	MFPS(const vi& c_) : n(sz(c_)), c(n) { rep(i, n) c[i] = c_[i]; }

	// 代入
	MFPS(const MFPS& f) = default;
	MFPS& operator=(const MFPS& f) = default;
	MFPS& operator=(const mint& c0) { n = 1; c = { c0 }; return *this; }

	// 比較
	bool operator==(const MFPS& g) const { return c == g.c; }
	bool operator!=(const MFPS& g) const { return c != g.c; }

	// アクセス
	mint const& operator[](int i) const { return c[i]; }
	mint& operator[](int i) { return c[i]; }

	// 次数
	int deg() const { return n - 1; }
	int size() const { return n; }

	static void set_conv(vm(*CONV_)(const vm&, const vm&)) {
		CONV = CONV_;
	}

	// 加算
	MFPS& operator+=(const MFPS& g) {
		if (n >= g.n) rep(i, g.n) c[i] += g.c[i];
		else {
			rep(i, n) c[i] += g.c[i];
			repi(i, n, g.n - 1)	c.push_back(g.c[i]);
			n = g.n;
		}
		return *this;
	}
	MFPS operator+(const MFPS& g) const { return MFPS(*this) += g; }

	// 定数加算
	MFPS& operator+=(const mint& sc) {
		if (n == 0) { n = 1; c = { sc }; }
		else { c[0] += sc; }
		return *this;
	}
	MFPS operator+(const mint& sc) const { return MFPS(*this) += sc; }
	friend MFPS operator+(const mint& sc, const MFPS& f) { return f + sc; }
	MFPS& operator+=(const int& sc) { *this += mint(sc); return *this; }
	MFPS operator+(const int& sc) const { return MFPS(*this) += sc; }
	friend MFPS operator+(const int& sc, const MFPS& f) { return f + sc; }

	// 減算
	MFPS& operator-=(const MFPS& g) {
		if (n >= g.n) rep(i, g.n) c[i] -= g.c[i];
		else {
			rep(i, n) c[i] -= g.c[i];
			repi(i, n, g.n - 1) c.push_back(-g.c[i]);
			n = g.n;
		}
		return *this;
	}
	MFPS operator-(const MFPS& g) const { return MFPS(*this) -= g; }

	// 定数減算
	MFPS& operator-=(const mint& sc) { *this += -sc; return *this; }
	MFPS operator-(const mint& sc) const { return MFPS(*this) -= sc; }
	friend MFPS operator-(const mint& sc, const MFPS& f) { return -(f - sc); }
	MFPS& operator-=(const int& sc) { *this += -sc; return *this; }
	MFPS operator-(const int& sc) const { return MFPS(*this) -= sc; }
	friend MFPS operator-(const int& sc, const MFPS& f) { return -(f - sc); }

	// 加法逆元
	MFPS operator-() const { return MFPS(*this) *= -1; }

	// 定数倍
	MFPS& operator*=(const mint& sc) { rep(i, n) c[i] *= sc; return *this; }
	MFPS operator*(const mint& sc) const { return MFPS(*this) *= sc; }
	friend MFPS operator*(const mint& sc, const MFPS& f) { return f * sc; }
	MFPS& operator*=(const int& sc) { *this *= mint(sc); return *this; }
	MFPS operator*(const int& sc) const { return MFPS(*this) *= sc; }
	friend MFPS operator*(const int& sc, const MFPS& f) { return f * sc; }

	// 右からの定数除算
	MFPS& operator/=(const mint& sc) { *this *= sc.inv(); return *this; }
	MFPS operator/(const mint& sc) const { return MFPS(*this) /= sc; }
	MFPS& operator/=(const int& sc) { *this /= mint(sc); return *this; }
	MFPS operator/(const int& sc) const { return MFPS(*this) /= sc; }

	// 積
	MFPS& operator*=(const MFPS& g) { c = CONV(c, g.c); n = sz(c); return *this; }
	MFPS operator*(const MFPS& g) const { return MFPS(*this) *= g; }

	// 除算
	MFPS inv(int d) const {
		// 参考：https://nyaannyaan.github.io/library/fps/formal-power-series.hpp
		// verify : https://judge.yosupo.jp/problem/inv_of_formal_power_series

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

		Assert(c[0] != 0);

		MFPS g(c[0].inv());
		for (int k = 1; k < d; k *= 2) {
			g = (2 - *this * g) * g;
			g.resize(2 * k);
		}

		return g.resize(d);
	}
	MFPS& operator/=(const MFPS& g) { return *this *= g.inv(max(n, g.n)); }
	MFPS operator/(const MFPS& g) const { return MFPS(*this) /= g; }

	// 余り付き除算
	MFPS quotient(const MFPS& g) const {
		// 参考 : https://nyaannyaan.github.io/library/fps/formal-power-series.hpp
		// verify : https://judge.yosupo.jp/problem/division_of_polynomials

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

		if (n < g.n) return MFPS();
		return ((this->rev() / g.rev()).resize(n - g.n + 1)).rev();
	}

	MFPS reminder(const MFPS& g) const {
		// verify : https://judge.yosupo.jp/problem/division_of_polynomials

		return (*this - this->quotient(g) * g).resize(g.n - 1);
	}

	pair<MFPS, MFPS> quotient_remainder(const MFPS& g) const {
		// verify : https://judge.yosupo.jp/problem/division_of_polynomials

		pair<MFPS, MFPS> res;
		res.first = this->quotient(g);
		res.second = (*this - res.first * g).resize(g.n - 1);
		return res;
	}

	// スパース積
	MFPS& operator*=(const SMFPS& g) {
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

				c[i + j] += c[i] * gj;
			}

			// 定数項は最後に配るか消去しないといけない．
			c[i] *= g0;
		}

		return *this;
	}
	MFPS operator*(const SMFPS& g) const { return MFPS(*this) *= g; }

	// スパース商
	MFPS& operator/=(const SMFPS& g) {
		// g の定数項だけ例外処理
		auto it0 = g.begin();
		Assert(it0->first == 0 && it0->second != 0);
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

				c[i + j] -= c[i] * gj;
			}
		}

		return *this;
	}
	MFPS operator/(const SMFPS& g) const { return MFPS(*this) /= g; }

	// 係数反転
	MFPS rev() const { MFPS h = *this; reverse(all(h.c)); return h; }

	// 単項式
	static MFPS monomial(int d) {
		MFPS mono(0, d + 1);
		mono[d] = 1;
		return mono;
	}

	// 不要な高次項の除去
	MFPS& resize() {
		// 最高次の係数が非 0 になるまで削る．
		while (n > 0 && c[n - 1] == 0) {
			c.pop_back();
			n--;
		}
		return *this;
	}

	// x^d 以上の項を除去する．
	MFPS& resize(int d) {
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
	MFPS& operator>>=(int d) {
		n += d;
		c.insert(c.begin(), d, 0);
		return *this;
	}
	MFPS& operator<<=(int d) {
		n -= d;
		if (n <= 0) { c.clear(); n = 0; }
		else c.erase(c.begin(), c.begin() + d);
		return *this;
	}
	MFPS operator>>(int d) const { return MFPS(*this) >>= d; }
	MFPS operator<<(int d) const { return MFPS(*this) <<= d; }

	// 累乗の剰余
	friend MFPS power_mod(const MFPS& f, ll d, const MFPS& g) {
		MFPS res(1), pow2(f);
		while (d > 0) {
			if (d & 1LL) res = (res * pow2).reminder(g);
			pow2 = (pow2 * pow2).reminder(g);
			d /= 2;
		}
		return res;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const MFPS& f) {
		if (f.n == 0) os << 0;
		else {
			rep(i, f.n) {
				os << f[i].val() << "z^" << i;
				if (i < f.n - 1) os << " + ";
			}
		}
		return os;
	}
#endif
};


//【展開係数】O(n log n log d)
/*
* 有理式 f(x)/g(x) を形式的冪級数に展開したときの x^d の係数を返す．
*
* 制約 : deg f < deg g, g[0] != 0
*/
mint bostan_mori(const MFPS& f, const MFPS& g, ll d) {
	// 参考 : http://q.c.titech.ac.jp/docs/progs/polynomial_division.html
	// verify : https://atcoder.jp/contests/tdpc/tasks/tdpc_fibonacci

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

	Assert(g.n >= 1 && g[0] != 0);

	// d = 0 のときは定数項を返す．
	if (d == 0) return f[0] / g[0];

	// f2(x) = f(x) g(-x), g2(x) = g(x) g(-x) を求める．
	MFPS f2, g2 = g;
	rep(i, g2.n) if (i % 2 == 1) g2[i] *= -1;
	f2 = f * g2;
	g2 *= g;

	// f3(x) = E(x) or O(x), g3(x) = e(x) を求める．
	MFPS f3, g3;
	if (d % 2 == 0) {
		for (int i = 0; 2 * i < f2.n; i++) {
			f3.c.push_back(f2[2 * i]);
		}
	}
	else {
		for (int i = 0; 2 * i + 1 < f2.n; i++) {
			f3.c.push_back(f2[2 * i + 1]);
		}
	}
	f3.n = sz(f3.c);
	rep(i, g.n) g3.c.push_back(g2[2 * i]);
	g3.n = sz(g3.c);

	// d を半分にして再帰を回す．
	return bostan_mori(f3, g3, d / 2);
}


mint TLE(int n, ll m, vi a) {
	int a_sum = accumulate(all(a), 0);

	MFPS f(1, a_sum + 1);
	rep(i, n) f *= MFPS::SMFPS({ {1, 1}, {a[i], -1} });

	return bostan_mori(MFPS(1), f, m);
}


int main() {
//	input_from_file("input.txt");
//	output_to_file("output.txt");

	int k, n;
	cin >> k >> n;

	MFPS::set_conv(convolution_arbitrary_mod);

	MFPS f(0, k + 1);
	f[1] = 1;
	repi(i, 3, k) f[i] = -(i - 2);
	dump(f);

	MFPS g(0, k + 1);
	g[0] = 1;
	repi(i, 1, k) g[i] = -1;
	dump(g);

	cout << bostan_mori(f, g, n) << endl;
}
