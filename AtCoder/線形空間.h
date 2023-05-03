#pragma once
#include "header.h"
#include "二項係数.h"
// ■■■■■ F_q 上線形空間の部分空間束のゼータ変換・メビウス変換 ■■■■■


//【部分空間関数】
/*
* 線形空間 (F_q)^N の部分空間全体を定義域とする関数を (F_q)^N 上の部分空間関数とよぶ．
*/


//【下位メビウス変換（次元依存，mod 998244353）】O(N log N)
/*
* (F_q)^N 上部分空間関数 f が V の次元 dim V のみに依存する関数を用いて
*		f(V) = f[dim V]
* と書けるとする．このとき f(V) の下位部分空間からの累積和
*		g(V) = ΣW⊂V f(V)
* もまた関数 g[dim V] を用いて
*		g(V) = g[dim V]
* と書ける．与えられた g[0..N] に対応する f[0..N] を返す．
* 
* 具体的には
*		f[n] = Σd∈[0..n] q-bin(n, d) (-1)^(n-d) q^((n-d)(n-d-1)/2) g[d]
* で表される．
*
* 制約：qfm は [N]_q まで計算可能．
*/
vm less_subspace_mobius(const vm& g, const Q_Factorial_mint& qfm) {
	// verify : https://atcoder.jp/contests/abc278/tasks/abc278_h

	//【方法】
	// f[n] の式を
	//		f[n] / [n]_q! = Σd∈[0..n] ((-1)^(n-d) q^((n-d)(n-d-1)/2) / [n-d]_q!) (g[d] / [d]_q!)
	// と書き直せば畳込みで計算できる．

	int N = sz(g) - 1, q = qfm.get_q();

	// a[d] : (-1)^d q^(d(d-1)/2) / [d]_q!
	vm a(N + 1, 1);

	// pow_q : q^d, ppow_q : q^(d(d-1)/2)
	mint pow_q = 1, ppow_q = 1;
	repi(d, 0, N) {
		a[d] = (d % 2 ? -1 : 1) * ppow_q * qfm.fact_inv(d);
		ppow_q *= pow_q;
		pow_q *= q;
	}

	// b[d] : g[d] / [d]_q!
	vm b(N + 1);
	repi(d, 0, N) b[d] = g[d] * qfm.fact_inv(d);

	vm c = convolution(a, b);
	c.resize(N + 1);
	repi(d, 0, N) c[d] *= qfm.fact(d);

	return c;
}


//【下位メビウス変換（(定数)^(次元) 依存）】
/*
* (F_q)^N 上部分空間関数 f の下位部分空間からの累積和
*		g(V) = ΣW⊂V f(V)
* が V の次元 dim V とある定数 C のみを用いて
*		g(V) = C^(dim V)
* と書けるとき，f(V) は
*		f(V)
*		= C^d (1/C; q)_d （d = dim V）
*		= Πk∈[0..d) (C - q^k)
* と表される．
* 
* verify : https://atcoder.jp/contests/abc278/tasks/abc278_h
*/


