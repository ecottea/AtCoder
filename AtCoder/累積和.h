#pragma once
#include "header.h"
// ■■■■■ 累積和，max 畳込み，min 畳込み ■■■■■


//【ゼータ変換（上位要素）】O(n)
/*
* a[0..n) を
*       A[i] = Σj≧i a[j]
* なる A[0..n) に上書きする（上位要素の値全てを自身に加える）
*/
template <typename T>
void geq_zeta(vector<T>& a) {
	// 参考 : https://qiita.com/convexineq/items/afc84dfb9ee4ec4a67d5

	//【例（n = 8 のとき）】：
	//	A[0] = a[0] + a[1] + a[2] + a[3] + a[4] + a[5] + a[6] + a[7]
	//	A[1] =      + a[1] + a[2] + a[3] + a[4] + a[5] + a[6] + a[7]
	//	A[2] =             + a[2] + a[3] + a[4] + a[5] + a[6] + a[7]
	//	A[3] =                    + a[3] + a[4] + a[5] + a[6] + a[7]
	//	A[4] =                           + a[4] + a[5] + a[6] + a[7]
	//	A[5] =                                  + a[5] + a[6] + a[7]
	//	A[6] =                                         + a[6] + a[7]
	//	A[7] =                                                + a[7]

	int n = sz(a);
	repir(i, n - 2, 0) a[i] += a[i + 1];
}


//【メビウス変換（上位要素）】O(n)
/*
* A[0..n) を
*       a[i] = Σj≧i a[j]
* なる a[0..n) に上書きする（上位要素から自身の値への寄与を取り除く）
*/
template <typename T>
void geq_mobius(vector<T>& A) {
	// 参考 : https://qiita.com/convexineq/items/afc84dfb9ee4ec4a67d5

	//【例（n = 8 のとき）】：
	//	a[0] = A[0] - A[1]
	//	a[1] =      + A[1] - A[2]
	//	a[2] =             + A[2] - A[3]
	//	a[3] =                    + A[3] - A[4]
	//	a[4] =                           + A[4] - A[5]
	//	a[5] =                                  + A[5] - A[6]
	//	a[6] =                                         + A[6] - A[7]
	//	a[7] =                                                + A[7]

	int n = sz(A);
	repi(i, 0, n - 2) A[i] -= A[i + 1];
}


//【添字 min での畳込み】
/*
* 与えられた a[0..n), b[0..n) に対して
*       c[k] = Σ_(min(i,j) = k) a[i] b[j]
* なる c[0..n) を返す．
*
* 利用：【ゼータ変換（上位要素）】,【メビウス変換（上位要素）】
*/
template <typename T>
vector<T> min_convolution(vector<T> a, vector<T> b) {
	// 参考 : https://qiita.com/convexineq/items/afc84dfb9ee4ec4a67d5

	int n = sz(a);

	geq_zeta(a);
	geq_zeta(b);

	rep(i, n) a[i] *= b[i];

	geq_mobius(a);

	return a;
}


//【ゼータ変換（下位要素）】O(n)
/*
* a[0..n) を
*       A[i] = Σj≦i a[j]
* なる A[0..n) に上書きする（下位要素の値全てを自身に加える）
*/
template <typename T>
void leq_zeta(vector<T>& a) {
	// 参考 : https://qiita.com/convexineq/items/afc84dfb9ee4ec4a67d5

	//【例（n = 8 のとき）】：
	//	A[0] = a[0]
	//	A[1] = a[0] + a[1]
	//	A[2] = a[0] + a[1] + a[2]
	//	A[3] = a[0] + a[1] + a[2] + a[3]
	//	A[4] = a[0] + a[1] + a[2] + a[3] + a[4]
	//	A[5] = a[0] + a[1] + a[2] + a[3] + a[4] + a[5]
	//	A[6] = a[0] + a[1] + a[2] + a[3] + a[4] + a[5] + a[6]
	//	A[7] = a[0] + a[1] + a[2] + a[3] + a[4] + a[5] + a[6] + a[7]

	int n = sz(a);
	repi(i, 1, n - 1) a[i] += a[i - 1];
}


