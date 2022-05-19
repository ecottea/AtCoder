#pragma once
#include "header.h"
// ■■■■■ ブロックデザイン ■■■■■


//【ブロックデザイン】
/*
* t-(v, k, λ)-デザインとは，以下の条件を満たすものである：
*	v: 点の数は v 個である．
*	k: ひとつのブロックに含まれる点の数は k 個である．
*	t,λ: 任意の異なる t 個の点について，それらを含むブロックは λ 個である．
* 
* さらに
*	b: ブロックの個数
*	r: ある点を含むブロックの個数
* なるパラメータを考えると，これらの間には
*	b k = v r
*	λ bin(v, t) = b bin(k, t)
* なる関係が成り立つ．
* 
* Mathematica 用：
*	Solve[{b k == v r, \[Lambda] Binomial[v, t] == b Binomial[k, t]}]
* 
* 例えば，ファノ平面
*	B1 = { 0, 1, 2             }
*	B2 = { 0,       3, 4       }
*	B3 = {    1,    3,    5    }
*	B4 = { 0,             5, 6 }
*	B5 = {    1,       4,    6 }
*	B6 = {       2, 3,       6 }
*	B7 = {       2,    4, 5    }
* は 2-(7, 3, 1)-デザインであり，b=7, r=3 である．
*/


//【双対ブロックデザイン】O(b k + v r)
/*
* t-(v, k, λ)-デザイン blk[0..b)[0..k) において点とブロックの役割を交換することにより，
*	b: 点の数は b 個である．
*	r: ひとつのブロックに含まれる点の数は r 個である．
*	v: ブロックの個数は v 個である．
*	k: ある点を含むブロックの個数は k 個である．
*	t,λ: 任意の異なる t 個のブロックについて，それらが共通して含む点は λ 個である．
* を満たすデザインを dblk[0..v)[0..r) に格納する．
*/
void create_dual_design(const vvi& blk, vvi& dblk) {
	// verify: https://atcoder.jp/contests/tenka1-2018/tasks/tenka1_2018_d

	int v = 0;
	repe(bl, blk) chmax(v, *max_element(all(bl)) + 1);
	dump(v);

	dblk = vvi(v);

	rep(i, sz(blk)) {
		repe(j, blk[i]) {
			// 元のデザインでブロック i に点 j が含まれているとき，
			// 双対デザインではブロック j に点 i が含まれている．
			dblk[j].push_back(i);
		}
	}
}


//【2-(p^2, p, 1)-デザイン】O(p^3)
/*
* 2-(p^2, p, 1)-デザインを構成し，blk[0..b)[0..p) に格納する．
* なお b = p(p+1), r = p+1 である．
* 
* 制約：p は素数
*/
void create_finite_affine_design(int p, vvi& blk) {
	// verify : https://atcoder.jp/contests/arc019/tasks/arc019_4

	//【方法】
	// 位数 p の有限アフィン平面 Af(p) には，n = p^2 個の点が存在する．
	// これらをデザインの p^2 個の点に対応させる．
	// 
	// Af(p) には，傾き [0..p) および ∞ の p(p+1) 本の直線が存在する．
	// 各直線には p 個の点が含まれ，どの異なる 2 点についても
	// それらを通る直線がただ 1 つ存在するので，これらの直線をブロックに選べる．

	blk = vvi(p * (p + 1), vi(p));

	// a : 直線の傾き∈[0..p)
	rep(a, p) {
		// b : 直線の y 切片
		rep(b, p) {
			// x : 直線の x 座標
			rep(x, p) {
				// y : 直線の y 座標
				int y = (a * x + b) % p;

				blk[a * p + b][x] = x * p + y;
			}
		}
	}

	// 直線の傾きが ∞ の場合
	// b : 直線の x 切片
	rep(b, p) {
		// y : 直線の y 座標
		rep(y, p) {
			// x : 直線の x 座標
			int x = b;

			blk[p * p + b][y] = x * p + y;
		}
	}
}


//【長方形を含まない彩色】O(p^4)
/*
* n = p^2 とおく．n*n 格子の p 色での彩色のうち，長方形の頂点を成す 4 点で
* すべて同色のものが含まれないものを col[0..n)[0..n) に格納する．
* 
* 制約：p は素数
*/
void non_rectangle_coloring(int p, vvi& col) {
	// verify : https://atcoder.jp/contests/arc140/tasks/arc140_e

	//【方法】
	// 位数 p の有限アフィン平面 Aff(p) には，n = p^2 個の点が存在する．
	// これらを col の n 個の列に対応させる．
	// 
	// Aff(p) には y 軸平行でない n = p^2 本の直線が存在する．
	// 傾き a を固定して，切片ごとに各直線上の点を色分けすれば，col の行が得られる．
	// 傾き a の選び方が p 通り，色番号の巡回のさせ方が p 通りあるので，
	// col の行が p * p = n 個得られた．

	col = vvi(p * p, vi(p * p));

	// a : 直線の傾き
	rep(a, p) {
		// b : 直線の切片
		rep(b, p) {
			// x : 直線の x 座標
			rep(x, p) {
				// y : 直線の y 座標
				int y = (a * x + b) % p;

				// k : 色番号をいくつ巡回するか
				rep(k, p) {
					col[a * p + k][x * p + y] = (b + k) % p;
				}
			}
		}
	}
}


