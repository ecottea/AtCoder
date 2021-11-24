#pragma once
#include "header.h"
// ■■■■■ 自作の簡易 mint ■■■■■


//【有限体 F_p 上の計算（32 bit）】
struct mint {
	// verify : https://codeforces.com/contest/1279/problem/D

	ll v;

	// 適切な法（素数）に書き換えてから用いる．
//	static const ll MOD = (ll)1e9 + 7;
	static const ll MOD = 998244353;

	// コンストラクタ
	mint() : v(0) {};
	mint(const mint& a) = default;
	mint(int a) : v(safe_mod(a)) {};
	mint(ll a) : v(safe_mod(a)) {};

	// 代入
	mint& operator=(const mint& a) { v = a.v; return *this; }
	mint& operator=(int a) { v = safe_mod(a); return *this; }
	mint& operator=(ll a) { v = safe_mod(a); return *this; }

	// 入出力
	friend istream& operator>> (istream& is, mint& x) { is >> x.v; x.v = safe_mod(x.v); return is; }
	friend ostream& operator<< (ostream& os, const mint& x) { os << x.v; return os; }

	// 非負 mod
	template <class T> static ll safe_mod(T a) { return (((ll)a % MOD) + MOD) % MOD; }

	// 比較
	bool operator==(const mint& b) const { return v == b.v; }
	bool operator==(int b) const { return v == safe_mod(b); }
	bool operator==(ll b) const { return v == safe_mod(b); }
	friend bool operator==(int a, const mint& b) { return b == a; }
	friend bool operator==(ll a, const mint& b) { return b == a; }

	// 単項演算
	mint operator-() const { mint a = *this; return a *= -1; }
	mint& operator++() { v = safe_mod(v + 1); return *this; }
	mint operator++(int) { mint tmp = *this; ++(*this); return tmp; }
	mint& operator--() { v = safe_mod(v - 1); return *this; }
	mint operator--(int) { mint tmp = *this; --(*this); return tmp; }

	// 二項演算
	mint& operator+=(const mint& b) { v = safe_mod(v + b.v); return *this; }
	mint& operator-=(const mint& b) { v = safe_mod(v - b.v); return *this; }
	mint& operator*=(const mint& b) { v = safe_mod(v * b.v); return *this; }
	mint& operator/=(const mint& b) { *this *= b.inv(); return *this; }
	mint operator+(const mint& b) const { mint a = *this; return a += b; }
	mint operator-(const mint& b) const { mint a = *this; return a -= b; }
	mint operator*(const mint& b) const { mint a = *this; return a *= b; }
	mint operator/(const mint& b) const { mint a = *this; return a /= b; }

	// int との演算
	mint& operator+=(int b) { v = safe_mod(v + b); return *this; }
	mint& operator-=(int b) { v = safe_mod(v - b); return *this; }
	mint& operator*=(int b) { v = safe_mod(v * b); return *this; }
	mint& operator/=(int b) { *this *= mint(b).inv(); return *this; }
	mint operator+(int b) const { mint a = *this; return a += b; }
	mint operator-(int b) const { mint a = *this; return a -= b; }
	mint operator*(int b) const { mint a = *this; return a *= b; }
	mint operator/(int b) const { mint a = *this; return a /= b; }
	friend mint operator+(int a, const mint& b) { return b + a; }
	friend mint operator-(int a, const mint& b) { return -(b - a); }
	friend mint operator*(int a, const mint& b) { return b * a; }
	friend mint operator/(int a, const mint& b) { return b.inv() * a; }

	// ll との演算
	mint& operator+=(ll b) { v = safe_mod(v + (b % MOD)); return *this; }
	mint& operator-=(ll b) { v = safe_mod(v - (b % MOD)); return *this; }
	mint& operator*=(ll b) { v = safe_mod(v * (b % MOD)); return *this; }
	mint& operator/=(ll b) { *this *= mint(b).inv(); return *this; }
	mint operator+(ll b) const { mint a = *this; return a += b; }
	mint operator-(ll b) const { mint a = *this; return a -= b; }
	mint operator*(ll b) const { mint a = *this; return a *= b; }
	mint operator/(ll b) const { mint a = *this; return a /= b; }
	friend mint operator+(ll a, const mint& b) { return b + a; }
	friend mint operator-(ll a, const mint& b) { return -(b - a); }
	friend mint operator*(ll a, const mint& b) { return b * a; }
	friend mint operator/(ll a, const mint& b) { return b.inv() * a; }

	// 累乗（0^0 := 1）
	mint pow(ll d) const {
		mint res(1), pow2 = *this;
		while (d > 0) {
			if (d & 1LL) res *= pow2;
			pow2 *= pow2;
			d /= 2;
		}
		return res;
	}

	// 逆元（1/0 は未定義）
	mint inv() const {
		assert(v != 0);
		return pow(MOD - 2);
	}

	// 法の確認
	static int mod() { return MOD; }

	// 値の確認
	int val() const { return int(v); }
};
using vm = vector<mint>; using vvm = vector<vm>; using vvvm = vector<vvm>;


//【有限体 F_p 上の計算（64 bit）】
/*
* 有限体 F_p 上ので様々な計算を行う．
*
* 制約 : p は素数，コンパイラは gcc
*/
#ifdef _MSC_VER
#define __int128 ll // デバッグ用
#endif
struct mll {
	__int128 v;
	static __int128 MOD;

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
	friend istream& operator>> (istream& is, mll& x) { ll tmp; is >> tmp; x.v = safe_mod(tmp); return is; }
	friend ostream& operator<< (ostream& os, const mll& x) { os << (ll)x.v; return os; }

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
	static void set_mod(ll MOD_) { MOD = MOD_; }
	static ll mod() { return (ll)MOD; }

	// 値の確認
	ll val() const { return (ll)safe_mod(v); }
};
__int128 mll::MOD;