//【メビウス変換（下位要素）】O(n)
/*
* A[0..n) を
*       A[i] = Σj≦i a[j]
* なる a[0..n) に上書きする（下位要素から自身の値への寄与を取り除く）
*/
template <typename T>
void leq_mobius(vector<T>& A) {
	// 参考 : https://qiita.com/convexineq/items/afc84dfb9ee4ec4a67d5

	//【例（n = 8 のとき）】：
	//	a[0] = + A[0]
	//	a[1] = - A[0] + A[1]
	//	a[2] =        - A[1] + A[2]
	//	a[3] =               - A[2] + A[3]
	//	a[4] =                      - A[3] + A[4]
	//	a[5] =                             - A[4] + A[5]
	//	a[6] =                                    - A[5] + A[6]
	//	a[7] =                                           - A[6] + A[7]

	int n = sz(A);
	repir(i, n - 1, 1) A[i] -= A[i - 1];
}


//【添字 max での畳込み】
/*
* 与えられた a[0..n), b[0..n) に対して
*       c[k] = Σ_(max(i,j) = k) a[i] b[j]
* なる c[0..n) を返す．
*
* 利用：【ゼータ変換（下位要素）】,【メビウス変換（下位要素）】
*/
template <typename T>
vector<T> max_convolution(vector<T> a, vector<T> b) {
	// 参考 : https://qiita.com/convexineq/items/afc84dfb9ee4ec4a67d5

	int n = sz(a);

	leq_zeta(a);
	leq_zeta(b);

	rep(i, n) a[i] *= b[i];

	leq_mobius(a);

	return a;
}


//【多次元ゼータ変換（上位要素）】O(n^d d)
/*
* a[0..n)^d を，マルチインデックスと非負整数の n 進表示を同一視し
*       A[I] = ΣJ≧I a[J]
* なる A[0..n)^k に上書きする（上位要素の値全てを自身に加える）
*/
template <typename T>
void geq_zeta_dD(int n, int d, vector<T>& a) {
	// 参考 : https://qiita.com/convexineq/items/afc84dfb9ee4ec4a67d5
	// verify : https://yukicoder.me/problems/no/2178

	//【例（n = 3, d = 2 のとき）】：
	//	A[0][0] = a[0][0] + a[0][1] + a[0][2] + a[1][0] + a[1][1] + a[1][2] + a[2][0] + a[2][1] + a[2][2]
	//	A[0][1] =         + a[0][1] + a[0][2]           + a[1][1] + a[1][2]           + a[2][1] + a[2][2]
	//	A[0][2] =                   + a[0][2]                     + a[1][2]                     + a[2][2]
	//	A[1][0] =                             + a[1][0] + a[1][1] + a[1][2] + a[2][0] + a[2][1] + a[2][2]
	//	A[1][1] =                                       + a[1][1] + a[1][2]           + a[2][1] + a[2][2]
	//	A[1][2] =                                                 + a[1][2]                     + a[2][2]
	//	A[2][0] =                                                           + a[2][0] + a[2][1] + a[2][2]
	//	A[2][1] =                                                                     + a[2][1] + a[2][2]
	//	A[2][2] =                                                                               + a[2][2]
	//
	// パスカルの三角形 mod n のパターンが見えている．

	vi pow_n(d + 1);
	pow_n[0] = 1;
	rep(i, d) pow_n[i + 1] = pow_n[i] * n;

	rep(i, d) repir(j, pow_n[d] - 1, 0) {
		// n 進表示での i 桁目を 1 増やし，繰り上がりが起こらないかを調べる．
		if (j % pow_n[i + 1] + pow_n[i] < pow_n[i + 1]) a[j] += a[j + pow_n[i]];
	}
}


