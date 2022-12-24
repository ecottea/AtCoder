#pragma once
#include "header.h"
#include "行列.h"
// ■■■■■ モノイド ■■■■■


//【モノイド】
/*
* モノイド (S, op, e) を表す．
* 
* すなわち，集合 S とその上の二項演算 * : S × S → S で
*	結合律 : ∀a, b, c ∈ S， (a b) c = a (b c)
*	単位元 : ∀a ∈ S,        a e = e a = a
* を満たすものとする．
*/


//【加算 可換モノイド】
/* verify : https://atcoder.jp/contests/arc035/tasks/arc035_d */
using S001 = ll;
S001 op001(S001 a, S001 b) { return a + b; }
S001 e001() { return 0; }
#define Add_monoid S001, op001, e001


//【組の加算 可換モノイド】
/* verify : https://atcoder.jp/contests/jsc2021/tasks/jsc2021_f */
using S023 = pli;
S023 op023(S023 a, S023 b) { return { a.first + b.first, a.second + b.second }; }
S023 e023() { return { 0, 0 }; }
#define Pairadd_monoid S023, op023, e023


//【max 可換モノイド】
/* verify: https://atcoder.jp/contests/abl/tasks/abl_d */
using S003 = ll;
S003 op003(S003 a, S003 b) { return max(a, b); }
S003 e003() { return -INFL; }
#define Max_monoid S003, op003, e003


//【min 可換モノイド】
/* verify: https://atcoder.jp/contests/abc170/tasks/abc170_e */
using S004 = ll;
S004 op004(S004 a, S004 b) { return min(a, b); }
S004 e004() { return INFL; }
#define Min_monoid S004, op004, e004


//【乗算 可換モノイド】
/* verify : https://codeforces.com/contest/1748/problem/D */
using S024 = mint;
S024 op024(S024 a, S024 b) { return a * b; }
S024 e024() { return 1; }
#define Mul_monoid S024, op024, e024


//【行列乗算 モノイド】（参照渡ししていないので遅い）
/* verify : https://codeforces.com/contest/1681/problem/E */
using S002 = Matrix<mint>;
S002 op002(S002 a, S002 b) { return a * b; }
S002 e002() { return Matrix<mint>(3); }
#define MatrixMul_monoid S002, op002, e002


//【逆行列乗算 モノイド】（参照渡ししていないので遅い）
/* verify: https://atcoder.jp/contests/arc025/tasks/arc025_4 */
using S020 = Matrix<mint>;
S020 op020(S020 a, S020 b) { return b * a; }
S020 e020() { return Matrix<mint>(3); }
#define InvMul_monoid S020, op020, e020


//【xor 可換モノイド】
using S010 = int;
S010 op010(S010 a, S010 b) { return a ^ b; }
S010 e010() { return 0; }
#define XOR_monoid S010, op010, e010


//【or 可換モノイド】
/* verify: https://atcoder.jp/contests/abc157/tasks/abc157_e */
using S011 = int;
S011 op011(S011 a, S011 b) { return a | b; }
S011 e011() { return 0; }
#define OR_monoid S011, op011, e011


//【and 可換モノイド】
using S012 = int;
S012 op012(S012 a, S012 b) { return a & b; }
S012 e012() { return ~0; }
#define AND_monoid S012, op012, e012


//【gcd 可換モノイド】
/* verify : https://atcoder.jp/contests/abc125/tasks/abc125_c */
using S015 = ll;
S015 op015(S015 a, S015 b) { return gcd(a, b); }
S015 e015() { return 0; }
#define GCD_monoid S015, op015, e015


//【lcm 可換モノイド】
using S016 = ll;
S016 op016(S016 a, S016 b) { return a / gcd(a, b) * b; }
S016 e016() { return 1; }
#define LCM_monoid S016, op016, e016


//【左変更 モノイド】
using S005 = int;
S005 e005() { return INF; } // 使わない値なら何でも OK
S005 op005(S005 a, S005 b) { return a == e005() ? b : a; }
#define LUpdate_monoid S005, op005, e005


//【右変更 モノイド】
using S006 = int;
S006 e006() { return INF; } // 使わない値なら何でも OK
S006 op006(S006 a, S006 b) { return b == e006() ? a : b; }
#define RUpdate_monoid S006, op006, e006


//【文字列連結 モノイド】
using S007 = string;
S007 op007(S007 a, S007 b) { return a + b; }
S007 e007() { return ""; }
#define Join_monoid S007, op007, e007


