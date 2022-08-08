#pragma once
#include "header.h"
// ¡¡¡¡¡ •¶š—ñ‚Ì—ñ‹“ ¡¡¡¡¡


//y”CˆÓ•¶š—ñ‚Ì—ñ‹“zO(n |cs|^n)
/*
* •¶šW‡ cs ‚Ì—v‘f‚©‚ç‚È‚é’·‚³ n ‚Ì•¶š—ñ‘S‚Ä‚ğ strs ‚ÉŠi”[‚·‚éD
*/
void enumerate_all_strings(int n, const vc& cs, vector<string>& strs) {
	strs.clear();
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
}


//yŠ‡ŒÊ—ñ‚Ì—ñ‹“zO(Catalan(n) n)in = 15 ‚­‚ç‚¢‚Ü‚Å OKj
/*
* ’·‚³ 2 n ‚ÌŠ‡ŒÊ—ñ‘S‚Ä‚ğ«‘‡‚É seqs ‚ÉŠi”[‚·‚éD
*/
void enumerate_parenthesis_sequences(int n, vector<string>& seqs) {
	// verify : https://atcoder.jp/contests/typical90/tasks/typical90_b

	string s = "";
	seqs.clear();

	// l, r : ¶‰E‚ÌŠ‡ŒÊ‚ğ‚ ‚Æ‚¢‚­‚Âg‚¦‚é‚©
	function<void(int, int)> rf = [&](int l, int r) {
		// ¶‰E‚ÌŠ‡ŒÊ‚ğg‚¢Ø‚Á‚Ä‚¢‚éê‡
		if (l == 0 && r == 0) {
			// ’·‚³ 2 n ‚ÌŠ‡ŒÊ—ñ‚ªŠ®¬‚µ‚Ä‚¢‚é‚Ì‚ÅŠi”[‚·‚éD
			seqs.push_back(s);
			return;
		}

		// ¶Š‡ŒÊ‚ğ‚Ü‚¾g‚¦‚éê‡
		if (l >= 1) {
			s += '(';
			rf(l - 1, r);
			s.pop_back();
		}

		// ¶Š‡ŒÊ‚ª\•ª‚ ‚è‰EŠ‡ŒÊ‚ğg‚¦‚éê‡
		if (r > l) {
			s += ')';
			rf(l, r - 1);
			s.pop_back();
		}

		return;
	};

	rf(n, n);
}


//yŠ‡ŒÊ•¶š—ñ‚Ì—ñ‹“zO(2^n)
/*
* '(', ')', '?' ‚©‚ç‚È‚é•¶š—ñ s[0..n) ‚Ì '?' ‚ğ‚¢‚¸‚ê‚©‚ÌŠ‡ŒÊ‚É’u‚«Š·‚¦‚Ä
* “¾‚ç‚ê‚éŠ‡ŒÊ•¶š—ñ‘S‚Ä‚ğ strs ‚ÉŠi”[‚·‚éD
*/
void enumerate_all_parenthesis_sequences(const string& s, vector<string>& strs) {
	int n = sz(s);
	strs.clear();
	string p;

	// l : ’·‚³
	function<void(int)> rf = [&](int l) {
		// ’·‚³‚ª n ‚Ìê‡‚Í‹L˜^
		if (l == n) {
			strs.push_back(p);
			return;
		}

		if (s[l] != ')') {
			p.push_back('(');
			rf(l + 1);
			p.pop_back();
		}
		if (s[l] != '(') {
			p.push_back(')');
			rf(l + 1);
			p.pop_back();
		}
	};

	rf(0);
}


