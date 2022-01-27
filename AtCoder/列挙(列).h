#pragma once
#include "header.h"
// ■■■■■ 列の列挙 ■■■■■


//【部分列の列挙】O(2^n n)
/*
* a[0..n) の部分列を seqs に格納する．
*/
template <class T>
void enumerate_subsequences(const vector<T>& a, vector<vector<T>>& seqs) {
	int n = sz(a);
	seqs.clear();

	repb(set, n) {
		vector<T> seq;

		rep(i, n) {
			if (set & (1 << i)) {
				seq.push_back(a[i]);
			}
		}

		seqs.push_back(seq);
	}
}


//【狭義単調増加列の列挙】O(binomial(m, n) n)
/*
* 0 <= a[0] < a[1] < ... < a[n-1] < m なる列 a[0..n) を seqs に格納する．
*/
void enumerate_strongly_increase_sequences(int n, int m, vvi& seqs) {
	vi a(n);
	seqs.clear();

	// len : 列の長さ
	function<void(int)> rf = [&](int len) {
		// 列の長さが n の場合
		if (len == n) {
			// 完成しているので記録
			seqs.push_back(a);
			return;
		}

		// i0 : 直前の数の大きさ
		int i0 = (len > 0 ? a[len - 1] : -1);

		// 直前の数 i0 より大きい数が選べる．
		repi(i, i0 + 1, m - 1) {
			a[len++] = i;
			rf(len);
			len--;
		}
	};

	rf(0);
}


//【広義単調増加列の列挙】O(binomial(n + m - 1, n) n)
/*
* 0 <= a[0] <= a[1] <= ... <= a[n-1] < m なる列 a[0..n) を seqs に格納する．
*/
void enumerate_weakly_increase_sequences(int n, int m, vvi& seqs) {
	vi a(n);
	seqs.clear();

	// len : 列の長さ
	function<void(int)> rf = [&](int len) {
		// 列の長さが n の場合
		if (len == n) {
			// 完成しているので記録
			seqs.push_back(a);
			return;
		}

		// i0 : 直前の数の大きさ
		int i0 = (len > 0 ? a[len - 1] : 0);

		// 直前の数 i0 以上の数が選べる．
		repi(i, i0, m - 1) {
			a[len++] = i;
			rf(len);
			len--;
		}
	};

	rf(0);
}


//【真の倍数列の列挙】O(?)（k = 10^4 くらいまで動く）
/*
* a[0] | a[1] | ... | a[m] <= k なる狭義単調列 a を seqs に格納する．
*/
void enumerate_strongly_multiple_sequences(ll k, vvl& seqs) {
	vl a;
	seqs.clear();

	function<void()> rf = [&]() {
		// 列の記録
		seqs.push_back(a);

		int i_min = (a.empty() ? 1 : 2);

		repi(i, i_min, k) {
			ll a_max = (a.empty() ? 1 : *a.rbegin());

			if (a_max * i > k) {
				break;
			}

			a.push_back(a_max * i);
			rf();
			a.pop_back();
		}
	};

	rf();
}


//【1 の連続しないビット列の列挙】O(1.6^n n)（フィボナッチ数）
/*
* 長さ n のビット列のうち 1 が連続しないものを seqs に格納する．
*/
void enumerate_noncontinuous_bitsequences(int n, vl& seqs) {
	ll b = 0;
	seqs.clear();

	function<void(int, int)> rf = [&](int len, int d) {
		// 残りの長さが 0 の場合
		if (len == 0) {
			seqs.push_back(b);
			return;
		}

		// 直前の桁が 0 である場合は 1 を使える．
		if (d == 0) {
			b = b * 2 + 1;
			rf(len - 1, 1);
			b = (b - 1) / 2;
		}

		// 0 を使う．
		b *= 2;
		rf(len - 1, 0);
		b /= 2;

		return;
	};

	rf(n, 0);
}


//【括弧列の列挙】O(Catalan(n) n)（n = 15 くらいまで OK）
/*
* 長さ 2 n の括弧列を辞書順に seqs に格納する．
*/
void enumerate_parenthesis_sequences(int n, vector<string>& seqs) {
	// verify : https://atcoder.jp/contests/typical90/tasks/typical90_b

	string s = "";
	seqs.clear();

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
}


