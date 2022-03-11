#pragma once
#include "header.h"
// ■■■■■ モノイド作用付きモノイド ■■■■■


//【モノイド左作用付きモノイド】
/*
* モノイド左作用付きモノイド (S, op, e, F, mapping, composition, id) を表す．
*
* すなわち，(S, op, e), (F, composition, id) がそれぞれモノイドであり，
* F の S への左作用 * : F × S → S が
*	準同型 : ∀x, y ∈ S，∀f ∈ F,  f(x y) = f(x) f(y)
*	合成   : ∀x ∈ S, ∀f, g ∈ F,  (f g)(x) = f(g(x))
*   恒等射 : ∀x ∈ S,               id(x) = x
* を満たすものとする．
*/


//【乗算 作用付き 加算 モノイド】
using S101 = mint;
S101 op101(S101 x, S101 y) { return x + y; }
S101 e101() { return 0; }
using F101 = mint;
S101 mapping101(F101 f, S101 x) { return f * x; }
F101 composition101(F101 f, F101 g) { return f * g; }
F101 id101() { return 1; }
#define Mul_Add_mlop_monoid S101, op101, e101, F101, mapping101, composition101, id101


//【変更 作用付き 左変更 モノイド】
using S102 = int;
S102 e102() { return INF; } // 使わない値なら何でも OK
S102 op102(S102 x, S102 y) { return x == e102() ? y : x; }
using F102 = int;
F102 id102() { return INF; } // 使わない値なら何でも OK
S102 mapping102(F102 f, S102 x) { return f == id102() ? x : f; }
F102 composition102(F102 f, F102 g) { return f == id102() ? g : f; }
#define Update_Lupdate_mlop_monoid S102, op102, e102, F102, mapping102, composition102, id102


//【変更 作用付き max モノイド】
using S103 = int;
S103 op103(S103 x, S103 y) { return max(x, y); }
S103 e103() { return -INF; }
using F103 = int;
F103 id103() { return INF; } // 使わない値なら何でも OK
S103 mapping103(F103 f, S103 x) { return f == id103() ? x : f; }
F103 composition103(F103 f, F103 g) { return f == id103() ? g : f; }
#define Update_max_mlop_monoid S103, op103, e103, F103, mapping103, composition103, id103


//【変更 作用付き min モノイド】
using S104 = int;
S104 op104(S104 x, S104 y) { return min(x, y); }
S104 e104() { return INF; }
using F104 = int;
F104 id104() { return INF; } // 使わない値なら何でも OK
S104 mapping104(F104 f, S104 x) { return f == id104() ? x : f; }
F104 composition104(F104 f, F104 g) { return f == id104() ? g : f; }
#define update_min_mlop_monoid S104, op104, e104, F104, mapping104, composition104, id104


//【加算 作用付き max モノイド】
using S105 = ll;
S105 op105(S105 x, S105 y) { return max(x, y); }
S105 e105() { return -INFL; }
using F105 = ll;
S105 mapping105(F105 f, S105 x) { return f + x; }
F105 composition105(F105 f, F105 g) { return f + g; }
F105 id105() { return 0; }
#define Add_max_mlop_monoid S105, op105, e105, F105, mapping105, composition105, id105


//【加算 作用付き min モノイド】
using S106 = ll;
S106 op106(S106 x, S106 y) { return min(x, y); }
S106 e106() { return INFL; }
using F106 = ll;
S106 mapping106(F106 f, S106 x) { return f + x; }
F106 composition106(F106 f, F106 g) { return f + g; }
F106 id106() { return 0; }
#define Add_min_mlop_monoid S106, op106, e106, F106, mapping106, composition106, id106


