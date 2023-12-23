#pragma once
#include "header.h"
// ■■■■■ mint ■■■■■


//【有限体 F_p 上の計算（32 bit）】
#ifndef _MSC_VER
template <ll MOD> struct static_modint {
	// verify : https://www.codechef.com/problems/MUSICAL

	ll v;

	// コンストラクタ
	static_modint() noexcept : v(0) {};
	static_modint(const static_modint& a) = default;
	static_modint(int a) noexcept : v(a% MOD) { if (v < 0) v += MOD; };
	static_modint(ll a) noexcept : v(a% MOD) { if (v < 0) v += MOD; };

	// 代入
	static_modint& operator=(const static_modint& a) = default;
	static_modint& operator=(int a) { v = a % MOD; if (v < 0) v += MOD; return *this; }
	static_modint& operator=(ll a) { v = a % MOD; if (v < 0) v += MOD; return *this; }

	// 入出力
	friend istream& operator>> (istream& is, static_modint& x) {
		is >> x.v; x.v %= MOD; if (x.v < 0) x.v += MOD; return is;
	}
	friend ostream& operator<< (ostream& os, const static_modint& x) { os << x.v; return os; }

	// 比較（参考 : https://twitter.com/KakurenboUni/status/1717463221190414472）
	friend bool operator==(const static_modint& a, const static_modint& b) { return a.v == b.v; }
	friend bool operator!=(const static_modint& a, const static_modint& b) { return a.v != b.v; }

	// 単項演算
	static_modint operator-() const { static_modint a; if (v > 0) a.v = MOD - v; return a; }
	static_modint& operator++() { v++; if (v == MOD) v = 0; return *this; }
	static_modint operator++(int) { static_modint tmp = *this; ++(*this); return tmp; }
	static_modint& operator--() { v--; if (v == -1) v = MOD - 1; return *this; }
	static_modint operator--(int) { static_modint tmp = *this; --(*this); return tmp; }

	// 二項演算
	static_modint& operator+=(const static_modint& b) { v += b.v; if (v >= MOD) v -= MOD; return *this; }
	static_modint& operator-=(const static_modint& b) { v -= b.v; if (v < 0) v += MOD; return *this; }
	static_modint& operator*=(const static_modint& b) { v = (v * b.v) % MOD; return *this; }
	static_modint& operator/=(const static_modint& b) { *this *= b.inv(); return *this; }
	friend static_modint operator+(static_modint a, const static_modint& b) { a += b; return a; }
	friend static_modint operator-(static_modint a, const static_modint& b) { a -= b; return a; }
	friend static_modint operator*(static_modint a, const static_modint& b) { a *= b; return a; }
	friend static_modint operator/(static_modint a, const static_modint& b) { a /= b; return a; }

	// 累乗（0^0 := 1）
	static_modint pow(ll d) const {
		static_modint res(1), pow2 = *this;
		while (d > 0) {
			if (d & 1) res *= pow2;
			pow2 *= pow2;
			d >>= 1;
		}
		return res;
	}

	// 逆元（0^(-1) は未定義）
	static_modint inv() const { Assert(v != 0); return pow(MOD - 2); }

	// 法の確認
	static int mod() { return int(MOD); }

	// 値の確認
	int val() const { return int(v); }
};
//using mint = static_modint<1000000007>;
using mint = static_modint<998244353>;
using vm = vector<mint>; using vvm = vector<vm>; using vvvm = vector<vvm>; 
#endif


//【有限体 F_p 上の計算（64 bit）】
/*
* 有限体 F_p 上ので様々な計算を行う．
* mll::set_mod(ll p) はあらゆる場所で使う法を書き換えてしまうので注意．
*
* 制約 : p は素数，コンパイラが gcc
*/
struct mll {
	// verify : https://judge.yosupo.jp/problem/factorize

	__int128 v;
	inline static __int128 MOD;

