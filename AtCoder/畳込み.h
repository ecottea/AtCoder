#pragma once
#include "header.h"
// ■■■■■ 畳込み ■■■■■


//【畳込みの使い所】
/*
* 数列 a, b が与えられているとする．
* 畳込みは，全ての k <= n についての Σi∈[0..k] a[i] b[k-i] を一括 O(n log n) で求めたいときに使う．
* 特定の k に対して求めたいだけであれば，愚直に和をとっても O(k) で計算できる．
* 
* verify : https://atcoder.jp/contests/abc276/tasks/abc276_g
*/


//【畳込みの一般化】
/*
* R(+,x) を半環，M(o) をモノイドとし，モノイド半環 R[M] を構成する．
* A = Σi a[i] i, B = Σj b[j] j ∈ R[M] の積 C = Σk c[k] k ∈ R[M] は
*	c[k] = +_(ioj=k) a[i] x b[j]
* によって定義され，これが数列 a, b の畳込みと同一視できる．
* 
* 例えば整数列の通常の畳込みは Z(+,x) 半環と Z(+) モノイドに対応する．
*/

 
//【畳込み（法が任意）】O((n + m) log (n + m))
/*
* a と b の mod を法とした畳込みを返す．
*/
vi convolution_arbitrary_mod(const vi& a, const vi& b, int mod = (int)1e9 + 7) {
	// verify : https://judge.yosupo.jp/problem/convolution_mod_1000000007

	int n = sz(a), m = sz(b);

	vl a0(n), a1(n), b0(m), b1(m); const int pow2 = 1 << 15;
	rep(i, n) {
		int ai = smod(a[i], mod);
		a0[i] = ai % pow2;
		a1[i] = ai / pow2;
	}
	rep(i, m) {
		int bi = smod(b[i], mod);
		b0[i] = bi % pow2;
		b1[i] = bi / pow2;
	}

	vl c00 = convolution_ll(a0, b0);
	vl c11 = convolution_ll(a1, b1);
	rep(i, n) a0[i] += a1[i];
	rep(i, m) b0[i] += b1[i];
	vl c01 = convolution_ll(a0, b0);
	rep(i, n + m - 1) {
		c00[i] %= mod;
		c11[i] %= mod;
		c01[i] = (c01[i] - c00[i] - c11[i] + 2LL * mod) % mod;
	}

	vi c(n + m - 1);
	rep(i, n + m - 1) {
		c[i] = (int)((c00[i] + c01[i] * pow2 + c11[i] * pow2 * pow2) % mod);
	}

	return c;
}


//【畳込み（mint，法が任意）】O((n + m) log (n + m))
/*
* a と b の mod を法とした畳込みを返す．
*/
vm convolution_arbitrary_mod(const vm& a, const vm& b) {
	int n = sz(a), m = sz(b);
	int mod = mint::mod();

	vl a0(n), a1(n), b0(m), b1(m); const int pow2 = 1 << 15;
	rep(i, n) {
		int ai = a[i].val();
		a0[i] = ai % pow2;
		a1[i] = ai / pow2;
	}
	rep(i, m) {
		int bi = b[i].val();
		b0[i] = bi % pow2;
		b1[i] = bi / pow2;
	}

	vl c00 = convolution_ll(a0, b0);
	vl c11 = convolution_ll(a1, b1);
	rep(i, n) a0[i] += a1[i];
	rep(i, m) b0[i] += b1[i];
	vl c01 = convolution_ll(a0, b0);
	rep(i, n + m - 1) {
		c00[i] %= mod;
		c11[i] %= mod;
		c01[i] = (c01[i] - c00[i] - c11[i] + 2LL * mod) % mod;
	}

	vm c(n + m - 1);
	rep(i, n + m - 1) {
		c[i] = c00[i] + c01[i] * pow2 + c11[i] * pow2 * pow2;
	}

	return c;
}


//【畳込み】O(n m)
/*
* a[0..n) と b[0..m) を畳み込んだ数列 c[0..n+m-1) を返す．
*/
template <class T>
vector<T> naive_convolution(const vector<T>& a, const vector<T>& b) {
	// verify : https://atcoder.jp/contests/abc214/tasks/abc214_g

	int n = sz(a), m = sz(b);

	// c[i] = Σj∈[0..i] a[j] b[i-j]  (∀i∈[0..n+m-1))
	vector<T> c(n + m - 1);
	rep(i, n + m - 1) {
		repi(j, max(i - (m - 1), 0), min(i, n - 1)) {
			c[i] += a[j] * b[i - j];
		}
	}

	return c;
}


//【上側畳込み】
/*
* 与えられた a[0..n], b[0..n] に対して
*		c[i] = Σj∈[i..n] a[n+i-j] b[j]
* なる c[0..n] を求めたい場合，convolution(a, b)[n..2n] を取得すればよい．
* 
* verify : https://atcoder.jp/contests/abc217/tasks/abc217_g
*/


//【自己畳込み】O(n^2 log k)
/*
* a[0..n) を k 個畳み込んだ数列 c[0..n) を返す．
*
* 利用：【畳込み】
*
*（繰り返し二乗法）
*/
template <class T>
vector<T> naive_self_convolution(const vector<T>& a, ll k) {
	// verify : https://atcoder.jp/contests/arc059/tasks/arc059_d

	int n = sz(a);

	vector<T> res(n);
	res[0] = 1;

	vector<T> pow2(a);
	while (k > 0) {
		if ((k & 1) != 0) {
			res = naive_convolution(res, pow2);
			res.resize(n);
		}

		pow2 = naive_convolution(pow2, pow2);
		pow2.resize(n);

		k /= 2;
	}

	return res;
}


//【二次元畳込み】O(h1 w1 h2 w2)
/*
* a[0..h1)[0..w1) と b[0..h2)[0..w2) を畳み込んだ二次元配列 c[0..h1+h2-1)[0..w1+w2-1) を返す．
*/
template <class T>
vector<vector<T>> naive_convolution_2D(const vector<vector<T>>& a, const vector<vector<T>>& b) {
	int h1 = sz(a), w1 = sz(a[0]), h2 = sz(b), w2 = sz(b[0]);

	// c[i][j] = Σs Σt a[s][t] b[i-s][j-t]  (∀i∈[0..h1+h2-1), j∈[0..w1+w2-1))
	vector<vector<T>> c(h1 + h2 - 1, vector<T>(w1 + w2 - 1));
	rep(i, h1 + h2 - 1) rep(j, w1 + w2 - 1) {
		repi(s, max(i - (h2 - 1), 0), min(i, h1 - 1)) repi(t, max(j - (w2 - 1), 0), min(j, w1 - 1)) {
			c[i][j] += a[s][t] * b[i - s][j - t];
		}
	}

	return c;
}


