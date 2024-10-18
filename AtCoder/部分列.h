#pragma once
#include "header.h"
#include "À•Wˆ³k.h"
// ¡¡¡¡¡ •”•ª—ñ ¡¡¡¡¡


//y•”•ª—ñ”»’èzO(n)
/*
* s[0..n) ‚ª•”•ª—ñ‚Æ‚µ‚Ä t[0..m) ‚ğŠÜ‚Ş‚©‚ğ•Ô‚·D
*
*iæÃ—~–@j
*/
template <class STR>
bool subsequenceQ(const STR& s, const STR& t) {
	// verify : https://yukicoder.me/problems/no/1909

	int n = sz(s), m = sz(t);

	if (m == 0) return true;

	int j = 0;
	rep(i, n) {
		if (s[i] == t[j]) j++;

		if (j == m) return true;
	}
	return false;
}


//y•”•ª—ñ”»’èi•¡”jzO(m)
/*
* s[0..n) ‚É‘Î‚µ ‘Oˆ—(•¶š—ñ).h ‚Ìy•¶š‚ÌŸ[‘O]‚ÌˆÊ’uz‚ğ—˜—p‚·‚ê‚ÎC
* s[0..n) ‚ª•”•ª—ñ‚Æ‚µ‚Ä t[0..m) ‚ğŠÜ‚Ş‚©‚ğ O(m) ‚Å”»’è‚Å‚«‚éD
* 
* verify : https://atcoder.jp/contests/ttpc2022/tasks/ttpc2022_e
*/


//yæÃ—~‘‰Á•”•ª—ñzO(n)
/*
* ”—ñ a[0..n) ‚Ì¶‚©‚ç‡‚ÉæÃ—~‚É‘I‚ñ‚¾‘‰Á•”•ª—ñ‚Ì’·‚³‚ğ•Ô‚·D
*/
template <class T>
int greedy_increasing_subsequence(const vector<T>& a) {
	int n = sz(a);

	T v = numeric_limits<T>::lowest();
	int len = 0;

	rep(i, n) if (chmax(v, a[i])) len++;

	return len;
}


//yæÃ—~‘‰Á•”•ª—ñiL‹`jzO(n)
/*
* ”—ñ a[0..n) ‚Ì¶‚©‚ç‡‚ÉæÃ—~‚É‘I‚ñ‚¾‘‰Á•”•ª—ñ‚Ì’·‚³‚ğ•Ô‚·D
*/
template <class T>
int weekly_greedy_increasing_subsequence(const vector<T>& a) {
	int n = sz(a);

	T v = numeric_limits<T>::lowest();
	int len = 0;

	rep(i, n) if (v <= a[i]) {
		v = a[i];
		len++;
	}

	return len;
}


