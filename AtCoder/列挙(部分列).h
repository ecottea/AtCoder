#pragma once
#include "header.h"
// ■■■■■ 列挙（部分列） ■■■■■


//【部分列の列挙】O(2^n n)
/*
* a[0..n) の部分列全てを格納したリストを返す（重複含む，空列含む）
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


//【部分列の列挙（文字列）】O(2^n n)
/*
* s[0..n) の部分列全てを格納したリストを返す（重複含む，空列含む）
*/
vector<string> enumerate_subsequences(const string& s) {
	int n = sz(s);
	vector<string> seqs;

	repb(set, n) {
		string seq;
		repis(i, set) seq.push_back(s[i]);
		seqs.push_back(seq);
	}

	return seqs;
}


//【部分列の列挙（長さ指定）】O(bin(n+1, m))（m が大きいとき bin(n,m) よりずっと大きいので注意！）
/*
* a[0..n) の長さ m の部分列全てを格納したリストを返す（重複含む）
*/
template <class T>
vector<vector<T>> enumerate_subsequences(const vector<T>& a, int m) {
	// verify : https://atcoder.jp/contests/abc386/tasks/abc386_e

	int n = sz(a);

	vector<vector<T>> seqs;
	vector<T> seq(m); // 作成途中の列

	// seq[j] の候補として a[l..n) が挙がっている．
	function<void(int, int)> rf = [&](int l, int j) {
		// 完成していれば記録する．
		if (j == m) {
			seqs.push_back(seq);
			return;
		}

		// seq[j] を決定する．
		repi(i, l, n - m + j) {
			seq[j] = a[i];
			rf(i + 1, j + 1);
		}
	};
	rf(0, 0);

	return seqs;
}


//【共通部分列の列挙】O((2^n + 2^m) (n + m)^2)
/*
* a[0..n) と b[0..m) の共通部分列全てを格納したリストを返す（空列含む）
*/
template <class T>
vector<vector<T>> enumerate_common_subsequences(const vector<T>& a, const vector<T>& b) {
	int n = sz(a), m = sz(b);

	vector<vector<T>> seqs_a;
	repb(set_a, n) {
		vector<T> seq;
		rep(i, n) if (set_a & (1 << i)) seq.push_back(a[i]);
		seqs_a.push_back(seq);
	}

	vector<vector<T>> seqs_b;
	repb(set_b, m) {
		vector<T> seq;
		rep(j, m) if (set_b & (1 << j)) seq.push_back(b[j]);
		seqs_b.push_back(seq);
	}

	uniq(seqs_a); uniq(seqs_b);

	vector<vector<T>> seqs;
	set_intersection(all(seqs_a), all(seqs_b), inserter(seqs, seqs.end()));

	return seqs;
}


