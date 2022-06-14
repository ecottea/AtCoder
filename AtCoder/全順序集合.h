#pragma once
#include "header.h"
// ■■■■■ 全順序集合 ■■■■■


//【全順序集合】
/*
* 全順序集合 (S, cmp(≦), inf) を表す．
*
* すなわち，集合 S とその上の二項関係 ≦ : S × S → bool で
*	反対称律 : ∀a, b ∈ S,	a ≦ b かつ a ≧ b ⇒ a = b
*	推移律 : ∀a, b, c ∈ S，	a ≦ b かつ b ≦ c ⇒ a ≦ c
*	完全律 : ∀a, b ∈ S,		a ≦ b または a ≧ b が成立
* を満たすものとし，仮想的な最大元を inf で表す．
*/


//【昇順 全順序集合】
/* verify : https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/all/DSL_3_D */
using S701 = ll;
bool cmp701(S701 a, S701 b) { return a <= b; }
S701 inf701() { return INFL; }
#define Ascending_toset S701, cmp701, inf701


//【降順 全順序集合】
/* verify : https://atcoder.jp/contests/abc228/tasks/abc228_f */
using S702 = ll;
bool cmp702(S702 a, S702 b) { return a >= b; }
S702 inf702() { return -INFL; }
#define Descending_toset S702, cmp702, inf702


