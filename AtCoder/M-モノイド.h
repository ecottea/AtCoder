#pragma once
#include "header.h"
// ■■■■■ モノイド作用付きモノイド ■■■■■


//【モノイド左作用付きモノイド】
/*
* モノイド左作用付きモノイド (S, op, e, F, act, comp, id) を表す．
*
* すなわち，(S, op, e), (F, comp, id) がそれぞれモノイドであり，
* F の S への左作用 act : F × S → S が
*	準同型 : ∀x, y ∈ S，∀f ∈ F,	f(x y) = f(x) f(y)
*	合成   : ∀x ∈ S, ∀f, g ∈ F,	(f g)(x) = f(g(x))
*   恒等射 : ∀x ∈ S,				id(x) = x
* を満たすものとする．
*/


//【乗算 作用付き 総和 モノイド】
/* verify : https://atcoder.jp/contests/acl1/tasks/acl1_e */
using S101 = mint;
S101 op101(S101 x, S101 y) { return x + y; }
S101 e101() { return 0; }
using F101 = mint;
S101 act101(F101 f, S101 x) { return f * x; }
F101 comp101(F101 f, F101 g) { return f * g; }
F101 id101() { return 1; }
#define Mul_Sum_mmonoid S101, op101, e101, F101, act101, comp101, id101


//【chmin 作用付き min モノイド】
/* verify : https://yukicoder.me/problems/no/1868 */
using S115 = ll;
S115 op115(S115 x, S115 y) { return min(x, y); }
S115 e115() { return INFL; }
using F115 = ll;
S115 act115(F115 f, S115 x) { return min(f, x); }
F115 comp115(F115 f, F115 g) { return min(f, g); }
F115 id115() { return INFL; }
#define Chmin_Min_mmonoid S115, op115, e115, F115, act115, comp115, id115


//【chmax 作用付き max モノイド】
/* verify : https://atcoder.jp/contests/abc177/tasks/abc177_f */
using S116 = ll;
S116 op116(S116 x, S116 y) { return max(x, y); }
S116 e116() { return -INFL; }
using F116 = ll;
S116 act116(F116 f, S116 x) { return max(f, x); }
F116 comp116(F116 f, F116 g) { return max(f, g); }
F116 id116() { return -INFL; }
#define Chmax_Max_mmonoid S116, op116, e116, F116, act116, comp116, id116


//【変更 作用付き 左変更 モノイド】
using S102 = int;
S102 e102() { return INF + 2; } // 使わない値なら何でも OK
S102 op102(S102 x, S102 y) { return x == e102() ? y : x; }
using F102 = int;
F102 id102() { return INF + 1; } // 使わない値なら何でも OK
S102 act102(F102 f, S102 x) { return f == id102() ? x : f; }
F102 comp102(F102 f, F102 g) { return f == id102() ? g : f; }
#define Update_LUpdate_mmonoid S102, op102, e102, F102, act102, comp102, id102


//【変更 作用付き max モノイド】
using S103 = int;
S103 op103(S103 x, S103 y) { return max(x, y); }
S103 e103() { return -INF; }
using F103 = int;
F103 id103() { return INF + 1; } // 使わない値なら何でも OK
S103 act103(F103 f, S103 x) { return f == id103() ? x : f; }
F103 comp103(F103 f, F103 g) { return f == id103() ? g : f; }
#define Update_Max_mmonoid S103, op103, e103, F103, act103, comp103, id103


//【変更 作用付き min モノイド】
/* verify : https://atcoder.jp/contests/abc177/tasks/abc177_f */
using S104 = int;
S104 op104(S104 x, S104 y) { return min(x, y); }
S104 e104() { return INF; }
using F104 = int;
F104 id104() { return INF + 1; } // 使わない値なら何でも OK
S104 act104(F104 f, S104 x) { return f == id104() ? x : f; }
F104 comp104(F104 f, F104 g) { return f == id104() ? g : f; }
#define Update_Min_mmonoid S104, op104, e104, F104, act104, comp104, id104


//【加算 作用付き max モノイド】
/* verify : https://atcoder.jp/contests/arc017/tasks/arc017_4 */
using S105 = ll;
S105 op105(S105 x, S105 y) { return max(x, y); }
S105 e105() { return -INFL; }
using F105 = ll;
S105 act105(F105 f, S105 x) { return f + x; }
F105 comp105(F105 f, F105 g) { return f + g; }
F105 id105() { return 0; }
#define Add_Max_mmonoid S105, op105, e105, F105, act105, comp105, id105


