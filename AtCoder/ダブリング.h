#pragma once
#include "header.h"
// ■■■■■ ダブリング ■■■■■


//【累乗】O(log n)
/*
* モノイド (S, op, e) の元 x の n 乗を返す．
*/
template <class S, S(*op)(S, S), S(*e)()>
S pow(const S& x, ll n) {
	// verify : https://atcoder.jp/contests/abc013/tasks/abc013_4

	S res(e()), pow2 = x;
	while (n > 0) {
		if ((n & 1) != 0) res = op(res, pow2);
		pow2 = op(pow2, pow2);
		n /= 2;
	}
	return res;
}


//【写像の合成】
/*
* Map_composite(vi s) : O(n log(INFL))
*	[0..n) 上の写像 i -> s[i] で初期化する．
*
* int apply(int x, ll k) : O(log(INFL))
*	s^k[x] を返す．
*
*（ダブリング）
*/
struct Map_composite {
	// verify : https://atcoder.jp/contests/abc030/tasks/abc030_d

	const int K = 62; // k の最大値に応じて調整
	int n;

	// dp[i][j] : s^(2^i)[j]
	vvi dp;

	// [0..n) 上の写像 i -> s[i] で初期化する．
	Map_composite(const vi& s) : n(sz(s)), dp(K, vi(n)) {
		// s^(2^0)[j] = s[j]
		rep(j, n) dp[0][j] = s[j];

		rep(i, K - 1) {
			rep(j, n) {
				// s^(2^(i+1))[j] = s^(2^i)[ s^(2^i)[j] ]
				dp[i + 1][j] = dp[i][dp[i][j]];
			}
		}
	}

	// s^k[x] を返す．
	int apply(int x, ll k) {
		rep(i, K) {
			if (k & (1LL << i)) {
				x = dp[i][x];
			}
		}
		return x;
	}
};


//【写像の累積和】
/*
* Map_accumulate(vi s, vT f) : O(n log(INFL))
*	[0..n) 上の写像 i -> s[i] と関数 i -> f[i] で初期化する．
*
* T accumulate(T x, ll k) : O(log(INFL))
*	Σt=[0..k) f(s^t[x]) を返す．
*
*（ダブリング）
*/
template <class T> struct Map_accumulate {
	// verify : https://atcoder.jp/contests/abc241/tasks/abc241_e

	const int K = 62; // k の最大値に応じて調整
	int n;

	// dp[i][j] : s^(2^i)[j]
	vvi dp;

	// dp_acc[i][j] : Σt=[0..2^i) f(s^t[j])
	vector<vector<T>> dp_acc;

	// [0..n) 上の写像 i -> s[i] と関数 i -> f[i] で初期化する．
	Map_accumulate(const vi& s, const vector<T>& f) : n(sz(s)), dp(K, vi(n)), dp_acc(K, vector<T>(n)) {
		// s^(2^0)[j] = s[j]
		rep(j, n) dp[0][j] = s[j];

		// Σt=[0..2^0) f(s^t[j]) = f(j)
		rep(j, n) dp_acc[0][j] = f[j];

		rep(i, K - 1) {
			rep(j, n) {
				// s^(2^(i+1))[j] = s^(2^i)[ s^(2^i)[j] ]
				dp[i + 1][j] = dp[i][dp[i][j]];

				// Σt=[0..2^(i+1)) f(s^t[j])
				// = Σt=[0..2^i) f(s^t[j]) + Σt=[2^i..2^(i+1)) f(s^t[j])
				// = Σt=[0..2^i) f(s^t[j]) + Σt=[0..2^i) f(s^(t + 2^i)[j])
				// = Σt=[0..2^i) f(s^t[j]) + Σt=[0..2^i) f(s^t[ s^(2^i)[j] ])
				dp_acc[i + 1][j] = dp_acc[i][j] + dp_acc[i][dp[i][j]];
			}
		}
	}

	// Σt=[0..k) f(s^t[x]) を返す．
	T accumulate(T x, ll k) {
		// k = 6 の場合の計算例：
		//	Σt=[0..6) f(s^t[x])
		//	= Σt=[0..2) f(s^t[x]) + Σt=[2..6) f(s^t[x])
		//	= Σt=[0..2) f(s^t[x]) + Σt=[0..4) f(s^(t + 2)[x])
		//	= Σt=[0..2) f(s^t[x]) + Σt=[0..4) f(s^t)[ s^2[x] ])

		T res = 0;
		rep(i, K) {
			if (k & (1LL << i)) {
				res += dp_acc[i][x];
				x = dp[i][x];
			}
		}
		return res;
	}
};


