#pragma once
#include "header.h"
#include "FPS(mint).h"
#include "エスパー.h"
// ■■■■■ 疎行列 ■■■■■


//【疎行列】
/*
* n 行の疎行列を表す構造体
*
* K 個の要素を除き値は 0 である．
* 行ごとに (列, 値) のリストをもつ．
*
* SMatrix(int n) : O(n)
*	n 行の零行列で初期化する．
*
* set(int i, int j, T v) : O(1)
*	A[i][j] = v とする．
*	制約：初期化にのみ利用する．同じ (i, j) に対して 2 度以上呼ばない．
*
* c * A ／ A * c : O(K)
*	疎行列 A とスカラー c のスカラー積を返す．*= も使用可．
*
* A * v : O(K)
*	疎行列 A と列ベクトル v の積を返す．
*/
template <class T>
struct SMatrix {
	int n; // 行列のサイズ（n 行）
	vector<vector<pair<int, T>>> v; // 行列の成分

	// n 行の零行列で初期化する．
	SMatrix(int n) : n(n), v(n) {
		// verify : https://atcoder.jp/contests/abc228/tasks/abc228_g
	}
	SMatrix() : n(0) {}

	// 代入
	SMatrix(const SMatrix& old) = default;
	SMatrix& operator=(const SMatrix& other) = default;

	// 要素の設定
	void set(int i, int j, T val) {
		// verify : https://atcoder.jp/contests/abc228/tasks/abc228_g
		
		v[i].emplace_back(j, val); 
	}

	// スカラー倍
	SMatrix& operator*=(const T& sc) {
		rep(i, n) repe(p, v[i]) p.second *= sc;
		return *this;
	}
	SMatrix operator*(const T& sc) const { return SMatrix(*this) *= sc; }
	friend SMatrix operator*(const T& sc, const SMatrix& mat) { return mat * sc; }

	// 行列ベクトル積
	vector<T> operator*(const vector<T>& vec) const {
		// verify : https://atcoder.jp/contests/abc228/tasks/abc228_g
		
		vector<T> res(n);
		rep(i, n) repe(p, v[i]) res[i] += p.second * vec[p.first];
		return res;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const SMatrix& a) {
		int m = 1;
		rep(i, a.n) repe(p, a.v[i]) chmax(m, p.first + 1);

		vector<vector<T>> tmp(a.n, vector<T>(m));
		rep(i, a.n) repe(p, a.v[i]) tmp[i][p.first] += p.second;

		rep(i, a.n) {
			rep(j, m) os << tmp[i][j] << " ";
			os << endl;
		}
		return os;
	}
#endif
};


//【疎行列同士の積】
/*
* 疎行列同士の積は疎行列とは限らない．
* 例えば，[1, 1, ..., 1]^T × [1, 1, ..., 1] で全て 1 の行列になる．
*/


//【最小多項式】O(n^2 + n K) （K : A の非 0 要素数）
/*
* n 次正方行列 A[0..n)[0..n) の最小多項式を返す．
* 最小多項式とは，f(A) = O を満たす次数最小なモニック多項式である．
*
* 利用：【線形漸化式の発見】
*/
MFPS minimal_polynomial(const SMatrix<mint>& A) {
	// 参考 : https://yukicoder.me/wiki/black_box_linear_algebra

	int n = A.n;

	mt19937 mt;
	mt.seed((int)time(NULL));
	uniform_int_distribution<int> rnd(0, mint::mod() - 1);

	vm u(n), v(n);
	rep(i, n) {
		u[i] = rnd(mt);
		v[i] = rnd(mt);
	}

	vm s(2 * n);
	rep(i, 2 * n) {
		rep(j, n) s[i] += u[j] * v[j];
		v = A * v;
	}

	vm c = berlekamp_massey(s);

	MFPS f(c);
	f = (1 - (f >> 1)).rev();

	return f;
}


//【行列式】O(n^2 + n K) （K : A の非 0 要素数）
/*
* n 次正方行列 A[0..n)[0..n) の行列式を返す．
*
* 利用：【線形漸化式の発見】
*/
mint determinant(const SMatrix<mint>& A) {
	// 参考 : https://yukicoder.me/wiki/black_box_linear_algebra
	// verify : https://judge.yosupo.jp/problem/sparse_matrix_det

	int n = A.n;

	mt19937 mt;
	mt.seed((int)time(NULL));
	uniform_int_distribution<int> rnd(1, mint::mod() - 1);

	vm u(n), v(n), D(n);
	rep(i, n) {
		u[i] = rnd(mt);
		v[i] = rnd(mt);
		D[i] = rnd(mt);
	}

	vm s(2 * n);
	rep(i, 2 * n) {
		rep(j, n) s[i] += u[j] * v[j];
		v = A * v;
		rep(j, n) v[j] *= D[j];
	}

	vm c = berlekamp_massey(s);

	if (sz(c) != n) return 0;

	mint res = (n & 1 ? 1 : -1) * c[n - 1];
	mint detD = 1;
	rep(i, n) detD *= D[i];
	res /= detD;

	return res;
}


