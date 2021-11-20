#pragma once
#include "header.h"
// ■■■■■ モノイド作用付きモノイド ■■■■■


//【モノイド左作用付きモノイド】
/*
* モノイド左作用付きモノイド (S, op, e, F, mapping, composition, id) を表す．
* （op, mapping, composition は * をオーバーロードする）
*
* すなわち，(S, op, e), (F, composition, id) がそれぞれモノイドであり，
* F の S への左作用 * : F × S → S が
*	準同型 : ∀x, y ∈ S，∀f ∈ F,  f(x y) = f(x) f(y)
*	合成   : ∀x ∈ S, ∀f, g ∈ F,  (f g)(x) = f(g(x))
*   恒等射 : ∀x ∈ S,               id(x) = x
* を満たすものとする．
*/
template <class S_, S_(*op)(S_, S_), S_(*e_)(),
	class F_, S_(*mapping)(F_, S_), F_(*composition)(F_, F_), F_(*id_)()>
struct MLop_Monoid {
	struct S {
		S_ v;

		// 単位元
		static S_ e() { return e_(); }

		// コンストラクタ
		S() : v(e()) {}
		S(S_ a) : v(a) {}

		// 比較
		bool operator==(const S& a) const { return v == a.v; }
		bool operator!=(const S& a) const { return v != a.v; }

		// 積
		S operator*(const S& a) const {
			if (v == e()) return a;
			if (a.v == e()) return *this;
			return op(v, a.v);
		}

		// 入出力
		friend istream& operator>>(istream& is, S& a) { is >> a.v; return is; }
		friend ostream& operator<<(ostream& os, const S& a) {
#ifdef _MSC_VER
			if (a.v == e()) return os << "e";
#endif
			return os << a.v;
		}
	};

	struct F {
		F_ v;

		// 単位元
		static F_ id() { return id_(); }

		// コンストラクタ
		F() : v(id()) {}
		F(F_ a) : v(a) {}

		// 比較
		bool operator==(const F& a) const { return v == a.v; }
		bool operator!=(const F& a) const { return v != a.v; }

		// 積
		F operator*(const F& a) const {
			if (v == id()) return a;
			if (a.v == id()) return *this;
			return composition(v, a.v);
		}

		// 入出力
		friend istream& operator>>(istream& is, F& a) { is >> a.v; return is; }
		friend ostream& operator<<(ostream& os, const F& a) {
#ifdef _MSC_VER
			if (a.v == id()) return os << "id";
#endif
			return os << a.v;
		}
	};

	// 単位元
	static S e() { return e_(); }
	static F id() { return id_(); }

	// 左作用
	friend S operator*(const F& f, const S& x) {
		if (f.v == f.id()) return x;
		return mapping(f.v, x.v);
	}
};


//【乗算 作用付き 加算 モノイド】
using S1 = mint;
S1 op(S1 x, S1 y) { return x + y; }
S1 e1() { return 0; }
using F1 = mint;
S1 mapping(F1 f, S1 x) { return f * x; }
F1 composition(F1 f, F1 g) { return f * g; }
F1 id1() { return 1; }
using T = MLop_Monoid<S1, op, e1, F1, mapping, composition, id1>;


//【変更 作用付き 左変更 モノイド】
using S2 = int;
S2 op(S2 x, S2 y) { return x; }
S2 e2() { return INF; } // 使わない値なら何でも OK
using F2 = int;
S2 mapping(F2 f, S2 x) { return f; }
F2 composition(F2 f, F2 g) { return f; }
F2 id2() { return INF; } // 使わない値なら何でも OK
using T = MLop_Monoid<S2, op, e2, F2, mapping, composition, id2>;


//【変更 作用付き max モノイド】
using S3 = int;
S3 op(S3 x, S3 y) { return max(x, y); }
S3 e3() { return -INF; }
using F3 = int;
S3 mapping(F3 f, S3 x) { return f; }
F3 composition(F3 f, F3 g) { return f; }
F3 id3() { return INF; } // 使わない値なら何でも OK
using T = MLop_Monoid<S3, op, e3, F3, mapping, composition, id3>;


