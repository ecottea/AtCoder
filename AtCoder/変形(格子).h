#pragma once
#include "header.h"
// ■■■■■ 格子データの向きの変換 ■■■■■


//【左右反転】O(h w)
/*
* a[0..h)[0..w) を左右反転したものを返す．
*/
template <class T>
vector<vector<T>> flip_y(const vector<vector<T>>& a) {
	// verify : https://mojacoder.app/users/kusirakusira/contests/GBC/tasks/4

	int h = sz(a), w = sz(a[0]);

	vector<vector<T>> b(h, vector<T>(w));
	rep(i, h) rep(j, w) b[i][j] = a[i][w - 1 - j];

	return b;
}


//【上下反転】O(h w)
/*
* a[0..h)[0..w) を上下反転したものを返す．
*/
template <class T>
vector<vector<T>> flip_x(const vector<vector<T>>& a) {
	// verify : https://atcoder.jp/contests/abc260/tasks/abc260_g
	
	int h = sz(a), w = sz(a[0]);

	vector<vector<T>> b(h, vector<T>(w));
	rep(i, h) rep(j, w) b[i][j] = a[h - 1 - i][j];

	return b;
}


//【転置】O(h w)
/*
* a[0..h)[0..w) を転置したものを返す．
*/
template <class T>
vector<vector<T>> transpose(const vector<vector<T>>& a) {
	// verify : https://yukicoder.me/problems/no/1974

	int h = sz(a), w = sz(a[0]);

	vector<vector<T>> b(w, vector<T>(h));
	rep(i, h) rep(j, w) b[j][i] = a[i][j];

	return b;
}


//【90°回転】O(h w)
/*
* a[0..h)[0..w) を反時計回りに 90°回転したものを返す．
*/
template <class T>
vector<vector<T>> rotate90(const vector<vector<T>>& a) {
	// verify : https://atcoder.jp/contests/abc218/tasks/abc218_c

	int h = sz(a), w = sz(a[0]);

	vector<vector<T>> b(w, vector<T>(h));
	rep(i, h) rep(j, w) b[w - 1 - j][i] = a[i][j];

	return b;
}


//【180°回転】O(h w)
/*
* a[0..h)[0..w) を 180°回転したものを返す．
*/
template <class T>
vector<vector<T>> rotate180(const vector<vector<T>>& a) {
	int h = sz(a), w = sz(a[0]);

	vector<vector<T>> b(h, vector<T>(w));
	rep(i, h) rep(j, w) b[i][j] = a[h - 1 - i][w - 1 - j];

	return b;
}


//【270°回転】O(h w)
/*
* a[0..h)[0..w) を反時計回りに 270°回転したものを返す．
*/
template <class T>
vector<vector<T>> rotate270(const vector<vector<T>>& a) {
	// verify : https://atcoder.jp/contests/abc036/tasks/abc036_b
	
	int h = sz(a), w = sz(a[0]);

	vector<vector<T>> b(w, vector<T>(h));
	rep(i, h) rep(j, w) b[j][h - 1 - i] = a[i][j];

	return b;
}


//【トリミング】O(h w)
/*
* a[0..h)[0..w) を bg 以外を全て含む最小矩形でトリミングした結果を返す．
*/
template <class T>
vector<vector<T>> trim(const vector<vector<T>>& a, T bg = '.') {
	// verify : https://atcoder.jp/contests/abc218/tasks/abc218_c

	int h = sz(a), w = sz(a[0]);

	// a[l..r)[u..d) : バウンディングボックス
	int l = INF, r = -INF, u = INF, d = -INF;
	rep(i, h) rep(j, w) {
		if (a[i][j] == bg) continue;

		chmin(l, i);
		chmax(r, i);
		chmin(u, j);
		chmax(d, j);
	}

	// 全て bg だった場合の例外処理
	if (l == INF) return vector<vector<T>>();

	vector<vector<T>> res(r - l + 1, vector<T>(d - u + 1));
	repi(i, l, r) repi(j, u, d) res[i - l][j - u] = a[i][j];

	return res;
}


//【ハニカム格子】
/*
* ハニカム格子は，正方格子において，マス (i, j) が通常の 4 近傍に加え，
* (i-1, j-1), (i+1, j+1)} とも隣接しているとみなしたものと対応する．
* 
* さらに (i, j) → (i, j, i-j) ∈ Z^3 により三次元空間に埋め込むと，
* ハニカム格子における 6 近傍距離と三次元空間におけるチェビシェフ距離が等しくなる．
* 
* verify : https://atcoder.jp/contests/abc280/tasks/abc280_g
*/


