#pragma once
#include "header.h"
// ■■■■■ 不完全モノイド作用付きモノイド ■■■■■


//【不完全モノイド左作用付きモノイドの定義】
/*
* 不完全モノイド左作用付きモノイド (S, op, e, F, act, comp, id, fail) を表す．
*
* すなわち，(S, op, e), (F, comp, id) がそれぞれモノイドであり，
* F の S への左作用 act : F × S → S が
*	ほぼ準同型	: ∀x, y ∈ S，∀f ∈ F,	f(x y) = f(x) f(y) or fail
*	合成			: ∀x ∈ S, ∀f, g ∈ F,	(f g)(x) = f(g(x))
*   恒等射		: ∀x ∈ S,				id(x) = x
* を満たし，f(x) の計算に失敗した場合 act() は fail を返すものとする．
*/


//【整除算,変更 不完全作用付き 総和,max モノイド】
/*
* S ∋ x = {s, m, c, e} :
*	s : 元の和
*	m : 元の max
*	c : 元の個数
*	e : 元が全て等しいか
* F ∋ f = {a, b} :
*	b = -1 のとき，f(x) = floor(x / a) を表す．（整除算作用）
*	a = -1 のとき，f(x) = b を表す．（変更作用）
*/
// verify : https://atcoder.jp/contests/abc256/tasks/abc256_h
const ll LA01 = (ll)1e9 + 7; // 各元の最大値より大きい数
using SA01 = tuple<ll, ll, ll, bool>; // {sum, max, cnt, equal} 
using FA01 = pll; // {div, update}
SA01 failA01() { return SA01{ -1, -1, -1, false }; }
SA01 opA01(SA01 x, SA01 y) {
	ll xs, xm, xc, ys, ym, yc; bool xe, ye;
	tie(xs, xm, xc, xe) = x; tie(ys, ym, yc, ye) = y;

	// (x が空 かつ y が揃っている) または (y が空 かつ x が揃っている)
	// または (x も y も揃っており，かつ値が等しい) ときに限り全体が揃っている．
	bool e = ((xc == 0 && ye) || (yc == 0 && xe) || (xe && ye && xm == ym));

	return SA01{ xs + ys, max(xm, ym), xc + yc, e };
}
SA01 eA01() { return SA01{ 0, -INFL, 0, true }; }
SA01 actA01(FA01 f, SA01 x) {
	ll xs, xm, xc, a, b; bool xe;
	tie(xs, xm, xc, xe) = x; tie(a, b) = f;

	// x が空の場合や，a が 1 の場合は，何も変わらない．
	if (xc == 0 || a == 1) return x;

	// 変更作用の場合，全てが b に書き換えられて揃う．
	if (a == -1) return SA01{ b * xc, b, xc, true };

	// a が最大値より大きい場合，全てが 0 に書き換えられて揃う．
	if (a > xm) return SA01{ 0, 0, xc, true };

	// x が揃っていた場合，全てが x / a に書き換えられて揃う．
	if (xe) return SA01{ (xm / a) * xc, xm / a, xc, true };

	// 以上に該当しない場合は f(x) の計算に失敗する．
	return failA01();
}
FA01 compA01(FA01 f, FA01 g) {
	ll fa, fb, ga, gb;
	tie(fa, fb) = f; tie(ga, gb) = g;

	// f が変更作用の場合，合成作用は f と変わらない．
	if (fa == -1) return f;

	// g が変更作用の場合，合成作用は gb / fa への変更作用になる．
	if (ga == -1) return FA01{ -1, gb / fa };

	// f, g が共に整除算作用の場合，合成作用は fa * ga での整除算作用になる．
	return FA01{ min(fa * ga, LA01), -1 };
}
FA01 idA01() { return { 1, 0 }; }
#define DivUp_SumMax_iamonoid SA01, opA01, eA01, FA01, actA01, compA01, idA01, failA01


