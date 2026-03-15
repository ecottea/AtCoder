#pragma once
#include "header.h"
#include "二項係数.h"
#include "包含.h"
// ■■■■■ 写像 12 相とその類題 ■■■■■


//【写像の数（ボールの区別あり，箱の区別あり，箱の中身は任意）】
/*
* n 点集合から m 点集合への写像の数は，重複順列の考え方より m^n 通りとなる．
* 
* verify : https://onlinejudge.u-aizu.ac.jp/courses/library/7/DPL/all/DPL_5_A
*/


//【単射の数（ボールの区別あり，箱の区別あり，箱の中身は 1 個以下）】
/*
* n 点集合から m 点集合への単射の数は，順列の考え方より perm(m, n) 通りとなる．
* 
* verify : https://onlinejudge.u-aizu.ac.jp/courses/library/7/DPL/all/DPL_5_B
*/


//【全射の数（ボールの区別あり，箱の区別あり，箱の中身は 1 個以上，n,m 一括）】O(n m)
/*
* 各 i∈[0..n], j∈[0..m] について，i 点集合から j 点集合への全射の数を c[i][j] に格納し c を返す．
*/
vvm count_surjections_all_N_M(int n, int m) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/7/DPL/all/DPL_5_C

	//【方法】
	// c[i][j] : [0..i) から [0..j) への全射の数，についての漸化式を作る．
	//
	// case 1. [0..i-1) の行き先が [0..j) のとき：
	// i の行き先は [0..j) の j 通りから自由に選べる．
	//
	// case 2. [0..i-1) の行き先が [0..j) でないとき：
	// [0..i-1) の行き先は [0..j) からある元 j0 を除いたものでなければならない．
	// その j0 の選び方が j 通りあり，i の行き先は j0 に限定される．
	//
	// これらをまとめて，漸化式
	//		c[i][j] = c[i-1][j] * j + c[i-1][j-1] * j
	// を得る．

	//【備考】
	// c[i][j] = s(i, j) * j! である（s(i, j) は第二種スターリング数）

	// c[i][j] : [0, i) から [0, j) への全射の数
	vvm c(n + 1, vm(m + 1));
	c[0][0] = 1;

	repi(i, 1, n) repi(j, 1, m) c[i][j] = (c[i - 1][j] + c[i - 1][j - 1]) * j;

	return c;
}


//【全射の数（ボールの区別あり，箱の区別あり，箱の中身は 1 個以上）】O(m log n)
/*
* n 点集合から m 点集合への全射の数を返す．
*
* 制約：fm は m! まで計算可能
*/
mint count_surjections(ll n, int m, const Factorial_mint& fm) {
	// verify : https://yukicoder.me/problems/no/391

	//【方法】
	// 包除原理を用いて数え上げる．
	//
	// n 点集合から m 点集合への写像のうち，特定の j 元以外には行かないものは j^n 通りあり，
	// 特定の j 元の選び方は bin(m, j) 通りある．
	//
	// j = m として数えたいが，足しすぎたので j = m - 1 のものは全て引かなければならず，
	// 引きすぎたので j = m - 2 のものは全て足さなければならず，...... と続けると，答えは
	//		Σj∈[0..m] (-1)^(m-j) bin(m, j) j^n 通り
	// である．

	mint res = 0;
	repi(j, 0, m) {
		res += ((m - j) % 2 == 0 ? 1 : -1) * fm.bin(m, j) * mint(j).pow(n);
	}

	return res;
}


