#pragma once
#include "header.h"
#include "分析(文字列).h"
#include "二項係数.h"
#include "数論変換.h"
// ■■■■■ 文字列の数え上げ ■■■■■


//【文字列の数え上げ（分布上限指定，長さ毎，mod 998244353）】O(n (log n)^2) （n = Σcnt）
/*
* 各文字 c を cnt[c] 個以下使って得られる文字列のうち，
* 長さが i（∈[0..n]）であるものの個数のリストを返す．
*
* 利用：【階乗など（法が大きな素数）】,【畳込み（複数，mod 998244353）】
*
*（挿入 DP，指数型母関数）
*/
vm count_strings(const vi& cnt) {
	// verify : https://atcoder.jp/contests/abc358/tasks/abc358_e

	//【方法】
	// 文字列の長さを状態にもち，1 種類ずつ文字を挿入していく挿入 DP を用いる．
	// dp_i[j] を i 種類目までの文字を使って得られる長さ j の文字列の個数とする．
	// 
	// 長さ j の文字列を得たければ，長さ t の文字列に j - t 個の文字を挿入すればいい．
	// 挿入の仕方は重複組合せの考え方より bin(j, t) 通りあるから，遷移式
	//		dp_(i+1)[j] = Σt bin(j, t) dp_i[t]
	//		⇔ dp_(i+1)[j] = Σt j! / ((j-t)! t!) dp_i[t]
	// を得る．このままでは畳込みの形になっていないが，これを
	//		dp_(i+1)[j] / j! = Σt (1 / (j-t)!) (dp_i[t] / t!)
	// と変形すれば畳込みの形になっている．
	// 畳込みは可換なので長さの短い順に計算していくのが良い．

	int k = sz(cnt);
	int n = accumulate(all(cnt), 0);

	Factorial_mint fm(n);

	vvm fs(k);
	rep(i, k) {
		fs[i].resize(cnt[i] + 1);
		repi(j, 0, cnt[i]) fs[i][j] = fm.fact_inv(j);
	}

	vm res = multi_convoluion(fs);
	repi(j, 0, n) res[j] *= fm.fact(j);

	return res;
}


//【文字列の数え上げ（部分列指定）】O(n)
/*
* 文字列 s[0..m) を部分列にもつ英小文字からなる長さ n の文字列の個数を返す．
*
* 利用：【階乗など（法が大きな素数）】
*/
mint count_strings_contain_substring(const string& s, int n, int k = 26) {
	// verify : https://atcoder.jp/contests/abc171/tasks/abc171_f

	//【方法】
	// n 箇所の中から m 箇所を選んでそこに s[0..m) を順に配置し，
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
		res += fm.bin(i - 1, m - 1) * pow25[i - m] * pow26[n - i];
	}

	return res;
}


//【文字列の数え上げ（置換，部分列指定）】O(n m)
/*
* k 種の英小文字と '?' からなる文字列 s[0..n) について，それぞれの '?' を英小文字に置き換えて
* 得られる文字列のうち，t[0..m) を部分列に含むものの個数を返す．
* 
*（耳 DP）
*/
mint count_replaced_strings_contain_subsequence(const string& s, const string& t, int k = 26) {
	int n = sz(s), m = sz(t);

	vvm dp(n + 1, vm(m + 1));
	dp[0][0] = 1;

	rep(i, n) repi(j, 0, m) {
		if (s[i] == '?') {
			dp[i + 1][min(j + 1, m)] += dp[i][j];
			dp[i + 1][j] += (k - 1) * dp[i][j];
		}
		else {
			if (j < m && s[i] == t[j]) dp[i + 1][j + 1] += dp[i][j];
			else dp[i + 1][j] += dp[i][j];
		}
	}

	return dp[n][m];
}


//【文字列の数え上げ（置換，部分文字列指定）】O(n m 26)
/*
* k 種の英小文字と '?' からなる文字列 s[0..n) について，それぞれの '?' を英小文字に置き換えて
* 得られる文字列のうち，w[0..m) を部分文字列に含むものの個数を返す．
*/
mint count_replaced_strings_contain_substring(const string& s, const string& w, int k = 26, char A = 'a') {
	// verify : https://mojacoder.app/users/milkcoffee/contests/milkcoffee-contest-001/tasks/5

	//【方法】
	// KMP 法を書き写し，数え上げ DP 用に書き直した．

	int n = sz(s), m = sz(w);

	// l[i] : w[0..i) の接頭辞と接尾辞が最大何文字一致しているか（i 文字未満）
	vi l(m + 1);
	l[0] = -1;

	// モーリスプラット
	int j = -1;
	rep(i, m) {
		while (j >= 0 && w[i] != w[j]) j = l[j];
		l[i + 1] = ++j;
	}

	vvm dp(n + 1, vm(m + 1));
	dp[0][0] = 1;

	rep(ij, n) {
		// c ごとに (i++, j--) のようなことが起こって dp[ij] が改変され，後に影響を与えてしまう．
		// それを元に戻すためのバックアップ．
		auto dp_prv(dp[ij]);

		rep(c, k) {
			if (s[ij] != '?' && s[ij] != A + c) continue;

			repir(j, min(ij, m), 0) {
				int i = ij - j;

				if (j == m) {
					dp[ij + 1][j] += dp[ij][j];
				}
				else if (w[j] == A + c) {
					dp[ij + 1][j + 1] += dp[ij][j];
				}
				else {
					int ni = i + j - l[j];
					int nj = (j > 0 ? l[j] : j);
					dp[ni + nj][nj] += dp[ij][j];
				}
			}

			repir(j, min(ij, m), 0) dp[ij][j] = dp_prv[j];
		}
	}

	return dp[n][m];
}


//【部分文字列の数え上げ】O(n)
/*
* 文字列 s[0..n) の部分文字列の個数を返す（空文字列含む）
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
template <class STR>
ll count_palindromes(const STR& s) {
	int n = sz(s);

	vi lo, le;
	manacher(s, lo, le);

	ll res = 0;
	rep(i, n) res += (lo[i] + 1) / 2;
	rep(i, n - 1) res += le[i] / 2;

	return res;
}


