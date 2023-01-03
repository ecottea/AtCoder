#pragma once
#include "header.h"
// ■■■■■ 列挙（区間） ■■■■■


//【区間分割の和の列挙】O(2^n n)
/*
* a[0..n) を区間分割したときの，各区間の a の和を並べた列のリストを返す．
*/
template <class T>
vector<vector<T>> enumerate_interval_partitions(const vector<T>& a) {
	// verify : https://atcoder.jp/contests/abc197/tasks/abc197_c
	
	int n = sz(a);

	vector<vector<T>> seqs;

	repb(sep, n - 1) {
		vector<T> b; T acc = a[0];
		rep(i, n - 1) {
			if (sep & (1 << i)) {
				b.push_back(acc);
				acc = a[i + 1];
			}
			else {
				acc += a[i + 1];
			}
		}
		b.push_back(acc);

		seqs.push_back(b);
	}

	return seqs;
}


