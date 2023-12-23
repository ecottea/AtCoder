#pragma once
#include "header.h"
// ■■■■■ ビット行列 ■■■■■


//【ビット行列】
/*
* ビット行列を表す構造体
*
* Bit_matrix<M>(int n, int m) : O(n M / 64)
*	n×m 零行列で初期化する．
*	制約：m ≦ M
*
* Bit_matrix<M>(int n) : O(n M / 64)
*	n×n 単位行列で初期化する．
*
* Bit_matrix<M>(vector<bitset<M>> a, int m) : O(n M / 64)
*	二次元配列 a[0..n)[0..m) の要素で初期化する．
*
* Bit_matrix<M>(vi[vl] a, int m) : O(n M / 64)
*	数 a[i] の第 j ビットを v[i][j] とする行列で初期化する．
*
* push_back(bitset<M> col) : O(M / 64)
*	最下行に col を追加する．
*
* A * x : O(n M / 64)
*	n×m 行列 A と m 次元列ベクトル x の積を返す．
*
* A * B : O(n m l)
*	n×m 行列 A と m×l 行列 B の積を返す．
*
* Bit_matrix<M> pow(ll d) : O(n^3 log d)
*	自身を d 乗した行列を返す．
*
* Bit_matrix<M> transpose() : O(n m)
*	自身を転置した行列を返す．
*	制約：n ≦ M
*/
template <int M>
struct Bit_matrix {
	int n, m; // 行列のサイズ（n 行 m 列）
	vector<bitset<M>> v; // 行列の成分

	// n×m 零行列で初期化する．
	Bit_matrix(int n, int m) : n(n), m(m), v(n) {}

	// n×n 単位行列で初期化する．
	Bit_matrix(int n) : n(n), m(n), v(n) { rep(i, n) v[i][i] = 1; }

	// 二次元配列 a[0..n)[0..m) の要素で初期化する．
	Bit_matrix(const vector<bitset<M>>& a, int m) : n(sz(a)), m(m), v(a) {}

	// 数 a[i] の第 j ビットを v[i][j] とする行列で初期化する．
	Bit_matrix(const vi& a, int m) : n(sz(a)), m(m), v(n) { rep(i, n) v[i] = bitset<M>(a[i]); }
	Bit_matrix(const vl& a, int m) : n(sz(a)), m(m), v(n) { rep(i, n) v[i] = bitset<M>(a[i]); }
	Bit_matrix() : m(0), n(0) {}

	// 代入
	Bit_matrix(const Bit_matrix& old) = default;
	Bit_matrix& operator=(const Bit_matrix& other) = default;

	// 比較
	bool operator==(const Bit_matrix& g) const { return n == g.n && m == g.m && v == g.v; }
	bool operator!=(const Bit_matrix& g) const { return !(*this == g); }

	// アクセス
	inline bitset<M> const& operator[](int i) const { return v[i]; }
	inline bitset<M>& operator[](int i) { return v[i]; }

	// 行の追加
	void push_back(const bitset<M>& col) { v.push_back(col); n++; }

	// 行列ベクトル積
	bitset<M> operator*(const bitset<M>& x) const {
		bitset<M> y;
		rep(i, n) y[i] = (v[i] & x).count() % 2;
		return y;
	}

	// 積
	Bit_matrix operator*(const Bit_matrix& b) const {
		Bit_matrix res(n, b.m);
		rep(i, res.n) rep(j, res.m) rep(k, m) res[i][j] = res[i][j] ^ (v[i][k] & b[k][j]);
		return res;
	}
	Bit_matrix& operator*=(const Bit_matrix& b) { *this = *this * b; return *this; }

	// 累乗
	Bit_matrix pow(ll d) const {
		// verify : https://atcoder.jp/contests/jag2013summer-day4/tasks/icpc2013summer_day4_f
		
		Bit_matrix res(n), pow2 = *this;
		while (d > 0) {
			if (d & 1) res *= pow2;
			pow2 *= pow2;
			d /= 2;
		}
		return res;
	}

	// 転置（A^T）
	Bit_matrix transpose() const {
		Bit_matrix res(m, n);
		rep(i, m) rep(j, n) res[i][j] = v[j][i];
		return res;
	}

#ifdef _MSC_VER
	friend ostream& operator<<(ostream& os, const Bit_matrix& a) {
		rep(i, a.n) {
			os << "[";
			rep(j, a.m) os << a[i][j] << (j < a.m - 1 ? " " : "]");
			if (i < a.n - 1) os << "\n";
		}
		return os;
	}
#endif
};


