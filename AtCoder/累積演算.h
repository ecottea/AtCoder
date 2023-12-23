#pragma once
#include "header.h"
// ■■■■■ 累積演算 ■■■■■


//【累積和】
/*
* Cumulative_sum<T>(vT a) : O(n)
*	配列 a[0..n) で初期化する．
*
* T sum(int l, int r) : O(1)
*	Σa[l..r) を返す．（空なら 0 を返す．範囲外の値は 0 とみなす）
*/
template <class T>
class Cumulative_sum {
	int n;

	// acc[i] : Σa[0..i)
	vector<T> acc;

public:
	// 配列 a[0..n) で初期化する．
	Cumulative_sum(const vector<T>& a) : n(sz(a)), acc(n + 1) {
		// verify : https://atcoder.jp/contests/tessoku-book/tasks/math_and_algorithm_ai

		rep(i, n) acc[i + 1] = acc[i] + a[i];
	}
	Cumulative_sum() : n(0) {}

	// Σa[l..r) を返す．（空なら 0 を返す．範囲外の値は 0 とみなす）
	inline T sum(int l, int r) {
		// verify : https://atcoder.jp/contests/tessoku-book/tasks/math_and_algorithm_ai

		chmax(l, 0); chmin(r, n);
		if (l >= r) return 0;

		return acc[r] - acc[l];
	}
};


//【間引き累積和】
/*
* Thinning_cumulative_sum<T>(vT a, int m) : O(n + m)
*	配列 a[0..n) と法 m で初期化する．
*
* T sum(int l, int r, int k) : O(1)
*	set = {i∈[l..r) | i=k (mod m)} とし Σa[set] を返す．
*	（空なら 0 を返す．範囲外の値は 0 とみなす）
*/
template <class T>
struct Thinning_cumulative_sum {
	int n, m;

	// acc[r] : set = {i∈[0..r) | i=r (mod m)} とし Σa[set]
	vector<T> acc;

	// 配列 a[0..n) で初期化する．
	Thinning_cumulative_sum(const vector<T>& a, int m) : n(sz(a)), m(m), acc(n + m) {
		// verify : https://atcoder.jp/contests/abc288/tasks/abc288_d

		Assert(m > 0);
		rep(i, n) acc[i + m] = acc[i] + a[i];
	}
	Thinning_cumulative_sum() : n(0), m(1) {} // ダミー

	// set = {i∈[l..r) | i=k (mod m)} とし Σa[set] を返す．
	inline T sum(int l, int r, int k) {
		// verify : https://atcoder.jp/contests/abc288/tasks/abc288_d

		chmax(l, 0); chmin(r, n);
		if (l >= r) return 0;

		r += smod(k - r, m);
		l += smod(k - l, m);
		return acc[r] - acc[l];
	}
};


//【線形加重累積和】
/*
* Linear_weighted_cumulative_sum<T>(vT v) : O(n)
*	配列 v[0..n) で初期化する．
*
* T sum(int l, int r, ll a, ll b) : O(1)
*	Σj∈[l..r) (a j + b) v[j] を返す．（空なら 0 を返す，範囲外の値は 0 とみなす）
*
* T sum_to_right(int l, int r, ll w0, ll w1) : O(1)
*	v[l..r) に昇順に等差重み w0, w1, ... を掛け合わせて和をとった値を返す．
*
* T sum_to_left(int r, int l, ll w0, ll w1) : O(1)
*	v(l..r] に降順に等差重み w0, w1, ... を掛け合わせて和をとった値を返す．
*/
template <class T>
class Linear_weighted_cumulative_sum {
	int n;

	// acc[0][i] : Σj∈[0..i) v[j]
	// acc[1][i] : Σj∈[0..i) j v[j]
	vector<vector<T>> acc;

public:
	// 配列 a[0..n) で初期化する．
	Linear_weighted_cumulative_sum(const vector<T>& v) : n(sz(v)), acc(2, vector<T>(n + 1)) {
		// verify : https://atcoder.jp/contests/agc030/tasks/agc030_b
		
		acc[0][0] = acc[1][0] = T(0);
		rep(i, n) {
			acc[0][i + 1] = acc[0][i] + v[i];
			acc[1][i + 1] = acc[1][i] + i * v[i];
		}
	}
	Linear_weighted_cumulative_sum() : n(0) {}

