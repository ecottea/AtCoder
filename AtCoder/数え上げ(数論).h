#pragma once
#include "header.h"
#include "ディリクレ畳込み.h"
// ■■■■■ 数え上げ（数論） ■■■■■


//【素数計数関数】O(N^(3/4))
/*
* N 以下の素数の個数 π(N) を返す．
*
*（Lucy DP）
*/
ll prime_pi(ll N) {
	// 参考 : https://rsk0315.hatenablog.com/entry/2021/05/18/015511
	// verify : https://judge.yosupo.jp/problem/counting_primes

	//【方法】
	// j 番目（1-indexed）の素数を p[j](≦ √N) と表し，dp_j[i] を
	//		dp_j[i]
	//		= ([2..i] 内の "素数または p[j] 以下の素因数をもたない合成数" の個数)
	//		= (エラトステネスの篩において，[2..i] 内の p[j] 以下の素数で篩い終えた後残っている数の個数)
	// とおく．dp_j[i] の求め方を考える．
	// 
	// p[j]^2 > i のときは，[2..i] 内には p[j] で新たに篩われる数は無いので
	//		dp_j[i] = dp_(j-1)[i]
	// である．
	// 
	// p[j]^2 ≦ i のときは，[2..i] 内の p[j] で新たに篩われる数は
	//		(i) p[j-1] 以下の素因数を持たない（まだ篩われていない）
	//		(ii) 2 p[j] 以上の p[j] の倍数（次に篩われる）
	// という条件を共に満たす数である．
	// 
	// [2..i] に条件 (i), (ii) を課す代わりに，全体を p[j] で割って，
	//		[2..i/p[j]] 内の p[j-1] 以下の素因数を持たない数
	// を数えても個数は変わらない．そのような数は，[2..i/p[j]] 内の
	//		(iii) p[j-1] 以下の素数で篩い終えた後残っている
	//		(iv) p[j-1] 以下の素数ではない
	// という条件を共に満たす数であり，!(iv) ⇒ (iii) に注意するとその個数は
	//		dp_(j-1)[i/p[j]] - dp_(j-1)[p[j-1]]
	// と表される．
	//
	// 以上をまとめると，DP の初期化は
	//		dp_0[i] = i - 1
	// で行い，遷移式は
	//		dp_j[i] = dp_(j-1)[i] （p[j]^2 > i のとき）
	//		dp_j[i] = dp_(j-1)[i] - (dp_(j-1)[i/p[j]] - dp_(j-1)[p[j-1]]) （p[j]^2 ≦ i のとき）
	// を用いれば良い．

	//【備考】
	// 途中で DP テーブルを参照すれば，K-rough number の数え上げにも対応できる．

	if (N <= 1) return 0;

	int m = (int)(sqrt(N) + 1e-9);

	// dp_j[i] : [2..i] 内の p[j] 以下の素数で篩い終えた後残っている数の個数
	//	dp_l_j[i] : dp_j[i]
	//	dp_h_j[d] : dp_j[N/d]
	vl dp_l(m + 1), dp_h(m + 1);
	repi(i, 1, m) {
		dp_l[i] = i - 1;
		dp_h[i] = N / i - 1;
	}

	repi(p, 2, m) {
		// cnt_p1 : p-1 以下の素数の個数
		ll cnt_p1 = dp_l[p - 1];

		// p が素数でなければ次の p へ
		if (dp_l[p] == cnt_p1) continue;

		repi(d, 1, m) {
			// p^2 > N/d なら更新不要
			if (p > (N / d) / p) break;

			if (d <= m / p) dp_h[d] -= dp_h[d * p] - cnt_p1;
			else dp_h[d] -= dp_l[(N / d) / p] - cnt_p1;
		}

		repir(i, m, 1) {
			// p^2 > i なら更新不要
			if (p > i / p) break;

			dp_l[i] -= dp_l[i / p] - cnt_p1;
		}
	}

	return dp_h[1];
}


