#pragma once
#include "header.h"
// ■■■■■ 区間 DP ■■■■■



//【連鎖行列積問題】O(n^3)
/*
* 行数 d[i]，列数 d[i + 1] の n 個の行列 M[i] の積を計算する場合の最小乗算回数を返す．
*
*（区間 DP）
*/
ll matrix_chain_multiplication_problem(vl& d) {
	int n = sz(d) - 1;

	// dp[i][j] : M[i..j] の積を計算する場合の最小乗算回数．
	vvl dp(n, vl(n, INFL));
	rep(i, n) {
		dp[i][i] = 0;
	}

	// 区間幅 w = j - i について昇順に考えていく
	repi(w, 1, n - 1) {
		rep(i, n) {
			int j = i + w;
			if (j >= n) {
				break;
			}

			repi(k, i, j - 1) {
				// M[i, k] * M[k + 1, j] と計算する場合
				chmin(dp[i][j], dp[i][k] + dp[k + 1][j] + d[i] * d[k + 1] * d[j + 1]);
			}
		}
	}

	return dp[0][n - 1];
}


//【スライム融合】O(n^3)
/*
* 大きさ c[i] のスライム n 匹が一列に並んでいる．
* 合体時に大きさの和だけのコストを払うとき，全部を合体させるための最小コストを返す．
*/
ll merge_slimes(const vl& c) {
	int n = sz(c);

	// 累積和
	vl acc(n + 1);
	rep(i, n) acc[i + 1] = acc[i] + c[i];

	// dp[l][r] : スライム [l..r) を合体させるための最小コスト
	vvl dp(n, vl(n + 1, INFL));
	rep(i, n) dp[i][i + 1] = 0;

	// 区間幅 w の小さい順に貰う DP
	repi(w, 2, n) {
		repi(l, 0, n - w) {
			int r = l + w;

			// k : [l..k) と [k..r) を最後に合体する
			repi(k, l, r - 1) {
				// [l..k) および [k..r) それぞれの合体にかかる最小コスト
				chmin(dp[l][r], dp[l][k] + dp[k][r]);
			}

			// [l..k) と [k..r) の合体にかかるコスト（k に依らない）を加算する．
			dp[l][r] += acc[r] - acc[l];
		}
	}

	return dp[0][n];
}


//【ダルマ落とし】O(n^3)
/*
* 重さ w[i] のブロックが塔状に積まれている．重さの差が d 以下の 2 ブロックを
* 同時に叩き出せるとき，取り除けるブロックの最大個数を返す．
*/
int daruma_otoshi(const vi& w, int d) {
	int n = sz(w);

	// dp[l][r] : ブロック [l..r) から叩き出せる最大ブロック数
	vvi dp(n + 1, vi(n + 1));

	// 区間幅 width の小さい順に貰う DP
	repi(width, 2, n) {
		repi(l, 0, n - width) {
			int r = l + width;

			// ブロック l を叩き出さない場合
			dp[l][r] = dp[l + 1][r];

			// ブロック l と l + (2 k + 1) を同時に叩き出す場合
			for (int k = 0; l + (2 * k + 1) < r; k++) {
				// w[l], w[l + (2 k + 1)] の差が d 以下でないといけない．
				if (abs(w[l] - w[l + (2 * k + 1)]) > d) continue;

				// ブロック l と l + (2 k + 1) の間が全て叩き出せないといけない．
				if (dp[l + 1][l + (2 * k + 1)] != 2 * k) continue;

				chmax(dp[l][r], (2 * k + 2) + dp[l + (2 * k + 2)][r]);
			}
		}
	}

	return dp[0][n];
}


//【iwi】O(n^3)
/*
* 'i' と 'w' からなる文字列 s に対し，連続する "iwi" を取り除くことを繰り返す．
* 取り除ける最大文字数を返す．
*/
int iwi(const string& s) {
	int n = sz(s);

	// dp[l][r] : s[l..r) から取り除ける "iwi" の数
	vvi dp(n + 1, vi(n + 1));

	// 区間幅 w の小さい順に貰う DP
	repi(w, 3, n) {
		repi(l, 0, n - w) {
			int r = l + w;

			// s[l] を取り除かない場合
			chmax(dp[l][r], dp[l + 1][r]);

			// s[r - 1] を取り除かない場合
			chmax(dp[l][r], dp[l][r - 1]);

			// 以降は s[l], s[r - 1] を取り除く場合のみを考えるので，
			// その文字が 'w' の場合は取り除けないから無視してよい．
			if (s[l] == 'w' || s[r - 1] == 'w') continue;

			// s[l] と s[r - 1] を別々に取り除く場合
			// どこで分けるかを全通り試せば良い．
			repi(k, l + 1, r - 1) {
				chmax(dp[l][r], dp[l][k] + dp[k][r]);
			}

			// s[l] と s[r - 1] を同時に取り除く場合
			// 同時に消される s[k] = 'w' の可能性を全通り試せば良い．
			for (int k = l + 1; k < r; k += 3) {
				if (s[k] == 'w'
					&& dp[l + 1][k] == k - (l + 1)
					&& dp[k + 1][r - 1] == (r - 1) - (k + 1)) {
					chmax(dp[l][r], r - l);
				}
			}
		}
	}

	return dp[0][n];
}