	// Σj∈[l..r) (a j + b) v[j] を返す．
	inline T sum(int l, int r, ll a, ll b) {
		chmax(l, 0); chmin(r, n);
		if (l >= r) return T(0);

		T res = a * (acc[1][r] - acc[1][l]);
		res += b * (acc[0][r] - acc[0][l]);
		return res;
	}

	// v[l..r) に昇順に等差重み w0, w1, ... を掛け合わせて和をとった値を返す．
	inline T sum_to_right(int l, int r, ll w0, ll w1) {
		// verify : https://atcoder.jp/contests/agc030/tasks/agc030_b

		// a l + b = w0, a(l+1) + b = w1 を解いて a, b を求める．
		ll a = w1 - w0;
		ll b = w0 - a * l;
		return sum(l, r, a, b);
	}

	// v(l..r] に降順に等差重み w0, w1, ... を掛け合わせて和をとった値を返す．
	inline T sum_to_left(int r, int l, ll w0, ll w1) {
		// verify : https://atcoder.jp/contests/agc030/tasks/agc030_b

		// a r + b = w0, a(r-1) + b = w1 を解いて a, b を求める．
		ll a = w0 - w1;
		ll b = w0 - a * r;
		return sum(l + 1, r + 1, a, b);
	}
};


//【指数加重累積和】
/*
* Exponential_weighted_cumulative_sum(vT v, T B) : O(n)
*	配列 v[0..n) と底 B で初期化する．
*
* mint sum(int l, int r) : O(1)
*	Σi∈[l..r) B^(i-l) v[i] を返す．（空なら 0 を返す，範囲外の値は 0 とみなす）
*/
template <class T>
class Exponential_weighted_cumulative_sum {
	int n;

	// acc[l] : Σi∈[l..n) B^(l-i) v[i]
	vm acc;

	// powB[i] : B^i
	vm powB;

public:
	// 配列 a[0..n) と底 B で初期化する．
	Exponential_weighted_cumulative_sum(const vector<T>& v, T B) : n(sz(v)), acc(n + 1), powB(n + 1) {
		// verify : https://yukicoder.me/problems/no/2170

		// B の冪の計算
		powB[0] = 1;
		rep(i, n) powB[i + 1] = powB[i] * B;

		// 上からの指数加重累積和の計算
		repir(i, n - 1, 0) acc[i] = acc[i + 1] * B + v[i];
	}
	Exponential_weighted_cumulative_sum() : n(0) {}

	// Σi∈[l..r) B^(i-l) v[i] を返す．
	inline mint sum(int l, int r) {
		// verify : https://yukicoder.me/problems/no/2170

		chmax(l, 0); chmin(r, n);
		if (l >= r) return 0;

		return acc[l] - acc[r] * powB[r - l];;
	}
};


//【二次元累積和（長方形）】
/*
* Cumulative_sum_2D<T>(vvT a) : O(h w)
*	二次元配列 a[0..h)[0..w) で初期化する．
*
* T sum(int x1, int x2, int y1, int y2) : O(1)
*	Σa[x1..x2)[y1..y2) を返す．（空なら 0 を返す，範囲外の値は 0 とみなす）
*/
template <class T>
class Cumulative_sum_2D {
	int h, w;

	using vT = vector<T>;
	using vvT = vector<vT>;

	// acc[i][j] : Σa[0..i)[0..j)
	vvT acc;

public:
	// 二次元配列 a[0..h)[0..w) で初期化する．
	Cumulative_sum_2D(const vvT& a) : h(sz(a)), w(sz(a[0])), acc(h + 1, vT(w + 1)) {
		// verify : https://atcoder.jp/contests/tessoku-book/tasks/tessoku_book_h

		// 元データを仮格納する．
		rep(i, h) rep(j, w) acc[i + 1][j + 1] = a[i][j];

		// 縦方向に累積和をとる．
		repi(i, 1, h) repi(j, 0, w) acc[i][j] += acc[i - 1][j];

		// 横方向に累積和をとる．
		repi(i, 0, h) repi(j, 1, w) acc[i][j] += acc[i][j - 1];
	}
	Cumulative_sum_2D() : h(0), w(0) {}

