#pragma once
#include "header.h"
#include "行列.h"
#include "二項係数.h"
#include "FPS(mint).h"
// ■■■■■ 有名数列 ■■■■■


//【フィボナッチ数】O(n)
/*
* i∈[0..n) について，i 番目のフィボナッチ数を fib[i] に格納する（fib[0] = 0, fib[1] = 1 とする．）
* 
*（DP）
*/
template <class T> void fibonacci(int n, vector<T>& fib) {
	// verify : https://atcoder.jp/contests/tenka1-2012-qualA/tasks/tenka1_2012_qualA_1

	fib.resize(n);
	fib[0] = 0;
	fib[1] = 1;

	repi(i, 2, n - 1) fib[i] = fib[i - 1] + fib[i - 2];
}


//【フィボナッチ数】O(log n)
/*
* n 番目のフィボナッチ数 fib[n] を返す（fib[0] = 0, fib[1] = 1 とする．）
*
*（行列累乗）
*
* 利用：【行列】
*/
mint fibonacci(ll n) {
	// verify : https://atcoder.jp/contests/math-and-algorithm/tasks/math_and_algorithm_at

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


//【k-ナッチ数】O(n)
/*
* i∈[0..n) について，i 番目の k-ナッチ数を seq[i] に格納する．
* 初項は seq[0..k-1) = 0, seq[k-1] = 1 とする．
*
*（累積和で高速化した DP）
*/
template <class T> void k_nacci_acc(int n, int k, vector<T>& seq) {
	// verify : https://atcoder.jp/contests/tdpc/tasks/tdpc_semiexp

	seq = vector<T>(n);
	if (n < k) return;
	seq[k - 1] = 1;

	// acc[i] : Σseq[0..i)
	vector<T> acc(n + 1);
	acc[k] = 1;

	repi(i, k, n - 1) {
		// seq[i] = Σseq[i-k..i) = acc[i] - acc[i-k]
		seq[i] = acc[i] - acc[i - k];

		// acc[i+1] = Σseq[i-k..i] = acc[i] + seq[i]
		acc[i + 1] = acc[i] + seq[i];
	}
}


//【k-ナッチ数】O(n + k)
/*
* i∈[0..n) について，i 番目の k-ナッチ数を seq[i] に格納する．
* 初項は seq[0..k-1) = 0, seq[k-1] = 1 とする．
*
*（いもす法で高速化した DP）
*/
template <class T> void k_nacci_imos(int n, int k, vector<T>& seq) {
	// verify : https://atcoder.jp/contests/tdpc/tasks/tdpc_semiexp

	seq = vector<T>(n + k + 1);
	seq[k - 1] = 1;

	// 初項からの寄与の種を蒔く．
	seq[k] += seq[k - 1];
	seq[k + k] -= seq[k - 1];

	repi(i, k, n - 1) {
		// seq[i] からの寄与の種を蒔く．
		seq[i + 1] += seq[i];
		seq[i + k + 1] -= seq[i];

		// 累積和をとる．
		seq[i + 1] += seq[i];
	}

	seq.resize(n);
}


//【一般化デラノイ数】O(h w)
/*
* 初項を seq[0][0] = 1 とし，任意の (x, y) ∈ [0..h)*[0..w) \ {(0, 0)} について
*		seq[x][y] = Σseq(x-s..x](y-t..y] - seq[x][y] （範囲外の値は 0 とみなす）
* なる漸化式で定められる数列を seq[x][y] に格納する．
*
*（二次元累積和で高速化した格子 DP）
*/
template <class T> void delannoy_number_acc(int h, int w, int s, int t, vector<vector<T>>& seq) {
	seq = vector<vector<T>>(h, vector<T>(w));
	seq[0][0] = 1;

	// acc[x][y] : Σseq[0..x][0..y]
	vector<vector<T>> acc(h, vector<T>(w));
	acc[0][0] = 1;

	rep(x, h) rep(y, w) {
		// 初項は更新しない．
		if (x == 0 && y == 0) continue;

		// tmp : acc[x-1][y] + acc[x][y-1] - acc[x-1][y-1]
		T tmp = 0;
		if (x - 1 >= 0) tmp += acc[x - 1][y];
		if (y - 1 >= 0) tmp += acc[x][y - 1];
		if (x - 1 >= 0 && y - 1 >= 0) tmp -= acc[x - 1][y - 1];

		// seq[x][y]
		// = Σseq(x-s..x](y-t..y] - seq[x][y]
		// = acc[x-1][y] + acc[x][y-1] - acc[x-1][y-1]
		//		- acc[x-s-1][y] - acc[x][y-t-1] + acc[x-s-1][y-t-1] 
		seq[x][y] += tmp;
		if (x - s - 1 >= 0) seq[x][y] -= acc[x - s - 1][y];
		if (y - t - 1 >= 0) seq[x][y] -= acc[x][y - t - 1];
		if (x - s - 1 >= 0 && y - t - 1 >= 0) seq[x][y] += acc[x - s - 1][y - t - 1];

		// acc[x][y]
		// = Σseq[0..x][0..y]
		// = seq[x][y] + acc[x-1][y] + acc[x][y-1] - acc[x-1][y-1]
		acc[x][y] = seq[x][y] + tmp;
	}
}


//【一般化デラノイ数】O(h w)
/*
* 初項を seq[0][0] = 1 とし，任意の (x, y) ∈ [0..h)*[0..w) \ {(0, 0)} について
*		seq[x][y] = Σseq(x-s..x](y-t..y] - seq[x][y] （範囲外の値は 0 とみなす）
* なる漸化式で定められる数列を seq[x][y] に格納する．
*
*（二次元いもす法で高速化した格子 DP）
*/
template <class T> void delannoy_number_imos(int h, int w, int s, int t, vector<vector<T>>& seq) {
	seq = vector<vector<T>>(h, vector<T>(w));
	seq[0][0] = 1;

	rep(x, h) rep(y, w) {
		T v = seq[x][y];

		// seq[x][y] からの寄与の種を蒔く．
		if (x + s + 1 < h) seq[x + s + 1][y] -= v;
		if (y + t + 1 < w) seq[x][y + t + 1] -= v;
		if (x + s + 1 < h && y + t + 1 < w) seq[x + s + 1][y + t + 1] += v;

		if (x != 0 || y != 0) v *= 2;

		// 累積和をとる．
		if (x + 1 < h) seq[x + 1][y] += v;
		if (y + 1 < w) seq[x][y + 1] += v;
		if (x + 1 < h && y + 1 < w) seq[x + 1][y + 1] -= v;
	}
}


//【ベルヌーイ数】O(n log n)
/*
* i=[0..n) についてベルヌーイ数 B(i) を b[i] に格納する．
*
* 制約：fm は (2(n+1))! まで計算可能であること
*
* 利用：【形式的冪級数（mod 998244353）】,【指数関数】,【階乗など（法が大きな素数）】
*/
void bernoulli(int n, vm& b, const Factorial_mint& fm) {
	// 参考 : https://ja.wikipedia.org/wiki/%E3%83%99%E3%83%AB%E3%83%8C%E3%83%BC%E3%82%A4%E6%95%B0
	// verify : https://judge.yosupo.jp/problem/bernoulli_number

	//【方法】
	// ベルヌーイ数 B(n) はそもそも
	//		x / (exp(x) - 1) = Σn=[0..∞) B(n) / n! x^n
	// で定義される．

	MFPS f = exp(MFPS(vm({ 0, 1 })), n + 1, fm);
	f <<= 1;
	f = f.inv(n);

	b.resize(n);
	rep(i, n) b[i] = f[i] * fm.factorial(i);
}


