#pragma once
#include "header.h"
#include "“ñ€ŒW”.h"
// ¡¡¡¡¡ ”—ñ ¡¡¡¡¡


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
* §–ñ : r ‚ 1, d < 4 ~ 10^6
*
* —˜—pFyŠKæ‚Æ“ñ€ŒW”imint—˜—pjz
*/
mint powered_geometric_series(mint r, int d) {
	assert(r != 1);

	vm f(d + 1);
	repi(i, 0, d) f[i] = mint(i).pow(d);

	factorial_mint fm(d + 1);
	vm g(d + 1);
	repi(i, 0, d) g[i] = (i & 1 ? -1 : 1) * fm.binomial(d + 1, i);

	f = convolution(f, g);

	mint res = 0;
	repir(i, d, 0) res = res * r + f[i];
	res /= mint(1 - r).pow(d + 1LL);

	return res;
}


//yƒtƒƒCƒh‚ÌzŠÂŒŸo–@zO(nc + c)
/*
* a[i+1] = f(a[i]), a[0] = a0 ‚È‚é”—ñ‚É‚Â‚¢‚ÄCa[0] ‚©‚çn‚Ü‚é”ñüŠú—ñ‚Ì’·‚³ nc ‚Æ
* a[nc] ‚©‚çn‚Ü‚éüŠú—ñ‚Ì’·‚³ c ‚Ì‘g {nc, c} ‚ğ•Ô‚·D
*/
template <class T> pii floyds_cycle_finding(function<T(T)>& f, T a0) {
	// Ql : https://ja.wikipedia.org/wiki/%E3%83%95%E3%83%AD%E3%82%A4%E3%83%89%E3%81%AE%E5%BE%AA%E7%92%B0%E6%A4%9C%E5%87%BA%E6%B3%95

	T x = a0, y = a0;
	int m = 0;
	do {
		x = f(x);
		y = f(f(y));
		m++;
	} while (x != y);

	x = a0;
	int nc = 0;
	while (x != y) {
		x = f(x);
		y = f(y);
		nc++;
	}

	int c = 0;
	do {
		x = f(x);
		y = f(f(y));
		c++;
	} while (x != y);

	return make_pair(nc, c);
}


