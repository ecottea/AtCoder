#pragma once
#include "header.h"
// ■■■■■ 母関数 ■■■■■


//【母関数の積と畳込み】
/*
*	[z^k] f(z) = a[k]
*	[z^k] g(z) = b[k]
* のとき，以下が成り立つ：
*	[z^n] f(z) g(z) = Σk∈[0..n] a[k] b[n-k]
*/


//【母関数の積と畳込み（複数個）】
/*
*	[z^k] f_i(z) = a_i[k]  (i∈[0..m))
* のとき，以下が成り立つ：
*	[z^n] Πi∈[0..m) f_i(z) = Σk[0..m) a_i[k[i]]  (Σk[0..m) = n)
*/


//【指数型母関数の積と畳込み】
/*
*	k! [z^k] f(z) = a[k]
*	k! [z^k] g(z) = b[k]
* のとき，以下が成り立つ：
*	n! [z^n] f(z) g(z) = Σk∈[0..n] bin(n, k) a[k] b[n-k]
* 
* 畳込みの形に二項係数が掛かっていて邪魔になったときに使える．
* 
* verify : https://atcoder.jp/contests/abc217/tasks/abc217_g
*/


//【指数型母関数の積と畳込み（複数個）】
/*
*	k! [z^k] f_i(z) = a_i[k]  (i∈[0..m))
* のとき，bin(n, k[0..m)) を多項係数として以下が成り立つ：
*	n! [z^n] Πi∈[0..m) f_i(z) = Σk[0..m) bin(n, k[0..m)) a_i[k[i]]  (Σk[0..m) = n)
* 
* 複数畳込みの形に多項係数が掛かっていて邪魔になったときに使える．
* 
* verify : https://atcoder.jp/contests/abc260/tasks/abc260_h
*/


//【二項定理（bin(n, i) の母関数）】
/*
* [z^i] (1+z)^n = bin(n, i)
* 
* verify : https://atcoder.jp/contests/agc009/tasks/agc009_e
*/


//【二項係数の畳込み】
/*
* Σk bin(a, c+k) bin(b, d-k) = bin(a+b, c+d)  (k∈[max(-c, d-b)..min(a-c, d)])
* Σk bin(a, c+k) bin(b, d+k) = bin(a+b, b+c-d)  (k∈[max(-c, -d)..min(a-c, b-d)])
* 
* 証明：上式は両辺とも a+b 個から c+d 個選ぶ場合の数に等しいことから従う．
* 下式は bin(b, d+k) = bin(b, b-d-k) と上式から従う．
* 
* verfy : https://atcoder.jp/contests/arc144/tasks/arc144_d
*/


//【負の二項定理】
/*
* [z^i] (1-z)^(-n) = bin(n-1+i, i)
*
* verify : https://atcoder.jp/contests/agc009/tasks/agc009_e
*/


//【bin(i, r) の母関数】
/*
* [z^i] z^r (1-z)^(-r-1) = bin(i, r)
*
* 証明：右辺を負の二項定理を用いて計算すると次のように左辺に一致する：
*	bin(i, r)
*	= bin(i, i - r)
*	= bin(j + r, j)  (j = i - r とおいた)
*	= bin(r + 1 - 1 + j, j)
*	= [z^j] (1-z)^(-(r+1))  (負の二項定理より)
*	= [z^(i-r)] (1-z)^(-r-1)  (j = i - r より)
*	= [z^i] z^r (1-z)^(-r-1)
*
* verify : https://atcoder.jp/contests/arc144/tasks/arc144_d
*/


//【1 個おきの係数和】
/*
* f(z) = Σi a[i] z^i について，
*	Σi:偶数 a[i] z^i = (f(z) + f(-z)) / 2
*	Σi:奇数 a[i] z^i = (f(z) - f(-z)) / 2
*
* verify : https://atcoder.jp/contests/code-festival-2014-morning-middle/tasks/code_festival_morning_med_c
*/


//【ラグランジュの反転公式】
/*
* f(z) と g(z) が互いに逆関数であり，条件
*	[z^0]f(z) = 0,  [z^0]g(z) = 0,
*	[z^1]f(z) != 0, [z^1]g(z) != 0
* を満たすとき，以下の等式が成り立つ：
*	[z^n]f(z) = (1/n) [z^(n-1)]((z / g(z))^n)
*
*（使い所）
* f(z) = (z の式) とは書けていないが z = (f(z) の式) という表示が得られている場合．
* 特に木の数え上げにおいて有用である．
*
* verify : https://atcoder.jp/contests/abc222/tasks/abc222_h
*/


