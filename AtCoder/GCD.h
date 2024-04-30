#pragma once
#include "header.h"
#include "数論.h"
// ■■■■■ GCD, LCM 等 ■■■■■


//【最大公約数】O(log min(a, b))
/*
* GCD(a, b) ≧ 0 を返す．
*/
template <class T>
T euclid(T a, T b) {
	// verify : https://atcoder.jp/contests/tessoku-book/tasks/math_and_algorithm_o

	a = abs(a);
	b = abs(b);

	// 改変しやすいよう再帰を用いずに書く
	while (b > 0) {
		a %= b;
		swap(a, b);
	}

	return a;
}


//【最大公約数（複数）】O(n + log max(a))
/*
* GCD a[0..n) を返す．（空列の gcd は 0 とする）
*/
ll gcd(const vl& a) {
	ll g = 0;
	repe(x, a) g = gcd(g, x);
	return g;
}


//【最小公倍数（複数，結果が小さな数）】O(n log max(a)) 
/*
* LCM a[0..n) を返す．（空列の lcm は 1 とする）
*/
ll lcm(const vl& a) {
	ll l = 0;
	repe(x, a) l = lcm(l, x);
	return l;
}


//【最小公倍数（複数，結果が大きな数）】O(n √max(a)) 
/*
* LCM a[0..n) を返す．（空列の lcm は 1 とする）
*
* 利用：【素因数分解】
*/
mint lcm(const vi& a) {
	// verify : https://atcoder.jp/contests/abc152/tasks/abc152_e

	int n = sz(a);

	map<ll, int> lpps;

	rep(i, n) {
		auto pps = factor_integer(a[i]);

		repe(pp, pps) chmax(lpps[pp.first], pp.second);
	}

	mint res = 1;
	repe(pp, lpps) res *= powi(pp.first, pp.second);

	return res;
}


//【拡張ユークリッドの互除法】O(log max(|a|, |b|))
/*
* g = GCD(a, b) ≧ 0 を返しつつ，a x + b y = g の解 (x, y) を求める．
* |x| + |y| は最小になるよう選ばれる．
*/
template <class T>
T extended_gcd(T a, T b, T& x, T& y) {
	// 参考 : https://ashiato45.hatenablog.jp/entry/2018/11/04/172848
	// verify : https://atcoder.jp/contests/abc340/tasks/abc340_f

	//【方法】
	// 行列を用いた非再帰の解法を採用する．
	//
	// はじめは
	//		[1 0] [a]   [a]
	//		[0 1].[b] = [b]
	// で初期化する．第 i ステップを終えて
	//		[x_i     y_i    ] [a]  [a_i]
	//		[x_(i+1) y_(i+1)].[b] = [b_i]
	// が成り立っているとする．このとき
	//		a_i = q b_i + r
	// なる q, r をとると，
	//		[0  1] [a_i] = [b_i]
	//		[1 -q].[b_i] = [ r ]
	// より
	//		[0  1] [x_i     y_i    ] [a]   [b_i]
	//		[1 -q].[x_(i+1) y_(i+1)].[b] = [ r ]
	// が成り立つので左辺の行列積をまとめる．この更新を続けていくと，いずれ
	//		[x y] [a]   [±1]
	//		[* *].[b] = [ 0]
	// の形になるので，1 行目から所望の等式が得られる．

	if (a == 0 && b == 0) {
		x = y = 0;
		return 0;
	}

	x = 1, y = 0;
	T nx = 0, ny = 1;

	while (b != 0) {
		T q = a / b;
		T r = a % b;

		x -= q * nx;
		y -= q * ny;
		swap(nx, x);
		swap(ny, y);

		a = b;
		b = r;
	}

	if (a < 0) {
		x = -x;
		y = -y;
		a = -a;
	}

	return a;
}


//【二元一次不定方程式】O(log max(|a|, |b|))
/*
* a x + b y = c の解 (x, y) のうち，x を非負最小にするものを格納する（無理なら負も許す）
* 解があれば GCD(a, b) ≧ 0，なければ -1 を返す．
*
* 利用：【拡張ユークリッドの互除法】
*/
template <class T = ll>
T bezout(T a, T b, T c, T& x, T& y) {
	// verify : https://atcoder.jp/contests/abc340/tasks/abc340_f

	if (b == 0) {
		if (a == 0) {
			if (c == 0) {
				x = y = 0;
				return 0;
			}
			else {
				return -1;
			}
		}

		if (c % a == 0) {
			x = c / a;
			y = 0;
			return abs(a);
		}
		else {
			return -1;
		}
	}

	if (b < 0) {
		a *= -1;
		b *= -1;
		c *= -1;
	}

	// a x + b y = g = gcd(a, b) 
	T g = extended_gcd(a, b, x, y);

	if (c % g != 0) return -1;
	a /= g;
	b /= g;
	c /= g;

	x *= c % b; // c が大きくてもオーバーフローしないようにする
	x %= b;
	if (x < 0) x += b;

	y = (c - a * x) / b;

	return g;
}