//【全射の数（ボールの区別あり，箱の区別あり，箱の中身は 1 個以上，m 一括, mod 998244353）】
/*
* 各 j∈[0..m] について，n 点集合から j 点集合への全射の数を格納したリストを返す．
*
* 制約 : fm は m! まで計算可能
*
* 利用：【下位集合メビウス変換（大きさ依存, mod 998244353）】
*/
vm count_surjections_all_M(ll n, int m, const Factorial_mint& fm) {
	// verify: https://yukicoder.me/problems/no/1100

	//【方法】
	// n 点集合から m 点集合への写像のうち，特定の j 要素のみを射つものは j^n 通りである．
	// これに対して大きさ依存の下位集合メビウス変換を適用すれば，
	// n 点集合から m 点集合への写像のうち，特定の j 要素のみを漏れなく射つものの個数が求まる．
	// これは n 点集合から j 点集合への全射の数にほかならない．

	vm g(m + 1);
	repi(j, 0, m) g[j] = mint(j).pow(n);

	vm f = set_submobius_size(g, fm);

	return f;
}


//【自然数の順序付き分割の数（ボールの区別なし，箱の区別あり，箱の中身は任意）】
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
* これは二項定理と負の二項定理を用いて O(n/k) で計算できる．
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
* n ≦ k のとき 1 通り，n > k のとき 0 通りとなる．
* 
* verify : https://onlinejudge.u-aizu.ac.jp/courses/library/7/DPL/all/DPL_5_H
*/


//【集合の分割の数（ボールの区別あり，箱の区別なし，箱の中身は 1 個以上）】O(n m)
/*
* 各 i∈[0..n], j∈[0..m] について，
* i 点集合をちょうど j 個に分割する方法の数を格納した二次元リストを返す．
*/
vvm stirling_S2_all_N_K(int n, int m) {
	// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/7/DPL/all/DPL_5_G

	//【方法】
	// c[i][j] : [0..i) をちょうど j 個に分割する方法の数，についての漸化式を作る．
	//
	// case 1. [0, i-1) が j 個に分割される場合：
	// i は j 個の集合（区別はつく）のいずれかと抱合せになる．
	//
	// case 2. [0, i-1) が j-1 個に分割される場合：
	// i は単独で {i} という集合にしなければならない．
	//
	// これらをまとめて，漸化式
	//		c[i][j] = c[i - 1][j] * j + c[i - 1][j - 1]
	// を得る．

	// c[i][j] : [0..i) をちょうど j 個に分割する方法の数
	vvm c(n + 1, vm(m + 1));
	c[0][0] = 1;

	repi(i, 1, n) repi(j, 1, m) c[i][j] = c[i - 1][j] * j + c[i - 1][j - 1];

	return c;
}


//【集合の分割の数（ボールの区別あり，箱の区別なし，箱の中身は 1 個以上）】O(k log n)
/*
* n 点集合をちょうど k 個に分割する方法の数 s(n, k) を返す．
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
		res += ((k - j) % 2 == 0 ? 1 : -1) * fm.bin(k, j) * mint(j).pow(n);
	}
	res *= fm.fact_inv(k);

	return res;
}


//【集合の分割の数（ボールの区別あり，箱の区別なし，箱の中身は 1 個以上，mod 998244353）】O(n log n)
/*
* 各 j∈[0..n] について，n 点集合をちょうど k 個に分割する方法の数 s(n, k) を格納したリストを返す．
*
* 利用：【階乗など（法が大きな素数）】
*/
vm stirling_S2_allK(int n) {
	// 参考 : https://ja.wikipedia.org/wiki/%E3%82%B9%E3%82%BF%E3%83%BC%E3%83%AA%E3%83%B3%E3%82%B0%E6%95%B0
	// verify : https://judge.yosupo.jp/problem/stirling_number_of_the_second_kind

	//【方法】
	// 第 2 種スターリング数の一般項は
	//		s(n, k)
	//		= (1/k!) Σm=[1..k] (-1)^(k-m) bin(k, m) m^n
	//		= Σm=[1..k] ((-1)^(k-m) / (k-m)!) (m^n / m!)
	// と書け，これは畳込みの形である．

	vm f(n + 1), g(n + 1);
	Factorial_mint fm(n);

	repi(i, 0, n) {
		f[i] = (i & 1 ? -1 : 1) * fm.fact_inv(i);
		g[i] = mint(i).pow(n) * fm.fact_inv(i);
	}

	vm s = convolution(f, g);
	s.resize(n + 1);

	return s;
}


