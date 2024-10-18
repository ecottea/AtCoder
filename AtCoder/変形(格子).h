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


//【上三角領域の矩形分割】O(n log n)
/*
* n×n 格子の狭義上三角領域 S = {(i,j) | 0 ≦ i < j < n} を矩形に分割し，矩形のリストを返す．
* strict = false にすると広義上三角部分 S = {(i,j) | 0 ≦ i ≦ j < n} を矩形に分割する．
* 矩形 [i1..i2)×[j1..j2) は 4 つ組 {i1, i2, j1, j2} で表す．
*
*（分割統治法）
*/
vector<tuple<int, int, int, int>> trig_to_rects(int n, bool strict = true) {
	// verify : https://yukicoder.me/problems/no/1867

	vector<tuple<int, int, int, int>> rects;

	// {(i,j) | l≦i<j<r} を矩形に分割する．
	function<void(int, int)> rf = [&](int l, int r) {
		if (r - l == 1) {
			if (!strict) rects.emplace_back(l, l + 1, l, l + 1);
			return;
		}

		int m = (l + r) / 2;
		rects.emplace_back(l, m, m, r);
		rf(l, m);
		rf(m, r);
	};
	rf(0, n);

	return rects;
}


//【帯領域の矩形分割】O((h + w) log(h + w))
/*
* h×w 格子の帯領域 S = {(x,y) | y_min[x] ≦ y ≦ y_max[x]} を矩形に分割し，矩形のリストを返す．
* 矩形 [x1..x2)×[y1..y2) は 4 つ組 {x1, x2, y1, y2} で表す．
*
* 制約：y_min[0..h), y_max[0..h) は広義単調増加，y_min[i] ≦ y_max[i]
*
*（分割統治法）
*/
vector<tuple<int, int, int, int>> band_to_rects(const vi& y_min, const vi& y_max) {
	int h = sz(y_min), w = y_max.back() + 1;

	// 各 y についての x の下限を求める．
	vi x_min(w, h);
	repir(x, h - 1, 0) x_min[y_max[x]] = x;
	repir(y, w - 2, 0) chmin(x_min[y], x_min[y + 1]);

	// 各 y についての x の上限を求める．
	vi x_max(w, -1);
	repi(x, 0, h - 1) x_max[y_min[x]] = x;
	repi(y, 1, w - 1) chmax(x_max[y], x_max[y - 1]);

	vector<tuple<int, int, int, int>> rects;

	// S ∩ [x1..x2]×[y1..y2] を矩形に分割する．
	function<void(int, int, int, int)> rf = [&](int x1, int x2, int y1, int y2) {		
		if (y_max[x1] >= y2 && y1 >= y_min[x2]) {
			rects.emplace_back(x1, x2 + 1, y1, y2 + 1);
			return;
		}

		if (x2 - x1 > y2 - y1) {
			int xm = (x1 + x2) / 2;

			int ny2 = min(y_max[xm], y2);
			if (y1 <= ny2) rf(x1, xm, y1, ny2);

			int ny1 = max(y_min[xm], y1);
			if (ny1 <= y2) rf(xm + 1, x2, ny1, y2);
		}
		else {
			int ym = (y1 + y2) / 2;

			int nx2 = min(x_max[ym], x2);
			if (x1 <= nx2) rf(x1, nx2, y1, ym);

			int nx1 = max(x_min[ym], x1);
			if (nx1 <= x2) rf(nx1, x2, ym + 1, y2);
		}
	};

	int y1 = 0;
	rep(y, w) {
		if (x_min[y] > x_max[y]) {
			if (y1 <= y - 1) rf(x_min[y1], x_max[y - 1], y1, y - 1);
			y1 = y + 1;
		}
	}
	if (y1 <= w - 1) rf(x_min[y1], x_max[w - 1], y1, w - 1);

	return rects;
}


//【ハニカム格子】
/*
* ハニカム格子は，正方格子において，マス (i, j) が通常の 4 近傍に加え，
* (i-1, j-1), (i+1, j+1) とも隣接しているとみなしたものと対応する．
* 
* さらに (i, j) → (i, j, i-j) ∈ Z^3 により三次元空間に埋め込むと，
* ハニカム格子における 6 近傍距離と三次元空間におけるチェビシェフ距離が等しくなる．
* 
* verify : https://atcoder.jp/contests/abc280/tasks/abc280_g
*/


