#pragma once
#include "header.h"
#include "行列.h"
#include "畳込み(他演算).h"
// ■■■■■ 半環 ■■■■■


//【半環の定義】
/*
* 半環 (S, add, o, mul, e) を表す．
*
* すなわち，(S, add, o) が可換モノイド，(S, mul, e) がモノイドで，
*	分配律 : ∀a, b, c ∈ S， a(b + c) = a b + a c, (a + b)c = a c + b c
*	零化   : ∀a ∈ S,        a o = o a = o
* を満たすものとする．
*/


//【加算 - 乗算 可換半環】
/* verify : https://www.codechef.com/AUG23A/problems/SPCHEF */
using S801 = mint;
S801 add801(S801 x, S801 y) { return x + y; }
S801 o801() { return S801(0); }
S801 mul801(S801 x, S801 y) { return x * y; }
S801 e801() { return S801(1); }
#define Add_mul_semiring S801, add801, o801, mul801, e801


//【行列の 加算 - 乗算 半環】
/* verify : https://yukicoder.me/problems/no/2448 */
constexpr int size813 = 2;
using S813 = Fixed_matrix<mint, size813>;
S813 add813(S813 x, S813 y) { return x + y; }
S813 o813() { return S813(0); }
S813 mul813(S813 x, S813 y) { return x * y; }
S813 e813() { return S813(1); }
#define MatrixAdd_mul_semiring S813, add813, o813, mul813, e813


//【FPS の 加算 - 乗算 可換半環】
/* verify : https://atcoder.jp/contests/abc317/tasks/abc317_h */
using S814 = MFPS;
S814 add814(S814 x, S814 y) { return x + y; }
S814 o814() { return S814(); }
S814 mul814(S814 x, S814 y) { return x * y; }
S814 e814() { return S814(1); }
#define MFPSAdd_mul_semiring S814, add814, o814, mul814, e814


//【min - plus 可環半環（トロピカル半環）】
/* verify : https://codeforces.com/contest/1681/problem/F */
using S803 = ll;
S803 add803(S803 x, S803 y) { return min(x, y); }
S803 o803() { return INFL; }
S803 mul803(S803 x, S803 y) { return x + y; }
S803 e803() { return 0; }
#define Min_plus_semiring S803, add803, o803, mul803, e803


//【max - plus 可環半環（トロピカル半環）】
/* verify : https://atcoder.jp/contests/abc310/tasks/abc310_h */
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
using T805 = ll;
using S805 = pair<T805, T805>; // 斉次行列 (a, b; -∞, 0)
S805 add805(S805 f, S805 g) {
	auto [a, b] = f; // 行列 (a, b; -∞, 0)
	auto [c, d] = g; // 行列 (c, d; -∞, 0)

	// (a, b; -∞, 0) + (c, d; -∞, 0) = (max(a, c), max(b, d); -∞, 0)
	return { max(a, c), max(b, d) };
}
S805 o805() { return { -INFL, -INFL }; }
S805 mul805(S805 f, S805 g) {
	auto [a, b] = f; // 行列 (a, b; -∞, 0)
	auto [c, d] = g; // 行列 (c, d; -∞, 0)

	// (a, b; -∞, 0) * (c, d; -∞, 0) = (a + c, max(a + d, b); -∞, 0)
	return { a + c, max(a + d, b) };
}
S805 e805() { return { 0, -INFL }; }
#define Tropical_affine_max_composite_semiring S805, add805, o805, mul805, e805


//【トロピカルアフィン変換の max - 逆合成 半環】
/*
* S ∋ f = {a, b} : トロピカル一次関数 f(x) = max(a + x, b) を表す．
* f add g : max(f, g)(x) = max(max(a + c) + x, max(b, d)) を表す．
* f mul g : 逆合成したトロピカル一次関数 g o f を返す．
* 
* グラフの隣接行列の成分として用いるなら，通る向きが f → g なのでこれを用いる．
*/
// verify : https://atcoder.jp/contests/ddcc2020-final/tasks/ddcc2020_final_b
using T806 = ll;
using S806 = pair<T806, T806>; // 斉次行列 (a, b; -∞, 0)
S806 add806(S806 f, S806 g) {
	auto [a, b] = g; // 行列 (a, b; -∞, 0)
	auto [c, d] = f; // 行列 (c, d; -∞, 0)

	// (a, b; -∞, 0) + (c, d; -∞, 0) = (max(a, c), max(b, d); -∞, 0)
	return { max(a, c), max(b, d) };
}
S806 o806() { return { -INFL, -INFL }; }
S806 mul806(S806 f, S806 g) {
	auto [a, b] = g; // 行列 (a, b; -∞, 0)
	auto [c, d] = f; // 行列 (c, d; -∞, 0)

	// (a, b; -∞, 0) * (c, d; -∞, 0) = (a + c, max(a + d, b); -∞, 0)
	return { a + c, max(a + d, b) };
}
S806 e806() { return { 0, -INFL }; }
#define Tropical_affine_max_invcomposite_semiring S806, add806, o806, mul806, e806


