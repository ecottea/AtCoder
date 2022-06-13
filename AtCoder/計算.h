#pragma once
#include "header.h"
#include "探索.h"
#include "数論.h"
// ■■■■■ 計算 ■■■■■


//【積との比較】O(1)
/*
* 関係式 a * b op c が成り立つかを返す．（a * b がオーバーフローしても良い）
* 比較演算子 op は，">", ">=", "=", "<=", "<" のいずれかとする．
*/
bool comp(ll a, ll b, string op_, ll c) {
	int op = 0;
	if (op_ == ">") op = -2;
	else if (op_ == ">=") op = -1;
	else if (op_ == "<=") op = 1;
	else if (op_ == "<") op = 2;

	// 等号付き不等式に統一する．
	if (op == -2) { c++; op = -1; }
	if (op == 2) { c--; op = 1; }

	// a = 0 または b = 0 の場合，左辺は 0 になるので容易に判断できる．
	if (a == 0 || b == 0) {
		if (op == -1) return 0 >= c;
		if (op == 0) return 0 == c;
		if (op == 1) return 0 <= c;
	}

	// a > 0 かつ b > 0となるように同値変形する．
	if (a < 0) { a *= -1; c *= -1; op *= -1; }
	if (b < 0) { b *= -1; c *= -1; op *= -1; }

	// (左辺) > 0 なので，0 >= c の場合は符号だけで判断できる．
	// op が ">=" なら不等式は成り立ち，さもなくば成り立たない．
	if (c <= 0) return op == -1;

	// 以降は a, b, c > 0 として考えて良い．

	// op が "=" の場合
	if (op == 0) {
		//	    a * b = c
		//	 ⇔ b = c / a
		//	 ⇔ a | c かつ b = floor(c / a)
		return c % a == 0 && c / a == b;
	}
	// op が "<=" の場合
	else if (op == 1) {
		//	    a * b <= c
		//	 ⇔ b <= c / a
		//	 ⇔ b <= floor(c / a)
		return b <= c / a;
	}
	// op が ">=" の場合
	else {
		//	    a * b >= c
		//	 ⇔ b >= c / a
		//	 ⇔ b >= ceil(c / a)
		return b >= (c + a - 1) / a;
	}
}


//【有理数】
/*
* Frac() : O(1)
*	0 で初期化する．
*
* Frac(ll num) : O(1)
*	num で初期化する．
*
* Frac(ll num, ll dnm) : O(1)
*	num / dnm で初期化する．
*
* a == b, a != b, a < b, a > b, a <= b, a >= b : O(1)
*	大小比較を行う．
*
* a + b, a - b, a * b, a / b : O(1)
*	加減乗除を行う．一方が整数でも構わない．複合代入演算子も使用可．
*
* reduction() : O(log max(num, dnm))
*	約分を行う．
*/
struct Frac {
	// verify : https://atcoder.jp/contests/abc057/tasks/abc057_d

	// 分子，分母
	ll num, dnm;

	// コンストラクタ
	Frac() : num(0), dnm(1) {}
	Frac(ll num_) : num(num_), dnm(1) {}
	Frac(ll num_, ll dnm_) : num(num_), dnm(dnm_) {}

	// 代入
	Frac(const Frac& b) = default;
	Frac& operator=(const Frac& b) = default;

	// キャスト
	operator double() const { return (double)num / dnm; }

	// 比較
	bool operator==(const Frac& b) const { return num * b.dnm == b.num * dnm; }
	bool operator!=(const Frac& b) const { return !(*this == b); }
	bool operator<(const Frac& b) const {
		if (dnm * b.dnm > 0) return (num * b.dnm < b.num* dnm);
		else return (num * b.dnm > b.num * dnm);
	}
	bool operator>=(const Frac& b) const { return !(*this < b); }
	bool operator>(const Frac& b) const { return b < *this; }
	bool operator<=(const Frac& b) const { return !(*this > b); }

	// 四則演算
	Frac& operator+=(const Frac& b) { num = num * b.dnm + b.num * dnm; dnm *= b.dnm; return *this; }
	Frac& operator-=(const Frac& b) { num = num * b.dnm - b.num * dnm; dnm *= b.dnm; return *this; }
	Frac& operator*=(const Frac& b) { num *= b.num; dnm *= b.dnm; return *this; }
	Frac& operator/=(const Frac& b) { num *= b.dnm; dnm *= b.num; return *this; }
	Frac operator+(const Frac& b) const { Frac a = *this; return a += b; }
	Frac operator-(const Frac& b) const { Frac a = *this; return a -= b; }
	Frac operator*(const Frac& b) const { Frac a = *this; return a *= b; }
	Frac operator/(const Frac& b) const { Frac a = *this; return a /= b; }

