#pragma once
#include "header.h"
#include "二項係数.h"
// ■■■■■ 包除原理 ■■■■■


//【状態系包除原理】O(2^n)
/*
* 集合族 S[0..n) について，添字集合が set で表されるような集合族の交わりの大きさ
* #(∩i∈set S[i]) が c[set] であるときの，どの集合にも属さない要素の個数を返す．
*/
template <class T> T state_PIE(const vector<T>& c) {
	// verify : https://atcoder.jp/contests/tokiomarine2020/tasks/tokiomarine2020_e

	int n = msb(sz(c));

	T res = 0;

	repb(set, n) {
		int sign = (popcount(set) % 2 ? -1 : 1);
		res += (T)sign * c[set];
	}

	return res;
}


//【状態系包除原理（一括）】O(2^n n)
/*
* 集合族 S[0..n) について，添字集合が set で表されるような集合族の交わりの大きさ
* #(∩i∈set S[i]) が c[set] であるとする．
* 
* set には属するが ~set には属さない要素の個数を c'[set] とおくと，
* c' は c に上位集合でメビウス変換を施すことで得られる．
*/


//【個数系包除原理】O(n)
/*
* 集合族 S[0..n) について，k 個の集合の交わり（k = 0 なら全体集合とする）の大きさが
* 集合の選び方によらず一律 c[k] であるときの，どの集合にも属さない要素の個数を返す．
*
* 制約：fm は n! まで計算可能であること
*
* 利用：【階乗など（法が大きな素数）】
*/
mint counting_PIE(const vm& c, const Factorial_mint& fm) {
	// verify : https://atcoder.jp/contests/abc172/tasks/abc172_e

	int n = sz(c) - 1;

	mint res = 0;

	int sign = 1;
	repi(k, 0, n) {
		res += sign * fm.binomial(n, k) * c[k];
		sign *= -1;
	}

	return res;
}


//【約数系包除原理】
/*
*【倍数変換，GCD 畳込み】や【約数変換，LCM 畳込み】のメビウス変換を利用すればよい．
*/


//【指定バウンディングボックスをもつ点配置の数え上げ】O(1)
/*
* バウンディングボックスが [0..h)×[0..w) になるような n 個の格子点の配置の数を返す．
* 
* 制約：fm は (h * w)! まで計算可能であること
*
*（状態系包除原理）
*
* 利用：【階乗など（法が大きな素数）】
*/
mint count_points_in_BB(int n, int h, int w, Factorial_mint& fm) {
	// verify : https://atcoder.jp/contests/abc003/tasks/abc003_4

	mint res = 0;

	// 無条件の場合
	res += fm.binomial(h * w, n);

	// 少なくとも 1 個の辺が条件を満たしていない場合
	res -= 2 * fm.binomial((h - 1) * w, n); // 上または下
	res -= 2 * fm.binomial(h * (w - 1), n); // 左または右

	// 少なくとも 2 個の辺が条件を満たしていない場合
	res += fm.binomial((h - 2) * w, n); // 上下
	res += fm.binomial(h * (w - 2), n); // 左右
	res += 4 * fm.binomial((h - 1) * (w - 1), n); //（上または下）かつ（左または右）

	// 少なくとも 3 個の辺が条件を満たしていない場合
	res -= 2 * fm.binomial((h - 2) * (w - 1), n); // 上下かつ（左または右）
	res -= 2 * fm.binomial((h - 1) * (w - 2), n); //（上または下）かつ左右

	// 全ての辺が条件を満たしていない場合
	if (h >= 2 && w >= 2) res += fm.binomial((h - 2) * (w - 2), n); // 上下かつ左右

	return res;
}


//【倍数和 → 非倍数の和】O(2^n n O(mf))
/*
* 互いに異なる素数 ps[0..n) のいずれの倍数でもない数の集合を S とし，Σi∈S f(i) を返す．
* ただし mf(d) := Σd|i f(i) とする．
*
* ps を数 a の素因数のリストとすれば，i は a と互いに素な数を走査する．
*
*（状態系包除原理）
*/
template <class T> T nonmultiple_sum(vl& ps, function<T(ll)>& mf) {
	// verify : https://atcoder.jp/contests/abc206/tasks/abc206_e

	int n = sz(ps);

	T res = 0;

	// 包除原理を用いて計算する．
	// 例えば ps[0..2) = (2, 3) のとき，
	//		Σi∈S f(i) = Σ1|i f(i) - Σ2|i f(i) - Σ3|i f(i) + Σ6|i f(i)
	// となる．
	repb(set, n) {
		ll d = 1; // d の倍数を考える．
		int sign = 1; // sign : 符号因子

		rep(i, n) {
			if (set & (1 << i)) {
				// オーバーフロー対策
				if (d > INFL / ps[i]) goto NEXT_LOOP;

				d *= ps[i];
				sign *= -1;
			}
		}

		res += sign * mf(d);

	NEXT_LOOP:;
	}

	return res;
}


