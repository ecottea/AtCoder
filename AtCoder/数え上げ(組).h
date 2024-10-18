#pragma once
#include "header.h"
#include "アダマール変換.h"
#include "整除.h"
#include "数論(一括).h"
// ■■■■■ 組（数え上げ） ■■■■■


//【組の数え上げ（XOR 毎）】O(n + m + A log A)（A = max(a, b)）
/*
* 与えられた a[0..n), b[0..m) に対し，各 x∈[0..2^K) について
* a[i] XOR b[j] = x となる組 (i, j) の個数を格納したリストを返す．
*
* 利用：【XOR 畳込み】
*/
vl count_pair_by_XOR_all(int K, const vi& a, const vi& b) {
	// verify : https://yukicoder.me/problems/no/1142

	vl cnt_a(1LL << K), cnt_b(1LL << K);
	repe(x, a) cnt_a[x]++;
	repe(x, b) cnt_b[x]++;

	auto res = xor_convolution(cnt_a, cnt_b);

	return res;
}


//【互いに素な組の数え上げ（要素毎）】O(?)
/*
* 与えられた正整数列 a[0..n) に対し，各 i∈[0..n) について，
* a[i] と互いに素な a[j] (j∈[0..n)) の個数を格納したリストを返す．
*
* 利用：【約数倍数変換】,【素因数分解（複数）】
*/
vi count_coprime_pair_all(const vi& a) {
	// 参考 : https://x.com/torii_kyopro/status/1841911315125961192

	int n = sz(a);

	int m = *max_element(all(a));

	// cnt[j] : a[0..n) 内の j の個数
	vi cnt(m + 1);
	rep(i, n) cnt[a[i]]++;

	Div_mul_transform<int> D(m);

	// cnt_mul[j] : a[0..n) 内の j の倍数の個数
	vi cnt_mul(cnt);
	D.multiple_zeta(cnt_mul);

	Osa_k O(m);

	// ans[j] : a[0..n) 内の j と互いに素な数の個数
	vi ans(m + 1);

	repi(j, 1, m) {
		if (cnt[j] == 0) continue;

		// j の素因数のリスト（重複除く）
		auto ps = O.unique_prime_factors(j);
		int K = sz(ps);

		// 包除原理で答えを求める．
		repb(set, K) {
			int sgn = popcount(set) & 1 ? -1 : 1;

			int v = 1;
			repis(k, set) v *= ps[k];

			ans[j] += sgn * cnt_mul[v];
		}
	}

	vi res(n);
	rep(i, n) res[i] = ans[a[i]];

	return res;
}


