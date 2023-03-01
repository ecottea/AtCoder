#pragma once
#include "header.h"
// ■■■■■ 列挙（部分列） ■■■■■


//【部分列の列挙】O(2^n n)
/*
* a[0..n) の部分列全てを格納したリストを返す．
*/
template <class T>
vector<vector<T>> enumerate_subsequences(const vector<T>& a) {
	int n = sz(a);
	vector<vector<T>> seqs;

	repb(set, n) {
		vector<T> seq;
		rep(i, n) if (set & (1 << i)) seq.push_back(a[i]);
		seqs.push_back(seq);
	}

	return seqs;
}


//【部分列の列挙（長さ指定）】O(bin(n, m) m)
/*
* a[0..n) の長さ m の部分列全てを格納したリストを返す．
*/
template <class T>
vector<vector<T>> enumerate_subsequences(const vector<T>& a, int m) {
	int n = sz(a);
	vector<vector<T>> seqs;

	vb p(n);
	rep(i, m) p[i] = true;

	repp(p) {
		vector<T> seq;
		rep(i, n) if (p[i]) seq.push_back(a[i]);
		seqs.push_back(seq);
	}

	return seqs;
}


