#pragma once
#include "header.h"
#include "前処理(列).h"
// ■■■■■ 辞書順 ■■■■■


//【辞書順比較の言い換え】
/*
* 文字列 s, t について，s < t であるための必要十分条件は，次の (1), (2) の一方が成り立つことである：
*	(1) s が t の真の接頭辞である．
*	(2) s と t を前から見て初めて文字が異なる位置を i とすると s[i] < t[i] である．
* 
* verify : https://atcoder.jp/contests/arc050/tasks/arc050_d
*/


//【連結と辞書順】
/*
* 文字列の連結 "+" と辞書順比較 "<" の間には
*		A + B < A + C ⇔ B < C
* なる関係がある（"+" と "<" が左両立する．）
* よって後ろから貪欲に考えるのが良い．
* 
* verify : https://yukicoder.me/problems/no/1894
*		
* 一方
*		A + C < B + C ⇔ A < B
* は成り立たない（反例： bac < bc かつ ba > b）
*/


//【無限連結と辞書順】
/*
* 文字列 s, t を無限個連結した文字列をそれぞれ S, T とするとき，以下が成り立つ：
*	s + t < t + s ⇔ S < T
* 
* verify : https://yukicoder.me/contests/395
*/


//【接尾辞配列】
/*
* ACL の vi sa = suffix_array(string s) を利用すればよい．
* 
* sa[i] は辞書順 i 番目の接尾辞が s[sa[i]..n) であることを表す．
*/


//【LCP array】
/*
* ACL の vi lcp = lcp_array(string s, vi sa) を利用すればよい（sa は s の【接尾辞配列】）
* 
* lcp[i] は s[sa[i]..n) と s[sa[i+1]..n) の最長共通接頭辞の長さ．
*/


//【任意列の辞書順 d 番目】
/*
* [0..k) からなる長さ n 以下の列（空列含む）を辞書順に並べたとき d 番目に現れる列を返す（なければ {-1} を返す）
*/
vi lex_order_allseq(int k, int n, ll d) {
	// verify : https://atcoder.jp/contests/arc084/tasks/arc084_c

	// cnt[i] : 長さ i の列が何個あるか
	vl cnt(n + 1, INFL);
	cnt[0] = k;
	rep(i, n) {
		if (INFL / k < cnt[i]) break;
		cnt[i + 1] = cnt[i] * k;
	}
	repi(i, 0, n) {
		if (cnt[i] == INFL) break;
		cnt[i] = (cnt[i] - 1) / (k - 1);
	}
	dump(cnt);

	// 列が足りないなら false を返す．
	if (d > cnt[n]) return vi{ -1 };

	// 左から順に要素を決定していく．
	vi res;
	repir(i, n - 1, 0) {
		if (d == 0) return res;
		d--;

		ll q = d / cnt[i];
		res.push_back((int)q);

		d %= cnt[i];
		dump(d, res);
	}

	return res;
}


//【最小部分列】O(n k)
/*
* k = 26 種類の英小文字からなる文字列 s[0..n) の部分列のうち，
* 長さが m で辞書順で最小であるものを返す．
*
*（前から貪欲）
*/
string smallest_subsequence(const string& s, int m) {
	// verify : https://atcoder.jp/contests/typical90/tasks/typical90_f

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

	// 後ろの長さが足りている限り，前から貪欲に選んでいく．
	string res = "";
	int i = 0;
	rep(j, m) {
		rep(c, k) {
			// いまの長さ j と今後確保できる最大長 n - nxt[i][c] の和が m 以上なら選ぶ．
			if (j + (n - nxt[i][c]) >= m) {
				res += c + 'a';
				i = nxt[i][c] + 1;
				break;
			}
		}
	}

	return res;
}


//【最小部分列（長さごと）】O(n)
/*
* 数列 a[0..n) の長さ m の部分列のうち，辞書順で最小であるものを S(m) とおく．
* 各 j∈[0..n) について，S(j+1) に含まれるが S(j) に含まれない文字の位置を格納したリストを返す．
*
* 利用：【デカルト木】
*/
vi smallest_subsequence(const vi& a) {
	// verify : https://atcoder.jp/contests/abc262/tasks/abc262_f

	//【方法】
	// 最小要素を根とするデカルト木を構築し，行きがけ順かつ右の子優先でなぞれば良い．

	int n = sz(a);
	vi pos;

	Cartesian_tree<int> ct(a);

	function<void(int)> dfs = [&](int s) {
		pos.push_back(s);
		if (ct[s].rc != -1) dfs(ct[s].rc);
		if (ct[s].lc != -1) dfs(ct[s].lc);
	};
	dfs(ct.rt);

	return pos;
}


//【i 番目の部分列】O(n k)
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


