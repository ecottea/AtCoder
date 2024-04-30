#pragma once
#include "header.h"
// ■■■■■ アダマール変換，XOR 畳込み ■■■■■


//【アダマール変換】: O(2^n n)
/*
* a[0..2^n) をアダマール変換したものに上書きする．
*/
template <class T>
void hadamard(vector<T>& a) {
	// verify : https://judge.yosupo.jp/problem/bitwise_xor_convolution

	// 具体例：
	//	A[0] = a[0] + a[1] + a[2] + a[3] + a[4] + a[5] + a[6] + a[7] + ...
	//	A[1] = a[0] - a[1] + a[2] - a[3] + a[4] - a[5] + a[6] - a[7] + ...
	//	A[2] = a[0] + a[1] - a[2] - a[3] + a[4] + a[5] - a[6] - a[7] + ...
	//	A[3] = a[0] - a[1] - a[2] + a[3] + a[4] - a[5] - a[6] + a[7] + ...
	//	A[4] = a[0] + a[1] + a[2] + a[3] - a[4] - a[5] - a[6] - a[7] + ...
	//	A[5] = a[0] - a[1] + a[2] - a[3] - a[4] + a[5] - a[6] + a[7] + ...
	//	A[6] = a[0] + a[1] - a[2] - a[3] - a[4] - a[5] + a[6] + a[7] + ...
	//	A[7] = a[0] - a[1] - a[2] + a[3] - a[4] + a[5] + a[6] - a[7] + ...
	
	int n = msb(sz(a));

	rep(i, n) repb(set, n) {
		if (!(set & (1 << i))) {
			T x = a[set];
			T y = a[set | (1 << i)];

			a[set] = x + y;
			a[set + (1 << i)] = x - y;
		}
	}
}


//【逆アダマール変換】: O(2^n n)
/*
* A[0..2^n) を逆アダマール変換したものに上書きする．
* 
* 制約：A の要素は 2 で割れる．
*/
template <class T>
void hadamard_inv(vector<T>& A) {
	// verify : https://judge.yosupo.jp/problem/bitwise_xor_convolution

	int n = msb(sz(A));

	rep(i, n) repb(set, n) {
		if (!(set & (1 << i))) {
			T x = A[set];
			T y = A[set | (1 << i)];

			// オーバーフローの危険があるので都度 /2 しないといけない．
			A[set] = (x + y) >> 1;
			A[set + (1 << i)] = (x - y) >> 1;
		}
	}
}


//【逆アダマール変換（mint）】: O(2^n n + log(mod))
/*
* A[0..2^n) を逆アダマール変換したものに上書きする．
* 
* 制約：mint の法は 2 の倍数でない
*
* 利用：【アダマール変換】
*/
void hadamard_inv(vm& A) {
	// verify : https://atcoder.jp/contests/abc265/tasks/abc265_h

	hadamard(A);

	// まとめて商をとらないと log(mod) 倍遅くなる．
	mint inv = mint(sz(A)).inv();
	rep(i, sz(A)) A[i] *= inv;
}


//【XOR 畳込み】O(2^n n)
/*
* 与えられた a[0..2^n), b[0..2^n) に対して
*       c[set] = Σ(set1 XOR set2 = set) a[set1] b[set2]
* なる c[0..2^n) を返す．
*
* 利用：【アダマール変換】,【逆アダマール変換】
*/
template <class T>
vector<T> xor_convolution(vector<T> a, vector<T> b) {
	// 参考 : https://kazuma8128.hatenablog.com/entry/2018/05/31/144519
	// verify : https://judge.yosupo.jp/problem/bitwise_xor_convolution

	int n = msb(sz(a));

	hadamard(a);
	hadamard(b);

	repb(set, n) a[set] *= b[set];

	hadamard_inv(a);

	return a;
}


//【高速アダマール変換と行列のクロネッカー積】
/*
* a[0..2^n) を高速アダマール変換して A[0..2^n) にする線形変換の表現行列は，
* アダマール行列 [1  1] の d 個のクロネッカー積に等しい．
*               [1 -1]
* 
* A[0..2^n) を逆高速アダマール変換して a[0..2^n) にする線形変換の表現行列は，
* アダマール行列の逆行列 [1/2  1/2] の d 個のクロネッカー積に等しい．
*                      [1/2 -1/2]
* 
* verify : https://atcoder.jp/contests/abc212/tasks/abc212_h
*/