	// 整数との四則演算
	Frac& operator+=(ll c) { return *this += Frac(c); }
	Frac& operator-=(ll c) { return *this -= Frac(c); }
	Frac& operator*=(ll c) { return *this *= Frac(c); }
	Frac& operator/=(ll c) { return *this /= Frac(c); }
	Frac operator+(ll c) const { Frac a = *this; return a += c; }
	Frac operator-(ll c) const { Frac a = *this; return a -= c; }
	Frac operator*(ll c) const { Frac a = *this; return a *= c; }
	Frac operator/(ll c) const { Frac a = *this; return a /= c; }
	friend Frac operator+(ll c, const Frac& a) { return a + c; }
	friend Frac operator-(ll c, const Frac& a) { return a - c; }
	friend Frac operator*(ll c, const Frac& a) { return a * c; }
	friend Frac operator/(ll c, const Frac& a) { return a / c; }

	// 約分
	void reduction() { ll g = gcd(num, dnm); num /= g; dnm /= g; }

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Frac& a) { os << a.num << '/' << a.dnm; return os; }
#endif
};


//【整数累乗根】O(n log a)
/*
* 非負の数 a の n 乗根（a^(1/n)）の切り捨て値を返す．
*
* 利用：【めぐる式二分探索】
*/
ll integer_root(ll a, int n) {
	// verify : https://atcoder.jp/contests/abc166/tasks/abc166_d

	if (a <= 1 || n == 1) return a;
	
	// x^k を返す．ただし a を超えた場合は a + 1 を返す．
	auto pow = [&](ll x, int k) {
		ll v = 1;
		rep(i, k) {
			if (v > a / x) return a + 1;
			v *= x;
		}
		return v;
	};

	// x^n <= a かを返す．
	function<bool(ll)> okQ = [&](ll x) {
		return pow(x, n) <= a;
	};

	ll res = meguru_search(1LL, a + 1, okQ);

	return res;
}


//【累乗で累乗】
/*
* a ^ (b ^ c) mod m を返す．
*
* 利用：【オイラー関数】
*/
int power_power(ll a, ll b, ll c, int m) {
	// verify : https://atcoder.jp/contests/abc228/tasks/abc228_e

	using mint_pm = dynamic_modint<47157>; // 他と被らなければ何でも良い．

	// a^0 = 1 の例外処理
	// 以降は b^c != 0 としてよい．
	if (b == 0 && c > 0) return 1;

	// m^(b^c) = 0 (mod m) の例外処理
	// 以降は a は m の倍数ではないとしてよい．
	if (a % m == 0) return 0;

	// a^(φ(m) + 1) = a なる pm = φ(m) を得る．
	ll pm = euler_phi(m);

	// d = b^c mod pm を求める．
	mint_pm::set_mod((int)pm);
	int d = mint_pm(b).pow(c).val();

	// a^(d+φ(m)) mod m を求める．
	mint_pm::set_mod(m);
	return mint_pm(a).pow(d + pm).val();
}


//【テトレーション】O(√m log b) ?
/*
* a↑↑b mod m を返す．（0^0 = 1 とする）
*
* 利用：【オイラー関数】
*/
int tetration(ll a, ll b, int m) {
	// verify : https://judge.yosupo.jp/problem/tetration_mod

	using mint_m = dynamic_modint<97968>; // 他と被らなければ何でも良い．

	// 特殊ケースの処理
	if (m == 1) return 0;
	if (b == 0) return 1;
	if (a == 0) return (b + 1) % 2;
	if (a == 1) return 1;

	// 小さいケースの処理
	if (a == 2 && b <= 4) {
		const vi ans = { 1, 2, 4, 16, 65536 };
		return ans[b] % m;
	}
	if (a == 3 && b <= 2) {
		const vi ans = { 1, 3, 27 };
		return ans[b] % m;
	}

	// a↑↑b mod m を返す．
	function<int(ll, int)> rf = [&](ll b, int m) {
		// mod 1 なら値は 0
		if (m == 1) return 0;

		// a↑↑1 = a
		if (b == 1) return (int)(a % m);

		// 最も下の a の肩は mod φ(m) で考えれば良い．
		int phi = (int)euler_phi(m);

		// どうせ値は大きいので 1 周余分に回しても問題ない．
		int d = rf(b - 1, phi) + phi;

		mint_m::set_mod(m);
		return (int)mint_m(a).pow(d).val();
	};

	return rf(b, m);
}


