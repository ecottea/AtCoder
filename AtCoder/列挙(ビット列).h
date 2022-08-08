#pragma once
#include "header.h"
// ■■■■■ 列挙（ビット列） ■■■■■


//【1 の連続しないビット列の列挙】O(fibonacci(n) n) = O(1.6^n n)
/*
* 長さ n のビット列のうち 1 が連続しないもの全てを seqs に格納する．
*/
void enumerate_noncontinuous_bitsequences(int n, vl& seqs) {
	ll b = 0;
	seqs.clear();

	// len : 残り長さ, d : 直前の桁の数
	function<void(int, int)> rf = [&](int len, int d) {
		// 残りの長さが 0 の場合
		if (len == 0) {
			seqs.push_back(b);
			return;
		}

		// 直前の桁が 0 である場合は 1 を使える．
		if (d == 0) {
			b = b * 2 + 1;
			rf(len - 1, 1);
			b = (b - 1) / 2;
		}

		// 0 を使う．
		b *= 2;
		rf(len - 1, 0);
		b /= 2;

		return;
	};

	rf(n, 0);
}


