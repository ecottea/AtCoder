#pragma once
#include "header.h"
// ■■■■■ ビット全探索 ■■■■■


//【上位集合の全探索】O(2^|Ω-A|)
/*
* 大きさ d の全体集合 Ω とその部分集合 A ⊂ Ω について，
* A ⊂ set ⊂ Ω なる set を昇順に全探索する．
*/
// verify : https://onlinejudge.u-aizu.ac.jp/courses/lesson/8/ITP2/all/ITP2_11_B
#define repbu(set, A, d) for(int set = A; set < (1 << int(d)); set = (set + 1) | A)


//【下位集合の全探索】O(2^|A|)
/*
* 大きさ d の全体集合 Ω とその部分集合 A ⊂ Ω について，
* set ⊂ A ⊂ Ω なる set を降順に全探索する．
*/
// verify : https://atcoder.jp/contests/dp/tasks/dp_u
#define repbs(set, A) for(int set = int(A), bset##A = 1; bset##A > 0; bset##A = set, set = (set - 1) & int(A)) 


//【部分集合の全探索（大きさ固定）】O(nCr)
/*
* 大きさ n の全体集合 Ω のうち，大きさ r の部分集合 set⊂Ω を昇順に全探索する．
*/
// verify : https://onlinejudge.u-aizu.ac.jp/courses/lesson/8/ITP2/all/ITP2_11_D
#define repbc(set, n, r) for(int set = (1 << int(r)) - 1, lb, nx; set < (1 << int(n)); lb = set & -set, nx = set + lb, set = (((set & ~nx) / lb) >> 1) | nx)


//【ビット全探索の高速化】
/*
* repb(set, n) rep(i, n) で O(2^n n) かかるビット全探索は，
* 再帰関数を用いて書く，グレイコードを使うなどして O(2^n) に高速化できる．
* 
* verify : https://yukicoder.me/problems/no/3105
*/


//【bitDP の遷移を細分化】
/*
* bitDP を「集合 → 集合」の遷移をする DP と解釈すれば，
* 遷移を細かくして「要素 → 要素」の遷移を考えることで計算量を落とせる場合がある．
* 
* verify : https://atcoder.jp/contests/typical90/tasks/typical90_w
*/


//【i 番目のグレイコード】
/*
* i 番目（0-indexed）のグレイコード c = g(i) は以下の式で得られる：
*	c = i XOR (i >> 1)
* 
* 逆に g(i) = c となるような i は以下の式で得られる：
*	i = c XOR (c >> 1) XOR (c >> 2) XOR ...
* 
* verify : https://atcoder.jp/contests/agc031/tasks/agc031_c
*/


//【2 選択和数え上げ問題】O(2^n n)
/*
* 長さ n の列 a, b と目標値 sum が与えられる．
* 各 i ごとに a[i] か b[i] のいずれかを選択して得られる和が
* ちょうど sum になるものの個数を返す．
*
*（ビット全探索）
*/
template <class T>
int count_2select_sum(vector<T>& a, vector<T>& b, T sum) {
	int n = sz(a);

	int cnt = 0;

	// set の i ビット目が a[i] と b[i] のどちらを選択するかを表す．
	repb(set, n) {
		// 選択されたものの和を計算する．
		T v = 0;
		rep(i, n) {
			// set の i ビット目が 1 であれば a[i] を選択する．
			if (set & (1 << i)) {
				v += a[i];
			}
			// set の i ビット目が 0 であれば b[i] を選択する．
			else {
				v += b[i];
			}
		}

		// 和が目標に一致していればカウントする．
		if (v == sum) {
			cnt++;
		}
	}

	return cnt;
}


//【3 選択和数え上げ問題】O(3^n n)
/*
* 長さ n の列 a, b, c と目標値 sum が与えられる．
* 各 i ごとに a[i], b[i], c[i] のいずれかを選択して得られる和が
* ちょうど sum になるものの個数を返す．
*
*（3 進全探索）
*/
template <class T>
int count_3select_sum(vector<T>& a, vector<T>& b, vector<T>& c, T sum) {
	int n = sz(a);

	int cnt = 0;

	// tit が 3 進数表記されているものと考える．
	// tit の i 桁目が a[i], b[i], c[i] のいずれを選択するかを表す．
	rep(tit, pow(3, n)) {
		// 選択されたものの和を計算する．
		T v = 0;
		rep(i, n) {
			// tit の i 桁目を抜き出す．
			switch ((tit / pow(3, i)) % 3) {
			case 0:
				v += a[i];
				break;
			case 1:
				v += b[i];
				break;
			case 2:
				v += c[i];
				break;
			default:;
			}
		}

		// 和が目標に一致していればカウントする．
		if (v == sum) {
			cnt++;
		}
	}

	return cnt;
}


