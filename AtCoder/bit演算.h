#pragma once
#include "header.h"
// ■■■■■ ビット演算 ■■■■■


//【ビット演算の別表現】
/*
* XOR : 体 F2 上での加算[減算]
* NOT : 体 F2 上での 1 からの減算[インクリメント, デクリメント]
* OR  : 全順序集合 {0, 1} 上での max
* AND : 全順序集合 {0, 1} 上での min
*/


//【XOR と 和 の関係】
/*
* 非負整数 a, b について，a XOR b は 2 進法で繰り上がりを捨てて計算した a + b に等しい：
*	a + b = (a XOR b) + 2(a AND b)
* 
* verify : https://atcoder.jp/contests/abc172/tasks/abc172_f
*/


//【XOR と 差 の関係】
/*
* 非負整数 a, b について，a XOR b は 2 進法で繰り下がりを戻して計算した a - b に等しい：
*	a - b = (a XOR b) - 2(~a AND b)
* 
* verify : https://atcoder.jp/contests/toyota2023spring-final-open/tasks/toyota2023spring_final_c
*/


//【多重集合の一斉 XOR】
/*
* 多重集合 a[0..n) について，操作
*		i∈[0..n) を選び，a[0..n) - {a[i]} ^= a[i]
* が任意の回数行えるとき，操作の回数を 1 回以下に制限しても遷移可能な全体は変わらない．
* 
* verify : https://atcoder.jp/contests/agc052/tasks/agc052_b
*/


//【OR, AND と和の関係】
/*
* a + b = (a OR b) + (a AND b)
*
* 証明：ビット毎に a + b = min(a, b) + max(a, b) を適用する．
*
* verify : https://atcoder.jp/contests/abc238/tasks/abc238_d
*/


//【連続自然数の部分集合の OR】
/*
* [1..n] の部分集合の OR で作れる数の集合は ub=2^(msb(n)+1) として [1..ub) である．
*
* verify : https://atcoder.jp/contests/agc015/tasks/agc015_d
*/


//【popcount の別表現】
/*
* popcount(x) = x - Σk∈[1..∞) floor(x / 2^k)
* 
* verify : https://atcoder.jp/contests/abc283/tasks/abc283_h
*/


//【繰り上がりビット】O(log max(x, y))
/*
* 非負整数 x, y の 2 進法での加算で繰り上がりの起こるビットだけを 1 にした数を返す．
*/
template <class T>
T get_carry(T x, T y) {
	// verify : https://atcoder.jp/contests/arc156/tasks/arc156_d

	T res = 0;

	while (y != 0) {
		T ny = x & y;
		res |= ny;
		x ^= y;
		y = ny << 1;
	}

	return res;
}


//【最上位ビット】
/*
* n の最上位ビットの位置を返す．
*/
inline int msb(__int128 n) { return (n >> 64) != 0 ? (127 - __builtin_clzll((ll)(n >> 64))) : n != 0 ? (63 - __builtin_clzll((ll)(n))) : -1; }


