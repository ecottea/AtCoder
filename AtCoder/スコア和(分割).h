#pragma once
#include "header.h"
#include "ƒrƒbƒg‘S’Tõ.h"
// ¡¡¡¡¡ ƒXƒRƒA˜aiW‡‚Ì•ªŠ„j ¡¡¡¡¡


//y•ªŠ„‚ÌÏƒXƒRƒA‚Ì˜azO(3^n)
/*
* [0..n) ‚Ì•ªŠ„ ƒÎ = ™k S_k ‚É‘Î‚·‚éƒXƒRƒA‚ª ƒ®k a[S_k] ‚Å—^‚¦‚ç‚ê‚é‚Æ‚·‚éD
* Še set¼[0..n) ‚É‚Â‚¢‚ÄC‘S‚Ä‚Ì•ªŠ„‚ÌƒXƒRƒA‚Ì˜a‚ğŠi”[‚µ‚½ƒŠƒXƒg‚ğ•Ô‚·D
*/
vm set_partition_mul_score_sum(const vm& a) {
	// verify : https://atcoder.jp/contests/abc236/tasks/abc236_h

	//y”õlz
	// ”ñŒğ˜aô‚İ‚ğg‚¦‚Î O(n^2 2^n) ‚Ü‚Å‚‘¬‰»‚Å‚«‚é‚ç‚µ‚¢D

	int N = sz(a);

	vm dp(N);
	dp[0] = 1;

	// SoS bit DP
	rep(set, N) {
		if (set == 0) continue;

		// set1 : set ‚©‚ç“Á’è‚Ì—v‘f x ‚ğæ‚èœ‚¢‚½W‡
		int set1 = (set - 1) & set;

		// sub ¼ set1 ‚ğ‘S’Tõ‚·‚éiset - sub ‚ª x ‚ğŠÜ‚Şj
		for (int sub = set1, tmp = 1; tmp > 0; tmp = sub, sub = (sub - 1) & set1) {
			dp[set] += dp[sub] * a[set - sub];
		}
	}

	return dp;
}


