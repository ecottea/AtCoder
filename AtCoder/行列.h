#pragma once
#include "header.h"
#include "FPS(mint).h"
#include "SPS.h"
// ■■■■■ 行列 ■■■■■


//【行列】
/*
* Matrix<T>(int n, int m) : O(n m)
*	n×m 零行列で初期化する．
*
* Matrix<T>(int n) : O(n^2)
*	n×n 単位行列で初期化する．
*
* Matrix<T>(vvT a) : O(n m)
*	二次元配列 a[0..n)[0..m) の要素で初期化する．
*
* bool empty() : O(1)
*	行列が空かを返す．
*
* A + B : O(n m)
*	n×m 行列 A, B の和を返す．+= も使用可．
*
* A - B : O(n m)
*	n×m 行列 A, B の差を返す．-= も使用可．
*
* c * A ／ A * c : O(n m)
*	n×m 行列 A とスカラー c のスカラー積を返す．*= も使用可．
*
* A * x : O(n m)
*	n×m 行列 A と n 次元列ベクトル x の積を返す．
*
* x * A : O(n m)
*	m 次元行ベクトル x と n×m 行列 A の積を返す．
*
* A * B : O(n m l)
*	n×m 行列 A と m×l 行列 B の積を返す．
*
* Mat pow(ll d) : O(n^3 log d)
*	自身を d 乗した行列を返す．
*/
template <class T>
struct Matrix {
	int n, m; // 行列のサイズ（n 行 m 列）
	vector<vector<T>> v; // 行列の成分

	// n×m 零行列で初期化する．
	Matrix(int n, int m) : n(n), m(m), v(n, vector<T>(m)) {}

	// n×n 単位行列で初期化する．
	Matrix(int n) : n(n), m(n), v(n, vector<T>(n)) { rep(i, n) v[i][i] = T(1); }

	// 二次元配列 a[0..n)[0..m) の要素で初期化する．
	Matrix(const vector<vector<T>>& a) : n(sz(a)), m(sz(a[0])), v(a) {}
	Matrix() : n(0), m(0) {}

	// 代入
	Matrix(const Matrix&) = default;
	Matrix& operator=(const Matrix&) = default;

	// アクセス
	inline vector<T> const& operator[](int i) const { return v[i]; }
	inline vector<T>& operator[](int i) {
		// verify : https://judge.yosupo.jp/problem/matrix_product

		// inline を付けて [] でアクセスするとなぜか v[] への直接アクセスより速くなった．
		return v[i];
	}

	// 入力
	friend istream& operator>>(istream& is, Matrix& a) {
		rep(i, a.n) rep(j, a.m) is >> a.v[i][j];
		return is;
	}
	
	// 行の追加
	void push_back(const vector<T>& a) {
		Assert(sz(a) == m);
		v.push_back(a);
		n++;
	}

	// 行の削除
	void pop_back() {
		Assert(n > 0);
		v.pop_back();
		n--;
	}
	void resize(int n_) {
		v.resize(n_);
		n = n_;
	}

	// 空か
	bool empty() const { return min(n, m) == 0; }

	// 比較
	bool operator==(const Matrix& b) const { return n == b.n && m == b.m && v == b.v; }
	bool operator!=(const Matrix& b) const { return !(*this == b); }

	// 加算，減算，スカラー倍
	Matrix& operator+=(const Matrix& b) {
		rep(i, n) rep(j, m) v[i][j] += b[i][j];
		return *this;
	}
	Matrix& operator-=(const Matrix& b) {
		rep(i, n) rep(j, m) v[i][j] -= b[i][j];
		return *this;
	}
	Matrix& operator*=(const T& c) {
		rep(i, n) rep(j, m) v[i][j] *= c;
		return *this;
	}
	Matrix operator+(const Matrix& b) const { return Matrix(*this) += b; }
	Matrix operator-(const Matrix& b) const { return Matrix(*this) -= b; }
	Matrix operator*(const T& c) const { return Matrix(*this) *= c; }
	friend Matrix operator*(const T& c, const Matrix<T>& a) { return a * c; }
	Matrix operator-() const { return Matrix(*this) *= T(-1); }

	// 行列ベクトル積 : O(m n)
	vector<T> operator*(const vector<T>& x) const {
		vector<T> y(n);
		rep(i, n) rep(j, m)	y[i] += v[i][j] * x[j];
		return y;
	}

	// ベクトル行列積 : O(m n)
	friend vector<T> operator*(const vector<T>& x, const Matrix& a) {
		vector<T> y(a.m);
		rep(i, a.n) rep(j, a.m) y[j] += x[i] * a[i][j];
		return y;
	}

	// 積：O(n^3)
	Matrix operator*(const Matrix& b) const {
		// verify : https://judge.yosupo.jp/problem/matrix_product

		Matrix res(n, b.m);
		rep(i, res.n) rep(k, m) rep(j, res.m) res[i][j] += v[i][k] * b[k][j];
		return res;
	}
	Matrix& operator*=(const Matrix& b) { *this = *this * b; return *this; }

	// 累乗：O(n^3 log d)
	Matrix pow(ll d) const {
		// verify : https://judge.yosupo.jp/problem/pow_of_matrix

		Matrix res(n), pow2 = *this;
		while (d > 0) {
			if (d & 1) res *= pow2;
			pow2 *= pow2;
			d >>= 1;
		}
		return res;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Matrix& a) {
		rep(i, a.n) {
			os << "[";
			rep(j, a.m) os << a[i][j] << " ]"[j == a.m - 1];
			if (i < a.n - 1) os << "\n";
		}
		return os;
	}
#endif
};


