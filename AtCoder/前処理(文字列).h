#pragma once
#include "header.h"
// ■■■■■ 文字列に対する汎用性のある前処理 ■■■■■


//【ランレングス符号（文字列）】O(n)
/*
* 文字列 s[0..n) をランレングス符号化し，結果を格納したリスト cls を返す．
* cls[i] = {c, l} は前から i 番目の列が l 個の文字 c からなることを表す．
*/
vector<pair<char, int>> run_length_encodeing(const string& s) {
	// verify : https://atcoder.jp/contests/abc124/tasks/abc124_d

	int n = sz(s);
	vector<pair<char, int>> cls;

	if (n == 0) return cls;

	cls.emplace_back(s[0], 1);

	// 今読んでいる文字の種類を記憶する．
	char c = s[0];

	repi(i, 1, n - 1) {
		// 記憶している文字と同じ文字の場合
		if (s[i] == c) {
			// 列の長さを増やす．
			cls.back().second++;
		}
		// 記憶している文字と異なる文字の場合
		else {
			// 新しい文字を記憶しておく．
			c = s[i];

			// 新たな列を追加する．
			cls.emplace_back(c, 1);
		}
	}

	return cls;
}


//【ランレングス符号】O(n)
/*
* a[0..n) をランレングス符号化し，結果を格納したリスト cls を返す．
* cls[i] = {c, l} は前から i 番目の列が l 個の文字 c からなることを表す．
*/
template <class T>
vector<pair<T, int>> run_length_encodeing(const vector<T>& a) {
	// verify : https://atcoder.jp/contests/arc024/tasks/arc024_2

	int n = sz(a);
	vector<pair<T, int>> cls;

	if (n == 0) return cls;

	cls.emplace_back(a[0], 1);

	// 今読んでいる文字の種類を記憶する．
	T c = a[0];

	repi(i, 1, n - 1) {
		// 記憶している文字と同じ文字の場合
		if (c == a[i]) {
			// 列の長さを増やす．
			cls.back().second++;
		}
		// 記憶している文字と異なる文字の場合
		else {
			// 新しい文字を記憶しておく．
			c = a[i];

			// 新たな列を追加する．
			cls.emplace_back(c, 1);
		}
	}

	return cls;
}


//【ランレングス符号（区切り位置）】O(n)
/*
* a[0..n) をランレングス符号化する．
* 前から i 番目の連が a[x[i]..x[i+1]) で，その文字が c[i] であったことを格納する．
*/
template <class T>
void run_length_encodeing(const vector<T>& a, vector<T>& c, vi& x) {
	// verify : https://atcoder.jp/contests/arc092/tasks/arc092_c

	int n = sz(a);
	c.clear(); x = vi{ 0 };

	if (n == 0) return;

	c.emplace_back(a[0]);
	repi(i, 1, n - 1) {
		// 最後に読んだ文字と異なる文字の場合
		if (a[i] != c.back()) {
			x.emplace_back(i);
			c.emplace_back(a[i]);
		}
	}
	x.emplace_back(n);
}


//【文字の次の位置】O(n k)
/*
* k = 26 種類の英小文字からなる文字列 s[0..n) について，
* s[i..n-1] で最初に文字 c が現れる位置を nxt[i][c] に格納する（無いなら n）
*/
void next_position(const string& s, vvi& nxt) {
	// verify : https://atcoder.jp/contests/abc138/tasks/abc138_e

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
void prev_position(const string& s, vvi& prv) {
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


//【異なる文字の次の位置】O(n k)
/*
* s[0..n) で，j > i かつ s[j] != s[i] なる最小の j を nxt[i] に格納する（なければ n）
*/
void next_different_position(const string& s, vi& nxt) {
	int n = sz(s);
	nxt.resize(n);

	char c = s[n - 1]; // 走査中の文字
	int pos = n; // 走査中の文字以外が最後に現れた位置

	// 後ろから走査していく
	repir(i, n - 1, 0) {
		if (s[i] == c) nxt[i] = pos;
		else {
			nxt[i] = pos = i + 1;
			c = s[i];
		}
	}
}


