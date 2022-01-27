#pragma once
#include "header.h"
// ■■■■■ 列に対する汎用性のある前処理 ■■■■■


//【自身より 1 大きい数の最左位置】O(n)
/*
* a[0..n) の各要素 a[i] について，j > i かつ a[j] = a[i] + 1 となる最小の j を
* nxt[j] に格納する．（存在しなければ -1）
*/
void left_most_next(const vi& a, vi& nxt) {
	int n = sz(a);

	// nxt[i] : j > i かつ a[j] = a[i] + 1 となる最小の j（なければ -1）
	nxt = vi(n, -1);

	// num_to_pos[x] : 値 x が最後に現れた位置（右から走査する）
	unordered_map<int, int> num_to_pos;

	repir(i, n - 1, 0) {
		if (num_to_pos.count(a[i] + 1)) {
			nxt[i] = num_to_pos[a[i] + 1];
		}
		num_to_pos[a[i]] = i;
	}
}


