#pragma once
#include "header.h"
// ■■■■■ 列挙（集合の分割） ■■■■■


//【集合の分割の列挙】O(BellB(n))（n=11 くらいまで動く）
/*
* [0..n) の分割全てからなるリストを返す．
* 例えば [0..6) の分割の 1 つに {{0, 1, 4}, {2, 5}, {3}} がある．
*/
vvvi set_partitions(int n) {
	// verify : https://yukicoder.me/problems/no/1561

	//【具体例】
	// n = 3 のとき：
	//	0: {0, 1, 2}
	//	1: {0, 1}, {2}
	//	2: {0, 2}, {1}
	//	3: {0}, {1, 2}
	//	4: {0}, {1}, {2}

	vvvi sps; vvi sp;

	function<void(int)> rf = [&](int x) {
		// 全ての要素の所属を決め終えた場合
		if (x == n) {
			sps.push_back(sp);
			return;
		}

		// 要素 x を既に存在する集合に含める場合
		rep(i, sz(sp)) {
			sp[i].push_back(x);
			rf(x + 1);
			sp[i].pop_back();
		}

		// 要素 x を単独で新たな集合とする場合
		sp.push_back(vi{ x });
		rf(x + 1);
		sp.pop_back();

		return;
	};
	rf(0);

	return sps;
}


//【集合の分割の列挙（k 個）】O(s2(n, k))
/*
* [0..n) の k 個の集合への分割全てからなるリストを返す．
*/
vvvi set_partitions(int n, int k) {
	//【具体例】
	// (n, k) = (3, 2) のとき：
	//	0: {0, 1}, {2}
	//	1: {0, 2}, {1}
	//	2: {0}, {1, 2}

	vvvi sps; vvi sp;

	function<void(int)> rf = [&](int x) {
		// 全ての要素の所属を決め終えた場合
		if (x == n) {
			if (sz(sp) == k) sps.push_back(sp);
			return;
		}

		// 要素 x を既に存在する集合に含める場合
		if (sz(sp) + (n - x - 1) >= k) {
			rep(i, sz(sp)) {
				sp[i].push_back(x);
				rf(x + 1);
				sp[i].pop_back();
			}
		}

		// 要素 x を単独で新たな集合とする場合
		if (sz(sp) < k) {
			sp.push_back(vi{ x });
			rf(x + 1);
			sp.pop_back();
		}

		return;
	};
	rf(0);

	return sps;
}