//【多次元メビウス変換（上位要素）】O(n^d d)
/*
* A[0..n)^d を，マルチインデックスと非負整数の n 進表示を同一視し
*       A[I] = ΣJ≧I a[J]
* なる a[0..n)^k に上書きする（上位要素から自身の値への寄与を取り除く）
*/
template <typename T>
void geq_mobius_dD(int n, int d, vector<T>& a) {
	// 参考 : https://qiita.com/convexineq/items/afc84dfb9ee4ec4a67d5

	//【例（n = 3, d = 2 のとき）】：
	//	a[0][0] = A[0][0] - A[0][1]           - A[1][0] + A[1][1]                                        
	//	a[0][1] =         + A[0][1] - A[0][2]           - A[1][1] + A[1][2]                              
	//	a[0][2] =                   + A[0][2]                     - A[1][2]                              
	//	a[1][0] =                             + A[1][0] - A[1][1]           - A[2][0] + A[2][1]          
	//	a[1][1] =                                       + A[1][1] - A[1][2]           - A[2][1] + A[2][2]
	//	a[1][2] =                                                 + A[1][2]                     - A[2][2]
	//	a[2][0] =                                                           + A[2][0] - A[2][1]          
	//	a[2][1] =                                                                     + A[2][1] - A[2][2]
	//	a[2][2] =                                                                               + A[2][2]

	vi pow_n(d + 1);
	pow_n[0] = 1;
	rep(i, d) pow_n[i + 1] = pow_n[i] * n;

	rep(i, d) repi(j, 0, pow_n[d] - 1) {
		// n 進表示での i 桁目を 1 増やし，繰り上がりが起こらないかを調べる．
		if (j % pow_n[i + 1] + pow_n[i] < pow_n[i + 1]) a[j] -= a[j + pow_n[i]];
	}
}


//【添字 min での多次元畳込み】O(n^d d)
/*
* 与えられた a[0..n)^d, b[0..n)^d に対して
*       c[K] = Σ_(min(I,J) = K) a[I] b[J]
* なる c[0..n)^d を返す（添字の min は各点でとる）
*
* 利用：【多次元ゼータ変換（上位要素）】,【多次元メビウス変換（上位要素）】
*/
template <typename T>
vector<T> min_convolution_dD(int n, int d, vector<T> a, vector<T> b) {
	// 参考 : https://qiita.com/convexineq/items/afc84dfb9ee4ec4a67d5

	geq_zeta_dD(n, d, a);
	geq_zeta_dD(n, d, b);

	int N = (int)pow(n, d);
	rep(i, N) a[i] *= b[i];

	geq_mobius_dD(n, d, a);

	return a;
}


//【添字 min での多次元上位畳込み】O(2^n n)
/*
* 与えられた a[0..n)^d, b[0..n)^d に対して
*       c[K] = Σ_(min(I,J) ≧ K) a[I] b[J]
* なる c[0..n)^d を返す（添字の min は各点でとる）
*
* 利用：【多次元ゼータ変換（上位要素）】
*/
template <typename T>
vector<T> geq_min_convolution_dD(int n, int d, vector<T> a, vector<T> b) {
	int n = msb(sz(a));

	geq_zeta_dD(n, d, a);
	geq_zeta_dD(n, d, b);

	int N = (int)pow(n, d);
	rep(i, N) a[i] *= b[i];

	return a;
}


