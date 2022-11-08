#pragma once
#include "header.h"
#include "行列.h"
// ■■■■■ 半環 ■■■■■


//【半環】
/*
* 半環 (S, add, o, mul, e) を表す．
*
* すなわち，(S, add, o) が可換モノイド，(S, mul, e) がモノイドで，
*	分配律 : ∀a, b, c ∈ S， a(b + c) = a b + a c, (a + b)c = a c + b c
*	零倍   : ∀a ∈ S,        a o = o a = o
* を満たすものとする．
*/


//【加算 - 乗算 可換半環】
using S801 = mint;
S801 add801(S801 x, S801 y) { return x + y; }
S801 o801() { return 0; }
S801 mul801(S801 x, S801 y) { return x * y; }
S801 e801() { return 1; }
#define Add_mul_semiring S801, add801, o801, mul801, e801


//【行列加算 - 乗算 半環】
int msize813 = 3;
using S813 = Matrix<mint>;
S813 add813(S813 x, S813 y) { return x + y; }
S813 o813() { return Matrix<mint>(msize813, msize813); }
S813 mul813(S813 x, S813 y) { return x * y; }
S813 e813() { return Matrix<mint>(msize813); }
#define MatrixAdd_mul_semiring S813, add813, o813, mul813, e813


//【XOR - AND 可環半環】
using S802 = unsigned int;
S802 add802(S802 x, S802 y) { return x ^ y; }
S802 o802() { return 0; }
S802 mul802(S802 x, S802 y) { return x & y; }
S802 e802() { return ~0; }
#define XOR_AND_semiring S802, add802, o802, mul802, e802


//【min - plus 可環半環（トロピカル半環）】
/* verify : https://codeforces.com/contest/1681/problem/F */
using S803 = ll;
S803 add803(S803 x, S803 y) { return min(x, y); }
S803 o803() { return INFL; }
S803 mul803(S803 x, S803 y) { return x + y; }
S803 e803() { return 0; }
#define Min_plus_semiring S803, add803, o803, mul803, e803


//【max - plus 可環半環（トロピカル半環）】
/* verify : https://onlinejudge.u-aizu.ac.jp/courses/library/7/DPL/all/DPL_1_B */
using S804 = ll;
S804 add804(S804 x, S804 y) { return max(x, y); }
S804 o804() { return -INFL; }
S804 mul804(S804 x, S804 y) { return x + y; }
S804 e804() { return 0; }
#define Max_plus_semiring S804, add804, o804, mul804, e804


//【トロピカルアフィン変換の max - 合成 半環】
/*
* S ∋ f = {a, b} : トロピカル一次関数 f(x) = max(a + x, b) を表す．
* f add g : max(f, g)(x) = max(max(a + c) + x, max(b, d)) を表す．
* f mul g : 合成したトロピカル一次関数 f o g を返す．
*/
using S805 = pll; // 斉次行列 (a, b; -∞, 0)
S805 add805(S805 f, S805 g) {
	ll a, b, c, d;
	tie(a, b) = f; // 行列 (a, b; -∞, 0)
	tie(c, d) = g; // 行列 (c, d; -∞, 0)

	// (a, b; -∞, 0) + (c, d; -∞, 0) = (max(a, c), max(b, d); -∞, 0)
	return { max(a, c), max(b, d) };
}
S805 o805() { return { -INFL, -INFL }; }
S805 mul805(S805 f, S805 g) {
	ll a, b, c, d;
	tie(a, b) = f; // 行列 (a, b; -∞, 0)
	tie(c, d) = g; // 行列 (c, d; -∞, 0)

	// (a, b; -∞, 0) * (c, d; -∞, 0) = (a + c, max(a + d, b); -∞, 0)
	return { a + c, max(a + d, b) };
}
S805 e805() { return { 0, -INFL }; }
#define Tropical_affine_max_cmposite_semiring S805, add805, o805, mul805, e805


