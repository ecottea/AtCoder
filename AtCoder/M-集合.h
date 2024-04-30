#pragma once
#include "header.h"
#include "行列.h"
#include "bit行列.h"
#include "FPS(bit).h"
#include "ヒープ.h"
// ■■■■■ モノイド作用付き集合 ■■■■■


//【モノイド作用付き集合の定義】
/*
* モノイド作用付き集合 (S, F, act, comp, id) を表す．
*
* すなわち，S が集合，(F, comp, id) がモノイド，act が作用で，
*	合成   : ∀x ∈ S, ∀f, g ∈ F,	(f g)(x) = f(g(x))
*   恒等射 : ∀x ∈ S,				id(x) = x
* を満たすものとする．
*/


//【変更 左作用付き 整数 集合】
/* verify : https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/all/DSL_2_D */
using SB08 = int;
using FB08 = int;
FB08 idB08() { return INF + 1; } // 使わない値なら何でも OK
SB08 actB08(FB08 f, SB08 x) { return f == idB08() ? x : f; }
FB08 compB08(FB08 f, FB08 g) { return f == idB08() ? g : f; }
#define LUpdate_Integer_mset SB08, FB08, actB08, compB08, idB08


//【加算 作用付き 整数 集合】
/* verify : https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/all/DSL_2_E */
using SB09 = ll;
using FB09 = ll;
SB09 actB09(FB09 f, SB09 x) { return f + x; }
FB09 compB09(FB09 f, FB09 g) { return f + g; }
FB09 idB09() { return 0; }
#define Add_Integer_mset SB09, FB09, actB09, compB09, idB09


//【乗算 作用付き 整数 集合】
/* verify : https://judge.yosupo.jp/problem/discrete_logarithm_mod */
using SB04 = mint;
using FB04 = mint;
SB04 actB04(FB04 f, SB04 x) { return f * x; }
FB04 compB04(FB04 f, FB04 g) { return f * g; }
FB04 idB04() { return 1; }
#define Mul_Integer_mset SB04, FB04, actB04, compB04, idB04


//【chmin 作用付き 整数 集合】
using SB11 = ll;
using FB11 = ll;
SB11 actB11(FB11 f, SB11 x) { return min(f, x); }
FB11 compB11(FB11 f, FB11 g) { return min(f, g); }
FB11 idB11() { return INFL; }
#define Chmin_Integer_mset SB11, FB11, actB11, compB11, idB11


//【chmax 作用付き 整数 集合】
/* verify : https://atcoder.jp/contests/joi2019yo/tasks/joi2019_yo_e */
using SB10 = ll;
using FB10 = ll;
SB10 actB10(FB10 f, SB10 x) { return max(f, x); }
FB10 compB10(FB10 f, FB10 g) { return max(f, g); }
FB10 idB10() { return -INFL; }
#define Chmax_Integer_mset SB10, FB10, actB10, compB10, idB10


//【2×2行列乗算 左作用付き 2次元ベクトル 集合】
/* verify : https://yukicoder.me/problems/no/1648 */
using TB12 = mint;
using SB12 = pair<TB12, TB12>; // ベクトル (x; y)
using FB12 = tuple<TB12, TB12, TB12, TB12>; // 行列 (a, b; c, d)
SB12 actB12(FB12 f, SB12 p) {
	auto [a, b, c, d] = f;
	auto [x, y] = p;

	// [a b] [x]   [a x + b y]
	// [c d].[y] = [c x + d y]
	return { a * x + b * y, c * x + d * y };
}
FB12 compB12(FB12 f, FB12 g) {
	auto [fa, fb, fc, fd] = f;
	auto [ga, gb, gc, gd] = g;

	// [fa fb] [ga gb]   [fa ga + fb gc  fa gb + fb gd]
	// [fc fd].[gc gd] = [fc ga + fd gc  fc gb + fd gd]
	TB12 a = fa * ga + fb * gc, b = fa * gb + fb * gd;
	TB12 c = fc * ga + fd * gc, d = fc * gb + fd * gd;
	return { a, b, c, d };
}
FB12 idB12() {
	// [1 0]
	// [0 1]
	return { 1, 0, 0, 1 };
}
#define Matrix2LMul_Vector2_mset SB12, FB12, actB12, compB12, idB12