//【加算 作用付き min モノイド】
/* verify : https://atcoder.jp/contests/abc245/tasks/abc245_e */
using S106 = ll;
S106 op106(S106 x, S106 y) { return min(x, y); }
S106 e106() { return INFL; }
using F106 = ll;
S106 act106(F106 f, S106 x) { return f + x; }
F106 comp106(F106 f, F106 g) { return f + g; }
F106 id106() { return 0; }
#define Add_Min_mmonoid S106, op106, e106, F106, act106, comp106, id106


//【アフィン 作用付き 総和 モノイド】
/*
* S ∋ x = {v, c} : c 個の元の和で値 v をとっていることを表す．
* F ∋ f = {a, b} : 一次関数 f(x) = a x + b を表す．
* x op y : cx + cy 個の元の和で値 vx + vy をとっている状態にする．
* f act x : c 個の元の和で値 f(v) をとっている状態にする．
* f comp g : 合成した一次関数 f o g を返す．
*/
// verify : https://judge.yosupo.jp/problem/range_affine_range_sum
using T107 = mint;
using S107 = pair<T107, T107>; // ベクトル (v, c)
using F107 = pair<T107, T107>; // 行列 (a, b; 0, 1)
S107 op107(S107 x, S107 y) {
	T107 vx, vy, cx, cy;
	tie(vx, cx) = x; // ベクトル (vx, cx)
	tie(vy, cy) = y; // ベクトル (vy, cy)

	// (vx, cx) + (vy, cy) = (vx + vy, cx + cy)
	return { vx + vy, cx + cy };
}
S107 e107() { return { 0, 0 }; }
S107 act107(F107 f, S107 x) {
	T107 v, c, a, b;
	tie(v, c) = x; // ベクトル (v, c)
	tie(a, b) = f; // 行列 (a, b; 0, 1)

	// (a, b; 0, 1).(v, c) = (a v + b c, c)
	return { a * v + b * c, c };
}
F107 comp107(F107 f, F107 g) {
	T107 a, b, c, d;
	tie(a, b) = f; // 行列 (a, b; 0, 1)
	tie(c, d) = g; // 行列 (c, d; 0, 1)

	// (a, b; 0, 1).(c, d; 0, 1) = (a c, a d + b; 0, 1)
	return { a * c, a * d + b };
}
F107 id107() { return { 1, 0 }; }
#define Affine_Sum_mmonoid S107, op107, e107, F107, act107, comp107, id107


//【加算 作用付き 総和 モノイド】
/*
* S ∋ x = {v, c} : c 個の元の和で値 v をとっていることを表す．
* F ∋ f : 一次関数 x ← 1 x + f を表す．
* x op y : cx + cy 個の元の和で値 vx + vy をとっている状態にする．
* f act x : c 個の元の和で値 v + c f をとっている状態にする．
* f comp g : 合成した一次関数 f o g を返す．
*/
using S108 = pair<ll, ll>; // ベクトル (v, c)
using F108 = ll; // 行列 (1, f; 0, 1)
S108 op108(S108 x, S108 y) {
	ll vx, vy, cx, cy;
	tie(vx, cx) = x; // ベクトル (vx, cx)
	tie(vy, cy) = y; // ベクトル (vy, cy)

	// (vx, cx) + (vy, cy) = (vx + vy, cx + cy)
	return { vx + vy, cx + cy };
}
S108 e108() { return { 0, 0 }; }
S108 act108(F108 f, S108 x) {
	ll v, c;
	tie(v, c) = x; // ベクトル (v, c)

	// (1, f; 0, 1).(v, c) = (v + f c, c)
	return { v + f * c, c };
}
F108 comp108(F108 f, F108 g) {
	// (1, f; 0, 1).(1, g; 0, 1) = (1, f + g; 0, 1)
	return f + g;
}
F108 id108() { return 0; }
#define Add_Sum_mmonoid S108, op108, e108, F108, act108, comp108, id108


