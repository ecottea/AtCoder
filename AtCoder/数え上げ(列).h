#pragma once
#include "header.h"
#include "整除算.h"
// ■■■■■ 数え上げ（列） ■■■■■


//【貪欲増加部分列長が指定された列の数え上げ】
/*
* 各要素が [0..k) の元である列 a[0..n) で，貪欲増加部分列の長さが k であるものの個数は
* 第二種スターリング数 s(n, k) に等しい．
* 
* 証明：各要素が [0..k) の元である列 a[0..n) で，貪欲増加部分列の長さが k であるものをとる．
* 各 j∈[0..k) について a[i]=j なる i の集合 a^(-1)(j)⊂[0..n) をとれば，
* [0..n) のちょうど k 個の集合への分割と 1:1 に対応する．
* 
* 例えば n=4, k=3 のときの [0,1,2,0] は，集合の分割 {0,3}∪{1}∪{2} と対応する．
*/


//【長さ 2 の倍数列の数え上げ】O(√n)
/*
* 1 <= x | y <= n を満たす組 (x, y) の個数を返す．
*
* 利用：【商列挙】
*/
template<class T>
T count_multiple_2chain(ll n) {
	// 参考 : http://oeis.org/A006218
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
template<class T>
T count_multiple_3chain(ll n) {
	// 参考 : http://oeis.org/A061201
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


//【真の倍数列の数え上げ】O(n (log n)^2)
/*
* 各 i∈[0..n] について，各項が n 以下の自然数である真の倍数列で，
* 初項が i で項数が j であるものの個数を cnt[i][j] に格納し cnt を返す．
*/
vvm count_multiple_chain(int n) {
	// verify : https://atcoder.jp/contests/arc116/tasks/arc116_c

	vvm dp(n + 1);

	repir(i, n, 1) {
		int j_max = msb(n / i) + 1;
		dp[i].resize(j_max + 1);

		dp[i][1] = 1;
		repi(j, 2, j_max) {
			for (int k = 2; (ll)i * k <= n && sz(dp[i * k]) >= j; k++) {
				dp[i][j] += dp[i * k][j - 1];
			}
		}
	}

	return dp;
}