//【変更 作用付き min モノイド】
using S4 = int;
S4 op(S4 x, S4 y) { return min(x, y); }
S4 e4() { return INF; }
using F4 = int;
S4 mapping(F4 f, S4 x) { return f; }
F4 composition(F4 f, F4 g) { return f; }
F4 id4() { return INF; } // 使わない値なら何でも OK
using T = MLop_Monoid<S4, op, e4, F4, mapping, composition, id4>;


//【加算 作用付き max モノイド】
using S5 = int;
S5 op(S5 x, S5 y) { return max(x, y); }
S5 e5() { return -INF; }
using F5 = int;
S5 mapping(F5 f, S5 x) { return f + x; }
F5 composition(F5 f, F5 g) { return f + g; }
F5 id5() { return 0; }
using T = MLop_Monoid<S5, op, e5, F5, mapping, composition, id5>;


//【加算 作用付き min モノイド】
using S6 = int;
S6 op(S6 x, S6 y) { return min(x, y); }
S6 e6() { return INF; }
using F6 = int;
S6 mapping(F6 f, S6 x) { return f + x; }
F6 composition(F6 f, F6 g) { return f + g; }
F6 id6() { return 0; }
using T = MLop_Monoid<S6, op, e6, F6, mapping, composition, id6>;


//【アフィン変換 作用付き 加算 モノイド】
/*
* S ∋ x = {v, c} : c 個の元の和で値 v をとっていることを表す．
* F ∋ f = {a, b} : 一次関数 f(x) = a x + b を表す．
* x op y : cx + cy 個の元の和で値 vx + vy をとっている状態にする．
* f mapping x : c 個の元の和で値 f(v) をとっている状態にする．
* f composition g : 合成した一次関数 f o g を返す．
*/
using S7 = pair<mint, mint>; // 斉次ベクトル (v, c)
using F7 = pair<mint, mint>; // 斉次行列 (a, b; 0, 1)
S7 op(S7 x, S7 y) {
	mint vx, vy, cx, cy;
	tie(vx, cx) = x; // ベクトル (vx, cx)
	tie(vy, cy) = y; // ベクトル (vy, cy)

	// (vx, cx) + (vy, cy) = (vx + vy, cx + cy)
	return { vx + vy, cx + cy };
}
S7 e7() { return { 0, 0 }; }
S7 mapping(F7 f, S7 x) {
	mint v, c, a, b;
	tie(v, c) = x; // ベクトル (v, c)
	tie(a, b) = f; // 行列 (a, b; 0, 1)

	// (a, b; 0, 1).(v, c) = (a v + b c, c)
	return { a * v + b * c, c };
}
F7 composition(F7 f, F7 g) {
	mint a, b, c, d;
	tie(a, b) = f; // 行列 (a, b; 0, 1)
	tie(c, d) = g; // 行列 (c, d; 0, 1)

	// (a, b; 0, 1).(c, d; 0, 1) = (a c, a d + b; 0, 1)
	return { a * c, a * d + b };
}
F7 id7() { return { 1, 0 }; }
using T = MLop_Monoid<S7, op, e7, F7, mapping, composition, id7>;


//【加算 作用付き 加算 モノイド】
/*
* S ∋ x = {v, c} : c 個の元の和で値 v をとっていることを表す．
* F ∋ f : 一次関数 x ← 1 x + f を表す．
* x op y : cx + cy 個の元の和で値 vx + vy をとっている状態にする．
* f mapping x : c 個の元の和で値 v + c f をとっている状態にする．
* f composition g : 合成した一次関数 f o g を返す．
*/
using S8 = pair<ll, ll>; // 斉次ベクトル (v, c)
using F8 = ll; // 斉次行列 (1, f; 0, 1)
S8 op(S8 x, S8 y) {
	ll vx, vy, cx, cy;
	tie(vx, cx) = x; // ベクトル (vx, cx)
	tie(vy, cy) = y; // ベクトル (vy, cy)

	// (vx, cx) + (vy, cy) = (vx + vy, cx + cy)
	return { vx + vy, cx + cy };
}
S8 e8() { return { 0, 0 }; }
S8 mapping(F8 f, S8 x) {
	ll v, c;
	tie(v, c) = x; // ベクトル (v, c)

	// (1, f; 0, 1).(v, c) = (v + f c, c)
	return { v + f * c, c };
}
F8 composition(F8 f, F8 g) {
	// (1, f; 0, 1).(1, g; 0, 1) = (1, f + g; 0, 1)
	return f + g;
}
F8 id8() { return 0; }
using T = MLop_Monoid<S8, op, e8, F8, mapping, composition, id8>;


