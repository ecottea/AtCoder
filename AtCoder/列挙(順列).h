#pragma once
#include "header.h"
// ■■■■■ 列挙（順列） ■■■■■


//【順列の列挙】O(n!)
/*
* [0..n) の順列全てを辞書順に seqs に格納する．
*/
void enumerate_all_permutations(int n, vvi& seqs) {
	// verify : https://atcoder.jp/contests/abc054/tasks/abc054_c

	seqs.clear();
	vi seq; // 作成途中の順列

	// 残っている数を記録する双方向リスト（0 番目は根として利用する）
	vi prv(n + 2), nxt(n + 2);
	iota(all(prv), -1);
	iota(all(nxt), 1);

	function<void()> rf = [&]() {
		// 完成していれば記録する．
		if (sz(seq) == n) {
			seqs.push_back(seq);
			return;
		}

		for (int i = nxt[0]; i <= n; i = nxt[i]) {
			seq.push_back(i - 1);
			nxt[prv[i]] = nxt[i];
			prv[nxt[i]] = prv[i];

			rf();

			prv[nxt[i]] = i;
			nxt[prv[i]] = i;
			seq.pop_back();
		}
	};

	rf();
}


