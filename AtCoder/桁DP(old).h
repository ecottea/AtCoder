#pragma once
#include "header.h"
// ■■■■■ 桁 DP ■■■■■


//【数字和倍数指定（数え上げ）】O(n m b)
/*
* b 進数で n 桁の数 num 以下の非負の整数で，数字和が m の倍数であるものの個数を返す．
*
*（桁 DP，smaller フラグ）
*/
mint count_digit_sum(const string& num, int m) {
	// verify : https://atcoder.jp/contests/dp/tasks/dp_s

	int n = sz(num);
	const int b = 10;

	// dp[i][sml][k] : 以下の条件を満たす num 以下の数が何通りあるか．
	//	i   : 上からの桁 d[0..i) まで決まっている．
	//	sml : d[0..i) だけで num より小さいことが確定していれば 1，さもなくば 0
	//	k   : Σd[0..i) (mod m)
	vvvm dp(n + 1, vvm(2, vm(m)));
	dp[0][0][0] = 1;

	// 上の桁から順に配る DP
	rep(i, n) {
		// x : num の上から i 桁目の数
		int x = num[i] - '0';

		rep(k, m) {
			// もう num より小さいことが確定している場合，
			// i + 1 桁目の数は何であっても num より小さいと確定する．
			rep(t, b) {
				dp[i + 1][1][(k + t) % m] += dp[i][1][k];
			}

			// まだ num より小さいと確定していない場合，
			// i + 1 桁目の数によって以下の場合分けが生じる：
			//		x 未満であれば num より小さいと確定する．
			//		x ちょうどであればまだ確定しない．
			//		x を超える数は選ぶことができない．
			rep(t, x) {
				dp[i + 1][1][(k + t) % m] += dp[i][0][k];
			}
			dp[i + 1][0][(k + x) % m] += dp[i][0][k];
		}
	}

	return dp[n][0][0] + dp[n][1][0];
}


//【広義昇順（数え上げ）】O(n b^2)
/*
* b 進数で n 桁の数 num 以下の非負の整数で，数字が広義昇順に並んでいるものの個数を返す．
*
*（桁 DP，smaller フラグ）
*/
mint count_increasing_number(const string& num) {
	int n = sz(num);
	const int b = 10;

	// dp[i][sml][k] : 以下の条件を満たす num 以下の数が何通りあるか．
	//	i   : 上からの桁 d[0..i) まで決まっている．
	//	sml : d[0..i) だけで num より小さいことが確定していれば 1，さもなくば 0
	//	k   : d[i-1]
	vvvm dp(n + 1, vvm(2, vm(10)));
	dp[0][0][0] = 1;

	// 上の桁から順に配る DP
	rep(i, n) {
		// x : num の上から i 桁目の数
		int x = num[i] - '0';

		rep(k, b) {
			// もう num より小さいことが確定している場合，
			// t = d[i] は何であっても num より小さいと確定する．
			repi(t, k, b - 1) {
				dp[i + 1][1][t] += dp[i][1][k];
			}

			// まだ num より小さいと確定していない場合，
			// t = d[i] によって以下の場合分けが生じる：
			//		d[i] < x であれば num より小さいと確定する．
			//		d[i] = x であればまだ確定しない．
			//		d[i] > x とは選べない．
			repi(t, k, x - 1) {
				dp[i + 1][1][t] += dp[i][0][k];
			}
			if (x >= k) dp[i + 1][0][x] += dp[i][0][k];
		}
	}

	mint res = 0;
	rep(k, b) res += dp[n][0][k] + dp[n][1][k];

	return res;
}


