#pragma once
#include "header.h"
#include "“ñ€ŒW”.h"
// ¡¡¡¡¡ ˆêd‚Ì˜a‚È‚Ç ¡¡¡¡¡


//y“™·”—ñ‚Ì˜azO(1)
/*
* ƒ°i=[i0..i1) (a + b i) ‚ğ•Ô‚·D
*/
mint arithmetic_series(mint a, mint b, ll i0, ll i1) {
	// 2^(-1) ‚ª‘¶İ‚µ‚È‚¢ê‡‚Å‚à–â‘è‚È‚¢‚æ‚¤‚É‹ô”‚ğæ‚É 2 ‚ÅŠ„‚Á‚Ä‚¨‚­D
	if ((i1 - i0) % 2 == 0) {
		return a * (i1 - i0) + b * (i1 + i0 - 1) * ((i1 - i0) / 2);
	}
	else {
		return a * (i1 - i0) + b * ((i1 + i0 - 1) / 2) * (i1 - i0);
	}
}


//y“™”ä”—ñ‚Ì˜az
/*
* geometric_series(r, n) : O(log n)
*	ƒ°i=[0..n) r^i ‚ğ•Ô‚·D
*
* geometric_series(r, i0, i1) : O(log max(i0, i1))
*	ƒ°i=[i0..i1) r^i ‚ğ•Ô‚·D
* 
*iƒ_ƒuƒŠƒ“ƒOj
*/
mint geometric_series(mint r, ll n) {
	// ‚¨‚È‚¶‚İ‚ÌŒö®‚Å‚Í r-1 ‚É‚æ‚éŠ„‚èZ‚ğ•K—v‚Æ‚·‚é‚Ì‚Å‚Ü‚¸‚¢D

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
*	ƒ°i=[0..n) i r^i ‚ğ•Ô‚·D
*
* arithmetic_geometric_series(a, b, r, i0, i1) : O(log max(i0, i1))
*	ƒ°i=[i0..i1) (a i + b) r^i ‚ğ•Ô‚·D
*
*iƒ_ƒuƒŠƒ“ƒOj
* 
* —˜—pFy“™”ä”—ñ‚Ì˜az
*/
mint arithmetic_geometric_series(mint r, ll n) {
	// ‚¨‚È‚¶‚İ‚ÌŒö®‚Å‚Í r-1 ‚É‚æ‚éŠ„‚èZ‚ğ•K—v‚Æ‚·‚é‚Ì‚Å‚Ü‚¸‚¢D

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


//y—İæ~“™”ä”—ñ‚Ì–³ŒÀ˜azO(d log d)
/*
* ƒ°i=[0..‡) i^d r^i ‚Ì’l‚ğ•Ô‚·D
*
* §–ñ : r != 1, d < 4 * 10^6
*
* —˜—pFyŠKæ‚Æ“ñ€ŒW”imint—˜—pjz
*/
mint powered_geometric_series(mint r, int d) {
	assert(r != 1);

	vm f(d + 1);
	repi(i, 0, d) f[i] = mint(i).pow(d);

	Factorial_mint fm(d + 1);
	vm g(d + 1);
	repi(i, 0, d) g[i] = (i & 1 ? -1 : 1) * fm.binomial(d + 1, i);

	f = convolution(f, g);

	mint res = 0;
	repir(i, d, 0) res = res * r + f[i];
	res /= mint(1 - r).pow(d + 1);

	return res;
}


//yXOR ‚Ì˜az
/*
* Xor_sum(vT a) : O(n log max(a))
*	a[0..n) ‚Å‰Šú‰»‚·‚éD
*
* ll sum(T x) : O(log max(a))
*	ƒ°i=[0..n) a[i] XOR x ‚Ì’l‚ğ•Ô‚·D
*/
template <class T> struct Xor_sum {
	// verify : https://atcoder.jp/contests/arc135/tasks/arc135_c

	int d;
	vvi cnt;

	// a[0..n) ‚Å‰Šú‰»‚·‚éD
	Xor_sum(const vector<T>& a) {
		T a_max = *max_element(all(a));
		if (a_max > 0) d = msbll((ll)a_max) + 1;
		else d = 0;

		cnt = vvi(d, vi(2));

		repe(v, a) {
			rep(j, d) {
				cnt[j][(v >> j) & 1]++;
			}
		}
	}

	// ƒ°i=[0..n) a[i] XOR x ‚Ì’l‚ğ•Ô‚·D
	ll sum(T x) {
		// ƒrƒbƒg–ˆ‚É“Æ—§‚ÉŠñ—^‚ğŒvZ‚µ˜a‚ğ‚Æ‚ê‚Î‚æ‚¢D
		ll res = 0;
		rep(j, d) {
			// (0,1), (1,0) ‚Ì‘g‚¾‚¯‚ªƒrƒbƒgˆÊ’u‚É‰‚¶‚½Šñ—^‚ğ‚à‚ÂD
			res += (ll)cnt[j][1 - ((x >> j) & 1)] << j;
		}
		return res;
	}
};


