#pragma once
#include "header.h"
// ■■■■■ 列挙（集合） ■■■■■


//【集合の分割の列挙】O(n 番目のベル数)（n=12 くらいまで動く）
/*
* [0..n) の分割全てからなるリストを返す．
* 例えば [0..6) の分割 {{0, 1, 4}, {2, 5}, {3}} は [0,0,1,2,0,1] と一意的に表す．
*/
vvi set_partitions(int n) {
	vi sp(n);
	vvi sps;

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

	return sps;
}


//【集合の分割の列挙（等分）】O(multinomial(n,[m]*k)/k!)
/*
* n = k m とし，[0..n) の k 個の m 点集合へ分割のリストを返す．
* 例えば [0..6) の 3 個の 2 点集合への分割の 1 つに {{0, 1}, {2, 5}, {3, 4}} がある．
*/
vvvi set_partitions(int k, int m) {
	// verify : https://atcoder.jp/contests/agc043/tasks/agc043_d
	
	int n = k * m;
	vvi sp;
	vvvi sps;

	function<void(int)> rf = [&](int x) {
		// 全ての要素の所属を決め終えた場合
		if (x == n) {
			sps.push_back(sp);
			return;
		}

		// 要素 x を既に存在する集合に含める場合
		rep(i, sz(sp)) {
			// 既に m 点集合になっているなら追加できない．
			if (sz(sp[i]) == m) continue;

			sp[i].push_back(x);
			rf(x + 1);
			sp[i].pop_back();
		}

		// 既に k 個の集合が存在している場合は新たな集合は作れない．
		if (sz(sp) == k) return;

		// 要素 x を単独で新たな集合とする場合
		sp.push_back(vi{ x });
		rf(x + 1);
		sp.pop_back();

		return;
	};

	rf(0);

	return sps;
}


//【自然数の分割の列挙（値が k 以下）】O(n の分割数)（n=50 くらいまで動く）
/*
* 自然数 n を k 以下の自然数（広義降順）に分割する方法のリストを返す．
*/
vvi integer_partitions_val(int n, int k) {
	// verify : https://atcoder.jp/contests/abc226/tasks/abc226_f

	//【具体例】
	// (n, k) = (6, 3) のとき：
	//	0: 3 3
	//	1: 3 2 1
	//	2: 3 1 1 1
	//	3: 2 2 2
	//	4: 2 2 1 1
	//	5: 2 1 1 1 1
	//	6: 1 1 1 1 1 1

	vvi ips;
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

	return ips;
}


//【自然数の分割の列挙（d 個以下）】O(n の分割数)（n=50 くらいまで動く）
/*
* 自然数 n を d 個以下の自然数（広義降順）に分割する方法のリストを返す．
*/
vvi integer_partitions_len(int n, int d) {
	//【具体例】
	// (n, k) = (6, 3) のとき：
	//	0 : 6
	//	1 : 5 1
	//	2 : 4 2
	//	3 : 4 1 1
	//	4 : 3 3
	//	5 : 3 2 1
	//	6 : 2 2 2

	vvi ips;
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

	return ips;
}


//【自然数の分割の列挙（値指定）】O(n の分割数)（n=50 くらいまで動く）
/*
* 自然数 n を正整数列 a[0..m) の要素に分割する方法のリストを返す．
*/
vvi integer_partitions_val(int n, const vi& a) {
	//【具体例】
	// n = 6, a[0..3) = [1, 3, 5] のとき：
	//	0: 5 1
	//	1: 3 3
	//	2: 3 1 1 1
	//	6: 1 1 1 1 1 1

	int m = sz(a);
	vvi ips; vi ip;

	// n を k 以下の数で分割する．
	function<void(int, int)> rf = [&](int n, int j) {
		// 分割しきった場合
		if (n == 0) {
			ips.push_back(ip);
			return;
		}

		// 分割に使える数がもうない場合
		if (j == m) return;

		int len = sz(ip);

		// n の分割に a[j] を i 個使用する．
		repi(i, 0, n / a[j]) {
			rf(n, j + 1);
			n -= a[j];
			ip.push_back(a[j]);
		}

		ip.resize(len);
	};
	rf(n, 0);

	return ips;
}


