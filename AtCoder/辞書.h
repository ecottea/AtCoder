#pragma once
#include "header.h"
#include "フェニック木.h"
// ■■■■■ 辞書 ■■■■■



//【動的辞書】
/*
* Dynamic_dictionary(n) : O(n)
*	[0..n) を記録可能な辞書を空で初期化する．
*
* Dynamic_dictionary(n, a) : O(n)
*	[0..n) を記録可能な辞書を多重集合 a で初期化する．
*
* size() : O(log n)
*	要素の総数を返す．
*
* count(v) : O(log n)
*	要素 v の個数を返す．
* 
* count(l, r) : O(log n)
*	値 [l..r) をもつ要素の個数を返す．
*
* insert(v) : O(log n)
*	要素 v を挿入する．
*
* erase(v) : O(log n)
*	要素 v を削除する．
*
* get(i) : O(log n)
*	昇順で i 番目の要素を返す．（i : 0-indexed）
*
* lower_bound(v) : O(log n)
*	v が昇順で何番目の要素かを返す．（0-indexed）
*
* 利用：【フェニック木】
*/
using S12 = int;
S12 op12(S12 x, S12 y) { return x + y; }
S12 e12() { return 0; }
S12 inv12(S12 x) { return -x; }
struct Dynamic_dictionary {
	int n;

	// ft[v] : 要素 v の個数
	using RSQ = Fenwick_tree<S12, op12, e12, inv12>;
	RSQ ft;


	// コンストラクタ（何もしない）
	Dynamic_dictionary() : n(0) {}

	// [0..n) を記録可能な辞書を空で初期化する．
	Dynamic_dictionary(int n_) : n(n_), ft(n) {}

	// [0..n) を記録可能な辞書を多重集合 a で初期化する．
	Dynamic_dictionary(int n_, const vi& a) : n(n_) {
		vi cnt(n);
		repe(v, a) cnt[v]++;
		ft = RSQ(cnt);
	}

	// 要素の総数を返す．
	int size() { return ft.prod(0, n); }

	// 要素 v の個数を返す．
	int count(int v) { return ft.get(v); }

	// 値[l..r) をもつ要素の個数を返す．
	int count(int l, int r) { return ft.prod(l, r); }

	// 要素 v を挿入する．
	void insert(int v) { ft.apply(v, 1); }

	// 要素 v を削除する．
	void erase(int v) { ft.apply(v, -1); }

	// 昇順で i 番目の要素を返す．
	int get(int i) {
		auto f = [&](ll x) { return x <= i; };
		return ft.max_right(f);
	}

	// v が昇順で何番目の要素かを返す．
	int lower_bound(int v) { return ft.prod(0, v); }

	// デバッグ出力用
	friend ostream& operator<<(ostream& os, const Dynamic_dictionary& dd) {
		rep(v, dd.n) rep(hoge, dd.ft.get(v)) os << v << " ";
		return os;
	}
};


