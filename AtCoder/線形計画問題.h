#pragma once
#include "header.h"
// ■■■■■ 線形計画問題 ■■■■■


//【線形計画問題の双対（等式制約）】
/*
* 主問題（変数は x）
*	maximize	c^T x			minimize	c^T x
*	subject to	A x = b			subject to	A x = b
*				x ≧ 0						x ≧ 0
* の双対問題（変数は y）はそれぞれ
*	minimize	b^T y			maximize	b^T y
*	subject to	A^T y ≧ c		subject to	A^T y ≦ c
* であり，目的関数の最適値は等しい（強双対性）
* 
* x に整数制約が付く場合，強双対性の成立のためには A に完全単模性が要求される．
*
* 双対をとると変数の数と制約条件の数が入れ替わるので，
* 変数は多いが制約条件は少ないようなときに双対をとると問題が簡単になることがある．
*/


//【線形計画問題の双対（不等式制約）】
/*
* 主問題（変数は x）
*	maximize	c^T x			minimize	c^T x
*	subject to	A x ≦ b			subject to	A x ≧ b
*				x ≧ 0						x ≧ 0
* の双対問題（変数は y）はそれぞれ
*	minimize	b^T y			maximize	b^T y
*	subject to	A^T y ≧ c		subject to	A^t y ≦ c
*				y ≧ 0						y ≧ 0
* であり，目的関数の最適値は等しい．
* 
* verify : https://atcoder.jp/contests/abc275/tasks/abc275_g
*/


//【線形計画問題の双対（混合制約）】
/*
* 主問題（変数は x）
*	maximize	c^T x								minimize	c^T x
*	subject to	A1 x ≦ b1							subject to	A1 x ≦ b1
*				A2 x ≧ b2										A2 x ≧ b2
*				A3 x = b3										A3 x = b3
*				x ≧ 0											x ≧ 0
* の双対問題（変数は y）はそれぞれ
*	minimize	b1^T y1 - b2^T y2 + b3^T y3			maximize	-b1^T y1 + b2^T y2 + b3^T y3
*	subject to	A1^T y1 - A2^T y2 + A3^T y3 ≧ c		subject to	-A1^T y1 + A2^T y2 + A3^T y3 ≦ c
*				y1 ≧ 0											y1 ≧ 0
*				y2 ≧ 0											y2 ≧ 0
* であり，目的関数の最適値は等しい．
*/


//【ラグランジュ双対（等式制約）】
/*
* 主問題
*	maximize_x	c^T x
*	subject to	A x ≦ b
*				D x = k
* のラグランジュ双対は
*	minimize_λ	max_x {c^T x - λ(D x - k)}
*	subject to	A x ≦ b
* であり，目的関数の最適値は等しい．
* 
* 解ける問題に邪魔な制約が付いているとき，制約を目的関数に移すことができる．
* 目的関数は λ について下に単峰なので，λ がスカラーなら三分探索を用いることができる．
* あるいは λ で微分したとし，D x - k の符号で二分探索を行うこともできる．
* 
* 非線形計画問題の場合は強双対性（ラグランジュ双対問題の解 = 主問題の解）が成り立つとは限らない．
*/


//【ラグランジュ双対（不等式制約）】
/*
* 主問題
*	maximize_x	c^T x
*	subject to	A x ≦ b
*				D x ≦ k
* のラグランジュ双対は
*	minimize_λ	max_x {c^T x - λ(D x - k)}
*	subject to	A x ≦ b
*				λ ≧ 0
* であり，目的関数の最適値は等しい．
* 
* verify : https://onlinejudge.u-aizu.ac.jp/problems/2736
*/


//【絶対値を含む目的関数の最小化】
/*
* 元の問題に
*	minimize	|x| 
* という因子があった場合，変数 y を導入して
*	minimize	y
*	subject to	|x| ≦ y
*				(y ≧ 0)
* に書き換える．絶対値を含む不等式 |x| ≦ y であれば，場合分けをすることにより
*	subject to	x ≦ y
*				-x ≦ y
* と書き換えられる．結局，全体は
*	minimize	y
*	subject to	x - y ≦ 0
*				-x - y ≦ 0
*				(y ≧ 0)
* と書き換えられる．
* 
* maximize |x| だと，x ≧ y OR -x ≧ y になってしまいうまくいかなさそう．
*/