//【行列乗算 左作用付き ベクトル 集合】
/* verify : https://yukicoder.me/problems/no/1000 */
constexpr int NB01 = 3;
using TB01 = ll;
using SB01 = array<TB01, NB01>;
using FB01 = Fixed_matrix<TB01, NB01>;
SB01 actB01(FB01 f, SB01 x) { return f * x; }
FB01 compB01(FB01 f, FB01 g) { return f * g; }
FB01 idB01() { return Fixed_matrix<TB01, NB01>(true); }
#define MatrixLMul_Vector_mset SB01, FB01, actB01, compB01, idB01


//【行列乗算 右作用付き ベクトル 集合】
int NB02 = 10;
using SB02 = vm;
using FB02 = Matrix<mint>;
SB02 actB02(FB02 f, SB02 x) { return f * x; }
FB02 compB02(FB02 f, FB02 g) { return g * f; }
FB02 idB02() { return Matrix<mint>(NB02); }
#define MatrixRMul_Vector_mset SB02, FB02, actB02, compB02, idB02


//【ビット行列乗算 左作用付き ビットベクトル 集合】
/* verify : https://atcoder.jp/contests/utpc2014/tasks/utpc2014_k */
constexpr int NB05 = 36;
using SB05 = bitset<NB05>;
using FB05 = Bit_matrix<NB05>;
SB05 actB05(FB05 f, SB05 x) { return f * x; }
FB05 compB05(FB05 f, FB05 g) { return f * g; }
FB05 idB05() { return Bit_matrix<NB05>(NB05); }
#define BitMatrixLMul_BitVector_mset SB05, FB05, actB05, compB05, idB05


//【コンパニオン行列乗算 右作用付き ベクトル 集合】
/*
* S ∋ x : ベクトル
* F ∋ f : m×n 行列
*	m = 0 のとき，単位行列を表す．
*	m = 1 のとき，f を 1 行目にもつコンパニオン行列を表す．
*	m ≧ 2 のとき，f そのものを表す．
*/
// verify : https://atcoder.jp/contests/arc027/tasks/arc027_4
int NB03 = 10;
using SB03 = vm;
using FB03 = Matrix<mint>;
SB03 actB03(FB03 f, SB03 x) {
	if (f.m == 0) return x;
	if (f.m == 1) {
		x.insert(x.begin(), (f * x)[0]);
		x.pop_back();
		return x;
	}
	return f * x;
}
FB03 compB03(FB03 f, FB03 g) {
	if (f.m == 0) return g;
	if (g.m == 0) return f;
	if (f.m == 1) {
		f.m = NB03;
		f.v.resize(NB03);
		repi(i, 1, NB03 - 1) {
			f[i].resize(NB03);
			f[i][i - 1] = 1;
		}
	}
	if (g.m == 1) {
		g.m = NB03;
		g.v.resize(NB03);
		repi(i, 1, NB03 - 1) {
			g[i].resize(NB03);
			g[i][i - 1] = 1;
		}
	}
	return g * f;
}
FB03 idB03() { return Matrix<mint>(); }
#define CompanionMatrixRMul_Vector_mset SB03, FB03, actB03, compB03, idB03


//【行列乗算 左作用付き 行列 集合】
/* verify : https://yukicoder.me/problems/no/950 */
constexpr int NB14 = 2;
using TB14 = mint;
using SB14 = Fixed_matrix<TB14, NB14>;
using FB14 = SB14;
SB14 actB14(FB14 f, SB14 x) { return f * x; }
FB14 compB14(FB14 f, FB14 g) { return f * g; }
FB14 idB14() { return Fixed_matrix<TB14, NB14>(1); }
#define MatrixLMul_Matrix_mset SB14, FB14, actB14, compB14, idB14


//【剰余環上 ビット多項式乗算 作用付き ビット多項式 集合】
/* verify : https://atcoder.jp/contests/utpc2014/tasks/utpc2014_k */
constexpr int NB06 = 80;
using SB06 = BFPS<NB06>;
using FB06 = BFPS<NB06>;
BFPS<NB06> MOD;
SB06 actB06(FB06 f, SB06 x) { return (f * x).reminder(MOD); }
FB06 compB06(FB06 f, FB06 g) { return (f * g).reminder(MOD); }
FB06 idB06() { return BFPS<NB06>(1); }
#define BFPSMul_BFPS_mset SB06, FB06, actB06, compB06, idB06


