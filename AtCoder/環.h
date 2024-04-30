#pragma once
#include "header.h"
#include "FPS(mint).h"
#include "‘½”{’·®”.h"
// ¡¡¡¡¡ ŠÂ ¡¡¡¡¡


//yŠÂ‚Ì’è‹`z
/*
* ŠÂ (S, add, o, mi, mul, e) ‚ğ•\‚·D
*
* ‚·‚È‚í‚¿C(S, add, o, mi) ‚ªƒA[ƒxƒ‹ŒQC(S, mul, e) ‚ªƒ‚ƒmƒCƒh‚ÅC
*	•ª”z—¥ : Ía, b, c ¸ SC a(b + c) = a b + a c, (a + b)c = a c + b c
* ‚ğ–‚½‚·‚à‚Ì‚Æ‚·‚éD
*/


//y‰ÁZ - æZ ‰ÂŠ·ŠÂz
/* verify : https://atcoder.jp/contests/arc059/tasks/arc059_c */
using S401 = ll;
S401 add401(S401 x, S401 y) { return x + y; }
S401 o401() { return 0; }
S401 mi401(S401 x) { return -x; }
S401 mul401(S401 x, S401 y) { return x * y; }
S401 e401() { return 1; }
#define Add_Mul_cring S401, add401, o401, mi401, mul401, e401


//yZ/2^64Z ‰ÁZ - æZ ‰ÂŠ·ŠÂz
/* verify : https://judge.yosupo.jp/problem/convolution_mod_2_64 */
using S406 = ull;
S406 add406(S406 x, S406 y) { return x + y; }
S406 o406() { return 0; }
S406 mi406(S406 x) { return ~x + 1; }
S406 mul406(S406 x, S406 y) { return x * y; }
S406 e406() { return 1; }
#define Ull_Add_Mul_cring S406, add406, o406, mi406, mul406, e406


//yXOR - AND ‰ÂŠ·ŠÂz
/* verify : https://atcoder.jp/contests/abc009/tasks/abc009_4 */
using S402 = unsigned int;
S402 add402(S402 x, S402 y) { return x ^ y; }
S402 o402() { return 0; }
S402 mi402(S402 x) { return x; }
S402 mul402(S402 x, S402 y) { return x & y; }
S402 e402() { return ~0; }
#define XOR_AND_cring S402, add402, o402, mi402, mul402, e402


//yFPS ‚Ì ‰ÁZ - æZ ‰ÂŠ·ŠÂz
/* verify : https://atcoder.jp/contests/abc327/tasks/abc327_g */
using S404 = MFPS;
S404 add404(S404 x, S404 y) { return x + y; }
S404 o404() { return S404(); }
S404 mi404(S404 x) { return -x; }
S404 mul404(S404 x, S404 y) { return x * y; }
S404 e404() { return S404(1); }
#define MFPSAdd_mul_cring S404, add404, o404, mi404, mul404, e404


//yÅ‚Ÿ€‚Ì‚İ ‰ÁZ - æZ ‰ÂŠ·ŠÂz
/*
* S ¹ {e, c} : c z^e ‚ğ•\‚·D
*/
// verify : https://yukicoder.me/problems/no/2429
using E403 = int;
using C403 = mint;
using S403 = pair<E403, C403>;
S403 add403(S403 x, S403 y) {
	auto [ex, cx] = x;
	auto [ey, cy] = y;

	if (ex > ey) return x;
	if (ex < ey) return y;
	return { ex, cx + cy };
}
S403 o403() { return { -INF, 0 }; }
S403 mi403(S403 x) {
	auto [ex, cx] = x;

	return { ex, -cx };
}
S403 mul403(S403 x, S403 y) {
	auto [ex, cx] = x;
	auto [ey, cy] = y;

	return { max(ex + ey, -INF), cx * cy };
}
S403 e403() { return { 0, 1 }; }
#define MaxDegTerm_Add_Mul_cring S403, add403, o403, mi403, mul403, e403