//【2-(p^2+p+1, p+1, 1)-デザイン】O(p^3)
/*
* 2-(p^2+p+1, p+1, 1)-デザインを構成し，blk[0..b)[0..p) に格納する．
* なお b = p^2+p+1, r = p+1 である．
*
* 制約：p は素数
*/
void create_finite_projective_design(int p, vvi& blk) {
	// verify : https://atcoder.jp/contests/cf17-final-open/tasks/cf17_final_f

	//【方法】
	// 位数 p の有限射影平面 Pj(p) には，n = p^2+p+1 個の点が存在する．
	// これらをデザインの p^2+p+1 個の点に対応させる．
	// 
	// Pj(p) には，傾き [0..p) および ∞ の p(p+1) 本の直線と
	// 無限遠直線を合わせた p^2+p+1 本の直線が存在する．
	// 各直線には無限遠点を合わせて p+1 個の点が含まれ，どの異なる 2 点についても
	// それらを通る直線がただ 1 つ存在するので，これらの直線をブロックに選べる．

	blk = vvi(p * p + p + 1, vi(p + 1));

	// a : 直線の方向が (1,a)
	rep(a, p) {
		// b : 直線の y 切片
		rep(b, p) {
			// x : 直線の x 座標
			rep(x, p) {
				// y : 直線の y 座標
				int y = (a * x + b) % p;

				blk[a * p + b][x] = x * p + y;
			}

			// (1,a) 方向の無限遠点
			blk[a * p + b][p] = p * p + a;
		}
	}

	// (0,1) 方向の直線
	// b : 直線の x 切片
	rep(b, p) {
		// y : 直線の y 座標
		rep(y, p) {
			// x : 直線の x 座標
			int x = b;

			blk[p * p + b][y] = x * p + y;
		}

		// (0,1) 方向の無限遠点
		blk[p * p + b][p] = p * p + p;
	}

	// 無限遠直線
	// 方向が (0,a)
	rep(a, p) {
		blk[p * p + p][a] = p * p + a;
	}
	// 方向が (1,0)
	blk[p * p + p][p] = p * p + p;
}


//【2-(v, 2, 1)-デザイン】O(v^2)
/*
* 2-(v, 2, 1)-デザインを構成し，blk[0..b)[0..2) に格納する．
* なお b = v(v-1)/2, r = v-1 である．
*/
void create_all_pair_design(int v, vvi& blk) {
	// verify : https://atcoder.jp/contests/tenka1-2018/tasks/tenka1_2018_d

	//【方法】
	// [0..v) の大きさ 2 の部分集合全てをブロックにすればよい．

	blk = vvi(v * (v - 1) / 2, vi(2));

	int cnt = 0;
	rep(j, v) {
		rep(i, j) {
			blk[cnt][0] = i;
			blk[cnt][1] = j;
			cnt++;
		}
	}
}


//【2-(2^n-1, 2^(n-1)-1, 2^(n-2)-1)-デザイン】O(4^n)
/*
* 2-(2^n-1, 2^(n-1)-1, 2^(n-2)-1)-デザインを構成し，blk[0..b)[0..2^(n-1)-1) に格納する．
* なお b = 2^n-1, r = 2^(n-1)-1 である．
*/
void create_hadamard_design(int n, vvi& blk) {
	//【方法】
	// 2^n * 2^n アダマール行列の 1 行目と 1 列目を削除し，
	// 2^n - 1 列を点，2^n - 1 行をブロックとみなせばよい．

	blk = vvi((1 << n) - 1);

	function<void(int, int, int, int)> rf = [&](int k, int x, int y, int sgn) {
		if (k == 0) {
			if (x > 0 && y > 0 && sgn == 1) blk[x - 1].push_back(y - 1);

			return;
		}

		int shift = 1 << (k - 1);
		rf(k - 1, x, y, sgn);
		rf(k - 1, x + shift, y, sgn);
		rf(k - 1, x, y + shift, sgn);
		rf(k - 1, x + shift, y + shift, -sgn);
	};

	rf(n, 0, 0, 1);
}


//【2-(2^n, 2^(n-1), 2^(n-1)-1)-デザイン】O(4^n)
/*
* 2-(2^n, 2^(n-1), 2^(n-1)-1)-デザインを構成し，blk[0..b)[0..2^(n-1)) に格納する．
* なお b = 2(2^n-1), r = 2^n-1 である．
*/
void create_double_hadamard_design(int n, vvi& blk) {
	// verify : https://atcoder.jp/contests/keyence2021/tasks/keyence2021_d

	//【方法】
	// 2^n * 2^n アダマール行列の 2 行目以降の 2^n - 1 行と，
	// それらを -1 倍した 2^n - 1 行とをブロックにすれば良い．

	blk = vvi(2 * (1 << n) - 2);

	function<void(int, int, int, int)> rf = [&](int k, int x, int y, int sgn) {
		if (k == 0) {
			if (x == 0) return;

			if (sgn == 1) blk[x - 1].push_back(y);
			else blk[(1 << n) - 1 + x - 1].push_back(y);

			return;
		}

		int shift = 1 << (k - 1);
		rf(k - 1, x, y, sgn);
		rf(k - 1, x + shift, y, sgn);
		rf(k - 1, x, y + shift, sgn);
		rf(k - 1, x + shift, y + shift, -sgn);
	};

	rf(n, 0, 0, 1);
}