//【アフィン変換 作用付き 加算 モノイド】
/*
* S ∋ x = {v, c} : c 個の元の和で値 v をとっていることを表す．
* F ∋ f = {a, b} : 一次関数 f(x) = a x + b を表す．
* x op y : cx + cy 個の元の和で値 vx + vy をとっている状態にする．
* f mapping x : c 個の元の和で値 f(v) をとっている状態にする．
* f composition g : 合成した一次関数 f o g を返す．
*/
// verify : https://judge.yosupo.jp/problem/range_affine_range_sum
using S107 = pair<mint, mint>; // 斉次ベクトル (v, c)
using F107 = pair<mint, mint>; // 斉次行列 (a, b; 0, 1)
S107 op107(S107 x, S107 y) {
	mint vx, vy, cx, cy;
	tie(vx, cx) = x; // ベクトル (vx, cx)
	tie(vy, cy) = y; // ベクトル (vy, cy)

	// (vx, cx) + (vy, cy) = (vx + vy, cx + cy)
	return { vx + vy, cx + cy };
}
S107 e107() { return { 0, 0 }; }
S107 mapping107(F107 f, S107 x) {
	mint v, c, a, b;
	tie(v, c) = x; // ベクトル (v, c)
	tie(a, b) = f; // 行列 (a, b; 0, 1)

	// (a, b; 0, 1).(v, c) = (a v + b c, c)
	return { a * v + b * c, c };
}
F107 composition107(F107 f, F107 g) {
	mint a, b, c, d;
	tie(a, b) = f; // 行列 (a, b; 0, 1)
	tie(c, d) = g; // 行列 (c, d; 0, 1)

	// (a, b; 0, 1).(c, d; 0, 1) = (a c, a d + b; 0, 1)
	return { a * c, a * d + b };
}
F107 id107() { return { 1, 0 }; }
#define Affine_add_mlop_monoid S107, op107, e107, F107, mapping107, composition107, id107


//【加算 作用付き 加算 モノイド】
/*
* S ∋ x = {v, c} : c 個の元の和で値 v をとっていることを表す．
* F ∋ f : 一次関数 x ← 1 x + f を表す．
* x op y : cx + cy 個の元の和で値 vx + vy をとっている状態にする．
* f mapping x : c 個の元の和で値 v + c f をとっている状態にする．
* f composition g : 合成した一次関数 f o g を返す．
*/
using S108 = pair<ll, ll>; // 斉次ベクトル (v, c)
using F108 = ll; // 斉次行列 (1, f; 0, 1)
S108 op108(S108 x, S108 y) {
	ll vx, vy, cx, cy;
	tie(vx, cx) = x; // ベクトル (vx, cx)
	tie(vy, cy) = y; // ベクトル (vy, cy)

	// (vx, cx) + (vy, cy) = (vx + vy, cx + cy)
	return { vx + vy, cx + cy };
}
S108 e108() { return { 0, 0 }; }
S108 mapping108(F108 f, S108 x) {
	ll v, c;
	tie(v, c) = x; // ベクトル (v, c)

	// (1, f; 0, 1).(v, c) = (v + f c, c)
	return { v + f * c, c };
}
F108 composition108(F108 f, F108 g) {
	// (1, f; 0, 1).(1, g; 0, 1) = (1, f + g; 0, 1)
	return f + g;
}
F108 id108() { return 0; }
#define Add_add_mlop_monoid S108, op108, e108, F108, mapping108, composition108, id108


//【変更 作用付き 加算 モノイド】
/*
* S ∋ x = {v, c} : c 個の元の和で値 v をとっていることを表す．
* F ∋ f : 零次関数 x ← 0 x + f を表す．
* x op y : cx + cy 個の元の和で値 vx + vy をとっている状態にする．
* f mapping x : c 個の元の和で値 c f をとっている状態にする．
* f composition g : 合成した零次関数 f o g を返す．
*/
// verify : https://atcoder.jp/contests/abc237/tasks/abc237_g
using S109 = pair<ll, ll>; // 斉次ベクトル (v, c)
using F109 = ll; // 斉次行列 (0, f; 0, 1)
S109 op109(S109 x, S109 y) {
	ll vx, vy, cx, cy;
	tie(vx, cx) = x; // ベクトル (vx, cx)
	tie(vy, cy) = y; // ベクトル (vy, cy)

	// (vx, cx) + (vy, cy) = (vx + vy, cx + cy)
	return { vx + vy, cx + cy };
}
S109 e109() { return { 0, 0 }; }
F109 id109() { return INFL; } // 使わない値なら何でも OK
S109 mapping109(F109 f, S109 x) {
	if (f == id109()) return x;

	ll v, c;
	tie(v, c) = x; // ベクトル (v, c)

	// (0, f; 0, 1).(v, c) = (f c, c)
	return { f * c, c };
}
F109 composition109(F109 f, F109 g) {
	if (f == id109()) return g;

	// (0, f; 0, 1).(0, g; 0, 1) = (0, f; 0, 1)
	return f;
}
#define Update_add_mlop_monoid S109, op109, e109, F109, mapping109, composition109, id109


