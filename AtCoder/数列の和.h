#pragma once
#include "header.h"
#include "“ñ€ŒW”.h"
// ¡¡¡¡¡ ”—ñ‚Ì˜a‚È‚Ç ¡¡¡¡¡


//y“™·”—ñ‚Ì˜azO(1)
/*
* ƒ°i¸[i0..i1) (a i + b) ‚ğ•Ô‚·D
*/
template<class T>
T arithmetic_series(T a, T b, ll i0, ll i1) {
	// verify : https://atcoder.jp/contests/arc035/tasks/arc035_b

	if (i0 >= i1) return 0;

	// 2^(-1) ‚ª‘¶İ‚µ‚È‚¢ê‡‚Å‚à–â‘è‚È‚¢‚æ‚¤‚É‹ô”‚ğæ‚É 2 ‚ÅŠ„‚Á‚Ä‚¨‚­D
	if ((i1 - i0) % 2 == 0) {
		return a * (i1 + i0 - 1) * ((i1 - i0) / 2) + b * (i1 - i0);
	}
	else {
		return a * ((i1 + i0 - 1) / 2) * (i1 - i0) + b * (i1 - i0);
	}
}


//y“™·”—ñ‚ÌÏz
/*
* Œö·‚ª 0 ‚Å‚È‚¢“™·”—ñ‚ÌÏ‚ÍC
*		ƒ®i¸[i0..i1) (a + b i)
*		= b^(i1-i0) ƒ®i¸[i0..i1) (a/b + i)
*		= b^(i1-i0) (a/b + i1 - 1)! / (a/b + i0 - 1)!
* ‚Æ•ÏŒ`‚µ‚ÄŠKæ‚ÌŒvZ‚É‹A’…‚Å‚«‚éD
* 
* verify : https://atcoder.jp/contests/m-solutions2019/tasks/m_solutions2019_e
*/


//y“™”ä”—ñ‚Ì˜azO(log n)
/*
* geometric_series(r, n) : O(log n)
*	ƒ°i¸[0..n) r^i ‚ğ•Ô‚·D
*
* geometric_series(r, i0, i1) : O(log max(i0, i1))
*	ƒ°i¸[i0..i1) r^i ‚ğ•Ô‚·D
* 
*iƒ_ƒuƒŠƒ“ƒOj
*/
mint geometric_series(mint r, ll n) {
	// verify : https://atcoder.jp/contests/arc050/tasks/arc050_c

	//y•û–@z
	// ‚¨‚È‚¶‚İ‚ÌŒö®‚Å‚Í r-1 ‚É‚æ‚éŠ„‚èZ‚ğ•K—v‚Æ‚µ‚Ä‚Ü‚¸‚¢‚Ì‚Åƒ_ƒuƒŠƒ“ƒO‚·‚éD

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


//y“™·~“™”äŒ^”—ñ‚Ì˜azO(log n)
/*
* arithmetic_geometric_series(r, n) : O(log n)
*	ƒ°i¸[0..n) i r^i ‚ğ•Ô‚·D
*
* arithmetic_geometric_series(a, b, r, i0, i1) : O(log max(i0, i1))
*	ƒ°i¸[i0..i1) (a i + b) r^i ‚ğ•Ô‚·D
*
*iƒ_ƒuƒŠƒ“ƒOj
* 
* —˜—pFy“™”ä”—ñ‚Ì˜az
*/
mint arithmetic_geometric_series(mint r, ll n) {
	//y•û–@z
	// ‚¨‚È‚¶‚İ‚ÌŒö®‚Å‚Í r-1 ‚É‚æ‚éŠ„‚èZ‚ğ•K—v‚Æ‚µ‚Ä‚Ü‚¸‚¢‚Ì‚Åƒ_ƒuƒŠƒ“ƒO‚·‚éD

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


//y—İæŒ^”—ñ‚Ì˜azO(k log k)
/*
* ƒ°i¸[0..n) i^k ‚Í n ‚Ì k+1 Ÿ®‚É‚È‚é‚Ì‚ÅCƒ‰ƒOƒ‰ƒ“ƒWƒ…•âŠÔ‚Å’l‚ğ‹‚ß‚ç‚ê‚éD
* 
* verify : https://codeforces.com/contest/622/problem/F
*/


//y—İæ~“™”äŒ^”—ñ‚Ì–³ŒÀ˜azO(d log d)
/*
* ƒ°i¸[0..‡) i^d r^i ‚ğ•Ô‚·D
*
* §–ñ : r != 1
* 
* —˜—pFyŠKæ‚È‚Çi–@‚ª‘å‚«‚È‘f”jz
*/
mint powered_geometric_series(mint r, int d) {
	// Ql : https://kyopro-friends.hatenablog.com/entry/2020/03/11/073122
	// verify : https://judge.yosupo.jp/problem/sum_of_exponential_times_polynomial_limit

	vm acc(d + 2), pow_r(d + 2);
	pow_r[0] = 1;
	rep(i, d + 1) {
		acc[i + 1] = acc[i] + pow_r[i] * mint(i).pow(d);
		pow_r[i + 1] = pow_r[i] * r;
	}

	Factorial_mint fm(d + 1);

	mint res = 0;
	repi(i, 0, d) {
		res += ((d - i) % 2 == 0 ? 1 : -1) * pow_r[d - i]
			* fm.bin(d + 1, i + 1) * acc[i + 1];
	}

	res /= mint(1 - r).pow(d + 1);

	return res;
}