//【変更 作用付き 総和 モノイド】
/*
* S ∋ x = {v, c} : c 個の元の和で値 v をとっていることを表す．
* F ∋ f : 零次関数 x ← 0 x + f を表す．
* x op y : cx + cy 個の元の和で値 vx + vy をとっている状態にする．
* f act x : c 個の元の和で値 c f をとっている状態にする．
* f comp g : 合成した零次関数 f o g を返す．
*/
// verify : https://atcoder.jp/contests/abc237/tasks/abc237_g
using S109 = pair<ll, ll>; // ベクトル (v, c)
using F109 = ll; // 行列 (0, f; 0, 1)
S109 op109(S109 x, S109 y) {
	ll vx, vy, cx, cy;
	tie(vx, cx) = x; // ベクトル (vx, cx)
	tie(vy, cy) = y; // ベクトル (vy, cy)

	// (vx, cx) + (vy, cy) = (vx + vy, cx + cy)
	return { vx + vy, cx + cy };
}
S109 e109() { return { 0, 0 }; }
F109 id109() { return INFL + 1; } // 使わない値なら何でも OK
S109 act109(F109 f, S109 x) {
	if (f == id109()) return x;

	ll v, c;
	tie(v, c) = x; // ベクトル (v, c)

	// (0, f; 0, 1).(v, c) = (f c, c)
	return { f * c, c };
}
F109 comp109(F109 f, F109 g) {
	if (f == id109()) return g;

	// (0, f; 0, 1).(0, g; 0, 1) = (0, f; 0, 1)
	return f;
}
#define Update_Sum_mmonoid S109, op109, e109, F109, act109, comp109, id109


//【AND 作用付き XOR モノイド】
using S110 = int;
S110 op110(S110 x, S110 y) { return x ^ y; }
S110 e110() { return 0; }
using F110 = int;
S110 act110(F110 f, S110 x) { return f & x; }
F110 comp110(F110 f, F110 g) { return f & g; }
F110 id110() { return ~0; }
#define AND_XOR_mmonoid S110, op110, e110, F110, act110, comp110, id110


//【トロピカルアフィン 作用付き max モノイド】
/*
* S ∋ x : トロピカル加法（max）が冪等なので斉次座標を持つ必要がない．
* F ∋ f = {a, b} : トロピカル一次関数 f(x) = max(a + x, b) を表す．
* x op y : トロピカル加法 max(x, y)
* f act x : トロピカル一次関数への代入 f(x)
* f comp g : 合成したトロピカル一次関数 f o g を返す．
*/
using S111 = ll; // ベクトル (x, 0)
using F111 = pll; // 行列 (a, b; -∞, 0)
S111 op111(S111 x, S111 y) { return max(x, y); }
S111 e111() { return -INFL; }
S111 act111(F111 f, S111 x) {
	ll a, b;
	tie(a, b) = f; // 行列 (a, b; -∞, 0)

	// (a, b; -∞, 0).(x, 0) = (max(a + x, b), 0)
	return max(a + x, b);
}
F111 comp111(F111 f, F111 g) {
	ll a, b, c, d;
	tie(a, b) = f; // 行列 (a, b; -∞, 0)
	tie(c, d) = g; // 行列 (c, d; -∞, 0)

	// (a, b; -∞, 0).(c, d; -∞, 0) = (a + c, max(a + d, b); -∞, 0)
	return { a + c, max(a + d, b) };
}
F111 id111() { return { 0, -INFL }; }
#define TropicalAffine_Max_mmonoid S111, op111, e111, F111, act111, comp111, id111


