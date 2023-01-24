#pragma once
#include "header.h"
#include "行列.h"
// ■■■■■ モノイド作用付き集合 ■■■■■


//【モノイド作用付き集合】
/*
* モノイド作用付き集合 (S, F, act, comp, id) を表す．
*
* すなわち，S が集合，(F, act, comp, id) がモノイドで，
*	合成   : ∀x ∈ S, ∀f, g ∈ F,	(f g)(x) = f(g(x))
*   恒等射 : ∀x ∈ S,				id(x) = x
* を満たすものとする．
*/


//【行列乗算 左作用付き ベクトル 集合】
int NB01 = 10;
using SB01 = vm;
using FB01 = Matrix<mint>;
SB01 actB01(FB01 f, SB01 x) { return f * x; }
FB01 compB01(FB01 f, FB01 g) { return f * g; }
FB01 idB01() { return Matrix<mint>(NB01); }
#define MatrixLMul_Vector_mset SB01, FB01, actB01, compB01, idB01


//【行列乗算 右作用付き ベクトル 集合】
int NB02 = 10;
using SB02 = vm;
using FB02 = Matrix<mint>;
SB02 actB02(FB02 f, SB02 x) { return f * x; }
FB02 compB02(FB02 f, FB02 g) { return g * f; }
FB02 idB02() { return Matrix<mint>(NB02); }
#define MatrixRMul_Vector_mset SB02, FB02, actB02, compB02, idB02


//【コンパニオン行列乗算 右作用付き ベクトル 集合】
/*
* S ∋ x : ベクトル
* F ∋ f : m * n 行列
*	m = 0 のとき，単位行列を表す．
*	m = 1 のとき，f を 1 行目にもつコンパニオン行列を表す．
*	m >= 2 のとき，f そのものを表す．
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