//【正方行列（固定サイズ）】
/*
* Fixed_matrix<T, n>() : O(n^2)
*	T の要素を成分にもつ n×n 零行列で初期化する．
*
* Fixed_matrix<T, n>(bool identity = true) : O(n^2)
*	T の要素を成分にもつ n×n 単位行列で初期化する．
*
* Fixed_matrix<T, n>(vvT a) : O(n^2)
*	二次元配列 a[0..n)[0..n) の要素で初期化する．
*
* A + B : O(n^2)
*	n×n 行列 A, B の和を返す．+= も使用可．
*
* A - B : O(n^2)
*	n×n 行列 A, B の差を返す．-= も使用可．
*
* c * A ／ A * c : O(n^2)
*	n×n 行列 A とスカラー c のスカラー積を返す．*= も使用可．
*
* A * x : O(n^2)
*	n×n 行列 A と n 次元列ベクトル array<T, n> x の積を返す．
*
* x * A : O(n^2)
*	n 次元行ベクトル array<T, n> x と n×n 行列 A の積を返す．
*
* A * B : O(n^3)
*	n×n 行列 A と n×n 行列 B の積を返す．
*
* Mat pow(ll d) : O(n^3 log d)
*	自身を d 乗した行列を返す．
*/
template <class T, int n>
struct Fixed_matrix {
	array<array<T, n>, n> v; // 行列の成分

	// n×n 零行列で初期化する．identity = true なら n×n 単位行列で初期化する．
	Fixed_matrix(bool identity = false) {
		rep(i, n) v[i].fill(T(0));
		if (identity) rep(i, n) v[i][i] = T(1);
	}

	// 二次元配列 a[0..n)[0..n) の要素で初期化する．
	Fixed_matrix(const vector<vector<T>>& a) {
		// verify : https://yukicoder.me/problems/no/1000

		Assert(sz(a) == n && sz(a[0]) == n);
		rep(i, n) rep(j, n) v[i][j] = a[i][j];
	}

	// 代入
	Fixed_matrix(const Fixed_matrix&) = default;
	Fixed_matrix& operator=(const Fixed_matrix&) = default;

	// アクセス
	inline array<T, n> const& operator[](int i) const { return v[i]; }
	inline array<T, n>& operator[](int i) { return v[i]; }

	// 入力
	friend istream& operator>>(istream& is, Fixed_matrix& a) {
		rep(i, n) rep(j, n) is >> a[i][j];
		return is;
	}

	// 比較
	bool operator==(const Fixed_matrix& b) const { return v == b.v; }
	bool operator!=(const Fixed_matrix& b) const { return !(*this == b); }

	// 加算，減算，スカラー倍
	Fixed_matrix& operator+=(const Fixed_matrix& b) {
		rep(i, n) rep(j, n) v[i][j] += b[i][j];
		return *this;
	}
	Fixed_matrix& operator-=(const Fixed_matrix& b) {
		rep(i, n) rep(j, n) v[i][j] -= b[i][j];
		return *this;
	}
	Fixed_matrix& operator*=(const T& c) {
		rep(i, n) rep(j, n) v[i][j] *= c;
		return *this;
	}
	Fixed_matrix operator+(const Fixed_matrix& b) const { return Fixed_matrix(*this) += b; }
	Fixed_matrix operator-(const Fixed_matrix& b) const { return Fixed_matrix(*this) -= b; }
	Fixed_matrix operator*(const T& c) const { return Fixed_matrix(*this) *= c; }
	friend Fixed_matrix operator*(const T& c, const Fixed_matrix& a) { return a * c; }
	Fixed_matrix operator-() const { return Fixed_matrix(*this) *= T(-1); }

	// 行列ベクトル積 : O(n^2)
	array<T, n> operator*(const array<T, n>& x) const {
		array<T, n> y{ 0 };
		rep(i, n) rep(j, n)	y[i] += v[i][j] * x[j];
		return y;
	}

	// ベクトル行列積 : O(n^2)
	friend array<T, n> operator*(const array<T, n>& x, const Fixed_matrix& a) {
		array<T, n> y{ 0 };
		rep(i, n) rep(j, n) y[j] += x[i] * a[i][j];
		return y;
	}

	// 積：O(n^3)
	Fixed_matrix operator*(const Fixed_matrix& b) const {
		// verify : https://yukicoder.me/problems/no/1000

		Fixed_matrix res;
		rep(i, n) rep(k, n) rep(j, n) res[i][j] += v[i][k] * b[k][j];
		return res;
	}
	Fixed_matrix& operator*=(const Fixed_matrix& b) { *this = *this * b; return *this; }

	// 累乗：O(n^3 log d)
	Fixed_matrix pow(ll d) const {
		// verify : https://yukicoder.me/problems/no/2810

		Fixed_matrix res(true), pow2(*this);
		while (d > 0) {
			if (d & 1) res *= pow2;
			pow2 *= pow2;
			d /= 2;
		}
		return res;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Fixed_matrix& a) {
		rep(i, n) {
			os << "[";
			rep(j, n) os << a[i][j] << " ]"[j == n - 1];
			if (i < n - 1) os << "\n";
		}
		return os;
	}
#endif
};


