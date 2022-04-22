#pragma once
#include "header.h"
// ■■■■■ 順列に関する組合せ論的問題 ■■■■■


//【攪乱順列の数（モンモール数）】O(n)
/*
* i=[0..n] について，i 番目のモンモール数を mon[i] に格納する．
* mon[i] は長さ i の攪乱順列の数と解釈できる．
*/
void montmort_number(int n, vm& mon) {
	// 参考 : https://ja.wikipedia.org/wiki/%E5%AE%8C%E5%85%A8%E9%A0%86%E5%88%97
	// verify : https://judge.yosupo.jp/problem/montmort_number_mod

	//【方法】
	// モンモール数は以下の 2 項間漸化式を満たす：
	//		mon[n] = n mon[n - 1] + (-1)^n

	mon.resize(n + 1);
	mon[0] = 1;
	repi(i, 1, n) mon[i] = mon[i - 1] * i + (i & 1 ? -1 : 1);
}


//【menage numbers】
/*
* 順列 p[0..n) で，任意の i∈[0..n) について
*		p[i] != i かつ p[i] != i + 1 (mod n)
* を満たすものの個数は
*		Σk=[0..n] (-1)^k 2n/(2n-k) bin(2n-k, k) (n-k)!
* である．
* 
*（証明）
* 包除原理を用いる．
* 条件に違反する箇所が k 箇所以上であるような順列の数は，
* サイクルグラフ C_2n の大きさ k のマッチングの個数 2n/(2n-k) bin(2n-k, k) と，
* マッチングに属さない p[i] の選び方 (n-k)! との積に等しい．
* 
* 参考 : http://oeis.org/A000179
* verify : https://atcoder.jp/contests/abc214/tasks/abc214_g
*/


//【順列の数（p[i] != i, p[i] != i + 1）】
/*
* 順列 p[0..n) で，任意の i∈[0..n) について
*		p[i] != i かつ p[i] != i + 1 (i < n - 1)
* を満たすものの個数は
*		Σk=[0..n] (-1)^k bin(2n-k, k) (n-k)!
* である．
*
*（証明）
* 包除原理を用いる．
* 条件に違反する箇所が k 箇所以上であるような順列の数は，
* パス P_2n の大きさ k のマッチングの個数 bin(2n-k, k) と，
* マッチングに属さない p[i] の選び方 (n-k)! との積に等しい．
*
* 参考 : http://oeis.org/A000271
* verify : https://atcoder.jp/contests/abc214/tasks/abc214_g
*/


//【第 1 種スターリング数】
/*
* 順列 p[0..n) で k 個の巡回置換の積で表されるものの個数は S(n, k) である．
* ここで S(n, k) は第 1 種スターリング数である．
*/

