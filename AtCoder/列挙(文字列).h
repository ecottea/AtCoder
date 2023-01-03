#pragma once
#include "header.h"
// ■■■■■ 文字列の列挙 ■■■■■


//【任意文字列の列挙】O(n |cs|^n)
/*
* 文字集合 cs の要素からなる長さ n の文字列全てを格納したリストを返す．
*/
vector<string> enumerate_all_strings(int n, const vc& cs) {
	vector<string> strs;
	string s;

	// l : 長さ
	function<void(int)> rf = [&](int l) {
		// 長さが n の場合は記録
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


//【括弧列の列挙】O(Catalan(n) n)（n = 15 くらいまで OK）
/*
* 長さ 2n の括弧列全てを辞書順に格納したリストを返す．
*/
vector<string> enumerate_parenthesis_sequences(int n) {
	// verify : https://atcoder.jp/contests/typical90/tasks/typical90_b

	string s = "";
	vector<string> seqs;

	// l, r : 左右の括弧をあといくつ使えるか
	function<void(int, int)> rf = [&](int l, int r) {
		// 左右の括弧を使い切っている場合
		if (l == 0 && r == 0) {
			// 長さ 2 n の括弧列が完成しているので格納する．
			seqs.push_back(s);
			return;
		}

		// 左括弧をまだ使える場合
		if (l >= 1) {
			s += '(';
			rf(l - 1, r);
			s.pop_back();
		}

		// 左括弧が十分あり右括弧を使える場合
		if (r > l) {
			s += ')';
			rf(l, r - 1);
			s.pop_back();
		}

		return;
	};
	rf(n, n);

	return seqs;
}


//【括弧文字列の列挙】O(2^n)
/*
* '(', ')', '?' からなる文字列 s[0..n) の '?' をいずれかの括弧に置き換えて
* 得られる括弧文字列全てを格納したリストを返す．
*/
vector<string> enumerate_all_parenthesis_sequences(const string& s) {
	int n = sz(s);
	vector<string> strs;
	string p;

	// l : 長さ
	function<void(int)> rf = [&](int l) {
		// 長さが n の場合は記録
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

	return strs;
}


