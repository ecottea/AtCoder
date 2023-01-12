#pragma once
#include "header.h"
// ■■■■■ 母関数 ■■■■■


//【通常型母関数】
/*
* 数列 a[0..∞) の通常型母関数が
*		A(z) = Σn∈[0..∞) a[n] z^n
* とであることを
*		a[n] ⇔ A(z)
* と表す．
* 
* 畳込み		: Σi∈[0..n] a[i] b[n-i] ⇔ A(z) B(z)
* 複数畳込み	: Σ_(i+j+k=n) a[i] b[j] c[k] ⇔ A(z) B(z) C(z)
* n 倍		: n a[n] ⇔ z A'(z)
* 1/n 倍		: a[n]/n ⇔ ∫ A(z)/z dz（ただし a[0]=0）
* 右シフト	: a[n-1] ⇔ z A(z)
* 左シフト	: a[n+1] ⇔ A(z)/z（ただし a[0]=0）
* 倍右シフト	: (n+1) a[n+1] ⇔ A'(z)
* 商左シフト : a[n-1]/n ⇔ ∫ A(z) dz（ただし a[0]=0）
* 累積和		: Σa[0..n] ⇔ A(z)/(1-z)
* 差分		: a[n]-a[n-1] ⇔ (1-z) A(z)（ただし a[-1]=0 とみなす）
*/


//【指数型母関数】
/*
* 数列 a[0..∞) の指数型母関数が
*		A(z) = Σn∈[0..∞) a[n]/n! z^n
* とであることを
*		a[n] ⇔ A(z)
* と表す．
*
* bin畳込み	: Σi∈[0..n] bin(n,i) a[i] b[n-i] ⇔ A(z) B(z)
* mul畳込み	: Σ_(i+j+k=n) mul(n,[i,j,k]) a[i] b[j] c[k] ⇔ A(z) B(z) C(z)
* n 倍		: n a[n] ⇔ z A'(z)
* 1/n 倍		: a[n]/n ⇔ ∫ A(z)/z dz（ただし a[0]=0）
* 右シフト	: a[n-1] ⇔ ∫ A(z) dz（ただし a[0]=0）
* 左シフト	: a[n+1] ⇔ A'(z)
* 倍右シフト	: n a[n-1] ⇔ z A(z)
* 商左シフト	: a[n+1]/(n+1) ⇔ A(z)/z（ただし a[0]=0）
* 二項累積和	: Σi∈[0..n] bin(n,i) a[i] ⇔ exp(z) A(z)
* 二項差分	: Σi∈[0..n] (-1)^(n-i) bin(n,i) a[i] ⇔ A(z) / exp(z)
* 分割累積和	: Σp∈([1..n]の分割) Πset∈p a[|set|] ⇔ exp(A(z))（ただし a[0]=0）
*
* 畳込みの形に二項係数が掛かっていて邪魔になったときに使える．
* 特にラベル付きの対象の数え上げで有用である．
* 
* 参考 : https://37zigen.com/exponential-generating-function/
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


//【負の二項定理（bin(n+i, i) の母関数）】
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


