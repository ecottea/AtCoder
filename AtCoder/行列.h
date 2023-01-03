#pragma once
#include "header.h"
#include "FPS(mint).h"
// ■■■■■ 行列 ■■■■■


//【行列】
/*
* Matrix<T>(m, n) : O(m n)
*	m * n 零行列で初期化する．
*
* Matrix<T>(n) : O(n^2)
*	n * n 単位行列で初期化する．
*
* Matrix<T>(vvT a) : O(m n)
*	配列 a の要素で初期化する．
*
* bool empty() : O(1)
*	行列が空かを返す．
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
* Mat pow(ll d) : O(n^3 log d)
*	自身を d 乗した行列を返す．
*/
template <class T>
struct Matrix {
	int m, n; // 行列のサイズ（m 行 n 列）
	vector<vector<T>> v; // 行列の成分

	// コンストラクタ（初期化なし，零行列，単位行列，二次元配列）
	Matrix() : m(0), n(0) {}
	Matrix(const int& m_, const int& n_) : m(m_), n(n_), v(m_, vector<T>(n_)) {}
	Matrix(const int& n_) : m(n_), n(n_), v(n_, vector<T>(n_)) { rep(i, n) v[i][i] = T(1); }
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

	// 空か
	bool empty() { return min(m, n) == 0; }

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
			os << "[";
			rep(j, a.n) os << a.v[i][j] << (j < a.n - 1 ? " " : "]");
			if (i < a.m - 1) os << "\n";
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
template <class T>
pii reduced_row_echelon_form(Matrix<T>& mat) {
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
		T vij_inv = T(1) / v[i][j];
		repi(t, j, n - 1) v[i][t] *= vij_inv;
		
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


//【連立一次方程式】O(m n min(m, n))
/*
* m * (n + 1) 拡大係数行列 mat で表される連立一次方程式の解の 1 つを sol に格納する．
* 解が存在しないなら false を返す．
*/
template <class T>
bool solve_eq(const Matrix<T>& mat, vector<T>* sol = nullptr) {
	// verify : https://onlinejudge.u-aizu.ac.jp/problems/2171

	int m = mat.m, n = mat.n - 1; auto v = mat.v;

	// ピボットの位置を記録しておくリスト
	vector<pii> pivots;

	// 未確定の列を記録しておくリスト
	list<int> rmd;
	repi(j, 0, n) rmd.push_back(j);

	rep(i, m) {
		// i 行目の係数を左から走査し非 0 を見つける．
		auto it = rmd.begin();
		for (; it != rmd.end(); it++) if (v[i][*it] != T(0)) break;

		// 全てが 0 なら無視
		if (it == rmd.end()) continue;
		int j = *it;
		
		// 定数項のみが非 0 なら解なし
		if (j == n) return false;
		rmd.erase(it);
		pivots.emplace_back(i, j);

		// v[i][j] が 1 になるよう行全体を v[i][j] で割る．
		T vij_inv = T(1) / v[i][j];
		repi(j2, j, n) v[i][j2] *= vij_inv;

		// j 列目に見つかったら他の行の j 列目を全て 0 にする．
		rep(i2, m) {
			if (v[i2][j] == T(0) || i2 == i) continue;

			T mul = v[i2][j];
			repi(j2, j, n) v[i2][j2] -= v[i][j2] * mul;
		}
	}

	// 解の例の構成（任意定数は全て 0 にする）
	if (sol != nullptr) {
		sol->assign(n, T(0));
		repe(p, pivots) (*sol)[p.second] = v[p.first][n];
	}

	return true;
}


//【連立一次方程式（2k+1 重対角行列）】O(n k^2)
/*
* (i,j) 成分が a[i][k-i+j] であるような 2k+1 重 n×n 対角行列 A と b[0..n) について，
* A x = b の解を返す（不定や不能の場合は空リストを返す）
*/
template <class T>
vector<T> solve_eq_diagonal(vector<vector<T>> a, vector<T> b) {
	// verify : https://atcoder.jp/contests/indeednow-finala-open/tasks/indeednow_2015_finala_e

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

		// 見つかったら i 行目とその行を入れ替える．
		// 行列式の値は -1 倍しておく．
		if (i2 != i) {
			swap(v[i], v[i2]);
			res *= T(-1);
		}

		// v[i][j] が 1 になるよう行全体を v[i][j] で割る．
		// 行列式の値は v[i][j] 倍しておく．
		res *= v[i][j];
		T vij_inv = T(1) / v[i][j];
		repi(j2, j, n - 1) v[i][j2] *= vij_inv;

		// v[i][j] より下方の行の成分が全て 0 になるよう i 行目を定数倍して減じる．
		// 行列式の値は変化しない．
		repi(i2, i + 1, n - 1) {
			T mul = v[i2][j];
			repi(j2, j, n - 1) v[i2][j2] -= v[i][j2] * mul;
		}

		// 注目位置を右下に移す．
		i++; j++;
	}

	return res;
}


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


//【逆行列】O(n^3)
/*
* n 次正方行列 mat の逆行列を返す（存在しなければ空）
*/
template <class T>
Matrix<T> inverse_matrix(const Matrix<T>& mat) {
	// verify : https://judge.yosupo.jp/problem/inverse_matrix

	int m = mat.m;

	// 元の行列 mat と単位行列を繋げた拡大行列を作る．
	Matrix<T> aug(m, 2 * m);
	rep(i, m) rep(j, m) {
		aug.v[i][j] = mat.v[i][j];
		aug.v[i][m + j] = T(i == j);
	}
	int n = 2 * m;
	auto& v = aug.v;

	// 拡大行列に対して行基本変形を行い，左側を単位行列にすることを目指す．

	// 注目位置を (i, j)（i 行目かつ j 列目）とする．
	int i = 0, j = 0;

	while (i < m && j < n) {
		// 同じ列の下方の行から非 0 成分を見つける．
		int i2 = i;
		while (i2 < m && v[i2][j] == T(0)) i2++;

		// 見つからなかったら全て 0 の列があったので mat は非正則
		if (i2 == m) return Matrix<T>();

		// 見つかったら i 行目とその行を入れ替える．
		if (i != i2) swap(v[i], v[i2]);

		// v[i][j] が 1 になるよう行全体を v[i][j] で割る．
		T vij_inv = T(1) / v[i][j];
		repi(t, j, n - 1) v[i][t] *= vij_inv;

		// v[i][j] と同じ列の成分が全て 0 になるよう i 行目を定数倍して減じる．
		rep(i2, m) {
			// i 行目だけは引かない．
			if (i2 == i) continue;

			T mul = v[i2][j];
			repi(j2, j, n - 1) v[i2][j2] -= v[i][j2] * mul;
		}

		// 注目位置を右下に移す．
		i++; j++;
	}

	// 拡大行列の右半分が mat の逆行列なのでコピーする．
	Matrix<T> mat_inv(m, m);
	rep(i, m) rep(j, m) mat_inv[i][j] = v[i][m + j];

	return mat_inv;
}


//【階数標準形】O(m n (m + n))
/*
* A = a[0..m)[0..n) を階数標準形 R_r := [I_r, O; O, O] に変換する行列，すなわち
*		P A Q = R_r （r = rank A）
* を満たす行列 P, Q を p[0..m)[0..m), q[0..n)[0..n) に格納し，r を返す．
*/
template <class T>
int rank_normal_form(const Matrix<T>& a, Matrix<T>& p, Matrix<T>& q) {
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
		T vij_inv = T(1) / v[i][j];
		repi(t, j, m + n - 1) v[i][t] *= vij_inv;

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
* 正方行列 A = a[0..n)[0..n) の最小多項式を返す．
* 最小多項式とは，f(A) = O を満たす次数最小なモニック多項式である．
*
* 利用：【線形漸化式の発見】
*/
MFPS minimal_polynomial(const Matrix<mint>& a) {
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
* 利用：【形式的冪級数（mod 998244353）】,【ヘッセンベルグ縮約】
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


//【行列のクロネッカー積】O(m1 m2 n1 n2)
/*
* 行列 a[0..m1)[0..n1) と b[0..m2)[0..n2) のクロネッカー積を返す．
*/
template <class T>
Matrix<T> kronecker_product(const Matrix<T>& a, const Matrix<T>& b) {
	Matrix<T> res(a.m * b.m, a.n * b.n);
	rep(i1, a.m) rep(j1, a.n) rep(i2, b.m) rep(j2, b.n) {
		res[i1 * b.m + i2][j1 * b.n + j2] = a[i1][j1] * b[i2][j2];
	}

	return res;
}


//【行列のクロネッカー積とベクトルとの積】O(k m n)（m : mats の行列の最大サイズ）
/*
* 行列の列 mats[0..k) のクロネッカー積を M とし，ベクトル M vec[0..n) を返す．
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
	dump(mats); dump(vec);

	// Ws : vec が大きさ Ws の小ブロックに分割されていることを表す
	int Ws = 1;

	repir(k, K - 1, 0) {
		auto& mat = mats[k];

		// Wl : vec が大きさ Wl の大ブロックに分割されていることを表す
		int Wl = Ws * mat.n;

		vector<T> nvec;

		// tl : 上から何個目の大ブロックを計算しているか
		rep(tl, sz(vec) / Wl) {
			// ts : 大ブロック内で上から何個目の小ブロックを計算しているか
			rep(ts, mat.m) {
				// i : 小ブロック内の何行目を計算しているか
				rep(i, Ws) {
					T val = 0;
					rep(j, mat.n) {
						val += mat[ts][j] * vec[tl * Wl + i + j * Ws];
					}
					nvec.emplace_back(val);
				}
			}
		}

		Ws *= mat.m;

		vec = move(nvec);
		dump(vec);
	}

	return vec;
}