//【転置との積】: O(n m l / 64)
/*
* n×m 行列 A と l×m 行列 B について，積 A * B^T（n×l 行列）を返す．
*/
template <int N>
Bit_matrix<N> prod_transpose(const Bit_matrix<N>& A, const Bit_matrix<N>& B) {
	Bit_matrix<N> res(A.n, B.n);
	rep(i, res.n) rep(j, res.m) res[i][j] = (A[i] & B[j]).count() % 2;
	return res;
}


//【行簡約階段形】O(n m min(n, m) / 64)
/*
* 行基本変形で m×n 行列 A を行簡約階段形に変形し，rank A を返す（ピボットの上下は全て 0）
* また変形後の第 i 行が元の第何行だったかを is[i] に，ピボットが第何列にあるかを js[i] に格納する．
*/
template <int M>
int reduced_row_echelon_form(Bit_matrix<M>& A, vi* is = nullptr, vi* js = nullptr) {
	// verify : https://atcoder.jp/contests/abc249/tasks/abc249_g

	int n = A.n, m = A.m;
	if (is == nullptr) { is = new vi(); } is->resize(n); iota(all(*is), 0);
	if (js == nullptr) { js = new vi(); } js->clear(); js->reserve(n);

	// 注目位置を A[i][j] とする．
	int i = 0, j = 0;

	while (i < n && j < m) {
		// 注目列の下方の行から 1 を見つける．
		int i2 = i;
		while (i2 < n && !A[i2][j]) i2++;

		// 見つからなかったら注目位置を右に移す．
		if (i2 == n) { j++; continue; }

		// 見つかったら第 i 行とその行を入れ替える．
		if (i != i2) { swap(A[i], A[i2]); swap((*is)[i], (*is)[i2]); }

		// A[i][j] をピボットに選択する．
		js->push_back(j);

		// 第 i 行以外の第 j 列の成分が全て 0 になるよう第 i 行を XOR する．
		rep(i2, n) if (A[i2][j] && i2 != i) A[i2] ^= A[i];

		// 注目位置を右下に移す．
		i++; j++;
	}
	is->resize(i);

	return i;
}


//【行簡約形（行交換なし）】O(n m min(n, m) / 64)
/*
* 行基本変形（行交換なし）で m×n 行列 A を行簡約形に変形し，rank A を返す（ピボットの上下は全て 0）
* また 1 のある行のリストを is に，対応する行のピボットが第何列にあるかを js に格納する．
*/
template <int M>
int row_reduced_form(Bit_matrix<M>& A, vi* is = nullptr, vi* js = nullptr) {
	// verify : https://atcoder.jp/contests/agc045/tasks/agc045_a

	int n = A.n, m = A.m;
	if (is == nullptr) { is = new vi(); } is->clear(); is->reserve(n);
	if (js == nullptr) { js = new vi(); } js->clear(); js->reserve(n);

	// 未確定の列を記録しておくリスト
	list<int> rjs;
	rep(j, m) rjs.push_back(j);

	rep(i, n) {
		// 第 i 行の係数を左から走査し 1 を見つける．
		auto it = rjs.begin();
		for (; it != rjs.end(); it++) if (A[i][*it]) break;

		// 第 i 行の全てが 0 なら無視する．
		if (it == rjs.end()) continue;

		// A[i][j] をピボットに選択する．
		int j = *it;
		rjs.erase(it);
		is->push_back(i);
		js->push_back(j);

		// 第 i 行以外の第 j 列の成分が全て 0 になるよう第 i 行を XOR する．
		rep(i2, n) if (A[i2][j] && i2 != i) A[i2] ^= A[i];
	}

	return sz(*is);
}


//【行列式】
/*
*【行簡約階段形】を用いて，full rank なら 1，さもなくば 0 と判断できる．
* 
* verify : https://yukicoder.me/problems/no/2255
*/


