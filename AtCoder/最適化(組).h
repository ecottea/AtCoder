#pragma once
#include "header.h"
#include "計算.h"
#include "関数.h"
// ■■■■■ 最適化（組） ■■■■■


//【組の差の最大化】O(n)
/*
* a[0..n) に対して以下の値を返す：
*		max_(i < j) (a[j] - a[i])
* また最大値を与える (i, j) を ids に格納する．
*/
template <class T>
T maximize_pair_diff(const vector<T>& a, pii* ids = nullptr) {
	//【方法】
	// 累積 min をもちながら左から線形走査すればいい．

	int n = sz(a);
	T res = numeric_limits<T>::lowest(), a_min = a[0]; int i_min = 0;
	if (ids == nullptr) ids = new pii;

	repi(i, 1, n - 1) {
		if (chmax(res, a[i] - a_min)) *ids = { i_min, i };
		if (chmin(a_min, a[i])) i_min = i;
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


//【組の内積の最小化】O((n + m) log n)
/*
* min_(i,j)∈[0..n)×[0..m) (a1[i], a2[i])・(b1[j], b2[j]) を返す．
* min_flag = false とすると最大値を返す．
*
* 制約 : b1[j] > 0 （∀j∈[0..m)）
*
* 利用：【Convex-Hull Trick】,【有理数】
*/
ll minimize_pair_inner_product(const vl& a1, const vl& a2, const vl& b1, const vl& b2, bool min_flag = true) {
	// verify : https://atcoder.jp/contests/arc051/tasks/arc051_d

	//【方法】
	// 式を整理すると
	//		min_(i,j) (a1[i], a2[i])・(b1[j], b2[j])
	//		= min_(i,j) (a1[i] b1[j] + a2[i] b2[j])
	//		= min_j (b1[j] min_i (a1[i] + a2[i] b2[j]/b1[j])) （b1[j] > 0 より）
	// となる．最後の式で i について最小化すべきものは，
	//		直線 y = a2[i] x + a1[i] の x = b2[j]/b1[j] における値
	// とみなせるから，CHT を用いて高速に求められる．

	int n = sz(a1), m = sz(b1);

	dump(a1, "\n", a2, "\n", b1, "\n", b2);
	Convex_hull_trick<Frac> cht(min_flag);

	// 直線群 {y = a2[i] x + a1[i]}_i で初期化する．
	rep(i, n) cht.insert(Frac(a2[i]), Frac(a1[i]));
	dump(cht);

	if (min_flag) {
		ll res = INFL;

		// 各 j について，x = b2[j]/b1[j] における最小値を求める．
		rep(j, m) {
			auto frac = cht.get(Frac(b2[j], b1[j])) * b1[j];
			frac.reduction();
			chmin(res, frac.num);
		}

		return res;
	}
	else {
		ll res = -INFL;

		// 各 j について，x = b2[j]/b1[j] における最大値を求める．
		rep(j, m) {
			auto frac = cht.get(Frac(b2[j], b1[j])) * b1[j];
			frac.reduction();
			chmax(res, frac.num);
		}

		return res;
	}
}