//【階段行列】O(n^2 m)
/*
* 行基本変形で n×m 行列 mat を階段行列に変形する．
* 最も右下のピボットの位置 (i, j) を返す．零行列なら (-1, -1) を返す．
*/
template <class T>
pii reduced_row_echelon_form(Matrix<T>& mat) {
	// verify : https://judge.yosupo.jp/problem/matrix_rank

	int n = mat.n, m = mat.m;
	auto& v = mat.v;
	
	// 直前に見つけたピボットの位置
	int pi = -1, pj = -1;

	// 注目位置を (i, j)（i 行目かつ j 列目）とする．
	int i = 0, j = 0;

	while (i < n && j < m) {
		// 同じ列の下方の行から非 0 成分を見つける．
		int i2 = i;
		while (i2 < n && v[i2][j] == 0) i2++;

		// 見つからなかったら注目位置を右に移す．
		if (i2 == n) {
			j++;
			continue;
		}

		// 見つかったら第 i 行とその行を入れ替える．
		pi = i; pj = j;
		if (i != i2) swap(v[i], v[i2]);

		// v[i][j] が 1 になるよう行全体を v[i][j] で割る．
		T vij_inv = T(1) / v[i][j];
		repi(j2, j, m - 1) v[i][j2] *= vij_inv;
		
		// v[i][j] より下方の行の成分が全て 0 になるよう i 行目を定数倍して減じる．
		repi(i2, i + 1, n - 1) {
			T mul = v[i2][j];
			repi(j2, j, m - 1) v[i2][j2] -= v[i][j2] * mul;
		}

		//// v[i][j] より上方の行の成分も全て 0 にしたい場合はこれも実行する．
		//repi(i2, 0, i - 1) {
		//	T mul = v[i2][j];
		//	repi(j2, j, m - 1) v[i2][j2] -= v[i][j2] * mul;
		//}

		// 注目位置を右下に移す．
		i++; j++;
	}

	return { pi, pj };
}


//【線形方程式】O(n m min(n, m))
/*
* 与えられた n×m 行列 A と n 次元ベクトル b に対し，
* 線形方程式 A x = b の特殊解 x0（m 次元ベクトル）を返す（なければ空リスト）
* また同次形 A x = 0 の解空間の基底（m 次元ベクトル）のリストを xs に格納する．
*/
template <class T>
vector<T> gauss_jordan_elimination(const Matrix<T>& A, const vector<T>& b, vector<vector<T>>* xs = nullptr) {
	// verify : https://judge.yosupo.jp/problem/system_of_linear_equations

	int n = A.n, m = A.m;

	// v : 拡大係数行列 (A | b)
	vector<vector<T>> v(n, vector<T>(m + 1));
	rep(i, n) rep(j, m) v[i][j] = A[i][j];
	rep(i, n) v[i][m] = b[i];

	// pivots[i] : 第 i 行のピボットが第何列にあるか
	vi pivots;
		
	// 注目位置を v[i][j] とする．
	int i = 0, j = 0;

	while (i < n && j <= m) {
		// 注目列の下方の行から非 0 成分を見つける．
		int i2 = i;
		while (i2 < n && v[i2][j] == 0) i2++;

		// 見つからなかったら注目位置を右に移す．
		if (i2 == n) { j++; continue; }

		// 見つかったら第 i 行とその行を入れ替える．
		if (i != i2) swap(v[i], v[i2]);

		// v[i][j] をピボットに選択する．
		pivots.push_back(j);

		// v[i][j] が 1 になるよう第 i 行全体を v[i][j] で割る．
		T vij_inv = T(1) / v[i][j];
		repi(j2, j, m) v[i][j2] *= vij_inv;

		// 第 i 行以外の第 j 列の成分が全て 0 になるよう第 i 行を定数倍して減じる．
		rep(i2, n) {
			if (v[i2][j] == T(0) || i2 == i) continue;

			T mul = v[i2][j];
			repi(j2, j, m) v[i2][j2] -= v[i][j2] * mul;
		}

		// 注目位置を右下に移す．
		i++; j++;
	}

	// 最後に見つかったピボットの位置が第 m 列ならば解なし．
	if (!pivots.empty() && pivots.back() == m) return vector<T>();

	// A x = b の特殊解 x0 の構成（任意定数は全て 0 にする）
	vector<T> x0(m);
	int rnk = sz(pivots);
	rep(i, rnk) x0[pivots[i]] = v[i][m];

	// 同次形 A x = 0 の一般解 {x} の基底の構成（任意定数を 1-hot にする）
	if (xs != nullptr) {
		xs->clear();

		int i = 0;
		rep(j, m) {
			if (i < rnk && j == pivots[i]) {
				i++;
				continue;
			}

			vector<T> x(m);
			x[j] = 1;
			rep(i2, i) x[pivots[i2]] = -v[i2][j];
			xs->emplace_back(move(x));
		}
	}

	return x0;
}


//【線形方程式（下ヘッセンベルグ行列）】O(n^2)
/*
* 与えられた n 次下ヘッセンベルグ行列 L と n 次元ベクトル b に対し，
* 線形方程式 L x = b の特殊解 x0（n 次元ベクトル）を返す（なければ空リスト）
* 下ヘッセンベルグ行列とは，対角の 2 つ上以上の成分が全て 0 であるような行列である．
*/
template <class T>
vector<T> gauss_jordan_elimination_Lhessenberg(const Matrix<T>& L, const vector<T>& b) {
	// verify : https://atcoder.jp/contests/abc249/tasks/abc249_h

	int n = sz(b);

	// v : 拡大係数行列 (L | b)
	vector<vector<T>> v(n, vector<T>(n + 1));
	rep(i, n) rep(j, n) v[i][j] = L[i][j];
	rep(i, n) v[i][n] = b[i];

	// ピボットの位置を記録しておくリスト
	vector<pii> pivots;

	// 未確定の列を記録しておくリスト
	list<int> rmd;
	repi(j, 0, n) rmd.push_back(j);

	// 下ヘッセンベルグ性を保つため，行の交換は行わずに基本変形していく．
	rep(i, n) {
		// i 行目の係数を左から走査し非 0 を見つける．
		auto it = rmd.begin();
		for (; it != rmd.end(); it++) if (v[i][*it] != T(0)) break;

		// 全てが 0 なら無視
		if (it == rmd.end()) continue;
		int j = *it;

		// 定数項のみが非 0 なら解なし
		if (j == n) return vector<T>();
		rmd.erase(it);
		pivots.emplace_back(i, j);

		// v[i][j] が 1 になるよう行全体を v[i][j] で割る．
		T vij_inv = T(1) / v[i][j];
		repi(j2, j, n) v[i][j2] *= vij_inv;

		// j 列目に見つかったら他の行の j 列目を全て 0 にする．
		rep(i2, n) {
			if (v[i2][j] == T(0) || i2 == i) continue;

			T mul = v[i2][j];
			repi(j2, j, i + 1) v[i2][j2] -= v[i][j2] * mul; // ここのループ回数が減る
			v[i2][n] -= v[i][n] * mul;
		}
	}

	// 解の例の構成（任意定数は全て 0 にする）
	vector<T> sol(n, T(0));
	repe(p, pivots) sol[p.second] = v[p.first][n];

	return sol;
}


