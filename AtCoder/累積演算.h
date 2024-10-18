#pragma once
#include "header.h"
#include "辞書(静的).h"
#include "セグメント木.h"
#include "座標圧縮.h"
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


//【巡回累積和】
/*
* Cyclic_cumulative_sum<T>(vT a) : O(n)
*	配列 a[0..n) が無限に繰り返された配列 A で初期化する（A[0] = a[0]）
*
* T sum(int l, int r) : O(1)
*	ΣA[l..r) を返す．（空なら 0 を返す）
*/
template <class T>
class Cyclic_cumulative_sum {
	ll n;

	// acc[i] : Σa[0..i)
	vector<T> acc;

public:
	// 配列 a[0..n) で初期化する．
	Cyclic_cumulative_sum(const vector<T>& a) : n(sz(a)), acc(n + 1) {
		rep(i, n) acc[i + 1] = acc[i] + a[i];
	}
	Cyclic_cumulative_sum() : n(0) {}

	// Σa[l..r) を返す．（空なら 0 を返す．範囲外の値は 0 とみなす）
	T sum(ll l, ll r) {
		if (l >= r) return 0;

		ll res = 0;

		ll r_rem = smod(r, n);
		ll r_quo = (r - r_rem) / n;
		res += acc[n] * r_quo + acc[r_rem];

		ll l_rem = smod(l, n);
		ll l_quo = (l - l_rem) / n;
		res -= acc[n] * l_quo + acc[l_rem];

		return res;
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
class Thinning_cumulative_sum {
	int n, m;

	// acc[r] : set = {i∈[0..r) | i=r (mod m)} とし Σa[set]
	vector<T> acc;

public:
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
		if (l >= r) return T(0);

		r += smod(k - r, m);
		l += smod(k - l, m);
		return acc[r] - acc[l];
	}
};


//【線形加重累積和】
/*
* Linear_cumulative_sum<T>(vT v) : O(n)
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
class Linear_cumulative_sum {
	int n;

	// acc[0][i] : Σj∈[0..i) v[j]
	// acc[1][i] : Σj∈[0..i) j v[j]
	vector<vector<T>> acc;

public:
	// 配列 a[0..n) で初期化する．
	Linear_cumulative_sum(const vector<T>& v) : n(sz(v)), acc(2, vector<T>(n + 1)) {
		// verify : https://atcoder.jp/contests/agc030/tasks/agc030_b
		
		acc[0][0] = acc[1][0] = T(0);
		rep(i, n) {
			acc[0][i + 1] = acc[0][i] + v[i];
			acc[1][i + 1] = acc[1][i] + i * v[i];
		}
	}
	Linear_cumulative_sum() : n(0) {}

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


//【間引き線形加重累積和】
/*
* Thinning_Linear_cumulative_sum<T>(vT a, int m) : O(n + m)
*	配列 a[0..n) と法 m で初期化する．
*
* T sum_to_right(int l, int r, ll w0, ll w1, int k) : O(1)
*	{i∈[l..r) | i=k (mod m)} に昇順に等差重み w0, w1, ... を掛け合わせて和をとった値を返す．
*
* T sum_to_left(int r, int l, ll w0, ll w1, int k) : O(1)
*	{i∈(l..r] | i=k (mod m)} に降順に等差重み w0, w1, ... を掛け合わせて和をとった値を返す．
*
* 利用：【線形加重累積和】
*/
template <class T>
class Thinning_linear_cumulative_sum {
	int n, m;
	vector<Linear_cumulative_sum<T>> As;

public:
	// 配列 a[0..n) で初期化する．
	Thinning_linear_cumulative_sum(const vector<T>& a, int m) : n(sz(a)), m(m), As(m) {
		// verify : https://codeforces.com/contest/1921/problem/F

		vector<vector<T>> a2(m);
		rep(i, n) a2[i % m].push_back(a[i]);
		rep(j, m) As[j] = Linear_cumulative_sum<T>(a2[j]);
	}
	Thinning_linear_cumulative_sum() : n(0), m(0) {}

	// {i∈[l..r) | i=k (mod m)} に昇順に等差重み w0, w1, ... を掛け合わせて和をとった値を返す．
	inline T sum_to_right(int l, int r, ll w0, ll w1, int k) {
		// verify : https://codeforces.com/contest/1921/problem/F

		k %= m;
		return As[k].sum_to_right((l - k + m - 1) / m, (r - k + m - 1) / m, w0, w1);
	}

	// {i∈(l..r] | i=k (mod m)} に降順に等差重み w0, w1, ... を掛け合わせて和をとった値を返す．
	inline T sum_to_left(int r, int l, ll w0, ll w1, int k) {
		k %= m;
		return As[k].sum_to_left((l - k + m - 1) / m, (r - k + m - 1) / m, w0, w1);
	}
};


//【指数加重累積和】
/*
* Exponential_cumulative_sum(vT v, T B) : O(n)
*	配列 v[0..n) と底 B で初期化する．
*
* mint sum(int l, int r) : O(1)
*	Σi∈[l..r) B^(i-l) v[i] を返す．（空なら 0 を返す，範囲外の値は 0 とみなす）
*/
template <class T>
class Exponential_cumulative_sum {
	int n;

	// acc[l] : Σi∈[l..n) B^(l-i) v[i]
	vm acc;

	// powB[i] : B^i
	vm powB;

public:
	// 配列 a[0..n) と底 B で初期化する．
	Exponential_cumulative_sum(const vector<T>& v, T B) : n(sz(v)), acc(n + 1), powB(n + 1) {
		// verify : https://yukicoder.me/problems/no/2170

		// B の冪の計算
		powB[0] = 1;
		rep(i, n) powB[i + 1] = powB[i] * B;

		// 上からの指数加重累積和の計算
		repir(i, n - 1, 0) acc[i] = acc[i + 1] * B + v[i];
	}
	Exponential_cumulative_sum() : n(0) {}

	// Σi∈[l..r) B^(i-l) v[i] を返す．
	inline mint sum(int l, int r) {
		// verify : https://yukicoder.me/problems/no/2170

		chmax(l, 0); chmin(r, n);
		if (l >= r) return 0;

		return acc[l] - acc[r] * powB[r - l];;
	}
};


//【mod 累積和】
/*
* Mod_cumulative_sum(vT a, int m) : O(n log n)
*	数列 a[0..n) と法 m で初期化する．
*
* T mod_sum(int l, int r, T b) : O(log n)
*	Σi∈[l..r) (a[i] + b) mod m を返す．
*
* T mod_sum_neg(int l, int r, T b) : O(log n)
*	Σi∈[l..r) (-a[i] + b) mod m を返す．
*
* T floor_sum(int l, int r, T b) : O(log n)
*	Σi∈[l..r) floor((a[i] + b) / m) を返す．
*
* T floor_sum_neg(int l, int r, T b) : O(log n)
*	Σi∈[l..r) floor((-a[i] + b) / m) を返す．
*
* 利用：【ウェーブレット行列】
*/
template <class T>
class Mod_cumulative_sum {
	int n;
	T m;

	// A[i] : Σa[0..i);
	vector<T> A;

	// A_rem : A[0..n) mod m
	Wavelet_matrix<T> A_rem;

public:
	// 数列 a[0..n) と法 m で初期化する．
	Mod_cumulative_sum(const vector<T>& a, T m) : n(sz(a)), m(m), A(n + 1) {
		// verify : https://yukicoder.me/problems/no/2627

		Assert(m > 0);

		rep(i, n) A[i + 1] = A[i] + a[i];

		vector<T> ini(n);
		rep(i, n) {
			ini[i] = a[i] % m;
			if (ini[i] < 0) ini[i] += m;
		}
		A_rem = Wavelet_matrix<T>(ini);
	}
	Mod_cumulative_sum() : n(0) {}

	// Σi∈[l..r) (a[i] + b) mod m を返す．
	T mod_sum(int l, int r, T b) {
		// verify : https://yukicoder.me/problems/no/2627
		
		chmax(l, 0); chmin(r, n);
		if (l >= r) return 0;

		T b_rem = b % m;
		if (b_rem < 0) b_rem += m;

		T res = T(r - l) * b_rem;
		res += A_rem.sum(l, r);
		res -= m * A_rem.count(l, r, m - b_rem, m);

		return res;
	}

