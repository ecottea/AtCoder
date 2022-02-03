#pragma once
#include "header.h"
// ■■■■■ 文字列に対する汎用性のある前処理 ■■■■■


//【ランレングス圧縮】O(n)
/*
* s[0..n) をランレングス圧縮し，結果を rlc に格納する．
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


//【文字の次の位置】O(n k)
/*
* k = 26 種類の英小文字からなる文字列 s[0..n) について，
* s[i..n-1] で最初に文字 c が現れる位置を nxt[i][c] に格納する（無いなら n）
*/
void next_position(const string& s, vvi& nxt) {
	int n = sz(s);
	const int k = 26;

	// nxt[i][c] : s[i..n-1] で最初に文字 c が現れる位置（無いなら n）
	nxt = vvi(n + 1, vi(k, n));

	repir(i, n - 1, 0) {
		rep(c, k) {
			nxt[i][c] = nxt[i + 1][c];
		}
		nxt[i][s[i] - 'a'] = i;
	}
}


//【文字の前の位置】O(n k)
/*
* k = 26 種類の英小文字からなる文字列 s[0..n) について，
* s[0..i] で最後に文字 c が現れる位置を prv[i][c] に格納する（無いなら -1）
*/
void next_position(const string& s, vvi& prv) {
	int n = sz(s);
	const int k = 26;
	
	// prv[i + 1][c] : s[0..i] で最後に文字 c が現れる位置（無いなら -1）
	prv = vvi(n + 1, vi(k, -1));
	
	rep(i, n) {
		rep(c, k) {
			prv[i + 1][c] = prv[i][c];
		}
		prv[i + 1][s[i] - 'a'] = i;
	}
}


