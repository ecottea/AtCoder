#pragma once
#include "header.h"
#include "二項係数.h"
// ■■■■■ 写像 12 相とその類題 ■■■■■


//【ボールの区別あり，箱の区別あり，箱の中身は任意】
/*
* ボールが n 個，箱が m 個の場合，重複順列の考え方より
* ボールの入れ方は m^n 通りとなる．
* 
* verify : https://onlinejudge.u-aizu.ac.jp/courses/library/7/DPL/all/DPL_5_A
*/


//【ボールの区別あり，箱の区別あり，箱の中身は 1 個以下】
/*
* ボールが n 個，箱が m 個の場合，順列の考え方より
* ボールの入れ方は perm(m, n) 通りとなる．
* 
* verify : https://onlinejudge.u-aizu.ac.jp/courses/library/7/DPL/all/DPL_5_B
*/


//【全射の数（ボールの区別あり，箱の区別あり，箱の中身は 1 個以上）】O(n m)
/*
* 各 i ∈ [0..n], j ∈ [0..m] について，
* i 点集合から j 点集合への全射の数を c[i][j] に格納する．
*/
void count_surjections(int n, int m, vvm& c) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/7/DPL/all/DPL_5_C

	//【方法】
	// c[i][j] : [0, i) から [0, j) への全射の数，についての漸化式を作る．
	//
	// case 1. [0, i-1) の行き先が [0, j) のとき：
	// i の行き先は [0, j) の j 通りから自由に選べる．
	//
	// case 2. [0, i-1) の行き先が [0, j) でないとき：
	// [0, i-1) の行き先は [0, j) からある元 j0 を除いたものでなければならない．
	// その j0 の選び方が j 通りあり，i の行き先は j0 に限定される．
	//
	// これらをまとめて，漸化式
	//		c[i][j] = c[i - 1][j] * j + c[i - 1][j - 1] * j
	// を得る．

	// c[i][j] : [0, i) から [0, j) への全射の数
	c = vvm(n + 1, vm(m + 1));
	c[0][0] = 1;

	repi(i, 1, n) repi(j, 1, m) c[i][j] = (c[i - 1][j] + c[i - 1][j - 1]) * j;
}


//【全射の数】O(m log n)
/*
* n 点集合から m 点集合への全射の数を返す．
*
* 利用：【階乗など（法が大きな素数）】
*/
mint count_surjections(ll n, int m) {
	// verify : https://yukicoder.me/problems/no/391

	//【方法】
	// 包除原理を用いて数え上げる．
	//
	// n 点集合から m 点集合への写像のうち，多くとも j 元にしか行かないものの個数は
	// どの j 元に行くかが bin(m, j) 通り，
	// n 元それぞれがどこに行くかが j^n 通りから選べるので，
	//		bin(m, j) j^n 通り
	// である．
	//
	// j = m として数えたいが，足しすぎたので j = m - 1 のものは引かなければならず，
	// 引きすぎたので j = m - 2 のものは足さなければならず，...... と続けると，答えは
	//		Σj=0..m (-1)^(m-j) bin(m, j) j^n 通り
	// である．

	Factorial_mint fm(m);

	mint res = 0;
	repi(j, 0, m) {
		res += ((m - j) % 2 == 0 ? 1 : -1) * fm.binomial(m, j) * mint(j).pow(n);
	}

	return res;
}


//【ボールの区別なし，箱の区別あり，箱の中身は任意】
/*
* ボールが n 個，箱が m 個の場合，重複組合せの考え方より
* ボールの入れ方は hom(m, n) = bin(n+m-1, m-1) 通りとなる．
* 
* verify : https://onlinejudge.u-aizu.ac.jp/courses/library/7/DPL/all/DPL_5_D
*/


//【ボールの区別なし，箱の区別あり，箱の中身は 1 個以下】
/*
* ボールが n 個，箱が m 個の場合，単にどの箱を選んだかなので，
* ボールの入れ方は bin(m, n) 通りとなる．
*
* verify : https://onlinejudge.u-aizu.ac.jp/courses/library/7/DPL/all/DPL_5_E
*/


