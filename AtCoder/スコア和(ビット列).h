#pragma once
#include "header.h"
#include "FPS(mint).h"
// ■■■■■ スコア和（ビット列） ■■■■■


//【1 が k 個連続しないビット列のスコア和（mod998244353）】O(k log k log n)
/*
* 長さ n のビット列のうち，1 が k 個連続しないもの全てについて，
* スコアを p^(1 の個数) と定めたときのスコア和を返す．
*
* 利用：【展開係数】
*/
mint scoresum_noncontinuous_bitsequences(ll n, int k, mint p) {
	if (k == 0) return 0;

	vm powp(k);
	powp[0] = 1;
	repi(i, 1, k - 1) powp[i] = powp[i - 1] * p;

	// 母関数は (1+pz+...+p^(k-1)z^(k-1)) / (1-z-pz^2...-p^(k-1)z^k) である．
	vm fc(k), gc(k + 1);
	rep(i, k) fc[i] = powp[i];
	gc[0] = 1;
	repi(i, 1, k) gc[i] = -powp[i - 1];

	return bostan_mori(MFPS(fc), MFPS(gc), n);
}


//【0 のみからなる区間を含まないビット列の積スコアの和】O(n log n + m)
/*
* ビット列 s[0..n) のスコアを Πi∈[0..n) sc[s[i]][i] と定める．
* m 個の半開区間 [l[j]..r[j]) それぞれについて少なくとも 1 つの '1' が含まれる
* ようなビット列全てについてのスコアの和を返す．
*/
mint op_n0bs(mint x, mint y) { return x + y; }
mint e_n0bs() { return 0; }
mint act_n0bs(mint f, mint x) { return f * x; }
mint comp_n0bs(mint f, mint g) { return f * g; }
mint id_n0bs() { return 1; }
mint non0interval_bitsequences_sum(const vvm& sc, const vi& l, const vi& r) {
	// verify : https://atcoder.jp/contests/abc262/tasks/abc262_h

	int n = sz(sc[0]), m = sz(l);

	// r_to_l[r] : 右端が r の半開区間のうちの左端の最大値（なければ -1）
	vi r_to_l(n + 1, -1);
	rep(j, m) chmax(r_to_l[r[j]], l[j]);
	rep(i, n) chmax(r_to_l[i + 1], r_to_l[i]);

	// dp_i[j] : s[0..i) のうち，s[0..j) までは条件を満たしており，
	//	s[j-1] = '1', s[j..i) = '0' であるもの全てのスコア和
	lazy_segtree<mint, op_n0bs, e_n0bs, mint, act_n0bs, comp_n0bs, id_n0bs> dp(n + 1);
	dp.set(0, 1);

	repi(r, 1, n) {
		// s[0..r) の接尾辞が "[ちょうどj-1個の0]1" となっているものの個数は dp[r-j] である．
		// '0' を連続して並べて良い左端の限界は r_to_l[r-1]+1 なので，その範囲で和をとる．
		int l = r_to_l[r - 1] + 1;
		dp.set(r, sc[1][r - 1] * dp.prod(l, r));
		dp.apply(0, r, sc[0][r - 1]);
	}

	return dp.prod(r_to_l[n] + 1, n + 1);
}


