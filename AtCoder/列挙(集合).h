#pragma once
#include "header.h"
// ■■■■■ 列挙（集合） ■■■■■


//【集合の分割の列挙】O((n / log n)^n)
/*
* n 点集合の分割を全出力する．
*
* 戻り値 : 分割の個数（ベル数 B_n）
*/
int set_partitions(int n) {
	vector<int> sets;
	int res = 0;

	function<void(int)> rf = [&](int e) {
		// 全ての要素を集合に割り振り終わった場合
		if (e == n) {
			// 分割を出力する．
			repe(set, sets) {
				rep(i, n) {
					if (set & (1 << i)) {
						cout << 1;
					}
					else {
						cout << 0;
					}
				}
				cout << " ";
			}
			cout << endl;

			res++;
			return;
		}

		// 要素 e を割り当てる集合 set それぞれについて
		rep(i, sz(sets)) {
			sets[i] += 1 << e;
			rf(e + 1);
			sets[i] -= 1 << e;
		}

		// 要素 e を単独で新たな集合とする場合
		sets.push_back({ 1 << e });
		rf(e + 1);
		sets.pop_back();

		return;
	};

	rf(0);

	return res;
}