//【桁の数字の分布】O(n b)
/*
* b 進数で n 桁の数 num 以下の正の整数すべてについて，
* 桁の数字に現れる数字 t の個数を cnt[t] に格納する．
*
*（桁 DP，smaller フラグ，leading-zero フラグ）
*/
void digits_distribution(const string& num, vm& cnt) {
	// verify : https://atcoder.jp/contests/abc029/tasks/abc029_d

	int n = sz(num);
	const int b = 10;
	cnt.resize(b);

	// dp[i][sml][lz][t] : 以下の条件を満たす数の桁の数字に含まれる t の個数
	//                     ただし t = b のときは条件を満たす数の個数を表すものとする．
	//	i   : 上からの桁 d[0..i) まで決まっている．
	//	sml : d[0..i) だけで num より小さいことが確定していれば 1，さもなくば 0
	//	lz  : d[0..i) 全てが 0 ならば 1，さもなくば 0
	vector<vvvm> dp(n + 1, vvvm(2, vvm(2, vm(b + 1))));
	dp[0][0][1][b] = 1;

	// 上の桁から順に配る DP
	rep(i, n) {
		// x : num の上から i 桁目の数（0-indexed）
		int x = num[i] - '0';

		// smaller = true, leading zero = true の場合
		dp[i + 1][1][1][b] += dp[i][1][1][b];
		repi(t, 1, b - 1) {
			dp[i + 1][1][0][t] += dp[i][1][1][b];
			dp[i + 1][1][0][b] += dp[i][1][1][b];
		}

		// smaller = true, leading zero = false の場合
		rep(t, b) {
			dp[i + 1][1][0][t] += dp[i][1][0][t] * b;
			dp[i + 1][1][0][t] += dp[i][1][0][b];
			dp[i + 1][1][0][b] += dp[i][1][0][b];
		}

		// smaller = false, leading zero = true の場合
		dp[i + 1][1][1][b] += dp[i][0][1][b];
		repi(t, 1, x - 1) {
			dp[i + 1][1][0][t] += dp[i][0][1][b];
			dp[i + 1][1][0][b] += dp[i][0][1][b];
		}
		dp[i + 1][0][0][x] += dp[i][0][1][b];
		dp[i + 1][0][0][b] += dp[i][0][1][b];

		// smaller = false, leading zero = false の場合
		rep(t, b) {
			dp[i + 1][1][0][t] += dp[i][0][0][t] * x;
			if (t < x) {
				dp[i + 1][1][0][t] += dp[i][0][0][b];
				dp[i + 1][1][0][b] += dp[i][0][0][b];
			}

			dp[i + 1][0][0][t] += dp[i][0][0][t];
			if (t == x) {
				dp[i + 1][0][0][t] += dp[i][0][0][b];
				dp[i + 1][0][0][b] += dp[i][0][0][b];
			}
		}

		//dump(i + 1);
		//dump("smaller && leading zero:");
		//dump(dp[i + 1][1][1]);
		//dump("smaller && !leading zero:");
		//dump(dp[i + 1][1][0]);
		//dump("!smaller && leading zero:");
		//dump(dp[i + 1][0][1]);
		//dump("!smaller && !leading zero:");
		//dump(dp[i + 1][0][0]);
	}

	rep(t, b) {
		cnt[t] = dp[n][0][0][t] + dp[n][1][0][t];
	}
}


//【差の最小化】O(n 2^b b)
/*
* b 進数で n 桁の正の整数 num が与えられる．桁に含まれる数字の集合が
* set であるような num 以下の正の整数と num との差の最小値を dif[set] に返す．
*
*（桁 DP，smaller フラグ，leading-zero フラグ）
*/
void minimize_difference(const string& num, vl& dif) {
	// verify : https://atcoder.jp/contests/code-festival-2014-quala/tasks/code_festival_qualA_d

	int n = sz(num);
	const int b = 10;
	dif.resize(1 << b);

	// dp[i][sml][lz][set] : 以下の条件を満たす数の個数
	//	i   : 上からの桁 d[0..i) まで決まっている．
	//	sml : d[0..i) だけで num より小さいことが確定していれば 1，さもなくば 0
	//	lz  : d[0..i) 全てが 0 ならば 1，さもなくば 0
	//	set : 桁に含まれる数字の集合
	vector<vvvl> dp(n + 1, vvvl(2, vvl(2, vl(1 << b, INFL / b))));
	dp[0][0][1][0] = 0;

	// 上の桁から順に配る DP
	rep(i, n) {
		// x : num の上から i 桁目の数（0-indexed）
		int x = num[i] - '0';

		repb(set, b) {
			// smaller = true, leading zero = true の場合
			chmin(dp[i + 1][1][1][set], dp[i][1][1][set] * b + (x - 0));
			repi(d, 1, b - 1) {
				chmin(dp[i + 1][1][0][set | (1 << d)], dp[i][1][1][set] * b + (x - d));
			}

			// smaller = true, leading zero = false の場合
			repi(d, 0, b - 1) {
				chmin(dp[i + 1][1][0][set | (1 << d)], dp[i][1][0][set] * b + (x - d));
			}

			// smaller = false, leading zero = true の場合
			chmin(dp[i + 1][1][1][set], dp[i][0][1][set] * b + (x - 0));
			repi(d, 1, x - 1) {
				chmin(dp[i + 1][1][0][set | (1 << d)], dp[i][0][1][set] * b + (x - d));
			}
			chmin(dp[i + 1][0][0][set | (1 << x)], dp[i][0][1][set] * b + (x - x));

			// smaller = false, leading zero = false の場合
			repi(d, 0, x - 1) {
				chmin(dp[i + 1][1][0][set | (1 << d)], dp[i][0][0][set] * b + (x - d));
			}
			chmin(dp[i + 1][0][0][set | (1 << x)], dp[i][0][0][set] * b + (x - x));
		}
	}

	repb(set, b) {
		dif[set] = min(dp[n][0][0][set], dp[n][1][0][set]);
	}
}


