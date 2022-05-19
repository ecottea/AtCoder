#pragma once
#include "header.h"
// ■■■■■ 混合基数 ■■■■■


//【混合基数表示】
/*
* 最下位を 0 桁目とし，[0..n) 桁目の重みが a[0..n) で与えられる混合基数について，
* 値 val を混合基数表示したときの i 桁目の数字を d[i] に格納する．
* また b[0..n) は i 桁目の数字の取りうる範囲が [0..b[i]) であることを表す．
*
* 制約：a[0] = 1，a[i] は a[i+1] の真の約数
*/
void mixed_radix(const vl& a, ll val, vl& d, vl& b) {
	// verify : https://atcoder.jp/contests/abc231/tasks/abc231_e

	int n = sz(a);

	b.resize(n);
	rep(i, n - 1) b[i] = a[i + 1] / a[i];
	b[n - 1] = INFL;

	d.resize(n);
	rep(i, n) {
		d[i] = val % b[i];
		val /= b[i];
	}
}


//【混合基数，下から桁 DP，桁上げフラグ，スコア最小化】O(n)
/*
* 混合基数 b[0..n) で表現 num[0..n) をもつ数について，全ての整数 d についての
* (d の数字和) + (num + d の数字和) の最小値を返す．
*/
ll minimize_pair_digit_sum(const vl& num, const vl& b) {
	// verify : https://atcoder.jp/contests/abc231/tasks/abc231_e

	int n = sz(num);

	// dp[i][f] : 以下の条件を満たす数の最小スコア：
	//	i : 下からの桁 d[0..i) まで決まっている．
	//	f : d[i+1] への桁上げがあるなら 1，さもなくば 0（桁上げフラグ）
	vvl dp(n + 1, vl(2, INFL));
	dp[0][0] = 0;

	// 下の桁から順に配る DP
	rep(i, n) {
		ll x = num[i];

		rep(f, 2) {
			// d の i 桁目を 0 にする場合
			chmin(dp[i + 1][0], dp[i][f] + (x + f));

			// num + d の i 桁目を 0 にする場合
			chmin(dp[i + 1][1], dp[i][f] + (b[i] - (x + f)));

			// この 2 つの場合以外はスコアを最小にすることはないので無視できる．
			// おつり問題でいうと，渡した硬貨がそのまま返ってくることに対応する．
		}
	}

	return dp[n][0];
}


//【冗長混合基数表示の列挙】O(?)（二進なら val = 400 くらいまで動く）
/*
* 最下位を 0 桁目とし，[0..n) 桁目の重みが a[0..n) で与えられる混合基数について，
* 値 val の i 番目の冗長混合基数表示の j 桁目の数字を ds[i][j] に格納する．
* 冗長混合基数表示では，桁の数字に任意の非負整数を認める．
*
* 制約：a[0] = 1，a[i] は a[i+1] の真の約数
*/
void enumerate_redundant_mixed_radix(const vl& a, ll val, vvl& ds) {
	int n = sz(a);
	vl d(n);

	function<void(int)> rf = [&](int j) {
		// a[0] = 1 の位に立つ数は残り全部に確定．
		if (j == 0) {
			d[0] = val;
			ds.push_back(d);
			d[0] = 0;

			return;
		}

		// q : a[j] の位に立つ数の最大値
		ll q = val / a[j];

		repi(k, 0, q) {
			val -= k * a[j];
			d[j] = k;

			rf(j - 1);

			d[j] = 0;
			val += k * a[j];
		}
	};

	rf(n - 1);
}


//【フィボナッチ進法表示】
/*
* Fibonacci_representation(ll n) : O(log n)
*	n 以下の整数のフィボナッチ進法表示を求められるよう初期化する．
*
* ll fibonacci(int i) : O(1)
*	i 番目のフィボナッチ数 fib[i] を得る（fib[0] = 0, fib[1] = 1 とする．）
*
* get_digits(ll n, vi& ds) : O(log n)
*	n のフィボナッチ進法表示を ds に格納する．（下位から順）
*	桁の数は {0, 1} であり，1 は連続せず，下 2 桁は常に "00" である．
*/
class Fibonacci_representation {
	// verify : https://atcoder.jp/contests/arc122/tasks/arc122_c

	int m;
	vl fib;

public:
	// n 以下の整数のフィボナッチ進法表示を求められるよう初期化する．
	Fibonacci_representation(ll n) {
		fib = vl{ 0, 1 }; m = 2;
		while (fib[m - 1] <= n) {
			fib.push_back(fib[m - 1] + fib[m - 2]);
			m++;
		}
	}

	// i 番目のフィボナッチ数 fib[i] を得る（fib[0] = 0, fib[1] = 1 とする．）
	ll fibonacci(int i) {
		assert(0 <= i && i < m);

		return fib[i];
	}

	// n のフィボナッチ進法表示を ds に格納する．（下位から順）
	void get_digits(ll n, vi& ds) {
		if (n == 0) {
			ds = vi{ 0 };
			return;
		}

		int i = 2;
		while (fib[i] <= n) i++;
		ds.resize(i);
		i--;

		while (i >= 2) {
			if (fib[i] <= n) {
				ds[i] = 1;
				n -= fib[i];
			}
			else ds[i] = 0;
			i--;
		}
		ds[1] = ds[0] = 0;
	}
};


