#pragma once
#include "header.h"
// ■■■■■ ヤング図形 ■■■■■


//【ヤング図形】
/*
* 広義単調減少な正整数列 a[0..n) で，左から順にマスが a[0..n) 個並んだヤング図形を表す．
*/


//【ヤング図形の転置】O(n)
/*
* ヤング図形 a を転置したヤング図形を返す．
*/
vi transpose_yd(const vi& a) {
	// verify : https://yukicoder.me/problems/no/2149

	if (a.empty()) return vi();

	int n = sz(a);

	vi at(a[0]);
	rep(i, n) at[a[i] - 1]++;
	repir(i, a[0] - 2, 0) at[i] += at[i + 1];

	return at;
}


//【ヤング図形のドミノ分割】O(n)
/*
* ヤング図形 a[0..n) を左上を 0 とする 0, 1 の市松模様に彩色する．
* a をドミノに分割し，各 k∈[0,1] に対し，右上が k であるようなドミノだけを抽出して作った
* 新たなヤング図形を b[k] に格納して b[0..1] を返す（分割不可能なら空配列を返す）
*/
vvi domino_division_yd(const vi& a) {
	// 参考 : https://mathlog.info/articles/2214
	// verify : https://yukicoder.me/problems/no/2149

	int n = sz(a);

	vvi b(2); int h = 0;

	// 左から順に縦にどんどん削っていき，余りが出たら右に階段状に削っていく．
	rep(i, n) {
		int p = (i + a[i] + 1) % 2;
		b[p].push_back((a[i] - h) / 2);

		if ((a[i] - h) & 1) {
			b[p].back() += ++h;
		}
		else {
			h = max(h - 1, 0);
		}
	}
	rep(k, 2) while (!b[k].empty() && b[k].back() == 0) b[k].pop_back();

	// 削りきれなかったならドミノ分割不可能．
	return h == 0 ? b : vvi();
}


//【標準タブローの数え上げ】O(Σa)
/*
* ヤング図形 a に対応する標準タブローの個数を返す．
* 戻り値は，ヤング図形であるという性質を保ちながら 1 マスずつ正方形を配置する方法の数とも解釈できる．
*
* 利用：【ヤング図形の転置】
*/
mint hook_length_formula(const vi& a) {
	// 参考 : https://zenn.dev/koboshi/articles/306304c0381c1e
	// verify : https://yukicoder.me/problems/no/2149

	int n = sz(a);

	// a_sum : マスの総数（分割対象の自然数）
	int a_sum = accumulate(all(a), 0);

	// 分子は a_sum!
	mint num = 1;
	repi(i, 1, a_sum) num *= i;

	vi at = transpose_yd(a);

	// 分母は各マスにおけるフック長の積
	mint dnm = 1;
	rep(i, n) rep(j, a[i]) dnm *= (a[i] - j) + (at[j] - i) - 1;

	return num / dnm;
}