//【集合の分割の列挙（等分）】O(mul(n,[m]*k)/k!)
/*
* n = k m とし，[0..n) の k 個の m 点集合へ分割のリストを返す．
* 例えば [0..6) の 3 個の 2 点集合への分割の 1 つに {{0, 1}, {2, 5}, {3, 4}} がある．
*/
vvvi set_partitions_equal(int k, int m) {
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


//【多重集合の順序付き分割の列挙（K 個）】O(Πbin(c[i]+K-1,K-1))
/*
* a[0..n) を各 c[0..n) 個含む多重集合を K 個の集合に順序付きで分割する方法全てからなるリストを返す．
*/
template <class T>
vector<vector<vector<T>>> ordered_multiset_partitions(const vector<T>& a, vi c, int K) {
	// verify : https://mojacoder.app/users/ocv_contest/problems/f26bcaf3-ae22-4fa2-acdc-81b92115ac2e

	//【具体例】
	// a[0..2) = [0, 1], c[0..2) = [2, 2], K = 2 のとき：
	//	0: {0, 0, 1}, {1}
	//	1: {0, 0}, {1, 1}
	//	2: {0, 1, 1}, {0}
	//	3: {0, 1}, {0, 1}
	//	4: {0}, {0, 1, 1}
	//	5: {1, 1}, {0, 0}
	//	6: {1}, {0, 0, 1}

	int n = sz(a);

	int rem = accumulate(all(c), 0);

	vector<vector<vector<T>>> sps; vector<vector<T>> sp(K); int emp_cnt = K;

	// k 番目の集合に a[i] を入れるかどうか決める．
	function<void(int, int)> rf = [&](int k, int i) {
		// 全ての要素の所属を決め終えた場合
		if (rem == 0) {
			sps.push_back(sp);
			return;
		}

		// k 番目の集合に a[i] を入れる場合
		if (c[i] > 0 && rem - 1 >= K - 1 - k) {
			sp[k].push_back(a[i]);
			c[i]--;
			rem--;

			rf(k, i);

			sp[k].pop_back();
			c[i]++;
			rem++;
		}

		// k 番目の集合に a[i] を入れない場合
		if (k < K - 1 || c[i] == 0) {
			if (i == n - 1) {
				if (!sp[k].empty()) rf(k + 1, 0);
			}
			else rf(k, i + 1);
		}
	};
	rf(0, 0);

	return sps;
}


//【自然数の分割の列挙（値が k 以下）】O(PartitionsP(n))（n=50 くらいまで動く）
/*
* 自然数 n を k 以下の自然数（広義降順）に分割する方法のリストを返す．
*/
vvi integer_partitions_val(int n, int k = INF) {
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
	rf(n, min(k, n));

	return ips;
}


//【自然数の分割の列挙（d 個以下）】O(PartitionsP(n))（n=50 くらいまで動く）
/*
* 自然数 n を d 個以下の自然数（広義降順）に分割する方法のリストを返す．
*/
vvi integer_partitions_len(int n, int d = INF) {
	// verify : https://yukicoder.me/problems/no/2788

	//【具体例】
	// (n, d) = (6, 3) のとき：
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

		// 分割に使える数がもうないか，分割の大きさが d に達した場合
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


//【自然数の分割の列挙（値指定）】O(PartitionsP(n))（n=50 くらいまで動く）
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

	// n を a[j..m) の要素で分割する．
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


//【非負整数の順序付き分割の列挙（d 個）】O(bin(n+d-1, d-1))
/*
* 非負整数 n を d 個の非負整数に順序付きで分割する方法のリストを返す．
*/
vvi ordered_integer_partitions_len(int n, int d) {
	// verify : https://yukicoder.me/problems/no/2788

	//【具体例】
	// (n, d) = (3, 3) のとき：
	//	0: 0 0 3
	//	1: 0 1 2
	//	2: 0 2 1
	//	3: 0 3 0
	//	4: 1 0 2
	//	5: 1 1 1
	//	6: 1 2 0
	//	7: 2 0 1
	//	8: 2 1 0
	//	9: 3 0 0

	vvi ips;
	vi ip(d);

	function<void(int, int)> rf = [&](int s, int i) {
		if (i == d) {
			if (s == 0) ips.push_back(ip);
			return;
		}

		repi(x, 0, s) {
			ip[i] = x;
			rf(s - x, i + 1);
		}
	};
	rf(n, 0);

	return ips;
}


//【非負整数の組の分割の列挙（d 個）】O(?)
/*
* 自然数の組 (n1, n2) を d 個の非負整数の組（昇順）に分割する方法のリストを返す．
*/
vector<vector<pii>> integer_pair_partitions_len(int n1, int n2, int d) {
	// verify : https://yukicoder.me/problems/no/2788

	//【具体例】
	// ((n1, n2), d) = ((2, 2), 3) のとき：
	//	0: (0,0) (0,0) (2,2)
	//	1: (0,0) (0,1) (2,1)
	//	2: (0,0) (0,2) (2,0)
	//	3: (0,0) (1,0) (1,2)
	//	4: (0,0) (1,1) (1,1)
	//	5: (0,1) (0,1) (2,0)
	//	6: (0,1) (1,0) (1,0)
	//	7: (0,2) (1,0) (1,0)

	vector<vector<pii>> ips; vector<pii> ip;

	// 組 (n1, n2) を辞書順で (j1, j2) 以上の組で分割する．
	function<void(int, int, int, int)> rf = [&](int n1, int n2, int j1, int j2) {
		// 分割しきった場合
		if (sz(ip) == d) {
			if (n1 == 0 && n2 == 0) ips.push_back(ip);
			return;
		}

		// n1 の分割が不可能になった場合は終了．
		if (n1 < j1 * (d - sz(ip))) return;

		// (n1, n2) の分割に (j1, j2) を使用する場合
		if (n2 >= j2) {
			ip.emplace_back(j1, j2);
			rf(n1 - j1, n2 - j2, j1, j2);
			ip.pop_back();
		}

		// (n1, n2) の分割に (j1, j2) を使用しない場合
		if (n2 >= j2 + 1) rf(n1, n2, j1, j2 + 1);
		else rf(n1, n2, j1 + 1, 0);
	};
	rf(n1, n2, 0, 0);

	return ips;
}


