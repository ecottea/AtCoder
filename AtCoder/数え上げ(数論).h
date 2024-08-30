#pragma once
#include "header.h"
#include "ディリクレ畳込み.h"
// ■■■■■ 数え上げ（数論） ■■■■■


//【素数計数関数】O(n^(3/4))
/*
* n 以下の素数の個数 π(n) を返す．
*
*（Lucy DP）
*/
ll prime_pi(ll n) {
	// 参考 : https://rsk0315.hatenablog.com/entry/2021/05/18/015511
	// verify : https://judge.yosupo.jp/problem/counting_primes

	//【方法】
	// S(v, p) = ([2..v] 内の "素数または p 以下の素因数をもたない合成数" の個数) とおく．
	// これはエラトステネスの篩において，p 以下の素数で篩い終えた後残っている数の個数である．
	//
	// p が合成数であるときと p^2 > v であるときは，篩のアルゴリズムより明らかに
	//		S(v, p) = S(v, p-1)
	// である．よって p が p^2 ≦ v なる素数の場合の遷移のみを考えれば良い．
	// 
	// p の 1 つ前の素数で篩い終わったときに残っている数の個数は S(v, p-1) である．
	// これらの数のうち p で篩われて消えるものの個数を考える．
	// そのような数は，[2..v] 内の
	//		(i) p より小さい素因数を持たない（まだ篩われていない）
	//		(ii) p より大きい p の倍数（次に篩われる）
	// という条件を共に満たす数である．
	// 
	// [2..v] に条件 (i), (ii) を課す代わりに，全体を p で割って，
	//		[2..v/p] 内の p より小さい素因数を持たない数
	// を数えても個数は変わらない．そのような数は，[2..v/p] 内の
	//		(iii) p-1 以下の素因数で篩い終えた後残っている
	//		(iv) p-1 以下の素数ではない
	// という条件を共に満たす数である．
	//
	// いま p^2 ≦ v なる場合を考えているので，p ≦ v/p である．
	// よって !(iv) ⇒ (iii) であるから，求める個数は
	//		S(v/p, p-1) - S(p-1, p-1)
	// と表される．
	//
	// 以上をまとめて，DP の遷移式
	//		S(v, p) = S(v, p-1) （p が合成数または p^2 > v のとき）
	//		S(v, p) = S(v, p-1) - (S(v/p, p-1) - S(p-1, p-1)) （その他の p のとき）
	// を得る．初項は
	//		S(v, 1) = v - 1
	// であり，求めたいものは
	//		π(n) = S(n, √n)
	// である．

	//【備考】
	// 途中で DP テーブルを参照すれば，k-rough number の数え上げにも対応できる．

	if (n <= 1) return 0;

	int m = (int)(sqrt(n) + 1e-6);

	// S(v, p) : [2..v] 内の p 以下の素数で篩い終えた後残っている数の個数
	//	dp_p[0][v] : S(v, p)
	//	dp_p[1][v] : S(n/v, p)
	vvl dp(2, vl(m + 1));
	repi(v, 1, m) {
		dp[0][v] = v - 1;
		dp[1][v] = n / v - 1;
	}

	repi(p, 2, m) {
		// S(p - 1, p - 1)
		ll s = dp[0][p - 1];

		// p が素数でなければ次の p へ
		if (dp[0][p] == s) continue;

		// dp[1][v] = S(n/v, p) の更新
		repi(v, 1, m) {
			// p^2 > n/v なら更新不要
			if (p > n / v / p) break;

			if (v <= m / p) dp[1][v] -= dp[1][v * p] - s;
			else dp[1][v] -= dp[0][n / v / p] - s;
		}

		// dp[0][v] = S(v, p) の更新
		repir(v, m, 1) {
			// p^2 > v なら更新不要
			if (p > v / p) break;

			dp[0][v] -= dp[0][v / p] - s;
		}
	}

	return dp[1][1];
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
		dp_l[0][i] = (i - 1) / 4;
		dp_l[1][i] = (i + 1) / 4;
		dp_h[0][i] = (N / i - 1) / 4;
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


//【無平方数の数え上げ】O(n^0.45) ?
/*
* n 以下の無平方数の個数を返す．
*/
constexpr ll W = 2000000;
ll dp_l[W], dp_h[W];
ll count_square_free(ll n) {
	// verify : https://judge.yosupo.jp/problem/counting_squarefrees

	//【注意】
	// 境界の決め方がかなり雑なので信用してはいけない．

	rep(i, W) dp_l[i] = dp_h[i] = -1;

	ll N = n;

	function<ll(int)> rf_l = [&](int n) {
		if (dp_l[n] != -1) return dp_l[n];

		ll res = n;

		int m = (int)(pow(n * 0.5, 1. / 3) + 1e-9);

		int i_max = (int)sqrt(1. * n / (m + 1));

		// q に対応する i が高々 1 個の部分は i ごとに愚直に考える．
		for (int i = 2; i <= i_max; i++) res -= rf_l(n / (i * i));

		// そうでない部分は q ごとにまとめて考える．
		int il, ir = i_max + 1;
		repir(q, m, 1) {
			il = ir;
			ir = (int)sqrt(1. * n / q) + 1;
			if (il != ir) res -= rf_l(q) * (ir - il);
		}

		return dp_l[n] = res;
	};

	function<ll(int)> rf_h = [&](int I) {
		if (dp_h[I] != -1) return dp_h[I];

		ll n = N / ((ll)I * I);

		ll res = n;

		ll m = (ll)(pow(n * 0.5, 1. / 3));

		ll i_max = (ll)sqrtl(1.L * n / (m + 1));

		// q に対応する i が高々 1 個の部分は i ごとに愚直に考える．
		for (ll i = 2; i <= i_max; i++) {
			ll q = n / (i * i);
			if (q < W) res -= rf_l((int)q);
			else res -= rf_h(i * I);
		}

		// そうでない部分は q ごとにまとめて考える．
		ll il, ir = i_max + 1;
		repir(q, m, 1) {
			il = ir;
			ir = (ll)sqrtl(1.L * n / q) + 1;
			if (il != ir) {
				if (q < W) res -= rf_l((int)q) * (ir - il);
				else res -= rf_h(il * I) * (ir - il);
			}
		}

		return dp_h[I] = res;
	};

	ll res = (n < W ? rf_l(n) : rf_h(1));

	return res;
}


//【二元一次不定方程式の解の数え上げ】
/*
* GCD.h へ
*/
