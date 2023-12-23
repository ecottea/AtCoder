#pragma once
#include "header.h"
// ■■■■■ 畳込み ■■■■■


//【畳込みの使い所】
/*
* 数列 a[0..n), b[0..n) が与えられているとする．
* 例えば通常の畳込みは，全ての k∈[0..n) についての Σi∈[0..k] a[i] b[k-i] を
* 一括で O(n log n) で求めたいときに使う．
* 特定の k に対して求めたいだけであれば，愚直に和をとっても O(k) で計算できる．
*
* verify : https://atcoder.jp/contests/abc276/tasks/abc276_g
*/


//【上側畳込み】
/*
* 与えられた a[0..N], b[0..N] に対して
*		c[i] = Σj∈[i..N] a[N+i-j] b[j]
* なる c[0..N] を求めたい場合，convolution(a, b)[N..2N] を取得すればよい．
*
* verify : https://atcoder.jp/contests/abc217/tasks/abc217_g
*/


//【畳込み（素朴）】O(n m)
/*
* a[0..n) と b[0..m) を畳み込んだ数列 c[0..n+m-1) を返す．
* すなわち c[k] = Σ_(i+j=k) a[i] b[j] である．
*/
template <class T>
vector<T> naive_convolution(const vector<T>& a, const vector<T>& b) {
	// verify : https://atcoder.jp/contests/abc214/tasks/abc214_g

	int n = sz(a), m = sz(b);
	if (n == 0 || m == 0) return vector<T>();

	// c[k] = Σ_(i+j=k) a[i] b[j]
	vector<T> c(n + m - 1);
	rep(i, n) rep(j, m) c[i + j] += a[i] * b[j];

	return c;
}


//【自己畳込み（素朴）】O(n^2 log k)
/*
* a[0..n) を k 個畳み込んで切り詰めた数列 c[0..n) を返す．
*
* 利用：【畳込み（素朴）】
*/
template <class T>
vector<T> naive_self_convolution(const vector<T>& a, ll k) {
	// verify : https://atcoder.jp/contests/arc059/tasks/arc059_d

	int n = sz(a);
	if (n == 0) return vector<T>();

	vector<T> res(n);
	res[0] = 1;

	// 繰り返し二乗法
	vector<T> pow2(a);
	while (k > 0) {
		if (k & 1) {
			res = naive_convolution(res, pow2);
			res.resize(n);
		}

		pow2 = naive_convolution(pow2, pow2);
		pow2.resize(n);

		k /= 2;
	}

	return res;
}


//【畳込み（複数，素朴）】O(n^2)
/*
* 数列の集合 a の要素を全て畳込んだ結果（長さは n）を返す．
*/
template <class T>
vector<T> naive_multi_convoluion(vector<vector<T>> a) {
	// verify : https://atcoder.jp/contests/nomura2020/tasks/nomura2020_d

	int m = sz(a);
	if (m == 0) return vector<T>{ 1 };

	// (要素数, 数列の番号) の組を要素数昇順に記録する．
	priority_queue_rev<pii> q;
	rep(i, m) {
		if (a[i].empty()) return vector<T>();
		q.push({ sz(a[i]), i });
	}

	// 積のコストが小さい順に掛けていく（マージテク）
	while (sz(q) >= 2) {
		auto [ni, i] = q.top(); q.pop();
		auto [nj, j] = q.top(); q.pop();

		vector<T> c(ni + nj - 1);
		rep(x, ni) rep(y, nj) c[x + y] += a[i][x] * a[j][y];

		a[i] = move(c);
		q.push({ ni + nj - 1, i });
	}

	return a[q.top().second];
}


//【二次元畳込み（素朴）】O(h1 w1 h2 w2)
/*
* a[0..h1)[0..w1) と b[0..h2)[0..w2) を畳み込んだ二次元配列 c[0..h1+h2-1)[0..w1+w2-1) を返す．
* すなわち c[k][k'] = Σ_(i+j=k) Σ_(i'+j'=k') a[i][i'] b[j][j'] である．
*/
template <class T>
vector<vector<T>> naive_convolution_2D(const vector<vector<T>>& a, const vector<vector<T>>& b) {
	int h1 = sz(a), w1 = sz(a[0]), h2 = sz(b), w2 = sz(b[0]);

	// c[k][k'] = Σ_(i+j=k) Σ_(i'+j'=k') a[i][i'] b[j][j']
	vector<vector<T>> c(h1 + h2 - 1, vector<T>(w1 + w2 - 1));
	rep(i, h1) rep(j, h2) rep(k, w1) rep(l, w2) {
		c[i + j][k + l] += a[i][k] * b[j][l];
	}

	return c;
}


//【整数商畳込み】O(m + n log n)
/*
* 与えられた a[0..n), b[0..m) に対して
*       c[k] = Σ(floor(i/j) = k) a[i] b[j]
* なる c[0..n) を返す．
*/
template<class T>
vector<T> floordiv_convolution(const vector<T>& a, const vector<T>& b) {
	// verify : https://yukicoder.me/problems/no/1233

	//【方法】
	// j=0 は無視する．和の範囲についての条件は
	//		floor(i/j) = k
	//		⇔ k ≦ i/j < k+1
	//		⇔ j k ≦ i < j k + j
	// と書き直せるので，
	//		c[k]
	//		= Σj∈[1..m) Σi∈[j k..j k + j) a[i] b[j]
	//		= Σj∈[1..m) b[j] Σa[j k..j k + j)
	// となる．a の累積和を前計算しておけば，
	// k ごとに独立に c[k] を計算しても計算量は調和級数で抑えられる．

	int n = sz(a), m = sz(b);

	vector<T> acc_a(n + 1);
	rep(i, n) acc_a[i + 1] = acc_a[i] + a[i];

	vector<T> c(n);
	repi(j, 1, m - 1) c[0] += b[j] * acc_a[min(j, n)];
	repi(k, 1, n - 1) {
		repi(j, 1, min(m - 1, n / k)) {
			c[k] += b[j] * (acc_a[min(j * k + j, n)] - acc_a[j * k]);
		}
	}

	return c;
}