	// Σa[x1..x2)[y1..y2) を返す．
	inline T sum(int x1, int x2, int y1, int y2) {
		// verify : https://atcoder.jp/contests/tessoku-book/tasks/tessoku_book_h

		chmax(x1, 0); chmax(y1, 0);
		chmin(x2, h); chmin(y2, w);
		if (x1 >= x2 || y1 >= y2) return T(0);

		T res = 0;
		res += acc[x2][y2];
		res -= acc[x1][y2];
		res -= acc[x2][y1];
		res += acc[x1][y1];

		return res;
	}
};


//【二次元累積和（長方形，三角形）】
/*
* Cumulative_sum_2D_tri<T>(vvT a) : O(h w)
*	二次元配列 a[0..h)[0..w) で初期化する（範囲外の値は 0 とみなす）
*
* T sum_rect(int x1, int x2, int y1, int y2) : O(1)
*	[x1..x2)×[y1..y2) の要素の和を返す．
*
* T sum_tri_DL(int x, int y, int d) : O(1)
*	[x-d..x)×[y-d..y) の右下がりの対角線以下の要素の和を返す．
*
* T sum_tri_UR(int x, int y, int d) : O(1)
*	[x-d..x)×[y-d..y) の右下がりの対角線以上の要素の和を返す．
*
* T sum_tri_DR(int x, int y, int d) : O(1)
*	[x-d..x)×[y..y+d) の右上がりの対角線以下の要素の和を返す．
*
* T sum_tri_UL(int x, int y, int d) : O(1)
*	[x-d..x)×[y..y+d) の右上がりの対角線以上の要素の和を返す．
*/
template <class T>
class Cumulative_sum_2D_tri {
	int h, w;

	using vT = vector<T>;
	using vvT = vector<vT>;

	// acc_rect[i][j] : [0..i) * [0..j) の要素の和
	// acc_tri_DL[i][j] : [0..i) * [0..j) の要素の内，(i,j) を通る右下がりの直線以下の要素の和
	// acc_tri_DR[i][w + j] : [0..i) * [j..w) の要素の内，(i,j-1) を通る右上がりの直線以下の要素の和
	vvT acc_rect;
	vvT acc_tri_DL;
	vvT acc_tri_DR;

public:
	// 二次元配列 a[0..h)[0..w) で初期化する．
	Cumulative_sum_2D_tri(const vvT& a) : h(sz(a)), w(sz(a[0])) {
		int h2 = h * 2, w2 = w * 2;

		acc_rect = vvT(h2 + 1, vT(w2 + 1));
		rep(i, h) rep(j, w) acc_rect[i + 1][j + 1] = a[i][j];
		repi(i, 0, h2) repi(j, 1, w2) acc_rect[i][j] += acc_rect[i][j - 1];
		repi(i, 1, h2) repi(j, 0, w2) acc_rect[i][j] += acc_rect[i - 1][j];

		acc_tri_DL = vvT(h2 + 1, vT(w2 + 1));
		rep(i, h) rep(j, w) acc_tri_DL[i + 1][j + 1] = a[i][j];
		repi(i, 0, h2) repi(j, 1, w2) acc_tri_DL[i][j] += acc_tri_DL[i][j - 1];
		repi(i, 1, h2) repi(j, 1, w2) acc_tri_DL[i][j] += acc_tri_DL[i][j];

		acc_tri_DR = vvT(h2 + 1, vT(w2 + 1));
		rep(i, h) rep(j, w) acc_tri_DR[i + 1][w + j] = a[i][j];
		repi(i, 0, h2) repir(j, w2 - 1, 0) acc_tri_DR[i][j] += acc_tri_DR[i][j + 1];
		repi(i, 1, h2) repir(j, w2 - 1, 0) acc_tri_DR[i][j] += acc_tri_DR[i - 1][j + 1];
	}
	Cumulative_sum_2D_tri() : h(0), w(0) {}

	// [x1..x2)×[y1..y2) の要素の和を返す．
	inline T sum_rect(int x1, int x2, int y1, int y2) {
		chmax(x1, 0);
		chmax(y1, 0);
		chmin(x2, h);
		chmin(y2, w);
		if (x1 >= x2 || y1 >= y2) return T(0);

		T res(0);
		res += acc_rect[x2][y2];
		res -= acc_rect[x1][y2];
		res -= acc_rect[x2][y1];
		res += acc_rect[x1][y1];

		return res;
	}

