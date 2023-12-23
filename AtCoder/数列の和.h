#pragma once
#include "header.h"
#include "二項係数.h"
// ■■■■■ 数列の和など ■■■■■


//【等差数列の和】O(1)
/*
* Σi∈[i0..i1) (a i + b) を返す．
*/
template<class T>
T arithmetic_series(T a, T b, ll i0, ll i1) {
	// verify : https://atcoder.jp/contests/arc035/tasks/arc035_b

	if (i0 >= i1) return 0;

	// 2^(-1) が存在しない場合でも問題ないように偶数を先に 2 で割っておく．
	if ((i1 - i0) % 2 == 0) {
		return a * (i1 + i0 - 1) * ((i1 - i0) / 2) + b * (i1 - i0);
	}
	else {
		return a * ((i1 + i0 - 1) / 2) * (i1 - i0) + b * (i1 - i0);
	}
}


//【等差数列の積】
/*
* 公差が 0 でない等差数列の積は，
*		Πi∈[i0..i1) (a + b i)
*		= b^(i1-i0) Πi∈[i0..i1) (a/b + i)
*		= b^(i1-i0) (a/b + i1 - 1)! / (a/b + i0 - 1)!
* と変形して階乗の計算に帰着できる．
* 
* verify : https://atcoder.jp/contests/m-solutions2019/tasks/m_solutions2019_e
*/


//【等比数列の和】O(log n)
/*
* geometric_series(r, n) : O(log n)
*	Σi∈[0..n) r^i を返す．
*
* geometric_series(r, i0, i1) : O(log max(i0, i1))
*	Σi∈[i0..i1) r^i を返す．
* 
*（ダブリング）
*/
mint geometric_series(mint r, ll n) {
	// verify : https://atcoder.jp/contests/arc050/tasks/arc050_c

	//【方法】
	// おなじみの公式では r-1 による割り算を必要としてまずいのでダブリングする．

	// pow2 = r^(2^i), sumpow2 = 1 + r + ... + r^((2^i) - 1)
	mint res(0), pow2 = r, sumpow2 = 1;
	while (n > 0) {
		if (n & 1LL) res = res * pow2 + sumpow2;
		sumpow2 = sumpow2 * pow2 + sumpow2;
		pow2 = pow2 * pow2;
		n /= 2;
	}
	return res;
}
mint geometric_series(mint r, ll i0, ll i1) {
	return geometric_series(r, i1) - geometric_series(r, i0);
}


//【等比数列の和（半環）】O(log n)
/*
* 半環 (S, add, o, mul, e) の元 r について Σi∈[0..n) r^i を返す．
*/
template <class S, S(*add)(S, S), S(*o)(), S(*mul)(S, S), S(*e)()>
S geometric_series(S r, ll n) {
	// verify : https://csacademy.com/contest/iati-shumen-2017-day-1/task/superstition/statement/

	// pow2 = r^(2^i), sumpow2 = 1 + r + ... + r^((2^i) - 1)
	S res(o()), pow2 = r, sumpow2(e());

	while (n > 0) {
		if (n & 1LL) res = add(mul(res, pow2), sumpow2);
		sumpow2 = add(sumpow2, mul(sumpow2, pow2));
		pow2 = mul(pow2, pow2);
		n /= 2;
	}

	return res;
}


//【等比数列の和（M-可換モノイド）】O(log n)
/*
* M-可換モノイド (S, op, o, F, act, comp, id) の元 f∈F, x∈S について Σi∈[0..n) f^i x を返す．
*/
template <class S, S(*op)(S, S), S(*o)(), class F, S(*act)(F, S), F(*comp)(F, F), F(*id)()>
S geometric_series(F f, S x, ll n) {
	// verify : https://atcoder.jp/contests/abc310/tasks/abc310_g

	//【方法】
	// 小さい冪から順に計算していく．
	//	a[0] = f^1,	b[0] = f^0 x
	//	a[1] = f^2,	b[1] = (f^0 x) + f^1 (f^0 x)
	//				     = f^0 x + f^1 x
	//	a[2] = f^4,	b[2] = (f^0 x + f^1 x) + f^2 (f^0 x + f^1 x)
	//				     = f^0 x + f^1 x + f^2 x + f^3 x
	// 
	// これらを組み合わせると
	//	f^0 x + f^1 x + f^2 x + f^3 x + f^4 x + f^5 x + f^6 x
	//	= f^0 x + f^1 (f^0 x + f^1 x + f^2 x + f^3 x + f^4 x + f^5 x)
	//	= b[0] + a[0] (f^0 x + f^1 x + f^2 x + f^3 x + f^4 x + f^5 x)
	//	= b[0] + a[0] ((f^0 x + f^1 x) + f^2 (f^0 x + f^1 x + f^2 x + f^3 x))
	//	= b[0] + a[0] (b[1] + a[1] (f^0 x + f^1 x + f^2 x + f^3 x))
	//	= b[0] + a[0] (b[1] + a[1] b[2])
	// のように計算できる．
	// 
	// 実際は a の累積積を持ちながら並行して計算していける．

	S res(o()); F a(f), a_acc(id()); S b(x);

	while (n > 0) {
		if (n & 1) {
			res = op(res, act(a_acc, b));
			a_acc = comp(a_acc, a);
		}
		b = op(b, act(a, b));
		a = comp(a, a);
		n /= 2;
	}

	return res;
}


