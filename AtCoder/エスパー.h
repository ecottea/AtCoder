#pragma once
#include "header.h"
// ■■■■■ エスパー ■■■■■


//【線形漸化式の発見】O(n^2)
/*
* 与えられた数列 a[0..n) に対し，以下の等式を満たす c[0..m) で m を最小とするものを返す：
*		a[i] = Σj∈[0..m) c[j] a[i-1-j]  (∀i∈[m..n))
*
* 制約 : mint::mod は素数
*/
vm berlekamp_massey(const vm& a) {
	// 参考 : https://en.wikipedia.org/wiki/Berlekamp%E2%80%93Massey_algorithm
	// verify : https://judge.yosupo.jp/problem/find_linear_recurrence

	vm S(a), C{ 1 }, B{ 1 };
	int N = sz(a), m = 1; mint b = 1;

	rep(n, N) {
		mint d = 0;
		rep(i, sz(C)) d += C[i] * S[n - i];

		if (d == 0) {
			m++;
		}
		else if (2 * (sz(C) - 1) <= n) {
			vm T(C);

			mint coef = d * b.inv();
			C.resize(max(sz(C), sz(B) + m));
			rep(j, sz(B)) C[j + m] -= coef * B[j];

			B = T;
			b = d;
			m = 1;
		}
		else {
			mint coef = d * b.inv();
			C.resize(max(sz(C), sz(B) + m));
			rep(j, sz(B)) C[j + m] -= coef * B[j];

			m++;
		}
	}

	C.erase(C.begin());
	rep(i, sz(C)) C[i] *= -1;

	return C;
}


