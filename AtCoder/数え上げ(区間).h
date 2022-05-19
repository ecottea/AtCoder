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


//【隣接和をとる操作で得られる列】
/*
* a[0..n) に対し，隣接する 2 数をその和に置き換えるという操作を考える．
* 操作を 0 回以上の任意回行って得られる列が何通りあるかを返す．
*
*（前処理で高速化した DP）
*/
mint count_adjacent_sum_contraction_seq(const vl& a) {
	// verify : https://atcoder.jp/contests/abc230/tasks/abc230_f

	int n = sz(a);

	//【方法】
	// 列を決めるには，n-1 箇所の隙間それぞれに仕切りを入れるかどうかを決めれば良い．
	// ただし同じ列が得られる場合仕切りは右優先で入れていくものとする．
	// 例えば a[0..5) = (1, 2, -2, 3, -3) から (1, 0) を作るような仕切りの入れ方は
	// (1, 2, -2 | 3, -3) に限定する．
	// これで仕切りの入れ方とできる列との間に一対一の対応ができる．
	//
	// dp[i] を「a[0..i) に対する仕切りの入れ方の数」とし，dp[i+1] を考える．
	// 仕切りを入れて
	//		(... | a[j], ..., a[i-1] | a[i])
	// とすることは常に行える．一方仕切りを入れず
	//		(... | a[j], ..., a[i-1], a[i])
	// とすることは，右優先の規則より
	//		a[j] + ... + a[i-1] = 0
	// となるような j がある場合は行えない．よって遷移式は
	//		dp[i+1] = 2 * dp[i] 
	//			-「a[0..i) に対する仕切りの入れ方で末尾が 0 かつ長さ 2 以上の個数」
	// となる．
	// 
	// a[0..i) に対しても右優先の規則が働いているので，
	//		a[j] + ... + a[i-1] = 0
	// であるならば，
	//		a[j'] + ... + a[j-1] = 0
	// であるような
	//		(... | a[j'], ..., a[j-1] | a[j], ..., a[i-1])
	// という仕切りの入れ方は dp[i] に数えられていない．
	// よって
	//		k = k(i) = max_j(a[j] + ... + a[i-1] = 0)
	// とおけば，求める場合の数は a[0..k) への仕切りの入れ方の数，すなわち dp[k] である．
	// ただし k = 0 のときは長さが 1 の列に対応してしまい，これは引いてはならないので，
	// 便宜上 dp[0] = 0 と定める．（空列のとき 0 通りというのは不自然ではあるが）
	//
	// あとは前処理を行い，各 i に対して
	//		k(i) = max_j(a[j] + ... + a[i-1] = 0)
	// を求めておけば良い．これは，累積和
	//		acc[i] := Σa[0..i)
	// を導入すれば
	//		k(i) = max_j(acc[j] = acc[i])
	// と言い換えられるので，累積和の値 s ごとに
	//		Σa[0..j) = s
	// となるような最大の j を持ちながら k(i) を下から順に求めていくことができる．

	// dp[i] : a[0..i) に対する仕切りの入れ方の数
	vm dp(n + 1);
	dp[0] = 0; dp[1] = 1;

	// k[i] : max_j(acc[j] = acc[i])
	vi k(n + 1);

	// acc_to_k[v] = k は，k = max_j(acc[j] = v) であることを表す．
	unordered_map<ll, int> acc_to_k;

	ll s = 0; // 累積和
	rep(i, n) {
		// いま s = Σa[0..i) = acc[i] なので，
		// Σa[0..j) = acc となる最大の j を k[i] として記録しておく．
		// なお j が存在しない場合は j = 0 となり，dp[0] = 0 なので問題ない．
		k[i] = acc_to_k[s];

		// Σa[0..i) = s であることを記録する．
		acc_to_k[s] = i;

		// 累積和 s を更新する．
		s += a[i];
	}

	// 貰う DP
	repi(i, 1, n - 1) {
		dp[i + 1] = 2 * dp[i] - dp[k[i]];
	}

	return dp[n];
}


