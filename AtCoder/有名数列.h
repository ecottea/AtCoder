#pragma once
#include "header.h"
#include "行列.h"
#include "二項係数.h"
#include "FPS(mint).h"
// ■■■■■ 場合の数 ■■■■■


//【フィボナッチ数】O(log n)
/*
* n 番目のフィボナッチ数 fib[n] を返す（fib[0] = 0, fib[1] = 1 とする．）
*
*（行列累乗）
*
* 利用：【行列】
*/
mint fibonacci(ll n) {
	//【方法】
	// フィボナッチ数列 fib[] の満たす漸化式は
	//		fib[n] = fib[n - 1] + fib[n - 2]（fib[0] = 0, fib[1] = 1）
	// なる 3 項間線形漸化式だが，
	//		a0[n] = fib[n]    （a0[0] = 0）
	//		a1[n] = fib[n - 1]（a1[0] = 1）
	// とおくことにより，これを
	//		a0[n] = 1 * a0[n - 1] + 1 * a1[n - 1]
	//		a1[n] = 1 * a0[n - 1] + 0 * a1[n - 1]
	// なるベクトルの 2 項間線形漸化式に書き換えることができる．
	// これの一般項は容易に
	//		[a0[n]] = [1 1]^n [0]
	//		[a1[n]] = [1 0]   [1]
	// と表示することができるので，行列の累乗を計算する問題に帰着する．

	Matrix<mint> coef({ {1, 1}, {1, 0} });
	return coef.pow(n).v[0][1];
}


//【攪乱順列の数（モンモール数）】O(n)
/*
* i=[0..n] について，i 番目のモンモール数を m[i] に格納する．
* m[i] は長さ i の攪乱順列の数と解釈できる．
*/
void montmort_number(int n, vm& m) {
	// 参考 : https://ja.wikipedia.org/wiki/%E5%AE%8C%E5%85%A8%E9%A0%86%E5%88%97
	// verify : https://judge.yosupo.jp/problem/montmort_number_mod

	//【方法】
	// モンモール数は，2 項間漸化式
	//		m[n] = n m[n - 1] + (-1)^n
	// を満たす．

	m = vm(n + 1);

	m[0] = 1;
	repi(i, 1, n) m[i] = m[i - 1] * i + (i & 1 ? -1 : 1);
}


//【ベルヌーイ数】O(n log n)
/*
* i=[0..n) についてベルヌーイ数 B(i) を b[i] に格納する．
*
* 利用：【形式的冪級数】
*/
void bernoulli(int n, vm& b) {
	// 参考 : https://ja.wikipedia.org/wiki/%E3%83%99%E3%83%AB%E3%83%8C%E3%83%BC%E3%82%A4%E6%95%B0
	// verify : https://judge.yosupo.jp/problem/bernoulli_number

	//【方法】
	// ベルヌーイ数 B(n) はそもそも
	//		x / (exp(x) - 1) = Σn=[0..∞) B(n) / n! x^n
	// で定義される．

	FPS f = exp(FPS(vm({ 0, 1 })), n + 1);
	f <<= 1;
	f = f.inv(n);

	mint fac = 1;
	b = vm(n);
	b[0] = 1;
	repi(i, 1, n - 1) {
		fac *= i;
		b[i] = f[i] * fac;
	}
}


