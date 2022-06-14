#pragma once
#include "header.h"
// ■■■■■ 全順序モノイド ■■■■■


//【全順序モノイド】
/*
* 全順序モノイド (S, op, e, cmp(≦), inf) を表す．
*
* すなわち，(S, op, e) がモノイド，(S, cmp, inf) が全順序集合で，
*	両立性 : ∀a, b, c ∈ S， a ≦ b ⇒ a c ≦ b c かつ c a ≦ c b
* を満たすものとする．
*/


//【加算 - 昇順 全順序モノイド】
using S901 = ll;
S901 op901(S901 a, S901 b) { return a + b; }
S901 e901() { return 0; }
bool cmp901(S901 a, S901 b) { return a <= b; }
S901 inf901() { return INFL; }
#define Add_ascending_omonoid S901, op901, e901, cmp901, inf901


//【加算 - 降順 全順序モノイド】
using S902 = ll;
S902 op902(S902 a, S902 b) { return a + b; }
S902 e902() { return 0; }
bool cmp902(S902 a, S902 b) { return a >= b; }
S902 inf902() { return -INFL; }
#define Add_descending_omonoid S902, op902, e902, cmp902, inf902

