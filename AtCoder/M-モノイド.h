#pragma once
#include "header.h"
// ■■■■■ モノイド作用付きモノイド ■■■■■


//【モノイド左作用付きモノイドの定義】
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


//【モノイド → M-モノイド】
/*
* モノイド (S, op, e) から積 op を作用とする M-モノイド (S, op, e, S, op, op, e) が誘導される．
* 
* verify : https://judge.yosupo.jp/problem/vertex_set_path_composite
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


//【2×2行列乗算 左作用付き 2次元ベクトル モノイド】
/* verify : https://yukicoder.me/problems/no/2439 */
using T124 = ll;
using S124 = pair<T124, T124>; // ベクトル (x; y)
using F124 = tuple<T124, T124, T124, T124>; // 行列 (a, b; c, d)
S124 op124(S124 p, S124 q) {
	auto [px, py] = p; // ベクトル (px; py)
	auto [qx, qy] = q; // ベクトル (qx; qy)

	// [px] [qx]   [px + qx]
	// [py].[qy] = [py + qy]
	return { px + qx, py + qy };
}
S124 e124() { return { 0, 0 }; }
S124 act124(F124 f, S124 p) {
	auto [a, b, c, d] = f;
	auto [x, y] = p;

	// [a b] [x]   [a x + b y]
	// [c d].[y] = [c x + d y]
	return { a * x + b * y, c * x + d * y };
}
F124 comp124(F124 f, F124 g) {
	auto [fa, fb, fc, fd] = f;
	auto [ga, gb, gc, gd] = g;

	// [fa fb] [ga gb]   [fa ga + fb gc  fa gb + fb gd]
	// [fc fd].[gc gd] = [fc ga + fd gc  fc gb + fd gd]
	T124 a = fa * ga + fb * gc, b = fa * gb + fb * gd;
	T124 c = fc * ga + fd * gc, d = fc * gb + fd * gd;
	return { a, b, c, d };
}
F124 id124() {
	// [1 0]
	// [0 1]
	return { 1, 0, 0, 1 };
}
#define Matrix2LMul_Vector2_mset S124, op124, e124, F124, act124, comp124, id124


//【chmin 作用付き min モノイド】
/* verify : https://yukicoder.me/problems/no/1868 */
using T115 = ll;
using S115 = T115;
S115 op115(S115 x, S115 y) { return min(x, y); }
S115 e115() { return INFL; }
using F115 = T115;
S115 act115(F115 f, S115 x) { return min(f, x); }
F115 comp115(F115 f, F115 g) { return min(f, g); }
F115 id115() { return INFL; }
#define Chmin_Min_mmonoid S115, op115, e115, F115, act115, comp115, id115


//【chmax 作用付き max モノイド】
/* verify : https://atcoder.jp/contests/abc177/tasks/abc177_f */
using T116 = ll;
using S116 = T116;
S116 op116(S116 x, S116 y) { return max(x, y); }
S116 e116() { return -INFL; }
using F116 = T116;
S116 act116(F116 f, S116 x) { return max(f, x); }
F116 comp116(F116 f, F116 g) { return max(f, g); }
F116 id116() { return -INFL; }
#define Chmax_Max_mmonoid S116, op116, e116, F116, act116, comp116, id116


//【変更 作用付き 左変更 モノイド】
/* verify : https://yukicoder.me/problems/no/2308 */
using S102 = int;
S102 e102() { return INF + 2; } // 使わない値なら何でも OK
S102 op102(S102 x, S102 y) { return x == e102() ? y : x; }
using F102 = int;
F102 id102() { return INF + 1; } // 使わない値なら何でも OK
S102 act102(F102 f, S102 x) { return f == id102() ? x : f; }
F102 comp102(F102 f, F102 g) { return f == id102() ? g : f; }
#define Update_LUpdate_mmonoid S102, op102, e102, F102, act102, comp102, id102