//【素数計数関数（一括）】O(N^(3/4))
/*
* i 以下の素数の個数を π(i) とし，π[N/d] のリストを返す．
*
*（Lucy DP）
*/
Floor_vector<ll> prime_pi_all(ll N) {
	// 参考 : https://rsk0315.hatenablog.com/entry/2021/05/18/015511
	// verify : https://atcoder.jp/contests/jsc2024-final/tasks/jsc2024_final_b

	//【方法】
	// j 番目（1-indexed）の素数を p[j](≦ √N) と表し，dp_j[i] を
	//		dp_j[i]
	//		= ([2..i] 内の "素数または p[j] 以下の素因数をもたない合成数" の個数)
	//		= (エラトステネスの篩において，[2..i] 内の p[j] 以下の素数で篩い終えた後残っている数の個数)
	// とおく．dp_j[i] の求め方を考える．
	// 
	// p[j]^2 > i のときは，[2..i] 内には p[j] で新たに篩われる数は無いので
	//		dp_j[i] = dp_(j-1)[i]
	// である．
	// 
	// p[j]^2 ≦ i のときは，[2..i] 内の p[j] で新たに篩われる数は
	//		(i) p[j-1] 以下の素因数を持たない（まだ篩われていない）
	//		(ii) 2 p[j] 以上の p[j] の倍数（次に篩われる）
	// という条件を共に満たす数である．
	// 
	// [2..i] に条件 (i), (ii) を課す代わりに，全体を p[j] で割って，
	//		[2..i/p[j]] 内の p[j-1] 以下の素因数を持たない数
	// を数えても個数は変わらない．そのような数は，[2..i/p[j]] 内の
	//		(iii) p[j-1] 以下の素数で篩い終えた後残っている
	//		(iv) p[j-1] 以下の素数ではない
	// という条件を共に満たす数であり，!(iv) ⇒ (iii) に注意するとその個数は
	//		dp_(j-1)[i/p[j]] - dp_(j-1)[p[j-1]]
	// と表される．
	//
	// 以上をまとめると，DP の初期化は
	//		dp_0[i] = i - 1
	// で行い，遷移式は
	//		dp_j[i] = dp_(j-1)[i] （p[j]^2 > i のとき）
	//		dp_j[i] = dp_(j-1)[i] - (dp_(j-1)[i/p[j]] - dp_(j-1)[p[j-1]]) （p[j]^2 ≦ i のとき）
	// を用いれば良い．

	//【備考】
	// 途中で DP テーブルを参照すれば，K-rough number の数え上げにも対応できる．

	int nl = (int)(sqrt(N) + 1e-9);

	// dp_j[i] : [2..i] 内の p[j] 以下の素数で篩い終えた後残っている数の個数
	Floor_vector<ll> dp(N, nl);
	int nh = dp.nh;

	repi(i, 1, nl) dp.set_l(i, i - 1);
	repi(d, 1, nh) dp.set_h(d, N / d - 1);

	// is_prime[i] : i が素数か
	vb is_prime(nl + 1, true);
	is_prime[0] = is_prime[1] = false;

	repi(p, 2, nl) {
		// p が素数でなければ次の p へ
		if (!is_prime[p]) continue;

		// p^2 以上の p の倍数は素数でないと確定する．
		for (ll j = (ll)p * p; j <= nl; j += p) is_prime[j] = false;

		// cnt_p1 : p-1 以下の素数の個数
		ll cnt_p1 = dp.get_l(p - 1);

		repi(d, 1, nh) {
			// p^2 > N/d なら更新不要
			if (p > (N / d) / p) break;

			dp[N / d] -= dp.get_h(d * p) - cnt_p1;
		}

		repir(i, nl, 1) {
			// p^2 > i なら更新不要
			if (p > i / p) break;

			dp[i] -= dp.get_l(i / p) - cnt_p1;
		}
	}

	return dp;
}


