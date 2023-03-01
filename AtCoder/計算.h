#pragma once
#include "header.h"
#include "探索.h"
#include "数論.h"
// ■■■■■ 計算 ■■■■■


//【n より大きい最小の 2 冪】
/*
* n より大きい最小の 2 冪は 1 << (msb(n) + 1) で得られる．
*/


//【累乗（mint 利用）】
/*
* Pow_mint(int n, ll B) : O(n)
*	底を B とし，B^(-n) から B^n まで計算可能として初期化する．
*	制約 : B は mint の法と互いに素
*
* mint [](int i) : O(1)
*	B^i を返す．
*/
class Pow_mint {
	int n;
	vm powB, powB_inv;

public:
	Pow_mint(int n, ll B) : n(n) {
		// verify : https://atcoder.jp/contests/arc116/tasks/arc116_b

		// B の累乗を計算する．
		powB.resize(n + 1);
		powB[0] = 1;
		rep(i, n) powB[i + 1] = powB[i] * B;

		// B の逆元の累乗を計算する．
		mint invB = mint(1) / B;
		powB_inv.resize(n + 1);
		powB_inv[0] = 1;
		rep(i, n) powB_inv[i + 1] = powB_inv[i] * invB;
	};

	// B^i を返す．
	mint const& operator[](int i) const {
		// verify : https://atcoder.jp/contests/arc116/tasks/arc116_b

		Assert(abs(i) <= n);

		return i >= 0 ? powB[i] : powB_inv[-i];
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Pow_mint& pw) {
		os << pw.powB << endl;
		os << pw.powB_inv << endl;
		return os;
	}
#endif
};


//【累乗】
/*
* Pow(int n, ll B, ll M) : O(n)
*	底を B とし，B^(-n) から B^n (mod M) まで計算可能として初期化する．
*	制約 : B と M は互いに素
*
* ll [](int i) : O(1)
*	B^i mod M を返す．
*/
class Pow {
	int n;
	vl powB, powB_inv;

	// a x + b y = g の解 (x, y) を返す．
	pll extended_gcd(ll a, ll b) {
		if (b == 0) {
			Assert(a == 1);
			return make_pair(1LL, 0LL);
		}

		ll q = a / b, r = a % b;
		ll X, Y;
		tie(X, Y) = extended_gcd(b, r);

		return make_pair(Y, X - q * Y);
	}

public:
	Pow(int n, ll B, ll M) : n(n) {
		// verify : https://codeforces.com/contest/715/problem/C

		B %= M;

		// B の累乗を計算する．
		powB.resize(n + 1);
		powB[0] = 1;
		rep(i, n) powB[i + 1] = powB[i] * B % M;

		// 拡張ユークリッドの互除法で B の逆元を計算する．
		ll invB, tmp;
		tie(invB, tmp) = extended_gcd(B, M);

		// B の逆元の累乗を計算する．
		powB_inv.resize(n + 1);
		powB_inv[0] = 1;
		rep(i, n) powB_inv[i + 1] = powB_inv[i] * invB % M;
	};

	// B^i を返す．
	ll const& operator[](int i) const {
		// verify : https://codeforces.com/contest/715/problem/C

		Assert(abs(i) <= n);

		return i >= 0 ? powB[i] : powB_inv[-i];
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Pow& pw) {
		os << pw.powB << endl;
		os << pw.powB_inv << endl;
		return os;
	}
#endif
};


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
* Frac<T>() : O(1)
*	0 で初期化する．
*
* Frac<T>(T num) : O(1)
*	num で初期化する．
*
* Frac<T>(T num, T dnm) : O(1)
*	num / dnm で初期化する（分母は自動的に正にする）
*
* a == b, a != b, a < b, a > b, a <= b, a >= b : O(1)
*	大小比較を行う（分母が共通の場合は積はとらない）
*
* a + b, a - b, a * b, a / b : O(1)
*	加減乗除を行う（和と差については，分母が共通の場合は積はとらない）
*	一方が整数でも構わない．複合代入演算子も使用可．
*
* reduction() : O(log min(num, dnm))
*	約分を行う．
*/
template <class T = ll>
struct Frac {
	// verify : https://atcoder.jp/contests/abc057/tasks/abc057_d

	// 分子，分母
	T num, dnm;

	// コンストラクタ
	Frac() : num(0), dnm(1) {}
	Frac(T num) : num(num), dnm(1) {}
	Frac(T num, T dnm) : num(num), dnm(dnm) {
		if (dnm < 0) { num *= -1; dnm *= -1; }
	}

	// 代入
	Frac(const Frac& b) = default;
	Frac& operator=(const Frac& b) = default;

	// キャスト
	operator double() const { return (double)num / dnm; }

