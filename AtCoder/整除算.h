#pragma once
#include "header.h"
// ■■■■■ 整数除算，GCD 等 ■■■■■


//【商列挙】O(√n)
/*
* i=[1..n] に対し，n/i の商が q となる i の範囲が [i1..i2) であることを
* {q, i1, i2} として q について降順に qi に格納する．
* 各範囲においては余りは公差 -q の等差数列を成す．
*/
void quotient_range(ll n, vector<tuple<ll, ll, ll>>& qis) {
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
	//		商 n/i	i の範囲		余り n%i
	//		15		[1..2)		[0]
	//		7		[2..3)		[1]
	//		5		[3..4)		[0]
	//		3		[4..6)		[3, 0]
	//		2		[6..8)		[3, 1]
	//		1		[8..16)		[7, 6, 5, 4, 3, 2, 1, 0]

	ll m = (ll)(sqrt(n) + EPS);

	// q に対応する i が高々 1 個の部分は i ごとに愚直に考える．
	for (int i = 1; n / i > m; i++) {
		qis.push_back({ n / i, i, i + 1 });
	}

	// そうでない部分は q ごとにまとめて考える．
	repir(q, m, 1) {
		ll i0 = n / (q + 1LL) + 1;
		ll i1 = n / q + 1;
		qis.push_back({ q, i0, i1 });
	}
}


//【長さ 2 の倍数列の数え上げ】O(√n)
/*
* 1 <= x | y <= n を満たす組 (x, y) の個数を返す．
*
* 利用：【商列挙】
*/
template<class T> T count_multiple_2chain(ll n) {
	// 参考：http://oeis.org/A006218
	// verify : https://yukicoder.me/problems/no/1276

	//【方法】
	// x を固定すれば，条件を満たす y は n 以下の x の倍数全てなので，その個数は floor(n/x) である．
	// よって求めるべき値は
	//		Σx∈[1..n] floor(n/x)
	// である．
	// これは floor(n/x) の値が等しいところをまとめて計算することにより高速化できる．

	vector<tuple<ll, ll, ll>> qis;
	quotient_range(n, qis);

	T res = 0;

	repe(qi, qis) {
		ll q, i1, i2;
		tie(q, i1, i2) = qi;

		res += q * T(i2 - i1);
	}

	return res;
}


//【長さ 3 の倍数列の数え上げ】O(n^(2/3)) ?
/*
* 1 <= x | y | z <= n を満たす組 (x, y, z) の個数を返す．
*
* 利用：【整数累乗根】,【長さ 2 の倍数列の数え上げ】
*/
template<class T> T count_multiple_3chain(ll n) {
	// 参考：http://oeis.org/A061201
	// verify : https://yukicoder.me/problems/no/1276

	int m = (int)integer_root(n, 3);

	T res = T(m) * m * m;

	repi(i, 1, m) {
		res += 3 * count_multiple_2chain<T>(n / i);
		repi(j, 1, m) {
			res -= 3 * (n / ((ll)i * j));
		}
	}

	return res;
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


//【gcd と階差】
/*
* 数列 a[0..n) の階差を d[0..n-1)（d[i] = a[i+1] - a[i]）とするとき，
*		gcd(a[l..r)) = gcd( a[i], gcd(d[l..r-1)) )（i∈[l..r)）
*
* verify : https://atcoder.jp/contests/arc017/tasks/arc017_4
*/


//【部分集合の gcd】
/*
* 与えられた非負整数の集合 U と非負整数 g について，
*	gcd(S) = g となるような S ⊂ U が存在する
*	⇔ g の倍数である U の元全ての gcd が g に一致する
*
* verify : https://codeforces.com/contest/1627/problem/D
*/


//【レピュニットの gcd】
/*
* rep(n) := (10^n - 1) / 9（1 が n 個並んだ数）とおくと，
*		gcd(rep(n), rep(m)) = rep(gcd(n, m))
* 
* 証明：
* 筆算をイメージすることで自明に思える以下の式
*		rep(n) mod rep(m) = rep(n mod m)
* をユークリッドの互除法のように繰り返し用いればよい．
* 
* verify : https://atcoder.jp/contests/arc050/tasks/arc050_c
*/


//【切り捨て除算】O(1)
/*
* a, b の正負によらず，数学的な floor(a / b) を返す．
*/
template <class T> T floor_div(T a, T b) {
	// verify : https://yukicoder.me/problems/no/2032

	return a / b - (T)(a % b < 0);
}


//【切り上げ除算】O(1)
/*
* a, b の正負によらず，数学的な ceil(a / b) を返す．
*/
template <class T> T ceil_div(T a, T b) {
	// verify : https://yukicoder.me/problems/no/2099

	T num = a + b - 1;
	return num / b - (T)(num % b < 0);
}


//【切り捨て除算の結合法則】
/*
* 切り捨て除算は結合法則を満たす．すなわち以下の式が成り立つ：
*		floor(floor(a / b) / c) = floor(a / (b * c))
* 
* verify : https://atcoder.jp/contests/abc256/tasks/abc256_h
*/


//【切り捨て（余り指定）】O(1)
/*
* 与えられた x に対し，x 以下の y で y = k (mod m) を満たす最大のものを返す．
*/
template <class T> T floor_mod(T x, T m, T k) {
	//【方法】
	// k = 0 の場合は
	//		y = x - (x mod m)
	// とすればよい．一般の k の場合は，k ずらして考えることにより
	//		y - k = (x - k) - ((x - k) mod m)
	// を得る．

	Assert(m > 0);

	k = smod(k, m);
	x -= k;
	T y = x - smod(x, m);

	return y + k;
}


//【切り上げ（余り指定）】O(1)
/*
* 与えられた x に対し，x 以上の y で y = k (mod m) を満たす最小のものを返す．
*/
template <class T> T ceil_mod(T x, T m, T k) {
	//【方法】
	// k = 0 の場合は
	//		y = x + (-x mod m)
	// とすればよい．一般の k の場合は，k ずらして考えることにより
	//		y - k = (x - k) + (-(x - k) mod m)
	// を得る．

	Assert(m > 0);

	k = smod(k, m);
	x -= k;
	T y = x + smod(-x, m);

	return y + k;
}


//【余りが k である数の個数】
/*
* x∈[l..r) で x = k (mod m) を満たすものの個数を返す．
*/
template <class T> T count_by_reminder(T l, T r, T m, T k) {
	// verify : https://codeforces.com/contest/628/problem/F
	
	//【方法】
	// l = k (mod m) になるように l を増加させても答えは変わらない．
	// こうすれば個数は [0..n) 内の m の倍数の数え上げと同様に考えて
	//		(r - l + m - 1) / m
	// で求められる．

	Assert(m > 0);
	if (l >= r) return 0;

	k = smod(k, m);

	l -= k;
	T l2 = l + smod(-l, m);
	l2 += k;

	return (r - l2 + m - 1) / m;
}


