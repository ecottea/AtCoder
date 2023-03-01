#pragma once
#include "header.h"
// ■■■■■ 数え上げ（集合） ■■■■■


//【部分集合の数え上げ】
/*
* Σset⊂[0..n) 1 = 2^n
*
* 証明：部分集合を定めるとき，各 i∈[0..n) について独立に含むか含まないかの 2 通りを選べる．
*/


//【直径 d 以下の部分集合の数え上げ（1 次元）】O(n)
/*
* 点群 a[0..n) の部分集合のうち，直径が d 以下であるものの個数を返す（空集合含む）
*/
template <class T>
mint count_distance_clique_1D(vector<T> a, T d) {
	int n = sz(a);

	// 点群を座標昇順にソートしておく．
	sort(all(a));

	vm pow2(n);
	pow2[0] = 1;
	rep(i, n - 1) pow2[i + 1] = pow2[i] * 2;

	mint res = 1; // 空集合を先に数えておく．

	// l, r : a[l..r] を走査中であることを表す．
	int l = 0, r = 0;

	// max(set) で場合分けしながら尺取法で数えていく．
	while (true) {
		// a[r] - a[l] <= d の場合
		if (a[r] - a[l] <= d) {
			// max(set) = a[r] であるのもの数え上げる．
			// いまの l は固定された r に対して最小の l となっているので，
			// 条件を満たす部分集合は全部で 2^(r - l) 個存在する．
			res += pow2[r - l];

			// 走査完了
			if (r == n - 1) break;

			// 右を 1 つ進める．
			r++;

		}
		// a[r] - a[l] > d の場合
		else {
			// 左を 1 つ進める．
			l++;
		}
	}

	return res;
}


//【部分和問題（数え上げ）】
/*
* 部分和問題.h へ
*/


