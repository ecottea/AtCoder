#pragma once
#include "header.h"
// ■■■■■ 列挙（区間） ■■■■■


//【区間分割の列挙】O(2^n n)
/*
* a[0..n) の区間分割全てからなるリストを返す．
*/
template <class T>
vector<vector<vector<T>>> enumerate_interval_partitions(const vector<T>& a) {
	// verify : https://atcoder.jp/contests/abc197/tasks/abc197_c

	int n = sz(a);

	vector<vector<vector<T>>> seps;

	repb(set, n - 1) {
		vector<vector<T>> sep; vector<T> itval{ a[0] };
		rep(i, n - 1) {
			if (get(set, i)) {
				sep.push_back(itval);
				itval = vector<T>{ a[i + 1] };
			}
			else {
				itval.push_back(a[i + 1]);
			}
		}
		sep.push_back(itval);

		seps.push_back(sep);
	}

	return seps;
}


