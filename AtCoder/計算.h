#pragma once
#include "header.h"
#include "探索.h"
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


//【累乗根】O(n log a)
/*
* 非負の数 a の n 乗根（a^(1/n)）の切り捨て値を返す．
*
* 利用：【めぐる式二分探索】
*/
ll root(ll a, int n) {
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


//【商列挙】O(√n)
/*
* i=[1..n] に対し，n/i の商が q となる i の範囲が [i1..i2) であることを
* {q, {i1, i2}} として q について降順に qi に格納する．
* 各範囲においては余りは公差 n/i の等差数列を成す．
*/
void quotient_range(ll n, vector<pair<ll, pll>>& qi) {
	// verify : https://atcoder.jp/contests/abc230/tasks/abc230_e

	//【方法】
	// n/i の商が q となるような i の範囲を考える．条件を i について整理すると
	//		q = floor(n / i)
	//		⇔ q <= n / i < q + 1
	//		⇔ i q <= n < i(q + 1)
	//		⇔ n / (q + 1) < i <= n / q
	// となる．
	//
	// この幅が 1 以下であれば，q に対応する i は高々 1 個である．その条件は
	//		n / q - n / (q + 1) <= 1
	//		⇔ (q + 1)n - q n <= q(q + 1)
	//		⇔ n <= q(q + 1)
	// である．条件をやや弱めて
	//		n <= q^2
	//		⇔ √n <= q
	// としてもオーダーに影響はない．

	//（例）
	// 例えば n = 15 のときは以下のように分類できる：
	//		商 n/i	i の範囲	余り n%i
	//		15		[1..2)		[0]
	//		7		[2..3)		[1]
	//		5		[3..4)		[0]
	//		3		[4..6)		[3, 0]
	//		2		[6..8)		[3, 1]
	//		1		[8..16)		[7, 6, 5, 4, 3, 2, 1, 0]

	ll m = (ll)(sqrt(n) + EPS);

	// q に対応する i が高々 1 個の部分は i ごとに愚直に考える．
	for (int i = 1; n / i > m; i++) {
		qi.push_back({ n / i, {i, i + 1} });
	}

	// そうでない部分は q ごとにまとめて考える．
	repir(q, m, 1) {
		ll i0 = n / (q + 1) + 1;
		ll i1 = n / q + 1;
		qi.push_back({ q, {i0, i1} });
	}
}


//【余りの取れる値の範囲】
/*
* 非負整数 a を m(<= a) で割った余りは a/2 未満になる．
*
* 証明：m <= a/2 のときは明らか．m > a/2 のときは
*		a mod m = a - m < a - a/2 = a/2
*
* verify : https://codeforces.com/contest/1617/problem/C
*/


//【切り捨て除算】
/*
* a, b の正負によらず，数学的な floor(a / b) は以下の式で計算できる：
*		a / b - (int)(a % b < 0)
*/


