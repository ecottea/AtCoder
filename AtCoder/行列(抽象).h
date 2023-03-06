#pragma once
#include "header.h"
// ■■■■■ 行列（抽象代数上） ■■■■■


//【行列（半環）】
/*
* Matrix<S, add, o, mul, e>(m, n) : O(m n)
*	m * n 零行列で初期化する．
*   成分は半環 <S, add, o, mul, e> の元とする．
*
* Matrix<S, add, o, mul, e>(n) : O(n^2)
*	n * n 単位行列で初期化する．
*
* Matrix<S, add, o, mul, e>(a) : O(m n)
*	配列 a の要素で初期化する．
*
* A + B : O(m n)
*	m * n 行列 A, B の和を返す．+= も使用可．
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
template <class S, S(*add)(S, S), S(*o)(), S(*mul)(S, S), S(*e)()>
struct Matrix {
	int m, n; // 行列のサイズ（m 行 n 列）
	vector<vector<S>> v; // 行列の成分

	// コンストラクタ（初期化なし，零行列，単位行列，二次元配列）
	Matrix() : m(0), n(0) {}
	Matrix(int m, int n) : m(m), n(n), v(m, vector<S>(n, o())) {}
	Matrix(int n) : m(n), n(n), v(n, vector<S>(n, o())) { rep(i, n) v[i][i] = e(); }
	Matrix(const vector<vector<S>>& a) : m(sz(a)), n(sz(a[0])), v(a) {}

	// 代入
	Matrix(const Matrix& b) = default;
	Matrix& operator=(const Matrix& b) = default;

	// 入力
	friend istream& operator>>(istream& is, Matrix& a) {
		rep(i, a.m) rep(j, a.n) is >> a[i][j];
		return is;
	}

	// アクセス
	vector<S> const& operator[](int i) const { return v[i]; }
	vector<S>& operator[](int i) { return v[i]; }

	// 比較
	bool operator==(const Matrix& b) const { return m == b.m && n == b.n && v == b.v; }
	bool operator!=(const Matrix& b) const { return !(*this == b); }

	// 加算
	Matrix& operator+=(const Matrix& b) {
		rep(i, m) rep(j, n) v[i][j] = add(v[i][j], b[i][j]);
		return *this;
	}
	Matrix operator+(const Matrix& b) const { Matrix a = *this; return a += b; }

	// 左右からのスカラー倍
	Matrix operator*(const S& c) const {
		Matrix res(*this);
		rep(i, m) rep(j, n) res[i][j] = mul(res[i][j], c);
		return res;
	}
	friend Matrix operator*(const S& c, const Matrix& a) {
		Matrix res(a);
		rep(i, a.m) rep(j, a.n) res[i][j] = mul(c, res[i][j]);
		return res;
	}

	// 行列ベクトル積 : O(m n)
	vector<S> operator*(const vector<S>& x) const {
		vector<S> y(m, o());
		rep(i, m) rep(j, n)	y[i] = add(y[i], mul(v[i][j], x[j]));
		return y;
	}

	// ベクトル行列積 : O(m n)
	friend vector<S> operator*(const vector<S>& x, const Matrix& a) {
		// verify : https://codeforces.com/contest/1681/problem/E

		vector<S> y(a.n, o());
		rep(i, a.m) rep(j, a.n) y[j] = add(y[j], mul(x[i], a[i][j]));
		return y;
	}

	// 積：O(n^3)
	Matrix operator*(const Matrix& b) const {
		// verify : https://codeforces.com/contest/1681/problem/E

		Matrix res(m, b.n);
		rep(i, res.m) rep(j, res.n) rep(k, n) res[i][j] = add(res[i][j], mul(v[i][k], b[k][j]));
		return res;
	}

	// 累乗：O(n^3 log d)
	Matrix pow(ll d) const {
		// verify : https://atcoder.jp/contests/abc009/tasks/abc009_4

		Matrix res(n), pow2(*this);
		while (d > 0) {
			if ((d & 1) != 0) res = res * pow2;
			pow2 = pow2 * pow2;
			d /= 2;
		}
		return res;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Matrix& a) {
		rep(i, a.m) {
			rep(j, a.n) os << a[i][j] << " ";
			os << endl;
		}
		return os;
	}
#endif
};


//【対称行列（半環）】
/*
* SymMatrix<S, add, o, mul, e>(n) : O(n^2)
*	n * n 零行列で初期化する．
*	成分は半環 <S, add, o, mul, e> の元とする．
*
* SymMatrix<S, add, o, mul, e>(a) : O(n^2)
*	配列 a の要素で初期化する．
*
* SymMatrix::identity(int n) : O(n^2)
*	n * n 単位行列を返す．
*
* A + B : O(n^2)
*	行列 A, B の和を返す．+= も使用可．
*
* c * A ／ A * c : O(n^2)
*	行列 A とスカラー c のスカラー積を返す．
*
* A * x : O(n^2)
*	行列ベクトル積を返す．
*
* A * B : O(n^3)
*	行列 A と行列 B の積を返す．
*
* pow(d) : O(n^3 log d)
*	自身を d 乗した行列を返す．
*/
template <class S, S(*add)(S, S), S(*o)(), S(*mul)(S, S), S(*e)()>
struct SymMatrix {
	int n; // 行列のサイズ（n 行 n 列）
	vector<vector<S>> v; // 行列の成分