	// コンストラクタ
	mll() noexcept : v(0) {}
	mll(const mll& a) = default;
	mll(int a) noexcept : v(a % MOD) { if (v < 0) v += MOD; }
	mll(ll a) noexcept : v(a % MOD) { if (v < 0) v += MOD; }

	// 代入
	mll& operator=(const mll& a) = default;
	mll& operator=(int a) { v = a % MOD; if (v < 0) v += MOD; return *this; }
	mll& operator=(ll a) { v = a % MOD; if (v < 0) v += MOD; return *this; }

	// 入出力
	friend istream& operator>>(istream& is, mll& x) {
		ll tmp; is >> tmp; x.v = tmp % MOD; if (x.v < 0) x.v += MOD; return is;
	}
	friend ostream& operator<<(ostream& os, const mll& x) { os << (ll)x.v; return os; }

	// 比較（参考 : https://twitter.com/KakurenboUni/status/1717463221190414472）
	friend bool operator==(const mll& a, const mll& b) { return a.v == b.v; }
	friend bool operator!=(const mll& a, const mll& b) { return a.v != b.v; }

	// 単項演算
	mll operator-() const { mll a; if (v > 0) a.v = MOD - v; return a; }
	mll& operator++() { v++; if (v == MOD) v = 0; return *this; }
	mll operator++(int) { mll tmp = *this; ++(*this); return tmp; }
	mll& operator--() { v--; if (v == -1) v = MOD - 1; return *this; }
	mll operator--(int) { mll tmp = *this; --(*this); return tmp; }

	// 二項演算
	mll& operator+=(const mll& b) { v += b.v; if (v >= MOD) v -= MOD; return *this; }
	mll& operator-=(const mll& b) { v -= b.v; if (v < 0) v += MOD; return *this; }
	mll& operator*=(const mll& b) { v = (v * b.v) % MOD; return *this; }
	mll& operator/=(const mll& b) { *this *= b.inv(); return *this; }
	friend mll operator+(mll a, const mll& b) { a += b; return a; }
	friend mll operator-(mll a, const mll& b) { a -= b; return a; }
	friend mll operator*(mll a, const mll& b) { a *= b; return a; }
	friend mll operator/(mll a, const mll& b) { a /= b; return a; }

	// 累乗
	mll pow(ll d) const {
		mll res(1), pow2 = *this;
		while (d > 0) {
			if (d & 1) res *= pow2;
			pow2 *= pow2;
			d >>= 1;
		}
		return res;
	}

	// 逆元
	mll inv() const { Assert(v != 0); return pow(MOD - 2); }

	// 法の設定，確認
	static void set_mod(ll MOD_) { Assert(MOD_ > 0); MOD = MOD_; }
	static ll mod() { return (ll)MOD; }

	// 値の確認
	ll val() const { return (ll)v; }
};


//【乗除算（32 bit，法が任意）】
/*
* set_mod(ll m) : O(√m)
*	法を m に設定する．
*	制約：インスタンスの生成前に呼び出すこと．
*
* Divisible_mint(ll x = 1) : O(ω(m))（ω(m) : m の素因数の種類数）
*	値 x で初期化する．
*
* x * y, x / y : O(ω(m))
*	積，商を返す．複合代入演算子も使用可．
*
* ll val() : O(ω(m))
*	m を法とした値を返す（存在しなければ -1 を返す）
*
* ll gcd(ll x) : O(ω(m))
*	x との GCD を返す．
*	制約：x は法 m の約数
* 
* Divisible_mint pow(ll k) : O(ω(m) + log k)
*	自身の k 乗を返す．
*/
class Divisible_mint {
	// MOD : 法
	static inline ll MOD;

	// ps[i] : 法の持つ i 番目の素因数（K は素因数の種類数）
	static inline int K;
	static inline vl ps;

	// num : 分子，dnm : 分母（ps からの寄与を除く）
	ll num, dnm;