	// [x-d..x)×[y-d..y) の右下がりの対角線以下の要素の和を返す．
	inline T sum_tri_DL(int x, int y, int d) {
		if (x > h) {
			d -= x - h;
			y -= x - h;
			x = h;
		}
		if (y > 2 * w) {
			d -= y - 2 * w;
			y = 2 * w;
		}
		if (x <= 0 || y <= 0 || d <= 0) return T(0);

		int x1 = max(x - d, 0);
		int y1 = max(y - d, 0);

		T res(0);
		res += acc_tri_DL[x][y];
		res -= acc_tri_DL[x1][y1];
		res -= acc_rect[x][y1];
		res += acc_rect[x1][y1];

		return res;
	}

	// [x-d..x)×[y-d..y) の右下がりの対角線以上の要素の和を返す．
	inline T sum_tri_UR(int x, int y, int d) {
		T res(0);
		res += sum_rect(x - d, y - d, x, y);
		res -= sum_tri_DL(x, y - 1, d - 1);

		return res;
	}

	// [x-d..x)×[y..y+d) の右上がりの対角線以下の要素の和を返す．
	inline T sum_tri_DR(int x, int y, int d) {
		if (x > h) {
			d -= x - h;
			y += x - h;
			x = h;
		}
		if (y < -w) {
			d -= -w - y;
			y = -w;
		}
		if (x <= 0 || y >= w || d <= 0) return T(0);

		int x1 = max(x - d, 0);
		int y2 = min(y + d, w);

		T res(0);
		res += acc_tri_DR[x][w + y];
		res -= acc_tri_DR[x1][w + y2];
		res -= sum_rect(x1, y2, x, w);

		return res;
	}

	// [x-d..x)×[y..y+d) の右上がりの対角線以上の要素の和を返す．
	inline T sum_tri_UL(int x, int y, int d) {
		T res(0);
		res += sum_rect(x - d, y, x, y + d);
		res -= sum_tri_DR(x, y + 1, d - 1);

		return res;
	}
};


//【三次元累積和（直方体）】
/*
* Cumulative_sum_3D<T>(vvT a) : O(h w d)
*	三次元配列 a[0..h)[0..w)[0..d) で初期化する．
*
* T sum(int x1, int x2, int y1, int y2, int z1, int z2) : O(1)
*	Σa[x1..x2)[y1..y2)[z1..z2) を返す．（空なら 0 を返す，範囲外の値は 0 とみなす）
*/
template <class T>
class Cumulative_sum_3D {
	int h, w, d;

	using vT = vector<T>;
	using vvT = vector<vT>;
	using vvvT = vector<vvT>;

	// acc[i][j][k] : Σa[0..i)[0..j)[0..k)
	vvvT acc;

public:
	// 三次元配列 a[0..h)[0..w)[0..d) で初期化する．
	Cumulative_sum_3D(const vvvT& a) : h(sz(a)), w(sz(a[0])), d(sz(a[0][0])), acc(h + 1, vvT(w + 1, vT(d + 1))) {
		// verify : https://atcoder.jp/contests/abc280/tasks/abc280_g

		// 元データを仮格納する．
		rep(i, h) rep(j, w) rep(k, d) acc[i + 1][j + 1][k + 1] = a[i][j][k];

		// 縦方向に累積和をとる．
		repi(i, 1, h) repi(j, 0, w) repi(k, 0, d) acc[i][j][k] += acc[i - 1][j][k];

		// 横方向に累積和をとる．
		repi(i, 0, h) repi(j, 1, w) repi(k, 0, d) acc[i][j][k] += acc[i][j - 1][k];

		// 奥行き方向に累積和をとる．
		repi(i, 0, h) repi(j, 0, w) repi(k, 1, d) acc[i][j][k] += acc[i][j][k - 1];
	}
	Cumulative_sum_3D() : h(0), w(0), d(0) {}