//【アフィン変換の合成 モノイド】
/*
* S ∋ f = {a, b} : 一次関数 f(x) = a x + b を表す．
* f op g : 合成した一次関数 f o g を返す．
* 
* 行列 (a, b; 0, 1) の全体が積に関して作っているモノイドともみなせる．
*/
using S008 = pair<mint, mint>;
S008 op008(S008 f, S008 g) {
	mint a, b, c, d;
	tie(a, b) = f; // f(x) = a x + b;
	tie(c, d) = g; // g(x) = c x + d;

	// (f o g)(x) = a (c x + d) + b = (a c)x + (a d + b)
	return { a * c, a * d + b };
}
S008 e008() { return { 1, 0 }; } // e(x) = x = 1 x + 0
#define AffineComposite_monoid S008, op008, e008


//【アフィン変換の逆合成 モノイド】
/*
* S ∋ f = {a, b} : 一次関数 f(x) = a x + b を表す．
* f op g : 逆向きに合成した一次関数 g o f を返す．
*/
// verify : https://judge.yosupo.jp/problem/point_set_range_composite
using S009 = pair<mint, mint>;
S009 op009(S009 f, S009 g) {
	mint a, b, c, d;
	tie(a, b) = g; // g(x) = a x + b;
	tie(c, d) = f; // f(x) = c x + d;

	// (g o f)(x) = a (c x + d) + b = (a c)x + (a d + b)
	return { a * c, a * d + b };
}
S009 e009() { return { 1, 0 }; } // e(x) = x = 1 x + 0
#define AffineInvcomposite_monoid S009, op009, e009


//【トロピカルアフィン変換の合成 モノイド】
/*
* S ∋ f = {a, b} : トロピカル一次関数 f(x) = max(a + x, b) を表す．
* f op g : 合成したトロピカル一次関数 f o g を返す．
*
* トロピカル半環上の行列 (a, b; -∞, 0) の全体が積に関して作っているモノイドともみなせる．
*/
using S013 = pair<ll, ll>;
S013 op013(S013 f, S013 g) {
	ll a, b, c, d;
	tie(a, b) = f; // f(x) = max(a + x, b);
	tie(c, d) = g; // g(x) = max(c + x, d);

	// (f o g)(x) = max(a + max(c + x, d), b) = max((a + c) + x, max(a + d, b))
	return { a + c, max(a + d, b) };
}
S013 e013() { return { 0, -INFL }; } // e(x) = x = max(0 + x, -∞)
#define TropicalAffineComposite_monoid S013, op013, e013


//【トロピカルアフィン変換の逆合成 モノイド】
/*
* S ∋ f = {a, b} : トロピカル一次関数 f(x) = max(a + x, b) を表す．
* f op g : 合成したトロピカル一次関数 g o f を返す．
*/
// verify : https://atcoder.jp/contests/yahoo-procon2017-qual/tasks/yahoo_procon2017_qual_d
using S014 = pair<ll, ll>;
S014 op014(S014 f, S014 g) {
	ll a, b, c, d;
	tie(a, b) = g; // g(x) = max(a + x, b);
	tie(c, d) = f; // f(x) = max(c + x, d);

	// (g o f)(x) = max(a + max(c + x, d), b) = max((a + c) + x, max(a + d, b))
	return { a + c, max(a + d, b) };
}
S014 e014() { return { 0, -INFL }; } // e(x) = x = max(0 + x, -∞)
#define TropicalAffineInvcomposite_monoid S014, op014, e014


//【ビット列上 転倒数 モノイド】
/*
* S ∋ x = {inv, c0, c1} : 列 x の転倒数，0 の個数，1 の個数の組
* x op y : 列 x, y を連結した列
*/
using S017 = tuple<ll, ll, ll>;
S017 op017(S017 x, S017 y) {
	ll x_inv, y_inv, x_c0, x_c1, y_c0, y_c1;
	tie(x_inv, x_c0, x_c1) = x;
	tie(y_inv, y_c0, y_c1) = y;

	// まず x, y それぞれをソートするのに x_inv + y_inv 回の隣接互換が必要．
	// その後 x の右側に寄った x_c1 個の 1 と y の左側に寄った y_c0 個の 0 を
	// 交換するのに x_c1 * y_c0 回の隣接互換が必要．
	ll inv = x_inv + y_inv + x_c1 * y_c0;
	ll c0 = x_c0 + y_c0, c1 = x_c1 + y_c1;

	return { inv, c0, c1 };
}
S017 e017() { return { 0LL, 0, 0 }; }
#define Inversion_monoid S017, op017, e017


//【置換の合成 モノイド】（参照渡ししていないので遅い）
/*
* S ∋ f[0..n) : 置換 i → f[i] を表す．
* f op g : 合成置換 f o g を返す．
*/
// verify : https://atcoder.jp/contests/abc013/tasks/abc013_4
using S018 = vi;
S018 op018(S018 a, S018 b) {
	if (sz(a) == 0) return b;
	if (sz(b) == 0) return a;

	int n = sz(a);
	S018 res(n);
	rep(i, n) res[i] = a[b[i]];

	return res;
}
S018 e018() { return S018(); }
#define PermutationComposite_monoid S018, op018, e018


