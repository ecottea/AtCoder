#pragma once
#include "header.h"
// ■■■■■ 列の列挙 ■■■■■


//【広義単調増加列の列挙】O(binomial(n + m - 1, n))
/*
* 0 <= a[0] <= a[1] <= ... <= a[n-1] < m なる列 a を列挙する
*
* 戻り値 : 列が何通りあるか（二項係数 binomial(n + m - 1, n)）
*/
int weakly_increase_sequence(int n, int m) {
	vi a(n);
	int res = 0;

	// len : 列の長さ
	function<void(int)> rf = [&](int len) {
		// 列の長さが n の場合
		if (len == n) {
			// 完成しているので出力
			cout << a << endl;

			res++;
			return;
		}

		int i0 = (len > 0 ? a[len - 1] : 0);
		repi(i, i0, m - 1) {
			a[len++] = i;
			rf(len);
			len--;
		}
	};

	rf(0);

	return res;
}


//【1 の連続しないビット列の列挙】O(1.6^n)（フィボナッチ数）
/*
* 長さ n のビット列のうち 1 が連続しないものを seqs に格納する．
*/
void noncontinuous_bitsequence(int n, vl& seqs) {
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


//【括弧列の列挙】O(Catalan(n))（n = 15 くらいまで OK）
/*
* 長さ 2 n の括弧列を辞書順に seqs に格納する．
*/
void parenthesis_sequence(int n, vector<string>& seqs) {
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