//【逆行列】O(n^3 / 64)
/*
* n 次正方行列 A の逆行列を返す（存在しなければ空）
*/
template <int M>
Bit_matrix<M> inverse_matrix(const Bit_matrix<M>& A) {
	// verify : https://atcoder.jp/contests/jag2013summer-day4/tasks/icpc2013summer_day4_f

	int n = A.n;

	// 元の行列 A と単位行列を繋げた拡大行列 v を作る．
	Bit_matrix<2 * M> v(n, 2 * n);
	rep(i, n) rep(j, n) {
		v[i][j] = A[i][j];
		v[i][n + j] = (i == j);
	}
	int m = 2 * n;

	// 注目位置を (i, j)（i 行目かつ j 列目）とする．
	int i = 0, j = 0;

	// 拡大行列に対して行基本変形を行い，左側を単位行列にすることを目指す．
	while (i < n && j < m) {
		// 同じ列の下方の行から非 0 成分を見つける．
		int i2 = i;
		while (i2 < n && !v[i2][j]) i2++;

		// 見つからなかったら全て 0 の列があったので A は非正則
		if (i2 == n) return Bit_matrix<M>();

		// 見つかったら i 行目とその行を入れ替える．
		if (i != i2) swap(v[i], v[i2]);

		// 第 i 行以外の第 j 列の成分が全て 0 になるよう第 i 行を XOR する．
		rep(i2, n) if (v[i2][j] && i2 != i) v[i2] ^= v[i];

		// 注目位置を右下に移す．
		i++; j++;
	}

	// 拡大行列の右半分が mat の逆行列なのでコピーする．
	Bit_matrix<M> invA(n, n);
	rep(i, n) rep(j, n) invA[i][j] = v[i][n + j];

	return invA;
}


//【線形方程式】O(n m min(n, m) / 64)
/*
* 与えられた n×m 行列 A と n 次元ベクトル b に対し，
* 線形方程式 A x = b の特殊解 x0（m 次元ベクトル）を格納する（なければ false を返す）
*
*（呼び出すとき gauss_jordan_elimination<M> としないと gcc でエラーになるので注意．）
*/
template <int M>
bool gauss_jordan_elimination(const Bit_matrix<M>& A, const vb& b, bitset<M>* x0 = nullptr) {
	// verify : https://atcoder.jp/contests/tenka1-2014-qualb/tasks/tenka1_2014_qualB_c

	int n = A.n, m = A.m;

	// v : 拡大係数行列 (A | b)
	vector<bitset<M + 1>> v(n);
	rep(i, n) rep(j, m) v[i][j] = A[i][j];
	rep(i, n) v[i][m] = b[i];

	// pivots[i] : 第 i 行のピボットが第何列にあるか
	vi pivots;

	// 注目位置を v[i][j] とする．
	int i = 0, j = 0;

	while (i < n && j <= m) {
		// 注目列の下方の行から 1 を見つける．
		int i2 = i;
		while (i2 < n && !v[i2][j]) i2++;

		// 見つからなかったら注目位置を右に移す．
		if (i2 == n) { j++; continue; }

		// 見つかったら第 i 行とその行を入れ替える．
		if (i != i2) swap(v[i], v[i2]);

		// v[i][j] をピボットに選択する．
		pivots.push_back(j);

		// 第 i 行以外の第 j 列の成分が全て 0 になるよう第 i 行を XOR する．
		rep(i2, n) if (v[i2][j] && i2 != i) v[i2] ^= v[i];

		// 注目位置を右下に移す．
		i++; j++;
	}

	// 最後に見つかったピボットの位置が第 m 列ならば解なし．
	if (pivots.back() == m) return false;

	// 解の例の構成
	if (x0 != nullptr) {
		x0->reset();
		int rnk = sz(pivots);
		rep(i, rnk) (*x0)[pivots[i]] = v[i][m];
	}

	return true;
}


//【基底の選択】O(n^2)
/*
* 与えられたビットベクトル v[0..n) に対し，Span(v) の基底を返す．
*/
template <class T>
vector<T> noshi_base(const vector<T>& v) {
	// verify : https://atcoder.jp/contests/apc001/tasks/apc001_f

	vector<T> res;
	for (auto x : v) {
		repe(y, res) chmin(x, x ^ y);
		if (x) res.push_back(x);
	}
	return res;
}


//【最小コスト基底】
/*
* ビットベクトル v[0..n) がコスト c[0..n) をもつとき，Span v の最小コスト基底は，
* {0} から始めてコスト昇順に部分空間が真に大きくなる限り v[i] を追加していくことで得られる．
* これには【行簡約形（行交換なし）】を用いることができる．
*
* verify : https://atcoder.jp/contests/abc236/tasks/abc236_f
*/