//【二元一次不定方程式の解の数え上げ】O(log max(|a|, |b|))
/*
* a x + b y = c の解 (x, y) ∈ [x1..x2)×[y1..y2) の個数を返す．
*
* 利用：【拡張ユークリッドの互除法】
*/
template <class T = ll>
T count_bezout(T a, T b, T c, T x1, T x2, T y1, T y2) {
	// verify : https://atcoder.jp/contests/abc315/tasks/abc315_g

	// 探索範囲が空の場合
	if (x1 >= x2 || y1 >= y2) return 0;

	// 方程式が 0 = c の場合
	if (a == 0 && b == 0) {
		if (c == 0) return (x2 - x1) * (y2 - y1);
		return 0;
	}

	// 方程式が b y = c の場合
	if (a == 0) {
		if (c % b != 0) return 0;
		T y0 = c / b;
		if (y1 <= y0 && y0 < y2) return x2 - x1;
		return 0;
	}

	// 方程式が a x = c の場合
	if (b == 0) {
		if (c % a != 0) return 0;
		T x0 = c / a;
		if (x1 <= x0 && x0 < x2) return y2 - y1;
		return 0;
	}

	// a x0 + b y0 = g = gcd(a, b) 
	T x0, y0;
	T g = extended_gcd(a, b, x0, y0);

	// gcd(a, b) = 1 になるよう係数を調整する．
	if (c % g != 0) return 0;
	a /= g;
	b /= g;
	c /= g;

	// (x0, y0) : a x + b y = c の特殊解
	//	一般解は (x, y) = (x0, y0) + k (b, -a) （k∈Z）と表される．
	x0 = (x0 * (c % b)) % b;
	y0 = (c - a * x0) / b;

	// floor(a / b) を返す．
	auto floor_div = [](T a, T b) {
		if (b < 0) { a *= -1; b *= -1; };
		if (a >= 0) return a / b;
		else return -((-a + b - 1) / b);
	};

	// ceil(a / b) を返す．
	auto ceil_div = [](T a, T b) {
		if (b < 0) { a *= -1; b *= -1; };
		if (a >= 0) return (a + b - 1) / b;
		else return -((-a) / b);
	};

	// k に課される条件は
	//		x1 ≦ x0 + k b < x2
	//		⇔	{ (x1 - x0) / b ≦ k < (x2 - x0) / b  （b > 0）
	//			{ (x1 - x0) / b ≧ k > (x2 - x0) / b  （b < 0）
	//		⇔	{  ceil( (x1 - x0) / b ) ≦ k <  ceil( (x2 - x0) / b )  （b > 0）
	//			{ floor( (x1 - x0) / b ) ≧ k > floor( (x2 - x0) / b )  （b < 0）
	T k_min = -T(INFL), k_max = T(INFL);
	if (b > 0) {
		chmax(k_min, ceil_div(x1 - x0, b));
		chmin(k_max, ceil_div(x2 - x0, b) - 1);
	}
	else {
		chmin(k_max, floor_div(x1 - x0, b));
		chmax(k_min, floor_div(x2 - x0, b) + 1);
	}
	if (-a > 0) {
		chmax(k_min, ceil_div(y1 - y0, -a));
		chmin(k_max, ceil_div(y2 - y0, -a) - 1);
	}
	else {
		chmin(k_max, floor_div(y1 - y0, -a));
		chmax(k_min, floor_div(y2 - y0, -a) + 1);
	}

	return max(k_max - k_min + 1, T(0));
}


