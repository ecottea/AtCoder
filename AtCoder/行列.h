#pragma once
#include "header.h"
// ■■■■■ 行列 ■■■■■


//【行列】
/*
* 行列を表す構造体
*
* Matrix(m, n) : O(m n)
*	m * n 零行列で初期化する．
*
* Matrix(n) : O(n^2)
*	n * n 単位行列で初期化する．
*
* Matrix(a) : O(m n)
*	配列 a の要素で初期化する．
*
* A + B : O(m n)
*	m * n 行列 A, B の和を返す．+= も使用可．
*
* A - B : O(m n)
*	m * n 行列 A, B の差を返す．-= も使用可．
*
* c * A ／ A * c : O(m n)
*	m * n 行列 A とスカラー c のスカラー積を返す．*= も使用可．
*
* A * x : O(m n)
*	m * n 行列 A と n 次元列ベクトル x の積を返す．
*
* x * A : O(m n)
*	m 次元行ベクトル x と m * n 行列 A の積を返す．
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

	// コンストラクタ（初期化なし，零行列，単位行列，二次元配列）
	Matrix() : m(0), n(0) {}
	Matrix(const int& m_, const int& n_) : m(m_), n(n_), v(m_, vector<T>(n_)) {}
	Matrix(const int& n_) : m(n_), n(n_), v(n_, vector<T>(n_)) { rep(i, n) v[i][i] = 1; }
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

	// 加算，減算，スカラー倍
	Matrix& operator+=(const Matrix& b) {
		rep(i, m) rep(j, n) v[i][j] += b.v[i][j];
		return *this;
	}
	Matrix& operator-=(const Matrix& b) {
		rep(i, m) rep(j, n) v[i][j] -= b.v[i][j];
		return *this;
	}
	Matrix& operator*=(const T& c) {
		rep(i, m) rep(j, n) v[i][j] *= c;
		return *this;
	}
	Matrix operator+(const Matrix& b) const { Matrix a = *this; return a += b; }
	Matrix operator-(const Matrix& b) const { Matrix a = *this; return a -= b; }
	Matrix operator*(const T& c) const { Matrix a = *this; return a *= c; }
	friend Matrix operator*(const T& c, const Matrix<T>& a) { return a * c; }

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
		// verify : https://judge.yosupo.jp/problem/matrix_product

		Matrix res(m, b.n);
		rep(i, res.m) rep(j, res.n) rep(k, n) res.v[i][j] += v[i][k] * b.v[k][j];
		return res;
	}
	Matrix& operator*=(const Matrix& b) { *this = *this * b; return *this; }

	// 累乗：O(n^3 log d)
	Matrix pow(ll d) const {
		Matrix res(n), pow2 = *this;
		while (d > 0) {
			if ((d & 1) != 0) res *= pow2;
			pow2 *= pow2;
			d /= 2;
		}
		return res;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Matrix& a) {
		rep(i, a.m) {
			rep(j, a.n) os << a.v[i][j] << " ";
			os << endl;
		}
		return os;
	}
#endif
};


//【階段行列】O(m^2 n)
/*
* 行基本変形で m * n 行列 mat を階段行列に変形する．
* 最も右下のピボットの位置 (i, j) を返す．零行列なら (-1, -1) を返す．
*/
template <class T> pii row_echelon_form(Matrix<T>& mat) {
	int m = mat.m, n = mat.n;
	auto& v = mat.v;
	
	// 直前に見つけたピボットの位置
	int pi = -1, pj = -1;

	// 注目位置を (i, j)（i 行目かつ j 列目）とする．
	int i = 0, j = 0;

	while (i < m && j < n) {
		// 同じ列の下方の行から非 0 成分を見つける．
		int k = i;
		while (k < m && v[k][j] == 0) k++;

		// 見つからなかったら注目位置を右に移す．
		if (k == m) {
			j++;
			continue;
		}

		// 見つかったら i 行目とその行を入れ替える．
		pi = i; pj = j;
		if (i != k) swap(v[i], v[k]);

		// v[i][j] が 1 になるよう行全体を v[i][j] で割る．
		T div = v[i][j];
		repi(t, j, n - 1) v[i][t] /= div;
		
		// v[i][j] より下方の行の成分が全て 0 になるよう i 行目を定数倍して減じる．
		repi(k, i + 1, m - 1) {
			T mul = v[k][j];
			repi(t, j, n - 1) v[k][t] -= v[i][t] * mul;
		}

		// 注目位置を右下に移す．
		i++; j++;
	}

	return { pi, pj };
}


