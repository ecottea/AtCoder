#pragma once
#include "header.h"
#include "探索.h"
#include "数論.h"
// ■■■■■ 計算 ■■■■■


//【分割統治積】O(n log n)
/*
* Πa[0..n) を返す（Πa[l1..r1) と Πa[l2..r2) の積の計算量が O((r1-l1)+(r2-l2)) 程度とする）
*/
template <class T>
T divide_and_conquer_product(vector<T> a) {
	int n = sz(a);

	// 2 冪個ずつ掛けていく（分割統治法）
	for (int k = 1; k < n; k *= 2) {
		for (int i = 0; i + k < n; i += 2 * k) {
			a[i] = a[i] * a[i + k];
		}
	}

	return a[0];
}


//【分割統治積（モノイド）】O(n log n)
/*
* Πa[0..n) を返す（Πa[l1..r1) と Πa[l2..r2) の積の計算量が O((r1-l1)+(r2-l2)) 程度とする）
*/
template <class S, S(*op)(S, S), S(*e)()>
S divide_and_conquer_product(vector<S> a) {
	int n = sz(a);

	// 2 冪個ずつ掛けていく（分割統治法）
	for (int k = 1; k < n; k *= 2) {
		for (int i = 0; i + k < n; i += 2 * k) {
			a[i] = op(a[i], a[i + k]);
		}
	}

	return a[0];
}


//【累積積の加重和】O(n (log n)^2)
/*
* 与えられた cs[0..n], as[0..n) に対し，Σi=[0..n] cs[i] Πas[0..i) を返す．
*/
template <class T>
T weighted_cumulative_product_sum(const vector<T>& cs, const vector<T>& as) {
	// verify : https://atcoder.jp/contests/abc348/tasks/abc348_g

	//【方法】
	// a だけ 1-indexed で考える．例えば k=7 のとき，答えは
	//	c0 + c1 a1 + c2 a1 a2 + c3 a1 a2 a3 + c4 a1 a2 a3 a4 + c5 a1 a2 a3 a4 a5 + c6 a1 a2 a3 a4 a5 a6 + c7 a1 a2 a3 a4 a5 a6 a7
	//	= (c0 + c1 a1) + a1 a2 (c2 + c3 a3) + a1 a2 a3 a4 (c4 + c5 a5 + a5 a6 (c6 + c7 a7))
	// である．
	//
	// フェニック木の初期化と同様にして
	//		b1 = a1, b2 = a1 a2, b3 = a3, b4 = a1 a2 a3 a4, ...
	// と定めれば，答えは
	//		(c0 + c1 b1) + b2 (c2 + c3 b3) + b4 (c4 + c5 b5 + b6 (c6 + c7 b7))
	// と表される．
	//
	// まず
	//		b[2i+1] ← c[2i] + c[2i+1] b[2i+1] 
	// とすれば答えは
	//		b1 + b2 b3 + b4 (b5 + b6 b7)
	// と表される．さらに
	//		b[4i+1] += b[4i+2] b[4i+3]
	// とすれば答えは
	//		b1 + b4 b5
	// と表される．最後に
	//		b[8i+1] += b[8i+4] b[8i+5]
	// とすれば答えは
	//		b1
	// と表される．

	//【備考】
	// 和を 1 回以上とられた後の式は積の右オペランドにしか入らない．

	int n = sz(as);

	// 1-indexed に直して格納する．
	vector<T> bs(n + 1);
	rep(i, n) bs[i + 1] = as[i];

	// フェニック木の初期化段階
	for (int pow2 = 1; 2 * pow2 <= n; pow2 *= 2) {
		for (int i = 2 * pow2; i <= n; i += 2 * pow2) {
			bs[i] *= bs[i - pow2];
		}
	}

	// 奇数番目の要素の変更
	for (int i = 1; i <= n; i += 2) bs[i] = cs[i - 1] + cs[i] * bs[i];

	// 積の加算の繰り返し
	for (int pow2 = 2; pow2 <= n; pow2 *= 2) {
		for (int i = 1; i + pow2 - 1 <= n; i += 2 * pow2) {
			bs[i] += bs[i + pow2 - 1] * (i + pow2 <= n ? bs[i + pow2] : cs.back());
		}
	}

	return bs[1];
}


//【符号関数】O(1)
/*
* sgn(a) = (a > 0) - (a < 0)
* sgn(x - y) = (x > y) - (x < y)
*/


//【積の分解（畳込み用）】
/*
*	ij = (i+j)(i+j-1)/2 - i(i-1)/2 - j(j-1)/2
*	   = (i+j)(i+j+1)/2 - i(i+1)/2 - j(j+1)/2
* verify : https://judge.yosupo.jp/problem/multipoint_evaluation_on_geometric_sequence
*
* i+j=k のとき，
*	ijk = k(k-1)(k+1)/3 - i(i-1)(i+1)/3 - j(j-1)(j+1)/3
* verify : https://yukicoder.me/problems/no/2506
*/


//【積との比較】O(1)
/*
* 関係式 a b op c が成り立つかを返す．（a b がオーバーフローしても良い）
* 比較演算子 op は，">", ">=", "=", "<=", "<" のいずれかとする．
*/
bool comp(ll a, ll b, string op_, ll c) {
	int op = 0;
	if (op_ == ">") op = -2;
	else if (op_ == ">=") op = -1;
	else if (op_ == "<=") op = 1;
	else if (op_ == "<") op = 2;

	// 等号付き不等式に統一する．
	if (op == -2) { c++; op = -1; }
	if (op == 2) { c--; op = 1; }

	// a = 0 または b = 0 の場合，左辺は 0 になるので容易に判断できる．
	if (a == 0 || b == 0) {
		if (op == -1) return 0 >= c;
		if (op == 0) return 0 == c;
		if (op == 1) return 0 <= c;
	}

	// a > 0 かつ b > 0となるように同値変形する．
	if (a < 0) { a *= -1; c *= -1; op *= -1; }
	if (b < 0) { b *= -1; c *= -1; op *= -1; }

	// (左辺) > 0 なので，0 ≧ c の場合は符号だけで判断できる．
	// op が ">=" なら不等式は成り立ち，さもなくば成り立たない．
	if (c <= 0) return op == -1;

	// 以降は a, b, c > 0 として考えて良い．

	// op が "=" の場合
	if (op == 0) {
		//	    a * b = c
		//	 ⇔ b = c / a
		//	 ⇔ a | c かつ b = floor(c / a)
		return c % a == 0 && c / a == b;
	}
	// op が "<=" の場合
	else if (op == 1) {
		//	    a * b ≦ c
		//	 ⇔ b ≦ c / a
		//	 ⇔ b ≦ floor(c / a)
		return b <= c / a;
	}
	// op が ">=" の場合
	else {
		//	    a * b ≧ c
		//	 ⇔ b ≧ c / a
		//	 ⇔ b ≧ ceil(c / a)
		return b >= (c + a - 1) / a;
	}
}