//【ビット列上 XOR 作用付き 転倒数 モノイド】
/*
* S ∋ x = {inv, c0, c1} : 列 x の転倒数，0 の個数，1 の個数の組
* F ∋ f : f との XOR をとる作用を表す
* x op y : 列 x, y を連結した列
* f act x : 列 x の各要素と f との XOR をとった列
* f comp g : f XOR g
*/
// verify : https://atcoder.jp/contests/practice2/tasks/practice2_l
using S112 = tuple<ll, ll, ll>;
using F112 = bool;
S112 op112(S112 x, S112 y) {
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
S112 e112() { return { 0LL, 0, 0 }; }
S112 act112(F112 f, S112 x) {
	if (!f) return x;

	ll inv, c0, c1;
	tie(inv, c0, c1) = x;

	// 0 と 1 の組は全部で c0 * c1 個存在する．
	// そのうち inv 個が転倒していたのだから，転倒していないのは c0 * c1 - inv 個である．
	// ビット反転するとこれらが入れ替わるので，転倒数は c0 * c1 - inv になる．
	inv = c0 * c1 - inv;

	return { inv, c1, c0 };
}
F112 comp112(F112 f, F112 g) {
	return f ^ g;
}
F112 id112() { return false; }
#define XOR_Inversion_mmonoid S112, op112, e112, F112, act112, comp112, id112


//【乗算 作用付き GCD モノイド】
using S113 = ll;
S113 op113(S113 x, S113 y) { return gcd(abs(x), abs(y)); }
S113 e113() { return 0; }
using F113 = ll;
S113 act113(F113 f, S113 x) { return f * x; }
F113 comp113(F113 f, F113 g) { return f * g; }
F113 id113() { return 1; }
#define Mul_GCD_mmonoid S113, op113, e113, F113, act113, comp113, id113


//【乗算 作用付き LCM モノイド】
using S114 = ll;
S114 op114(S114 x, S114 y) { return x / gcd(abs(x), abs(y)) * y; }
S114 e114() { return 1; }
using F114 = ll;
S114 act114(F114 f, S114 x) { return f * x; }
F114 comp114(F114 f, F114 g) { return f * g; }
F114 id114() { return 1; }
#define Mul_LCM_mmonoid S114, op114, e114, F114, act114, comp114, id114


//【線形加算 作用付き 総和 モノイド】
/*
* S ∋ x = {v, i, 1} : 添字 i の元が値 v をとっていることを表す．
* F ∋ f = {a, b} : 一次関数 f(x) = a x + b を表す．
* x op y : 値 vx + vy をとっている状態にする（添字とかは壊れる）
* f act x : v に値 a i + b を加算する．
* f comp g : (af + ag) i + (bf + bg) を加算する作用にする．
*/
// verify : https://atcoder.jp/contests/abc268/tasks/abc268_e
using T117 = ll;
using S117 = tuple<T117, T117, T117>; // ベクトル (v, i, 1)
using F117 = pair<T117, T117>; // 行列 (1, a, b; 0, 1, 0; 0, 0, 1)
S117 op117(S117 x, S117 y) {
	T117 vx, vy, sx, sy, cx, cy;
	tie(vx, sx, cx) = x; // ベクトル (vx, sx, cx)
	tie(vy, sy, cy) = y; // ベクトル (vy, sy, cy)

	// (vx, sx, cx) + (vy, sy, cy) = (vx + vy, sx + sy, cx + cy)
	return { vx + vy, sx + sy, cx + cy };
}
S117 e117() { return { 0, 0, 0 }; }
S117 act117(F117 f, S117 x) {
	T117 v, s, c, a, b;
	tie(v, s, c) = x; // ベクトル (v, s, c)
	tie(a, b) = f; // 行列 (1, a, b; 0, 1, 0; 0, 0, 1)

	// (1, a, b; 0, 1, 0; 0, 0, 1).(v, s, c) = (v + a s + b c, s, c)
	return { v + a * s + b * c, s, c };
}
F117 comp117(F117 f, F117 g) {
	T117 a, b, c, d;
	tie(a, b) = f; // 行列 (1, a, b; 0, 1, 0; 0, 0, 1)
	tie(c, d) = g; // 行列 (1, c, d; 0, 1, 0; 0, 0, 1)

	// (1, a, b; 0, 1, 0; 0, 0, 1).(1, c, d; 0, 1, 0; 0, 0, 1) = (1, a + c, b + d; 0, 1, 0; 0, 0, 1)
	return { a + c, b + d };
}
F117 id117() { return { 0, 0 }; }
#define LinearAdd_Sum_mmonoid S117, op117, e117, F117, act117, comp117, id117


//【変更 作用付き OR モノイド】
/* verify : https://codeforces.com/contest/620/problem/E */
using S118 = ll;
S118 op118(S118 x, S118 y) { return x | y; }
S118 e118() { return 0; }
using F118 = ll;
F118 id118() { return -INFL - 1; } // 使わない値なら何でも OK
S118 act118(F118 f, S118 x) { return f == id118() ? x : f; }
F118 comp118(F118 f, F118 g) { return f == id118() ? g : f; }
#define Update_OR_mmonoid S118, op118, e118, F118, act118, comp118, id118


//【加算 作用付き 最小元の個数 モノイド】
/*
* S ∋ x = {v, c} : 最小値 v をもつ要素が c 個あることを表す．
* F ∋ f : f 加算する作用を表す．
*/
using S119 = pair<ll, int>; // (v, c)
using F119 = ll;
S119 op119(S119 x, S119 y) {
	auto [vx, cx] = x;
	auto [vy, cy] = y;

	if (vx < vy) return x;
	if (vx > vy) return y;
	return { vx, cx + cy };
}
S119 e119() { return { INFL, 0 }; }
S119 act119(F119 f, S119 x) {
	auto [vx, cx] = x;

	return { vx + f, cx };
}
F119 comp119(F119 f, F119 g) {
	return f + g;
}
F119 id119() { return 0; }
#define Add_CntMin_mmonoid S119, op119, e119, F119, act119, comp119, id119


