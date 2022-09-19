#pragma once
#include "header.h"
#include "二項係数.h"
#include "分析(文字列).h"
// ■■■■■ 文字列の数え上げ ■■■■■


//【部分文字列の数え上げ】O(n)
/*
* 文字列 s[0..n) の部分文字列の個数を返す（空文字列も s の部分文字列とみなす）
*/
ll count_substring(const string& s) {
	int n = sz(s);

	auto sa = suffix_array(s);
	auto la = lcp_array(s, sa);

	ll res = n - sa[0];
	repi(i, 1, n - 1) res += n - sa[i] - la[i - 1];

	return res;
}


//【回文部分文字列の数え上げ】O(n)
/*
* s[0..n) の部分文字列のうち回文であるものの個数を返す．
*
* 利用：【最長回文長】
*/
template <class STR> ll count_palindromes(const STR& s) {
	int n = sz(s);

	vi lo, le;
	manacher(s, lo, le);

	ll res = 0;
	rep(i, n) res += (lo[i] + 1) / 2;
	rep(i, n - 1) res += le[i] / 2;

	return res;
}


//【部分列の数え上げ】O(n k)
/*
* k = 26 種類の英小文字からなる文字列 s[0..n) の部分列の個数を返す．
* 空文字列も s の部分列とみなす．
*
*（前処理で高速化した部分列 DP）
*/
mint count_subseq(const string& s) {
	// 参考 : https://qiita.com/drken/items/a207e5ae3ea2cf17f4bd

	int n = sz(s);
	const int k = 26;

	// nxt[i][c] : s[i..n) で最初に文字 c が現れる位置（無いなら n）
	vvi nxt(n + 1, vi(k, n));
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
			if (j == n) {
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
*（前処理で高速化した部分列 DP）
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


//【s を部分列にもつ文字列の数え上げ】O(n)
/*
* k = 26 種類の英小文字からなる文字列 s[0..m) を部分列にもつ長さ n の文字列の個数を返す．
*
* 利用：【階乗など（法が大きな素数）】
*/
mint count_supersequences(const string& s, int n, int k = 26) {
	// verify : https://atcoder.jp/contests/abc171/tasks/abc171_f

	//【方法】
	// n 文字の中から m 文字を選んでそこに s[0..m) を順に配置し，
	// 残る箇所には k 種類の文字を自由に選んで配置すると考えたい．
	//
	// ただしこのままでは同じ文字列を複数回数え上げてしまうため，
	// s[i-1] と s[i] の間には s[i] と同じ文字は選べないことにする．
	// これを s[m-1] の位置で場合分けしながら足し合わせる．

	int m = sz(s);

	Factorial_mint fm(n);

	vm pow25(n), pow26(n);
	pow25[0] = pow26[0] = 1;
	rep(i, n - 1) {
		pow25[i + 1] = pow25[i] * (k - 1);
		pow26[i + 1] = pow26[i] * k;
	}

	mint res = 0;

	// i : s[m-1] の位置（1-indexed）
	repi(i, m, n) {
		// 位置 i より左への s[0..m-1) の配置の仕方が bin(i-1, m-1) 通り．
		// 位置 i より左の残り文字の選び方が (k-1)^(i-m) 通り．
		// 位置 i より右の残り文字の選び方が k^(n-i) 通り．
		res += fm.binomial(i - 1, m - 1) * pow25[i - m] * pow26[n - i];
	}

	return res;
}


