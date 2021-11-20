#pragma once
#include "header.h"
// ■■■■■ 半環上の行列 ■■■■■


//【半環上の行列】
/*
* 半環上の行列を表す構造体
*
* Matrix<T>(m, n) : O(m n)
*	m * n 零行列で初期化する．
*   成分は半環 T = (S, add, o, mul, e) の元とする．
*
* Matrix<T>(n) : O(n^2)
*	n * n 単位行列で初期化する．
*
* Matrix<T>(a) : O(m n)
*	配列 a の要素で初期化する．
*
* A + B ／ A - B : O(m n)
*	m * n 行列 A, B の和[差]を返す．+=[-=] も使用可．
*
* c * A ／ A * c : O(m n)
*	m * n 行列 A とスカラー c のスカラー積を返す．
*
* A * x ／ x * A : O(m n)
*	行列ベクトル積[ベクトル行列積]を返す．
*
* A * B : O(l m n)
*	l * m 行列 A と m * n 行列 B の積を返す．
*
* pow(d) : O(n^3 log d)
*	自身を d 乗した行列を返す．
*/
template <class T> struct Matrix {
	int m, n; // 行列のサイズ（m 行 n 列）
	vector<vector<T>> v; // 行列の成分

	// コンストラクタ（初期化なし）
	Matrix() {}

	// 零行列で初期化
	Matrix(const int& m_, const int& n_) : m(m_), n(n_), v(m_, vector<T>(n_)) {}

	// 単位行列で初期化
	Matrix(const int& n_) : m(n_), n(n_), v(n_, vector<T>(n_)) {
		rep(i, n) v[i][i] = T::e();
	}

	// 二次元配列で初期化
	Matrix(const vector<vector<T>>& a) : m(sz(a)), n(sz(a[0])), v(a) {}

	// 代入
	Matrix(const Matrix& b) = default;
	Matrix& operator=(const Matrix& b) = default;

	// 入力
	friend istream& operator>>(istream& is, Matrix& a) {
		rep(i, a.m) rep(j, a.n) is >> a.v[i][j];
		return is;
	}

	// アクセス
	vector<T> const& operator[](int i) const { return v[i]; }
	vector<T>& operator[](int i) { return v[i]; }

	// 比較
	bool operator==(const Matrix& b) const {
		return m == b.m && n == b.n && v == b.v;
	}
	bool operator!=(const Matrix& b) const { return !(*this == b); }

	// 加算，減算
	Matrix& operator+=(const Matrix& b) {
		rep(i, m) rep(j, n) v[i][j] += b.v[i][j];
		return *this;
	}
	Matrix& operator-=(const Matrix& b) {
		rep(i, m) rep(j, n) v[i][j] -= b.v[i][j];
		return *this;
	}
	Matrix operator+(const Matrix& b) const { Matrix a = *this; return a += b; }
	Matrix operator-(const Matrix& b) const { Matrix a = *this; return a -= b; }

	// 左右からのスカラー倍
	Matrix operator*(const T& c) const {
		Matrix res(*this);
		rep(i, m) rep(j, n) res.v[i][j] = res.v[i][j] * c;
		return res;
	}
	friend Matrix operator*(const T& c, const Matrix<T>& a) {
		Matrix res(a);
		rep(i, a.m) rep(j, a.n) res.v[i][j] = c * res.v[i][j];
		return res;
	}

	// 行列ベクトル積 : O(m n)
	vector<T> operator*(const vector<T>& x) const {
		vector<T> y(m);
		rep(i, m) rep(j, n)	y[i] += v[i][j] * x[j];
		return y;
	}

	// ベクトル行列積 : O(m n)
	friend vector<T> operator*(const vector<T>& x, const Matrix& a) {
		vector<T> y(a.n);
		rep(i, a.m) rep(j, a.n) y[j] += x[i] * a.v[i][j];
		return y;
	}

	// 積：O(n^3)
	Matrix operator*(const Matrix& b) const {
		Matrix res(m, b.n);
		rep(i, res.m) rep(j, res.n) rep(k, n) res.v[i][j] += v[i][k] * b.v[k][j];
		return res;
	}

	// 累乗：O(n^3 log d)
	Matrix pow(ll d) const {
		Matrix res(n), pow2(*this);
		while (d > 0) {
			if ((d & 1) != 0) res = res * pow2;
			pow2 = pow2 * pow2;
			d /= 2;
		}
		return res;
	}

	// デバッグ出力用
	friend ostream& operator<<(ostream& os, const Matrix& a) {
		rep(i, a.m) {
			rep(j, a.n) os << a.v[i][j] << " ";
			os << endl;
		}
		return os;
	}
};


//【線形漸化式】O(d^3 log n)
/*
* 初項 a[0..d) と漸化式 a[i] = Σj=[0..d) c[j]a[i-1-j] で定義される
* 半環 T 上の数列 a について，a[n] の値を返す．
*
* 利用：【半環上の行列】
*/
template <class T>
T linearly_recurrent_sequence(vector<T> a, const vector<T>& c, ll n) {
	int d = sz(a);

	// 係数行列を作成する．
	Matrix<T> mat(d, d);
	rep(j, d) mat[0][j] = c[j];
	repi(i, 1, d - 1) mat[i][i - 1] = T::e();

	// 初項は逆順にしてベクトル化する．
	reverse(all(a));

	// 行列累乗をし，初項ベクトルに掛けて結果を得る．
	return (mat.pow(n - 1) * a)[d - 1];
}