//【and 作用付き xor モノイド】
using S110 = int;
S110 op110(S110 x, S110 y) { return x ^ y; }
S110 e110() { return 0; }
using F110 = int;
S110 mapping110(F110 f, S110 x) { return f & x; }
F110 composition110(F110 f, F110 g) { return f & g; }
F110 id110() { return ~0; }
#define AND_XOR_mlop_monoid S110, op110, e110, F110, mapping110, composition110, id110


//【トロピカルアフィン変換 作用付き max モノイド】
/*
* S ∋ x : トロピカル加法（max）が冪等なので斉次座標を持つ必要がない．
* F ∋ f = {a, b} : トロピカル一次関数 f(x) = max(a + x, b) を表す．
* x op y : トロピカル加法 max(x, y)
* f mapping x : トロピカル一次関数への代入 f(x)
* f composition g : 合成したトロピカル一次関数 f o g を返す．
*/
using S111 = ll; // 斉次ベクトル (x, 0)
using F111 = pll; // 斉次行列 (a, b; -∞, 0)
S111 op111(S111 x, S111 y) { return max(x, y); }
S111 e111() { return -INFL; }
S111 mapping111(F111 f, S111 x) {
	ll a, b;
	tie(a, b) = f; // 行列 (a, b; -∞, 0)

	// (a, b; -∞, 0).(x, 0) = (max(a + x, b), 0)
	return max(a + x, b);
}
F111 composition111(F111 f, F111 g) {
	ll a, b, c, d;
	tie(a, b) = f; // 行列 (a, b; -∞, 0)
	tie(c, d) = g; // 行列 (c, d; -∞, 0)

	// (a, b; -∞, 0).(c, d; -∞, 0) = (a + c, max(a + d, b); -∞, 0)
	return { a + c, max(a + d, b) };
}
F111 id111() { return { 0, -INFL }; }
#define Tropical_affine_max_mlop_monoid S111, op111, e111, F111, mapping111, composition111, id111


//【ビット列上 xor 作用付き 転倒数 モノイド】
/*
* S ∋ x = {inv, c0, c1} : 列 x の転倒数，0 の個数，1 の個数の組
* F ∋ f : f との xor をとる作用を表す
* x op y : 列 x, y を連結した列
* f mapping x : 列 x の各要素と f との xor をとった列
* f composition g : f xor g
*/
// verify : https://atcoder.jp/contests/practice12/tasks/practice12_l
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
S112 mapping112(F112 f, S112 x) {
	if (!f) return x;

	ll inv, c0, c1;
	tie(inv, c0, c1) = x;

	// 0 と 1 の組は全部で c0 * c1 個存在する．
	// そのうち inv 個が転倒していたのだから，転倒していないのは c0 * c1 - inv 個である．
	// ビット反転するとこれらが入れ替わるので，転倒数は c0 * c1 - inv になる．
	inv = c0 * c1 - inv;

	return { inv, c1, c0 };
}
F112 composition112(F112 f, F112 g) {
	return f ^ g;
}
F112 id112() { return false; }
#define XOR_inversion_mlop_monoid S112, op112, e112, F112, mapping112, composition112, id112


//【乗算 作用付き gcd モノイド】
using S113 = ll;
S113 op113(S113 x, S113 y) { return gcd(x, y); }
S113 e113() { return 0; }
using F113 = ll;
S113 mapping113(F113 f, S113 x) { return f * x; }
F113 composition113(F113 f, F113 g) { return f * g; }
F113 id113() { return 1; }
#define Mul_GCD_mlop_monoid S113, op113, e113, F113, mapping113, composition113, id113


//【乗算 作用付き lcm モノイド】
using S114 = ll;
S114 op114(S114 x, S114 y) { return x / gcd(x, y) * y; }
S114 e114() { return 1; }
using F114 = ll;
S114 mapping114(F114 f, S114 x) { return f * x; }
F114 composition114(F114 f, F114 g) { return f * g; }
F114 id114() { return 1; }
#define Mul_LCM_mlop_monoid S114, op114, e114, F114, mapping114, composition114, id114