//【中国剰余定理】O(n log LCM m[0..n))
/*
* L = LCM m[0..n) とする．各 i∈[0..n) についての合同式
*	x ≡ r[i] (mod m[i])
* を全て満たす x∈[0..L) を求め，組 {x, L} を返す（解がなければ {0, 0} を返す）
*
* 利用：【拡張ユークリッドの互除法】
*/
template <class T>
pair<T, T> CRT(const vector<T>& r, const vector<T>& m) {
	// verify : https://atcoder.jp/contests/wtf19/tasks/wtf19_c2

	//【方法】
	// __int128 で使いたかったから ACL のをパクってきただけ．

	int n = sz(r);

	// Contracts: 0 <= r0 < m0
	T r0 = 0, m0 = 1;
	rep(i, n) {
		Assert(m[i] >= 1);

		T r1 = smod(r[i], m[i]), m1 = m[i];
		if (m0 < m1) {
			swap(r0, r1);
			swap(m0, m1);
		}
		if (m0 % m1 == 0) {
			if (r0 % m1 != r1) return { 0, 0 };
			continue;
		}
		// assume: m0 > m1, lcm(m0, m1) >= 2 * max(m0, m1)

		// (r0, m0), (r1, m1) -> (r2, m2 = lcm(m0, m1));
		// r2 % m0 = r0
		// r2 % m1 = r1
		// -> (r0 + x*m0) % m1 = r1
		// -> x*u0*g = r1-r0 (mod u1*g) (u0*g = m0, u1*g = m1)
		// -> x = (r1 - r0) / g * inv(u0) (mod u1)

		// im = inv(u0) (mod u1) (0 <= im < u1)
		T im, tmp;
		auto g = extended_gcd(m0, m1, im, tmp);

		T u1 = m1 / g;
		// |r1 - r0| < (m0 + m1) <= lcm(m0, m1)
		if ((r1 - r0) % g) return { 0, 0 };

		// u1 * u1 <= m1 * m1 / g / g <= m0 * m1 / g = lcm(m0, m1)
		T x = (r1 - r0) / g % u1 * im % u1;

		// |r0| + |m0 * x|
		// < m0 + m0 * (u1 - 1)
		// = m0 + m0 * m1 / g - m0
		// = lcm(m0, m1)
		r0 += x * m0;
		m0 *= u1;  // -> lcm(m0, m1)
		if (r0 < 0) r0 += m0;
	}
	return { r0, m0 };
}


//【GCD-LCM 包除】
/*
* a[0..n) の LCM[GCD] は，その部分集合の GCD[LCM] を用いて次のように表される：
* 	LCM(a[0..n)) = Π_set⊂[0..n),set≠φ GCD(a[set])^((-1)^(|set|-1))
* 	GCD(a[0..n)) = Π_set⊂[0..n),set≠φ LCM(a[set])^((-1)^(|set|-1))
*
* 2 数の場合：
*	LCM(a,b) = GCD(a) GCD(b) / GCD(a,b)
*	GCD(a,b) = LCM(a) LCM(b) / LCM(a,b)
*
* 3 数の場合：
*	LCM(a,b,c) = GCD(a) GCD(b) GCD(c) GCD(a,b,c) / (GCD(a,b) GCD(a,c) GCD(b,c))
*	GCD(a,b,c) = LCM(a) LCM(b) LCM(c) LCM(a,b,c) / (LCM(a,b) LCM(a,c) LCM(b,c))
*
*（証明）素因数毎に【min-max 包除】を適用すればよい．
*/


//【積 → LCM, GCD】
/*
* a[0..n) の積は，その部分集合の GCD や LCM を用いて次のように表される：
*	Πa[0..n) = LCM(a[0..n)) Π_set⊂[0..n),set≠φ GCD(a[set])^((-1)^|set|)
*	Πa[0..n) = GCD(a[0..n)) Π_set⊂[0..n),set≠φ LCM(a[set])^((-1)^|set|)
*
* 2 数の場合：
*	a b = LCM(a,b) GCD(a,b)
*
* 3 数の場合：
*	a b c = LCM(a,b,c) GCD(a,b) GCD(a,c) GCD(b,c) / GCD(a,b,c)
*	a b c = GCD(a,b,c) LCM(a,b) LCM(a,c) LCM(b,c) / LCM(a,b,c)
*
*（証明）【GCD-LCM 包除】で Σ_|set|=1 の部分のみを左辺に移せばよい．
* 
* verify : https://www.codechef.com/problems/LCM3GCD2
*/


//【集合の GCD と商】
/*
* (L..R] の元からなる集合 S のうち GCD(S) = g となるものの個数は，
* (L/g..R/g]（切り捨て）の元からなる集合 S のうち GCD(S) = 1 となるものの個数に等しい．
*
* verify : https://atcoder.jp/contests/tupc2022/tasks/tupc2022_i
*/