	// コンストラクタ（初期化なし，零行列，二次元配列）
	SymMatrix() : n(0) {}
	SymMatrix(int n) : n(n), v(n, vector<S>(n, o())) {}
	SymMatrix(const vector<vector<S>>& a) : n(sz(a)), v(a) {}

	// 代入
	SymMatrix(const SymMatrix& b) = default;
	SymMatrix& operator=(const SymMatrix& b) = default;

	// 入力
	friend istream& operator>>(istream& is, SymMatrix& a) {
		rep(i, a.n) rep(j, a.n) is >> a[i][j];
		return is;
	}

	// アクセス
	vector<S> const& operator[](int i) const { return v[i]; }
	vector<S>& operator[](int i) { return v[i]; }

	// 比較
	bool operator==(const SymMatrix& b) const { return n == b.n && v == b.v; }
	bool operator!=(const SymMatrix& b) const { return !(*this == b); }

	// 単位行列
	static SymMatrix identity(int n) {
		SymMatrix res(n); rep(i, n) res[i][i] = e(); return res;
	}

	// 加算
	SymMatrix& operator+=(const SymMatrix& b) {
		rep(i, n) rep(j, n) v[i][j] = add(v[i][j], b[i][j]);
		return *this;
	}
	SymMatrix operator+(const SymMatrix& b) const { SymMatrix a = *this; return a += b; }

	// 左右からのスカラー倍
	SymMatrix operator*(const S& c) const {
		SymMatrix res(*this);
		rep(i, n) rep(j, n) res[i][j] = mul(res[i][j], c);
		return res;
	}
	friend SymMatrix operator*(const S& c, const SymMatrix& a) {
		SymMatrix res(a);
		rep(i, a.n) rep(j, a.n) res[i][j] = mul(c, res[i][j]);
		return res;
	}

	// 行列ベクトル積 : O(n^2)
	vector<S> operator*(const vector<S>& x) const {
		vector<S> y(n, o());
		rep(i, n) rep(j, n)	y[i] = add(y[i], mul(v[i][j], x[j]));
		return y;
	}

	// 積：O(n^3)
	SymMatrix operator*(const SymMatrix& b) const {
		SymMatrix res(n);
		rep(i, n) repi(j, 0, i) rep(k, n) res[i][j] = add(res[i][j], mul(v[i][k], b[k][j]));
		rep(i, n - 1) repi(j, i + 1, n - 1) res[i][j] = res[j][i];
		return res;
	}

	// 累乗：O(n^3 log d)
	SymMatrix pow(ll d) const {
		SymMatrix res(n), pow2(*this);
		while (d > 0) {
			if ((d & 1) != 0) res = res * pow2;
			pow2 = pow2 * pow2;
			d /= 2;
		}
		return res;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const SymMatrix& a) {
		rep(i, a.n) {
			rep(j, a.n) os << a[i][j] << " ";
			os << endl;
		}
		return os;
	}
#endif
};


//【線形漸化式】O(d^3 log n)
/*
* 初項 a[0..d) と漸化式 a[i] = Σj=[0..d) c[j]a[i-1-j] で定義される
* 半環 <S, add, o, mul, e> 上の数列 a について，a[n] の値を返す．
*
* 利用：【行列（半環）】
*/
template <class S, S(*add)(S, S), S(*o)(), S(*mul)(S, S), S(*e)()>
S linearly_recurrent_sequence(vector<S> a, const vector<S>& c, ll n) {
	// verify : https://atcoder.jp/contests/abc009/tasks/abc009_4

	int d = sz(a);

	// 係数行列を作成する．
	Matrix<S, add, o, mul, e> mat(d, d);
	rep(j, d) mat[0][j] = c[j];
	repi(i, 1, d - 1) mat[i][i - 1] = e();

	// 初項は逆順にしてベクトル化する．
	reverse(all(a));

	// 行列累乗をし，初項ベクトルに掛けて結果を得る．
	return (mat.pow(n) * a)[d - 1];
}