//【GCD,変更 不完全作用付き 総和,max,GCD,LCM モノイド】
/*
* S ∋ x = {s, m, g, l, c} :
*	s : 元の和
*	m : 元の max
*	g : 元の GCD
*	l : 元の LCM
*	c : 元の個数
* F ∋ f = {a, b} :
*	b = -1 のとき，f(x) = gcd(a, x) を表す．（GCD 作用）
*	a = -1 のとき，f(x) = b を表す．（変更作用）
*/
// verify : https://yukicoder.me/problems/no/880
const ll LA02 = (ll)1e9 + 7; // 各元の最大値より大きい素数
using SA02 = tuple<ll, ll, ll, ll, ll>; // {sum, max, gcd, lcm, cnt} 
using FA02 = pll; // {gcd, update}
SA02 failA02() { return SA02{ -1, -1, -1, -1, -1 }; }
SA02 opA02(SA02 x, SA02 y) {
	ll xs, xm, xg, xl, xc, ys, ym, yg, yl, yc;
	tie(xs, xm, xg, xl, xc) = x; tie(ys, ym, yg, yl, yc) = y;

	// x と y の LCM を求める（ただしオーバーフローした場合は十分大きい素数で代用する．）
	ll l = min(xl / gcd(xl, yl) * yl, LA02);

	return SA02{ xs + ys, max(xm, ym), gcd(xg, yg), l, xc + yc };
}
SA02 eA02() { return SA02{ 0, -INFL, 0, 1, 0 }; }
SA02 actA02(FA02 f, SA02 x) {
	ll xs, xm, xg, xl, xc, a, b;
	tie(xs, xm, xg, xl, xc) = x; tie(a, b) = f;

	// 変更作用の場合，全てが b に書き換えられて揃う．
	if (a == -1) return SA02{ b * xc, b, b, b, xc };

	// x が空の場合や，a が LCM(x) の倍数の場合は，何も変わらない．
	if (a % xl == 0 || xc == 0) return x;

	// a が GCD(x) の約数の場合は，全てが a に書き換えられて揃う．
	if (xg % a == 0) return SA02{ a * xc, a, a, a, xc };

	// x が揃っていた場合，全てが GCD(a, x) に書き換えられて揃う．
	if (xg == xl && xl != LA02) { ll g = gcd(a, xm); return SA02{ g * xc, g, g, g, xc }; }

	// 以上に該当しない場合は f(x) の計算に失敗する．
	return failA02();
}
FA02 compA02(FA02 f, FA02 g) {
	ll fa, fb, ga, gb;
	tie(fa, fb) = f; tie(ga, gb) = g;

	// f が変更作用の場合，合成作用は f と変わらない．
	if (fa == -1) return f;

	// g が変更作用の場合，合成作用は gcd(fa, gb) への変更作用になる．
	if (ga == -1) return FA02{ -1, gcd(fa, gb) };

	// f, g が共に GCD 作用の場合，合成作用は gcd(fa, ga) との GCD 作用になる．
	return FA02{ gcd(fa, ga), -1 };
}
FA02 idA02() { return FA02{ 0, -1 }; }
#define GCDUp_SumMaxGCDLCM_iamonoid SA02, opA02, eA02, FA02, actA02, compA02, idA02, failA02


//【AND,OR 不完全作用付き max,AND,OR モノイド】
/*
* S ∋ x = {m, l, u} :
*	m : 元の max
*	l : 元の AND
*	u : 元の OR
* F ∋ f = {a, b} :
*	f(x) = (a AND x) OR b を表す．
*/
// 参考 : https://rsm9.hatenablog.com/entry/2021/02/01/220408
// verify : https://csacademy.com/contest/round-70/task/and-or-max
using SA03 = tuple<int, int, int>; // {max, AND, OR}
using FA03 = pii; // {AND, OR}
SA03 failA03() { return SA03{ -1, -1, -1 }; }
SA03 opA03(SA03 x, SA03 y) {
	int xm, xl, xu, ym, yl, yu;
	tie(xm, xl, xu) = x; tie(ym, yl, yu) = y;

	return SA03{ max(xm, ym), xl & yl, xu | yu };
}
SA03 eA03() { return SA03{ 0, ~0, 0 }; }
SA03 actA03(FA03 f, SA03 x) {
	int a, b, m, l, u;
	tie(a, b) = f; tie(m, l, u) = x;

	// x で 0,1 が混在しているビットに対し 0 との AND や 1 との OR を行う場合は失敗する．
	if ((u - l) & (~a | b)) return failA03();

	return SA03{ (a & m) | b, (a & l) | b, (a & u) | b };
}
FA03 compA03(FA03 f, FA03 g) {
	int a, b, c, d;
	tie(a, b) = f; tie(c, d) = g;

	return FA03{ a & c, (a & d) | b };
}
FA03 idA03() { return FA03{ ~0, 0 }; }
#define ANDOR_MaxANDOR_iamonoid SA03, opA03, eA03, FA03, actA03, compA03, idA03, failA03