//y‘½”{’·®” ‰ÁZ - æZ ‰ÂŠ·ŠÂzi’x‚¢j
/* verify : https://mojacoder.app/users/first_vil/contests/Iamsorry2/tasks/3 */
using S405 = string;
S405 add405(S405 a, S405 b) {
	int ca = 1, cb = 1;

	if (a[0] == '-') {
		ca = -1;
		a.erase(a.begin());
	}

	if (b[0] == '-') {
		cb = -1;
		b.erase(b.begin());
	}

	string res;

	if (ca == 1 && cb == 1) {
		res = add_bint(a, b);
	}
	else if (ca == 1 && cb == -1) {
		if (comp_bint(a, "<", b)) {
			res = sub_bint(b, a);
			res.insert(res.begin(), '-');
		}
		else {
			res = sub_bint(a, b);
		}
	}
	else if (ca == -1 && cb == 1) {
		if (comp_bint(a, ">", b)) {
			res = sub_bint(a, b);
			res.insert(res.begin(), '-');
		}
		else {
			res = sub_bint(b, a);
		}
	}
	else if (ca == -1 && cb == -1) {
		res = add_bint(a, b);
		res.insert(res.begin(), '-');
	}

	return res;
}
S405 o405() { return "0"; }
S405 mi405(S405 a) {
	if (a == "0") {
		;
	}
	else if (a[0] == '-') {
		a.erase(a.begin());
	}
	else {
		a.insert(a.begin(), '-');
	}

	return a;
}
S405 mul405(S405 a, S405 b) {
	int ca = 1, cb = 1;

	if (a[0] == '-') {
		ca = -1;
		a.erase(a.begin());
	}

	if (b[0] == '-') {
		cb = -1;
		b.erase(b.begin());
	}

	string res;

	if (ca == cb) {
		res = mul_bint(a, b);
	}
	else {
		res = mul_bint(b, a);
		if (res != "0") res.insert(res.begin(), '-');
	}

	return res;

}
S405 e405() { return "1"; }
#define BintAdd_Mul_cring S405, add405, o405, mi405, mul405, e405


// ======================================================

//yŠÂz
/*
* ŠÂ (S, add, o, mi, mul, e) ‚ÌŒ³‚ğ•\‚·iadd, mi, mul ‚Í +, -, * ‚ğ‚»‚ê‚¼‚êƒI[ƒo[ƒ[ƒh‚·‚éj
*/
template <class S, S(*add)(S, S), S(*o_)(), S(*mi)(S), S(*mul)(S, S), S(*e_)()>
struct Ring {
	// verify : https://mojacoder.app/users/first_vil/contests/Iamsorry2/tasks/3

	S v;

	// —ëŒ³
	static S o() { return o_(); }

	// ’PˆÊŒ³
	static S e() { return e_(); }

	// ƒRƒ“ƒXƒgƒ‰ƒNƒ^
	Ring() : v(o()) {}
	Ring(S v) : v(v) {}

	// ƒLƒƒƒXƒg
	operator S() const { return v; }

	// ”äŠr
	bool operator==(const Ring& b) const { return v == b.v; }
	bool operator!=(const Ring& b) const { return v != b.v; }

	// ’P€‰‰Z
	Ring operator-() const { return Ring(mi(v)); }

	// “ñ€‰‰Z
	Ring& operator+=(const Ring& b) { v = add(v, b.v); return *this; }
	Ring& operator-=(const Ring& b) { v = add(v, mi(b.v)); return *this; }
	Ring& operator*=(const Ring& b) { v = mul(v, b.v); return *this; }
	friend Ring operator+(Ring a, const Ring& b) { a += b; return a; }
	friend Ring operator-(Ring a, const Ring& b) { a -= b; return a; }
	friend Ring operator*(Ring a, const Ring& b) { a *= b; return a; }

	// “üo—Í
	friend istream& operator>>(istream& is, Ring& a) { is >> a.v; return is; }
	friend ostream& operator<<(ostream& os, const Ring& a) {
#ifdef _MSC_VER
		if (a.v == o()) return os << "o";
		if (a.v == e()) return os << "e";
#endif
		return os << a.v;
	}
};


