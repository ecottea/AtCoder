#pragma once
#include "header.h"
// ■■■■■ 括弧列 ■■■■■


//【括弧列の正しさ判定】
/*
* 括弧列 s[0..2n) に対して，'(' を +1, ')' を -1 に置き換える操作を行い，
* さらに左から累積和をとったものを a[0..2n] とする．このとき，
*		s が正しい括弧列 ⇔ min(a) = a[2n] = 0
* 
* verify : https://atcoder.jp/contests/abc223/tasks/abc223_f
*/


//【括弧列の対応】
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
	// よって先頭から順に優先度付きキューに a の要素を入れていき，
	// a[2i] まで読む度に最大の要素を +1 と積をとる要素として選択すれば良い．

	int n = sz(a);

	priority_queue<ll> q;
	ll res = 0;

	rep(i, n) {
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


