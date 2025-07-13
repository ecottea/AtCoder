#pragma once
#include "header.h"
// ■■■■■ アダマール変換，XOR 畳込み ■■■■■


//【アダマール変換】O(2^N N)
/*
* a[0..2^N) を
*       A[set] = Σset2 (-1)^popcount(set ∩ set2) a[set2]
* なる A[0..2^N) に上書きする．
*/
template <class T>
void hadamard(vector<T>& a) {
	// verify : https://judge.yosupo.jp/problem/bitwise_xor_convolution

	// 具体例：
	//	A[0] = a[0] + a[1] + a[2] + a[3] + a[4] + a[5] + a[6] + a[7] + ...
	//	A[1] = a[0] - a[1] + a[2] - a[3] + a[4] - a[5] + a[6] - a[7] + ...
	//	A[2] = a[0] + a[1] - a[2] - a[3] + a[4] + a[5] - a[6] - a[7] + ...
	//	A[3] = a[0] - a[1] - a[2] + a[3] + a[4] - a[5] - a[6] + a[7] + ...
	//	A[4] = a[0] + a[1] + a[2] + a[3] - a[4] - a[5] - a[6] - a[7] + ...
	//	A[5] = a[0] - a[1] + a[2] - a[3] - a[4] + a[5] - a[6] + a[7] + ...
	//	A[6] = a[0] + a[1] - a[2] - a[3] - a[4] - a[5] + a[6] + a[7] + ...
	//	A[7] = a[0] - a[1] - a[2] + a[3] - a[4] + a[5] + a[6] - a[7] + ...
	
	int n = msb(sz(a));

	rep(i, n) repb(set, n) {
		if (!(set & (1 << i))) {
			T x = a[set];
			T y = a[set ^ (1 << i)];

			a[set] = x + y;
			a[set ^ (1 << i)] = x - y;
		}
	}
}


//【逆アダマール変換】O(2^N N)
/*
* A[0..2^N) を
*       A[set] = Σset2 (-1)^popcount(set ∩ set2) a[set2]
* なる a[0..2^N) に上書きする．
* 
* 制約：A の要素は 2 で割れる．
*/
template <class T>
void hadamard_inv(vector<T>& A) {
	// verify : https://judge.yosupo.jp/problem/bitwise_xor_convolution

	int n = msb(sz(A));

	rep(i, n) repb(set, n) {
		if (!(set & (1 << i))) {
			T x = A[set];
			T y = A[set ^ (1 << i)];

			// オーバーフローの危険があるので都度 /2 すべき．
			A[set] = (x + y) >> 1;
			A[set ^ (1 << i)] = (x - y) >> 1;
		}
	}
}


//【逆アダマール変換（mint）】O(2^N N + log(mod))
/*
* A[0..2^N) を
*       A[set] = Σset2 (-1)^popcount(set ∩ set2) a[set2]
* なる a[0..2^N) に上書きする．
* 
* 制約：mint の法は 2 の倍数でない
*
* 利用：【アダマール変換】
*/
void hadamard_inv(vm& A) {
	// verify : https://atcoder.jp/contests/abc265/tasks/abc265_h

	hadamard(A);

	// まとめて商をとらないと log(mod) 倍遅くなる．
	mint inv = mint(sz(A)).inv();
	rep(i, sz(A)) A[i] *= inv;
}


//【XOR 畳込み】O(2^N N)
/*
* 与えられた a[0..2^N), b[0..2^N) に対して
*       c[set] = Σ(set1 XOR set2 = set) a[set1] b[set2]
* なる c[0..2^N) を返す．
*
* 利用：【アダマール変換】,【逆アダマール変換】
*/
template <class T>
vector<T> xor_convolution(vector<T> a, vector<T> b) {
	// 参考 : https://kazuma8128.hatenablog.com/entry/2018/05/31/144519
	// verify : https://judge.yosupo.jp/problem/bitwise_xor_convolution

	int n = msb(sz(a));

	hadamard(a);
	hadamard(b);

	repb(set, n) a[set] *= b[set];

	hadamard_inv(a);

	return a;
}


