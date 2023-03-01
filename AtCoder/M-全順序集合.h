#pragma once
#include "header.h"
// ■■■■■ モノイド作用付き全順序集合 ■■■■■


//【モノイド左作用付き全順序集合】
/*
* モノイド左作用付き全順序集合 (S, leq(≦), inf, F, act, comp, id) を表す．
*
* すなわち，(S, leq, inf) が全順序集合，(S, F, act, comp, id) がモノイド左作用付き集合で，
*	両立性 : ∀x, y ∈ S，∀f ∈ F,  x ≦ y ⇒ f(x) ≦ f(y)
* を満たすものとする．
*/


//【加算 作用付き 昇順 全順序集合】
using S901 = ll;
bool leq901(S901 a, S901 b) { return a <= b; }
S901 inf901() { return INFL; }
using F901 = ll;
S901 act901(F901 f, S901 x) { return f + x; }
F901 comp901(F901 f, F901 g) { return f + g; }
F901 id901() { return 0; }
#define Add_ascending_atoset S901, leq901, inf901, F901, act901, comp901, id901


//【加算 作用付き 降順 全順序集合】
using S902 = ll;
bool leq902(S902 a, S902 b) { return a >= b; }
S902 inf902() { return -INFL; }
using F902 = ll;
S902 act902(F902 f, S902 x) { return f + x; }
F902 comp902(F902 f, F902 g) { return f + g; }
F902 id902() { return 0; }
#define Add_ascending_atoset S902, leq902, inf902, F902, act902, comp902, id902

