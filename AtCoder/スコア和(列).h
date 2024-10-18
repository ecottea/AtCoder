#pragma once
#include "header.h"
// ■■■■■ スコア和（列） ■■■■■


//【連続自然数の popcount の和】O(log N)
/*
* Σi∈[0..N) popcount(i) を返す．
*/
template <class T>
T continuous_popcount_sum(ll N) {
	// verify : https://yukicoder.me/problems/no/2939

	//【方法】
	// ビット毎に独立に寄与を計算する．
	// 第 k ビットは ['0'×k, '1'×k] を 1 周期として変化することを利用できる．

	T res = T(0);

	repi(k, 0, msb(N)) {
		// w : 周期
		ll w = 1LL << (k + 1);
		ll hw = w / 2;

		ll q = N / w;
		ll r = N % w;

		res += T(q) * hw;
		res += T(max(r - hw, 0LL));
	}

	return res;
}


//【連続自然数の XOR】O(1)
/*
* XOR[0..n) を返す．
*/
template <class T>
T continuous_XOR(T n) {
	// verify : https://atcoder.jp/contests/abc121/tasks/abc121_d

	//【方法】
	// s[i] := XOR[0..i) とおく．
	//		(2 k) XOR (2 k + 1) = 1
	// に注意すると，s[i] が周期 4 をもつことが分かり，
	//		s[4 k]     = 0
	//		s[4 k + 1] = 4 k
	//		s[4 k + 2] = 1
	//		s[4 k + 3] = 4 k + 3
	// と簡単に表すことができる．

	if (n <= 0) return 0;

	T res = -1;
	if (n % 4 == 0) res = 0;
	else if (n % 4 == 1) res = n - 1;
	else if (n % 4 == 2) res = 1;
	else if (n % 4 == 3) res = n;

	return res;
}


//【一次式の総 XOR】O((log(an+b))^2)
/*
* XOR_i∈[0..n) (ai+b) を返す．
*
* 利用：【一次式の切り捨て和】
*/
template <class T>
T arithmetic_XOR(T n, T a, T b) {
	// verify : https://mojacoder.app/users/Tonegawac/problems/linear-xor

	//【方法】
	// ビット毎に独立に寄与を計算する．
	// 第 k ビットが 1 かどうかは，Σi∈[0..n) (ai+b)/2^k の偶奇から判断できる．

	int K = msb(a * (n - 1) + b);

	T res = 0;
	repi(k, 0, K) {
		ll val = floor_sum_large(n, 1LL << k, a, b);
		if (val & 1LL) res |= T(1) << k;
	}

	return res;
}