//【線形方程式（多重対角行列）】O(n k^2)
/*
* (i,j) 成分が a[i][k-i+j] であるような n 次 2k+1 重対角行列 D と b[0..n) について，
* 線形方程式 D x = b の唯一解を返す（不定や不能の場合は空リストを返す）
*/
template <class T>
vector<T> gauss_jordan_elimination_diagonal(vector<vector<T>> a, vector<T> b) {
	// verify : https://atcoder.jp/contests/indeednow-finala-open/tasks/indeednow_2015_finala_e

	//【例】
	//	a = [-, 3, 1]
	//	    [4, 1, 5]
	//	    [9, 2, 6]
	//	    [5, 3, -]
	// が表す 3 重対角行列 D は
	//	D = [3 1 0 0]
	//	    [4 1 5 0]
	//	    [0 9 2 6]
	//	    [0 0 5 3]
	// である．

	int n = sz(a), k = sz(a[0]) / 2;
	vector<T> sol(n);

	// p[i] : i 行目のピボットのある列
	vi p(n);

	rep(i, n) {
		// i 行目の係数を左から走査し非 0 を見つける．
		int j = i - k, j_max = min(n - 1, i + k);
		for (; j <= j_max; j++) if (a[i][k - i + j] != T(0)) break;

		// 非 0 成分が無かった場合は不定または不能
		if (j > j_max) return vector<T>();
		p[i] = j;

		// A[i][j] が 1 になるよう行全体を A[i][j] で割る．
		T inv = T(1) / a[i][k - i + j];
		repi(j2, j, j_max) a[i][k - i + j2] *= inv;
		b[i] *= inv;

		// A[i][j] より下の行の j 列目を全て 0 にする．
		int i_max = min(n - 1, j + k);
		repi(i2, i + 1, i_max) {
			if (a[i2][k - i2 + j] == T(0)) continue;

			T mul = a[i2][k - i2 + j];
			repi(j2, j, j_max) a[i2][k - i2 + j2] -= a[i][k - i + j2] * mul;
			b[i2] -= b[i] * mul;
		}
	}

	// p_inv[j] : j 列目のピボットのある行
	vi p_inv(n);
	rep(i, n) p_inv[p[i]] = i;

	// 解の構成
	repir(j, n - 1, 0) {
		int i = p_inv[j];

		sol[i] = b[i];

		int j_max = min(n - 1, i + k);
		repi(j2, j + 1, j_max) sol[i] -= sol[j2] * a[i][k - i + j2];
	}

	return sol;
}


//【functional graph 上の線形方程式】
/*
* 与えられた V=[0..n) 上の functional graph i→f[i] に対し，線形方程式
*	x[i] = a[i] x[f[i]] + b[i]  (i∈[0..n))
* の解 x[0..n) を返す．
*
* 利用：【一次多項式】
*/
template <class T>
vd functional_equation(const vi& f, const vector<T>& a, const vector<T>& b) {
	// verify : https://projecteuler.net/problem=863

	int n = sz(f);

	vector<Poly1<T>> dp(n);

	// id[s] : s を探索したときの探索開始地点
	vi id(n, -1);
	rep(s, n) {
		if (id[s] != -1) continue;

		int v = s;
		while (id[v] == -1) {
			id[v] = s;
			v = f[v];
		}

		if (id[v] == s) {
			id[v] = INF;
			dp[v] = Poly1<T>(1, 0);
		}
	}

	// id[s]=s なる頂点の値を z とおいて，他の頂点の値を z を用いて表す．
	function<Poly1<T>(int)> rf = [&](int s) {
		if (id[s] == INF) return dp[s];
		id[s] = -1;

		return dp[s] = a[s] * rf(f[s]) + b[s];
	};
	rep(s, n) if (id[s] != -1) rf(s);

	vd res(n);

	// d[s]=s なる頂点において一次方程式を解いて z の値を求め，他の頂点の値を特定する．
	function<double(int)> rf2 = [&](int s) {
		if (id[s] == INF) return res[s] = dp[s].solve(a[s] * dp[f[s]] + b[s]);
		id[s] = -2;

		return res[s] = a[s] * rf2(f[s]) + b[s];
	};
	rep(s, n) if (id[s] != -2) rf2(s);

	return res;
}


