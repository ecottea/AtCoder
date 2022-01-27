#pragma once
#include "header.h"


//【Z-加群】
/*
* Z-加群 (S, op, o, inv, mul) を表す．
*（op は + を，inv は - を，mul は * をオーバーロードする）
*
* すなわち，(S, op, o, inv) がアーベル群であり，Z の S への左作用 * : Z × S → S が
*	累加 : ∀x ∈ S，∀a ∈ Z,  a x = (x の a 個の和) = mul(a, x)
* を満たすものとする．
*/
template <class S, S(*op)(S, S), S(*o_)(), S(*inv)(S), S(*mul)(int, S)>
struct Z_module {
	S v;

	// 零元
	static S o() { return o_(); }

	// コンストラクタ
	Z_module() : v(o()) {}
	Z_module(S x) : v(x) {}

	// 比較
	bool operator==(const Z_module& x) const { return v == x.v; }
	bool operator!=(const Z_module& x) const { return v != x.v; }

	// 和
	Z_module& operator+=(const Z_module& x) {
		if (v == o()) return *this = x;
		if (x.v == o()) return *this;
		return *this = op(v, x.v);
	}
	Z_module operator+(const Z_module& x) const { return Z_module(*this) += x; }

	// マイナス元
	Z_module operator-() const { return inv(v); }

	// 差
	Z_module& operator-=(const Z_module& x) { return *this += -x; }
	Z_module operator-(const Z_module& x) const { return Z_module(*this) -= x; }

	// Z-作用
	Z_module& operator*=(int a) {
		if (a == 0) return *this = o();
		if (a > 0) return *this = mul(a, v);
		return *this = -mul(-a, v);
	}
	Z_module operator*(int a) const { return Z_module(*this) *= a; }
	friend Z_module operator*(int a, const Z_module& x) { return x * a; }

	// 入出力
	friend istream& operator>>(istream& is, Z_module& x) { is >> x.v; return is; }
	friend ostream& operator<<(ostream& os, const Z_module& x) {
#ifdef _MSC_VER
		if (x.v == o()) return os << "o";
#endif
		return os << x.v;
	}
};


//【アーベル群】
/*
* アーベル群 (S, op, o, inv) を表す（op は + を，inv は - をオーバーロードする）
*
* すなわち，集合 S とその上の二項演算 + : S × S → S で
*	結合律 : ∀a, b, c ∈ S,  (a + b) + c = a + (b + c)
*	零元   : ∀a ∈ S,        a + o = o + a = a
*   逆元   : ∀a ∈ S,        a + inv(a) = inv(a) + a = o
*	交換律 : ∀a, b ∈ S,     a + b = b + a
* を満たすものとする．
*/
template <class S, S(*op)(S, S), S(*o_)(), S(*inv)(S)>
struct Abel_group {
	S v;

	// 零元
	static S o() { return o_(); }

	// コンストラクタ
	Abel_group() : v(o()) {}
	Abel_group(S a) : v(a) {}

	// 比較
	bool operator==(const Abel_group& a) const { return v == a.v; }
	bool operator!=(const Abel_group& a) const { return v != a.v; }

	// 和
	Abel_group& operator+=(const Abel_group& a) {
		if (v == o()) return *this = a;
		if (a.v == o()) return *this;
		return *this = op(v, a.v);
	}
	Abel_group operator+(const Abel_group& a) const { return Abel_group(*this) += a; }

	// マイナス元
	Abel_group operator-() const { return inv(v); }

	// 差
	Abel_group& operator-=(const Abel_group& a) { return *this += -a; }
	Abel_group operator-(const Abel_group& a) const { return Abel_group(*this) -= a; }

	// 入出力
	friend istream& operator>>(istream& is, Abel_group& a) { is >> a.v; return is; }
	friend ostream& operator<<(ostream& os, const Abel_group& a) {
#ifdef _MSC_VER
		if (a.v == o()) return os << "o";
#endif
		return os << a.v;
	}
};


//【モノイド】
/*
* モノイド (S, op, e) を表す（op は * をオーバーロードする）
*
* すなわち，集合 S とその上の二項演算 * : S × S → S で
*	結合律 : ∀a, b, c ∈ S， (a b) c = a (b c)
*	単位元 : ∀a ∈ S,        a e = e a = a
* を満たすものとする．
*/
template <class S, S(*op)(S, S), S(*e_)()>
struct Monoid {
	// verify : https://judge.yosupo.jp/problem/point_set_range_composite

	S v;

	// 単位元
	static S e() { return e_(); }

	// コンストラクタ
	Monoid() : v(e()) {}
	Monoid(S a) : v(a) {}

