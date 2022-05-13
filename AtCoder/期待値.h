#pragma once
#include "header.h"
#include "多項式.h"
#include "探索.h"
// ■■■■■ 期待値 ■■■■■


//【すごろく（任意サイコロ）】O(n m)
/*
* a[j] の目が p[j] の確率で出る m 面サイコロを用いてすごろくを行う．
* マス 0 からスタートしてマス n に辿り着くまでの回数の期待値を返す．
*
*（期待値 DP）
*/
double sugoroku(const vi& a, const vd& p, int n) {
	int m = sz(a);

	// dp[i] : マス i からマス n に辿り着くまでの回数の期待値
	vd dp(n + 1);

	// 貰う DP
	repir(i, n - 1, 0) {
		// 必ずかかる手数で初期化
		double e = 1;

		rep(j, m) {
			// 確率で重みを付けながら先の期待値を足していく．
			e += dp[min(i + a[j], n)] * p[j];
		}

		// 結果の記録
		dp[i] = e;
	}

	return dp[0];
}


//【すごろく】O(n)
/*
* 1 から m の目が等確率で出る m 面サイコロを用いてすごろくを行う．
* マス 0 からスタートしてマス n に辿り着くまでの回数の期待値を返す．
*
*（累積和で高速化した期待値 DP）
*/
double sugoroku(int m, int n) {
	// マス i からマス n に辿り着くまでの回数の期待値を e[i] とおく．
	// dp[i] : Σe[i..n)
	vd dp(n + 1);

	// 貰う DP
	repir(i, n - 1, 0) {
		// e[i] = 1 + Σe[i+1..i+m] / m である．
		// ただしゴールを超えたものもゴールとして扱う．
		double sub = (1 + i + m) <= n ? dp[i + 1 + m] : 0;
		double e = 1 + (dp[i + 1] - sub) / m;

		dp[i] = dp[i + 1] + e;
	}

	return dp[0] - dp[1];
}


//【すごろく（振り出しに戻る付き）】O(n + k log k)
/*
* 1 から m の目が等確率で出る m 面サイコロを用いてすごろくを行う．
* ただしマス a[0..k) に止まるとマス 0 まで戻される．
* マス 0 からスタートしてマス n に辿り着くまでの回数の期待値を返す．
* ゴール不可能なら (double)INFL を返す．
*
*（累積和で高速化した期待値 DP）
* 
* 利用：【一次多項式】
*/
double sugoroku_back_poly(int m, int n, vi a) {
	// verify : https://atcoder.jp/contests/abc189/tasks/abc189_f

	//【方法】
	// マス i に居るときの回数の期待値を e[i] とおくと，振り出しに戻るマスでなければ
	//		e[i] = 1 + Σe[i+1..i+m] / m
	// などとなるため後ろからの累積和で計算できる．一方振り出しに戻るマスでは
	//		e[a[j]] = e[0]
	// となるため循環参照が起こってしまう．
	//
	// そこで，e[0] = x とおき，x を含んだ式を許して e[0..n) を後ろから計算する．
	// 最後に e[0] について一次方程式を立ててそれを解けば x = e[0] が求まる．

	int k = sz(a);
	sort(all(a));

	// ゴール不可能な場合を判定
	int len = 1;
	rep(i, k) {
		// len : 振り出しに戻すマスが連続する個数
		if (i > 0 && a[i] == a[i - 1] + 1) len++;
		else len = 1;

		// 振り出しに戻すマスが m 個連続したらゴール不可能
		if (len == m) return (double)INFL;
	}

	// マス i からマス n に辿り着くまでの回数の期待値を e[i] とおく．
	// dp[i] : Σe[i..n)
	vector<Poly1<double>> dp(n + 1);

	// 変数 x = e[0]
	const Poly1<double> X(1., 0.);

	int pt = k - 1;
	repir(i, n - 1, 0) {
		Poly1<double> e(1);

		// 振り出しに戻すマスの場合
		if (pt >= 0 && i == a[pt]) {
			e = X;
			pt--;
		}
		// 通常のマスの場合
		else {
			Poly1<double> sub = (1 + i + m) <= n ? dp[i + 1 + m] : 0.;
			e = 1. + (dp[i + 1] - sub) * (1. / m);
		}

		dp[i] = dp[i + 1] + e;
	}

	// 一次方程式を解く．
	return (dp[0] - dp[1]).solve(X);
}