//【変更 作用付き 加算 モノイド】
/*
* S ∋ x = {v, c} : c 個の元の和で値 v をとっていることを表す．
* F ∋ f : 零次関数 x ← 0 x + f を表す．
* x op y : cx + cy 個の元の和で値 vx + vy をとっている状態にする．
* f mapping x : c 個の元の和で値 c f をとっている状態にする．
* f composition g : 合成した零次関数 f o g を返す．
*/
using S9 = pair<ll, ll>; // 斉次ベクトル (v, c)
using F9 = ll; // 斉次行列 (0, f; 0, 1)
S9 op(S9 x, S9 y) {
	ll vx, vy, cx, cy;
	tie(vx, cx) = x; // ベクトル (vx, cx)
	tie(vy, cy) = y; // ベクトル (vy, cy)

	// (vx, cx) + (vy, cy) = (vx + vy, cx + cy)
	return { vx + vy, cx + cy };
}
S9 e9() { return { 0, 0 }; }
S9 mapping(F9 f, S9 x) {
	ll v, c;
	tie(v, c) = x; // ベクトル (v, c)

	// (0, f; 0, 1).(v, c) = (f c, c)
	return { f * c, c };
}
F9 composition(F9 f, F9 g) {
	// (0, f; 0, 1).(0, g; 0, 1) = (0, f; 0, 1)
	return f;
}
F9 id9() { return INFL; } // 使わない値なら何でも OK
using T = MLop_Monoid<S9, op, e9, F9, mapping, composition, id9>;


//【and 作用付き xor モノイド】
using S10 = int;
S10 op(S10 x, S10 y) { return x ^ y; }
S10 e10() { return 0; }
using F10 = int;
S10 mapping(F10 f, S10 x) { return f & x; }
F10 composition(F10 f, F10 g) { return f & g; }
F10 id10() { return ~0; }
using T = MLop_Monoid<S10, op, e10, F10, mapping, composition, id10>;


//【トロピカルアフィン変換 作用付き max モノイド】
/*
* S ∋ x : トロピカル加法（max）が冪等なので斉次座標を持つ必要がない．
* F ∋ f = {a, b} : トロピカル一次関数 f(x) = max(a + x, b) を表す．
* x op y : トロピカル加法 max(x, y)
* f mapping x : トロピカル一次関数への代入 f(x)
* f composition g : 合成したトロピカル一次関数 f o g を返す．
*/
using S11 = ll; // 斉次ベクトル (x, 0)
using F11 = pll; // 斉次行列 (a, b; -∞, 0)
S11 op(S11 x, S11 y) { return max(x, y); }
S11 e11() { return -INFL; }
S11 mapping(F11 f, S11 x) {
	ll a, b;
	tie(a, b) = f; // 行列 (a, b; -∞, 0)

	// (a, b; -∞, 0).(x, 0) = (max(a + x, b), 0)
	return max(a + x, b);
}
F11 composition(F11 f, F11 g) {
	ll a, b, c, d;
	tie(a, b) = f; // 行列 (a, b; -∞, 0)
	tie(c, d) = g; // 行列 (c, d; -∞, 0)

	// (a, b; -∞, 0).(c, d; -∞, 0) = (a + c, max(a + d, b); -∞, 0)
	return { a + c, max(a + d, b) };
}
F11 id11() { return { 0, -INFL }; }
using T = MLop_Monoid<S11, op, e11, F11, mapping, composition, id11>;


