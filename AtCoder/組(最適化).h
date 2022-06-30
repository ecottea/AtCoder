#pragma once
#include "header.h"
// ■■■■■ 組，区間の最小化[最大化] ■■■■■


//【組の差の最大化】O(n)
/*
* a[0..n) に対して以下の値を返す：
*		max_(i < j) (a[j] - a[i])
* また最大値を与える (i, j) を ids に格納する．
*/
template <class T> T maximize_pair_diff(const vector<T>& a, pii* ids = nullptr) {
	//【方法】
	// 累積 min をもちながら左から線形走査すればいい．

	int n = sz(a);
	T res = numeric_limits<T>::min(), a_min = a[0]; int i_min = 0;
	if (ids == nullptr) ids = new pii;

	repi(i, 1, n - 1) {
		if (chmax(res, a[i] - a_min)) *ids = { i_min, i };
		if (chmin(a_min, a[i])) i_min = i;
	}

	return res;
}


//【区間の和の最大化】O(n)
/*
* a[0..n) に対して以下の値を返す：
*		max_(i < j) Σa[i..j)
* また最大値を与える (i, j) を ids に格納する．
*/
ll maximize_interval_sum(const vl& a, pii* ids = nullptr) {
	// verify : https://atcoder.jp/contests/dwango2016-prelims/tasks/dwango2016qual_d

	//【方法】
	// 累積和 acc[i] = Σa[0..i) を導入すれば，求める値は以下のように表される：
	//		max_(i < j) (acc[j] - acc[i])
	// すなわち【組の差の最大化】に帰着する．

	int n = sz(a);

	vl acc(n + 1);
	rep(i, n) acc[i + 1] = acc[i] + a[i];

	ll res = -INFL, acc_min = acc[0]; int i_min = 0;
	if (ids == nullptr) ids = new pii;

	repi(i, 1, n) {
		if (chmax(res, acc[i] - acc_min)) *ids = { i_min, i };
		if (chmin(acc_min, acc[i])) i_min = i;
	}

	return res;
}


//【組の LCM の最小化】O(A log A)（A = max(a)）
/*
* a[0..n) > 0 に対して以下の値を返す：
*		min_(i < j) LCM(a[i], a[j])
* また最小値を与える (i, j) を ids に格納する．
*/
ll minimize_pair_lcm(const vi& a, pii& ids) {
	// 参考 : https://drken1215.hatenablog.com/entry/2019/04/17/193100
	// verify : https://codeforces.com/contest/1154/problem/G

	//【方法】
	// g = GCD(a[i], a[j]) を決め打ち全探索する．
	// このとき LCM(a[i], a[j]) の最小化は積 a[i]a[j] の最小化と同値である．
	// 
	// そこで，g の倍数であるような a[0..n) を昇順に 2 つ選ぶことにする．
	// これでは g | GCD(a[i], a[j]) となることしか保証されないが，
	// より大きい g を探索したときに発見されるものを先に見つけただけなので問題ない．

	int n = sz(a);

	int a_max = *max_element(all(a));
	vi cnt(a_max + 1);
	rep(i, n) cnt[a[i]]++;

	ll res = INFL, ai_min = -1, aj_min = -1;

	// g : GCD(a[i], a[j])
	repi(g, 1, a_max) {
		int ai = -1, aj = -1;

		for (int v = g; v <= a_max; v += g) {
			if (cnt[v] == 0) continue;

			if (cnt[v] == 1) {
				if (ai == -1) ai = v;
				else if (aj == -1) {
					aj = v;
					break;
				}
			}
			else {
				if (ai == -1) ai = v;
				if (aj == -1) aj = v;
				break;
			}
		}

		if (aj == -1) continue;

		if (chmin(res, (ll)ai * aj / g)) {
			ai_min = ai;
			aj_min = aj;
		}
	}

	rep(i, n) {
		if (a[i] == ai_min) {
			ids.first = i;
			break;
		}
	}
	rep(j, n) {
		if (j != ids.first && a[j] == aj_min) {
			ids.second = j;
			break;
		}
	}

	return res;
}


