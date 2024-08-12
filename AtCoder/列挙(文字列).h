#pragma once
#include "header.h"
// ¡¡¡¡¡ •¶š—ñ‚Ì—ñ‹“ ¡¡¡¡¡


//y”CˆÓ•¶š—ñ‚Ì—ñ‹“zO(n |cs|^n)
/*
* •¶šW‡ cs ‚Ì—v‘f‚©‚ç‚È‚é’·‚³ n ‚Ì•¶š—ñ‘S‚Ä‚ğŠi”[‚µ‚½ƒŠƒXƒg‚ğ•Ô‚·D
*/
vector<string> enumerate_all_strings(int n, const string& cs) {
	// verify : https://yukicoder.me/problems/no/3015

	vector<string> strs;
	string s;

	// l : ’·‚³
	function<void(int)> rf = [&](int l) {
		// ’·‚³‚ª n ‚Ìê‡‚Í‹L˜^
		if (l == n) {
			strs.push_back(s);
			return;
		}

		// c : s[l]
		repe(c, cs) {
			s.push_back(c);
			rf(l + 1);
			s.pop_back();
		}
	};
	rf(0);

	return strs;
}


//y”CˆÓ•¶š—ñ‚Ì—ñ‹“i’uŠ·jzO(n |cs|^n)
/*
* s[0..n) ‚ÉŠÜ‚Ü‚ê‚é '?' ‚»‚ê‚¼‚ê‚ğ cs ‚Ì—v‘f‚Ì‚¢‚¸‚ê‚©‚É’u‚«Š·‚¦‚Ä
* “¾‚ç‚ê‚é•¶š—ñ‘S‚Ä‚ğŠi”[‚µ‚½ƒŠƒXƒg‚ğ•Ô‚·D
*/
vector<string> enumerate_all_strings(string s, const string& cs) {
	int n = sz(s);
	vector<string> strs;

	function<void(int)> rf = [&](int i) {
		if (i == n) {
			strs.push_back(s);
			return;
		}

		if (s[i] == '?') {
			char c0 = s[i];
			repe(c, cs) {
				s[i] = c;
				rf(i + 1);
			}
			s[i] = c0;
		}
		else {
			rf(i + 1);
		}
	};
	rf(0);

	return strs;
}