//【行列式】O(n^3)
/*
* n 次正方行列 mat の行列式を返す．
*/
template <class T>
T determinant(const Matrix<T>& mat) {
	// verify : https://judge.yosupo.jp/problem/matrix_det

	int n = mat.n; auto v = mat.v;

	// 注目位置を (i, j)（i 行目かつ j 列目）とする．
	int i = 0, j = 0;

	// 行列式の値
	T res(1);

	while (i < n && j < n) {
		// 同じ列の下方の行から非 0 成分を見つける．
		int i2 = i;
		while (i2 < n && v[i2][j] == T(0)) i2++;

		// 見つからなかったら零列ベクトルを含むので行列式は 0 である．
		if (i2 == n) return T(0);

		// 見つかったら i 行目とその行を入れ替え，行列式の値は -1 倍しておく．
		if (i2 != i) {
			swap(v[i], v[i2]);
			res *= T(-1);
		}

		// v[i][j] が 1 になるよう行全体を v[i][j] で割り，行列式の値は v[i][j] 倍しておく．
		res *= v[i][j];
		T vij_inv = T(1) / v[i][j];
		repi(j2, j, n - 1) v[i][j2] *= vij_inv;

		// v[i][j] より下方の行の成分が全て 0 になるよう i 行目を定数倍して減じる（行列式の値は変化しない）
		repi(i2, i + 1, n - 1) {
			T mul = v[i2][j];
			repi(j2, j, n - 1) v[i2][j2] -= v[i][j2] * mul;
		}

		// 注目位置を右下に移す．
		i++; j++;
	}

	return res;
}


//【行列式（法が任意）】O(n^3 log(mat))
/*
* n 次正方行列 mat の行列式を返す．
*/
mint determinant_arbitrary_mod(const Matrix<mint>& mat) {
	// verify : https://judge.yosupo.jp/problem/matrix_det_arbitrary_mod

	int n = mat.n; auto v = mat.v;

	// 注目位置を (i, j)（i 行目かつ j 列目）とする．
	int i = 0, j = 0;

	// 行列式の値
	mint res = 1;

	while (i < n && j < n) {
		// 同じ列の下方の行から非 0 成分を見つける．
		int i2 = i;
		while (i2 < n && v[i2][j] == 0) i2++;

		// 見つからなかったら零列ベクトルを含むので行列式は 0 である．
		if (i2 == n) return 0;

		// 見つかったら i 行目とその行を入れ替え，行列式の値は -1 倍しておく．
		if (i2 != i) {
			swap(v[i], v[i2]);
			res *= -1;
			i2++;
		}

		// v[i][j] が gcd( v[0..n)[j] ) に等しくなるよう調整する．
		int vij = v[i][j].val();
		while (i2 < n) {
			int vi2j = v[i2][j].val();
			int g = gcd(vij, vi2j);
			while (vij != g) {
				int q = vi2j / vij;

				repi(j2, j, n - 1) v[i2][j2] -= q * v[i][j2];
				vi2j -= q * vij;

				res *= -1;
				swap(v[i], v[i2]);
				swap(vij, vi2j);
			}
			i2++;
		}

		// v[i][j] より下方の行の成分が全て 0 になるよう i 行目を定数倍して減じる（行列式の値は変化しない）
		repi(i2, i + 1, n - 1) {
			mint mul = v[i2][j].val() / vij;
			repi(j2, j, n - 1) v[i2][j2] -= v[i][j2] * mul;
		}
		res *= vij;

		// 注目位置を右下に移す．
		i++; j++;
	}

	return res;
}


//【行列式（1 次多項式）】O(n^3)
/*
* 与えられた n 次正方行列 A, B に対し n 次多項式 det(z A + B) を返す．
*
* 制約：fm は n! まで計算可能
*
* 利用：【逆行列】,【行列式】，【特性多項式（mod 998244353）】
*/
MFPS determinant_FPS_1deg(const Matrix<mint>& A, const Matrix<mint>& B, const Factorial_mint& fm) {
	// verify : https://yukicoder.me/problems/no/1907
	
	//【方法】
	// もし A が正則行列だったら，
	//		|z A + B|
	//		= |A A^(-1) (z A + B)|
	//		= |A| |z I + A^(-1) B|
	// となるので，-A^(-1) B の固有多項式を求めて |A| 倍すれば良い．
	// 一般には A が正則とは限らないのでこれは実際には使えない．
	// 
	// もし B が正則行列だったら，先と同様に考えて，
	//		|z A + B|
	//		= |(z A + B) B^(-1) B|
	// 		= |z A B^(-1) + I| |B|
	// 		= |A B^(-1) + z^(-1) I| |B| z^n
	// となるので，-A B^(-1) の固有多項式を求めて係数を反転し，|B| 倍すれば良い．
	// 一般には B も正則とは限らないが，次の工夫により B が正則の場合に帰着できる．
	// 
	// taylor_shift() を使えば |z A + B| を c だけ平行移動した
	//		|(z - c) A + B| = |z A + (B - c A)|
	// さえ求まれば高速に |z A + B| を復元できる．
	// そこで，c をランダムに選んで B - c A が正則になるようにする．
	// もしそのような c が見つからなければ多項式として |z A + B| = 0 である．

	int n = A.n;

	mt19937 mt;
	mt.seed((int)time(NULL));
	uniform_int_distribution<> rnd(0, 998244352);

	mint c; Matrix<mint> B2, B2_inv;
	int i = 5;
	while (i > 0) {
		c = rnd(mt);
		B2 = B - c * A;
		B2_inv = inverse_matrix(B2);
		if (!B2_inv.empty()) break;
		i--;
	}

	// 5 回やってだめなら非正則と判断する．
	if (i == 0) return MFPS(0, n + 1);

	MFPS f = characteristic_polynomial(-A * B2_inv);
	f = f.rev();
	f = taylor_shift(f, c, fm);
	f *= determinant(B2);

	return f;
}


//【行列式（任意）】O(2^n n)
/*
* n 次正方行列 mat の行列式を返す．
*/
template <class T>
T determinant_arbitrary(const Matrix<T>& mat) {
	// verify : https://yukicoder.me/problems/no/2445

	int n = mat.n;

	// dp[set] : 行の集合が set，列の集合が [n-|set|..n) である小行列式
	vector<T> dp(1LL << n);
	dp[0] = 1;

	// より小さい行列式から順に余因子展開で計算していく．
	repi(set, 1, (1 << n) - 1) {
		int j = n - popcount(set);

		rep(i, n) {
			if (!(set & (1 << i))) continue;

			T sign = popcount(((1 << i) - 1) & set) & 1 ? -1 : 1;
			dp[set] += sign * mat[i][j] * dp[set - (1LL << i)];
		}
	}

	return dp[(1LL << n) - 1];
}