	// 比較
	bool operator==(const Frac& b) const {
		// 分母が等しいときはオーバーフロー防止のために掛け算はせず比較する．
		if (dnm == b.dnm) return num == b.num;
		return num * b.dnm == b.num * dnm;
	}
	bool operator!=(const Frac& b) const { return !(*this == b); }
	bool operator<(const Frac& b) const {
		// verify : https://www.codechef.com/problems/ARCTR

		// 分母が等しいときはオーバーフロー防止のために掛け算はせず比較する．
		if (dnm == b.dnm) return num < b.num;
		return (num * b.dnm < b.num* dnm);
	}
	bool operator>=(const Frac& b) const { return !(*this < b); }
	bool operator>(const Frac& b) const { return b < *this; }
	bool operator<=(const Frac& b) const { return !(*this > b); }

	// 整数との比較
	bool operator==(T b) const { return num == b * dnm; }
	bool operator!=(T b) const { return !(*this == b); }
	bool operator<(T b) const { return num < b * dnm; }
	bool operator>=(T b) const { return !(*this < b); }
	bool operator>(T b) const { return b < *this; }
	bool operator<=(T b) const { return !(*this > b); }
	friend bool operator==(T b, const Frac& a) { return a.num == b * a.dnm; }
	friend bool operator!=(T b, const Frac& a) { return !(a == b); }
	friend bool operator<(T b, const Frac& a) { return b * a.dnm < a.num; }
	friend bool operator>=(T b, const Frac& a) { return !(a < b); }
	friend bool operator>(T b, const Frac& a) { return b < a; }
	friend bool operator<=(T b, const Frac& a) { return !(a > b); }

	// 四則演算
	Frac& operator+=(const Frac& b) {
		// verify : https://www.codechef.com/problems/ARCTR
		
		// 分母が等しいときはオーバーフロー防止のために掛け算はせず加算する．
		if (dnm == b.dnm) num += b.num;
		else { num = num * b.dnm + b.num * dnm; dnm *= b.dnm; }
		return *this;
	}
	Frac& operator-=(const Frac& b) {
		// verify : https://www.codechef.com/problems/ARCTR

		// 分母が等しいときはオーバーフロー防止のために掛け算はせず加算する．
		if (dnm == b.dnm) num -= b.num;
		else { num = num * b.dnm - b.num * dnm; dnm *= b.dnm; }
		return *this;
	}
	Frac& operator*=(const Frac& b) { num *= b.num; dnm *= b.dnm; return *this; }
	Frac& operator/=(const Frac& b) { num *= b.dnm; dnm *= b.num; return *this; }
	Frac operator+(const Frac& b) const { Frac a = *this; return a += b; }
	Frac operator-(const Frac& b) const { Frac a = *this; return a -= b; }
	Frac operator*(const Frac& b) const { Frac a = *this; return a *= b; }
	Frac operator/(const Frac& b) const { Frac a = *this; return a /= b; }
	Frac operator-() const { return Frac(*this) *= Frac(-1); }

	// 整数との四則演算
	Frac& operator+=(T c) { num += dnm * c; return *this; }
	Frac& operator-=(T c) { num -= dnm * c; return *this; }
	Frac& operator*=(T c) { num *= c; return *this; }
	Frac& operator/=(T c) {
		dnm *= c;
		if (c < 0) { num *= -1; dnm *= -1; }
		return *this;
	}
	Frac operator+(T c) const { Frac a = *this; return a += c; }
	Frac operator-(T c) const { Frac a = *this; return a -= c; }
	Frac operator*(T c) const { Frac a = *this; return a *= c; }
	Frac operator/(T c) const { Frac a = *this; return a /= c; }
	friend Frac operator+(T c, const Frac& a) { return a + c; }
	friend Frac operator-(T c, const Frac& a) { return Frac(c) - a; }
	friend Frac operator*(T c, const Frac& a) { return a * c; }
	friend Frac operator/(T c, const Frac& a) { return Frac(c) / a; }

	// 約分を行う．
	void reduction() {
		auto g = gcd(abs(num), abs(dnm));
		num /= g; dnm /= g;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Frac& a) { os << a.num << '/' << a.dnm; return os; }
#endif
};


//【整数累乗根】O(n log a)
/*
* 非負の数 a の n 乗根（a^(1/n)）の切り捨て値を返す．
*/
ll integer_root(ll a, int n) {
	// verify : https://atcoder.jp/contests/abc166/tasks/abc166_d

	if (a <= 1 || n == 1) return a;

	// x^k を返す．ただし a を超えた場合は a + 1 を返す．
	auto pow_lim = [&](ll x, int k) {
		ll v = 1;
		rep(i, k) {
			if (v > a / x) return a + 1;
			v *= x;
		}
		return v;
	};

	ll ok = 1LL, ng = a + 1;

	while (abs(ok - ng) > 1) {
		ll mid = (ok + ng) / 2;

		if (pow_lim(mid, n) <= a) ok = mid;
		else ng = mid;
	}

	return ok;
}


//【累乗で累乗】O(√m)
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