	// es[i] : 素因数 ps[i] の個数
	vl es;

	// is_zero : 値が 0 か
	bool is_zero;

	// m を素因数分解し結果を ps[0..K), es_max[0..K) に格納する．
	static void factor_integer(ll m) {
		ps.clear();

		// m のもつ素因数を ps に格納する．
		for (ll p = 2; p * p <= m; p++) {
			if (m % p != 0) continue;
			while (m % p == 0) m /= p;
			ps.push_back(p);
		}

		if (m > 1) ps.push_back(m);
		K = sz(ps);
	}

	// 与えられた a, b（互いに素）に対し a x + b y = 1 の解 (x, y) を格納する．
	void extended_gcd(ll a, ll b, ll& x, ll& y) const {
		if (b == 0) {
			x = 1;
			y = 0;
			return;
		}

		ll q = a / b, r = a % b, X, Y;
		extended_gcd(b, r, X, Y);

		x = Y;
		y = X - q * Y;
	}

	// a^n (mod MOD) を返す．
	ll pow_mod(ll a, ll n) const {
		ll res = 1, pow_a = a;
		while (n > 0) {
			if (n & 1) res = (res * pow_a) % MOD;
			pow_a = (pow_a * pow_a) % MOD;
			n /= 2;
		}
		return res;
	}

public:
	// 法を m に設定する．
	static void set_mod(ll m) {
		// verify : https://yukicoder.me/problems/no/2273

		MOD = m;
		factor_integer(m);
	}

	// 値 x で初期化する．
	Divisible_mint(ll x) : num(x), dnm(1), es(K), is_zero(false) {
		// verify : https://yukicoder.me/problems/no/2273

		if (x == 0) {
			is_zero = true;
			return;
		}

		rep(k, K) {
			while (num % ps[k] == 0) {
				es[k]++;
				num /= ps[k];
			}
		}
		num %= MOD;
		if (num < 0) num += MOD;
	}

	// 値 1 で初期化する．
	Divisible_mint() : num(1), dnm(1), es(K), is_zero(false) {}

	// 積，商
	Divisible_mint& operator*=(const Divisible_mint& b) {
		// verify : https://yukicoder.me/problems/no/2273

		if (b.is_zero) {
			is_zero = true;
			return *this;
		}
		num = (num * b.num) % MOD;
		dnm = (dnm * b.dnm) % MOD;
		rep(k, K) es[k] += b.es[k];
		return *this;
	}
	Divisible_mint& operator/=(const Divisible_mint& b) {
		// verify : https://yukicoder.me/problems/no/2273

		Assert(!b.is_zero);
		num = (num * b.dnm) % MOD;
		dnm = (dnm * b.num) % MOD;
		rep(k, K) es[k] -= b.es[k];
		return *this;
	}
	Divisible_mint operator*(const Divisible_mint& b) const { Divisible_mint a = *this; return a *= b; }
	Divisible_mint operator/(const Divisible_mint& b) const { Divisible_mint a = *this; return a /= b; }

	// m を法とした値を返す（存在しなければ -1 を返す）
	ll val() const {
		// verify : https://yukicoder.me/problems/no/2273

		if (is_zero) return 0;

		// 分母の逆数を求める．
		ll dnm_inv, tmp;
		extended_gcd(dnm, MOD, dnm_inv, tmp);

		ll res = (num * dnm_inv) % MOD;
		if (res < 0) res += MOD;

		// 素因数 ps[0..K) を個数分だけ掛ける．
		rep(k, K) {
			if (es[k] < 0) return -1;
			res = (res * pow_mod(ps[k], es[k])) % MOD;
		}

		return res;
	}

	// x との GCD を返す．
	ll gcd(ll x) const {
		// verify : https://yukicoder.me/problems/no/2273

		ll res = 1;
		rep(k, K) {
			ll e = 0;
			while (x % ps[k] == 0) {
				e++;
				x /= ps[k];
			}
			rep(hoge, min(es[k], e)) res *= ps[k];
		}
		return res;
	}

