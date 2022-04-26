#pragma once
#include "header.h"
#include "FPS(mint).h"
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
	bool operator==(const Matrix& b) const { return m == b.m && n == b.n && v == b.v; }
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
	Matrix operator+(const Matrix& b) const { return Matrix(*this) += b; }
	Matrix operator-(const Matrix& b) const { return Matrix(*this) -= b; }
	Matrix operator*(const T& c) const { return Matrix(*this) *= c; }
	friend Matrix operator*(const T& c, const Matrix<T>& a) { return a * c; }
	Matrix operator-() const { return Matrix(*this) *= T(-1); }

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

	// Mathematica の書式に合わせた出力
	void print() const {
		cerr << "{\n";
		rep(i, m) {
			cerr << "{";
			rep(j, n) cerr << v[i][j] << (j < n - 1 ? "," : "}");
			cerr << (i < m - 1 ? ",\n" : "\n");
		}
		cerr << "}\n";
	}
#endif
};


//【階段行列】O(m^2 n)
/*
* 行基本変形で m * n 行列 mat を階段行列に変形する．
* 最も右下のピボットの位置 (i, j) を返す．零行列なら (-1, -1) を返す．
*/
template <class T> pii reduced_row_echelon_form(Matrix<T>& mat) {
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
template <class T> bool inverse_matrix(const Matrix<T>& mat, Matrix<T>& mat_inv) {
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
		if (k == m) { j++; continue; }

		// 見つかったら i 行目とその行を入れ替える．
		pi = i; pj = j;
		if(i != k) swap(v[i], v[k]);

		// v[i][j] が 1 になるよう行全体を v[i][j] で割る．
		T div = T(1) / v[i][j];
		repi(t, j, n - 1) v[i][t] *= div;
		
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


//【階数標準形】O(m n (m + n))
/*
* A = a[0..m)[0..n) を階数標準形 R_r := [I_r, O; O, O] に変換する行列，すなわち
*		P A Q = R_r （r = rank A）
* を満たす行列 P, Q を p[0..m)(0..m), q[0..n)[0..n) に格納し，r を返す．
*/
template <class T> int rank_normal_form(const Matrix<T>& a, Matrix<T>& p, Matrix<T>& q) {
	int m = a.m, n = a.n;

	// 元の行列 mat と単位行列を繋げた拡大行列を作る．
	Matrix<T> v(m + n, m + n);
	rep(i, m) rep(j, n) v[i][j] = a[i][j];
	rep(i, m) rep(j, m) v[i][m + j] = (i == j ? T(1) : T(0));
	rep(i, n) rep(j, n) v[n + i][j] = (i == j ? T(1) : T(0));

	// 拡大行列に対して行基本変形を行い，左側を単位行列にすることを目指す．

	// 直前に見つけたピボットの位置
	int pi = -1, pj = -1;

	// 注目位置を (i, j)（i 行目かつ j 列目）とする．
	int i = 0, j = 0;

	while (i < m && j < n) {
		dump(v);
		// 同じ列の下方の行から非 0 成分を見つける．
		int k = i;
		while (k < m && v[k][j] == 0) k++;

		// 見つからなかったら注目位置を右に移す．
		if (k == m) { j++; continue; }

		// 見つかったら i 行目とその行を入れ替える．
		pi = i; pj = j;
		if (i != k) swap(v[i], v[k]);

		// v[i][j] が 1 になるよう行全体を v[i][j] で割る．
		T div = T(1) / v[i][j];
		repi(t, j, m + n - 1) v[i][t] *= div;

		// v[i][j] と同じ列の成分が全て 0 になるよう i 行目を定数倍して減じる．
		rep(k, m) {
			// i 行目だけは引かない．
			if (k == i) continue;

			T mul = v[k][j];
			repi(t, j, m + n - 1) v[k][t] -= v[i][t] * mul;
		}

		// 注目位置を右下に移す．
		i++; j++;
	}

	// 続けて拡大行列に対して列基本変形を行い，上側を単位行列にすることを目指す．
	// 直前に見つけたピボットの位置
	pi = -1; pj = -1;

	// 注目位置を (i, j)（i 行目かつ j 列目）とする．
	i = 0; j = 0;

	while (i < m && j < n) {
		dump(v);
		// 同じ行の右方の列から非 0 成分を見つける．
		int k = j;
		while (k < n && v[i][k] == 0) k++;

		// 見つからなかったら注目位置を下に移す．
		if (k == n) { i++; continue; }

		// 見つかったら j 列目とその列を入れ替える．
		pi = i; pj = j;
		if (j != k) rep(t, m + n) swap(v[t][j], v[t][k]);

		// v[i][j] が 1 になるよう列全体を v[i][j] で割る．
		T div = T(1) / v[i][j];
		repi(t, i, m + n - 1) v[t][j] *= div;

		// v[i][j] と同じ行の成分が全て 0 になるよう j 列目を定数倍して減じる．
		rep(k, n) {
			// j 列目だけは引かない．
			if (k == j) continue;

			T mul = v[i][k];
			repi(t, 0, m + n - 1) v[t][k] -= v[t][i] * mul;
		}

		// 注目位置を右下に移す．
		i++; j++;
	}

	// 拡大行列の右側が P, 下側が Q なのでコピーする．
	p = Matrix<T>(m, m); q = Matrix<T>(n, n);
	rep(i, m) rep(j, m) p[i][j] = v[i][m + j];
	rep(i, n) rep(j, n) q[i][j] = v[n + i][j];

	return pi + 1;
}


//【最小多項式】O(n^3)
/*
* 正方行列 A = a[0..n)[0..n) の最小多項式を f に格納する．
* 最小多項式とは，f(A) = O を満たす次数最小なモニック多項式である．
*
* 利用：【線形漸化式の発見】
*/
void minimal_polynomial(const Matrix<mint>& a, MFPS& f) {
	// 参考 : https://yukicoder.me/wiki/black_box_linear_algebra

	int n = a.n;

	mt19937 mt;
	mt.seed((int)time(NULL));
	uniform_int_distribution<> rnd(0, 998244352);

	vm u(n), v(n);
	rep(i, n) {
		u[i] = rnd(mt);
		v[i] = rnd(mt);
	}

	vm s(2 * n);
	rep(i, 2 * n) {
		rep(j, n) s[i] += u[j] * v[j];
		v = a * v;
	}
	
	vm c;
	berlekamp_massey(s, c);

	f = MFPS(c);
	f = (1 - (f >> 1)).rev();
}


//【ヘッセンベルグ縮約】O(n^3)
/*
* 正方行列 A = a[0..n)[0..n) を相似な上ヘッセンベルグ行列 H = P^(-1) A P に書き換える．
* 上ヘッセンベルグ行列とは，対角の 2 つ下以下の成分が全て 0 であるような行列である．
*/
template <class T> void hessenberg_reduction(Matrix<T>& a) {
	// 参考 : https://hitonanode.github.io/cplib-cpp/linear_algebra_matrix/characteristic_poly.hpp
	// verify : https://judge.yosupo.jp/problem/characteristic_polynomial

	//【方法】
	// 基本的にはガウスの消去法であるが，相似変換でなければならないので工夫をする．
	// 
	// ガウスの消去法なら最初は 1 行目を何倍かして r(r > 1) 行目に足し込むが，
	// 相似変換では同時に r 列目が何倍かされて 1 列目から引かれてしまい，
	// せっかくの 1 列目に作った 0 が台無しになる．
	//
	// そこで，2 行目を何倍かして r(r > 2) 行目に足し込むことにすれば，
	// 同時に r 列目が何倍かされて 2 列目から引かれてしまっても 1 列目の 0 は無事である．
	// これを最後まで繰り返せば良い．

	//【注意】
	// K が代数閉体なら T = P^(-1) A P を上三角行列にすることも可能ではあるが，
	// それは A の固有値を求めることと同等に難しい．

	const int n = a.n;

	repi(r, 0, n - 3) {
		int k = r + 1;
		while (k < n) {
			if (a[k][r] != 0) break;
			k++;
		}
		if (k == n) continue;

		if (k != r + 1) {
			rep(i, n) swap(a[r + 1][i], a[k][i]);
			rep(i, n) swap(a[i][r + 1], a[i][k]);
		}

		T r_inv = T(1) / a[r + 1][r];
		repi(i, r + 2, n - 1) {
			T t = a[i][r] * r_inv;
			rep(j, n) a[i][j] -= a[r + 1][j] * t;
			rep(j, n) a[j][r + 1] += a[j][i] * t;
		}
	}
}


//【特性多項式】O(n^3)
/*
* 正方行列 A = a[0..n)[0..n) の特性多項式 |xI - A| を f に格納する．
*
* 利用：【形式的冪級数（mint）】，【ヘッセンベルグ縮約】
*/
void characteristic_polynomial(Matrix<mint> a, MFPS& f) {
	// verify : https://judge.yosupo.jp/problem/characteristic_polynomial

	//【方法】
	// A を相似な上ヘッセンベルグ行列に縮約しておく（相似なので特性多項式は不変）
	// xI - A の首座小行列式を，最右列で余因子展開しながら再帰的に求めていく．

	int n = a.n;
	hessenberg_reduction(a);

	// acc[i][j] : Πk=[i..j] a[k][k-1]（対角の 1 つ下の累積積）
	vvm acc(n, vm(n));
	repi(i, 1, n - 1) {
		acc[i][i] = a[i][i - 1];
		repi(j, i + 1, n - 1) acc[i][j] = acc[i][j - 1] * a[j][j - 1];
	}

	// dp[j] : xI - A の j * j 首座小行列式
	vector<MFPS> dp(n + 1);
	dp[0] = MFPS(1);

	repi(j, 1, n) {
		rep(i, j - 1) dp[j] -= dp[i] * a[i][j - 1] * acc[i + 1][j - 1];
		dp[j] += dp[j - 1] * MFPS(vm{ -a[j - 1][j - 1], 1 });
	}

	f = dp[n];
}


//【アダマール行列】O(4^n)
/*
* 2^n * 2^n のアダマール行列を mat に格納する．
*/
void hadamard_matrix(int n, vvi& mat) {
	// verify : https://atcoder.jp/contests/keyence2021/tasks/keyence2021_d

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


