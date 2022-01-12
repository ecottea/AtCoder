#pragma once
#include "header.h"
// ■■■■■ 列挙（数論） ■■■■■


//【自然数の分割の列挙】O(n の分割数)（n = 50 くらいまで動く）
/*
* 自然数 n を k 以下の数に分割する方法を ips に格納する．
*/
void integer_partitions(int n, int k, vvi& ips) {
	ips.clear();
	map<int, int> ip; // ip[i] : 分割に i を何個用いたか

	// n を k 以下の数で分割する．
	function<void(int, int)> rf = [&](int n, int k) {
		// 分割しきった場合
		if (n == 0) {
			// 分割の記録
			ips.push_back(vi());
			repitr(it, ip) {
				rep(i, it->second) {
					ips.rbegin()->push_back(it->first);
				}
			}
			return;
		}

		// 分割に使える数がもうない場合
		if (k == 0) {
			return;
		}

		// n が k 以上のときは，n を k と n-k に分割できる．
		if (n >= k) {
			ip[k]++;

			rf(n - k, k);

			ip[k]--;
			if (ip[k] == 0) {
				ip.erase(k);
			}
		}

		// これ以上 n の分割に k を使わない場合
		rf(n, k - 1);
	};

	rf(n, k);
}


