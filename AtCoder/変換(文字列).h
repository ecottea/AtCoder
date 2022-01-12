#pragma once
#include "header.h"
// ■■■■■ 文字列の変換 ■■■■■


//【ランレングス符号】O(n)
/*
* s[0..n) をランレングス符号化し，結果を rlc に格納する．
* rlc[i] = {c, l} は前から i 番目の列が l 個の文字 c からなることを表す．
*/
template <class T>
void run_length_encodeing(vector<T>& a, vector<pair<T, int>>& rlc) {
	int n = sz(a);
	if (n == 0) {
		rlc = {};
		return;
	}

	rlc = { {a[0], 1} };

	// 今読んでいる文字の種類を記憶する．
	T c = a[0];

	repi(i, 1, n - 1) {
		// 記憶している文字と同じ文字の場合
		if (c == a[i]) {
			// 列の長さを増やす．
			rlc.rbegin()->second++;
		}
		// 記憶している文字と異なる文字の場合
		else {
			// 新しい文字を記憶しておく．
			c = a[i];

			// 新たな列を追加する．
			rlc.push_back({ c, 1 });
		}
	}
}


