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


