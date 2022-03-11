#pragma once
#include "header.h"
// ■■■■■ 文字列に対する汎用性のある前処理 ■■■■■


//【ランレングス圧縮（文字列）】O(n)
/*
* 文字列 s[0..n) をランレングス圧縮し，結果を cls に格納する．
* cls[i] = {c, l} は前から i 番目の列が l 個の文字 c からなることを表す．
*/
void run_length_encodeing(const string& s, vector<pair<char, int>>& cls) {
	// verify : https://atcoder.jp/contests/abc124/tasks/abc124_d

	int n = sz(s);
	cls.clear();

	if (n == 0) return;

	cls.push_back({ s[0], 1 });

	// 今読んでいる文字の種類を記憶する．
	char c = s[0];

	repi(i, 1, n - 1) {
		// 記憶している文字と同じ文字の場合
		if (s[i] == c) {
			// 列の長さを増やす．
			(*cls.rbegin()).second++;
		}
		// 記憶している文字と異なる文字の場合
		else {
			// 新しい文字を記憶しておく．
			c = s[i];

			// 新たな列を追加する．
			cls.push_back({ c, 1 });
		}
	}
}


//【ランレングス圧縮】O(n)
/*
* a[0..n) をランレングス圧縮し，結果を cls に格納する．
* cls[i] = {c, l} は前から i 番目の列が l 個の文字 c からなることを表す．
*/
template <class T>
void run_length_encodeing(const vector<T>& a, vector<pair<T, int>>& cls) {
	int n = sz(a);
	cls.clear();

	if (n == 0) return;

	cls.push_back( {a[0], 1} );

	// 今読んでいる文字の種類を記憶する．
	T c = a[0];

	repi(i, 1, n - 1) {
		// 記憶している文字と同じ文字の場合
		if (c == a[i]) {
			// 列の長さを増やす．
			cls.rbegin()->second++;
		}
		// 記憶している文字と異なる文字の場合
		else {
			// 新しい文字を記憶しておく．
			c = a[i];

			// 新たな列を追加する．
			cls.push_back({ c, 1 });
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


//【文字の最初の位置】O(n + k)
/*
* k = 26 種類の英小文字からなる文字列 s[0..n) について，
* s で最初に文字 c が現れる位置を pos[c] に格納する（無いなら n）
*/
void first_position(const string& s, vi& pos) {
	int n = sz(s);
	const int k = 26;

	// pos[c] : s で最初に文字 c が現れる位置（無いなら n）
	pos = vi(k, n);

	repir(i, n - 1, 0) {
		pos[s[i] - 'a'] = i;
	}
}


//【文字の最後の位置】O(n + k)
/*
* k = 26 種類の英小文字からなる文字列 s[0..n) について，
* s で最後に文字 c が現れる位置を pos[c] に格納する（無いなら -1）
*/
void last_position(const string& s, vi& pos) {
	int n = sz(s);
	const int k = 26;

	// pos[c] : s で最初に文字 c が現れる位置（無いなら -1）
	pos = vi(k, -1);

	rep(i, n) {
		pos[s[i] - 'a'] = i;
	}
}