//【XOR 畳込み（二項式）】O(2^N N)
/*
* 与えられた [0..N) 上の集合関数族 f_i = z_0 + c[i]z_i (i⊂[0..N)) に対して
* 総 XOR 畳込みをとって得られる集合関数 g[0..2^N) を返す．
*/
vm multi_xor_convolution_binomial(const vm& c) {
	int n = sz(c);
	int N = msb(n);

	// 乗法単位元である 1 を加算し，採用/不採用どちらも可とする．
	vm c_pos_num(n), c_pos_dnm(n, 1), c_neg_num(n), c_neg_dnm(n, 1);
	repb(set, N) {
		c_pos_num[set] = 1 + c[set];
		c_neg_num[set] = 1 - c[set];
	}

	// c_pos, c_neg それぞれを演算を積としてアダマール変換する．
	rep(i, N) repb(set, N) {
		if (getb(set, i)) {
			auto x_pos_num = c_pos_num[set ^ (1 << i)];
			auto x_pos_dnm = c_pos_dnm[set ^ (1 << i)];
			auto y_pos_num = c_pos_num[set];
			auto y_pos_dnm = c_pos_dnm[set];

			c_pos_num[set ^ (1 << i)] = x_pos_num * y_pos_num; // 積
			c_pos_dnm[set ^ (1 << i)] = x_pos_dnm * y_pos_dnm;
			c_pos_num[set] = x_pos_num * y_pos_dnm; // 商（逆数との積）
			c_pos_dnm[set] = x_pos_dnm * y_pos_num;

			auto x_neg_num = c_neg_num[set ^ (1 << i)];
			auto x_neg_dnm = c_neg_dnm[set ^ (1 << i)];
			auto y_neg_num = c_neg_num[set];
			auto y_neg_dnm = c_neg_dnm[set];

			c_neg_num[set ^ (1 << i)] = x_neg_num * y_neg_num; // 積
			c_neg_dnm[set ^ (1 << i)] = x_neg_dnm * y_neg_dnm;
			c_neg_num[set] = x_neg_num * y_neg_dnm; // 商（逆数との積）
			c_neg_dnm[set] = x_neg_dnm * y_neg_num;
		}
	}

	// 適切に符号付けされた Πset (1 ± c[set]) の形の式を得る．
	repb(set, N) c_pos_num[set] *= c_neg_dnm[set];

	rep(i, N) repb(set, N) {
		if (getb(set, i)) {
			auto x = c_pos_num[set ^ (1 << i)];
			auto y = c_pos_num[set];

			c_pos_num[set ^ (1 << i)] = x + y;
			c_pos_num[set] = x - y;
		}
	}

	mint inv = mint(n).inv();
	repb(set, N) c_pos_num[set] *= inv;

	return c_pos_num;
}


//【XOR 畳込みの表現行列】
/*
* 例えば c = [0..8) との XOR 畳込み
*		g[set] = Σ(set1 XOR set2 = set) c[set1] f[set2]
* の表現行列は次のようになる：
*		[g[0]]   [0 1 2 3 4 5 6 7] [f[0]]
*		[g[1]]   [1 0 3 2 5 4 7 6] [f[1]]
*		[g[2]]   [2 3 0 1 6 7 4 5] [f[2]]
*		[g[3]] = [3 2 1 0 7 6 5 4] [f[3]]
*		[g[4]]   [4 5 6 7 0 1 2 3] [f[4]]
*		[g[5]]   [5 4 7 6 1 0 3 2] [f[5]]
*		[g[6]]   [6 7 4 5 2 3 0 1] [f[6]]
*		[g[7]]   [7 6 5 4 3 2 1 0] [f[7]]
* 
* verify : https://atcoder.jp/contests/abc396/tasks/abc396_g
*/


//【XOR 畳込みと多項式環】
/*
* XOR 畳込みは n 変数多項式環の剰余環
*		F[z_1, ..., z_n] / (1-z_1^2, ..., 1-z_n^2)
* における積と解釈できる．
*/


//【高速アダマール変換と行列のクロネッカー積】
/*
* a[0..2^n) を高速アダマール変換して A[0..2^n) にする線形変換の表現行列は，
* アダマール行列 [1  1] の n 個のクロネッカー積に等しい．
*               [1 -1]
* 
* A[0..2^n) を逆高速アダマール変換して a[0..2^n) にする線形変換の表現行列は，
* アダマール行列の逆行列 [1/2  1/2] の n 個のクロネッカー積に等しい．
*                      [1/2 -1/2]
* 
* verify : https://atcoder.jp/contests/abc212/tasks/abc212_h
*/