//【ボールの区別なし，箱の区別あり，箱の中身は 1 個以上】
/*
* ボールが n 個，箱が m 個の場合，事前に配る考え方と重複組合せの考え方より
* ボールの入れ方は hom(m, n-m) = bin(n-1, m-1) 通りとなる．
*
* verify : https://onlinejudge.u-aizu.ac.jp/courses/library/7/DPL/all/DPL_5_F
*/


//【ボールの区別なし，箱の区別あり，箱の中身は k 個未満】
/*
* ボールが n 個，箱が m 個の場合，ボールの入れ方は以下の式で与えられる：
*	[z^n] (1-z^k)^m (1-z)^(-m)
* これは二項定理と負の二項定理を用いて O(n / k) で計算できる．
* 
* verify : https://atcoder.jp/contests/agc009/tasks/agc009_e
*/


//【ボールの区別あり，箱の区別なし，箱の中身は任意】
/*
* 箱の中身が 1 個以上のときの累積和をとればよい．
* 
* verify : https://onlinejudge.u-aizu.ac.jp/courses/library/7/DPL/all/DPL_5_G
*/


//【ボールの区別あり，箱の区別なし，箱の中身は 1 個以下】
/*
* ボールが n 個，箱が m 個の場合，ボールの入れ方は明らかに
* n <= k のとき 1 通り，n > k のとき 0 通りとなる．
* 
* verify : https://onlinejudge.u-aizu.ac.jp/courses/library/7/DPL/all/DPL_5_H
*/


//【集合の分割の数（ボールの区別あり，箱の区別なし，箱の中身は 1 個以上）】O(n^2)
/*
* 各 i ∈ [0..n], j ∈ [0..n] について，
* i 点集合をちょうど j 個に分割する方法の数を c[i][j] に格納する．
*/
void stirling_S2(int n, vvm& c) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/7/DPL/all/DPL_5_G

	//【方法】
	// c[i][j] : [0..i) をちょうど j 個に分割する方法の数，についての漸化式を作る．
	//
	// case 1. [0, i-1) が j 個に分割される場合：
	// i は j 個の集合のいずれかと抱合せになる．
	//
	// case 2. [0, i-1) が j-1 個に分割される場合：
	// i は単独で {i} という集合にしなければならない．
	//
	// これらをまとめて，漸化式
	//		c[i][j] = c[i - 1][j] * j + c[i - 1][j - 1]
	// を得る．

	// c[i][j] : [0..i) をちょうど j 個に分割する方法の数
	c = vvm(n + 1, vm(n + 1));
	c[0][0] = 1;

	repi(i, 1, n) repi(j, 1, n) c[i][j] = c[i - 1][j] * j + c[i - 1][j - 1];
}


//【集合の分割の数（第 2 種スターリング数）】O(k log n)
/*
* n 点集合をちょうど k 個に分割する方法の数 s(n, k) を返す．
*
*（ボールの区別あり，箱の区別なし，箱の中身は 1 個以上）
*
* 利用：【階乗など（法が大きな素数）】
*/
mint stirling_S2(int n, int k) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/7/DPL/all/DPL_5_G

	//【方法】
	// n 点集合から k 点集合への全射をとれば，
	// k 点集合の各元の逆像を集めたものが n 点集合の分割となる．
	// これら k 個の集合がどの元の逆像だったかを忘れれば良い．
	// 
	// なお全射の数は包除原理を用いれば数え上げられる．

	Factorial_mint fm(k);

	mint res = 0;
	repi(j, 1, k) {
		res += ((k - j) % 2 == 0 ? 1 : -1) * fm.binomial(k, j) * mint(j).pow(n);
	}
	res *= fm.factorial_inv(k);

	return res;
}


