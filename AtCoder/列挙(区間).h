#pragma once
#include "header.h"
// ¡¡¡¡¡ —ñ‹“i‹æŠÔj ¡¡¡¡¡


//y‹æŠÔ•ªŠ„‚Ì˜a‚Ì—ñ‹“zO(2^n n)
/*
* a[0..n) ‚ğ‹æŠÔ•ªŠ„‚µ‚½‚Æ‚«‚ÌCŠe‹æŠÔ‚Ì a ‚Ì˜a‚ğ•À‚×‚½—ñ‚ğ seqs ‚ÉŠi”[‚·‚éD
*/
template <class T> void enumerate_interval_partitions(const vector<T>& a, vector<vector<T>>& seqs) {
	// verify : https://atcoder.jp/contests/abc197/tasks/abc197_c
	
	int n = sz(a);

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
}