	// Σa[x1..x2)[y1..y2)[z1..z2) を返す．
	T sum(int x1,int x2, int y1, int y2, int z1, int z2) {
		// verify : https://atcoder.jp/contests/abc280/tasks/abc280_g

		chmax(x1, 0); chmax(y1, 0); chmax(z1, 0);
		chmin(x2, h); chmin(y2, w); chmin(z2, d);
		if (x1 >= x2 || y1 >= y2 || z1 >= z2) return T(0);

		T res(0);
		res += acc[x2][y2][z2];
		res -= acc[x1][y2][z2];
		res -= acc[x2][y1][z2];
		res += acc[x1][y1][z2];
		res -= acc[x2][y2][z1];
		res += acc[x1][y2][z1];
		res += acc[x2][y1][z1];
		res -= acc[x1][y1][z1];

		return res;
	}
};


//【スライド最小値】O(n)
/*
* 配列 a[0..n) に対し min a(i-w..i] を a_min[i] に格納する．
* min_flag = false のときはスライド最大値を求める．範囲外の値は無視する．
*/
template <class T>
void sliding_window_minimum(vector<T> a, int w, vector<T>& a_min, bool min_flag = true) {
	// 参考：https://qiita.com/kuuso1/items/318d42cd089a49eeb332
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/all/DSL_3_D

	int n = sz(a);
	a_min.resize(n);

	if (!min_flag) rep(i, n) a[i] *= -1;

	// 現在の最小値の位置と，今後最小値になりうる数の位置を昇順に入れておくデック
	deque<int> q;

	// a[0..7) = [4, 2, 3, 1, 5, 7, 6], w = 3 のときの遷移例
	//        i:  0  1  2  3  4  5  6
	//	a[0..0]: [4]					q: [0]
	//	a[0..1]: [4, 2]					q: [1]
	//	a[0..2]: [4, 2, 3]				q: [1, 2]
	//  a(0..3]:    [2, 3, 1]			q: [3]
	//  a(1..4]:       [3, 1, 5]		q: [3, 4]
	//  a(2..5]:          [1, 5, 7]		q: [3, 4, 5]
	//  a(3..6]:             [5, 7, 6]	q: [4, 6]

	rep(i, n) {
		// 現在の最小値が注目区間の外に出たら，デックの先頭から削除する．
		if (!q.empty() && q.front() <= i - w) q.pop_front();

		// 新しく区間に入る数以上の数は，今後最小値とはなりえないのでデックの末尾から削除する．
		while (!q.empty() && a[i] <= a[q.back()]) q.pop_back();

		// 新しく区間に入る数は，常に今後最小値となる可能性があるのでデックの末尾に追加する．
		q.push_back(i);

		// 現時点での最小値を知るには，デックの先頭が指す位置を見れば良い．
		a_min[i] = a[q.front()];
	}

	if (!min_flag) rep(i, n) a_min[i] *= -1;
}


//【間引きスライド最小値】O(n)
/*
* 配列 a[0..n) に対し，a_min[i] に以下の値（m 個おきでの直前 w 個の最小値）を格納する：
*		min( a[i], a[i-m], a[i-2m], ..., a[i-(w-1)m] )
* min_flag = false のときはスライド最大値を求める．範囲外の値は無視する．
*/
template <class T>
void thinning_sliding_window_minimum(vector<T> a, int w, int m, vector<T>& a_min, bool min_flag = true) {
	int n = sz(a);
	a_min.resize(n);

	if (!min_flag) rep(i, n) a[i] *= -1;

	// 添字が ir (mod m) のところだけに対してスライド最小値のアルゴリズムを適用する．
	rep(ir, min(m, n)) {
		// 現在の最小値の位置と，今後最小値になりうる数の位置を昇順に入れておくデック
		deque<int> q;

		repi(iq, 0, (n - 1 - ir) / m) {
			int i = iq * m + ir;

			// 現在の最小値が注目区間の外に出たら，デックの先頭から削除する．
			if (!q.empty() && q.front() <= i - w * m) q.pop_front();

			// 新しく区間に入る数より大きい数は，今後最小値とはなりえないのでデックの末尾から削除する．
			while (!q.empty() && a[i] <= a[q.back()]) q.pop_back();

			// 新しく区間に入る数は，常に今後最小値となる可能性があるのでデックの末尾に追加する．
			q.push_back(i);

			// 現時点での最小値を知るには，デックの先頭が指す位置を見れば良い．
			a_min[i] = a[q.front()];
		}
	}

	if (!min_flag) rep(i, n) a_min[i] *= -1;
}


