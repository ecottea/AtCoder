#pragma once
#include "header.h"
// ¡¡¡¡¡ Šiqƒf[ƒ^‚ÌŒü‚«‚Ì•ÏŠ· ¡¡¡¡¡


//y¶‰E”½“]zO(h w)
/*
* a[0..h)[0..w) ‚ğ¶‰E”½“]‚µ‚½‚à‚Ì‚ğ•Ô‚·D
*/
template <class T> vector<vector<T>> flip_y(const vector<vector<T>>& a) {
	int h = sz(a), w = sz(a[0]);

	vector<vector<T>> b(h, vector<T>(w));
	rep(i, h) rep(j, w) b[i][j] = a[i][w - 1 - j];

	return b;
}


//yã‰º”½“]zO(h w)
/*
* a[0..h)[0..w) ‚ğã‰º”½“]‚µ‚½‚à‚Ì‚ğ•Ô‚·D
*/
template <class T> vector<vector<T>> flip_x(const vector<vector<T>>& a) {
	// verify : https://atcoder.jp/contests/abc260/tasks/abc260_g
	
	int h = sz(a), w = sz(a[0]);

	vector<vector<T>> b(h, vector<T>(w));
	rep(i, h) rep(j, w) b[i][j] = a[h - 1 - i][j];

	return b;
}


//y“]’uzO(h w)
/*
* a[0..h)[0..w) ‚ğ“]’u‚µ‚½‚à‚Ì‚ğ•Ô‚·D
*/
template <class T> vector<vector<T>> transpose(const vector<vector<T>>& a) {
	// verify : https://yukicoder.me/problems/no/1974

	int h = sz(a), w = sz(a[0]);

	vector<vector<T>> b(w, vector<T>(h));
	rep(i, h) rep(j, w) b[j][i] = a[i][j];

	return b;
}


//y90‹‰ñ“]zO(h w)
/*
* a[0..h)[0..w) ‚ğ”½Œv‰ñ‚è‚É 90‹‰ñ“]‚µ‚½‚à‚Ì‚ğ•Ô‚·D
*/
template <class T> vector<vector<T>> rotate90(const vector<vector<T>>& a) {
	int h = sz(a), w = sz(a[0]);

	vector<vector<T>> b(w, vector<T>(h));
	rep(i, h) rep(j, w) b[w - 1 - j][i] = a[i][j];

	return b;
}


//y180‹‰ñ“]zO(h w)
/*
* a[0..h)[0..w) ‚ğ 180‹‰ñ“]‚µ‚½‚à‚Ì‚ğ•Ô‚·D
*/
template <class T> vector<vector<T>> rotate180(const vector<vector<T>>& a) {
	int h = sz(a), w = sz(a[0]);

	vector<vector<T>> b(h, vector<T>(w));
	rep(i, h) rep(j, w) b[i][j] = a[h - 1 - i][w - 1 - j];

	return b;
}


//y270‹‰ñ“]zO(h w)
/*
* a[0..h)[0..w) ‚ğ”½Œv‰ñ‚è‚É 270‹‰ñ“]‚µ‚½‚à‚Ì‚ğ•Ô‚·D
*/
template <class T> vector<vector<T>> rotate270(const vector<vector<T>>& a) {
	// verify : https://atcoder.jp/contests/abc036/tasks/abc036_b
	
	int h = sz(a), w = sz(a[0]);

	vector<vector<T>> b(w, vector<T>(h));
	rep(i, h) rep(j, w) b[j][h - 1 - i] = a[i][j];

	return b;
}


