#pragma once
#include "header.h"
// ■■■■■ ダブリング ■■■■■


//【累乗】O(log n)
/*
* モノイド (S, op, e) の元 x の n 乗を返す．
* 
*（繰り返し二乗法）
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


//【等比数列の和】O(log n)
/*
* 半環 (S, add, o, mul, e) の元 r について Σi∈[0..n) r^i を返す．
*
*（繰り返し二乗法）
*/
template <class S, S(*add)(S, S), S(*o)(), S(*mul)(S, S), S(*e)()>
S geometric_series(S r, ll n) {
	// verify : https://csacademy.com/contest/iati-shumen-2017-day-1/task/superstition/statement/
	
	// pow2 = r^(2^i), sumpow2 = 1 + r + ... + r^((2^i) - 1)
	S res(o()), pow2 = r, sumpow2(e());

	while (n > 0) {
		if (n & 1LL) res = add(mul(res, pow2), sumpow2);
		sumpow2 = add(sumpow2, mul(sumpow2, pow2));
		pow2 = mul(pow2, pow2);
		n /= 2;
	}

	return res;
}


//【写像の合成】
/*
* Map_composite(vi s, ll k_max) : O(n log(k_max))
*	[0..n) 上の写像 i -> s[i] で初期化する．
*	以降で apply() に渡す k の最大値を k_max とする．
*
* int apply(int x, ll k) : O(log(k_max))
*	s^k[x] を返す．
*
* ll max_right(int x, function<bool(int)>& okQ) : O(log(k_max))
*	okQ(s^k[x]) = true かつ okQ(s^(k+1)[x]) = false なる k を返す．
*
*（ダブリング）
*/
class Map_composite {
	int n, K;

	// dp[i][j] : s^(2^i)[j]
	vvi dp;

public:
	// [0..n) 上の写像 i -> s[i] で初期化する．
	Map_composite(const vi& s, ll k_max = 1LL << 62) : n(sz(s)), K(msb(k_max) + 1), dp(K, vi(n)) {
		// verify : https://atcoder.jp/contests/abc212/tasks/abc212_f

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
	int apply(int x, ll k) const {
		// verify : https://atcoder.jp/contests/abc212/tasks/abc212_f

		rep(i, K) {
			if (k & (1LL << i)) {
				x = dp[i][x];
			}
		}
		return x;
	}

	// okQ[s^k[x]] = true かつ okQ[s^(k+1)[x]] = false なる k を返す．
	ll max_right(int x, function<bool(int)>& okQ) const {
		// verify : https://atcoder.jp/contests/arc060/tasks/arc060_c

		ll res = 0;
		repir(i, K - 1, 0) {
			res <<= 1;
			if (okQ(dp[i][x])) {
				res++;
				x = dp[i][x];
			}
		}
		return res;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Map_composite& mc) {
		rep(i, mc.K) {
			os << (1LL << i) << ":" << endl;
			rep(j, mc.n) os << mc.dp[i][j] << " "; os << endl;
		}
		return os;
	}
#endif
};


//【写像の累積和】
/*
* Map_accumulate(vi s, vT f, ll k_max) : O(n log(k_max))
*	[0..n) 上の写像 i -> s[i] と関数 i -> f[i] で初期化する．
*	以降で accumulate() に渡す k の最大値を k_max とする．
*
* T accumulate(T x, ll k) : O(log(k_max))
*	Σt=[0..k) f(s^t[x]) を返す．
*
*（ダブリング）
*/
template <class T>
struct Map_accumulate {
	// verify : https://atcoder.jp/contests/abc241/tasks/abc241_e

	int n, K;

	// dp[i][j] : s^(2^i)[j]
	vvi dp;

	// dp_acc[i][j] : Σt=[0..2^i) f(s^t[j])
	using vT = vector<T>; using vvT = vector<vT>;
	vvT dp_acc;

	// [0..n) 上の写像 i -> s[i] と関数 i -> f[i] で初期化する．
	Map_accumulate(const vi& s, const vT& f, ll k_max = 1LL << 62)
		: n(sz(s)), K(msb(k_max) + 1), dp(K, vi(n)), dp_acc(K, vT(n))
	{
		// s^(2^0)[j] = s[j]
		rep(j, n) dp[0][j] = s[j];

		// Σt=[0..2^0) f(s^t[j]) = f(j)
		rep(j, n) dp_acc[0][j] = f[j];

		rep(i, K - 1) {
			rep(j, n) {
				// 例：
				// s^8[j] = s^4[ s^4[j] ]
				dp[i + 1][j] = dp[i][dp[i][j]];

				// 例：
				// f(s^0[j]) + f(s^1[j]) + f(s^2[j]) + f(s^3[j])
				//	+ f(s^4[j]) + f(s^5[j]) + f(s^6[j]) + f(s^7[j])
				// = f(s^0[j]) + f(s^1[j]) + f(s^2[j]) + f(s^3[j])
				//	+ f(s^0[ s^4[j] ]) + f(s^1[ s^4[j] ]) + f(s^2[ s^4[j] ]) + f(s^3[ s^4[j] ])
				dp_acc[i + 1][j] = dp_acc[i][j] + dp_acc[i][dp[i][j]];
			}
		}
	}

	// Σt=[0..k) f(s^t[x]) を返す．
	T accumulate(T x, ll k) const {
		// 例：
		// f(s^0[x]) + f(s^1[x]) + f(s^2[x]) + f(s^3[x]) + f(s^4[x]) + f(s^5[x])
		// = f(s^0[x]) + f(s^1[x])
		//	+ f(s^0[ s^2[x] ]) + f(s^1[ s^2[x] ]) + f(s^2[ s^2[x] ]) + f(s^3[ s^2[x] ])

		T res = 0;
		rep(i, K) {
			if (k & (1LL << i)) {
				res += dp_acc[i][x];
				x = dp[i][x];
			}
		}
		return res;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Map_accumulate& ma) {
		rep(i, ma.K) {
			os << (1LL << i) << ":" << endl;
			rep(j, ma.n) os << ma.dp[i][j] << " "; os << endl;
			rep(j, ma.n) os << ma.dp_acc[i][j] << " "; os << endl;
		}
		return os;
	}
#endif
};


//【倍数の数え上げ（桁の数の制限あり）】O(b^2 log n)
/*
* 桁の数に 1 <= c[i] <= 9 しか含まない n 桁の正の b の倍数の個数を返す．
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

	rep(i, m) {
		pow10[i + 1] = (pow10[i] * pow10[i]) % b;
	}

	// dp_pow2[i][j] : 2^i 桁で j (mod b) となる数の個数
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


