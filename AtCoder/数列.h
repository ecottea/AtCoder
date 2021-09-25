#pragma once
#include "header.h"
// ¡¡¡¡¡ ”—ñ ¡¡¡¡¡



//y“™·”—ñ‚Ì˜azO(1)
/*
* ƒ°i=[i0..i1) (a + b i) ‚ð•Ô‚·D
*/
mint arithmetic_series(mint a, mint b, ll i0, ll i1) {
	// 2^(-1) ‚ª‘¶Ý‚µ‚È‚¢ê‡‚Å‚à–â‘è‚È‚¢‚æ‚¤‚É‹ô”‚ðæ‚É 2 ‚ÅŠ„‚Á‚Ä‚¨‚­D
	if ((i1 - i0) % 2 == 0) {
		return a * (i1 - i0) + b * (i1 + i0 - 1) * ((i1 - i0) / 2);
	}
	else {
		return a * (i1 - i0) + b * ((i1 + i0 - 1) / 2) * (i1 - i0);
	}
}


//y“™”ä”—ñ‚Ì˜az
/*
* geometric_series(r, n) : O(log(n))
*	ƒ°i=[0..n) r^i ‚ð•Ô‚·D
*
* geometric_series(r, i0, i1) : O(log(max(i0, i1)))
*	ƒ°i=[i0..i1) r^i ‚ð•Ô‚·D
*/
mint geometric_series(mint r, ll n) {
	// ‚¨‚È‚¶‚Ý‚ÌŒöŽ®‚Å‚Í r-1 ‚É‚æ‚éŠ„‚èŽZ‚ð•K—v‚Æ‚·‚é‚Ì‚Å‚Ü‚¸‚¢D

	// pow2 = r^(2^i), sumpow2 = 1 + r + ... + r^((2^i) - 1)
	mint res(0), pow2 = r, sumpow2 = 1;
	while (n > 0) {
		if (n & 1LL) res = res * pow2 + sumpow2;
		sumpow2 += sumpow2 * pow2;
		pow2 *= pow2;
		n /= 2;
	}
	return res;
}
mint geometric_series(mint r, ll i0, ll i1) {
	return geometric_series(r, i1) - geometric_series(r, i0);
}


//y“™·~“™”ä”—ñ‚Ì˜azO(log n)
/*
* arithmetic_geometric_series(r, n) : O(log n)
*	ƒ°i=[0..n) i r^i ‚ð•Ô‚·D
*
* arithmetic_geometric_series(a, b, r, i0, i1) : O(log(max(i0, i1)))
*	ƒ°i=[i0..i1) (a i + b) r^i ‚ð•Ô‚·D
*
* —˜—pFy“™”ä”—ñ‚Ì˜az
*/
mint arithmetic_geometric_series(mint r, ll n) {
	// ‚¨‚È‚¶‚Ý‚ÌŒöŽ®‚Å‚Í r-1 ‚É‚æ‚éŠ„‚èŽZ‚ð•K—v‚Æ‚·‚é‚Ì‚Å‚Ü‚¸‚¢D

	// pow2 = 2^i, powr = r^(2^i)
	// sumpowr1 = 0 * 1 + 1 * r + ... + ((2^i) - 1) * r^((2^i) - 1)
	// sumpowr0 = 1 + r + ... + r^((2^i) - 1)
	mint res1(0), res0(0), pow2 = 1, powr = r, sumpowr1 = 0, sumpowr0 = 1;
	while (n > 0) {
		if (n & 1LL) {
			res1 += pow2 * res0;
			res1 *= powr;
			res1 += sumpowr1;
			res0 *= powr;
			res0 += sumpowr0;
		}
		sumpowr1 *= 1 + powr;
		sumpowr1 += pow2 * powr * sumpowr0;
		sumpowr0 *= 1 + powr;
		powr *= powr;
		pow2 *= 2;

		n /= 2;
	}
	return res1;
}
mint arithmetic_geometric_series(mint a, mint b, mint r, ll i0, ll i1) {
	mint res = a * geometric_series(r, i0, i1);
	res += b * (arithmetic_geometric_series(r, i1) - arithmetic_geometric_series(r, i0));
	return res;
}


