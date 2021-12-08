#pragma once
#include "header.h"
// ■■■■■ 部分列 DP ■■■■■


//【部分列の数え上げ】O(n k)
/*
* k = 26 種類の英小文字からなる文字列 s[0..n) の部分列の個数を返す．
* 空文字列も s の部分列とみなす．
*
*（部分列 DP）
*/
mint count_subseq(const string& s) {
	// 参考 : https://qiita.com/drken/items/a207e5ae3ea2cf17f4bd
	// verify : https://judge.yosupo.jp/problem/number_of_substrings

	int n = sz(s);
	const int k = 26;

	// nxt[i][c] : s[i..n) で最初に文字 c が現れる位置（無いなら -1）
	vvi nxt(n + 1, vi(k, -1));
	repir(i, n - 1, 0) {
		rep(c, k) {
			nxt[i][c] = nxt[i + 1][c];
		}
		nxt[i][s[i] - 'a'] = i;
	}

	// dp[i + 1] : s[0..i] から得られる s[i] を含む部分列の個数．
	//	ただし同じ部分列については選択する位置の組が辞書順最小になるもののみを認める．
	//	この制約を設けることにより同じ部分列を重複して数えてしまわないようにする．
	vm dp(n + 1);
	dp[0] = 1; // 空文字列に対応

	// 配る DP
	rep(i, n) {
		// 次に選ぶ文字 c について
		rep(c, k) {
			// s[i..n) で最初に文字 c が現れる位置
			int j = nxt[i][c];

			// もう c が現れないなら c を選ぶことはできない．
			if (j == -1) {
				continue;
			}

			// 課した制約のため最も左の c 以外は選べない．
			dp[j + 1] += dp[i];
		}
	}

	// 最も右の選んだ文字について場合分けし結果を足し合わせる．
	mint res = 0;
	repi(i, 0, n) {
		res += dp[i];
	}

	return res;
}


//【回文部分列の数え上げ】O(n^2 k)
/*
* k = 26 種類の英小文字からなる文字列 s[0..n) の回文部分列の個数を返す．
* 空文字列も s の回文部分列とみなす．
*
*（部分列 DP）
*/
mint count_subseq_palindrome(const string& s) {
	// 参考 : https://qiita.com/drken/items/a207e5ae3ea2cf17f4bd
	// verify : https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2895

	int n = sz(s);
	const int k = 26;

	// nxt[i][c] : s[i..n-1] で最初に文字 c が現れる位置（無いなら n）
	vvi nxt(n + 1, vi(k, n));
	repir(i, n - 1, 0) {
		rep(c, k) {
			nxt[i][c] = nxt[i + 1][c];
		}
		nxt[i][s[i] - 'a'] = i;
	}

	// prv[i + 1][c] : s[0..i] で最後に文字 c が現れる位置（無いなら -1）
	vvi prv(n + 1, vi(k, -1));
	rep(i, n) {
		rep(c, k) {
			prv[i + 1][c] = prv[i][c];
		}
		prv[i + 1][s[i] - 'a'] = i;
	}

	// dp[i + 1][j] : 
	//		回文の前半が s[0..i] から，後半が s[j..n-1] から得られる
	//		s[i] と s[j] を含む部分列の個数．
	// ただし同じ部分列については選択する位置の組が前半は辞書順最小，
	// 後半は辞書順最大になるもののみを認める．
	// この制約を設けることにより同じ回文部分列を重複して数えてしまわないようにする．
	vvm dp(n + 1, vm(n + 1));
	dp[0][n] = 1; // 空文字列に対応

	// 配る DP
	rep(i, n) {
		repir(j, n, i + 1) {
			// 次に選ぶ文字 c について
			rep(c, k) {
				// s[i..n-1] で最初に文字 c が現れる位置
				int l = nxt[i][c];

				// s[0..j] で最後に文字 c が現れる位置
				int r = prv[j][c];

				// もう c が現れないか前後が逆転するなら c を選ぶことはできない．
				if (l > r) {
					continue;
				}

				// 課した制約のため選べる c が一意に限定される．
				dp[l + 1][r] += dp[i][j];
			}
		}
	}

	// 空文字列は除いて数え上げる．
	mint res = 0;
	repi(i, 1, n) {
		repi(j, 0, n - 1) {
			// 長さが奇数の回文
			if (i == j + 1) {
				res += dp[i][j];
			}
			// 長さが偶数の回文
			else {
				// 真ん中を削れば長さが奇数の回文も作れるので倍カウント
				res += 2 * dp[i][j];
			}
		}
	}

	// 空文字列の分を加算する．
	return res + 1;
}