	// 比較
	bool operator==(const Monoid& a) const { return v == a.v; }
	bool operator!=(const Monoid& a) const { return v != a.v; }

	// 積
	Monoid operator*(const Monoid& a) const {
		if (v == e()) return a;
		if (a.v == e()) return *this;
		return op(v, a.v);
	}

	// 入出力
	friend istream& operator>>(istream& is, Monoid& a) { is >> a.v; return is; }
	friend ostream& operator<<(ostream& os, const Monoid& a) {
#ifdef _MSC_VER
		if (a.v == e()) return os << "e";
#endif
		return os << a.v;
	}
};


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
	// verify : https://judge.yosupo.jp/problem/range_affine_range_sum

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


//【可換モノイド】
/*
* 可換モノイド (S, op, o) を表す（op は + をオーバーロードする）
*
* すなわち，集合 S とその上の二項演算 + : S × S → S で
*	結合律 : ∀a, b, c ∈ S， (a + b) + c = a + (b + c)
*	単位元 : ∀a ∈ S,        a + o = o + a = a
*	交換律 : ∀a, b ∈ S,     a + b = b + a
* を満たすものとする．
*/
template <class S, S(*op)(S, S), S(*o_)()>
struct Commutative_monoid {
	S v;

	// 単位元
	static S o() { return o_(); }

	// コンストラクタ
	Commutative_monoid() : v(o()) {}
	Commutative_monoid(S a) : v(a) {}

	// 比較
	bool operator==(const Commutative_monoid& a) const { return v == a.v; }
	bool operator!=(const Commutative_monoid& a) const { return v != a.v; }

	// 和
	Commutative_monoid& operator+=(const Commutative_monoid& a) {
		if (v == o()) return *this = a;
		if (a.v == o()) return *this;
		return *this = op(v, a.v);
	}
	Commutative_monoid operator+(const Commutative_monoid& a) const {
		return Commutative_monoid(*this) += a;
	}

	// 入出力
	friend istream& operator>>(istream& is, Commutative_monoid& a) { is >> a.v; return is; }
	friend ostream& operator<<(ostream& os, const Commutative_monoid& a) {
#ifdef _MSC_VER
		if (a.v == o()) return os << "o";
#endif
		return os << a.v;
	}
};


//【可換環】
/*
* 可換環 (S, add, o, mi, mul, e) を表す．
* （add は + を，mins は - を，mul は * をオーバーロードする）
*
* すなわち，(S, add, o, mi) がアーベル群，(S, mul, e) が可換モノイドで，
*	分配律 : ∀a, b, c ∈ S， a(b + c) = a b + a c, (a + b)c = a c + b c
* を満たすものとする．
*/
template <class S, S(*add)(S, S), S(*o_)(), S(*mi)(S), S(*mul)(S, S), S(*e_)()>
struct CRing {
	S v;

	// 零元，単位元
	static S o() { return o_(); }
	static S e() { return e_(); }

	// コンストラクタ
	CRing() : v(o()) {}
	CRing(S a) : v(a) {}

	// 比較
	bool operator==(const CRing& a) const { return v == a.v; }
	bool operator!=(const CRing& a) const { return v != a.v; }

	// 和
	CRing& operator+=(const CRing& a) {
		if (v == o()) return *this = a;
		if (a.v == o()) return *this;
		return *this = add(v, a.v);
	}
	CRing operator+(const CRing& a) const { return CRing(*this) += a; }

	// マイナス元
	CRing operator-() const { return mi(v); }

	// 差
	CRing& operator-=(const CRing& a) { return *this += -a; }
	CRing operator-(const CRing& a) const { return CRing(*this) -= a; }

	// 積
	CRing& operator*=(const CRing& a) {
		if (v == o() || a.v == o()) return *this = o();
		if (v == e()) return *this = a;
		if (a.v == e()) return *this;
		return *this = mul(v, a.v);
	}
	CRing operator*(const CRing& a) const { return CRing(*this) *= a; }

	// 入出力
	friend istream& operator>>(istream& is, CRing& a) { is >> a.v; return is; }
	friend ostream& operator<<(ostream& os, const CRing& a) {
#ifdef _MSC_VER
		if (a.v == o()) return os << "o";
		if (a.v == e()) return os << "e";
#endif
		return os << a.v;
	}
};


//【群】
/*
* 群 (S, op, e, inv) を表す（op は * をオーバーロードする）
*
* すなわち，集合 S とその上の二項演算 * : S × S → S で
*	結合律 : ∀a, b, c ∈ S， (a b) c = a (b c)
*	単位元 : ∀a ∈ S,        a e = e a = a
*	逆元   : ∀a ∈ S,        a inv(a) = inv(a) a = e
* を満たすものとする．
*/
template <class S, S(*op)(S, S), S(*e_)(), S(*inv_)(S)>
struct Group {
	// verify : https://judge.yosupo.jp/problem/queue_operate_all_composite

