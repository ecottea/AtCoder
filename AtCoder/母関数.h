#pragma once
#include "header.h"
#include "二項係数.h"
#include "FPS(mint).h"
// ■■■■■ 母関数 ■■■■■


//【通常型母関数】
/*
* 数列 a[0..∞) の通常型母関数が
*		A(z) = Σn∈[0..∞) a[n] z^n
* とであることを
*		a[n] ⇔ A(z)
* と表す．
* 
* 畳込み		: Σi∈[0..n] a[i] b[n-i]  ⇔  A(z) B(z)
* 複数畳込み	: ΣΣ_(i+j+k=n) a[i] b[j] c[k]  ⇔  A(z) B(z) C(z)
* n 倍		: n a[n]  ⇔  z A'(z)
* 1/n 倍		: a[n]/n  ⇔  ∫ A(z)/z dz（ただし a[0]=0）
* 右シフト	: a[n-1]  ⇔  z A(z)
* 左シフト	: a[n+1]  ⇔  A(z)/z（ただし a[0]=0）
* 倍右シフト	: (n+1) a[n+1]  ⇔  A'(z)
* 商左シフト : a[n-1]/n  ⇔  ∫ A(z) dz（ただし a[0]=0）
* 累積和		: Σa[0..n]  ⇔  A(z)/(1-z)
* 差分		: a[n]-a[n-1]  ⇔  (1-z) A(z)（ただし a[-1]=0 とみなす）
* 分割累積和	: Σλ∈(nの分割) Πi∈λ a[i]  ⇔  exp( Σk∈[1..∞) 1/k A(z^k) )（ただし a[0]=0）
* 区間累積和 : Σσ∈([1..n]の区間分割) ΠI∈σ a[|I|]  ⇔  1/(1-A(z))（ただし a[0]=0）
* 二項変換	: Σi∈[0..n] bin(n,i) a[i]  ⇔  1/(1-z) A(z/(1-z))
*/


//【指数型母関数】
/*
* 数列 a[0..∞) の指数型母関数が
*		A(z) = Σn∈[0..∞) a[n]/n! z^n
* とであることを
*		a[n] ⇔ A(z)
* と表す．
*
* bin畳込み	: Σi∈[0..n] bin(n,i) a[i] b[n-i]  ⇔  A(z) B(z)
* mul畳込み	: ΣΣ_(i+j+k=n) mul(n,[i,j,k]) a[i] b[j] c[k]  ⇔  A(z) B(z) C(z)
* n 倍		: n a[n]  ⇔  z A'(z)
* 1/n 倍		: a[n]/n  ⇔  ∫ A(z)/z dz（ただし a[0]=0）
* 右シフト	: a[n-1]  ⇔  ∫ A(z) dz（ただし a[0]=0）
* 左シフト	: a[n+1]  ⇔  A'(z)
* 倍右シフト	: n a[n-1]  ⇔  z A(z)
* 商左シフト	: a[n+1]/(n+1)  ⇔  A(z)/z（ただし a[0]=0）
* 二項変換	: Σi∈[0..n] bin(n,i) a[i]  ⇔  exp(z) A(z)
* 逆二項変換	: Σi∈[0..n] (-1)^(n-i) bin(n,i) a[i]  ⇔  exp(-z) A(z)
* 分割累積和	: Σπ∈([1..n]の分割) Πset∈π a[|set|]  ⇔  exp(A(z))（ただし a[0]=0）
* 区間累積和 : Σσ∈([1..n]の区間分割) mul(n,|σ|) ΠI∈σ a[|I|]  ⇔  1/(1-A(z))（ただし a[0]=0）
*
* 畳込みの形に二項係数が掛かっていて邪魔になったときに使える．
* 特にラベル付きの対象の数え上げで有用である．
* 
* 参考 : https://37zigen.com/exponential-generating-function/
*/


//【多変数母関数】
/*
* 多変数母関数がそのまま役に立つことはあまりないが，因数分解して変数分離できる場合は役に立つ．
* 
* 例えばマンハッタン距離に対する 45° 回転は，
*	s + 1/s + t + 1/t = (s + t) (1 + 1/st)
* なる因数分解に対応する．
* verify : https://atcoder.jp/contests/abc240/tasks/abc240_g
*/


//【二項定理（bin(n, i) の母関数）】
/*
* [z^i] (1+z)^n = bin(n, i)
* 
* verify : https://atcoder.jp/contests/agc009/tasks/agc009_e
*/


//【二項係数の畳込み】
/*
* Σk bin(a, c+k) bin(b, d-k) = bin(a+b, c+d)    (k∈[max(-c, d-b)..min(a-c, d)])
* Σk bin(a, c+k) bin(b, d+k) = bin(a+b, b+c-d)  (k∈[max(-c, -d)..min(a-c, b-d)])
* 
* 証明：上式は両辺とも a+b 個から c+d 個選ぶ場合の数に等しいことから従う．
* 下式は bin(b, d+k) = bin(b, b-d-k) と上式から従う．
* 
* verfy : https://atcoder.jp/contests/arc144/tasks/arc144_d
*/


//【二項係数の二乗和】
/*
* 二項係数の二乗和は中央二項係数になる．すなわち以下の式が成り立つ：
*	Σr∈[0..n] bin(n, r)^2 = bin(2n, n)
* 
* 証明：【二項係数の畳込み】の下式で a=b=n, c=d=0 とすれば直ちに成り立つ．
*/


