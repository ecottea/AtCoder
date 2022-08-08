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


