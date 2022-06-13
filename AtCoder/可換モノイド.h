#pragma once
#include "header.h"
// ¡¡¡¡¡ ‰ÂŠ·ƒ‚ƒmƒCƒh ¡¡¡¡¡


//y‰ÂŠ·ƒ‚ƒmƒCƒhz
/*
* ‰ÂŠ·ƒ‚ƒmƒCƒh (S, op, o) ‚ğ•\‚·D
*
* ‚·‚È‚í‚¿CW‡ S ‚Æ‚»‚Ìã‚Ì“ñ€‰‰Z + : S ~ S ¨ S ‚Å
*	Œ‹‡—¥ : Ía, b, c ¸ SC (a + b) + c = a + (b + c)
*	’PˆÊŒ³ : Ía ¸ S,        a + o = o + a = a
*	ŒğŠ·—¥ : Ía, b ¸ S,     a + b = b + a
* ‚ğ–‚½‚·‚à‚Ì‚Æ‚·‚éD
*/


//y‰ÁZ ‰ÂŠ·ƒ‚ƒmƒCƒhz
using S501 = ll;
S501 op501(S501 a, S501 b) { return a + b; }
S501 o501() { return 0; }
#define Add_cmonoid S501, op501, o501


//yæZ ‰ÂŠ·ƒ‚ƒmƒCƒhz
using S502 = mint;
S502 op502(S502 a, S502 b) { return a * b; }
S502 o502() { return 1; }
#define Mul_cmonoid S502, op502, o502


//ymax ‰ÂŠ·ƒ‚ƒmƒCƒhz
/* verify: https://atcoder.jp/contests/abl/tasks/abl_d */
using S503 = ll;
S503 op503(S503 a, S503 b) { return max(a, b); }
S503 o503() { return -INFL; }
#define Max_cmonoid S503, op503, o503


//ymin ‰ÂŠ·ƒ‚ƒmƒCƒhz
/* verify: https://atcoder.jp/contests/abc170/tasks/abc170_e */
using S504 = ll;
S504 op504(S504 a, S504 b) { return min(a, b); }
S504 o504() { return INFL; }
#define Min_cmonoid S504, op504, o504


//yxor ‰ÂŠ·ƒ‚ƒmƒCƒhz
using S505 = int;
S505 op505(S505 a, S505 b) { return a ^ b; }
S505 o505() { return 0; }
#define XOR_cmonoid S505, op505, o505


//yor ‰ÂŠ·ƒ‚ƒmƒCƒhz
/* verify: https://atcoder.jp/contests/abc157/tasks/abc157_e */
using S506 = int;
S506 op506(S506 a, S506 b) { return a | b; }
S506 o506() { return 0; }
#define OR_cmonoid S506, op506, o506


//yand ‰ÂŠ·ƒ‚ƒmƒCƒhz
using S507 = int;
S507 op507(S507 a, S507 b) { return a & b; }
S507 o507() { return ~0; }
#define AND_cmonoid S507, op507, o507


//ygcd ‰ÂŠ·ƒ‚ƒmƒCƒhz
/* verify : https://atcoder.jp/contests/abc125/tasks/abc125_c */
using S508 = ll;
S508 op508(S508 a, S508 b) { return gcd(a, b); }
S508 o508() { return 0; }
#define GCD_cmonoid S508, op508, o508


//ylcm ‰ÂŠ·ƒ‚ƒmƒCƒhz
using S509 = ll;
S509 op509(S509 a, S509 b) { return a / gcd(a, b) * b; }
S509 o509() { return 1; }
#define LCM_cmonoid S509, op509, o509


//y‘æ“ñÅ‘å’l ‰ÂŠ·ƒ‚ƒmƒCƒhz
/* verify: https://atcoder.jp/contests/arc100/tasks/arc100_c */
using S510 = pair<ll, ll>; // (Å‘å’l, ‘æ“ñÅ‘å’l)
S510 op510(S510 a, S510 b) {
	vector<ll> vals(4);
	vals[0] = a.first;
	vals[1] = a.second;
	vals[2] = b.first;
	vals[3] = b.second;
	sort(all(vals));

	return { vals[3], vals[2] };
}
S510 o510() { return { -INFL, -INFL }; }
#define Second_max_cmonoid S510, op510, o510


//y‘æ“ñÅ¬’l ‰ÂŠ·ƒ‚ƒmƒCƒhz
using S511 = pair<ll, ll>; // (Å¬’l, ‘æ“ñÅ¬’l)
S511 op511(S511 a, S511 b) {
	vector<ll> vals(4);
	vals[0] = a.first;
	vals[1] = a.second;
	vals[2] = b.first;
	vals[3] = b.second;
	sort(all(vals));

	return { vals[0], vals[1] };
}
S511 o511() { return { INFL, INFL }; }
#define Second_min_cmonoid S511, op511, o511