	// Σi∈[l..r) (-a[i] + b) mod m を返す．
	T mod_sum_neg(int l, int r, T b) {
		// verify : https://yukicoder.me/problems/no/2627
		
		chmax(l, 0); chmin(r, n);
		if (l >= r) return 0;

		T b_rem = b % m;
		if (b_rem < 0) b_rem += m;

		T res = T(r - l) * b_rem;
		res -= A_rem.sum(l, r);
		res += m * A_rem.count(l, r, b_rem + 1, m);

		return res;
	}

	// Σi∈[l..r) floor((a[i] + b) / m) を返す．
	T floor_sum(int l, int r, T b) {
		// verify : https://yukicoder.me/problems/no/2627
		
		chmax(l, 0); chmin(r, n);
		if (l >= r) return 0;

		T res = A[r] - A[l];
		res += T(r - l) * b;
		res -= mod_sum(l, r, b);
		res /= m;

		return res;
	}

	// Σi∈[l..r) floor((-a[i] + b) / m) を返す．
	T floor_sum_neg(int l, int r, T b) {
		// verify : https://yukicoder.me/problems/no/2627
		
		chmax(l, 0); chmin(r, n);
		if (l >= r) return 0;

		T res = -(A[r] - A[l]);
		res += T(r - l) * b;
		res -= mod_sum_neg(l, r, b);
		res /= m;

		return res;
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

	// Σa[x1..x2)[y1..y2) を返す．（空なら 0 を返す）
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


//【間引き二次元累積和（長方形）】
/*
* Thinning_Cumulative_sum_2D<T>(vvT a, int mx, int my) : O((h + mx)(w + my))
*	二次元配列 a[0..h)[0..w) と法 (mx, my) で初期化する．
*
* T sum(int x1, int x2, int y1, int y2, int kx, int ky) : O(1)
*	set = {(i,j)∈[x1..x2)×[y1..y2) | i=kx (mod mx), j=ky (mod my)} とし Σa[set] を返す．
*	（空なら 0 を返す，範囲外の値は 0 とみなす）
*/
template <class T>
class Thinning_Cumulative_sum_2D {
	int h, w;
	int mx, my; // 法

	using vT = vector<T>;
	using vvT = vector<vT>;
	vvT acc;

public:
	// 二次元配列 a[0..h)[0..w) と法 (mx, my) で初期化する．
	Thinning_Cumulative_sum_2D(const vvT& a, int mx, int my) : h(sz(a)), w(sz(a[0])), mx(mx), my(my), acc(h + mx, vT(w + my)) {
		// verify : https://mojacoder.app/users/shinnshinn/contests/nycc-24/tasks/8

		// 元データを仮格納する．
		rep(i, h) rep(j, w) acc[i + mx][j + my] = a[i][j];

		// 縦方向に累積和をとる．
		rep(i, h) rep(j, w + my) acc[i + mx][j] += acc[i][j];

		// 横方向に累積和をとる．
		rep(i, h + mx) rep(j, w) acc[i][j + my] += acc[i][j];
	}
	Thinning_Cumulative_sum_2D() : h(0), w(0), mx(1), my(1) {}

	// set = {(i,j)∈[x1..x2)×[y1..y2) | i=kx (mod mx), j=ky (mod my)} とし Σa[set] を返す．
	inline T sum(int x1, int x2, int y1, int y2, int kx, int ky) {
		// verify : https://mojacoder.app/users/shinnshinn/contests/nycc-24/tasks/8

		chmax(x1, 0); chmax(y1, 0);
		chmin(x2, h); chmin(y2, w);
		if (x1 >= x2 || y1 >= y2) return T(0);

		x1 += smod(kx - x1, mx);
		x2 += smod(kx - x2, mx);
		y1 += smod(ky - y1, my);
		y2 += smod(ky - y2, my);

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
* T sum_tri_DL(int x, int y, int len) : O(1)
*	[x..x+len)×[y..y+len) の右下がりの対角線以下の要素の和を返す．
*
* T sum_tri_UR(int x, int y, int len) : O(1)
*	[x..x+len)×[y..y+len) の右下がりの対角線以上の要素の和を返す．
*
* T sum_tri_DR(int x, int y, int len) : O(1)
*	[x..x+len)×[y..y+len) の右上がりの対角線以下の要素の和を返す．
*
* T sum_tri_UL(int x, int y, int len) : O(1)
*	[x..x+len)×[y..y+len) の右上がりの対角線以上の要素の和を返す．
*/
template <class T>
class Cumulative_sum_2D_tri {
	int h, w;

	using vT = vector<T>;
	using vvT = vector<vT>;

	// acc_rect[i][j] : [0..i)×[0..j) の要素の和
	// acc_tri_DL[i][j] : [0..i)×[0..j) の要素の内，(i,j) を通る右下がりの直線以下の要素の和
	// acc_tri_DR[i][j] : [0..i)×[j..w) の要素の内，(i,j-1) を通る右上がりの直線以下の要素の和
	vvT acc_rect, acc_tri_DL, acc_tri_DR;

public:
	// 二次元配列 a[0..h)[0..w) で初期化する．
	Cumulative_sum_2D_tri(const vvT& a) : h(sz(a)), w(sz(a[0])) {
		// verify : https://codeforces.com/contest/1921/problem/G

		acc_rect = vvT(h + 1, vT(w + 1));
		rep(i, h) rep(j, w) acc_rect[i + 1][j + 1] = a[i][j];
		repi(i, 0, h) repi(j, 1, w) acc_rect[i][j] += acc_rect[i][j - 1];
		repi(i, 1, h) repi(j, 0, w) acc_rect[i][j] += acc_rect[i - 1][j];

		acc_tri_DL = vvT(h + 1, vT(w + 1));
		rep(i, h) rep(j, w) acc_tri_DL[i + 1][j + 1] = a[i][j];
		repi(i, 0, h) repi(j, 1, w) acc_tri_DL[i][j] += acc_tri_DL[i][j - 1];
		repi(i, 1, h) repi(j, 1, w) acc_tri_DL[i][j] += acc_tri_DL[i - 1][j - 1];

		acc_tri_DR = vvT(h + 1, vT(w + 1));
		rep(i, h) rep(j, w) acc_tri_DR[i + 1][j] = a[i][j];
		repi(i, 0, h) repir(j, w - 1, 0) acc_tri_DR[i][j] += acc_tri_DR[i][j + 1];
		repi(i, 1, h) repir(j, w - 1, 0) acc_tri_DR[i][j] += acc_tri_DR[i - 1][j + 1];
	}
	Cumulative_sum_2D_tri() : h(0), w(0) {}

	// [x1..x2)×[y1..y2) の要素の和を返す．
	T sum_rect(int x1, int x2, int y1, int y2) const {
		// verify : https://codeforces.com/contest/1921/problem/G

		chmax(x1, 0); chmax(y1, 0); chmin(x2, h); chmin(y2, w);
		if (x1 >= x2 || y1 >= y2) return T(0);

		T res(0);
		res += acc_rect[x2][y2];
		res -= acc_rect[x1][y2];
		res -= acc_rect[x2][y1];
		res += acc_rect[x1][y1];

		return res;
	}

	// [x..x+len)×[y..y+len) の右下がりの対角線以下の要素の和を返す．
	T sum_tri_DL(int x1, int y1, int len) const {
		// verify : https://codeforces.com/contest/1921/problem/G

		int x2 = x1 + len, y2 = y1 + len;

		T res(0);

		// 左にはみ出ている場合は削る．
		if (y1 < 0) {
			int del = 0 - y1;

			x1 += del;
			y1 = 0;
			len -= del;
		}

		// 下にはみ出ている場合は削る．
		if (x2 > h) {
			int del = x2 - h;

			x2 = h;
			y2 -= del;
			len -= del;
		}

		if (x1 >= h || y1 >= w || x2 <= 0 || y2 <= 0 || len <= 0) return res;

		// 右にはみ出ている場合は左下の長方形と左上の三角形に分割する．
		if (y2 > w) {
			int del = y2 - w;

			res += acc_rect[x2][w];
			res -= acc_rect[x2][y1];
			if (x2 - del >= 0) {
				res -= acc_rect[x2 - del][w];
				res += acc_rect[x2 - del][y1];
			}

			x2 -= del;
			y2 = w;
			len -= del;
		}

		// 全体が上にはみ出ている場合はその分からの寄与は無い．
		if (x2 <= 0) return res;

		// 長方形内からの寄与を加える．
		res += acc_tri_DL[x2][y2];
		res -= acc_rect[x2][y1];
		if (x1 >= 0) {
			res -= acc_tri_DL[x1][y1];
			res += acc_rect[x1][y1];
		}

		return res;
	}

	// [x..x+len)×[y..y+len) の右下がりの対角線以上の要素の和を返す．
	T sum_tri_UR(int x, int y, int len) const {
		// verify : https://codeforces.com/contest/1921/problem/G

		T res(0);
		res += sum_rect(x, x + len, y, y + len);
		res -= sum_tri_DL(x + 1, y, len - 1);

		return res;
	}

	// [x..x+len)×[y..y+len) の右上がりの対角線以下の要素の和を返す．
	T sum_tri_DR(int x1, int y1, int len) const {
		// verify : https://codeforces.com/contest/1921/problem/G

		int x2 = x1 + len, y2 = y1 + len;

		T res(0);

		// 右にはみ出ている場合は削る．
		if (y2 > w) {
			int del = y1 - w;

			x1 += del;
			y2 = w;
			len -= del;
		}

		// 下にはみ出ている場合は削る．
		if (x2 > h) {
			int del = x2 - h;

			x2 = h;
			y1 += del;
			len -= del;
		}

		if (x1 >= h || y1 >= w || x2 <= 0 || y2 <= 0 || len <= 0) return res;

		// 左にはみ出ている場合は右下の長方形と右上の三角形に分割する．
		if (y1 < 0) {
			int del = 0 - y1;

			res += acc_rect[x2][y2];
			if (x2 - del >= 0) {
				res -= acc_rect[x2 - del][y2];
			}

			x2 -= del;
			y1 = 0;
			len -= del;
		}

		// 全体が上にはみ出ている場合はその分からの寄与は無い．
		if (x2 <= 0) return res;

		// 長方形内からの寄与を加える．
		res += acc_tri_DR[x2][y1];
		if (x1 >= 0) {
			res -= acc_tri_DR[x1][y2];
		}
		res -= sum_rect(x1, x2, y2, w);

		return res;
	}

	// [x..x+len)×[y..y+len) の右上がりの対角線以上の要素の和を返す．
	T sum_tri_UL(int x, int y, int len) const {
		// verify : https://codeforces.com/contest/1921/problem/G

		T res(0);
		res += sum_rect(x, x + len, y, y + len);
		res -= sum_tri_DR(x + 1, y + 1, len - 1);

		return res;
	}
};


//【二次元巡回累積和（長方形）】
/*
* Cyclic_cumulative_sum_2D<T>(vT a) : O(h w)
*	二次元配列 a[0..h)[0..w) が無限に繰り返された二次元配列 A で初期化する（A[0][0] = a[0][0]）
*
* T sum(ll x1, ll x2, ll y1, ll y2) : O(1)
*	ΣA[x1..x2)[y1..y2) を返す．（空なら 0 を返す）
*/
template <class T>
class Cyclic_cumulative_sum_2D {
	ll h, w;

	using vT = vector<T>;
	using vvT = vector<vT>;

	// acc[i][j] : Σa[0..i)[0..j)
	vvT acc;

public:
	// 配列 a[0..n) で初期化する．
	Cyclic_cumulative_sum_2D(const vvT& a) : h(sz(a)), w(sz(a[0])), acc(h + 1, vT(w + 1)) {
		// verify : https://atcoder.jp/contests/abc354/tasks/abc354_d

		// 元データを仮格納する．
		rep(i, h) rep(j, w) acc[i + 1][j + 1] = a[i][j];

		// 縦方向に累積和をとる．
		repi(i, 1, h) repi(j, 0, w) acc[i][j] += acc[i - 1][j];

		// 横方向に累積和をとる．
		repi(i, 0, h) repi(j, 1, w) acc[i][j] += acc[i][j - 1];
	}
	Cyclic_cumulative_sum_2D() : h(0), w(0) {}

	// ΣA[x1..x2)[y1..y2) を返す．（空なら 0 を返す）
	T sum(ll x1, ll x2, ll y1, ll y2) {
		// verify : https://atcoder.jp/contests/abc354/tasks/abc354_d

		if (x1 >= x2 || y1 >= y2) return 0;

		ll x1_rem = smod(x1, h);
		ll x1_quo = (x1 - x1_rem) / h;

		ll x2_rem = smod(x2, h);
		ll x2_quo = (x2 - x2_rem) / h;

		ll y1_rem = smod(y1, w);
		ll y1_quo = (y1 - y1_rem) / w;

		ll y2_rem = smod(y2, w);
		ll y2_quo = (y2 - y2_rem) / w;

		T res = 0;

		res += acc[h][w] * x2_quo * y2_quo;
		res += acc[h][y2_rem] * x2_quo;
		res += acc[x2_rem][w] * y2_quo;
		res += acc[x2_rem][y2_rem];

		res -= acc[h][w] * x2_quo * y1_quo;
		res -= acc[h][y1_rem] * x2_quo;
		res -= acc[x2_rem][w] * y1_quo;
		res -= acc[x2_rem][y1_rem];

		res -= acc[h][w] * x1_quo * y2_quo;
		res -= acc[h][y2_rem] * x1_quo;
		res -= acc[x1_rem][w] * y2_quo;
		res -= acc[x1_rem][y2_rem];

		res += acc[h][w] * x1_quo * y1_quo;
		res += acc[h][y1_rem] * x1_quo;
		res += acc[x1_rem][w] * y1_quo;
		res += acc[x1_rem][y1_rem];

		return res;
	}
};


//【三次元累積和（直方体）】
/*
* Cumulative_sum_3D<T>(vvvT a) : O(h w d)
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
		// verify : https://atcoder.jp/contests/abc366/tasks/abc366_d

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
		// verify : https://atcoder.jp/contests/abc366/tasks/abc366_d

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


//【多次元累積和】
/*
* 代数(束) の 大小.h へ．ただしクエリあたり O(2^D) かかるので注意．
*/


//【スライド最小値】O(n)
/*
* 配列 a[0..n) に対し min a(i-w..i] を a_min[i] に格納する．
* max_flag = true のときはスライド最大値を求める．範囲外の値は無視する．
*/
template <class T>
void sliding_window_minimum(vector<T> a, int w, vector<T>& a_min, bool max_flag = false) {
	// 参考：https://qiita.com/kuuso1/items/318d42cd089a49eeb332
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/all/DSL_3_D

	int n = sz(a);
	a_min.resize(n);

	if (max_flag) rep(i, n) a[i] *= -1;

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

	if (max_flag) rep(i, n) a_min[i] *= -1;
}


//【間引きスライド最小値】O(n)
/*
* 配列 a[0..n) に対し，a_min[i] に以下の値（m 個おきでの直前 w 個の最小値）を格納する：
*		min( a[i], a[i-m], a[i-2m], ..., a[i-(w-1)m] )
* max_flag = true のときはスライド最大値を求める．範囲外の値は無視する．
*/
template <class T>
void thinning_sliding_window_minimum(vector<T> a, int w, int m, vector<T>& a_min, bool max_flag = false) {
	int n = sz(a);
	a_min.resize(n);

	if (max_flag) rep(i, n) a[i] *= -1;

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

	if (max_flag) rep(i, n) a_min[i] *= -1;
}


//【二次元スライド最小値】O(h w)
/*
* 二次元配列 a[0..h)[0..w) に対し min a(i-dh..i](j-dw..j] を a_min[i][j] に格納する．
* max_flag = true のときはスライド最大値を求める．範囲外の値は無視する．
*/
template <class T>
void sliding_window_minimum_2D(vector<vector<T>> a, int dh, int dw,
	vector<vector<T>>& a_min, bool max_flag = false)
{
	// verify : https://atcoder.jp/contests/abc228/tasks/abc228_f

	Assert(dh > 0 && dw > 0);
	int h = sz(a), w = sz(a[0]);
	vector<vector<T>> a_tmp(h, vector<T>(w));
	a_min = vector<vector<T>>(h, vector<T>(w));

	if (max_flag) rep(i, h) rep(j, w) a[i][j] *= -1;

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

	if (max_flag) rep(i, h) rep(j, w) a_min[i][j] *= -1;
}


//【Sparse Table（最小値）】
/*
* Sparse_table<T>(vS a, bool max_flag = false) : O(n log n)
*	配列 a[0..n) で初期化する．max_flag = false[true] のときは最小値[最大値] を求める．
*
* T get(int l, int r) : O(1)
*	min a[l..r) を返す．（空なら INFL を返す）
*/
template <class T>
class Sparse_table {
	// 参考 : https://tookunn.hatenablog.com/entry/2016/07/13/211148

	int n, m;
	bool max_flag;

	// acc[j][i] : min a[i..i+2^j)
	vector<vector<T>> acc;

public:
	// 配列 a[0..n) で初期化する．max_flag = false[true] のときは最小値[最大値] を求める．
	Sparse_table(const vector<T>& a, bool max_flag = false)
		: n(sz(a)), m(msb(n) + 1), max_flag(max_flag), acc(m, vector<T>(n))
	{
		// verify : https://judge.yosupo.jp/problem/staticrmq

		rep(i, n) acc[0][i] = a[i];

		if (max_flag) rep(i, n) acc[0][i] *= -1;

		repi(j, 1, m - 1) {
			int d = 1 << (j - 1);
			rep(i, n - d) {
				// acc[j - 1] の後ろの方には適切に初期化していない要素が入っているが，
				// それに基づき計算された acc[j][i] は get で参照されることはない．
				acc[j][i] = min(acc[j - 1][i], acc[j - 1][i + d]);
			}
		}
	}
	Sparse_table() : n(0), m(0), max_flag(false) {}

	// min a[l..r) を返す．
	T get(int l, int r) {
		// verify : https://judge.yosupo.jp/problem/staticrmq

		chmax(l, 0); chmin(r, n);
		if (l >= r) {
			if (!max_flag) return T(INFL);
			else return -T(INFL);
		}

		int j = msb(r - l);
		return min(acc[j][l], acc[j][r - (1 << j)]) * (max_flag ? -1 : 1);
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
* Sparse_table_indexed<T>(vS a, bool max_flag = false) : O(n log n)
*	配列 a[0..n) で初期化する．max_flag = false[true] のときは最小値[最大値] を求める．
*
* pTi get(int l, int r) : O(1)
*	min a[l..r) とそれを与える位置の組を返す．（空なら {INFL, -1} を返す）
*/
template <class T>
class Sparse_table_indexed {
	// 参考 : https://tookunn.hatenablog.com/entry/2016/07/13/211148

	int n, m;
	bool max_flag;

	// acc[j][i] : min a[i..i+2^j)
	vector<vector<pair<T, int>>> acc;

public:
	// 配列 a[0..n) で初期化する．max_flag = false[true] のときは最小値[最大値] を求める．
	Sparse_table_indexed(const vector<T>& a, bool max_flag = false)
		: n(sz(a)), m(msb(n) + 1), max_flag(max_flag), acc(m, vector<pair<T, int>>(n))
	{
		// verify : https://atcoder.jp/contests/agc026/tasks/agc026_d

		rep(i, n) acc[0][i] = { a[i], i };

		if (max_flag) rep(i, n) acc[0][i].first *= -1;

		repi(j, 1, m - 1) {
			int d = 1 << (j - 1);
			rep(i, n - d) acc[j][i] = min(acc[j - 1][i], acc[j - 1][i + d]);
		}
	}
	Sparse_table_indexed() : n(0), m(0), max_flag(false) {}

	// min a[l..r) とそれを与える位置の組を返す．（空なら {INFL, -1} を返す）
	pair<T, int> get(int l, int r) const{
		// verify : https://atcoder.jp/contests/agc026/tasks/agc026_d

		chmax(l, 0); chmin(r, n);
		if (l >= r) {
			if (!max_flag) return make_pair(T(INFL), -1);
			else return make_pair(-T(INFL), -1);
		}

		int j = msb(r - l);
		auto [val, i] = min(acc[j][l], acc[j][r - (1 << j)]);

		if (!max_flag) return make_pair(val, i);
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
* Thinning_sparse_table<T>(vT a, int m, bool max_flag = false) : O(n log n)
*	配列 a[0..n) と法 m で初期化する．max_flag = false[true] のときは最小値[最大値] を求める．
*
* T get(int l, int r, int k) : O(1)
*	set = {i∈[l..r) | i=k (mod m)} とし，min a[set] を返す．（空なら numeric_limits<T>::max() を返す）
*
* 利用：【Sparse Table（最小値）】
*/
template <class T>
class Thinning_sparse_table {
	int m; // 法
	vector<Sparse_table<T>> sts;

public:
	// 配列 a[0..n) と法 m で初期化する
	Thinning_sparse_table(const vector<T>& a, int m, bool max_flag = false) : m(m), sts(m) {
		// verify : https://atcoder.jp/contests/arc080/tasks/arc080_c

		vector<vector<T>> a2(m);
		rep(i, sz(a)) a2[i % m].push_back(a[i]);
		rep(j, m) sts[j] = Sparse_table<T>(a2[j], max_flag);
	}
	Thinning_sparse_table() : m(1) {}

	// set = {x∈[l..r) | x mod m = k} とし，min a[set] を返す．（空なら numeric_limits<T>::max() を返す）
	T get(int l, int r, int k) {
		// verify : https://atcoder.jp/contests/arc080/tasks/arc080_c

		k %= m;
		if (k < 0) k += m;

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
* Sparse_table<T>(vvT a, bool max_flag = false) : O(h w log h log w)
*	二次元配列 a[0..h)[0..w) で初期化する．max_flag = false[true] のときは最小値[最大値] を求める．
*
* T get(int x1, int x2, int y1, int y2) : O(1)
*	min a[x1..x2)[y1..y2) を返す．（空なら INFL を返す）
*/
template <class T>
struct Sparse_table_2D {
	// 参考 : https://kopricky.github.io/code/DataStructure_Advanced/sparse_table_2D.html

	int h, w, bh, bw;
	bool max_flag;

	using vT = vector<T>;
	using vvT = vector<vT>;
	using vvvT = vector<vvT>;
	using vvvvT = vector<vvvT>;

	// acc[bx][by][x][y] : Σa[x..x+2^bx)[y..y+2^by)
	vvvvT acc;

	// コンストラクタ（初期化なし，二次元配列で初期化）
	Sparse_table_2D() : h(0), w(0), bh(0), bw(0) {}
	Sparse_table_2D(const vvT& a, bool max_flag = false)
		: h(sz(a)), w(sz(a[0])), bh(msb(h) + 1), bw(msb(w) + 1),
		max_flag(max_flag), acc(bh, vvvT(bw, vvT(h, vT(w))))
	{
		// verify : https://atcoder.jp/contests/abc233/tasks/abc233_g

		rep(x, h) rep(y, w) acc[0][0][x][y] = a[x][y];

		if (max_flag) rep(x, h) rep(y, w) acc[0][0][x][y] *= -1;

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
		if (x1 >= x2 || y1 >= y2) return (!max_flag ? T(INFL) : -T(INFL));

		int bx = msb(x2 - x1), by = msb(y2 - y1);

		T res = acc[bx][by][x1][y1];
		chmin(res, acc[bx][by][x2 - (1 << bx)][y1]);
		chmin(res, acc[bx][by][x1][y2 - (1 << by)]);
		chmin(res, acc[bx][by][x2 - (1 << bx)][y2 - (1 << by)]);

		return res * (!max_flag ? 1 : -1);
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


// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■ 以下抽象代数上の実装 ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■


//【累積可逆積（群）】
/*
* Cumulative_prod<S, op, e, inv>(vS a) : O(n)
*	配列 a[0..n) で初期化する．
*	要素は群 <S, op, e, inv> の元とする．
*
* S prod(int l, int r) : O(1)
*	Πa[l..r) を返す．（空なら e() を返す．範囲外の値は e() とみなす）
*/
template <class S, S(*op)(S, S), S(*e)(), S(*inv)(S)>
struct Cumulative_prod {
	int n;

	// acc[i]     : a[0] a[1] ... a[i - 1]
	// acc_inv[i] : a[i - 1]^(-1) ... a[1]^(-1) a[0]^(-1)
	vector<S> acc, acc_inv;

	// 配列 a[0..n) で初期化する．
	Cumulative_prod(const vector<S>& a) : n(sz(a)), acc(n + 1), acc_inv(n + 1) {
		// verify : https://yukicoder.me/problems/no/2443

		// acc[0] = e()
		acc[0] = e();

		// acc[i + 1] = acc[i] a[i]
		rep(i, n) acc[i + 1] = op(acc[i], a[i]);

		// acc_inv[n] = acc[n]^(-1)
		acc_inv[n] = inv(acc[n]);

		// acc_inv[i] = a[i] acc_inv[i + 1]
		repir(i, n - 1, 0) acc_inv[i] = op(a[i], acc_inv[i + 1]);
	}
	Cumulative_prod() : n(0) {} // ダミー

	// Πa[l..r) を返す．（空なら e() を返す．範囲外の値は e() とみなす）
	S prod(int l, int r) {
		// verify : https://yukicoder.me/problems/no/2443

		chmax(l, 0); chmin(r, n);
		if (l >= r) return e();

		// a[l] ... a[r - 1]
		// = (a[l - 1]^(-1) ... a[0]^(-1)) (a[0] ... a[l - 1] a[l] ... a[r - 1])
		return op(acc_inv[l], acc[r]);
	}
};


//【間引き累積可逆積（群）】
/*
* Thinning_cumulative_prod<S, op, e, inv>(vS a, int m) : O(n)
*	配列 a[0..n) と法 m で初期化する．
*	要素は群 <S, op, e, inv> の元とする．
*
* S prod(int l, int r, int k) : O(1)
*	set = {i∈[l..r) | i=k (mod m)} とし，Πa[set] を返す．（空なら e() を返す）
*
* 利用：【累積可逆積（群）】
*/
template <class S, S(*op)(S, S), S(*e)(), S(*inv)(S)>
struct Thinning_cumulative_prod {
	// verify : https://atcoder.jp/contests/aising2019/tasks/aising2019_d

	int m; // 法
	vector<Cumulative_prod<S, op, e, inv>> cps;

	// 配列 a[0..n) と法 m で初期化する．
	Thinning_cumulative_prod(const vector<S>& a, int m_) : m(m_), cps(m) {
		vector<vector<S>> a2(m);
		rep(i, sz(a)) a2[i % m].push_back(a[i]);
		rep(j, m) cps[j] = Cumulative_prod<S, op, e, inv>(a2[j]);
	}
	Thinning_cumulative_prod() : m(1) {} // ダミー

	// set = {i∈[l..r) | i=k (mod m)} とし，Πa[set] を返す．（空なら e() を返す）
	S prod(int l, int r, int k) {
		k %= m;
		if (k < 0) k += m;

		return cps[k].prod((l - k + m - 1) / m, (r - k + m - 1) / m);
	}
};


//【線形加重累積和（Z-加群）】
/*
* Linear_weighted_cumulative_sum<S, op, o, inv, mul>(vS v) : O(n)
*	配列 v[0..n) で初期化する．
*	要素は Z-加群 <S, op, o, inv, mul> の元とする．
*
* sum(int l, int r, ll a, ll b) : O(1)
*	Σj∈[l..r) (a j + b) v[j] を返す．（空なら o() を返す，範囲外の値は o() とみなす）
*
* sum_right(int l, int r, ll w0, ll w1) : O(1)
*	v[l..r) に昇順に等差重み w0, w1, ... を掛け合わせて和をとった値を返す．
*
* sum_left(int r, int l, ll w0, ll w1) : O(1)
*	v(l..r] に降順に等差重み w0, w1, ... を掛け合わせて和をとった値を返す．
*/
template <class S, S(*op)(S, S), S(*o)(), S(*inv)(S), S(*mul)(ll, S)>
class Linear_weighted_cumulative_sum {
	int n;

	// acc[0][i] : Σj∈[0..i) v[j]
	// acc[1][i] : Σj∈[0..i) j v[j]
	vector<vector<S>> acc;

public:
	// 配列 a[0..n) で初期化する．
	Linear_weighted_cumulative_sum(const vector<S>& v) : n(sz(v)), acc(2, vector<S>(n + 1)) {
		// verify : https://atcoder.jp/contests/agc030/tasks/agc030_b

		acc[0][0] = acc[1][0] = o();
		rep(i, n) {
			acc[0][i + 1] = op(acc[0][i], v[i]);
			acc[1][i + 1] = op(acc[1][i], mul(i, v[i]));
		}
	}
	Linear_weighted_cumulative_sum() : n(0) {}

	// Σj∈[l..r) (a j + b) v[j] を返す．
	S sum(int l, int r, ll a, ll b) {
		chmax(l, 0);  chmin(r, n);
		if (l >= r) return o();

		S res = mul(a, op(acc[1][r], inv(acc[1][l])));
		res += mul(b, op(acc[0][r], inv(acc[0][l])));
		return res;
	}

	// v[l..r) に昇順に等差重み w0, w1, ... を掛け合わせて和をとった値を返す．
	S sum_right(int l, int r, ll w0, ll w1) {
		// verify : https://atcoder.jp/contests/agc030/tasks/agc030_b

		// a l + b = w0, a(l+1) + b = w1 を解いて a, b を求める．
		ll a = w1 - w0;
		ll b = w0 - a * l;
		return sum(l, r, a, b);
	}

	// v(l..r] に降順に等差重み w0, w1, ... を掛け合わせて和をとった値を返す．
	S sum_left(int r, int l, ll w0, ll w1) {
		// verify : https://atcoder.jp/contests/agc030/tasks/agc030_b

		// a r + b = w0, a(r-1) + b = w1 を解いて a, b を求める．
		ll a = w0 - w1;
		ll b = w0 - a * r;
		return sum(l + 1, r + 1, a, b);
	}
};


//【累積非可逆積（モノイド）】
/*
* Cumulative_lossy_prod<S, op, e>(vS a) : O(n)
*	配列 a[0..n) で初期化する
*	要素はモノイド <S, op, e> の元とする．
*
* S left_prod(int r) : O(1)
*	Πa[0..r] を返す．
*
* S right_prod(int l) : O(1)
*	Πa[l..n) を返す．
*
* S without_prod(int i) : O(1)
*	Πa[0..i)a(i..n) を返す．
*/
template <class S, S(*op)(S, S), S(*e)()>
class Cumulative_lossy_prod {
	int n;

	// acc_l[i] : Πa[0..i)
	// acc_r[i] : Πa[i..n)
	vector<S> acc_l, acc_r;

public:
	// コンストラクタ（配列で初期化）
	Cumulative_lossy_prod(const vector<S>& a) : n(sz(a)), acc_l(n + 1), acc_r(n + 1) {
		// verify : https://atcoder.jp/contests/tessoku-book/tasks/tessoku_book_j

		acc_l[0] = acc_r[n] = e();
		rep(i, n) acc_l[i + 1] = op(acc_l[i], a[i]);
		repir(i, n - 1, 0) acc_r[i] = op(a[i], acc_r[i + 1]);
	}
	Cumulative_lossy_prod() : n(0) {} // ダミー

	// Πa[0..r] を返す．
	S left_prod(int r) {
		// verify : https://atcoder.jp/contests/tessoku-book/tasks/tessoku_book_j

		return acc_l[r + 1];
	}

	// Πa[l..n) を返す．
	S right_prod(int l) {
		// verify : https://atcoder.jp/contests/tessoku-book/tasks/tessoku_book_j

		return acc_r[l];
	}

	// Πa[0..i)∪a(i..n) を返す．
	S without_prod(int i) {
		// verify : https://atcoder.jp/contests/abc134/tasks/abc134_c

		return op(acc_l[i], acc_r[i + 1]);
	}
};


//【二次元累積和（アーベル群，長方形）】
/*
* Cumulative_sum_2D<S, op, o, inv>(vvS a) : O(h w)
*	二次元配列 a[0..h)[0..w) で初期化する．
*	要素はアーベル群 <S, op, o, inv> の元とする．
*
* S sum(int x1, int x2, int y1, int y2) : O(1)
*	Σa[x1..x2)[y1..y2) を返す．（空なら o() を返す，範囲外の値は o() とみなす）
*/
template <class S, S(*op)(S, S), S(*o)(), S(*inv)(S)>
class Cumulative_sum_2D {
	int h, w;

	// acc[i][j] : Σa[0..i)[0..j)
	vector<vector<S>> acc;

public:
	// 二次元配列 a[0..h)[0..w) で初期化する．
	Cumulative_sum_2D(const vector<vector<S>>& a)
		: h(sz(a)), w(sz(a[0])), acc(h + 1, vector<S>(w + 1, o())) {
		// verify : https://atcoder.jp/contests/abc005/tasks/abc005_4

		// 元データを仮格納する．
		rep(i, h) rep(j, w) acc[i + 1][j + 1] = a[i][j];

		// 縦方向に累積和をとる．
		repi(i, 1, h) repi(j, 0, w) acc[i][j] = op(acc[i][j], acc[i - 1][j]);

		// 横方向に累積和をとる．
		repi(i, 0, h) repi(j, 1, w) acc[i][j] = op(acc[i][j], acc[i][j - 1]);
	}
	Cumulative_sum_2D() : h(0), w(0) {}

	// Σa[x1..x2)[y1..y2) を返す．
	S sum(int x1, int x2, int y1, int y2) {
		// verify : https://atcoder.jp/contests/abc005/tasks/abc005_4

		if (x1 >= h || y1 >= w || x2 <= 0 || y2 <= 0) return o();
		chmax(x1, 0);
		chmax(y1, 0);
		chmin(x2, h);
		chmin(y2, w);

		S res = o();
		res = op(res, acc[x2][y2]);
		res = op(res, inv(acc[x1][y2]));
		res = op(res, inv(acc[x2][y1]));
		res = op(res, acc[x1][y1]);

		return res;
	}
};


//【二次元累積和（アーベル群，長方形，三角形）】
/*
* Cumulative_sum_2D_tri<S, op, o, inv>(vvS a) : O(h w)
*	二次元配列 a[0..h)[0..w) で初期化する（範囲外の値は o() とみなす）
*	要素はアーベル群 <S, op, o, inv> の元とする．
*
* S sum_rect(int x1, int x2, int y1, int y2) : O(1)
*	[x1..x2)×[y1..y2) の要素の和を返す．
*
* S sum_tri_DL(int x, int y, int d) : O(1)
*	[x-d..x)×[y-d..y) の右下がりの対角線以下の要素の和を返す．
*
* S sum_tri_UR(int x, int y, int d) : O(1)
*	[x-d..x)×[y-d..y) の右下がりの対角線以上の要素の和を返す．
*
* S sum_tri_DR(int x, int y, int d) : O(1)
*	[x-d..x)×[y..y+d) の右上がりの対角線以下の要素の和を返す．
*
* S sum_tri_UL(int x, int y, int d) : O(1)
*	[x-d..x)×[y..y+d) の右上がりの対角線以上の要素の和を返す．
*/
template <class S, S(*op)(S, S), S(*o)(), S(*inv)(S)>
class Cumulative_sum_2D_tri {
	int h, w;

	using vS = vector<S>;
	using vvS = vector<vS>;

	// acc_rect[i][j] : [0..i) * [0..j) の要素の和
	// acc_tri_DL[i][j] : [0..i) * [0..j) の要素の内，(i,j) を通る右下がりの直線以下の要素の和
	// acc_tri_DR[i][w + j] : [0..i) * [j..w) の要素の内，(i,j-1) を通る右上がりの直線以下の要素の和
	vvS acc_rect;
	vvS acc_tri_DL;
	vvS acc_tri_DR;

public:
	// 二次元配列 a[0..h)[0..w) で初期化する．
	Cumulative_sum_2D_tri(const vvS& a) : h(sz(a)), w(sz(a[0])) {
		// verify : https://atcoder.jp/contests/indeednow-finala-open/tasks/indeednow_2015_finala_f

		int h2 = h * 2, w2 = w * 2;

		acc_rect = vvS(h2 + 1, vS(w2 + 1, o()));
		rep(i, h) rep(j, w) acc_rect[i + 1][j + 1] = a[i][j];
		repi(i, 0, h2) repi(j, 1, w2) acc_rect[i][j] = op(acc_rect[i][j], acc_rect[i][j - 1]);
		repi(i, 1, h2) repi(j, 0, w2) acc_rect[i][j] = op(acc_rect[i][j], acc_rect[i - 1][j]);

		acc_tri_DL = vvS(h2 + 1, vS(w2 + 1, o()));
		rep(i, h) rep(j, w) acc_tri_DL[i + 1][j + 1] = a[i][j];
		repi(i, 0, h2) repi(j, 1, w2) acc_tri_DL[i][j] = op(acc_tri_DL[i][j], acc_tri_DL[i][j - 1]);
		repi(i, 1, h2) repi(j, 1, w2) acc_tri_DL[i][j] = op(acc_tri_DL[i][j], acc_tri_DL[i - 1][j - 1]);

		acc_tri_DR = vvS(h2 + 1, vS(w2 + 1, o()));
		rep(i, h) rep(j, w) acc_tri_DR[i + 1][w + j] = a[i][j];
		repi(i, 0, h2) repir(j, w2 - 1, 0) acc_tri_DR[i][j] = op(acc_tri_DR[i][j], acc_tri_DR[i][j + 1]);
		repi(i, 1, h2) repir(j, w2 - 1, 0) acc_tri_DR[i][j] = op(acc_tri_DR[i][j], acc_tri_DR[i - 1][j + 1]);
	}
	Cumulative_sum_2D_tri() : h(0), w(0) {}

	// [x1..x2)×[y1..y2) の要素の和を返す．
	S sum_rect(int x1, int x2, int y1, int y2) {
		chmax(x1, 0);
		chmax(y1, 0);
		chmin(x2, h);
		chmin(y2, w);
		if (x1 >= x2 || y1 >= y2) return o();

		S res = o();
		res = op(res, acc_rect[x2][y2]);
		res = op(res, inv(acc_rect[x1][y2]));
		res = op(res, inv(acc_rect[x2][y1]));
		res = op(res, acc_rect[x1][y1]);

		return res;
	}

	// [x-d..x)×[y-d..y) の右下がりの対角線以下の要素の和を返す．
	S sum_tri_DL(int x, int y, int d) {
		if (x > h) {
			d -= x - h;
			y -= x - h;
			x = h;
		}
		if (y > 2 * w) {
			d -= y - 2 * w;
			y = 2 * w;
		}
		if (x <= 0 || y <= 0 || d <= 0) return o();

		int x1 = max(x - d, 0);
		int y1 = max(y - d, 0);

		S res = o();
		res = op(res, acc_tri_DL[x][y]);
		res = op(res, inv(acc_tri_DL[x1][y1]));
		res = op(res, inv(acc_rect[x][y1]));
		res = op(res, acc_rect[x1][y1]);

		return res;
	}

	// [x-d..x)×[y-d..y) の右下がりの対角線以上の要素の和を返す．
	S sum_tri_UR(int x, int y, int d) {
		S res = o();
		res = op(res, sum_rect(x - d, y - d, x, y));
		res = op(res, inv(sum_tri_DL(x, y - 1, d - 1)));

		return res;
	}

	// [x-d..x)×[y..y+d) の右上がりの対角線以下の要素の和を返す．
	S sum_tri_DR(int x, int y, int d) {
		// verify : https://atcoder.jp/contests/indeednow-finala-open/tasks/indeednow_2015_finala_f

		if (x > h) {
			d -= x - h;
			y += x - h;
			x = h;
		}
		if (y < -w) {
			d -= -w - y;
			y = -w;
		}
		if (x <= 0 || y >= w || d <= 0) return o();

		int x1 = max(x - d, 0);
		int y2 = min(y + d, w);

		S res = o();
		res = op(res, acc_tri_DR[x][w + y]);
		res = op(res, inv(acc_tri_DR[x1][w + y2]));
		res = op(res, inv(sum_rect(x1, y2, x, w)));

		return res;
	}

	// [x-d..x)×[y..y+d) の右上がりの対角線以上の要素の和を返す．
	S sum_tri_UL(int x, int y, int d) {
		S res = o();
		res = op(res, sum_rect(x - d, y, x, y + d));
		res = op(res, inv(sum_tri_DR(x, y + 1, d - 1)));

		return res;
	}
};


//【二次元累積非可逆和（可換モノイド）】
/*
* Cumulative_lossy_sum_2D<S, op, o>(vvS a) : O(h w)
*	二次元配列 a[0..h)[0..w) で初期化する．
*	要素は可換モノイド <S, op, o> の元とする．
*
* get_ul(x, y) : O(1)
*	Σa[0..x)[0..y) を返す．
*
* get_ur(x, y) : O(1)
*	Σa[0..x)[y..w) を返す．
*
* get_dl(x, y) : O(1)
*	Σa[x..h)[0..y) を返す．
*
* get_dr(x, y) : O(1)
*	Σa[x..h)[y..w) を返す．
*/
template <class S, S(*op)(S, S), S(*o)()>
class Cumulative_lossy_sum_2D {
	int h, w;

	// acc_ul[i][j] : Σa[0..i)[0..j)
	// acc_ur[i][j] : Σa[0..i)[j..w)
	// acc_dl[i][j] : Σa[i..h)[0..j)
	// acc_dr[i][j] : Σa[i..h)[j..w)
	vector<vector<S>> acc_ul, acc_ur, acc_dl, acc_dr;

public:
	// 二次元配列 a[0..h)[0..w) で初期化する
	Cumulative_lossy_sum_2D(const vector<vector<S>>& a) : h(sz(a)), w(sz(a[0])) {
		// verify : https://yukicoder.me/problems/no/1141

		acc_ul = acc_ur = acc_dl = acc_dr = vector<vector<S>>(h + 1, vector<S>(w + 1, o()));

		// 元データを仮格納する．
		rep(i, h) rep(j, w) {
			acc_ul[i + 1][j + 1] = acc_ur[i + 1][j]
				= acc_dl[i][j + 1] = acc_dr[i][j] = a[i][j];
		}

		// 下方向に累積和をとる．
		repi(i, 1, h) repi(j, 0, w) {
			acc_ul[i][j] = op(acc_ul[i][j], acc_ul[i - 1][j]);
			acc_ur[i][j] = op(acc_ur[i][j], acc_ur[i - 1][j]);
		}

		// 上方向に累積和をとる．
		repir(i, h - 1, 0) repi(j, 0, w) {
			acc_dl[i][j] = op(acc_dl[i][j], acc_dl[i + 1][j]);
			acc_dr[i][j] = op(acc_dr[i][j], acc_dr[i + 1][j]);
		}

		// 右方向に累積和をとる．
		repi(i, 0, h) repi(j, 1, w) {
			acc_ul[i][j] = op(acc_ul[i][j], acc_ul[i][j - 1]);
			acc_dl[i][j] = op(acc_dl[i][j], acc_dl[i][j - 1]);
		}

		// 左方向に累積和をとる．
		repi(i, 0, h) repir(j, w - 1, 0) {
			acc_ur[i][j] = op(acc_ur[i][j], acc_ur[i][j + 1]);
			acc_dr[i][j] = op(acc_dr[i][j], acc_dr[i][j + 1]);
		}
	}
	Cumulative_lossy_sum_2D() : h(0), w(0) {}

	// Σa[0..x)[0..y) を返す．
	S get_ul(int x, int y) {
		// verify : https://yukicoder.me/problems/no/1141

		x = clamp(x, 0, h); y = clamp(y, 0, w);
		return acc_ul[x][y];
	}

	// Σa[0..x)[y..w) を返す．
	S get_ur(int x, int y) {
		// verify : https://yukicoder.me/problems/no/1141

		x = clamp(x, 0, h); y = clamp(y, 0, w);
		return acc_ur[x][y];
	}

	// Σa[x..h)[0..y) を返す．
	S get_dl(int x, int y) {
		// verify : https://yukicoder.me/problems/no/1141

		x = clamp(x, 0, h); y = clamp(y, 0, w);
		return acc_dl[x][y];
	}

	// Σa[x..h)[y..w) を返す．
	S get_dr(int x, int y) {
		// verify : https://yukicoder.me/problems/no/1141

		x = clamp(x, 0, h); y = clamp(y, 0, w);
		return acc_dr[x][y];
	}
};


//【多次元累積和】
/*
* 代数(束) の 大小.h へ．ただしクエリあたり O(2^D) かかるので注意．
*/


//【スライド最小値（全順序集合）】O(n)
/*
* 配列 a[0..n) に対し min a(i-w..i] を a_min[i] に格納する．
* 要素は全順序集合 <S, leq, inf> の元とする．（範囲外の値は inf とみなす）
*/
template <class S, bool(*leq)(S, S), S(*inf)()>
void sliding_window_minimum(const vector<S>& a, int w, vector<S>& a_min) {
	// 参考：https://qiita.com/kuuso1/items/318d42cd089a49eeb332
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/all/DSL_3_D

	int n = sz(a);
	a_min.resize(n);

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
		while (!q.empty() && leq(a[i], a[q.back()])) q.pop_back();

		// 新しく区間に入る数は，常に今後最小値となる可能性があるのでデックの末尾に追加する．
		q.push_back(i);

		// 現時点での最小値を知るには，デックの先頭が指す位置を見れば良い．
		a_min[i] = a[q.front()];
	}
}


//【間引きスライド最小値（全順序集合）】O(n)
/*
* 配列 a[0..n) に対し，a_min[i] に以下の値（m 個おきでの直前 w 個の最小値）を格納する：
*		min( a[i], a[i-m], a[i-2m], ..., a[i-(w-1)m] )
* 要素は全順序集合 <S, leq, inf> の元とする．（範囲外の値は inf とみなす）
*/
template <class S, bool(*leq)(S, S), S(*inf)()>
void thinning_sliding_window_minimum(const vector<S>& a, int w, int m, vector<S>& a_min) {
	int n = sz(a);
	a_min.resize(n);

	// 添字が ir (mod m) のところだけに対してスライド最小値のアルゴリズムを適用する．
	rep(ir, min(m, n)) {
		// 現在の最小値の位置と，今後最小値になりうる数の位置を昇順に入れておくデック
		deque<int> q;

		repi(iq, 0, (n - 1 - ir) / m) {
			int i = iq * m + ir;

			// 現在の最小値が注目区間の外に出たら，デックの先頭から削除する．
			if (!q.empty() && q.front() <= i - w * m) q.pop_front();

			// 新しく区間に入る数以上の数は，今後最小値とはなりえないのでデックの末尾から削除する．
			while (!q.empty() && leq(a[i], a[q.back()])) q.pop_back();

			// 新しく区間に入る数は，常に今後最小値となる可能性があるのでデックの末尾に追加する．
			q.push_back(i);

			// 現時点での最小値を知るには，デックの先頭が指す位置を見れば良い．
			a_min[i] = a[q.front()];
		}
	}
}


//【二次元スライド最小値（全順序集合）】O(h w)
/*
* 二次元配列 a[0..h)[0..w) に対し min a(i-dh..i](j-dw..j] を a_min[i][j] に格納する．
* 要素は全順序集合 <S, leq, inf> の元とする．（範囲外の値は inf とみなす）
*/
template <class S, bool(*leq)(S, S), S(*inf)()>
void sliding_window_minimum_2D(const vector<vector<S>>& a, int dh, int dw, vector<vector<S>>& a_min) {
	// verify : https://atcoder.jp/contests/abc228/tasks/abc228_f

	Assert(dh > 0 && dw > 0);
	int h = sz(a), w = sz(a[0]);
	vector<vector<S>> a_tmp(h, vector<S>(w));
	a_min = vector<vector<S>>(h, vector<S>(w));

	// 縦方向のスライド最小値を求め a_tmp に格納する．
	rep(j, w) {
		// 現在の最小値の位置と，今後最小値になりうる数の位置を入れておくデック
		deque<int> q;

		rep(i, h) {
			// 現在の最小値が注目区間の外に出たら，デックの先頭から削除する．
			if (!q.empty() && q.front() <= i - dh) q.pop_front();

			// 新しく区間に入る数以上の数は，今後最小値とはなりえないのでデックの末尾から削除する．
			while (!q.empty() && leq(a[i][j], a[q.back()][j])) q.pop_back();

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
			while (!q.empty() && leq(a_tmp[i][j], a_tmp[i][q.back()])) q.pop_back();

			// 新しく区間に入る数は，常に今後最小値となる可能性があるのでデックの末尾に追加する．
			q.push_back(j);

			// 現時点での最小値を知るには，デックの先頭が指す位置を見れば良い．
			a_min[i][j] = a_tmp[i][q.front()];
		}
	}
}


//【Sparse Table（冪等可換モノイド）】
/*
* Sparse_table<S, op, o>(vS a) : O(n log n)
*	配列 a[0..n) で初期化する．要素は冪等可換モノイド <S, op, o> の元とする．
*
* S sum(int l, int r) : O(1)
*	Σa[l..r) を返す．（空なら o() を返す）
*/
template <class S, S(*op)(S, S), S(*o)()>
class Sparse_table {
	// 参考 : https://tookunn.hatenablog.com/entry/2016/07/13/211148

	int n, m;

	// acc[j][i] : Σa[i..i+2^j)
	vector<vector<S>> acc;

public:
	// 配列 a[0..n) で初期化する．
	Sparse_table(const vector<S>& a) : n(sz(a)), m(msb(n) + 1), acc(m, vector<S>(n, o())) {
		// verify : https://codeforces.com/contest/689/problem/D

		rep(i, n) acc[0][i] = a[i];

		repi(j, 1, m - 1) {
			int d = 1 << (j - 1);
			rep(i, n - d) {
				acc[j][i] = op(acc[j - 1][i], acc[j - 1][i + d]);
			}
		}
	}
	Sparse_table() : n(0), m(0) {}

	// Σa[l..r) を返す．
	S sum(int l, int r) {
		// verify : https://codeforces.com/contest/689/problem/D

		chmax(l, 0); chmin(r, n);
		if (l >= r) return o();

		int j = msb(r - l);
		return op(acc[j][l], acc[j][r - (1 << j)]);
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


//【間引き Sparse Table（冪等可換モノイド）】
/*
* Sparse_table_mod<S, op, o>(vS a, int m) : O(n log n)
*	配列 a[0..n) と法 m で初期化する
*	要素は冪等可換モノイド <S, op, o> の元とする．
*
* S sum(int l, int r, int k) : O(1)
*	set = {i∈[l..r) | i=k (mod m)} とし，Σa[set] を返す．（空なら o() を返す）
*
* 利用：【Sparse Table（冪等可換モノイド）】
*/
template <class S, S(*op)(S, S), S(*o)()>
class Thinning_sparse_table {
	int m; // 法
	vector<Sparse_table<S, op, o>> sts;

public:
	// 配列 a[0..n) と法 m で初期化する
	Thinning_sparse_table(const vector<S>& a, int m_) : m(m_), sts(m) {
		// verify : https://atcoder.jp/contests/arc080/tasks/arc080_c

		vector<vector<S>> a2(m);
		rep(i, sz(a)) a2[i % m].push_back(a[i]);
		rep(j, m) sts[j] = Sparse_table<S, op, o>(a2[j]);
	}
	Thinning_sparse_table() : m(1) {}

	// set = {x∈[l..r) | x mod m = k} とし，Σa[set] を返す．（空なら o() を返す）
	S sum(int l, int r, int k) {
		// verify : https://atcoder.jp/contests/arc080/tasks/arc080_c

		k %= m;
		if (k < 0) k += m;

		return sts[k].sum((l - k + m - 1) / m, (r - k + m - 1) / m);
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Thinning_sparse_table& stm) {
		rep(j, stm.m) os << stm.sts[j] << "\n";
		return os;
	}
#endif
};


//【二次元 Sparse Table（冪等可換モノイド）】
/*
* Sparse_table<S, op, o>(vvS a) : O(h w log h log w)
*	二次元配列 a[0..h)[0..w) で初期化する．
*	要素は冪等可換モノイド <S, op, o> の元とする．
*
* S get(int x1, int x2, int y1, int y2) : O(1)
*	Σa[x1..x2)[y1..y2) を返す．（空なら o() を返す）
*/
template <class S, S(*op)(S, S), S(*o)()>
struct Sparse_table_2D {
	// 参考 : https://kopricky.github.io/code/DataStructure_Advanced/sparse_table_2D.html
	// verify : https://codeforces.com/problemset/problem/713/D

	int h, w, bh, bw;

	// acc[bx][by][x][y] : Σa[x..x+2^bx)[y..y+2^by)
	vector<vector<vector<vector<S>>>> acc;

	// 二次元配列 a[0..h)[0..w) で初期化する．
	Sparse_table_2D(const vector<vector<S>>& a) : h(sz(a)), w(sz(a[0])), bh(msb(h) + 1), bw(msb(w) + 1),
		acc(bh, vector<vector<vector<S>>>(bw, vector<vector<S>>(h, vector<S>(w, o()))))
	{
		// verify : https://codeforces.com/problemset/problem/713/D

		rep(x, h) rep(y, w) acc[0][0][x][y] = a[x][y];

		repi(bx, 1, bh - 1) {
			int dx = 1 << (bx - 1);
			rep(x, h - dx) {
				rep(y, w) {
					acc[bx][0][x][y] = op(acc[bx - 1][0][x][y], acc[bx - 1][0][x + dx][y]);
				}
			}
		}

		rep(bx, bh) {
			int dx = (bx > 0 ? 1 << (bx - 1) : 0);
			repi(by, 1, bw - 1) {
				int dy = 1 << (by - 1);
				rep(x, h - dx) {
					rep(y, w - dy) {
						acc[bx][by][x][y]
							= op(acc[bx][by - 1][x][y], acc[bx][by - 1][x][y + dy]);
					}
				}
			}
		}
	}
	Sparse_table_2D() : h(0), w(0), bh(0), bw(0) {}

	// Σa[x1..x2)[y1..y2) を返す．（空なら o() を返す）
	S get(int x1, int x2, int y1, int y2) {
		// verify : https://codeforces.com/problemset/problem/713/D

		chmax(x1, 0); chmax(y1, 0); chmin(x2, h); chmin(y2, w);
		if (x1 >= x2 || y1 >= y2) return o();

		int bx = msb(x2 - x1), by = msb(y2 - y1);

		S res = o();
		res = op(res, acc[bx][by][x1][y1]);
		res = op(res, acc[bx][by][x2 - (1 << bx)][y1]);
		res = op(res, acc[bx][by][x1][y2 - (1 << by)]);
		res = op(res, acc[bx][by][x2 - (1 << bx)][y2 - (1 << by)]);

		return res;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Sparse_table_2D& st) {
		rep(bx, st.bh) {
			rep(by, st.bw) {
				rep(x, st.h) {
					rep(y, st.w) os << st.acc[bx][by][x][y] << " ";
					os << "\n";
				}
				os << "\n";
			}
		}
		return os;
	}
#endif
};


//【Disjoint Sparse Table（モノイド）】
/*
* Sparse_table<S, op, e>(vS a) : O(n log n)
*	配列 a[0..n) で初期化する．要素はモノイド <S, op, e> の元とする．
*
* S get(int l, int r) : O(1)
*	Πa[l..r) を返す．（空なら e() を返す）
*/
template <class S, S(*op)(S, S), S(*e)()>
class Disjoint_sparse_table {
	// 参考 : https://noshi91.hatenablog.com/entry/2018/05/08/183946

	int n, m;

	// acc[j][i] : a[0..n) を幅 2^(j+1) のブロックに区切ったときの中央からの累積積
	vector<vector<S>> acc;

public:
	// 配列 a[0..n) で初期化する．
	Disjoint_sparse_table(const vector<S>& a) : n(sz(a)), m(msb(n) + 1), acc(m, vector<S>(n, e())) {
		// verify : https://judge.yosupo.jp/problem/staticrmq

		rep(i, n) acc[0][i] = a[i];

		repi(j, 1, m - 1) {
			int J = 1 << j, J1 = J << 1;

			for (int k = 0; ; k += J1) {
				// 左方向への累積積（積は左から順にとる）
				int i_max = k + J - 1, i_min = k;
				if (i_max >= n) break;
				acc[j][i_max] = a[i_max];
				repir(i, i_max - 1, i_min) {
					acc[j][i] = op(a[i], acc[j][i + 1]);
				}

				// 右方向への累積積（積は左から順にとる）
				i_min = k + J; i_max = min(k + J1, n) - 1;
				if (i_min >= n) break;
				acc[j][i_min] = a[i_min];
				repi(i, i_min + 1, i_max) {
					acc[j][i] = op(acc[j][i - 1], a[i]);
				}
			}
		}
	}
	Disjoint_sparse_table() : n(0), m(0) {}

	// Πa[l..r) を返す．（空なら e() を返す）
	S get(int l, int r) {
		// verify : https://judge.yosupo.jp/problem/staticrmq

		// 空の場合の例外処理
		chmax(l, 0); chmin(r, n);
		if (l >= r) return e();

		// 閉区間 [l..r] になおす．
		r--;

		// 1 要素のみだった場合の例外処理
		if (l == r) return acc[0][l];

		// 対応する左方向への累積積と右方向への累積積との積をとって返す．
		int j = msb(l ^ r);
		return op(acc[j][l], acc[j][r]);
	}
};