//【素数の数え上げ（mod 4）】O(N^(3/4))
/*
* N 以下の (4Z+1 型素数の個数, 4Z+3 型素数の個数) の組を返す．
*
*（Lucy DP）
*/
pll prime_pi_mod4(ll N) {
	if (N <= 2) return { 0, 0 };

	int sqrt_N = (int)(sqrt(N) + 1e-6);

	// S_t(i, p) : [2..i] 内の p 以下の素数で篩い終えた後残っている [t=0 ? 4Z+1 : 4Z+3] 型整数の個数
	//	dp_l_p[t][v] : S_t(i, p)
	//	dp_h_p[t][v] : S_t(N/i, p)
	vvl dp_l(2, vl(sqrt_N + 1)), dp_h(2, vl(sqrt_N + 1));
	repi(i, 1, sqrt_N) {
		// [2..i] 内の 4Z+1 型整数の個数
		dp_l[0][i] = (i - 1) / 4;
		
		// [2..i] 内の 4Z+3 型整数の個数
		dp_l[1][i] = (i + 1) / 4;
		
		// [2..N/i] 内の 4Z+1 型整数の個数
		dp_h[0][i] = (N / i - 1) / 4;
		
		// [2..N/i] 内の 4Z+3 型整数の個数
		dp_h[1][i] = (N / i + 1) / 4;
	}

	for (ll p = 3; p <= sqrt_N; p += 2) {
		// p が素数でなければ次の p へ
		if (dp_l[0][p] + dp_l[1][p] == dp_l[0][p - 1] + dp_l[1][p - 1]) continue;

		int tp = (p % 4 == 3);

		// dp_h[i] = S(N/i, p) の更新
		repi(i, 1, sqrt_N) {
			// p^2 > N/i なら更新不要
			if (p * p > N / i) break;

			if (i * p <= sqrt_N) {
				dp_h[0][i] -= dp_h[0 ^ tp][i * p] - dp_l[0 ^ tp][p - 1];
				dp_h[1][i] -= dp_h[1 ^ tp][i * p] - dp_l[1 ^ tp][p - 1];
			}
			else {
				dp_h[0][i] -= dp_l[0 ^ tp][N / (i * p)] - dp_l[0 ^ tp][p - 1];
				dp_h[1][i] -= dp_l[1 ^ tp][N / (i * p)] - dp_l[1 ^ tp][p - 1];
			}
		}

		// dp_l[i] = S(i, p) の更新
		repir(i, sqrt_N, 1) {
			// p^2 > i なら更新不要
			if (p * p > i) break;

			dp_l[0][i] -= dp_l[0 ^ tp][i / p] - dp_l[0 ^ tp][p - 1];
			dp_l[1][i] -= dp_l[1 ^ tp][i / p] - dp_l[1 ^ tp][p - 1];
		}
	}

	return { dp_h[0][1], dp_h[1][1] };
}


