#pragma once
#include "header.h"
// ■■■■■ 自作の簡易 mint ■■■■■


//【有限体 F_p 上の計算（32 bit）】（遅い）
#ifndef _MSC_VER
template <ll MOD> struct static_modint {
	ll v;

	// コンストラクタ
	static_modint() : v(0) {};
	static_modint(const static_modint& a) = default;
	static_modint(int a) : v(safe_mod(a)) {};
	static_modint(ll a) : v(safe_mod(a)) {};

	// 代入
	static_modint& operator=(const static_modint& a) { v = a.v; return *this; }
	static_modint& operator=(int a) { v = safe_mod(a); return *this; }
	static_modint& operator=(ll a) { v = safe_mod(a); return *this; }

	// 入出力
	friend istream& operator>> (istream& is, static_modint& x) { is >> x.v; x.v = safe_mod(x.v); return is; }
	friend ostream& operator<< (ostream& os, const static_modint& x) { os << x.v; return os; }

	// 非負 mod
	template <class T> static ll safe_mod(T a) { return (((ll)a % MOD) + MOD) % MOD; }

	// 比較
	bool operator==(const static_modint& b) const { return v == b.v; }
	bool operator==(int b) const { return v == safe_mod(b); }
	bool operator==(ll b) const { return v == safe_mod(b); }
	friend bool operator==(int a, const static_modint& b) { return b == a; }
	friend bool operator==(ll a, const static_modint& b) { return b == a; }
	bool operator!=(const static_modint& b) const { return v != b.v; }
	bool operator!=(int b) const { return v != safe_mod(b); }
	bool operator!=(ll b) const { return v != safe_mod(b); }
	friend bool operator!=(int a, const static_modint& b) { return b != a; }
	friend bool operator!=(ll a, const static_modint& b) { return b != a; }

	// 単項演算
	static_modint operator-() const { static_modint a = *this; return a *= -1; }
	static_modint& operator++() { v = safe_mod(v + 1); return *this; }
	static_modint operator++(int) { static_modint tmp = *this; ++(*this); return tmp; }
	static_modint& operator--() { v = safe_mod(v - 1); return *this; }
	static_modint operator--(int) { static_modint tmp = *this; --(*this); return tmp; }

	// 二項演算
	static_modint& operator+=(const static_modint& b) { v = safe_mod(v + b.v); return *this; }
	static_modint& operator-=(const static_modint& b) { v = safe_mod(v - b.v); return *this; }
	static_modint& operator*=(const static_modint& b) { v = safe_mod(v * b.v); return *this; }
	static_modint& operator/=(const static_modint& b) { *this *= b.inv(); return *this; }
	static_modint operator+(const static_modint& b) const { static_modint a = *this; return a += b; }
	static_modint operator-(const static_modint& b) const { static_modint a = *this; return a -= b; }
	static_modint operator*(const static_modint& b) const { static_modint a = *this; return a *= b; }
	static_modint operator/(const static_modint& b) const { static_modint a = *this; return a /= b; }

	// int との演算
	static_modint& operator+=(int b) { v = safe_mod(v + b); return *this; }
	static_modint& operator-=(int b) { v = safe_mod(v - b); return *this; }
	static_modint& operator*=(int b) { v = safe_mod(v * b); return *this; }
	static_modint& operator/=(int b) { *this *= static_modint(b).inv(); return *this; }
	static_modint operator+(int b) const { static_modint a = *this; return a += b; }
	static_modint operator-(int b) const { static_modint a = *this; return a -= b; }
	static_modint operator*(int b) const { static_modint a = *this; return a *= b; }
	static_modint operator/(int b) const { static_modint a = *this; return a /= b; }
	friend static_modint operator+(int a, const static_modint& b) { return b + a; }
	friend static_modint operator-(int a, const static_modint& b) { return -(b - a); }
	friend static_modint operator*(int a, const static_modint& b) { return b * a; }
	friend static_modint operator/(int a, const static_modint& b) { return b.inv() * a; }

	// ll との演算
	static_modint& operator+=(ll b) { v = safe_mod(v + (b % MOD)); return *this; }
	static_modint& operator-=(ll b) { v = safe_mod(v - (b % MOD)); return *this; }
	static_modint& operator*=(ll b) { v = safe_mod(v * (b % MOD)); return *this; }
	static_modint& operator/=(ll b) { *this *= static_modint(b).inv(); return *this; }
	static_modint operator+(ll b) const { static_modint a = *this; return a += b; }
	static_modint operator-(ll b) const { static_modint a = *this; return a -= b; }
	static_modint operator*(ll b) const { static_modint a = *this; return a *= b; }
	static_modint operator/(ll b) const { static_modint a = *this; return a /= b; }
	friend static_modint operator+(ll a, const static_modint& b) { return b + a; }
	friend static_modint operator-(ll a, const static_modint& b) { return -(b - a); }
	friend static_modint operator*(ll a, const static_modint& b) { return b * a; }
	friend static_modint operator/(ll a, const static_modint& b) { return b.inv() * a; }

	// 累乗（0^0 := 1）
	static_modint pow(ll d) const {
		static_modint res(1), pow2 = *this;
		while (d > 0) {
			if (d & 1LL) res *= pow2;
			pow2 *= pow2;
			d /= 2;
		}
		return res;
	}

	// 逆元（0^(-1) は未定義）
	static_modint inv() const {
		Assert(v != 0);
		return pow(MOD - 2);
	}

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
* 制約 : p は素数，コンパイラは gcc
*/
#ifdef _MSC_VER
#define __int128 ll // デバッグ用
#endif
struct mll {
	__int128 v;
	inline static __int128 MOD;

	// コンストラクタ
	mll() : v(0) {};
	mll(const mll& a) = default;
	mll(const int& a) : v(safe_mod(a)) {};
	mll(const ll& a) : v(safe_mod(a)) {};

	// 代入
	mll& operator=(const mll& a) { v = a.v; return *this; }
	mll& operator=(const int& a) { v = safe_mod(a); return *this; }
	mll& operator=(const ll& a) { v = safe_mod(a); return *this; }

	// 入出力
	friend istream& operator>>(istream& is, mll& x) { ll tmp; is >> tmp; x.v = safe_mod(tmp); return is; }
	friend ostream& operator<<(ostream& os, const mll& x) { os << (ll)x.v; return os; }

	// 非負 mod
	template <class T> static __int128 safe_mod(T a) { return ((a % MOD) + MOD) % MOD; }

	// 比較
	bool operator==(const mll& b) const { return v == b.v; }
	bool operator==(const int& b) const { return v == safe_mod(b); }
	bool operator==(const ll& b) const { return v == safe_mod(b); }
	friend bool operator==(const int& a, const mll& b) { return b == a; }
	friend bool operator==(const ll& a, const mll& b) { return b == a; }

	// 演算
	mll& operator+=(const mll& b) { v = safe_mod(v + b.v); return *this; }
	mll& operator-=(const mll& b) { v = safe_mod(v - b.v); return *this; }
	mll& operator*=(const mll& b) { v = safe_mod(v * b.v); return *this; }
	mll& operator/=(const mll& b) { *this *= b.inv(); return *this; }
	mll operator+(const mll& b) const { mll a = *this; return a += b; }
	mll operator-(const mll& b) const { mll a = *this; return a -= b; }
	mll operator*(const mll& b) const { mll a = *this; return a *= b; }
	mll operator/(const mll& b) const { mll a = *this; return a /= b; }
	mll operator-() const { mll a = *this; return a *= -1; }

	// int との演算
	mll& operator+=(const int& b) { v = safe_mod(v + b); return *this; }
	mll& operator-=(const int& b) { v = safe_mod(v - b); return *this; }
	mll& operator*=(const int& b) { v = safe_mod(v * b); return *this; }
	mll& operator/=(const int& b) { *this *= mll(b).inv(); return *this; }
	mll operator+(const int& b) const { mll a = *this; return a += b; }
	mll operator-(const int& b) const { mll a = *this; return a -= b; }
	mll operator*(const int& b) const { mll a = *this; return a *= b; }
	mll operator/(const int& b) const { mll a = *this; return a /= b; }
	friend mll operator+(const int& a, const mll& b) { return b + a; }
	friend mll operator-(const int& a, const mll& b) { return -(b - a); }
	friend mll operator*(const int& a, const mll& b) { return b * a; }
	friend mll operator/(const int& a, const mll& b) { return mll(a) * b.inv(); }

	// ll との演算
	mll& operator+=(const ll& b) { v = safe_mod(v + b); return *this; }
	mll& operator-=(const ll& b) { v = safe_mod(v - b); return *this; }
	mll& operator*=(const ll& b) { v = safe_mod(v * b); return *this; }
	mll& operator/=(const ll& b) { *this *= mll(b).inv(); return *this; }
	mll operator+(const ll& b) const { mll a = *this; return a += b; }
	mll operator-(const ll& b) const { mll a = *this; return a -= b; }
	mll operator*(const ll& b) const { mll a = *this; return a *= b; }
	mll operator/(const ll& b) const { mll a = *this; return a /= b; }
	friend mll operator+(const ll& a, const mll& b) { return b + a; }
	friend mll operator-(const ll& a, const mll& b) { return -(b - a); }
	friend mll operator*(const ll& a, const mll& b) { return b * a; }
	friend mll operator/(const ll& a, const mll& b) { return mll(a) * b.inv(); }

	// 累乗
	mll pow(ll d) const {
		mll res(1), pow2 = *this;
		while (d > 0) {
			if (d & 1) res *= pow2;
			pow2 *= pow2;
			d /= 2;
		}
		return res;
	}

	// 逆元
	mll inv() const { return pow(MOD - 2); }

	// 法の設定，確認
	static void set_mod(ll MOD_) { Assert(MOD_ > 0); MOD = MOD_; }
	static ll mod() { return (ll)MOD; }

	// 値の確認
	ll val() const { return (ll)safe_mod(v); }
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
*/
struct QF {
	// a + b √d を表す．
	inline static mint d;
	mint a, b;

	// d を定める
	static void set_base(mint d_) {
		// verify : https://judge.yosupo.jp/problem/sqrt_mod

		d = d_;
	}

	// 初期化
	QF() : a(0), b(0) {}
	QF(const mint& a) : a(a), b(0) {}
	QF(const mint& a, const mint& b) : a(a), b(b) {
		// verify : https://judge.yosupo.jp/problem/sqrt_mod
	}

	// 代入
	QF(const QF&) = default;
	QF& operator=(const QF&) = default;

	// 和
	QF& operator+=(const QF& y) {
		a += y.a; b += y.b;
		return *this;
	}
	QF operator+(const QF& y) const { QF x = *this; return x += y; }

	// 差
	QF& operator-=(const QF& y) {
		// verify : https://judge.yosupo.jp/problem/sqrt_mod

		a -= y.a; b -= y.b;
		return *this;
	}
	QF operator-(const QF& y) const { QF x = *this; return x -= y; }

	// 積
	QF operator*(const QF& y) const {
		// verify : https://judge.yosupo.jp/problem/sqrt_mod

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
		// verify : https://judge.yosupo.jp/problem/sqrt_mod

		QF res(1), pow2 = *this;
		while (n > 0) {
			if (n & 1) res *= pow2;
			pow2 *= pow2;
			n /= 2;
		}
		return res;
	}
};


