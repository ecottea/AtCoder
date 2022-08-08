#pragma once
#include "header.h"
// ■■■■■ ビット演算 ■■■■■


//【ビット演算の別の解釈】
/*
* XOR : 体 F2 上での加算[減算]
* NOT : 体 F2 上での 1 からの減算[インクリメント, デクリメント]
* OR  : 順序集合 {0, 1} 上での max
* AND : 順序集合 {0, 1} 上での min
*/


//【XOR と 和 の関係】
/*
* a XOR b は，2 進法で繰り上がりを捨てて計算した a + b に等しい．
* 特に以下の式が成り立つ：
*	a + b = (a XOR b) + 2(a AND b)
* 
* verify : https://atcoder.jp/contests/abc172/tasks/abc172_f
*/


//【OR, AND と和の関係】
/*
* a + b = (a OR b) + (a AND b)
* 
* 証明：ビット毎に a + b = min(a, b) + max(a, b) を適用する．
* 
* verify : https://atcoder.jp/contests/abc238/tasks/abc238_d
*/


//【連続自然数の XOR】
/*
* XOR[0..n) を返す．
*/
ll acc_XOR(ll n) {
	// verify : https://atcoder.jp/contests/abc121/tasks/abc121_d

	//【方法】
	// s[i] := XOR[0..i) とおく．
	//		(2 k) XOR (2 k + 1) = 1
	// に注意すると，s[i] が周期 4 をもつことが分かり，
	//		s[4 k]     = 0
	//		s[4 k + 1] = 4 k
	//		s[4 k + 2] = 1
	//		s[4 k + 3] = 4 k + 3
	// と簡単に表すことができる．

	if (n <= 0) return 0;

	ll res = -1;

	if (n % 4 == 0) res = 0;
	else if (n % 4 == 1) res = n - 1;
	else if (n % 4 == 2) res = 1;
	else if (n % 4 == 3) res = n;

	return res;
}


//【連続自然数の部分集合の OR】
/*
* [1..n] の部分集合の OR で作れる数の集合は ub=2^(msb(n)+1) として [1..ub) である．
* 
* verify : https://atcoder.jp/contests/agc015/tasks/agc015_d
*/


//【多重集合の一斉 XOR】
/*
* 多重集合 a[0..n) について，操作
*		i∈[0..n) を選び，a[0..n) - {a[i]} ^= a[i]
* が任意の回数行えるとき，操作の回数を 1 回以下に制限しても遷移可能な全体は変わらない．
* 
* verify : https://atcoder.jp/contests/agc052/tasks/agc052_b
*/


