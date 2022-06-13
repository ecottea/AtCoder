#pragma once
#include "header.h"
// ■■■■■ 括弧列 ■■■■■


//【括弧列の正しさ判定】O(n)
/*
* 文字列 s[0..n) が正しい括弧列かを返す．
*/
bool parenthesis_sequenceQ(const vc& s) {
	// verify : https://atcoder.jp/contests/arc141/tasks/arc141_c

	//【方法】
	// 括弧文字列 s[0..n) に対して，'(' を +1, ')' を -1 に置き換える操作を行い，
	// さらに左から累積和をとったものを acc[0..n] とする．このとき，
	//		s が正しい括弧列 ⇔ min(acc) = acc[n] = 0

	int n = sz(s);

	vi acc(n + 1);
	rep(i, n) {
		int val = 0;
		if (s[i] == '(') val = 1;
		if (s[i] == ')') val = -1;
		if (val == 0) return false;

		acc[i + 1] = acc[i] + val;
	}

	return *min_element(all(acc)) == 0 && acc[n] == 0;
}


//【色付き括弧列の正しさ判定】
/*
* [0..n) が各 2 回含まれる列 p[0..2n) について，各 i∈[0..n) について，
* 左右の i を色 i の "()" に対応させた列が正しい色付き括弧列かどうかを返す．
*/
bool parenthesis_sequenceQ(const vi& p) {
	// verify : https://atcoder.jp/contests/arc076/tasks/arc076_c

	int n = sz(p) / 2;
	if (n == 0) return true;

	// '(' を +1, ')' を -1 に置き換えつつ，'(' の位置を記憶する．
	vi seen(n, -1); vi a(2 * n);
	rep(i, 2 * n) {
		if (seen[p[i]] != -1) {
			a[i] = -1;
		}
		else {
			a[i] = 1;
			seen[p[i]] = i;
		}
	}

	// ±1 に直した列の累積和をとり，括弧のネスト数を得る．
	vi acc(2 * n + 1);
	rep(i, 2 * n) acc[i + 1] = acc[i] + a[i];

	// 各番号について対応する括弧の位置のネスト数が等しいかどうかをチェックする．
	rep(i, 2 * n) {
		int j = seen[p[i]];
		if (i == j) continue;

		// j < i
		if (acc[i + 1] != acc[j]) return false;
	}

	return true;
}


//【括弧列の対応と偶奇】
/*
* 括弧列 s において，s[i] = '(' と s[j] = ')' が対応する括弧であるとき i と j の偶奇は異なる．
*
* 証明：s(i..j) もまた括弧列であり，これの長さが偶数であることから直ちに従う．
* 
* verify : https://atcoder.jp/contests/agc048/tasks/agc048_b
*/


//【括弧列との内積の最大化】O(n log n)
/*
* 与えられた数列 a[0..2n) について，括弧列 s[0..2n) の '(' を +1, ')' を -1 に
* 置き換えた列と a[0..2n) との内積の最大値を返す．
*/
ll maximize_parenthesis_inner_product(const vl& a) {
	// verify : https://atcoder.jp/contests/abc250/tasks/abc250_g

	//【方法】
	// 括弧列に課された条件より，
	//		s[0..0] には +1 が 1 個以上
	//		s[0..2] には +1 が 2 個以上
	//		s[0..4] には +1 が 3 個以上 ...
	// 存在する必要があり，逆にこれらを満たす括弧列を構成することができる．
	// 
	// よって先頭から順に優先度付きキューに a の要素を入れていき，
	// a[2i] まで読む度に，その時点での最大の要素を取り出し，
	// これを +1 と積をとる要素として採用すれば良い．
	//
	// 最終的にキューに残った要素は -1 と積をとることになる．

	int n2 = sz(a);

	priority_queue<ll> q;
	ll res = 0;

	rep(i, n2) {
		q.push(a[i]);

		if (i % 2 == 0) {
			res += q.top();	q.pop();
		}
	}
	while (!q.empty()) {
		res -= q.top(); q.pop();
	}

	return res;
}