//【等差×等比型数列の和】O(log n)
/*
* arithmetic_geometric_series(r, n) : O(log n)
*	Σi∈[0..n) i r^i を返す．
*
* arithmetic_geometric_series(a, b, r, i0, i1) : O(log max(i0, i1))
*	Σi∈[i0..i1) (a i + b) r^i を返す．
*
*（ダブリング）
* 
* 利用：【等比数列の和】
*/
mint arithmetic_geometric_series(mint r, ll n) {
	//【方法】
	// おなじみの公式では r-1 による割り算を必要としてまずいのでダブリングする．

	// pow2 = 2^i, powr = r^(2^i)
	// sumpowr1 = 0 * 1 + 1 * r + ... + ((2^i) - 1) * r^((2^i) - 1)
	// sumpowr0 = 1 + r + ... + r^((2^i) - 1)
	mint res1(0), res0(0), pow2 = 1, powr = r, sumpowr1 = 0, sumpowr0 = 1;
	while (n > 0) {
		if (n & 1LL) {
			res1 += pow2 * res0;
			res1 *= powr;
			res1 += sumpowr1;
			res0 *= powr;
			res0 += sumpowr0;
		}
		sumpowr1 *= 1 + powr;
		sumpowr1 += pow2 * powr * sumpowr0;
		sumpowr0 *= 1 + powr;
		powr *= powr;
		pow2 *= 2;

		n /= 2;
	}
	return res1;
}
mint arithmetic_geometric_series(mint a, mint b, mint r, ll i0, ll i1) {
	mint res = a * geometric_series(r, i0, i1);
	res += b * (arithmetic_geometric_series(r, i1) - arithmetic_geometric_series(r, i0));
	return res;
}


//【累乗型数列の和】O(k log k)
/*
* Σi∈[0..n) i^k は n の k+1 次式になるので，ラグランジュ補間で値を求められる．
* 
* verify : https://codeforces.com/contest/622/problem/F
*/


//【累乗×等比型数列の無限和】O(d log d)
/*
* Σi∈[0..∞) i^d r^i を返す．
*
* 制約：r != 1
* 
* 利用：【階乗など（法が大きな素数）】
*/
mint powered_geometric_series(mint r, int d) {
	// 参考 : https://kyopro-friends.hatenablog.com/entry/2020/03/11/073122
	// verify : https://judge.yosupo.jp/problem/sum_of_exponential_times_polynomial_limit

	vm acc(d + 2), pow_r(d + 2);
	pow_r[0] = 1;
	rep(i, d + 1) {
		acc[i + 1] = acc[i] + pow_r[i] * mint(i).pow(d);
		pow_r[i + 1] = pow_r[i] * r;
	}

	Factorial_mint fm(d + 1);

	mint res = 0;
	repi(i, 0, d) {
		int sign = ((d - i) % 2 == 0 ? 1 : -1);
		res += sign * pow_r[d - i] * fm.bin(d + 1, i + 1) * acc[i + 1];
	}

	res /= mint(1 - r).pow(d + 1);

	return res;
}


//【差が {0, 1} の数列の組の等しい要素の数え上げ】
/*
* 数列 a[0..n), b[0..n) が
*		∀i∈[0..n) a[i] - b[i] ∈ {0, 1}
* をみたしているとき，a と b の一致箇所の個数は次の式で求められる：
*		Σi∈[0..n) Boole[a[i] = b[i]] = n - (Σa - Σb)
* 
* verify : https://atcoder.jp/contests/arc123/tasks/arc123_e
*/

