#pragma once
#include "header.h"
#include "辞書(静的).h"
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

	// min a[l..r) を返す．
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