//【ヴァンデルモンド行列式】O(n (log n)^2)
/*
* FPS(mint).h の【差積】を用いれば良い．
*/


//【パーマネント】O(2^n n)
/*
* n 次正方行列 mat のパーマネントを返す．
*/
template <class T>
T parmanent(const Matrix<T>& mat) {
	// verify : https://atcoder.jp/contests/dp/tasks/dp_o

	int n = mat.n;

	// dp[set] : 行の集合が set，列の集合が [n-|set|..n) である小パーマネント
	vector<T> dp(1LL << n);
	dp[0] = 1;

	// より小さいパーマネントから順に余因子展開で計算していく．
	repi(set, 1, (1 << n) - 1) {
		int j = n - popcount(set);

		rep(i, n) {
			if (!(set & (1 << i))) continue;

			dp[set] += mat[i][j] * dp[set - (1LL << i)];
		}
	}

	return dp[(1LL << n) - 1];
}


//【ハフニアン】O(2^(n/2) n^2)
/*
* n 次対称行列 a[0..n)[0..n) のハフニアンを返す．
*
* 制約 : n は偶数，対角成分は 0
* 
* 利用：【SPS 指数関数】
*/
mint hafnian(const vvm& a) {
	// 参考 : https://maspypy.com/%e9%9b%86%e5%90%88%e3%81%b9%e3%81%8d%e7%b4%9a%e6%95%b0%e9%96%a2%e9%80%a3-4-%e5%95%8f%e9%a1%8c%e4%be%8b#toc6
	// verify : https://judge.yosupo.jp/problem/hafnian_of_matrix

	//【方法】
	// a を重み付き隣接行列とする重み付き無向グラフ g について，
	// そのマッチングのスコアをマッチングに使われた辺の重みの積と定め，
	// 全てのマッチングをわたるスコアの和を求めれば良い．
	//
	// 仮の辺 e={2e, 2e+1},（e∈[0..n/2)） を追加すれば，
	// g のマッチングと追加辺を合わせると [0..n) のサイクル分解を得る．
	// 各 es⊂[0..n/2) ごとに es を利用して作れる単純サイクルのスコアが求まれば，
	// これを集合冪級数とみなして exp をとることにより [0..n) のサイクル分解全てをわたるスコア和が求まる．

	int n = sz(a);
	int hn = n / 2;

	vm cycle(1LL << hn);

	// e : set に含まれる番号最大の辺
	rep(e, hn) {
		// dp[s][set] : 頂点 s から頂点 2e+1 までの辺集合 set⊂[0..e] を通る単純パスのスコア和
		vvm dp(2 * e + 2, vm(1LL << (e + 1)));
		dp[2 * e][1LL << e] = 1;

		// 貰う DP
		repi(set, 1 << e, (1 << (e + 1)) - 1) {
			// es, et ∈ set なる辺 2es+{0,1} → 2et+{0,1} をチェックする．
			repis(es, set) repis(et, set - (1 << es)) rep(bs, 2) rep(bt, 2) {
				int s = es * 2 + bs, t = et * 2 + bt;
				dp[s][set] += a[s ^ 1][t] * dp[t][set - (1 << es)];
			}
		}

		// 辺集合 set を使う単純パス s→2e+1 に辺 2e+1→s を追加して単純閉路を得る．
		repi(set, 1 << e, (1 << (e + 1)) - 1) repis(es, set) rep(bs, 2) {
			int s = es * 2 + bs;
			cycle[set] += dp[s][set] * a[2 * e + 1][s];
		}
	}

	auto res = exp_sps(cycle);

	return res.back();
}


//【逆行列】O(n^3)
/*
* n 次正方行列 mat の逆行列を返す（存在しなければ空）
*/
template <class T>
Matrix<T> inverse_matrix(const Matrix<T>& mat) {
	// verify : https://judge.yosupo.jp/problem/inverse_matrix

	int n = mat.n;

	// 元の行列 mat と単位行列を繋げた拡大行列 v を作る．
	vector<vector<T>> v(n, vector<T>(2 * n));
	rep(i, n) rep(j, n) {
		v[i][j] = mat[i][j];
		if (i == j) v[i][n + j] = 1;
	}
	int m = 2 * n;

	// 注目位置を (i, j)（i 行目かつ j 列目）とする．
	int i = 0, j = 0;

	// 拡大行列に対して行基本変形を行い，左側を単位行列にすることを目指す．
	while (i < n && j < m) {
		// 同じ列の下方の行から非 0 成分を見つける．
		int i2 = i;
		while (i2 < n && v[i2][j] == T(0)) i2++;

		// 見つからなかったら全て 0 の列があったので mat は非正則
		if (i2 == n) return Matrix<T>();

		// 見つかったら i 行目とその行を入れ替える．
		if (i != i2) swap(v[i], v[i2]);

		// v[i][j] が 1 になるよう行全体を v[i][j] で割る．
		T vij_inv = T(1) / v[i][j];
		repi(j2, j, m - 1) v[i][j2] *= vij_inv;

		// v[i][j] と同じ列の成分が全て 0 になるよう i 行目を定数倍して減じる．
		rep(i2, n) {
			// i 行目だけは引かない．
			if (i2 == i) continue;

			T mul = v[i2][j];
			repi(j2, j, m - 1) v[i2][j2] -= v[i][j2] * mul;
		}

		// 注目位置を右下に移す．
		i++; j++;
	}

	// 拡大行列の右半分が mat の逆行列なのでコピーする．
	Matrix<T> mat_inv(n, n);
	rep(i, n) rep(j, n) mat_inv[i][j] = v[i][n + j];

	return mat_inv;
}


