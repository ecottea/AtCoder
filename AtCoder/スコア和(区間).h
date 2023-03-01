#pragma once
#include "header.h"
#include "数え上げ(区間).h"
// ■■■■■ スコア和（区間） ■■■■■


//【区間の min の総和】O(n log n)
/*
* Σi<j min( a[i..j) ) の値を返す．
*
* 利用：【区間の数え上げ（最小値指定）】
*/
template <class T>
T interval_min_sum(const vector<T>& a) {
	// verify : https://atcoder.jp/contests/agc005/tasks/agc005_b

	auto cnt = count_min_intervals(a);

	T res = T(0);
	repe(p, cnt) res += p.first * p.second;

	return res;
}


