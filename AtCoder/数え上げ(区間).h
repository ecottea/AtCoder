#pragma once
#include "header.h"
#include "二分木.h"
// ■■■■■ 数え上げ（区間） ■■■■■


//【区間の数え上げ（区間端範囲制約）】O(n log n)
/*
* [0..n) の区間 [l..r] (l <= r) で，l_min[r] <= l <= l_max[r] かつ
* r_min[l] <= r <= r_max[l] を満たすものの個数を返す．
*/
ll count_intervals(const vi& l_min, const vi& l_max, const vi& r_min, const vi& r_max) {
	// 参考 : https://betrue12.hateblo.jp/entry/2020/03/28/112326
	// verify : https://yukicoder.me/problems/no/1031

	int n = sz(l_min);

	vvi r_min_to_ls(n), r_max_to_ls(n);
	rep(l, n) {
		// r_max[l] < l であるような条件は満たせないから無視する．
		if (r_max[l] < l) continue;

		// r_min[l] < l は r_min[l] = l だったことにして問題ない．
		r_min_to_ls[max(r_min[l], l)].push_back(l);
		r_max_to_ls[r_max[l]].push_back(l);
	}

	// ft_r[l] : 区間 [l..r] が許せるか
	fenwick_tree<int> ft(n);

	ll res = 0;

	// 区間の右端 r を昇順に見ていく．
	rep(r, n) {
		// 新たに r = r_min[l] なる l を許す．
		repe(l, r_min_to_ls[r]) {
			ft.add(l, 1);
		}

		// r を右端とする区間の個数を加算する．
		if (l_min[r] <= r) {
			res += ft.sum(l_min[r], min(l_max[r], r) + 1);
		}

		// 以降は r = r_max[l] なる l を許さない．
		repe(l, r_max_to_ls[r]) {
			ft.add(l, -1);
		}
	}

	return res;
}


//【区間の数え上げ（最小値指定）】O(n log n)
/*
* 列 a[0..n) について，最小値が m の区間 a[l..r) (l < r) の個数を cnt[m] に格納する．
*
* 利用：【デカルト木】
*/
template <class T> void count_min_intervals_dc(const vector<T>& a, unordered_map<T, ll>& cnt) {
	// verify : https://atcoder.jp/contests/agc005/tasks/agc005_b
	
	//【方法】
	// min(a[0..n)) = m とすると，最小値が m でない区間は a[i] = m なる a[i] を
	// 1 つも含まない区間なので，そのような a[i] を境界にして独立に数え上げられる．

	int n = sz(a);
	cnt.clear();

	Binary_Tree ct;
	cartesian_tree(a, ct);

	// a[rt] を最小値とする区間 a[l..r) についての処理を行う．
	function<void(int, int, int)> rf = [&](int rt, int l, int r) {
		int p = ct[rt].parent;
		if (p == -1 || a[p] != a[rt]) {
			cnt[a[rt]] += (ll)(r - l) * (r - l + 1) / 2;
		}

		int lrt = ct[rt].left;
		if (lrt != -1) {
			cnt[a[rt]] -= (ll)(rt - l) * (rt - l + 1) / 2;
			rf(lrt, l, rt);
		}

		int rrt = ct[rt].right;
		if (rrt != -1) {
			if (a[rt] < a[rrt]) {
				cnt[a[rt]] -= (ll)(r - rt - 1) * (r - rt) / 2;
			}
			rf(rrt, rt + 1, r);
		}
	};

	rf(ct.root, 0, n);
}


//【区間の数え上げ（最小値指定）】O(n log n)
/*
* 列 a[0..n) について，最小値が m の区間 a[l..r) (l < r) の個数を cnt[m] に格納する．
*
* 利用：【自身より小さい数の次の位置】，【自身より小さい数の前の位置】
*/
template <class T> void count_min_intervals(const vector<T>& a, unordered_map<T, ll>& cnt) {
	// verify : https://atcoder.jp/contests/agc005/tasks/agc005_b

	int n = sz(a);
	cnt.clear();

	vi pl, nl, pleq, nleq;
	prev_less_position(a, pl);
	next_less_position(a, nl);
	prev_less_position(a, pleq, true);
	next_less_position(a, nleq, true);

	rep(i, n) {
		if (pl[i] == pleq[i]) {
			cnt[a[i]] += (ll)(nl[i] - pl[i] - 1) * (nl[i] - pl[i]) / 2;
			cnt[a[i]] -= (ll)(i - pleq[i] - 1) * (i - pleq[i]) / 2;
		}
		cnt[a[i]] -= (ll)(nleq[i] - i - 1) * (nleq[i] - i) / 2;
	}
}