//【二次元スライド最小値】O(h w)
/*
* 二次元配列 a[0..h)[0..w) に対し min a(i-dh..i](j-dw..j] を a_min[i][j] に格納する．
* min_flag = false のときはスライド最大値を求める．範囲外の値は無視する．
*/
template <class T>
void sliding_window_minimum_2D(vector<vector<T>> a, int dh, int dw,
	vector<vector<T>>& a_min, bool min_flag = true)
{
	// verify : https://atcoder.jp/contests/abc228/tasks/abc228_f

	Assert(dh > 0 && dw > 0);
	int h = sz(a), w = sz(a[0]);
	vector<vector<T>> a_tmp(h, vector<T>(w));
	a_min = vector<vector<T>>(h, vector<T>(w));

	if (!min_flag) rep(i, h) rep(j, w) a[i][j] *= -1;

	// 縦方向のスライド最小値を求め a_tmp に格納する．
	rep(j, w) {
		// 現在の最小値の位置と，今後最小値になりうる数の位置を入れておくデック
		deque<int> q;

		rep(i, h) {
			// 現在の最小値が注目区間の外に出たら，デックの先頭から削除する．
			if (!q.empty() && q.front() <= i - dh) q.pop_front();

			// 新しく区間に入る数以上の数は，今後最小値とはなりえないのでデックの末尾から削除する．
			while (!q.empty() && a[i][j] <= a[q.back()][j]) q.pop_back();

			// 新しく区間に入る数は，常に今後最小値となる可能性があるのでデックの末尾に追加する．
			q.push_back(i);

			// 現時点での最小値を知るには，デックの先頭が指す位置を見れば良い．
			a_tmp[i][j] = a[q.front()][j];
		}
	}

	// 横方向のスライド最小値を求め a_min に格納する
	rep(i, h) {
		// 現在の最小値の位置と，今後最小値になりうる数の位置を入れておくデック
		deque<int> q;

		rep(j, w) {
			// 現在の最小値が注目区間の外に出たら，デックの先頭から削除する．
			if (!q.empty() && q.front() <= j - dw) q.pop_front();

			// 新しく区間に入る数以上の数は，今後最小値とはなりえないのでデックの末尾から削除する．
			while (!q.empty() && a_tmp[i][j] <= a_tmp[i][q.back()]) q.pop_back();

			// 新しく区間に入る数は，常に今後最小値となる可能性があるのでデックの末尾に追加する．
			q.push_back(j);

			// 現時点での最小値を知るには，デックの先頭が指す位置を見れば良い．
			a_min[i][j] = a_tmp[i][q.front()];
		}
	}

	if (!min_flag) rep(i, h) rep(j, w) a_min[i][j] *= -1;
}


//【Sparse Table（最小値）】
/*
* Sparse_table<T>(vS a, bool min_flag = true) : O(n log n)
*	配列 a[0..n) で初期化する．min_flag = true[false] のときは最小値[最大値] を求める．
*
* T get(int l, int r) : O(1)
*	min a[l..r) を返す．（空なら numeric_limits<T>::max() を返す）
*/
template <class T>
class Sparse_table {
	// 参考 : https://tookunn.hatenablog.com/entry/2016/07/13/211148

	int n, m;
	bool min_flag;

	// acc[j][i] : min a[i..i+2^j)
	vector<vector<T>> acc;

public:
	// 配列 a[0..n) で初期化する．min_flag = true[false] のときは最小値[最大値] を求める．
	Sparse_table(const vector<T>& a, bool min_flag = true)
		: n(sz(a)), m(msb(n) + 1), min_flag(min_flag), acc(m, vector<T>(n))
	{
		// verify : https://atcoder.jp/contests/arc045/tasks/arc045_b

		rep(i, n) acc[0][i] = a[i];

		if (!min_flag) rep(i, n) acc[0][i] *= -1;

		repi(j, 1, m - 1) {
			int d = 1 << (j - 1);
			rep(i, n - d) {
				// acc[j - 1] の後ろの方には適切に初期化していない要素が入っているが，
				// それに基づき計算された acc[j][i] は get で参照されることはない．
				acc[j][i] = min(acc[j - 1][i], acc[j - 1][i + d]);
			}
		}
	}
	Sparse_table() : n(0), m(0), min_flag(true) {}