//【階数標準形】O(n m (n + m))
/*
* A = a[0..n)[0..m) を階数標準形 R_r := [I_r, O; O, O] に変換する行列，すなわち
*		P A Q = R_r （r = rank A）
* を満たす正則行列 P, Q を p[0..n)[0..n), q[0..m)[0..m) に格納し，r を返す．
*/
template <class T>
int rank_normal_form(const Matrix<T>& a, Matrix<T>& p, Matrix<T>& q) {
	int n = a.n, m = a.m;

	// 元の行列 mat と単位行列を繋げた拡大行列を作る．
	Matrix<T> v(n + m, n + m);
	rep(i, n) rep(j, m) v[i][j] = a[i][j];
	rep(i, n) rep(j, n) v[i][m + j] = (i == j ? T(1) : T(0));
	rep(i, m) rep(j, m) v[n + i][j] = (i == j ? T(1) : T(0));

	// 直前に見つけたピボットの位置
	int pi = -1, pj = -1;

	// 注目位置を (i, j)（i 行目かつ j 列目）とする．
	int i = 0, j = 0;

	// 拡大行列に対して行基本変形を行い，左側を単位行列にすることを目指す．
	while (i < n && j < m) {
		// 同じ列の下方の行から非 0 成分を見つける．
		int i2 = i;
		while (i2 < n && v[i2][j] == 0) i2++;

		// 見つからなかったら注目位置を右に移す．
		if (i2 == n) { j++; continue; }

		// 見つかったら i 行目とその行を入れ替える．
		pi = i; pj = j;
		if (i != i2) swap(v[i], v[i2]);

		// v[i][j] が 1 になるよう行全体を v[i][j] で割る．
		T vij_inv = T(1) / v[i][j];
		repi(j2, j, n + m - 1) v[i][j2] *= vij_inv;

		// v[i][j] と同じ列の成分が全て 0 になるよう i 行目を定数倍して減じる．
		rep(i2, n) {
			// i 行目だけは引かない．
			if (i2 == i) continue;

			T mul = v[i2][j];
			repi(j2, j, n + m - 1) v[i2][j2] -= v[i][j2] * mul;
		}

		// 注目位置を右下に移す．
		i++; j++;
	}

	// 直前に見つけたピボットの位置
	pi = -1; pj = -1;

	// 注目位置を (i, j)（i 行目かつ j 列目）とする．
	i = 0; j = 0;

	// 続けて拡大行列に対して列基本変形を行い，上側を単位行列にすることを目指す．
	while (i < n && j < m) {
		// 同じ行の右方の列から非 0 成分を見つける．
		int j2 = j;
		while (j2 < m && v[i][j2] == 0) j2++;

		// 見つからなかったら注目位置を下に移す．
		if (j2 == m) { i++; continue; }

		// 見つかったら j 列目とその列を入れ替える．
		pi = i; pj = j;
		if (j != j2) rep(i2, n + m) swap(v[i2][j], v[i2][j2]);

		// v[i][j] が 1 になるよう列全体を v[i][j] で割る．
		T div = T(1) / v[i][j];
		repi(i2, i, n + m - 1) v[i2][j] *= div;

		// v[i][j] と同じ行の成分が全て 0 になるよう j 列目を定数倍して減じる．
		rep(j2, m) {
			// j 列目だけは引かない．
			if (j2 == j) continue;

			T mul = v[i][j2];
			repi(i2, 0, n + m - 1) v[i2][j2] -= v[i2][i] * mul;
		}

		// 注目位置を右下に移す．
		i++; j++;
	}

	// 拡大行列の右側が P, 下側が Q なのでコピーする．
	p = Matrix<T>(n, n); q = Matrix<T>(m, m);
	rep(i, n) rep(j, n) p[i][j] = v[i][m + j];
	rep(i, m) rep(j, m) q[i][j] = v[n + i][j];

	return pi + 1;
}


//【最小多項式】O(n^3)
/*
* n 次正方行列 A = a[0..n)[0..n) の最小多項式を返す．
* 最小多項式とは，f(A) = O を満たす次数最小なモニック多項式である．
*
* 利用：【線形漸化式の発見】
*/
MFPS minimal_polynomial(const Matrix<mint>& a) {
	// 参考 : https://yukicoder.me/wiki/black_box_linear_algebra

	int n = a.n;

	mt19937 mt;
	mt.seed((int)time(NULL));
	uniform_int_distribution<> rnd(0, mint::mod() - 1);

	vm u(n), v(n);
	rep(i, n) {
		u[i] = rnd(mt);
		v[i] = rnd(mt);
	}

	vm s(2 * n);
	rep(i, 2 * n) {
		rep(j, n) s[i] += u[j] * v[j];
		v = a * v; // ここが O(n^2) かかり，それを O(n) 回ループするので O(n^3)
	}
	
	vm c = berlekamp_massey(s);

	MFPS f(c);
	f = (1 - (f >> 1)).rev();

	return f;
}