//【集合の分割の数（第 2 種スターリング数，mod998244353）】O(n log n)
/*
* n 点集合をちょうど k 個に分割する方法の数 s(n, k) を s[k] に格納する．
* 
*（ボールの区別あり，箱の区別なし，箱の中身は 1 個以上）
*
* 利用：【階乗など（法が大きな素数）】
*/
void stirling_S2(int n, vm& s) {
	// 参考 : https://ja.wikipedia.org/wiki/%E3%82%B9%E3%82%BF%E3%83%BC%E3%83%AA%E3%83%B3%E3%82%B0%E6%95%B0
	// verify : https://judge.yosupo.jp/problem/stirling_number_of_the_second_kind

	//【方法】
	// 第 2 種スターリング数の一般項は
	//		s(n, k)
	//		= (1/k!) Σm=[1..k] (-1)^(k-m) binomial(k, m) m^n
	//		= Σm=[1..k] ((-1)^(k-m) / (k-m)!) (m^n / m!)
	// と書け，これは畳込みの形である．

	vm f(n + 1), g(n + 1);
	Factorial_mint fm(n);

	repi(i, 0, n) {
		f[i] = (i & 1 ? -1 : 1) * fm.factorial_inv(i);
		g[i] = mint(i).pow(n) * fm.factorial_inv(i);
	}

	s = convolution(f, g);
	s.resize(n + 1);
}


//【自然数の分割の数（ボールの区別なし，箱の区別なし，箱の中身は任意）】O(n m)
/*
* 各 i ∈ [0..n], j ∈ [0..m] について，
* 自然数 i を j 個以下に分割する方法の数を c[i][j] に格納する．
*
* c[i][j] は，自然数 i を j 以下の自然数に分割する方法の数とも解釈できる．
*/
void count_integer_partitions(int n, int m, vvm& c) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/7/DPL/all/DPL_5_J

	//【方法 1】
	// case 1. ちょうど j 個に分割する場合：
	// j 個の自然数は 1 以上なので，それぞれから 1 を引くことで
	// 残り i - j の j 個以下への分割に対応させることができる．
	// ただし i >= j であるときに限る．
	//
	// case 2. j 個未満に分割する場合：
	// これは i を j - 1 個以下に分割するのと等価である．
	//
	// これらをまとめて，漸化式
	//		c[i][j] = c[i][j - 1] + (i >= j ? c[i - j][j] : 0)
	// を得る．

	//【方法 2】
	// 双対なヤング図形を考えることにより，
	//	 c[i][j] : 自然数 i を j 以下の自然数に分割する方法の数
	// とも解釈できるので，この解釈で漸化式を作る．
	// 
	// case 1. 分割に j を使う場合：
	// 残りの i - j を j 以下の自然数で分割すればよい．
	// ただし i >= j であるときに限る．
	//
	// case 2. 分割に j を使わない場合：
	// i を j - 1 以下の自然数で分割すればよい．
	//
	// これらをまとめて，漸化式
	//		c[i][j] = c[i][j - 1] + (i >= j ? c[i - j][j] : 0)
	// を得る．

	// なお，添字 j については直前しか必要ないので必要ならインライン化できる．

	// c[i][j] : 自然数 i を j 以下の自然数に分割する方法の数
	c = vvm(n + 1, vm(m + 1));
	repi(j, 0, m) c[0][j] = 1;

	repi(j, 1, m) repi(i, 1, n) c[i][j] = c[i][j - 1] + (i >= j ? c[i - j][j] : 0);
}


