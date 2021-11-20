#pragma once
#include "header.h"
// ■■■■■ 部分列 DP ■■■■■


//【部分列の数え上げ】O(k |s|)
/*
* k = 26 種類の英小文字からなる文字列 s[0..n) の部分列の個数を返す．
* 空文字列も s の部分列とみなす．
*
*（部分列 DP）
*/
mint count_subseq(const string& s) {
	// 参考 : https://qiita.com/drken/items/a207e5ae3ea2cf17f4bd

	int n = sz(s);
	const int k = 26;

	// nx[i][c] : 部分文字列 s[i..n) で最初に文字 c が現れる位置（無いなら -1）
	vvi nx(n + 1, vi(k, -1));
	repir(i, n - 1, 0) {
		rep(c, k) {
			nx[i][c] = nx[i + 1][c];
		}
		nx[i][s[i] - 'a'] = i;
	}

	// dp[i + 1] : 部分文字列 s[0..i] から得られる s[i] を含む部分列の個数．
	//	ただし同じ部分列については選択する位置の組が辞書順最小になるもののみを認める．
	//	この制約を設けることにより同じ部分列を重複して数えてしまわないようにする．
	vm dp(n + 1);
	dp[0] = 1; // 空文字列に対応

	// 配る DP
	rep(i, n) {
		// 次に選ぶ文字 c について
		rep(c, k) {
			// 部分文字列 s[i..n) で最初に文字 c が現れる位置
			int j = nx[i][c];

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


//【回文部分列の数え上げ】O(k |s|^2)
/*
* k = 26 種類の英小文字からなる文字列 s[0..n) の回文部分列の個数を返す．
* 空文字列も s の回文部分列とみなす．
*
*（部分列 DP）
*/
mint count_subseq_palindrome(const string& s) {
	// 参考 : https://qiita.com/drken/items/a207e5ae3ea2cf17f4bd

	int n = sz(s);
	const int k = 26;

	// nx[i][c] : 部分文字列 s[i..n-1] で最初に文字 c が現れる位置（無いなら n）
	vvi nx(n + 1, vi(k, n));
	repir(i, n - 1, 0) {
		rep(c, k) {
			nx[i][c] = nx[i + 1][c];
		}
		nx[i][s[i] - 'a'] = i;
	}

	// pv[i + 1][c] : 部分文字列 s[0..i] で最後に文字 c が現れる位置（無いなら -1）
	vvi pv(n + 1, vi(k, -1));
	rep(i, n) {
		rep(c, k) {
			pv[i + 1][c] = pv[i][c];
		}
		pv[i + 1][s[i] - 'a'] = i;
	}

	// dp[i + 1][j] : 
	//		回文の前半が部分文字列 s[0..i] から，後半が s[j..n-1] から得られる
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
				// 部分文字列 s[i..n-1] で最初に文字 c が現れる位置
				int l = nx[i][c];

				// 部分文字列 s[0..j] で最後に文字 c が現れる位置
				int r = pv[j][c];

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


//【i 番目の部分列】O(k |s|)
/*
* k = 26 種類の英小文字からなる文字列 s[0..n) の辞書順 d 番目の部分列を res に格納する．
* そのようなものがなければ false を返す．
* 空文字列も s の部分列とみなし，辞書順で 0 番目に現れるとする．
*
*（部分列 DP）
*/
bool lex_order_subseq(const string& s, ll d, string& res) {
	// 参考 : https://qiita.com/drken/items/a207e5ae3ea2cf17f4bd
	// verify : https://atcoder.jp/contests/tdpc/tasks/tdpc_lexicographical

	int n = sz(s);
	const int k = 26;

	// dp[i][c] : 部分文字列 s[i..n) の文字 c から始まる部分列の個数
	//	ただし同じ部分列については選択する位置の組が辞書順最小になるもののみを認める．
	//	この制約を設けることにより同じ部分列を重複して数えてしまわないようにする．
	vvl dp(n, vl(k));
	dp[n - 1][s[n - 1] - 'a'] = 1;

	// オーバーフローしないように注意した足し算
	auto add = [](ll& tgt, ll val) {
		tgt = min(tgt + val, INFL);
	};

	// 後ろから順に貰う DP
	repir(i, n - 2, 0) {
		// sum : s[i+1..n) の部分列の総数（初期値の 1 は空文字列に相当する）
		ll sum = 1;

		rep(c, k) {
			// c != s[i] のとき
			if (c != s[i] - 'a') {
				// s[i] は選べないので，s[i+1..n) のときと個数は同じ
				dp[i][c] = dp[i + 1][c];
			}

			// s[i+1..n) の部分列の総数に加算する．
			add(sum, dp[i + 1][c]);
		}

		// c = s[i] のとき
		// 課した制約のため，必ず s[i] を選ばなければならない．
		// s[i] を選べば c で始まるので，s[i+1] 以降は何でも良い．
		dp[i][s[i] - 'a'] = sum;
	}

	// DP 復元
	res.clear();
	rep(i, n) {
		// 求める部分列が見つかったら終了する．
		if (d == 0) {
			return true;
		}

		rep(c, k) {
			// s[i..n) の c で始まる部分列が d 個未満の場合
			if (dp[i][c] < d) {
				// 求める部分列は c では始まらないので次の c に進む．
				// d は c で始まる部分列の個数分だけ減らしておく．
				d -= dp[i][c];
			}
			// s[i..n) の c で始まる部分列が d 個以上の場合
			else {
				// 求める部分列の最初の文字は c に確定する．
				res.push_back(c + 'a');

				// 文字 c のある位置の次まで進める．
				//（ちょうどのところで止めているが，rep 文があるので 1 つ進む）
				while (s[i] != c + 'a') {
					i++;
				}

				// c のみで終わる部分列の分を引いておく．
				d--;

				goto LOOP_END;
			}
		}

		// 全ての文字を調べきったなら，部分列の総数が d 未満だったことになる．
		return false;

	LOOP_END:;
	}

	// ちょうど最後の文字を使った場合はここにくる．
	return true;
}