	S v;

	// 単位元
	static S e() { return e_(); }

	// コンストラクタ
	Group() : v(e()) {}
	Group(S a) : v(a) {}

	// 比較
	bool operator==(const Group& a) const { return v == a.v; }
	bool operator!=(const Group& a) const { return v != a.v; }

	// 積
	Group operator*(const Group& a) const {
		if (v == e()) return a;
		if (a.v == e()) return *this;
		return op(v, a.v);
	}

	// 逆元
	Group inv() const { return inv_(v); }

	// 入出力
	friend istream& operator>>(istream& is, Group& a) { is >> a.v; return is; }
	friend ostream& operator<<(ostream& os, const Group& a) {
#ifdef _MSC_VER
		if (a.v == e()) return os << "e";
#endif
		return os << a.v;
	}
};


//【整列集合】
/*
* 整列集合 (S, comp, inf) を表す（comp は < をオーバーロードする）
*
* すなわち，集合 S とその上の二項関係 < : S × S → bool で
*	推移律 : ∀a, b, c ∈ S， a < b かつ b < c ⇒ a < c
*	三分律 : ∀a, b ∈ S,     a < b, a = b, a > b のちょうど 1 つが成立する
*	最小元の存在 : ∀A ⊂ S, ∃a ∈ A, ∀x ∈ A,  a <= x
* を満たすものとし，仮想的な最大元を inf で表す．
*/
template <class S, bool(*comp)(S, S), S(*inf_)()>
struct Wellordered_set {
	S v;

	// 最大元
	static S inf() { return inf_(); }

	// コンストラクタ
	Wellordered_set() : v(inf()) {}
	Wellordered_set(S a) : v(a) {}

	// 比較
	bool operator<(const Wellordered_set& a) const {
		if (v == inf()) return false;
		if (a.v == inf()) return true;
		return comp(v, a.v);
	}
	bool operator>(const Wellordered_set& a) const { return a < *this; }
	bool operator>=(const Wellordered_set& a) const { return !(*this < a); }
	bool operator<=(const Wellordered_set& a) const { return !(*this > a); }
	bool operator!=(const Wellordered_set& a) const { return (*this < a) || (*this > a); }
	bool operator==(const Wellordered_set& a) const { return !(*this != a); }

	// 入出力
	friend istream& operator>>(istream& is, Wellordered_set& a) { is >> a.v; return is; }
	friend ostream& operator<<(ostream& os, const Wellordered_set& a) {
#ifdef _MSC_VER
		if (a.v == inf()) return os << "inf";
#endif
		return os << a.v;
	}
};


//【半環】
/*
* 半環 (S, add, o, mul, e) を表す（add は + を，mul は * をオーバーロードする）
*
* すなわち，(S, add, o) が可換モノイド，(S, mul, e) がモノイドで，
*	分配律 : ∀a, b, c ∈ S， a(b + c) = a b + a c, (a + b)c = a c + b c
*	零倍   : ∀a ∈ S,        a o = o a = o
* を満たすものとする．
*/
template <class S, S(*add)(S, S), S(*o_)(), S(*mul)(S, S), S(*e_)()>
struct Semiring {
	// 参考 : https://nyaannyaan.github.io/library/math/semiring.hpp

	S v;

	// 零元，単位元
	static S o() { return o_(); }
	static S e() { return e_(); }

	// コンストラクタ
	Semiring() : v(o()) {}
	Semiring(S a) : v(a) {}

	// 比較
	bool operator==(const Semiring& a) const { return v == a.v; }
	bool operator!=(const Semiring& a) const { return v != a.v; }

	// 和
	Semiring& operator+=(const Semiring& a) {
		if (v == o()) return *this = a;
		if (a.v == o()) return *this;
		return *this = add(v, a.v);
	}
	Semiring operator+(const Semiring& a) const { return Semiring(*this) += a; }

	// 積
	Semiring operator*(const Semiring& a) const {
		if (v == o() || a.v == o()) return o();
		if (v == e()) return a;
		if (a.v == e()) return *this;
		return mul(v, a.v);
	}

	// 入出力
	friend istream& operator>>(istream& is, Semiring& a) { is >> a.v; return is; }
	friend ostream& operator<<(ostream& os, const Semiring& a) {
#ifdef _MSC_VER
		if (a.v == o()) return os << "o";
		if (a.v == e()) return os << "e";
#endif
		return os << a.v;
	}
};