//【整数計画問題（2 変数，1 制約，最大化）】O(√e)
/*
* 変数 x, y についての整数計画問題
*	maximize	a x + b y
*	subject to	c x + d y ≦ e
*				x, y ≧ 0
* の解の目的関数値を返し，実行可能解を sx, sy に格納する．
*
* 制約：c > 0, d > 0, e ≧ 0
*
*（解法平方分割）
*/
ll integer_programming_maximize(ll a, ll b, ll c, ll d, ll e, ll* sx_ = nullptr, ll* sy_ = nullptr) {
	// verify : https://atcoder.jp/contests/arc139/tasks/arc139_b

	Assert(c > 0 && d > 0 && e >= 0);

	ll sx = -1, sy = -1, res = -INFL;

	// 直線 a x + b y = const を左下方向に移動させる場合
	if (a <= 0 && b <= 0) {
		// 明らかに原点で最大となる．
		sx = 0;
		sy = 0;
		res = 0;
	}
	// 直線 a x + b y = const を左上方向に移動させる場合
	else if (a <= 0 && b > 0) {
		// 明らかに y 軸上で最大となる．
		sx = 0;
		sy = e / d;
		res = b * sy;
	}
	// 直線 a x + b y = const を右下方向に移動させる場合
	else if (a > 0 && b <= 0) {
		// 明らかに x 軸上で最大となる．
		sx = e / c;
		sy = 0;
		res = a * sx;
	}
	// 以降は直線 a x + b y = const を右上方向に移動させる場合について考える．
	else {
		// a d - b c ≧ 0 としておく．
		bool swap_flag = false;
		if (a * d - b * c < 0) {
			swap(a, b); swap(c, d);
			swap_flag = true;
		}

		// O(e/c) の全探索を採用する場合
		if (e / c < c) {
			// x の動ける範囲は 0 ≦ x ≦ e/c なので，x を決め打ち全探索する．
			repi(x, 0, e / c) {
				ll y = (e - c * x) / d;

				if (chmax(res, a * x + b * y)) { sx = x; sy = y; };
			}
		}
		// O(c) の全探索を採用する場合
		else {
			// 最適解 (x0, y0) においては 0 ≦ y0 < c なので，y を決め打ち全探索する．
			//（もし y0 ≧ c だと (x0 + d, y0 - c) の方が目的関数値を大きくする．）
			repi(y, 0, min(c - 1, e / d)) {
				ll x = (e - d * y) / c;

				if (chmax(res, a * x + b * y)) { sx = x; sy = y; };
			}
		}

		if (swap_flag) swap(sx, sy);
	}

	if (sx_ != nullptr) *sx_ = sx;
	if (sy_ != nullptr) *sy_ = sy;
	return res;
}


//【整数計画問題（2 変数，1 制約，最小化）】O(√e)
/*
* 変数 x, y についての整数計画問題
*	minimize	a x + b y
*	subject to	c x + d y ≧ e
*				x, y ≧ 0
* の解の目的関数値を返し，実行可能解を sx, sy に格納する．
*
* 制約：a > 0, b > 0, c > 0, d > 0, e ≧ 0
*
*（解法平方分割）
*/
ll integer_programming_minimize(ll a, ll b, ll c, ll d, ll e, ll* sx_ = nullptr, ll* sy_ = nullptr) {
	// verify : https://atcoder.jp/contests/arc174/tasks/arc174_b
	
	Assert(a > 0 && b > 0 && c > 0 && d > 0 && e >= 0);

	ll sx = -1, sy = -1, res = INFL;

	// a d - b c ≧ 0 としておく．
	bool swap_flag = false;
	if (a * d - b * c < 0) {
		swap(a, b); swap(c, d);
		swap_flag = true;
	}

	// O(e/d) の全探索を採用する場合
	if (e / d < d) {
		// (x, y) = (0, ceil(e/d)) より目的関数値を大きくしてしまうものは調べる必要はないので，
		// 0 ≦ y ≦ ceil(e/d) の範囲で y を決め打ち全探索する．
		repi(y, 0, (e + d - 1) / d) {
			// c x + d y ≧ e より x ≧ (e - d y) / c
			ll x = max((e - d * y + c - 1) / c, 0LL);

			if (chmin(res, a * x + b * y)) { sx = x; sy = y; };
		}
	}
	// O(d) の全探索を採用する場合
	else {
		// 最適解 (x0, y0) においては 0 ≦ x0 < d なので，x を決め打ち全探索する．
		//（もし x0 ≧ d だと (x0 - d, y0 + c) の方が目的関数値を小さくする．）
		repi(x, 0, min(d - 1, e / c)) {
			// c x + d y ≧ e より y ≧ (e - c x) / d
			ll y = max((e - c * x + d - 1) / d, 0LL);

			if (chmin(res, a * x + b * y)) { sx = x; sy = y; };
		}
	}

	if (swap_flag) swap(sx, sy);

	if (sx_ != nullptr) *sx_ = sx;
	if (sy_ != nullptr) *sy_ = sy;
	return res;
}