//【すごろく（振り出しに戻る付き）】O(n + k log k)
/*
* 1 から m の目が等確率で出る m 面サイコロを用いてすごろくを行う．
* ただしマス a[0..k) に止まるとマス 0 まで戻される．
* マス 0 からスタートしてマス n に辿り着くまでの回数の期待値を返す．
* ゴール不可能なら (long double)INFL を返す．
*
*（累積和で高速化した期待値 DP）
*
* 利用：【二分探索（実数）】
*/
long double sugoroku_back_bs(int m, int n, vi a) {
	// verify : https://atcoder.jp/contests/abc189/tasks/abc189_f

	//【方法】
	// マス i に居るときの回数の期待値を e[i] とおくと，振り出しに戻るマスでなければ
	//		e[i] = 1 + Σe[i+1..i+m] / m
	// などとなるため後ろからの累積和で計算できる．一方振り出しに戻るマスでは
	//		e[a[j]] = e[0]
	// となるため循環参照が起こってしまう．
	//
	// そこで，e[0] = e0 と決め打って e[0..n) を後ろから計算する．
	// その結果得られた e[0] が e0 以下ならまだ余裕があるとして二分探索を行う．

	int k = sz(a);
	sort(all(a));

	// ゴール不可能な場合を判定
	int len = 1;
	rep(i, k) {
		// len : 振り出しに戻すマスが連続する個数
		if (i > 0 && a[i] == a[i - 1] + 1) len++;
		else len = 1;

		// 振り出しに戻すマスが m 個連続したらゴール不可能
		if (len == m) return (long double)INFL;
	}

	// e[0] = e0 と決め打って計算した答えが e0 以下かを返す．
	function<bool(long double)> okQ = [&](long double e0) {
		// マス i からマス n に辿り着くまでの回数の期待値を e[i] とおく．
		// dp[i] : Σe[i..n)
		vector<long double> dp(n + 1);

		int pt = k - 1; long double e = 1;
		repir(i, n - 1, 0) {
			// 振り出しに戻すマスの場合
			if (pt >= 0 && i == a[pt]) {
				e = e0;
				pt--;
			}
			// 通常のマスの場合
			else {
				long double sub = (1 + i + m) <= n ? dp[i + 1 + m] : 0.;
				e = 1. + (dp[i + 1] - sub) * (1. / m);
			}

			dp[i] = dp[i + 1] + e;
		}

		return e <= e0;
	};

	return binary_search((long double)INFL, (long double)0, okQ);
}