//【トロピカルアフィン変換の max - 逆合成 半環】
/*
* S ∋ f = {a, b} : トロピカル一次関数 f(x) = max(a + x, b) を表す．
* f add g : max(f, g)(x) = max(max(a + c) + x, max(b, d)) を表す．
* f mul g : 逆合成したトロピカル一次関数 g o f を返す．
* 
* グラフの隣接行列の成分として用いるなら，通る向きが f → g なのでこれを用いる．
*/
using S806 = pll; // 斉次行列 (a, b; -∞, 0)
S806 add806(S806 f, S806 g) {
	ll a, b, c, d;
	tie(a, b) = g; // 行列 (a, b; -∞, 0)
	tie(c, d) = f; // 行列 (c, d; -∞, 0)

	// (a, b; -∞, 0) + (c, d; -∞, 0) = (max(a, c), max(b, d); -∞, 0)
	return { max(a, c), max(b, d) };
}
S806 o806() { return { -INFL, -INFL }; }
S806 mul806(S806 f, S806 g) {
	ll a, b, c, d;
	tie(a, b) = g; // 行列 (a, b; -∞, 0)
	tie(c, d) = f; // 行列 (c, d; -∞, 0)

	// (a, b; -∞, 0) * (c, d; -∞, 0) = (a + c, max(a + d, b); -∞, 0)
	return { a + c, max(a + d, b) };
}
S806 e806() { return { 0, -INFL }; }
#define Tropical_affine_max_invcmposite_semiring S806, add806, o806, mul806, e806


//【min - max 可環半環】
/*
* 最大値の最小化で用いる．
* 
* verify : https://atcoder.jp/contests/abc236/tasks/abc236_g
*/
using S807 = ll;
S807 add807(S807 x, S807 y) { return min(x, y); }
S807 o807() { return INFL; }
S807 mul807(S807 x, S807 y) { return max(x, y); }
S807 e807() { return -INFL; }
#define Min_max_semiring S807, add807, o807, mul807, e807


//【max - min 可環半環】
/*
* 最小値の最大化で用いる．
*/
using S808 = ll;
S808 add808(S808 x, S808 y) { return max(x, y); }
S808 o808() { return -INFL; }
S808 mul808(S808 x, S808 y) { return min(x, y); }
S808 e808() { return INFL; }
#define Max_min_semiring S808, add808, o808, mul808, e808


//【OR - AND 可環半環】
using S809 = unsigned int;
S809 add809(S809 x, S809 y) { return x | y; }
S809 o809() { return 0; }
S809 mul809(S809 x, S809 y) { return x & y; }
S809 e809() { return ~0; }
#define OR_AND_semiring S809, add809, o809, mul809, e809


//【AND - OR 可環半環】
using S810 = unsigned int;
S810 add810(S810 x, S810 y) { return x & y; }
S810 o810() { return ~0; }
S810 mul810(S810 x, S810 y) { return x | y; }
S810 e810() { return 0; }
#define AND_OR_semiring S810, add810, o810, mul810, e810


//【gcd - lcm 可環半環】
using S811 = ll;
S811 add811(S811 x, S811 y) { return gcd(x, y); }
S811 o811() { return 0; }
S811 mul811(S811 x, S811 y) { return x / gcd(x, y) * y; }
S811 e811() { return 1; }
#define GCD_LCM_semiring S811, add811, o811, mul811, e811


//【lcm - gcd 可環半環】
using S812 = ll;
S812 add812(S812 x, S812 y) { return x / gcd(x, y) * y; }
S812 o812() { return 1; }
S812 mul812(S812 x, S812 y) { return gcd(x, y); }
S812 e812() { return 0; }
#define LCM_GCD_semiring S812, add812, o812, mul812, e812


//【ニム和 - ニム積 可環半環】


//【半環でないもの】
/*
* min - OR は半環でない．
*	2 OR min(1, 2) = 2 OR 1 = 3
*	min(2 OR 1, 2 OR 2) = min(3, 2) = 2
* 
* XOR - 積 は半環でない．
*	3 * (1 XOR 2) = 3 * 3 = 9
*	(3 * 1) XOR (3 * 2) = 3 XOR 6 = 5
*/