//【行列式】O(n^3)
/*
* n 次正方行列 mat の行列式を返す．
*/
template <class T> T determinant(Matrix<T>& mat) {
	// verify : https://judge.yosupo.jp/problem/matrix_det

	int n = mat.n;
	auto& v = mat.v;

	// 注目位置を (i, j)（i 行目かつ j 列目）とする．
	int i = 0, j = 0;

	// 行列式の値
	T res = 1;

	while (i < n && j < n) {
		// 同じ列の下方の行から非 0 成分を見つける．
		int k = i;
		while (k < n && v[k][j] == 0) k++;

		// 見つからなかったら零列ベクトルを含むので行列式は 0 である．
		if (k == n) return T(0);

		// 見つかったら i 行目とその行を入れ替える．
		// 行列式の値は -1 倍しておく．
		if (k != i) {
			swap(v[i], v[k]);
			res *= T(-1);
		}

		// v[i][j] が 1 になるよう行全体を v[i][j] で割る．
		// 行列式の値は v[i][j] 倍しておく．
		T div = v[i][j];
		repi(t, j, n - 1) v[i][t] /= div;
		res *= div;

		// v[i][j] より下方の行の成分が全て 0 になるよう i 行目を定数倍して減じる．
		// 行列式の値は変化しない．
		repi(k, i + 1, n - 1) {
			T mul = v[k][j];
			repi(t, j, n - 1) v[k][t] -= v[i][t] * mul;
		}

		// 注目位置を右下に移す．
		i++; j++;
	}

	return res;
}


//【逆行列】O(n^3)
/*
* n 次正方行列 mat の逆行列が存在すればそれを mat_inv に格納する．
* また存在する場合は true，存在しない場合は false を返す．
*/
template <class T> bool inverse_matrix(Matrix<T>& mat, Matrix<T>& mat_inv) {
	// verify : https://judge.yosupo.jp/problem/inverse_matrix

	int m = mat.m;

	// 元の行列 mat と単位行列を繋げた拡大行列を作る．
	Matrix<T> aug(m, 2 * m);
	rep(i, m) {
		rep(j, m) {
			aug.v[i][j] = mat.v[i][j];
			aug.v[i][m + j] = (i == j ? T(1) : T(0));
		}
	}
	int n = 2 * m;
	auto& v = aug.v;

	// 拡大行列に対して行基本変形を行い，左側を単位行列にすることを目指す．

	// 直前に見つけたピボットの位置
	int pi = -1, pj = -1;

	// 注目位置を (i, j)（i 行目かつ j 列目）とする．
	int i = 0, j = 0;

	while (i < m && j < n) {
		// 同じ列の下方の行から非 0 成分を見つける．
		int k = i;
		while (k < m && v[k][j] == 0) k++;

		// 見つからなかったら注目位置を右に移す．
		if (k == m) {
			j++;
			continue;
		}

		// 見つかったら i 行目とその行を入れ替える．
		pi = i; pj = j;
		if(i != k) swap(v[i], v[k]);

		// v[i][j] が 1 になるよう行全体を v[i][j] で割る
		T div = v[i][j];
		repi(t, j, n - 1) v[i][t] /= div;
		
		// v[i][j] と同じ列の成分が全て 0 になるよう i 行目を定数倍して減じる．
		rep(k, m) {
			// i 行目だけは引かない．
			if (k == i) continue;
			
			T mul = v[k][j];
			repi(t, j, n - 1) v[k][t] -= v[i][t] * mul;
		}

		// 注目位置を右下に移す．
		i++; j++;
	}

	// mat が単位行列になっていれば，最後に発見したピボットの位置は (n-1, n-1)．
	// そうなっていなければ mat は正則ではないので false を返す．
	if (pi != m - 1 || pj != m - 1) return false;

	// 拡大行列の右半分が mat の逆行列なのでコピーする．
	mat_inv = Matrix<T>(m, m);
	rep(i, m) {
		rep(j, m) {
			mat_inv.v[i][j] = aug.v[i][m + j];
		}
	}

	return true;
}


//【アダマール行列】O(4^n)
/*
* 2^n * 2^n のアダマール行列を mat に格納する．
*/
void hadamard_matrix(int n, vvi& mat) {
	mat = vvi(1LL << n, vi(1LL << n));

	function<void(int, int, int, int)> rf = [&](int n, int x, int y, int sgn) {
		if (n == 0) {
			mat[x][y] = sgn;
			return;
		}

		int b = 1 << (n - 1);
		rf(n - 1, x, y, sgn);
		rf(n - 1, x + b, y, sgn);
		rf(n - 1, x, y + b, sgn);
		rf(n - 1, x + b, y + b, -sgn);
	};

	rf(n, 0, 0, 1);
}


