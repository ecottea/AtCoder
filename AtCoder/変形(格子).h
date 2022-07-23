#pragma once
#include "header.h"
// ¡¡¡¡¡ Šiqƒf[ƒ^‚ÌŒü‚«‚Ì•ÏŠ· ¡¡¡¡¡


//y¶‰E”½“]zO(h w)
/*
* a[0..h)[0..w) ‚ğ¶‰E”½“]‚µ‚½‚à‚Ì‚ğ b ‚ÉŠi”[‚·‚éD
*/
template <class T> void flip_y(const vector<vector<T>>& a, vector<vector<T>>& b) {
	int h = sz(a), w = sz(a[0]);

	b = vector<vector<T>>(h, vector<T>(w));
	rep(i, h) rep(j, w) b[i][j] = a[i][w - 1 - j];
}


//yã‰º”½“]zO(h w)
/*
* a[0..h)[0..w) ‚ğã‰º”½“]‚µ‚½‚à‚Ì‚ğ b ‚ÉŠi”[‚·‚éD
*/
template <class T> void flip_x(const vector<vector<T>>& a, vector<vector<T>>& b) {
	// verify : https://atcoder.jp/contests/abc260/tasks/abc260_g
	
	int h = sz(a), w = sz(a[0]);

	b = vector<vector<T>>(h, vector<T>(w));
	rep(i, h) rep(j, w) b[i][j] = a[h - 1 - i][j];
}


//y“]’uzO(h w)
/*
* a[0..h)[0..w) ‚ğ“]’u‚µ‚½‚à‚Ì‚ğ b ‚ÉŠi”[‚·‚éD
*/
template <class T> void transpose(const vector<vector<T>>& a, vector<vector<T>>& b) {
	// verify : https://yukicoder.me/problems/no/1974

	int h = sz(a), w = sz(a[0]);

	b = vector<vector<T>>(w, vector<T>(h));
	rep(i, h) rep(j, w) b[j][i] = a[i][j];
}


//y90‹‰ñ“]zO(h w)
/*
* a[0..h)[0..w) ‚ğ”½Œv‰ñ‚è‚É 90‹‰ñ“]‚µ‚½‚à‚Ì‚ğ b ‚ÉŠi”[‚·‚éD
*/
template <class T> void rotate90(const vector<vector<T>>& a, vector<vector<T>>& b) {
	int h = sz(a), w = sz(a[0]);

	b = vector<vector<T>>(w, vector<T>(h));
	rep(i, h) rep(j, w) b[w - 1 - j][i] = a[i][j];
}


//y180‹‰ñ“]zO(h w)
/*
* a[0..h)[0..w) ‚ğ 180‹‰ñ“]‚µ‚½‚à‚Ì‚ğ b ‚ÉŠi”[‚·‚éD
*/
template <class T> void rotate180(const vector<vector<T>>& a, vector<vector<T>>& b) {
	int h = sz(a), w = sz(a[0]);

	b = vector<vector<T>>(h, vector<T>(w));
	rep(i, h) rep(j, w) b[i][j] = a[h - 1 - i][w - 1 - j];
}


//y270‹‰ñ“]zO(h w)
/*
* a[0..h)[0..w) ‚ğ”½Œv‰ñ‚è‚É 270‹‰ñ“]‚µ‚½‚à‚Ì‚ğ b ‚ÉŠi”[‚·‚éD
*/
template <class T> void rotate270(const vector<vector<T>>& a, vector<vector<T>>& b) {
	// verify : https://atcoder.jp/contests/abc036/tasks/abc036_b
	
	int h = sz(a), w = sz(a[0]);

	b = vector<vector<T>>(w, vector<T>(h));
	rep(i, h) rep(j, w) b[j][h - 1 - i] = a[i][j];
}