//【自然数の分割の数（個数上限付き）】O(n m)
/*
* 各 i ∈ [0..n], j ∈ [0..m] について，同じ自然数は k 個以下しか使えない条件で，
* 自然数 i を j 個以下に分割する方法の数を c[i][j] に格納する．
*
* c[i][j] は，次に大きい自然数との差が k 以下でなくてはならない条件で，
* 自然数 i を j 以下の自然数に分割する方法の数とも解釈できる．
*/
void count_cntlimited_integer_partitions(int n, int m, int k, vvm& c) {
	// verify : https://atcoder.jp/contests/abc221/tasks/abc221_h

	//【方法】
	// case 1. j 個に分割する場合：
	// j 個の自然数それぞれから 1 を引くことで，
	// 残りの i - j の j - k 個以上 j 個以下への分割に帰着する．
	//
	// case 2. j 個未満に分割する場合：
	// i を j - 1 個以下の自然数に分割することと等価である．
	//
	// これらをまとめて，漸化式
	//		c[i][j]
	//		 = c[i][j - 1] + Σt=[j-k..j] (i - j の t 個への分割)
	//		 = c[i][j - 1] + c[i - j][j] - c[i - j][j - k - 1]
	// を得る．（ただし無効な添字の値は 0 とする）

	// c[i][j] : 自然数 i を j 個以下に分割する方法の数
	c = vvm(n + 1, vm(m + 1));
	repi(j, 0, m) c[0][j] = 1;

	// 貰う DP
	repi(j, 1, m) repi(i, 1, n) {
		c[i][j] = c[i][j - 1];

		if (i - j >= 0) {
			c[i][j] += c[i - j][j];

			if (j - k - 1 >= 0) {
				c[i][j] -= c[i - j][j - k - 1];
			}
		}
	}
}


//【自然数の分割の数（大きさ上限付き）】O(n m d)
/*
* 各 i ∈ [0..n], j ∈ [0..m], k ∈ [0..d] について，
* 自然数 i を j 以下の自然数 k 個以下に分割する方法の数を c[i][j][k] に格納する．
*/
void count_maxlimited_integer_partitions(int n, int m, int d, vvvm& c) {
	//【方法】
	// case 1. 分割に j を使う場合：
	// 残りの i - j を j 以下の自然数 k - 1 個以下で分割すればよい．
	// ただし i >= j かつ k >= 1 であるときに限る．
	//
	// case 2. 分割に j を使わない場合：
	// i を j - 1 以下の自然数 k 個以下で分割すればよい．
	//
	// これらをまとめて，漸化式
	//		dp[j][k][i] = dp[j - 1][k][i] + ((i >= j && k >= 1) ? dp[j][k - 1][i - j] : 0)
	// を得る．

	// なお，添字 j については直前しか必要ないので必要ならインライン化できる．

	// c[i][j][k] : 自然数 i を j 以下の自然数 k 個以下に分割する方法の数
	c = vvvm(n + 1, vvm(m + 1, vm(d + 1)));
	repi(j, 0, m) repi(k, 0, d) c[0][j][k] = 1;

	repi(j, 1, m) repi(k, 1, d) repi(i, 1, n) {
		c[i][j][k] = c[i][j - 1][k] + (i >= j ? c[i - j][j][k - 1] : 0);
	}
}


//【分割数】O(n)
/*
* 各 i ∈ [0..n] について自然数 i を分割する方法の数を p[i] に格納する．
*/
void partition_function(int n, vm& p) {
	// 参考 : https://ja.wikipedia.org/wiki/%E5%88%86%E5%89%B2%E6%95%B0
	// verify : https://judge.yosupo.jp/problem/partition_function

	// pen : 一般五角数の昇順列
	vi pen;
	repi(i, 1, n) {
		pen.push_back(i * (3 * i - 1) / 2);
		pen.push_back(i * (3 * i + 1) / 2);
		if (*pen.rbegin() > n) break;
	}
	int m = sz(pen);

	// 漸化式により計算する．
	p = vm(n + 1);
	p[0] = 1;

	repi(i, 1, n) rep(j, m) {
		if (i - pen[j] < 0) break;

		// 符号は 4 で割った余りで場合分けされる．
		p[i] += (j & 2 ? -1 : 1) * p[i - pen[j]];
	}
}


//【ボールの区別なし，箱の区別なし，箱の中身は 1 個以下】
/*
* ボールが n 個，箱が m 個の場合，ボールの入れ方は明らかに
* n <= k のとき 1 通り，n > k のとき 0 通りとなる．
* 
* verify : https://onlinejudge.u-aizu.ac.jp/courses/library/7/DPL/all/DPL_5_K
*/


//【ボールの区別なし，箱の区別なし，箱の中身は 1 個以上】
/*
* 箱の中身が任意のときの結果の差分を取ればよい．
* 
* verify : https://onlinejudge.u-aizu.ac.jp/courses/library/7/DPL/all/DPL_5_L
*/