	// min a[l..r) を返す．
	T get(int l, int r) {
		// verify : https://atcoder.jp/contests/arc045/tasks/arc045_b

		chmax(l, 0); chmin(r, n);
		if (l >= r) {
			if (min_flag) return numeric_limits<T>::max();
			else return numeric_limits<T>::lowest();
		}

		int j = msb(r - l);
		return min(acc[j][l], acc[j][r - (1 << j)]) * (min_flag ? 1 : -1);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Sparse_table& st) {
		rep(j, st.m) {
			rep(i, st.n) os << st.acc[j][i] << " ";
			os << "\n";
		}
		return os;
	}
#endif
};


//【Sparse Table（最小値と位置）】
/*
* Sparse_table_indexed<T>(vS a, bool min_flag = true) : O(n log n)
*	配列 a[0..n) で初期化する．min_flag = true[false] のときは最小値[最大値] を求める．
*
* pTi get(int l, int r) : O(1)
*	min a[l..r) とそれを与える位置の組を返す．（空なら {numeric_limits<T>::max(), -1} を返す）
*/
template <class T>
class Sparse_table_indexed {
	// 参考 : https://tookunn.hatenablog.com/entry/2016/07/13/211148

	int n, m;
	bool min_flag;

	// acc[j][i] : min a[i..i+2^j)
	vector<vector<pair<T, int>>> acc;

public:
	// 配列 a[0..n) で初期化する．min_flag = true[false] のときは最小値[最大値] を求める．
	Sparse_table_indexed(const vector<T>& a, bool min_flag = true)
		: n(sz(a)), m(msb(n) + 1), min_flag(min_flag), acc(m, vector<pair<T, int>>(n))
	{
		// verify : https://atcoder.jp/contests/agc026/tasks/agc026_d

		rep(i, n) acc[0][i] = { a[i], i };

		if (!min_flag) rep(i, n) acc[0][i].first *= -1;

		repi(j, 1, m - 1) {
			int d = 1 << (j - 1);
			rep(i, n - d) acc[j][i] = min(acc[j - 1][i], acc[j - 1][i + d]);
		}
	}
	Sparse_table_indexed() : n(0), m(0), min_flag(true) {}

	// min a[l..r) を返す．
	pair<T, int> get(int l, int r) {
		// verify : https://atcoder.jp/contests/agc026/tasks/agc026_d

		chmax(l, 0); chmin(r, n);
		if (l >= r) {
			if (min_flag) return make_pair(numeric_limits<T>::max(), -1);
			else return make_pair(numeric_limits<T>::lowest(), -1);
		}

		int j = msb(r - l);
		T val; int i;
		tie(val, i) = min(acc[j][l], acc[j][r - (1 << j)]);

		if (min_flag) return make_pair(val, i);
		else return make_pair(-val, i);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Sparse_table_indexed& st) {
		rep(j, st.m) {
			rep(i, st.n) os << st.acc[j][i].first << " ";
			os << "\n";
		}
		return os;
	}
#endif
}; 


//【間引き Sparse Table（最小値）】
/*
* Thinning_sparse_table<T>(vT a, int m, bool min_flag = true) : O(n log n)
*	配列 a[0..n) と法 m で初期化する．min_flag = true[false] のときは最小値[最大値] を求める．
*
* T get(int l, int r, int k) : O(1)
*	set = {i∈[l..r) | i=k (mod m)} とし，min a[set] を返す．（空なら numeric_limits<T>::max() を返す）
*
* 利用：【Sparse Table（最小値）】
*/
template <class T>
class Thinning_sparse_table {
	int m; // 法
	bool min_flag;
	vector<Sparse_table<T>> sts;

public:
	// 配列 a[0..n) と法 m で初期化する
	Thinning_sparse_table(const vector<T>& a, int m, bool min_flag = true)
		: m(m), min_flag(min_flag), sts(m)
	{
		// verify : https://atcoder.jp/contests/arc080/tasks/arc080_c

		vector<vector<T>> a2(m);
		rep(i, sz(a)) a2[i % m].push_back(a[i]);
		rep(j, m) sts[j] = Sparse_table<T>(a2[j], min_flag);
	}
	Thinning_sparse_table() : m(1), min_flag(true) {}