//【多次元ゼータ変換（下位要素）】O(n^d d)
/*
* a[0..n)^d を，マルチインデックスと非負整数の n 進表示を同一視し
*       A[I] = ΣJ≦I a[J]
* なる A[0..n)^k に上書きする（下位要素の値全てを自身に加える）
*/
template <typename T>
void leq_zeta_dD(int n, int d, vector<T>& a) {
	// 参考 : https://qiita.com/convexineq/items/afc84dfb9ee4ec4a67d5
	// verify : https://atcoder.jp/contests/arc136/tasks/arc136_d

	vi pow_n(d + 1);
	pow_n[0] = 1;
	rep(i, d) pow_n[i + 1] = pow_n[i] * n;

	rep(i, d) repi(j, 0, pow_n[d] - 1) {
		// n 進表示での i 桁目を 1 増やし，繰り上がりが起こらないかを調べる．
		if (j % pow_n[i + 1] + pow_n[i] < pow_n[i + 1]) a[j + pow_n[i]] += a[j];
	}
}


//【多次元メビウス変換（下位要素）】O(n^d d)
/*
* A[0..n)^d を，マルチインデックスと非負整数の n 進表示を同一視し
*       A[I] = ΣJ≦I a[J]
* なる a[0..n)^k に上書きする（下位要素から自身の値への寄与を取り除く）
*/
template <typename T>
void leq_mobius_dD(int n, int d, vector<T>& a) {
	// 参考 : https://qiita.com/convexineq/items/afc84dfb9ee4ec4a67d5

	vi pow_n(d + 1);
	pow_n[0] = 1;
	rep(i, d) pow_n[i + 1] = pow_n[i] * n;

	rep(i, d) repir(j, pow_n[d] - 1, 0) {
		// n 進表示での i 桁目を 1 増やし，繰り上がりが起こらないかを調べる．
		if (j % pow_n[i + 1] + pow_n[i] < pow_n[i + 1]) a[j + pow_n[i]] -= a[j];
	}
}


//【添字 max での多次元畳込み】O(n^d d)
/*
* 与えられた a[0..n)^d, b[0..n)^d に対して
*       c[K] = Σ_(max(I,J) = K) a[I] b[J]
* なる c[0..n)^d を返す（添字の max は各点でとる）
*
* 利用：【多次元ゼータ変換（下位要素）】,【多次元メビウス変換（下位要素）】
*/
template <typename T>
vector<T> max_convolution_dD(int n, int d, vector<T> a, vector<T> b) {
	// 参考 : https://qiita.com/convexineq/items/afc84dfb9ee4ec4a67d5

	leq_zeta_dD(n, d, a);
	leq_zeta_dD(n, d, b);

	int N = (int)pow(n, d);
	rep(i, N) a[i] *= b[i];

	leq_mobius_dD(n, d, a);

	return a;
}


//【添字 max での多次元下位畳込み】O(n^d d)
/*
* 与えられた a[0..n)^d, b[0..n)^d に対して
*       c[K] = Σ_(max(I,J) ≦ K) a[I] b[J]
* なる c[0..n)^d を返す（添字の max は各点でとる）
*
* 利用：【多次元ゼータ変換（下位要素）】
*/
template <typename T>
vector<T> leq_max_convolution_dD(int n, int d, vector<T> a, vector<T> b) {
	leq_zeta_dD(n, d, a);
	leq_zeta_dD(n, d, b);

	int N = (int)pow(n, d);
	rep(i, N) a[i] *= b[i];

	return a;
}


//【多次元高速ゼータ／メビウス変換と行列のクロネッカー積】
/*
* a[0..n)^d を上位要素で多次元高速ゼータ変換して A[0..n)^d にする線形変換の表現行列は，
* 上三角成分が全て 1 の n×n 行列 d 個のクロネッカー積に等しい．
* verify : https://yukicoder.me/problems/no/2178
* 
* a[0..n)^d を下位要素で多次元高速ゼータ変換して A[0..n)^d にする線形変換の表現行列は，
* 下三角成分が全て 1 の n×n 行列 d 個のクロネッカー積に等しい．
* verify : https://atcoder.jp/contests/arc136/tasks/arc136_d
* 
* 多次元高速メビウス変換の表現行列は，上記行列の逆行列 d 個のクロネッカー積に等しい．
*/


