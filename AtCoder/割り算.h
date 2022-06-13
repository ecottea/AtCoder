#pragma once
#include "header.h"
// ■■■■■ 余り付き割り算，gcd 等 ■■■■■


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


//【切り捨て除算】
/*
* a, b の正負によらず，数学的な floor(a / b) は以下の式で計算できる：
*		a / b - (int)(a % b < 0)
*/


