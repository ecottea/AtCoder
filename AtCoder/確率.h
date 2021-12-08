#pragma once
#include "header.h"
#include "数論.h"
// ■■■■■ 確率 ■■■■■


//【コインの表の枚数の確率】O(n^2)
/*
* 表が出る確率がそれぞれ p[0..n) のコインをすべて投げたとき，
* 表が j 枚出る確率を head[j] に格納する．
* 
*（確率 DP）
*/
void coin_probability(const vd& p, vd& head) {
	// verify : https://atcoder.jp/contests/dp/tasks/dp_i

	int n = sz(p);
	head.resize(n + 1);

	// dp[i][j] : コイン [0..i) のうち表が j 枚出る確率
	vvd dp(n + 1, vd(n + 1));
	dp[0][0] = 1;

	// 配る DP
	rep(i, n) {
		repi(j, 0, i) {
			// 表が出た場合
			dp[i + 1][j + 1] += p[i] * dp[i][j];

			// 裏が出た場合
			dp[i + 1][j] += (1 - p[i]) * dp[i][j];
		}
	}
	
	repi(j, 0, n) {
		head[j] = dp[n][j];
	}
}


//【トーナメントの優勝確率】O(n^2)
/*
* レートがそれぞれ r[i] である n = 2^k 人を集めてトーナメントを行うときの
* それぞれの人の優勝する確率を win[i] に格納する．
* レート r の人にレート r0 の人に対する勝率は f(r, r0) で計算される．
* 
*（確率DP）
*/
void tournament_probability(const vd& r, vd& win, function<double(double, double)>& f) {
	// verify : https://atcoder.jp/contests/tdpc/tasks/tdpc_tournament

	int n = sz(r);
	int k = msb(n);
	win = vd(n);

	// dp[d][i] : i 番目の人が d 回戦で勝つ確率
	vvd dp(k + 1, vd(n));

	// 0 回戦で勝つ（出場資格を得る）確率は 1 とする．
	rep(i, n) {
		dp[0][i] = 1.;
	}

	repi(d, 1, k) {
		rep(i, n) {
			// d 回戦で人 i と対戦する可能性のある人の番号の始めと終わりを得る．
			int from = ((i >> (d - 1)) ^ 1) << (d - 1);
			int to = from + (1 << (d - 1)) - 1;

			// d 回戦で人 i と対戦する可能性のある人 j それぞれについて
			repi(j, from, to) {
				// j が d-1 回戦に進出し，かつ i が j に勝つ確率を加算する．
				dp[d][i] += dp[d - 1][j] * f(r[i], r[j]);
			}

			// そもそも i が d-1 回戦に進出しないといけないのでその確率を乗算する．
			dp[d][i] *= dp[d - 1][i];
		}
	}

	rep(i, n) {
		win[i] = dp[k][i];
	}
}


//【サイコロの目の積の確率】O(n^2)
/*
* 6 面サイコロを n 回振ったときの出目の積が d の倍数になる確率を返す．
*
*（確率 DP）
* 
* 利用：【素因数分解】
*/
double dice_product_probability(int n, ll d) {
	// verify : https://atcoder.jp/contests/tdpc/tasks/tdpc_dice

	// d を素因数分解する．
	map<ll, int> pps;
	factor_integer(d, pps);

	// d に 2, 3, 5 以外の素因数が含まれていれば確率は明らかに 0
	auto it = pps.upper_bound(5);
	if (it != pps.end()) return 0;

	// d の素因数 2, 3, 5 の個数を得る．
	int m2 = pps[2], m3 = pps[3], m5 = pps[5];

	// dp_k[i2][i3][i5] : 
	//  k 回サイコロを振ったときの出た目の積に素因数 2, 3, 5 がそれぞれ
	//  i2, i3, i5 回含まれている確率（ただし m2, m3, m5 回以上は同一視する）
	vvvd dp(m2 + 1, vvd(m3 + 1, vd(m5 + 1)));
	dp[0][0][0] = 1;

	// サイコロの各目が出たときの素因数の増加量
	const vi d2 = { 0, 1, 0, 2, 0, 1 };
	const vi d3 = { 0, 0, 1, 0, 0, 1 };
	const vi d5 = { 0, 0, 0, 0, 1, 0 };

	rep(k, n) {
		// 計算結果を入れておくための一時配列
		vvvd dp_(m2 + 1, vvd(m3 + 1, vd(m5 + 1)));

		repi(i2, 0, m2) {
			repi(i3, 0, m3) {
				repi(i5, 0, m5) {
					// 各サイコロの目 j について
					rep(j, 6) {
						// 素因数 2, 3, 5 の個数がいくつになるか
						int n2 = min(i2 + d2[j], m2);
						int n3 = min(i3 + d3[j], m3);
						int n5 = min(i5 + d5[j], m5);

						// 確率 1 / 6 でそのように遷移する．
						dp_[n2][n3][n5] += dp[i2][i3][i5] / 6.;
					}
				}
			}
		}

		// 計算結果を dp に代入する．
		dp = dp_;
	}

	return dp[m2][m3][m5];
}


