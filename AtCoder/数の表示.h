#pragma once
#include "header.h"
// ■■■■■ 数の表示 ■■■■■


//【桁の数の取得】O(log n)
/*
* n を b 進表記したときの桁の数字を上位桁から順に並べたリストを返す．
*
* 制約：|b| ≧ 2
*/
vi integer_digits(ll n, ll b = 10) {
	// verify : https://atcoder.jp/contests/abc105/tasks/abc105_c

	Assert(abs(b) >= 2);

	// n = 0 の場合の例外処理
	if (n == 0) return vi{ 0 };

	// mod |b| を取れば最下位桁から順に決定していく．
	vi ds;
	while (n != 0) {
		int d = (int)smod(n, abs(b));
		ds.push_back(d);
		n = (n - d) / b;
	}

	// 上位桁から順になるように並べ直す．
	reverse(all(ds));

	return ds;
}


//【桁の数の取得（桁数固定）】O(log n)
/*
* n を len 桁で b 進表記したときの桁の数字を上位桁から順に並べたリストを返す．
*
* 制約：|b| ≧ 2
*/
vi integer_digits(ll n, int len, ll b = 10) {
	// verify : https://yukicoder.me/problems/no/327

	Assert(abs(b) >= 2);

	// mod |b| を取れば最下位桁から順に決定していく．
	vi ds(len);
	rep(i, len) {
		int d = (int)smod(n, abs(b));
		ds[len - 1 - i] = d;
		n = (n - d) / b;
	}

	return ds;
}


//【桁の数からの復元】O(n)
/*
* b 進表記で上位桁から順に ds[0..n) が並んだ数の値を返す．
*/
ll from_digits(const vi& ds, ll b = 10) {
	// verify : https://atcoder.jp/contests/abc105/tasks/abc105_c

	int n = sz(ds);

	ll res = 0, powb = 1;
	repir(i, n - 1, 0) {
		res += ds[i] * powb;
		powb *= b;
	}

	return res;
}


//【桁の数からの復元（文字列）】O(n)
/*
* b 進表記で表された数 s[0..n) の値を返す．桁の '0' は zero とする．
*/
template <class T>
T from_digits(const string& s, int b = 10, char zero = '0') {
	// verify : https://atcoder.jp/contests/abc242/tasks/abc242_e

	T res = 0, powb = 1;

	int n = sz(s);
	repir(i, n - 1, 0) {
		res += (s[i] - zero) * powb;
		powb *= b;
	}

	return res;
}


//【数字和】O(log n)
/*
* 非負整数 n を B 進表記したときの桁の数字の和を返す．
*
* 制約：B ≧ 2
*/
ll digit_sum(ll n, ll B = 10) {
	// verify : https://atcoder.jp/contests/tessoku-book/tasks/tessoku_book_ed

	Assert(B >= 2);

	ll sum = 0;
	while (n > 0) {
		sum += n % B;
		n /= B;
	}
	return sum;
}


//【数字和と繰り上がり】
/*
* B 進表記での数字和を s で表す．x + y の B 進での繰り上がりが k 回あるとき以下が成り立つ：
*	s(x + y) = s(x) + s(y) - (B - 1)k
* 
* verify : https://atcoder.jp/contests/arc144/tasks/arc144_a
*/


//【繰り上がり回数の別表現】
/*
* B 進表記された非負整数 n に対し，s(n) を
*	s(n) := Σi∈[0..∞) floor(n / B^i)
* と定める．このとき，x + y の B 進での繰り上がり回数は
*	s(x + y) - (s(x) + s(y))
* で与えられる．
* 
*（証明）floor(n / B^i) は，n の B^[i..∞) の位のみを抜き出した数を表すので，
*	floor((x+y) / B^i) - (floor(x / B^i) + floor(y / B^i))
* は B^(i-1) の位から B^i の位への繰り上がりの量を表す．これを全ての桁にわたって加え合わせている．
* 
* verify : https://judge.yosupo.jp/problem/multivariate_convolution
*/


//【数字根】
/*
* 正の数 n を B 進表記したときの数字根は以下で与えられる：
*	(n - 1) mod (B - 1) + 1
* 
* verify : https://yukicoder.me/problems/no/1252
*/


//【二進数の繰り上がり[下がり] と XOR】
/*
* 非負整数 a, b が二進数表示されているものとするとき，
*	a + b で繰り上がりが起こらない ⇔ a + b = a XOR b
*	a - b で繰り下がりが起こらない ⇔ a - b = a XOR b
* 
* verify : https://yukicoder.me/problems/no/1674
* verify : https://atcoder.jp/contests/abc138/tasks/abc138_f
*/


//【連続自然数の桁数の和】
/*
* 連続自然数 [l..r) について，l, r の B 進表示での桁数がそれぞれ a, b のとき，
*	Σ_i∈[l..r) (B 進表示での i の桁数) = (b r - a l) - (B^b - B^a) / (B - 1)
* 
* verify : https://atcoder.jp/contests/arc090/tasks/arc090_d
*/


//【倍数の数え上げ（数字指定）】O(b^2 log n)
/*
* 桁の数に c⊂[1..9] しか含まない n 桁の正の b の倍数の個数を返す．
*
*（ダブリングで高速化した DP）
*/
mint count_restricted_multiplier(ll n, const vi& c, int b) {
	// verify : https://atcoder.jp/contests/typical90/tasks/typical90_e

	int k = sz(c);
	const int m = msb(n) + 1;

	// pow10[i] : 10^(2^i) mod b
	vi pow10(m + 1);
	pow10[0] = 10 % b;

	rep(i, m) pow10[i + 1] = (pow10[i] * pow10[i]) % b;

	// dp_pow2[i][j] : 2^i 桁で =j (mod b) となる数の個数
	vvm dp_pow2(m + 1, vm(b));
	rep(i, k) dp_pow2[0][c[i] % b]++;

	// 配る DP
	rep(i, m) {
		// j1 : 上 2^i 桁，j2 : 下 2^i 桁の mod b の値
		rep(j1, b) {
			rep(j2, b) {
				int nj = (j1 * pow10[i] + j2) % b;
				dp_pow2[i + 1][nj] += dp_pow2[i][j1] * dp_pow2[i][j2];
			}
		}
	}

	// n の 2^i の位より下の桁だけを読んだ数を n(i) と書くことにする．
	// dp[i][j] : n(i) 桁の数で j (mod b) となる数の個数
	vvm dp(m + 1, vm(b));
	dp[0][0] = 1;

	// 配る DP
	int p = 1; // 10^n(i) mod b
	rep(i, m) {
		if (n & 1LL) {
			// j1 : 上 2^i 桁，j2 : 下 n(i) 桁の mod b の値
			rep(j1, b) {
				rep(j2, b) {
					dp[i + 1][(j1 * p + j2) % b] += dp_pow2[i][j1] * dp[i][j2];
				}
			}
			p = (p * pow10[i]) % b;
		}
		else {
			rep(j2, b) {
				dp[i + 1][j2] = dp[i][j2];
			}
		}
		n /= 2;
	}

	return dp[m][0];
}