//【自然数の分割の数（ボールの区別なし，箱の区別なし，箱の中身は任意）】O(n m)
/*
* 各 i∈[0..n], j∈[0..m] について，
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
	// ただし i ≧ j であるときに限る．
	//
	// case 2. j 個未満に分割する場合：
	// これは i を j - 1 個以下に分割するのと等価である．
	//
	// これらをまとめて，漸化式
	//		c[i][j] = c[i][j - 1] + (i ≧ j ? c[i - j][j] : 0)
	// を得る．

	//【方法 2】
	// ヤング図形の転置を考えることにより，
	//	 c[i][j] : 自然数 i を j 以下の自然数に分割する方法の数
	// とも解釈できるので，この解釈で漸化式を作る．
	// 
	// case 1. 分割に j を使う場合：
	// 残りの i - j を j 以下の自然数で分割すればよい．
	// ただし i ≧ j であるときに限る．
	//
	// case 2. 分割に j を使わない場合：
	// i を j - 1 以下の自然数で分割すればよい．
	//
	// これらをまとめて，漸化式
	//		c[i][j] = c[i][j - 1] + (i ≧ j ? c[i - j][j] : 0)
	// を得る．

	// なお，添字 j については直前しか必要ないので必要ならインライン化できる．

	// c[i][j] : 自然数 i を j 以下の自然数に分割する方法の数
	c = vvm(n + 1, vm(m + 1));
	repi(j, 0, m) c[0][j] = 1;

	repi(j, 1, m) repi(i, 1, n) c[i][j] = c[i][j - 1] + (i >= j ? c[i - j][j] : 0);
}


//【自然数の分割の数（個数上限付き）】O(n m)
/*
* 各 i∈[0..n], j∈[0..m] について，同じ自然数は k 個以下しか使えない条件で，
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
* 各 i∈[0..n], j∈[0..m], k∈[0..d] について，
* 自然数 i を j 以下の自然数 k 個以下に分割する方法の数を c[i][j][k] に格納する．
*/
void count_maxlimited_integer_partitions(int n, int m, int d, vvvm& c) {
	//【方法】
	// case 1. 分割に j を使う場合：
	// 残りの i - j を j 以下の自然数 k - 1 個以下で分割すればよい．
	// ただし i ≧ j かつ k ≧ 1 であるときに限る．
	//
	// case 2. 分割に j を使わない場合：
	// i を j - 1 以下の自然数 k 個以下で分割すればよい．
	//
	// これらをまとめて，漸化式
	//		dp[j][k][i] = dp[j - 1][k][i] + ((i ≧ j && k ≧ 1) ? dp[j][k - 1][i - j] : 0)
	// を得る．

	// なお，添字 j については直前しか必要ないので必要ならインライン化できる．

	// c[i][j][k] : 自然数 i を j 以下の自然数 k 個以下に分割する方法の数
	c = vvvm(n + 1, vvm(m + 1, vm(d + 1)));
	repi(j, 0, m) repi(k, 0, d) c[0][j][k] = 1;

	repi(j, 1, m) repi(k, 1, d) repi(i, 1, n) {
		c[i][j][k] = c[i][j - 1][k] + (i >= j ? c[i - j][j][k - 1] : 0);
	}
}


//【ボールの区別なし，箱の区別なし，箱の中身は 1 個以下】
/*
* ボールが n 個，箱が m 個の場合，ボールの入れ方は明らかに
* n ≦ k のとき 1 通り，n > k のとき 0 通りとなる．
* 
* verify : https://onlinejudge.u-aizu.ac.jp/courses/library/7/DPL/all/DPL_5_K
*/


//【ボールの区別なし，箱の区別なし，箱の中身は 1 個以上】
/*
* 箱の中身が任意のときの結果の差分を取ればよい．
* 
* verify : https://onlinejudge.u-aizu.ac.jp/courses/library/7/DPL/all/DPL_5_L
*/


