#pragma once
#include "header.h"
// ■■■■■ 整列集合 ■■■■■


//【整列集合】
/*
* 整列集合 (S, comp, inf) を表す．
*
* すなわち，集合 S とその上の二項関係 < : S × S → bool で
*	推移律 : ∀a, b, c ∈ S， a < b かつ b < c ⇒ a < c
*	三分律 : ∀a, b ∈ S,     a < b, a = b, a > b のちょうど 1 つが成立する
*	最小元の存在 : ∀A ⊂ S, ∃a ∈ A, ∀x ∈ A,  a <= x
* を満たすものとし，仮想的な最大元を inf で表す．
*/


//【昇順 整列集合】
using S701 = int;
bool comp701(S701 a, S701 b) { return a < b; }
S701 inf701() { return INF; }
#define Ascending_wellordered_set S701, comp701, inf701


//【降順 整列集合】
using S702 = int;
bool comp702(S702 a, S702 b) { return a > b; }
S702 inf702() { return -INF; }
#define Descending_wellordered_set S702, comp702, inf702


