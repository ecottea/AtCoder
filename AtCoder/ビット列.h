#pragma once
#include "header.h"
#include "探索.h"
#include "フロー双対.h"
// ■■■■■ ビット列 ■■■■■


//【別の表現】
/*
* 0, 1 からなるビット列の有用な別の表現として，
*	(a) 累積和（区間内の 1 の個数が分かる）
*	(b) 初項と差分
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


//【1 の連の長さ】O(n)
/*
* ビット列 s[0..n) について，'0' で区切られた '1' の連の長さを順に並べた列を len に格納する．
*/
void length1(const string& s, vi& len, char one = '1') {
	// verify : https://atcoder.jp/contests/agc046/tasks/agc046_c

	len.clear();

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
}


//【2 連の変換の swap への帰着】
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


//【区間のビット反転】
/*
* ビット列の区間 [l1, r1] と [l2, r2] を共にビット反転した結果は，
* 区間 [l1, r2] と [l2, r1] を共にビット反転した結果に一致する．
*
* verify : https://atcoder.jp/contests/jsc2019-qual/tasks/jsc2019_qual_c
*/


//【区間幅固定ビット反転】O(n)
/*
* ビット列 s[0..n) について，幅 k の連続部分列 s[i..i+k) に対するビット反転操作を
* 任意の位置と回数で行うことで全てのビットを '0' とできるかを返す．
*/
bool interval_flip(const string& s, int k, char one = '1') {
	int n = sz(s);

	//【方法】
	// 操作の順序は無視でき，2 回行うと元に戻るので，
	// 左から順に操作を行うか否かを貪欲に定めていき，右側が全て 0 となるかを見れば良い．
	// 実際に操作を行うのではなく，操作によって反転されたかどうかが分かるようにする．

	// flip : 今走査している箇所がビット反転されているか
	bool flip = false;

	// ed[i] : s[i-k..i) のビット反転を行ったか
	vb ed(n + 1);

	repi(i, 0, n - k) {
		flip ^= ed[i];

		// s[i] == '1' なら s[i..i+k) を反転する．
		if (flip ^ (s[i] == one)) {
			ed[i + k] = true;
			flip = !flip;
		}
	}
	dump(ed);

	repi(i, n - k + 1, n - 1) {
		flip ^= ed[i];

		// s[i] == '1' なら不可能
		if (flip ^ (s[i] == one)) {
			return false;
		}
	}

	return true;
}


//【区間 01 平衡化】O(n + m)
/*
* ビット列 b[0..n) で，m 個の区間 [l[i], r[i]) に含まれる 0, 1 の個数が等しいもののうち，
* 辞書順最小のものを b に格納する．（存在しなければ false を返す．）
*
* 利用：【牛ゲー（01-上界指定のみ）】
*/
bool interval_balanced_bitseq(int n, const vi& l, const vi& r, vi& b) {
	// verify : https://atcoder.jp/contests/agc056/tasks/agc056_c

	//【方法】
	// b[0..n) のビット '0'['1'] を数値 1[-1] に置き換え，
	// さらに累積和 acc[i] = Σb[0..i) を導入すると，満たすべき条件は
	//		|acc[i] - acc[i-1]| = 1  ...(1)
	//		acc[r] - acc[l-1] = 0    ...(2)
	// と書き直すことができ，目指すべきは「acc の辞書順最大化」となる．
	// 
	// 満たすべき条件 (1) を緩めて
	//		|acc[i] - acc[i-1]| <= 1 ...(3)
	// とすれば，acc の各項の大きさにはトレードオフの関係は無いので，
	// 単に「acc の各項の最大化」を目指すだけで良い．
	// 
	// 条件 (2), (3) はまとめて
	//		acc[i] - acc[i-1] <= 1
	//		acc[i-1] - acc[i] <= 1
	//		acc[r] - acc[l-1] <= 0
	//		acc[l-1] - acc[r] <= 0
	// と差の上界指定のみで書き直せるので，これは 01-BFS を用いた牛ゲーで解ける．
	//
	// 条件を (1) → (3) と緩めてしまったことが気になるが，最適解では
	//		acc[i] = acc[i-1]
	// となる i は存在しないので問題ない．
	// 
	// 実際，もしそのような i があるとすれば，
	// 条件 (2) より (i, j) が区間端を跨がないような j であって
	//		acc[j] = acc[j-1]
	// を満たすものが存在する．
	// しかしそれならば acc[i] と acc[j-1] の間を 1 ずつ増やせるはずだから矛盾である．

	int m = sz(l);
	b.resize(n);

	Ushige_ub01_only ushi(n + 1);

	rep(j, m) {
		if ((r[j] - l[j]) % 2 == 1) return false;

		ushi.set_ub(l[j], r[j], 0);
		ushi.set_ub(r[j], l[j], 0);
	}

	rep(i, n) {
		ushi.set_ub(i, i + 1, 1);
		ushi.set_ub(i + 1, i, 1);
	}

	vi diff;
	ushi.maximize_diff(0, diff);

	rep(i, n) {
		if (diff[i + 1] - diff[i] == 1) b[i] = 0;
		else b[i] = 1;
	}

	return true;
}


