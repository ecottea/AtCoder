#pragma once
#include "header.h"
// ■■■■■ 順列に関するスコア和 ■■■■■


//【容積の和】O(n log n)
/*
* 幅と高さが w[i], h[i] で与えられる n 個の長方形の順列全てについての容積の和を返す．
*
*（挿入 DP）
*/
mint rectangle_capacity_sum(const vl& h, const vl& w) {
	// verify : https://atcoder.jp/contests/tenka1-2015-final-open/tasks/tenka1_2015_final_e

	int n = sz(h);

	// 長方形を高さ降順にソートする．
	vector<pll> hw(n);
	rep(i, n) hw[i] = { h[i], w[i] };
	sort(all(hw), greater<pll>());

	// dp[i] : 深さ h[i] の隙間を選ぶ確率（h[n] = 0 とする．）
	// 
	// dp[i] が well-defined であることの確認：
	//	長方形 j を挿入する直前の深さ h[i] の隙間を選ぶ確率を p[i] とおく．
	//	長方形 j を挿入する直前には長方形が j 個あり，したがって隙間は j + 1 個ある．
	//	よって深さ h[i] の隙間の個数の期待値は (j + 1)p[i] 個である．
	// 
	//	長方形 j を深さ h[i] の隙間に挿入する確率は p[i] であり，
	//	このとき深さ h[i] の隙間は 1 個増える．
	//	よって長方形 j を挿入した直後の深さ h[i] の隙間の個数の期待値は
	//		(j + 1)p[i] + p[i] = (j + 2)p[i]
	//	である．
	//	長方形 j を挿入した直後には隙間は j + 2 個なので，
	//	結局どの長方形を挿入するタイミングかによって確率が変わることはない．
	//
	// （ポリアの壺と同じ議論である．）
	vm dp(n + 1);
	dp[n] = 1;

	repi(i, 1, n - 1) {
		// 深さ h[i] の隙間ができるのは，直前の深さ 0 の隙間（両端のこと）に
		// 長方形 i を挿入した場合に限られる．
		// よって深さ h[i] の隙間の個数の期待値は 1 * dp[n] = dp[n] である．
		// 長方形 i を挿入した直後は隙間は i + 2 個なので，
		// 以降深さ h[i] の隙間が選ばれる確率は dp[n] / (i + 2) である．
		dp[i] = dp[n] / (i + 2);

		// 新しい隙間が選ばれる確率の分だけ両端が選ばれる確率を引いておく．
		dp[n] -= dp[i];
	}

	// acc_prob[i] : 深さ h[i] 以上の隙間を選ぶ確率
	// acc_dep[i] : 深さ h[i] 以上の隙間のみについての深さの期待値
	vm acc_prob(n + 1), acc_dep(n + 1);
	repi(i, 1, n - 1) {
		// 累積確率と累積期待値を計算する．
		acc_prob[i] = acc_prob[i - 1] + dp[i];
		acc_dep[i] = acc_dep[i - 1] + hw[i].first * dp[i];
	}

	mint res = 0;
	repi(i, 1, n - 1) {
		// 長方形 i を挿入することによって増える容積の期待値を考える．
		// 長方形 i（高さ h[i]）を h[i] より深い隙間に挿入する確率は acc_prob[i - 1] で，
		// このとき容積は，w[i] * (深さ - h[i]) だけ増える．
		// よって増える容積の期待値は
		//		w[i] * (深さ - h[i]) * acc_prob[i - 1]
		//		= (深さ * acc_prob[i - 1] - h[i] * acc_prob[i - 1]) * w[i]
		//		= (深さの期待値 - h[i] * 確率の和) * w[i]
		// である．
		res += (acc_dep[i - 1] - hw[i].first * acc_prob[i - 1]) * hw[i].second;
	}

	// 求めるべきものは総和なので，長方形の並べ方の数 n! を掛ける．
	repi(i, 1, n) res *= i;

	return res;
}


//【転倒数の和】O(n log n)
/*
* 位置 [0..n) の値が p[0..n) と指定（-1 なら任意）された [0..n) の順列全ての転倒数の和を返す．
*/
mint inversion_sum(const vi& p) {
	// verify : https://yukicoder.me/problems/no/2327

	int n = sz(p);

	// rem[j] : 数 i が未使用か
	vi rem(n, 1);
	rep(i, n) if (p[i] != -1) rem[p[i]] = 0;

	// acc_rem[j] : 数 [0..j) の中の未使用のものの個数
	vi acc_rem(n + 1);
	rep(i, n) acc_rem[i + 1] = acc_rem[i] + rem[i];

	// acc_open[i] : 位置 [0..i) の中の空きの個数
	vi acc_open(n + 1);
	rep(i, n) acc_open[i + 1] = acc_open[i] + (int)(p[i] == -1);

	// K : 空きの総数
	int K = acc_open[n];

	// 期待値を考える．
	mint ex = 0;

	// (-1, -1) からの寄与
	ex += mint(K) * (K - 1) / 4;

	if (K > 0) {
		mint K_inv = mint(K).inv();
		rep(i, n) {
			if (p[i] == -1) continue;

			// (p[i], -1) からの寄与
			ex += mint(acc_open[n] - acc_open[i]) * acc_rem[p[i]] * K_inv;

			// (-1, p[i]) からの寄与
			ex += mint(acc_open[i]) * (acc_rem[n] - acc_rem[p[i]]) * K_inv;
		}
	}

	// (p[i2], p[i]) からの寄与
	fenwick_tree<mint> FT(n);
	rep(i, n) {
		if (p[i] == -1) continue;

		ex += FT.sum(p[i], n);
		FT.add(p[i], 1);
	}

	// 個数に直す．
	mint res = ex;
	repi(i, 1, K) res *= i;

	return res;
}