//【部分集合の GCD】
/*
* 与えられた非負整数の集合 U と非負整数 g について，
*	GCD(S) = g となるような S ⊂ U が存在する
*	⇔ g の倍数である U の元全ての GCD が g に一致する
*
* verify : https://codeforces.com/contest/1627/problem/D
*/


//【列の GCD と階差】
/*
* 数列 a[0..n) の階差を d[0..n-1)（d[i] = a[i+1] - a[i]）とするとき，
*		GCD(a[l..r)) = GCD( a[i], GCD(d[l..r-1)) )（i∈[l..r)）
*
* verify : https://atcoder.jp/contests/arc017/tasks/arc017_4
*/


//【等比数列の和の GCD】
/*
* 固定された正整数 r に対し，
*	S(n) := 1 + r + r^2 + ... + r^(n-1) = (r^n - 1) / (r - 1)
* とおくと，以下の等式が成り立つ：
*	GCD(S(n), S(m)) = S(GCD(n, m))
*
* 証明：
* r 進法での筆算をイメージすることで自明に思える以下の式
*		S(n) mod S(m) = S(n mod m)
* をユークリッドの互除法のように繰り返し用いればよい．
*
* verify : https://atcoder.jp/contests/arc050/tasks/arc050_c
*/


//【ガウス整数の最大公約数】
/*
* GCD(a1 + b1 i, a2 + b2 i) を返す．
*/
pll gcd_gaussian_integers(ll a1, ll b1, ll a2, ll b2) {
	// verify : https://judge.yosupo.jp/problem/gcd_of_gaussian_integers

	// 整数 a, 正整数 b に対し floor(a / b) を返す．
	auto floor_div = [](ll a, ll b) {
		return a / b - (a % b < 0);
	};

	while (a2 != 0 || b2 != 0) {
		// x := a1 + b1 i, y := a2 + b2 i とおき，
		// x/y = (x y*)/(y y*) の分子と分母を計算する．
		ll a_num = a1 * a2 + b1 * b2;
		ll b_num = -a1 * b2 + b1 * a2;
		ll dnm = a2 * a2 + b2 * b2;

		// z := round(x/y) = a3 + b3 i
		ll a3 = floor_div(2 * a_num + dnm, 2 * dnm);
		ll b3 = floor_div(2 * b_num + dnm, 2 * dnm);

		// x - y z は y よりノルムが真に小さい
		a1 -= a2 * a3 - b2 * b3;
		b1 -= a2 * b3 + b2 * a3;

		swap(a1, a2);
		swap(b1, b2);
	}

	return { a1, b1 };
}


//【拡張ユークリッドの互除法（再帰）】O(log max(|a|, |b|))
/*
* g = GCD(a, b) ≧ 0 を返しつつ，a x + b y = g の解 (x, y) を求める．
* |x| + |y| は最小になるよう選ばれる．
*/
template <class T = ll>
T extended_gcd_rec(T a, T b, T& x, T& y) {
	// 参考：https://qiita.com/drken/items/b97ff231e43bce50199a
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/6/NTL/all/NTL_1_E

	//【方法】
	// b = 0 の場合は，明らかに g = a で，(x, y) = (1, 0) が解である．
	// 
	// b ≠ 0 の場合を考える．a を b で割り
	//		a = q b + r (0 ≦ r < b)
	// なる q, r を得ておく．これを元の式に代入すると
	//		(q b + r) x + b y = g
	//		⇔ b (q x + y) + r x = g
	// となるので，
	//		b X + r Y = g
	// の解 (X, Y) = (q x + y, x) を求めれば
	//		(x, y) = (Y, X - q Y)
	// として元の式の解が得られる．これを再帰的に繰り返す．

	// b = 0 になったら自明解を返す．
	if (b == 0) {
		x = (a > 0) - (a < 0); // x = sgn(a)
		y = 0;
		return a * x; // g ≧ 0 とする
	}

	// a を b で割った商 q と余り r を求めておく（負でも大丈夫）
	T q = a / b, r = a % b;

	// a, b を更新し解 X, Y を得る．
	T X, Y;
	T d = extended_gcd_rec(b, r, X, Y);

	// X, Y から x, y を得る．
	x = Y;
	y = X - q * Y;

	return d;
}