//【クーポンコレクター問題】O(n^max(a))
/*
* 等確率で当たる n 枚のクーポンを各 a[i] (∈[0..3]) 枚揃えるまでの回数の期待値を返す．
*
*（期待値 DP）
*/
double coupon_collectors_problem(const vi& a) {
	// verify : https://atcoder.jp/contests/dp/tasks/dp_j

	int n = sz(a);

	vector<int> cnt(4);
	rep(i, n) {
		cnt[a[i]]++;
	}

	auto f1 = [&]() {
		double res = 0;
		repir(i, cnt[1], 1) res += 1. / i;
		res *= n;
		return res;
	};

	auto f2 = [&]() {
		// dp[i][j] : 残り 1, 2 個のクーポンが各 i, j 種類残っている場合の
		// 揃えるまでの残り試行回数の期待値
		vvd dp(n + 1, vd(n + 1));

		for (int j = 0; j <= cnt[2]; j++) {
			for (int i = 0; i + j <= cnt[1] + cnt[2]; i++) {
				if (i == 0 && j == 0) {
					dp[i][j] = 0;
					continue;
				}

				dp[i][j] = (double)n / (i + j);
				if (i > 0) {
					dp[i][j] += dp[i - 1][j] * i / (i + j);
				}
				if (j > 0) {
					dp[i][j] += dp[i + 1][j - 1] * j / (i + j);
				}
			}
		}

		return dp[cnt[1]][cnt[2]];
	};

	auto f3 = [&]() {
		// dp[i][j][k] : 残り 1, 2, 3 個のクーポンが各 i, j, k 種類残っている場合の
		// 揃えるまでの残り試行回数の期待値
		vvvd dp(n + 1, vvd(n + 1, vd(n + 1)));

		for (int k = 0; k <= cnt[3]; k++) {
			for (int j = 0; j + k <= cnt[2] + cnt[3]; j++) {
				for (int i = 0; i + j + k <= cnt[1] + cnt[2] + cnt[3]; i++) {
					if (i == 0 && j == 0 && k == 0) {
						dp[i][j][k] = 0;
						continue;
					}

					dp[i][j][k] = (double)n / (i + j + k);
					if (i > 0) {
						dp[i][j][k] += dp[i - 1][j][k] * i / (i + j + k);
					}
					if (j > 0) {
						dp[i][j][k] += dp[i + 1][j - 1][k] * j / (i + j + k);
					}
					if (k > 0) {
						dp[i][j][k] += dp[i][j + 1][k - 1] * k / (i + j + k);
					}
				}
			}
		}

		return dp[cnt[1]][cnt[2]][cnt[3]];
	};

	if (cnt[3] > 0) return f3();
	else if (cnt[2] > 0) return f2();
	else if (cnt[1] > 0) return f1();
	else return 0;
}


//【不正確な射撃】O(2^n n)
/*
* 位置 x[0..n) それぞれに的がある．左右それぞれに確率 1/3 でぶれる射撃を行うとき，
* 全ての的に命中するまでの射撃回数の期待値を返す．
*
*（bit DP）
*/
double blurred_shooting(const vi& x) {
	// verify : https://atcoder.jp/contests/tdpc/tasks/tdpc_ball

	int n = *max_element(all(x)) + 1;

	// 初期状態の的の集合
	int set = 0;
	repe(i, x) {
		set += (1 << i);
	}

	// dp[set] : set に的がある場合の射撃回数の期待値
	vd dp(1LL << n, INFL);
	vb seen(1LL << n);
	dp[0] = 0;
	seen[0] = true;

	// set に的がある場合の射撃回数の期待値を返す．
	function<double(int)> rf = [&](int set) {
		// 計算済なら結果を返す．
		if (seen[set]) return dp[set];
		seen[set] = true;

		// i : 狙う位置
		rep(i, n) {
			double sum = 0; // 的に当たった場合の残り回数の期待値の和
			int tgts = 0; // 当たる可能性のある的の数

			// 狙いが左にぶれた場合
			if (i > 0 && (set & (1 << (i - 1)))) {
				sum += rf(set - (1 << (i - 1)));
				tgts++;
			}

			// 狙い通りに弾が飛んだ場合
			if (set & (1 << i)) {
				sum += rf(set - (1 << i));
				tgts++;
			}

			// 狙いが右にぶれた場合
			if (i < n - 1 && (set & (1 << (i + 1)))) {
				sum += rf(set - (1 << (i + 1)));
				tgts++;
			}

			// 範囲内に的が無いような所を狙う意味は無いので無視する．
			if (tgts == 0) continue;

			// いずれかの的に命中するまでの回数の期待値が 3 / tgts，
			// いずれかの的に命中してからの回数の期待値が sum / tgts，
			// それらの和をとったものが i を狙った場合の回数の期待値となる．
			chmin(dp[set], (3 + sum) / tgts);
		}

		return dp[set];
	};

	return rf(set);
}