	// set = {x∈[l..r) | x mod m = k} とし，min a[set] を返す．（空なら numeric_limits<T>::max() を返す）
	T get(int l, int r, int k) {
		// verify : https://atcoder.jp/contests/arc080/tasks/arc080_c

		return sts[k].get((l - k + m - 1) / m, (r - k + m - 1) / m);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Thinning_sparse_table& stm) {
		rep(j, stm.m) os << stm.sts[j] << "\n";
		return os;
	}
#endif
};


//【二次元 Sparse Table（最小値）】
/*
* Sparse_table<T>(vvT a, bool min_flag = true) : O(h w log h log w)
*	二次元配列 a[0..h)[0..w) で初期化する．min_flag = true[false] のときは最小値[最大値] を求める．
*
* T get(int x1, int x2, int y1, int y2) : O(1)
*	min a[x1..x2)[y1..y2) を返す．（空なら numeric_limits<T>::max() を返す）
*/
template <class T>
struct Sparse_table_2D {
	// 参考 : https://kopricky.github.io/code/DataStructure_Advanced/sparse_table_2D.html

	int h, w, bh, bw;
	bool min_flag;

	using vT = vector<T>;
	using vvT = vector<vT>;
	using vvvT = vector<vvT>;
	using vvvvT = vector<vvvT>;

	// acc[bx][by][x][y] : Σa[x..x+2^bx)[y..y+2^by)
	vvvvT acc;

	// コンストラクタ（初期化なし，二次元配列で初期化）
	Sparse_table_2D() : h(0), w(0), bh(0), bw(0) {}
	Sparse_table_2D(const vvT& a, bool min_flag = true)
		: h(sz(a)), w(sz(a[0])), bh(msb(h) + 1), bw(msb(w) + 1),
		min_flag(min_flag), acc(bh, vvvT(bw, vvT(h, vT(w))))
	{
		// verify : https://atcoder.jp/contests/abc233/tasks/abc233_g

		rep(x, h) rep(y, w) acc[0][0][x][y] = a[x][y];

		if (!min_flag) rep(x, h) rep(y, w) acc[0][0][x][y] *= -1;

		repi(bx, 1, bh - 1) {
			int dx = 1 << (bx - 1);
			rep(x, h - dx) rep(y, w) {
				acc[bx][0][x][y] = min(acc[bx - 1][0][x][y], acc[bx - 1][0][x + dx][y]);
			}
		}

		rep(bx, bh) {
			int dx = (bx > 0 ? 1 << (bx - 1) : 0);
			repi(by, 1, bw - 1) {
				int dy = 1 << (by - 1);
				rep(x, h - dx) rep(y, w - dy) {
					acc[bx][by][x][y] = min(acc[bx][by - 1][x][y], acc[bx][by - 1][x][y + dy]);
				}
			}
		}
	}

	// min a[x1..x2)[y1..y2) を返す．
	T get(int x1, int x2, int y1, int y2) {
		// verify : https://atcoder.jp/contests/abc233/tasks/abc233_g

		chmax(x1, 0); chmax(y1, 0); chmin(x2, h); chmin(y2, w);
		if (x1 >= x2 || y1 >= y2) return (min_flag ? T(INFL) : T(-INFL));

		int bx = msb(x2 - x1), by = msb(y2 - y1);

		T res = acc[bx][by][x1][y1];
		chmin(res, acc[bx][by][x2 - (1 << bx)][y1]);
		chmin(res, acc[bx][by][x1][y2 - (1 << by)]);
		chmin(res, acc[bx][by][x2 - (1 << bx)][y2 - (1 << by)]);

		return res * (min_flag ? 1 : -1);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Sparse_table_2D& st) {
		rep(bx, st.bh) rep(by, st.bw) {
			rep(x, st.h) {
				rep(y, st.w) os << st.acc[bx][by][x][y] << " ";
				os << "\n";
			}
			os << "\n";
		}
		return os;
	}
#endif
};