//【最小最短非部分列】O(n k)
/*
* k = 26 種類の英小文字からなる文字列 s[0..n) の部分列でない最短の文字列のうち，
* 辞書順で最小であるものを返す．
*
*（部分列 DP）
*/
string shortest_nonsubsequence(const string& s) {
	// 参考 : https://qiita.com/drken/items/a207e5ae3ea2cf17f4bd
	// verify : https://atcoder.jp/contests/arc081/tasks/arc081_c

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

	// dp[i] : s[i..n) の部分列でない最短の文字列の長さ
	// head[i] : s[i..n) の部分列でない最短で辞書順最小の文字列の頭文字
	vi dp(n + 1, INF), head(n + 1);
	dp[n] = 1;

	// 後ろから貰う DP
	repir(i, n - 1, 0) {
		// 次に現れる文字 c について
		rep(c, k) {
			// s[i..n) で最初に文字 c が現れる位置
			int j = nxt[i][c];

			// もう c が現れないなら c を選ぶことができるからそれで良い．
			if (j == n) {
				dp[i] = 1;
				head[i] = c;
				break;
			}

			// c を選んだほうが短い文字列を作れるなら更新
			if (chmin(dp[i], dp[j + 1] + 1)) {
				head[i] = c;
			}
		}
	}

	// DP 復元
	string res;
	int i = 0;
	while (i < n) {
		int c = head[i];
		res += char(c + 'a');

		i = nxt[i][c] + 1;
	}

	return res;
}


//【最小最短共通非部分列】O(n m k)
/*
* k = 2 種類の数字からなる文字列 s[0..n), t[0..m) それぞれの
* 部分列でない最短の文字列のうち，辞書順で最小であるものを返す．
*
*（2 次元部分列 DP）
*/
string shortest_common_nonsubsequence(const string& s, const string& t) {
	int n = sz(s), m = sz(t);
	const int k = 2;

	// s_nxt[i][c] : s[i..n) で最初に文字 c が現れる位置（無いなら n）
	// t_nxt[j][c] : t[j..m) で最初に文字 c が現れる位置（無いなら n）
	vvi s_nxt(n + 1, vi(k, n)), t_nxt(m + 1, vi(k, m));
	repir(i, n - 1, 0) {
		rep(c, k) s_nxt[i][c] = s_nxt[i + 1][c];
		s_nxt[i][s[i] - '0'] = i;
	}
	repir(j, m - 1, 0) {
		rep(c, k) t_nxt[j][c] = t_nxt[j + 1][c];
		t_nxt[j][t[j] - '0'] = j;
	}

	// dp[i][j] : s[i..n), t[j..m) それぞれの部分列でない最短の文字列の長さ
	// head[i][j] : dp[i][j] を実現する辞書順最小の文字列の頭文字
	vvi dp(n + 1, vi(m + 1, INF)), head(n + 1, vi(m + 1));

	// 後ろから貰う DP
	repir(i, n, 0) {
		repir(j, m, 0) {
			if (i == n && j == m) {
				dp[i][j] = 1;
				head[i][j] = 0;
				continue;
			}

			// 次に現れる文字 c について
			rep(c, k) {
				// i2 : s[i..n) で最初に文字 c が現れる位置
				// j2 : t[j..m) で最初に文字 c が現れる位置
				int i2 = s_nxt[i][c], j2 = t_nxt[j][c];

				// もう c が現れないなら c を選ぶことができるからそれで良い．
				if (i2 == n && j2 == m) {
					dp[i][j] = 1;
					head[i][j] = c;
					break;
				}

				// c を選んだほうが短い文字列を作れるなら更新
				if (chmin(dp[i][j], dp[min(i2 + 1, n)][min(j2 + 1, m)] + 1)) {
					head[i][j] = c;
				}
			}
		}
	}

	// DP 復元
	string res;
	int i = 0, j = 0;
	bool s_flag = false, t_flag = false; // 部分列でないことが確定したか
	while (!s_flag || !t_flag) {
		int c = head[i][j];
		res += char(c + '0');

		i = s_nxt[i][c] + 1;
		j = t_nxt[j][c] + 1;

		if (i == n + 1) {
			i = n;
			s_flag = true;
		}
		if (j == m + 1) {
			j = m;
			t_flag = true;
		}
	}

	return res;
}


//【最小の連結文字列】O(n k^2 max|s[i]|)
/*
* n 個の文字列 s から k 個を選び連結して作られる辞書順最小の文字列を返す．
*
*（後ろから DP）
*/
string minimum_string_concat(vector<string> s, int k) {
	// verify : https://atcoder.jp/contests/abc225/tasks/abc225_f

	int n = sz(s);

	// 連結した時どっち向きの方が辞書順で小さいかで全順序を定義する．
	auto compare = [&](const string& a, const string& b) {
		return a + b < b + a;
	};

	// 先の全順序で s をソートする．
	// 全順序が推移律を満たすことにより
	//		∀i < j, s[i] + s[j] <= s[j] + s[i]
	// が成り立つので，この順での連結のみを考えれば良い．
	sort(all(s), compare);

	// dp_i[j] : s[i..n) から j 個選んだ場合の最小（s[0..i) としてはいけない！）
	string str_max = "z"; str_max[0]++;
	vector<string> dp(k + 1, str_max);
	dp[0] = "";

	repir(i, n - 1, 0) repir(j, min(n - i, k), 1) {
		// s[i] を使う方が小さくなるなら更新する．
		chmin(dp[j], s[i] + dp[j - 1]);

		// この更新式で大丈夫なのは，文字列の連結 "+" と辞書順比較 "<" の間に
		//		A + B < A + C ⇔ B < C
		// なる関係がある（"+" と "<" が左両立する）からである．
	}

	return dp[k];
}