//【アフィン 左作用付き 整数 集合】
/*
* S ∋ x : 値が x であることを表す．
* F ∋ f = {a, b} : 一次関数 f(x) = a x + b を表す．
* f act x : a x + b を返す．
* f comp g : 合成した一次関数 f o g を返す．
*/
// verify : https://judge.yosupo.jp/problem/range_affine_point_get
using TB07 = mint;
using SB07 = TB07;
using FB07 = pair<TB07, TB07>; // 行列 (a, b; 0, 1)
SB07 actB07(FB07 f, SB07 x) {
	auto [a, b] = f; // 行列 (a, b; 0, 1)

	// (a, b; 0, 1).(x; 1) = (a x + b; 1)
	return a * x + b;
}
FB07 compB07(FB07 f, FB07 g) {
	auto [a, b] = f; // 行列 (a, b; 0, 1)
	auto [c, d] = g; // 行列 (c, d; 0, 1)

	// (a, b; 0, 1).(c, d; 0, 1) = (a c, a d + b; 0, 1)
	return { a * c, a * d + b };
}
FB07 idB07() { return { 1, 0 }; }
#define LAffine_Integer_mset SB07, FB07, actB07, compB07, idB07


//【整除算 左作用付き 整数 集合】
/*
* S ∋ x : 値が x であることを表す．
* F ∋ f = {a, b} : 関数 f(x) = floor((x + a) / b) を表す．
* f act x : f(x) を返す．
* f comp g : 合成関数 f o g を返す．
*/
using TB13 = ll;
using SB13 = TB13;
using FB13 = pair<TB13, TB13>; // {a, b} : x → floor((x + a) / b)
SB13 actB13(FB13 f, SB13 x) {
	auto [a, b] = f;

	return (a + x) / b;
}
FB13 compB13(FB13 f, FB13 g) {
	auto [c, d] = f;
	auto [a, b] = g;

	// ((x + a) / b + c) / d
	// = (x + a + b c) / b / d
	// = (x + (a + b c) / (b d)
	return { a + b * c, b * d }; // オーバーフロー注意
}
FB13 idB13() { return { 0, 1 }; }
#define Div_Integer_mset SB13, FB13, actB13, compB13, idB13


//【S_3 作用付き {0,1,2} 集合】
/* verify : https://atcoder.jp/contests/agc044/tasks/agc044_c */
using SB15 = int; // {0, 1, 2}
typedef enum { S0, S012, S021, S12, S02, S01 } FB15; // S_3
constexpr int ACTB15[6][3] = {
	{0, 1, 2},
	{1, 2, 0},
	{2, 0, 1},
	{0, 2, 1},
	{2, 1, 0},
	{1, 0, 2}
};
constexpr FB15 COMPB15[6][6] = {
	{ S0, S012, S021, S12, S02, S01 },
	{ S012, S021, S0, S01, S12, S02 },
	{ S021, S0, S012, S02, S01, S12 },
	{ S12, S02, S01, S0, S012, S021 },
	{ S02, S01, S12, S021, S0, S012 },
	{ S01, S12, S02, S012, S021, S0 }
};
SB15 actB15(FB15 f, SB15 x) { return ACTB15[f][x]; }
FB15 compB15(FB15 f, FB15 g) { return COMPB15[f][g]; }
FB15 idB15() { return S0; }
#define S3_012_mset SB15, FB15, actB15, compB15, idB15


//【可換モノイド作用付き生成集合の定義】
/*
* 可換モノイド作用付き生成集合 (S, F, act, id, E, comp) を表す．
*
* すなわち，S が集合，(F, comp, id) が E で生成されたモノイド，act が作用で，
*	合成   : ∀x ∈ S, ∀f, g ∈ F,	(f g)(x) = f(g(x))
*	可換   : ∀f, g ∈ F,				f o g = g o f
*   恒等射 : ∀x ∈ S,				id(x) = x
* を満たすものとする．
*/


//【要素の挿入&削除 作用付き 最大値 集合】
/* verify : https://atcoder.jp/contests/abc342/tasks/abc342_g */
using S201 = int;
using F201 = Eraseable_heap<int>;
using E201 = pii; // (val, tp) (tp=1:挿入, -1:削除)
void act201(F201& f, S201& x) {
	if (f.empty()) return;
	chmax(x, f.top());
}
void comp201(E201 e, F201& f) {
	auto [val, tp] = e;
	if (tp == 1) f.push(val);
	else f.erase(val);
}
F201 id201() { return F201(); }
#define SetInsErs_MaxInt_mset S201, F201, act201, id201, E201, comp201 