//【素数の数え上げ（mod 4，一括）】O(N^(3/4))
/*
* N 以下の 4Z+1 型素数の個数を π_1(i)，4Z+3 型素数の個数を π_3(i) とし，
* π_1[N/d] のリストと π_3[N/d] のリストの組を返す．
*
*（Lucy DP）
*/
pair<Floor_vector<ll>, Floor_vector<ll>> prime_pi_mod4_all(ll N) {
	int nl = (int)(sqrt(N) + 1e-9);

	// dp1_j[i] : [2..i] 内の p[j] 以下の素数で篩い終えた後残っている 4Z+1 型整数の個数
	// dp3_j[i] : [2..i] 内の p[j] 以下の素数で篩い終えた後残っている 4Z+3 型整数の個数
	Floor_vector<ll> dp1(N, nl), dp3(N, nl);
	int nh = dp1.nh;

	repi(i, 1, nl) {
		dp1.set_l(i, (i - 1) / 4);
		dp3.set_l(i, (i + 1) / 4);
	}
	repi(d, 1, nh) {
		dp1.set_h(d, (N / d - 1) / 4);
		dp3.set_h(d, (N / d + 1) / 4);
	}

	for (int p = 3; p <= nl; p += 2) {
		// p が素数でなければ次の p へ
		if (dp1.get_l(p) + dp3.get_l(p) == dp1.get_l(p - 1) + dp3.get_l(p - 1)) continue;

		// cnt1 : p-1 以下の 4Z+1 型素数の個数
		// cnt3 : p-1 以下の 4Z+3 型素数の個数
		ll cnt1 = dp1.get_l(p - 1);
		ll cnt3 = dp3.get_l(p - 1);

		if (p % 4 == 1) {
			repi(d, 1, nh) {
				// p^2 > N/d なら更新不要
				if ((ll)p * p > N / d) break;

				dp1[N / d] -= dp1.get_h(d * p) - cnt1;
				dp3[N / d] -= dp3.get_h(d * p) - cnt3;
			}

			repir(i, nl, 1) {
				// p^2 > i なら更新不要
				if ((ll)p * p > i) break;

				dp1[i] -= dp1.get_l(i / p) - cnt1;
				dp3[i] -= dp3.get_l(i / p) - cnt3;
			}
		}
		else {
			repi(d, 1, nh) {
				// p^2 > N/d なら更新不要
				if ((ll)p * p > N / d) break;

				dp1[N / d] -= dp3.get_h(d * p) - cnt3;
				dp3[N / d] -= dp1.get_h(d * p) - cnt1;
			}

			repir(i, nl, 1) {
				// p^2 > i なら更新不要
				if ((ll)p * p > i) break;

				dp1[i] -= dp3.get_l(i / p) - cnt3;
				dp3[i] -= dp1.get_l(i / p) - cnt1;
			}
		}
	}

	return { dp1, dp3 };
}


//【無平方数の数え上げ】O(N^(4/9))
/*
* N 以下の無平方数の個数を返す．
*
*（除原理）
*/
ll count_square_free(ll N) {
	// verify : https://judge.yosupo.jp/problem/counting_squarefrees

	int nl = (int)(pow(2. * N, 1. / 3) + 1e-9);
	int nh = (int)sqrt(N / (nl + 1) + 1);

	// isqrt[i] : floor(√i)
	vi isqrt(nl + 1);
	repi(i, 1, nl) isqrt[i] = (int)sqrtl((long double)i);

	// dp_l[i] : i 以下の無平方数の個数
	// dp_h[d] : N/d 以下の無平方数の個数
	vl dp_l(nl + 1), dp_h(nh + 1);

	// ちゃんとやれば O(nl log(log nl)) でできるが，全体の計算量は変わらない．
	repi(i, 1, nl) {
		dp_l[i] = i;

		int m = (int)(pow(2. * i, 2. / 3) + 1e-9);

		int j_max = isqrt[m];
		int k_max = i / (m + 1);

		repi(j, 2, j_max) {
			dp_l[i] -= dp_l[i / j / j];
		}

		repi(k, 1, k_max) {
			dp_l[i] -= (dp_l[k] - dp_l[k - 1]) * (isqrt[i / k] - j_max);
		}
	}

	repir(d, nh, 1) {
		ll i = N / d / d;

		dp_h[d] = i;

		ll m = (ll)(pow(2. * i, 2. / 3) + 1e-9);

		int j_max = (int)sqrtl((long double)m);
		int k_max = (int)(i / (m + 1));

		repi(j, 2, j_max) {
			if (i / j / j <= nl) {
				dp_h[d] -= dp_l[i / j / j];
			}
			else {
				dp_h[d] -= dp_h[d * j];
			}
		}

		repi(k, 1, k_max) {
			dp_h[d] -= (dp_l[k] - dp_l[k - 1]) * ((int)sqrtl((long double)(i / k)) - j_max);
		}
	}

	return dp_h[1];
}


//【二元一次不定方程式の解の数え上げ】
/*
* GCD.h へ
*/