//【ヘッセンベルグ縮約】O(n^3)
/*
* 正方行列 A = a[0..n)[0..n) を相似な上ヘッセンベルグ行列 H = P^(-1) A P に書き換える．
* 上ヘッセンベルグ行列とは，対角の 2 つ下以下の成分が全て 0 であるような行列である．
*/
template <class T>
void hessenberg_reduction(Matrix<T>& a) {
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


//【特性多項式（mod 998244353）】O(n^3)
/*
* 正方行列 A = a[0..n)[0..n) の特性多項式 |zI - A| を返す．
*
* 利用：【形式的冪級数】,【ヘッセンベルグ縮約】
*/
MFPS characteristic_polynomial(Matrix<mint> a) {
	// verify : https://judge.yosupo.jp/problem/characteristic_polynomial

	//【方法】
	// A を相似な上ヘッセンベルグ行列に縮約しておく（相似なので特性多項式は不変）
	// zI - A の首座小行列式を，最右列で余因子展開しながら再帰的に求めていく．

	int n = a.n;
	hessenberg_reduction(a);

	// acc[i][j] : Πk=[i..j] a[k][k-1]（対角の 1 つ下の累積積）
	vvm acc(n, vm(n));
	repi(i, 1, n - 1) {
		acc[i][i] = a[i][i - 1];
		repi(j, i + 1, n - 1) acc[i][j] = acc[i][j - 1] * a[j][j - 1];
	}

	// dp[j] : zI - A の j*j 首座小行列式
	vector<MFPS> dp(n + 1);
	dp[0] = MFPS(1);

	repi(j, 1, n) {
		rep(i, j - 1) dp[j] -= dp[i] * a[i][j - 1] * acc[i + 1][j - 1];
		dp[j] += dp[j - 1] * MFPS(vm{ -a[j - 1][j - 1], 1 });
	}

	return dp[n];
}


//【アダマール行列】O(4^n)
/*
* 2^n * 2^n のアダマール行列を返す．
*/
vvi hadamard_matrix(int n) {
	// verify : https://atcoder.jp/contests/keyence2021/tasks/keyence2021_d

	vvi mat(1LL << n, vi(1LL << n));

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

	return mat;
}


//【行列のクロネッカー積】O(n1 n2 m1 m2)
/*
* 行列 a[0..n1)[0..m1) と b[0..n2)[0..m2) のクロネッカー積を返す．
*/
template <class T>
Matrix<T> kronecker_product(const Matrix<T>& a, const Matrix<T>& b) {
	Matrix<T> res(a.n * b.n, a.m * b.m);
	rep(i1, a.n) rep(j1, a.m) rep(i2, b.n) rep(j2, b.m) {
		res[i1 * b.n + i2][j1 * b.m + j2] = a[i1][j1] * b[i2][j2];
	}

	return res;
}


//【行列のクロネッカー積とベクトルとの積】O(k n m)（n : mats の行列の最大サイズ）
/*
* 行列の列 mats[0..k) のクロネッカー積を M とし，ベクトル M vec[0..m) を返す．
*/
template <class T>
vector<T> kronecker_matrix_vector_product(const vector<Matrix<T>>& mats, vector<T> vec) {
	// verify : https://atcoder.jp/contests/arc151/tasks/arc151_d

	//【方法】
	// k=2 で，
	//		mats[0] = [a00, a01], mats[1] = B, vec = [vx0]
	//		          [a10, a11]                     [vx1]
	// の場合を考える．
	//
	// 行列のクロネッカー積を先に計算し，ブロック積を用いて M vec を計算すると，
	//		[a00 B, a01 B] [vx0] = [a00 B vx0 + a01 B vx1]
	//		[a10 B, a11 B] [vx1] = [a10 B vx0 + a11 B vx1]
	// となる．一方これは B と vec のブロック積を先に計算することにより
	//		[a00, a01] [B vx0] 
	//		[a10, a11] [B vx1]
	// とも表される．
	//
	// このように右から順に行列ベクトル積を計算していけば，巨大な行列になりうる M を陽に求める必要はない．

	int K = sz(mats);

	// Ws : vec が大きさ Ws の小ブロックに分割されていることを表す
	int Ws = 1;

	repir(k, K - 1, 0) {
		auto& mat = mats[k];

		// Wl : vec が大きさ Wl の大ブロックに分割されていることを表す
		int Wl = Ws * mat.m;

		vector<T> nvec;

		// tl : 上から何個目の大ブロックを計算しているか
		rep(tl, sz(vec) / Wl) {
			// ts : 大ブロック内で上から何個目の小ブロックを計算しているか
			rep(ts, mat.n) {
				// i : 小ブロック内の何行目を計算しているか
				rep(i, Ws) {
					T val = 0;
					rep(j, mat.m) {
						val += mat[ts][j] * vec[tl * Wl + i + j * Ws];
					}
					nvec.emplace_back(val);
				}
			}
		}

		Ws *= mat.n;

		vec = move(nvec);
	}

	return vec;
}


//【テプリッツ行列の累乗（mod 998244353）】O(n log n log d)
/*
* 左下から右上までの成分が順に a(-n..n) であるテプリッツ行列を d 乗したテプリッツ行列を返す．
* 
* 制約：積もテプリッツ行列
*/
vm toeplitz_pow(const vm& a, ll d) {
	int n = (sz(a) + 1) / 2;

	// テプリッツ行列 a, b の積を返す（制約 : 結果もテプリッツ行列）
	auto mul = [&](const vm& a, const vm& b) {
		vm bl(n), br(n);
		rep(i, n) bl[i] = b[i];
		rep(i, n) br[i] = b[n - 1 + i];

		auto resl = convolution(a, bl);
		auto resr = convolution(a, br);

		vm res(2 * n - 1);
		rep(i, n) res[i] = resl[n - 1 + i];
		repi(i, 1, n - 1) res[n - 1 + i] = resr[n - 1 + i];

		return res;
	};

	vm res(2 * n - 1), pow2 = a;
	res[n - 1] = 1;

	// ダブリングでテプリッツ行列を累乗する．
	while (d > 0) {
		if (d & 1) res = mul(res, pow2);
		pow2 = mul(pow2, pow2);
		d /= 2;
	}

	return res;
}


