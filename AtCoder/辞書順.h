#pragma once
#include "header.h"
// ■■■■■ 辞書順 ■■■■■


//【辞書順最小の連結文字列】O(n k^2 max|s[i]|)
/*
* n 個の文字列 s から k 個を選び連結して作られる辞書順最小の文字列を返す．
* 
*（後ろから DP）
*/
string minimum_string_concat(vector<string> s, int k) {
	// verify : https://atcoder.jp/contests/abc225/tasks/abc225_f

	int n = sz(s);

	// 連結した時どっち向きの方が辞書順で小さいかで半順序を定義する．
	auto compare = [&](const string& a, const string& b) {
		return a + b < b + a;
	};

	// 先の半順序で s をソートする．
	// 半順序が推移律を満たすことにより
	//		∀i < j, s[i] + s[j] <= s[j] + s[i]
	// が成り立つので，この順での連結のみを考えれば良い．
	sort(all(s), compare);

	// dp_i[j] : s[i..n) から j 個選んだ場合の最小（s[0..i) としてはいけない！）
	string str_max = "z"; str_max[0]++;
	vector<string> dp(k + 1, str_max);
	dp[0] = "";

	repir(i, n - 1, 0) {
		repir(j, min(n - i, k), 1) {
			// s[i] を使う方が小さくなるなら更新する．
			chmin(dp[j], s[i] + dp[j - 1]);

			// この更新式で大丈夫なのは，文字列の連結 "+" と辞書順比較 "<" の間に
			//		A + B < A + C ⇔ B < C
			// なる関係がある（"+" と "<" が左両立する）からである．
			//
			// なお，
			//		A + C < B + C ⇔ A < B
			// は成り立たないので dp テーブルの持ち方には要注意！
			// 反例： bac < bc かつ ba > b
		}
	}

	return dp[k];
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


