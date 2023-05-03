#pragma once
#include "header.h"
#include "数論.h"
// ■■■■■ GCD, LCM 等 ■■■■■


//【最大公約数】O(log min(a, b))
/*
* gcd(a, b) >= 0 を返す．
*/
ll euclid(ll a, ll b) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/1/ALDS1_1_B

	a = abs(a);
	b = abs(b);

	// 改変しやすいよう再帰を用いずに書く
	while (b > 0) {
		a %= b;
		swap(a, b);
	}

	return a;
}


//【最大公約数（複数）】O(n + log(max a[0..n)))
/*
* gcd a[0..n) を返す．（空列の gcd は 0 とする）
*/
ll gcd(const vl& a) {
	// verify : https://atcoder.jp/contests/arc048/tasks/arc048_c

	int n = sz(a);

	ll g = 0;
	rep(i, n) g = gcd(g, a[i]);

	return g;
}


//【最小公倍数】O(log max(a, b))
/*
* lcm(a, b) を返す．
*/
ll lcm(ll a, ll b) {
	//verify: https://atcoder.jp/contests/abc131/tasks/abc131_c

	return a / gcd(a, b) * b;
}


//【最小公倍数（複数，結果が小さな数）】O(n log(max a[0..n))) 
/*
* lcm a[0..n) を返す．（空列の lcm は 1 とする）
*/
ll lcm(const vl& a) {
	// verify : https://yukicoder.me/problems/no/2045

	int n = sz(a);

	ll l = 1;
	rep(i, n) l = l / gcd(l, a[i]) * a[i];

	return l;
}


//【最小公倍数（複数，結果が大きな数）】O(n √max(a)) 
/*
* lcm a[0..n) を返す．（空列の lcm は 1 とする）
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
	repe(pp, lpps) res *= pow(pp.first, pp.second);

	return res;
}


//【拡張ユークリッドの互除法】O(log max(|a|, |b|))
/*
* g = gcd(a, b) > 0 を返しつつ，a x + b y = g の解 (x, y) を求める．
* |x| + |y| は最小になるよう選ばれる．
*/
ll extended_gcd(ll a, ll b, ll& x, ll& y) {
	// 参考：https://qiita.com/drken/items/b97ff231e43bce50199a
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/6/NTL/all/NTL_1_E

	//【方法】
	// b = 0 の場合は，明らかに g = a で，(x, y) = (1, 0) が解である．
	// 
	// b != 0 の場合を考える．a を b で割り
	//		a = q b + r (0 <= r < b)
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
		// 最大公約数は正とする．
		x = (a > 0) ? 1 : -1;
		y = 0;
		return a * x;
	}

	// a を b で割った商 q と余り r を求めておく．
	ll q = a / b, r = a % b;

	// a, b を更新し解 X, Y を得る．
	ll X, Y;
	ll d = extended_gcd(b, r, X, Y);

	// X, Y から x, y を得る．
	x = Y;
	y = X - q * Y;

	return d;
}


//【一次不定方程式】O(log max(|a|, |b|))
/*
* a x + b y = c の特殊解 (x, y) を求める．
* 解があれば gcd(a, b) > 0，なければ -1 を返す．
*
* 利用：【拡張ユークリッドの互除法】
*/
ll bezout(ll a, ll b, ll c, ll& x, ll& y) {
	// verify : https://atcoder.jp/contests/arc091/tasks/arc091_d

	ll g = extended_gcd(a, b, x, y);

	if (c % g != 0) return -1;

	x *= c / g;
	y *= c / g;

	// x を非負最小にしたければ，x = smod(x, b / g); y = (n - a * x) / b; とする．
	// y を非負最小にしたければ，y = smod(y, a / g); x = (n - b * y) / a; とする．
	// verify : https://atcoder.jp/contests/arc091/tasks/arc091_d

	return g;
}


//【積 → GCD, LCM】
/*
* 2 数の積について以下の等式が成り立つ：
*	x y = LCM(x, y) GCD(x, y)
*
* 3 数の積について以下の等式が成り立つ：
*	x y z
*	= LCM(x, y, z) GCD(x, y) GCD(y, z) GCD(z, x) / GCD(x, y, z)
*	= GCD(x, y, z) LCM(x, y) LCM(y, z) LCM(z, x) / LCM(x, y, z)
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


//【レピュニットの GCD】
/*
* rep(n) := (10^n - 1) / 9（1 が n 個並んだ数）とおくと，
*		GCD(rep(n), rep(m)) = rep(GCD(n, m))
*
* 証明：
* 筆算をイメージすることで自明に思える以下の式
*		rep(n) mod rep(m) = rep(n mod m)
* をユークリッドの互除法のように繰り返し用いればよい．
*
* verify : https://atcoder.jp/contests/arc050/tasks/arc050_c
*/