	// 自身の m 乗を返す．
	Divisible_mint pow(ll m) {
		if (is_zero) return *this;

		auto res(*this);
		res.num = pow_mod(num, m);
		res.dnm = pow_mod(dnm, m);
		rep(k, K) res.es[k] *= m;

		return res;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, Divisible_mint DM) {
		os << DM.val();
		return os;
	}
#endif
};


//【二次拡大体】
/*
* a + b √d ∈ F_p(√d) を表す．
*
* set_base(mint d) : O(1)
*	体を F_p(√d) とする（p = mint::mod）
*	制約：√d !∈ F_p
*
* QF() : O(1)
*	0 で初期化する．
*
* QF(mint a) : O(1)
*	a で初期化する．
*
* QF(mint a, mint b) : O(1)
*	a + b √d で初期化する．
*
* x + y, x - y, x * y : O(1)
*	和，差，積を返す．複合代入演算子も使用可．
*
* x / y : O(log p)
*	商を返す．複合代入演算子も使用可．
*
* QF inv() : O(log p)
*	逆元を返す．
*
* QF pow(ll n) : O(log n)
*	n 乗を返す．
* 
* mint norm() : O(1)
*	a^2 - d b^2 を返す．
*/
struct QF {
	// verify : https://judge.yosupo.jp/problem/sqrt_mod

	// a + b √d を表す．
	inline static mint d;
	mint a, b;

	// d を定める
	static void set_base(mint d_) { d = d_; }

	// 初期化
	QF() : a(0), b(0) {}
	QF(const mint& a) : a(a), b(0) {}
	QF(const mint& a, const mint& b) : a(a), b(b) {}
	QF(const int& a) : a(a), b(0) {}
	QF(const int& a, const int& b) : a(a), b(b) {}
	QF(const ll& a) : a(a), b(0) {}
	QF(const ll& a, const ll& b) : a(a), b(b) {}

	// 代入
	QF(const QF&) = default;
	QF& operator=(const QF&) = default;

	// 比較
	bool operator==(const QF& y) const { return a == y.a && b == y.b; }
	bool operator!=(const QF& y) const { return !(*this == y); }

	// 和
	QF& operator+=(const QF& y) { a += y.a; b += y.b; return *this; }
	QF operator+(const QF& y) const { QF x = *this; return x += y; }

	// 差
	QF& operator-=(const QF& y) { a -= y.a; b -= y.b; return *this; }
	QF operator-(const QF& y) const { QF x = *this; return x -= y; }

	// 負元
	QF operator-() const { QF x = *this; x.a *= -1; x.b *= -1; return x; }

	// 積
	QF operator*(const QF& y) const {
		// (a1 + b1√d)(a2 + b2√d) = (a1 a2 + b1 b2 d) + (a1 b2 + a2 b1)√d
		return QF(a * y.a + b * y.b * d, a * y.b + b * y.a);
	}
	QF& operator*=(const QF& y) { *this = *this * y; return *this; }
		
	// 逆元
	QF inv() const {
		// 1/(a + b√d) = (a - b√d) / (a^2 - b^2 d)
		mint dnm = (a * a - b * b * d).inv();
		return QF(a * dnm, -b * dnm);
	}

	// 商
	QF& operator/=(const QF& y) { return *this *= y.inv(); }
	QF operator/(const QF& y) const { return *this * y.inv(); }

	// 累乗
	QF pow(ll n) const {		
		QF res(1), pow2 = *this;
		while (n > 0) {
			if (n & 1) res *= pow2;
			pow2 *= pow2;
			n /= 2;
		}
		return res;
	}

	// ノルム
	mint norm() const { return a * a - d * b * b; }

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const QF& x) {
		os << x.a << "+" << x.b << "√" << x.d;
		return os;
	}
#endif
};


