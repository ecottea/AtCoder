#pragma once
#include "header.h"
// ■■■■■ 最適化（列） ■■■■■


//【内積の最大化（広義単調非負実数列）】O(n)
/*
* 与えられた非負数列 a[0..n) に対し，和が s である広義単調減少非負実数列 x[0..n) で
* Σi a[i] x[i] を最大にするものを sol に格納し，そのときの最大値を返す．
*/
template <class T>
double maximize_inner_product_monotone_real(const vector<T>& a, T s, vd* sol = nullptr) {
	// 参考 : https://atcoder.jp/contests/arc128/editorial/2785
	// verify : https://atcoder.jp/contests/arc128/tasks/arc128_c

	//【方法】
	// a[0..n) の累積和 acc[i] = Σa[0..i) を導入し，
	//		m := argmax_i∈[1..n] acc[i] / i
	// と定めると，最適な x[0..n) は
	//		x[0..m) = s/m, x[m..n) = 0
	// である．

	//【証明】
	// ある x[0..n) において，i ≠ m かつ d := x[i-1] - x[i] > 0 なる i が存在した場合，
	//		x[0..i) -= d
	//		x[0..m) += d i / m
	// なる操作をすれば，内積を減少させることなく x[i-1] = x[i] とできる．
	// 実際，内積の増加量は
	//		-d acc[i] + (d i / m) acc[m]
	//		= (d i) (acc[m]/m - acc[i]/i)
	// であり，m の決め方よりこれは非負である．
	// この操作を繰り返せば，x[m-1] > x[m] 以外は定値であるような最適解が得られる．

	int n = sz(a);

	vector<T> acc(n + 1);
	rep(i, n) acc[i + 1] = acc[i] + a[i];

	double r = -1.; int m = -1;
	repi(i, 1, n) if (chmax(r, (double)acc[i] / i)) m = i;

	double x0 = (double)s / m;

	if (sol != nullptr) {
		sol->assign(m, x0);
		sol->resize(n, 0.);
	}

	return acc[m] * x0;
}


