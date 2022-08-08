#pragma once
#include "header.h"
// ■■■■■ 列挙（集合） ■■■■■


//【集合の分割の列挙】O(n 番目のベル数)（n = 12 くらいまで動く）
/*
* n 点集合の全ての分割を sps に格納する．
* 例えば [0..6) の分割 {{0, 1, 4}, {2, 5}, {3}} は [0,0,1,2,0,1] と一意的に表す．
*/
void set_partitions(int n, vvi& sps) {
	vi sp(n);
	sps.clear();

	// [0..i) までを m 個の集合に分割し終えているとする．
	function<void(int, int)> rf = [&](int i, int m) {
		// 全ての要素の所属を決め終えた場合
		if (i == n) {
			sps.push_back(sp);
			return;
		}

		// 要素 i を既に存在する集合に含める場合
		rep(j, m) {
			sp[i] = j;
			rf(i + 1, m);
		}

		// 要素 i を単独で新たな集合とする場合
		sp[i] = m;
		rf(i + 1, m + 1);

		return;
	};

	rf(0, 0);
}


//【自然数の分割の列挙】O(n の分割数)（n = 50 くらいまで動く）
/*
* 自然数 n を k 以下の自然数に分割する方法を ips に格納する．
*/
void integer_partitions_val(int n, int k, vvi& ips) {
	//【具体例】
	// (n, k) = (6, 3) のとき：
	//	0: 3 3
	//	1: 3 2 1
	//	2: 3 1 1 1
	//	3: 2 2 2
	//	4: 2 2 1 1
	//	5: 2 1 1 1 1
	//	6: 1 1 1 1 1 1

	ips.clear();
	map<int, int> ip; // ip[i] : 分割に i を何個用いたか

	// n を k 以下の数で分割する．
	function<void(int, int)> rf = [&](int n, int k) {
		// 分割しきった場合
		if (n == 0) {
			// 分割の記録
			ips.push_back(vi());
			for (auto it = ip.rbegin(); it != ip.rend(); it++) {
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


//【自然数の分割の列挙】O(n の分割数)（n = 50 くらいまで動く）
/*
* 自然数 n を d 個以下の自然数に分割する方法を ips に格納する．
*/
void integer_partitions_len(int n, int d, vvi& ips) {
	//【具体例】
	// (n, k) = (6, 3) のとき：
	//	0 : 6
	//	1 : 5 1
	//	2 : 4 2
	//	3 : 4 1 1
	//	4 : 3 3
	//	5 : 3 2 1
	//	6 : 2 2 2

	ips.clear();
	map<int, int> ip; // ip[i] : 分割に i を何個用いたか
	int len = 0;

	// n を k 以下の数で分割する．
	function<void(int, int)> rf = [&](int n, int k) {
		// 分割しきった場合
		if (n == 0) {
			// 分割の記録
			ips.push_back(vi());
			for (auto it = ip.rbegin(); it != ip.rend(); it++) {
				rep(i, it->second) ips.rbegin()->push_back(it->first);
			}
			return;
		}

		// 分割に使える数がもうない場か，分割の大きさが d に達した場合
		if (k == 0 || len == d) return;

		// n が k 以上のときは，n を k と n-k に分割できる．
		if (n >= k) {
			ip[k]++; len++;
			rf(n - k, k);
			len--; ip[k]--;
			if (ip[k] == 0) ip.erase(k);
		}

		// これ以上 n の分割に k を使わない場合
		rf(n, k - 1);
	};

	rf(n, n);
}


