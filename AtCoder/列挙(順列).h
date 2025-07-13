#pragma once
#include "header.h"
// ■■■■■ 列挙（順列） ■■■■■


//【順列の列挙】O(nPk)
/*
* a[0..n) の k 個の要素からなる順列全てからなるリストを返す（要素の重複は検出しない）
*/
template <class T>
vector<vector<T>> enumerate_permutations(const vector<T>& a, int k = -1) {
	// verify : https://atcoder.jp/contests/abc054/tasks/abc054_c

	int n = sz(a);
	if (k == -1) k = n;
	vector<vector<T>> seqs;
	if (n < k) return seqs;

	vector<T> seq; // 作成途中の順列

	// 残っている要素の位置を記録する双方向リスト（0 番目は根として利用する）
	vi prv(n + 2), nxt(n + 2);
	iota(all(prv), -1);
	iota(all(nxt), 1);

	function<void()> rf = [&]() {
		// 完成していれば記録する．
		if (sz(seq) == k) {
			seqs.push_back(seq);
			return;
		}

		for (int i = nxt[0]; i <= n; i = nxt[i]) {
			seq.push_back(a[i - 1]);
			nxt[prv[i]] = nxt[i];
			prv[nxt[i]] = prv[i];

			rf();

			prv[nxt[i]] = i;
			nxt[prv[i]] = i;
			seq.pop_back();
		}
	};

	rf();

	return seqs;
}


//【順列の列挙（隣接 swap）】
/*
* 以下に示す再帰的方法により，隣接要素の swap のみで順列を列挙することができる：
*	n=1 :	1*
*	n=2 :	1 2*
*			2* 1
*	n=3 :	1 2 3* (n=2 の step0 において 3* を右から左へ移動する)
*			1 3* 2
*			3* 1 2
*			3* 2 1 (n=2 の step1 において 3* を左から右へ移動する)
*			2 3* 1
*			2 1 3*
* 
* 参考 : https://en.wikipedia.org/wiki/Steinhaus%E2%80%93Johnson%E2%80%93Trotter_algorithm
*/


//【順列の列挙（置き換え）】O(k!)（k : Q の個数）
/*
* p[0..n) の各 Q を [0..n) のいずれかに置き換えて得られる順列全てからなるリストを返す．
*/
vvi enumerate_permutations_replace(const vi& p, int Q = -1) {
	// verify : https://atcoder.jp/contests/abc330/tasks/abc330_g

	int n = sz(p);

	vb used(n);
	rep(i, n) if (p[i] != Q) used[p[i]] = true;

	vi rem;
	rep(i, n) if (!used[i]) rem.push_back(i);

	vvi seqs;
	repp(rem) {
		vi seq(n); int pt = 0;
		rep(i, n) {
			if (p[i] == Q) seq[i] = rem[pt++];
			else seq[i] = p[i];
		}
		seqs.emplace_back(move(seq));
	}

	return seqs;
}