//【置換の逆合成 モノイド】（参照渡ししていないので遅い）
/*
* S ∋ f[0..n) : 置換 i → f[i] を表す．
* f op g : 合成置換 g o f を返す．
*/
// verify : https://atcoder.jp/contests/abc013/tasks/abc013_4
using S019 = vi;
S019 op019(S019 a, S019 b) {
	if (sz(a) == 0) return b;
	if (sz(b) == 0) return a;

	int n = sz(a);
	S019 res(n);
	rep(i, n) res[i] = b[a[i]];

	return res;
}
S019 e019() { return S019(); }
#define PermutationInvcomposite_monoid S019, op019, e019


//【第二最大値 可換モノイド】
/* verify: https://atcoder.jp/contests/arc100/tasks/arc100_c */
using S021 = pair<ll, ll>; // (最大値, 第二最大値)
S021 op021(S021 a, S021 b) {
	vector<ll> vals(4);
	vals[0] = a.first;
	vals[1] = a.second;
	vals[2] = b.first;
	vals[3] = b.second;
	sort(all(vals));

	return { vals[3], vals[2] };
}
S021 e021() { return { -INFL, -INFL }; }
#define SecondMax_monoid S021, op021, e021


//【第二最小値 可換モノイド】
using S022 = pair<ll, ll>; // (最小値, 第二最小値)
S022 op022(S022 a, S022 b) {
	vector<ll> vals(4);
	vals[0] = a.first;
	vals[1] = a.second;
	vals[2] = b.first;
	vals[3] = b.second;
	sort(all(vals));

	return { vals[0], vals[1] };
}
S022 e022() { return { INFL, INFL }; }
#define SecondMin_monoid S022, op022, e022


//【混合トロピカルアフィン変換の合成 モノイド】
/*
* S ∋ f = {a, b, c} : 混合トロピカル一次関数 f(x) = max(min(a + x, b), c) を表す．
* f op g : 合成した混合トロピカル一次関数 f o g を返す．
*/
using S025 = tuple<ll, ll, ll>; // {add, min, max}
S025 op025(S025 f, S025 g) {
	ll fa, fb, fc, ga, gb, gc;
	tie(fa, fb, fc) = f; // f(x) = max(min(fa + x, fb), fc)
	tie(ga, gb, gc) = g; // g(x) = max(min(ga + x, gb), gc)

	// まず + が min, max の上に分配的であることを利用して
	//		(f o g)(x)
	//		= max(min(fa + max(min(ga + x, gb), gc), fb), fc)
	//		= max(min(max(min(fa + ga + x, fa + gb), fa + gc), fb), fc)
	// となる．
	//		x' = fa + ga + x
	//		gb' = fa + gb
	//		gc' = fa + gc
	// とおき，残る部分を max-min 半環において計算すると，
	//		(x' gb' + gc')fb + fc
	//		= x' (gb' fb) + (gc' fb + fc)
	// となる．よって
	//		A = fa + ga
	//		B = min(fa + gb, fb)
	//		C = max(min(fa + gc ,fb), fc)
	// とおけば，
	//		(f o g)(x) = max(min(A + x, B), C)
	// となる．

	ll A = fa + ga;
	ll B = min(fa + gb, fb);
	ll C = max(min(fa + gc, fb), fc);
	return S025{ A, B, C };
}
S025 e025() { return S025{ 0, INFL, -INFL }; } // e(x) = max(min(a + 0, ∞), -∞)
#define MixedTropicalAffineComposite_monoid S025, op025, e025


//【混合トロピカルアフィン変換の逆合成 モノイド】
/*
* S ∋ f = {a, b, c} : 混合トロピカル一次関数 f(x) = max(min(a + x, b), c) を表す．
* f op g : 合成した混合トロピカル一次関数 g o f を返す．
*/
// verify : https://atcoder.jp/contests/arc082/tasks/arc082_d
using S026 = tuple<ll, ll, ll>; // {add, min, max}
S026 op026(S026 f, S026 g) {
	ll fa, fb, fc, ga, gb, gc;
	tie(fa, fb, fc) = g;
	tie(ga, gb, gc) = f;

	ll A = fa + ga;
	ll B = min(fa + gb, fb);
	ll C = max(min(fa + gc, fb), fc);
	return S026{ A, B, C };
}
S026 e026() { return S026{ 0, INFL, -INFL }; }
#define MixedTropicalAffineInvcomposite_monoid S026, op026, e026


