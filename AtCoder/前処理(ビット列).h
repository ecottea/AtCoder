#pragma once
#include "header.h"
// ■■■■■ ビット列に対する汎用性のある前処理 ■■■■■


//【別の表現】
/*
* 0, 1 からなるビット列の有用な別の表現として，
*	(a) 累積和（区間内の 1 の個数が分かる）
*	(b) 初項と差分（区間への操作を局所操作に変換できる）
*	(c) -1 と 1 の列
*	(d) -1 と 1 の列の累積和（区間内の 0, 1 の個数の偏りが分かる）
*	(e) 1 がある位置を並べた列（2 列の swap 距離が分かる）
*	(f) i 番目の 1 がある位置の位置 i からの偏差を並べた列（1 の連が判別できる）
*	(g) 0 で区切られた 1 の連の長さを並べた列
* などがある．
*
* 例：
*	元  :     0  1  0  1  1  1  0  0  1  0  1
*	(a) :  0  0  1  1  2  3  4  4  4  5  5  6
*	(b) :     0  1 -1  1  0  0 -1  0  1 -1  1
*	(c) :    -1  1 -1  1  1  1 -1 -1  1 -1  1
*	(d) :  0 -1  0 -1  0  1  2  1  0  1  0  1
*	(e) :        1     3  4  5        8    10
*	(f) :        1     2  2  2        4     5
*	(g) :  0  1     3           0  1     1
*
* verify :
*	(d) : https://atcoder.jp/contests/agc056/tasks/agc056_c
*	(e) : https://atcoder.jp/contests/tkppc6-2/tasks/tkppc6_2_b
*	(f) : https://atcoder.jp/contests/abc229/tasks/abc229_g
*	(g) : https://atcoder.jp/contests/agc046/tasks/agc046_c
*/


//【1 の位置】O(n)
/*
* ビット列 s[0..n) について，'1' の位置のリストを返す．
*/
vi position1(const string& s, char one = '1') {
	// verify : https://atcoder.jp/contests/arc166/tasks/arc166_a

	vi pos;

	int n = sz(s);
	rep(i, n) if (s[i] == one) pos.push_back(i);

	return pos;
}


//【1 の連の長さ】O(n)
/*
* ビット列 s[0..n) について，'0' で区切られた '1' の連の長さを順に並べた列を返す．
*/
vi length1(const string& s, char one = '1') {
	// verify : https://atcoder.jp/contests/agc046/tasks/agc046_c

	vi len;

	int l = 0;
	repe(c, s) {
		if (c == one) {
			l++;
		}
		else {
			len.push_back(l);
			l = 0;
		}
	}
	len.push_back(l);

	return len;
}


//【2 連の反転の swap への帰着】
/*
* ビット列 a に対する
*		[00] → [11], [11] → [00]
* という遷移は，a の奇数ビットだけを反転させることにより swap と等価になる．
*
* verify : https://atcoder.jp/contests/tkppc6-2/tasks/tkppc6_2_b
*/


//【隣からの和の swap への帰着】
/*
* 3 項間の遷移
*		a[i] += a[i-1] + a[i+1]
* は，標数 2 なら
*		b[i] = a[i] + a[i+1]
* とおくことで，
*		b[i] += b[i] + b[i+1]	⇔ b[i] ← b[i+1]
*		b[i+1] += b[i] + b[i+1]	⇔ b[i+1] ← b[i]
* なる 2 項間の遷移に書き換えられ，これは swap(b[i], b[i+1]) と等価である．
*
* verify : https://yukicoder.me/problems/no/2018
*/