//【多項係数の累乗和（mod 998244353）】O(n (log n + log d))
/*
* 各 n∈[0..N] について，
*	Σ_(r[0..m) : Σr=n) mul(n, r[0..m))^d （mul は多項係数）
* の値を格納したリストを返す．
*
* 制約：fm は (2(N+1))! まで計算可能
*
* 利用：【形式的冪級数】,【累乗】
*/
vm multinomial_power_sum(int N, ll m, int d, const Factorial_mint& fm) {
	// verify : https://yukicoder.me/problems/no/2347

	//【方法】
	// 多項係数を階乗を用いて書き直すと，求める値は
	//		(n!)^d Σ_(r[0..m) : Σr=n) Πj∈[0..m) 1/(r[j]!)^d
	// と表される．これは畳込みの形なので，
	//		f(z) = Σi∈[0..n] 1/(i!)^d
	// とおき，
	//		(n!)^d [z^n] f(z)^m
	// を一括計算すればよい．

	MFPS f(0, N + 1);
	repi(n, 0, N) f[n] = fm.fact_inv(n).pow(d);

	f = pow_fps(f, m, N + 1, fm);
	repi(n, 0, N) f[n] *= fm.fact(n).pow(d);

	return f.c;
}


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
*	[z^n]f(z) = 1/n [z^(n-1)](z / g(z))^n
*
*（使い所）
* f(z) = (z の式) とは書けていないが z = (f(z) の式) という表示が得られている場合．
* 特に木の数え上げにおいて有用である．
*
* verify : https://atcoder.jp/contests/abc222/tasks/abc222_h
*/


//【ラグランジュの反転公式（累乗）】
/*
*【ラグランジュの反転公式】と同じ条件で以下の等式が成り立つ：
*	[z^n]f(z)^k = k/n [z^(n-k)](z / g(z))^n
* 
* 参考 : https://mathlog.info/articles/881
* verify : https://yukicoder.me/problems/10539
*/


//【ラグランジュの反転公式（一般化）】
/*
*【ラグランジュの反転公式】と同じ条件で以下の等式が成り立つ：
*	[z^n]h(f(z)) = 1/n [z^(n-1)] h'(z) (z / g(z))^n
*
* 参考 : https://atcoder.jp/contests/abc345/editorial/9549
*/


//【カタラン数の母関数の累乗】
/*
* カタラン数の母関数を
*	C(z) = Σi Catalan(i) z^i = (1-√(1-4z))/(2z)
* とするとき，
*	[z^n] C(z)^k = k/(2n+k) bin(2n+k, n)
* が成り立つ．
* 
*（証明）
* C(z) は
*	C(z) = (1-√(1-4z))/(2z)
*	⇒ 2z C(z) - 1 = -√(1-4z)
*	⇒ 4z^2 C(z)^2 - 4z C(z) + 1 = 1 - 4z
*	⇒ -z^2 C(z)^2 + z C(z) = z
* なる関数等式を満たす．そこで
*	f(z) = z C(z)
* とおくと，f(z) は
*	-f(z)^2 + f(z) = z
* という関数等式を満たし，したがって逆関数
*	g(z) = -z^2 + z
* をもつ．さらに f(z), g(z) はラグランジュの反転公式の前提条件
*	[z^0]f(z) = 0,  [z^0]g(z) = 0,
*	[z^1]f(z) != 0, [z^1]g(z) != 0
* を満たす．
* 
* 求めたいものは f(z) を用いて
*	[z^n] C(z)^k = [z^(n+k)] f(z)^k
* と表されるので，【ラグランジュの反転公式（累乗）】より
*	[z^(n+k)] f(z)^k
*	= k/(n+k) [z^(n+k-k)](z / g(z))^(n+k)
*	= k/(n+k) [z^n](1-z)^(-(n+k))
*	= k/(n+k) bin(n+(n+k)-1, (n+k)-1)  (負の二項定理より)
*	= k/(n+k) bin(2n+k-1, n+k-1)
*	= k/(2n+k) bin(2n+k, n+k)
*	= k/(2n+k) bin(2n+k, n)
* となる．
*
* verify : https://atcoder.jp/contests/xmascon22/tasks/xmascon22_d
*/


//【2×2 変数行列の累乗】
/*
* M = [x, y; z, w] の累乗について，以下の等式が成り立つ：
*	M^n = Σ_{a+b+c+d=n} C(a,b,c,d) x^a y^b z^c w^d
*		C(a,b,c,d)[0][0] = Boole[b=c] bin(a+b,a) bin(d+c-1,d)
*		C(a,b,c,d)[0][1] = Boole[b=c+1] bin(a+b-1,a) bin(d+c,d)
*		C(a,b,c,d)[1][0] = Boole[b=c-1] bin(a+b,a) bin(d+c-1,d)
*		C(a,b,c,d)[1][1] = Boole[b=c] bin(a+b-1,a) bin(d+c,d)
* 
* 証明：頂点 {0,1}, 辺 0→0, 0→1, 1→0, 1→1 をもつグラフ G を考える．
*	C(a,b,c,d)[i][j] は各辺を a,b,c,d 回通るような i→j ウォークの個数であり，
*	各移動辺の間に 0 本以上の停滞辺を挿入する方法を重複組合せで数え上げることで求まる．
* 
* verify : https://atcoder.jp/contests/agc051/tasks/agc051_d
*/

