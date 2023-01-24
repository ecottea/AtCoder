#pragma once
#include "header.h"
// ■■■■■ 列の列挙 ■■■■■


//【任意列の列挙】O(m^n n)
/*
* 集合 a[0..m) の要素からなる長さ n の列全てを格納したリストを返す．
*/
template <class T>
vector<vector<T>> enumerate_all_sequences(int n, const vector<T>& a) {
	// verify : https://atcoder.jp/contests/agc043/tasks/agc043_d

	vector<vector<T>> seqs;
	vector<T> seq; // 作成途中の列

	function<void()> rf = [&]() {
		// 完成していれば記録する．
		if (sz(seq) == n) {
			seqs.push_back(seq);
			return;
		}

		repe(x, a) {
			seq.push_back(x);
			rf();
			seq.pop_back();
		}
	};
	rf();

	return seqs;
}


//【任意列の列挙（要素ごと上限指定）】O(Πub[0..n) n)
/*
* 数列 a[0..n) で，∀i, a[i] ∈ [0..ub[i]) を満たすもの全てを格納したリストを返す．
*/
vvi enumerate_all_sequences(const vi& ub) {
	int n = sz(ub);
	vvi seqs;

	vi seq; // 作成途中の列
	int i = 0; // 列の長さ

	function<void()> rf = [&]() {
		// 完成していれば記録する．
		if (i == n) {
			seqs.push_back(seq);
			return;
		}

		rep(x, ub[i]) {
			seq.push_back(x); i++;
			rf();
			seq.pop_back(); i--;
		}
	};
	rf();

	return seqs;
}


//【任意列の列挙（要素ごと上下限指定）】O(Π(ub[0..n)-lb[0..n)) n)
/*
* 数列 a[0..n) で，∀i, lb[i] <= a[i] < ub[i] を満たすもの全てを格納したリストを返す．
*/
vvi enumerate_all_sequences(const vi& lb, const vi& ub) {
	// verify : https://atcoder.jp/contests/arc066/tasks/arc066_c

	int n = sz(lb);
	vvi seqs;

	vi seq; // 作成途中の列
	int i = 0; // 列の長さ

	function<void()> rf = [&]() {
		// 完成していれば記録する．
		if (i == n) {
			seqs.push_back(seq);
			return;
		}

		repi(x, lb[i], ub[i] - 1) {
			seq.push_back(x); i++;
			rf();
			seq.pop_back(); i--;
		}
	};
	rf();

	return seqs;
}


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


//【狭義単調増加列の列挙】O(bin(m, n) n)
/*
* 0 <= a[0] < a[1] < ... < a[n-1] < m なる列 a[0..n) を格納したリストを返す．
*/
vvi enumerate_strongly_increase_sequences(int n, int m) {
	// verify : https://atcoder.jp/contests/abc263/tasks/abc263_c

	vi a(n);
	vvi seqs;

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

	return seqs;
}


//【広義単調増加列の列挙】O(bin(n+m-1, n) n)
/*
* 0 <= a[0] <= a[1] <= ... <= a[n-1] < m なる列 a[0..n) を格納したリストを返す．
*/
vvi enumerate_weakly_increase_sequences(int n, int m) {
	vi a(n);
	vvi seqs;

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

	return seqs;
}


//【真の倍数列の列挙】O(?)（k = 10^4 くらいまで動く）
/*
* a[0] | a[1] | ... | a[m] <= k なる狭義単調列 a を格納したリストを返す．
*/
vvl enumerate_strongly_multiple_sequences(ll k) {
	vl a;
	vvl seqs;

	function<void()> rf = [&]() {
		// 列の記録
		if (!a.empty()) seqs.push_back(a);

		int i_min = (a.empty() ? 1 : 2);

		repi(i, i_min, k) {
			ll a_max = (a.empty() ? 1 : a.back());

			if (a_max * i > k) break;

			a.push_back(a_max * i);
			rf();
			a.pop_back();
		}
	};
	rf();

	return seqs;
}


//【作業用スタックを利用して得られる列の列挙】O(Catalan(n) n)
/*
* a[0..n) に対し，先頭から順にスタックに積んでいき，任意のタイミングでスタックから
* 要素を降ろしてくることで構成できる列を格納したリストを返す．
*/
vvi enumerate_stack_perm_sequences(const vi& a) {
	// verify : https://atcoder.jp/contests/abc262/tasks/abc262_g

	//（例）a[0..4) = [1, 2, 3, 4] のとき
	//	4 3 2 1  (((())))
	//	3 4 2 1  ((()()))
	//	3 2 4 1  ((())())
	//	3 2 1 4  ((()))()
	//	2 4 3 1  (()(()))
	//	2 3 4 1  (()()())
	//	2 3 1 4  (()())()
	//	2 1 4 3  (())(())
	//	2 1 3 4  (())()()
	//	1 4 3 2  ()((()))
	//	1 3 4 2  ()(()())
	//	1 3 2 4  ()(())()
	//	1 2 4 3  ()()(())
	//	1 2 3 4  ()()()()
	//
	// '(' を左から順に a[0..n) で彩色したとき，')' の色を左から順に並べたものになっている．
	// '(' をヘッダを 1 つ右へ移動，')' を最も右の空きへの書き込み，と対応させられる．
	// 任意の i < j < k について，[a[k], a[i], a[j]] は部分列に含まれない．

	int n = sz(a);
	vvi seqs;

	vi seq; // 作成途中の列
	int i = 0; // 次にスタックに積む a の要素番号
	vi stk(n); int pt = -1; // 作業用スタックとスタックトップの位置

	function<void()> rf = [&]() {
		// 列が完成している場合は記録する．
		if (sz(seq) == n) {
			seqs.push_back(seq);
			return;
		}

		// a[i] をスタックに積む場合
		if (i < n) {
			stk[++pt] = a[i++];
			rf();
			pt--; i--;
		}

		// スタックトップから作成途中の列に移動する場合
		if (pt >= 0) {
			int v = stk[pt--];
			seq.push_back(v);
			rf();
			seq.pop_back();
			stk[++pt] = v;
		}
	};
	rf();

	return seqs;
}