//【変更 作用付き max モノイド】
/* verify : https://atcoder.jp/contests/typical90/tasks/typical90_ac */
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
	auto [vx, cx] = x; // ベクトル (vx, cx)
	auto [vy, cy] = y; // ベクトル (vy, cy)

	// (vx, cx) + (vy, cy) = (vx + vy, cx + cy)
	return { vx + vy, cx + cy };
}
S107 e107() { return { 0, 0 }; }
S107 act107(F107 f, S107 x) {
	auto [v, c] = x; // ベクトル (v, c)
	auto [a, b] = f; // 行列 (a, b; 0, 1)

	// (a, b; 0, 1).(v, c) = (a v + b c, c)
	return { a * v + b * c, c };
}
F107 comp107(F107 f, F107 g) {
	auto [a, b] = f; // 行列 (a, b; 0, 1)
	auto [c, d] = g; // 行列 (c, d; 0, 1)

	// (a, b; 0, 1).(c, d; 0, 1) = (a c, a d + b; 0, 1)
	return { a * c, a * d + b };
}
F107 id107() { return { 1, 0 }; }
#define Affine_Sum_mmonoid S107, op107, e107, F107, act107, comp107, id107


//【加算 作用付き 総和 モノイド】
/*
* S ∋ x = {v, c} : c 個の元の和で値 v をとっていることを表す．
* F ∋ f = b : 一次関数 f(x) = 1 x + b を表す．
* x op y : cx + cy 個の元の和で値 vx + vy をとっている状態にする．
* f act x : c 個の元の和で値 v + c f をとっている状態にする．
* f comp g : 合成した一次関数 f o g を返す．
*/
using T108 = ll;
using S108 = pair<T108, T108>; // ベクトル (v, c)
using F108 = T108; // 行列 (1, f; 0, 1)
S108 op108(S108 x, S108 y) {
	auto [vx, cx] = x; // ベクトル (vx, cx)
	auto [vy, cy] = y; // ベクトル (vy, cy)

	// (vx, cx) + (vy, cy) = (vx + vy, cx + cy)
	return { vx + vy, cx + cy };
}
S108 e108() { return { 0, 0 }; }
S108 act108(F108 f, S108 x) {
	auto [v, c] = x; // ベクトル (v, c)

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
* F ∋ f = b : 零次関数 f(x) = 0 x + b を表す．
* x op y : cx + cy 個の元の和で値 vx + vy をとっている状態にする．
* f act x : c 個の元の和で値 c f をとっている状態にする．
* f comp g : 合成した零次関数 f o g を返す．
*/
// verify : https://atcoder.jp/contests/abc237/tasks/abc237_g
using T109 = ll;
using S109 = pair<T109, T109>; // ベクトル (v, c)
using F109 = T109; // 行列 (0, f; 0, 1)
S109 op109(S109 x, S109 y) {
	auto [vx, cx] = x; // ベクトル (vx, cx)
	auto [vy, cy] = y; // ベクトル (vy, cy)

	// (vx, cx) + (vy, cy) = (vx + vy, cx + cy)
	return { vx + vy, cx + cy };
}
S109 e109() { return { 0, 0 }; }
F109 id109() { return INFL + 1; } // 使わない値なら何でも OK
S109 act109(F109 f, S109 x) {
	if (f == id109()) return x;

	auto [v, c] = x; // ベクトル (v, c)

	// (0, f; 0, 1).(v, c) = (f c, c)
	return { f * c, c };
}
F109 comp109(F109 f, F109 g) {
	if (f == id109()) return g;

	// (0, f; 0, 1).(0, g; 0, 1) = (0, f; 0, 1)
	return f;
}
#define Update_Sum_mmonoid S109, op109, e109, F109, act109, comp109, id109


//【加算 作用付き 平方和 モノイド】
/*
* S ∋ x = {s2, s, c} : 区間の平方和が s2，和が s，要素数が c であることを表す．
* F ∋ f : f を加算する作用を表す．
* x op y : x, y に対応する区間を結合した区間を表す．
* f act x : 区間 x に一斉に f を加算した区間を表す．
* f comp g : (f + g) を加算する作用を表す．
*/
// verify : https://yukicoder.me/problems/no/1099
using T120 = ll;
using S120 = tuple<T120, T120, T120>; // ベクトル (s2, s, c)
using F120 = ll; // 行列 (1, 2f, f^2; 0, 1, f; 0, 0, 1)
S120 op120(S120 x, S120 y) {
	auto [s2x, sx, cx] = x;
	auto [s2y, sy, cy] = y;

	return { s2x + s2y, sx + sy, cx + cy };
}
S120 e120() { return { 0, 0, 0 }; }
S120 act120(F120 f, S120 x) {
	auto [s2x, sx, cx] = x;

	// (1, 2f, f^2; 0, 1, f; 0, 0, 1).(s2x, sx, cx)
	return { s2x + 2 * f * sx + f * f * cx, sx + f * cx, cx };
}
F120 comp120(F120 f, F120 g) {
	return f + g;
}
F120 id120() { return 0; }
#define Add_Sqsum_mmonoid S120, op120, e120, F120, act120, comp120, id120


//【AND 作用付き XOR モノイド】
using S110 = int;
S110 op110(S110 x, S110 y) { return x ^ y; }
S110 e110() { return 0; }
using F110 = int;
S110 act110(F110 f, S110 x) { return f & x; }
F110 comp110(F110 f, F110 g) { return f & g; }
F110 id110() { return ~0; }
#define AND_XOR_mmonoid S110, op110, e110, F110, act110, comp110, id110


//【混合トロピカルアフィン 作用付き min,max モノイド】
/*
* S ∋ x = {p, q} :
*	p : 区間の min
*	q : 区間の max
* F ∋ f = {a, b, c} : 混合トロピカル一次関数 f(x) = max(min(a + x, b), c) を表す．
*	a : バイアス（加算する）
*	b : 上限（min をとる）
*	c : 下限（max をとる）
* x op y : 区間 x, y を結合する．
* f act x : 区間 x の元全てに f を作用させる．
* f comp g : 関数の合成 f o g
*/
// verify : https://yukicoder.me/problems/no/2439
using T128 = ll;
using S128 = pair<T128, T128>;			// {min, max}
using F128 = tuple<T128, T128, T128>;	// {add, min, max}
S128 op128(S128 x, S128 y) {
	auto [px, qx] = x;	// {x_min, x_max}
	auto [py, qy] = x;	// {y_min, y_max}

	T128 P = min(px, py);
	T128 Q = max(px, py);
	return S128{ P, Q };
}
S128 e128() { return { INFL, -INFL }; }
S128 act128(F128 f, S128 x) {
	auto [a, b, c] = f;	// f(x) = max(min(a + x, b), c)
	auto [p, q] = x;	// {min, max}

	T128 P = max(min(a + p, b), c);
	T128 Q = max(min(a + q, b), c);
	return S128{ P, Q };
}
F128 comp128(F128 f, F128 g) {
	auto [fa, fb, fc] = f; // f(x) = max(min(fa + x, fb), fc)
	auto [ga, gb, gc] = g; // g(x) = max(min(ga + x, gb), gc)

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

	T128 A = fa + ga;
	T128 B = min(fa + gb, fb);
	T128 C = max(min(fa + gc, fb), fc);
	return F128{ A, B, C };
}
F128 id128() { return F128{ 0, INFL, -INFL }; } // e(x) = max(min(a + 0, ∞), -∞)
#define MixedTropicalAffine_MinMax_mmonoid S128, op128, e128, F128, act128, comp128, id128


//【bitアフィン 作用付き XOR モノイド】
/*
* S ∋ x = {v, c} :
*	v : 区間の総 XOR
*	c : 区間の幅が偶数なら 0，奇数なら ~0
* F ∋ f = {a, b} : 一次関数 f(x) = (a AND x) XOR b を表す．
*	{a, b} = { c,  0} : f(x) = c AND x
*	{a, b} = {~c,  c} : f(x) = c OR x
*	{a, b} = {~0,  c} : f(x) = x XOR c
*	{a, b} = {~0, ~0} : f(x) = NOT x
*	{a, b} = {~c, ~0} : f(x) = x IMPLY c
* x op y : 区間 x, y を結合する．
* f act x : 区間 x の元全てに対して f を作用させる．
* f comp g : 合成した一次関数 f o g を返す．
*/
// verify : https://yukicoder.me/problems/no/2498
using T127 = int;
using S127 = pair<T127, T127>; // ベクトル (v, c)
using F127 = pair<T127, T127>; // 行列 (a, b; 0, 1)（1 := ~0）
S127 op127(S127 x, S127 y) {
	auto [vx, cx] = x; // ベクトル (vx, cx)
	auto [vy, cy] = y; // ベクトル (vy, cy)

	// [vx] + [vy] = [vx + vy]
	// [cx]   [cy]   [cx + cy]
	return { vx ^ vy, cx ^ cy };
}
S127 e127() { return { 0, 0 }; }
S127 act127(F127 f, S127 x) {
	auto [v, c] = x; // ベクトル (v, c)
	auto [a, b] = f; // 行列 (a, b; 0, 1)

	// [a, b] [v] = [a v + b c]
	// [0, 1] [c]   [    c    ]
	return { (a & v) ^ (b & c), c };
}
F127 comp127(F127 f, F127 g) {
	auto [a, b] = f; // 行列 (a, b; 0, 1)
	auto [c, d] = g; // 行列 (c, d; 0, 1)

	// [a, b] [c, d] = [a c , a d + b ]
	// [0, 1] [0, 1]   [ 0  ,    1    ]
	return { (a & c), (a & d) ^ b };
}
F127 id127() { return { ~0, 0 }; }
#define BitAffine_XOR_mmonoid S127, op127, e127, F127, act127, comp127, id127


//【乗算 作用付き GCD モノイド】
using S113 = ll;
S113 op113(S113 x, S113 y) { return gcd(x, y); }
S113 e113() { return 0; }
using F113 = ll;
S113 act113(F113 f, S113 x) { return f * x; }
F113 comp113(F113 f, F113 g) { return f * g; } // オーバーフロー注意
F113 id113() { return 1; }
#define Mul_GCD_mmonoid S113, op113, e113, F113, act113, comp113, id113


//【乗算 作用付き LCM モノイド】
using S114 = ll;
S114 op114(S114 x, S114 y) { return lcm(x, y); }
S114 e114() { return 1; }
using F114 = ll;
S114 act114(F114 f, S114 x) { return f * x; }
F114 comp114(F114 f, F114 g) { return f * g; } // オーバーフロー注意
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


//【線形変更 作用付き 総和 モノイド】
/*
* S ∋ x = {v, i, 1} : 添字 i の元が値 v をとっていることを表す．
* F ∋ f = {a, b} : 一次関数 f(x) = a x + b を表す．
* x op y : 値 vx + vy をとっている状態にする（添字とかは壊れる）
* f act x : v を値 a i + b に変更．
* f comp g : (af + ag) i + (bf + bg) に変更する作用にする．
*/
// verify : https://atcoder.jp/contests/DEGwer2023/tasks/1202Contest_e
using T121 = ll;
using S121 = tuple<T121, T121, T121>; // ベクトル (v, i, 1)
using F121 = pair<T121, T121>; // 行列 (0, a, b; 0, 1, 0; 0, 0, 1)
S121 op121(S121 x, S121 y) {
	auto [vx, sx, cx] = x; // ベクトル (vx, sx, cx)
	auto [vy, sy, cy] = y; // ベクトル (vy, sy, cy)

	// (vx, sx, cx) + (vy, sy, cy) = (vx + vy, sx + sy, cx + cy)
	return { vx + vy, sx + sy, cx + cy };
}
S121 e121() { return { 0, 0, 0 }; }
F121 id121() { return { INFL + 1, INFL + 1 }; } // 使わない値なら何でも OK
S121 act121(F121 f, S121 x) {
	if (f == id121()) return x;

	auto [v, s, c] = x; // ベクトル (v, s, c)
	auto [a, b] = f; // 行列 (0, a, b; 0, 1, 0; 0, 0, 1)

	// (0, a, b; 0, 1, 0; 0, 0, 1).(v, s, c) = (a s + b c, s, c)
	return { a * s + b * c, s, c };
}
F121 comp121(F121 f, F121 g) {
	if (f == id121()) return g;
	return f;
}
#define LinearUpdate_Sum_mmonoid S121, op121, e121, F121, act121, comp121, id121


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


//【ビット列上 XOR 作用付き 最長連 モノイド】
/*
* S ∋ x = {l0, l1, m0, m1, r0, r1, len} : x に対応する区間についての以下の値を表す：
*	l0[l1] : 左端要素を含む 0[1] の連の長さ
*	m0[m1] : 区間内の 0[1] の連の最大長
*	r0[r1] : 左端要素を含む 0[1] の連の長さ
*	len : 区間の長さ
* F ∋ f : f との XOR をとる作用を表す
* x op y : 列 x, y を連結した列
* f act x : 列 x の各要素と f との XOR をとった列
* f comp g : f XOR g
*/
// verify : https://atcoder.jp/contests/abc322/tasks/abc322_f
using S126 = tuple<int, int, int, int, int, int, int>; // (l0, l1, m0, m1, r0, r1, len)
using F126 = bool;
S126 op126(S126 x, S126 y) {
	auto [l0x, l1x, m0x, m1x, r0x, r1x, lenx] = x;
	auto [l0y, l1y, m0y, m1y, r0y, r1y, leny] = y;

	int l0 = l0x == lenx ? l0x + l0y : l0x;
	int l1 = l1x == lenx ? l1x + l1y : l1x;
	int m0 = max({ m0x, m0y, r0x + l0y });
	int m1 = max({ m1x, m1y, r1x + l1y });
	int r0 = r0y == leny ? r0x + r0y : r0y;
	int r1 = r1y == leny ? r1x + r1y : r1y;
	int len = lenx + leny;

	return { l0, l1, m0, m1, r0, r1, len };
}
S126 e126() { return { 0, 0, 0, 0, 0, 0, 0 }; }
S126 act126(F126 f, S126 x) {
	if (!f) return x;
	auto [l0, l1, m0, m1, r0, r1, len] = x;
	return { l1, l0, m1, m0, r1, r0, len };
}
F126 comp126(F126 f, F126 g) {
	return f ^ g;
}
F126 id126() { return false; }
#define XOR_RunLength_mmonoid S126, op126, e126, F126, act126, comp126, id126


//【加算 作用付き 最小元の個数 モノイド】
/*
* S ∋ x = {v, c} : 最小値 v をもつ要素が c 個あることを表す．
* F ∋ f : f 加算する作用を表す．
*/
// verify : https://judge.yosupo.jp/problem/area_of_union_of_rectangles
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


//【AND,OR 作用付き AND モノイド】
/*
* S ∋ x
* F ∋ f = {a, b} : 関数 f(x) = (a OR x) AND b を表す．
* x op y : x AND y
* f act x : 関数への代入 f(x)
* f comp g : 合成関数 f o g を返す．
*/
// verify : https://www.codechef.com/problems/CHEFDIVISION
using S122 = ll; // ベクトル (x, e)
using F122 = pair<S122, S122>; // 行列 (a, b; o, e)
S122 op122(S122 x, S122 y) { return x & y; }
S122 e122() { return ~0; }
S122 act122(F122 f, S122 x) {
	auto [a, b] = f; // 行列 (a, b; o, e)

	// (a, b; o, e).(x, e) = (a x + b, e)
	return (a | x) & b;
}
F122 comp122(F122 f, F122 g) {
	auto [a, b] = f; // 行列 (a, b; o, e)
	auto [c, d] = g; // 行列 (c, d; o, e)

	// (a, b; o, e).(c, d; o, e) = (a c, a d + b; o, e)
	return { a | c, (a | d) & b };
}
F122 id122() { return { 0, ~0 }; }
#define OrAnd_And_mmonoid S122, op122, e122, F122, act122, comp122, id122


//【写像 作用付き ベクトル総和 モノイド】（参照渡ししていないので遅い）
/*
* S ∋ x[0..n) : ベクトル
* F ∋ f[0..n) : 写像 i → f[i]
* x op y : x + y
* f act x : ∀i∈[0..n), y[f[i]] += x[i] とした y[0..n)
* f comp g : 合成写像 h = f o g（h[i] = f[g[i]]）
*/
// verify : https://atcoder.jp/contests/abc310/tasks/abc310_g
int N123 = 10;
using T123 = mint;
using S123 = vector<T123>; // ベクトル x[0..n)
using F123 = vi; // 写像 i → f[i]
S123 op123(S123 x, S123 y) {
	rep(i, N123) x[i] += y[i];
	return x;
}
S123 e123() {
	return S123(N123, 0);
}
S123 act123(F123 f, S123 x) {
	S123 y(N123);
	rep(i, N123) y[f[i]] += x[i];
	return y;
}
F123 comp123(F123 f, F123 g) {
	F123 h(N123);
	rep(i, N123) h[i] = f[g[i]];
	return h;
}
F123 id123() {
	F123 id(N123);
	iota(all(id), 0);
	return id;
}
#define Map_VectorSum_mmonoid S123, op123, e123, F123, act123, comp123, id123