//【max-plus 数列の 各点 max - 畳込み 可換半環】
/*
* S ∋ a : 数列 a[0..n) を表す．
* a add b : a[0..n) と b[0..m) の各点 max を表す（無い要素は -∞ とする）
* a mul b : a[0..n) と b[0..m) の max-plus 畳込みを表す．
*
* 利用：【max-plus 畳込み】
*/
using S815 = vl;
S815 add815(S815 x, S815 y) { // O(n + m)
	int n = sz(x), m = sz(y);

	S815 res(max(n, m), -INFL);
	rep(i, n) chmax(res[i], x[i]);
	rep(j, m) chmax(res[j], y[j]);

	return res;
}
S815 o815() { return S815(); }
S815 mul815(S815 x, S815 y) { return naive_max_plus_convolution(x, y); } // O(n m)
S815 e815() { return S815(0); }
#define MPlusSeq_Max_Conv_semiring S815, add815, o815, mul815, e815


//【max-plus 数列（上に凸）の 各点 max - 畳込み 可換半環】
/*
* S ∋ a : 上に凸な数列 a[0..n) を表す．
* a add b : a[0..n) と b[0..m) の各点 max を表す（無い要素は -∞ とする）
* a mul b : a[0..n) と b[0..m) の max-plus 畳込みを表す．
*
* 利用：【max-plus 畳込み（上に凸）】
*/
/* verify : https://atcoder.jp/contests/abc218/tasks/abc218_h */
using S816 = vl;
S816 add816(S816 x, S816 y) { // O(n + m)
	int n = sz(x), m = sz(y);

	S816 res(max(n, m), -INFL);
	rep(i, n) chmax(res[i], x[i]);
	rep(j, m) chmax(res[j], y[j]);

	return res;
}
S816 o816() { return S816(); }
S816 mul816(S816 x, S816 y) { return concave_max_plus_convolution(x, y); } // O(n + m)
S816 e816() { return S816(0); }
#define MaxPlusConcaveSeq_Max_Conv_semiring S816, add816, o816, mul816, e816


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


//【XOR - AND 可環半環】
using S802 = unsigned int;
S802 add802(S802 x, S802 y) { return x ^ y; }
S802 o802() { return 0; }
S802 mul802(S802 x, S802 y) { return x & y; }
S802 e802() { return ~0; }
#define XOR_AND_semiring S802, add802, o802, mul802, e802


//【OR - AND 可環半環】
/* verify : https://yukicoder.me/problems/no/1340 */
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


//【GCD - LCM 可環半環】
using S811 = ll;
S811 add811(S811 x, S811 y) { return gcd(x, y); }
S811 o811() { return 0; }
S811 mul811(S811 x, S811 y) { return lcm(x, y); }
S811 e811() { return 1; }
#define GCD_LCM_semiring S811, add811, o811, mul811, e811


//【LCM - GCD 可環半環】
using S812 = ll;
S812 add812(S812 x, S812 y) { return lcm(x, y); }
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


// ======================================================

//【半環】
/*
* 環 (S, add, o, mul, e) の元を表す（add, mul は +, * をそれぞれオーバーロードする）
*/
template <class S, S(*add)(S, S), S(*o_)(), S(*mul)(S, S), S(*e_)()>
struct Semiring {
	// verify : https://atcoder.jp/contests/abc348/tasks/abc348_g

	S v;

	// 零元
	static S o() { return o_(); }

	// 単位元
	static S e() { return e_(); }

	// コンストラクタ
	Semiring() : v(o()) {}
	Semiring(S v) : v(v) {}

	// キャスト
	operator S() const { return v; }

	// 比較
	bool operator==(const Semiring& b) const { return v == b.v; }
	bool operator!=(const Semiring& b) const { return v != b.v; }

	// 二項演算
	Semiring& operator+=(const Semiring& b) { v = add(v, b.v); return *this; }
	Semiring& operator*=(const Semiring& b) { v = mul(v, b.v); return *this; }
	friend Semiring operator+(Semiring a, const Semiring& b) { a += b; return a; }
	friend Semiring operator*(Semiring a, const Semiring& b) { a *= b; return a; }

	// 入出力
	friend istream& operator>>(istream& is, Semiring& a) { is >> a.v; return is; }
	friend ostream& operator<<(ostream& os, const Semiring& a) {
#ifdef _MSC_VER
		if (a.v == o()) return os << "o";
		if (a.v == e()) return os << "e";
#endif
		return os << a.v;
	}
};


