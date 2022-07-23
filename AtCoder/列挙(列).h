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

		rep(i, n) if (set & (1 << i)) seq.push_back(a[i]);

		seqs.push_back(seq);
	}
}


//【狭義単調増加列の列挙】O(bin(m, n) n)
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


//【広義単調増加列の列挙】O(bin(n+m-1, n) n)
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