//【min,max,add 不完全作用付き min,max,sum モノイド】
/*
* S ∋ x = {l, u, l2, u2, s, c, cl, cu} :
*	l : 元の min
*	u : 元の max
*	l2 : 元の 2nd min (>min)
*	u2 : 元の 2nd max (<max)
*	s : 元の和
*	c : 元の個数
*	cl : min をとる元の個数
*	cu : max をとる元の個数
* F ∋ f = {a, b, c} :
*	f(x) = max(min(a, x), b) + c を表す．
*/
// 参考 : https://rsm9.hatenablog.com/entry/2021/02/01/220408
// verify : https://judge.yosupo.jp/problem/range_chmin_chmax_add_range_sum
using SA04 = tuple<ll, ll, ll, ll, ll, int, int, int>; // {min, max, 2ndmin, 2ndmax, sum, cnt, cntmin, cntmax}
using FA04 = tuple<ll, ll, ll>; // {min, max, add}
SA04 failA04() { return SA04{ -1, -1, -1, -1, -1, -1, -1, -1 }; }
SA04 opA04(SA04 x, SA04 y) {
	ll xl, xu, xl2, xu2, xs, yl, yu, yl2, yu2, ys; int xc, xcl, xcu, yc, ycl, ycu;
	tie(xl, xu, xl2, xu2, xs, xc, xcl, xcu) = x; tie(yl, yu, yl2, yu2, ys, yc, ycl, ycu) = y;

	ll l, u, l2, u2, s; int c, cl, cu;
	if (xl < yl) {
		l = xl;
		l2 = min(xl2, yl);
		cl = xcl;
	}
	else if (xl > yl) {
		l = yl;
		l2 = min(xl, yl2);
		cl = ycl;
	}
	else {
		l = xl;
		l2 = min(xl2, yl2);
		cl = xcl + ycl;
	}
	if (xu > yu) {
		u = xu;
		u2 = max(xu2, yu);
		cu = xcu;
	}
	else if (xu < yu) {
		u = yu;
		u2 = max(xu, yu2);
		cu = ycu;
	}
	else {
		u = xu;
		u2 = max(xu2, yu2);
		cu = xcu + ycu;
	}
	s = xs + ys;
	c = xc + yc;

	return SA04{ l, u, l2, u2, s, c, cl, cu };
}
SA04 eA04() { return SA04{ INFL, -INFL, INFL, -INFL, 0, 0, 0, 0 }; }
SA04 actA04(FA04 f, SA04 x) {
	ll xl, xu, xl2, xu2, xs, fa, fb, fc; int xc, xcl, xcu;
	tie(xl, xu, xl2, xu2, xs, xc, xcl, xcu) = x; tie(fa, fb, fc) = f;

	// x が空の場合，何も変わらない．
	if (xc == 0) return x;

	// 以下の場合については，x の全ての要素が max(min(a, x), b) + c に揃う．
	//	x が一種類の要素からなる場合 ⇔ l = u
	//	f が更新作用に退化している場合 ⇔ a <= b
	//  x ⊂ (-∞..b] の場合 ⇔ u <= b
	//	x ⊂ [a..∞) の場合 ⇔ a <= l
	if (xl == xu || fa <= fb || xu <= fb || fa <= xl) {
		ll v = max(min(fa, xl), fb) + fc;
		return SA04{ v, v, INFL, -INFL, v * xc, xc, xc, xc };
	}

	// x が二種類の要素からなる場合，それぞれに対して f(x) を計算できる．
	if (xl2 == xu) {
		Assert(xl == xu2);
		ll l = max(min(fa, xl), fb) + fc;
		ll u = max(min(fa, xu), fb) + fc;
		Assert(l < u);
		ll s = l * xcl + u * xcu;
		return SA04{ l, u, u, l, s, xc, xcl, xcu };
	}

	// x が三種類の要素からなる場合，それぞれに対して f(x) を計算できる．
	if (xl2 == xu2) {
		ll l = max(min(fa, xl), fb) + fc;
		ll m = max(min(fa, xl2), fb) + fc;
		ll u = max(min(fa, xu), fb) + fc;
		Assert(l < u);
		ll l2 = (l < m ? m : u);
		ll u2 = (u > m ? m : l);
		ll s = l * xcl + m * (xc - xcl - xcu) + u * xcu;
		int cl = (l < m ? xcl : (xc - xcu));
		int cu = (u > m ? xcu : (xc - xcl));
		return SA04{ l, u, l2, u2, s, xc, cl, cu };
	}

	// b < l2 かつ u2 < a の場合，min, max の作用を受ける可能性があるのは l, u のみであり，
	// その後 [b..a] の範囲の要素全てに c が加算される．
	if (fb < xl2 && xu2 < fa) {
		Assert(xl2 < xu2);
		ll l, u, l2 = xl2 + fc, u2 = xu2 + fc, s = xs + fc * xc;
		if (fb <= xl) {
			l = xl + fc;
		}
		else {
			l = fb + fc;
			s += (fb - xl) * xcl;
		}
		if (fa >= xu) {
			u = xu + fc;
		}
		else {
			u = fa + fc;
			s -= (xu - fa) * xcu;
		}
		Assert(l < l2);
		Assert(l2 < u2);
		Assert(u2 < u);
		return SA04{ l, u, l2, u2, s, xc, xcl, xcu };
	}

	// 以上に該当しない場合は f(x) = max(min(a, x), b) + c の計算に失敗する．
	return failA04();
}
FA04 compA04(FA04 f, FA04 g) {
	ll fa, fb, fc, ga, gb, gc;
	tie(fa, fb, fc) = f; tie(ga, gb, gc) = g;

	ll A = min(fa - gc, ga);
	ll B = max(min(fa - gc, gb), fb - gc);
	ll C = fc + gc;
	return FA04{ A, B, C };
}
FA04 idA04() { return FA04{ INFL, -INFL, 0 }; }
#define MinMaxAdd_MinMaxSum_iamonoid SA04, opA04, eA04, FA04, actA04, compA04, idA04, failA04


