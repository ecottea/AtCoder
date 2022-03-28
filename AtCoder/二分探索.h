#pragma once
#include "header.h"
#include "探索.h"
// ■■■■■ 二分探索を利用する問題 ■■■■■


//【濃度の最大化】O(n log(-EPS))
/*
* 質量 a[0..n) で濃度が p[0..n) の液体を k 個混ぜ合わせたときの最大濃度を返す．
* 
* 利用：【二分探索（実数）】
*/
double maximize_concentration(const vd& a, const vd& p, int k) {
	//verify : https://atcoder.jp/contests/abc034/tasks/abc034_d

	//【方法】
	// 目標とする濃度 c を決め打ちして二分探索すると，それが達成できるということは
	//		(Σi a[i] p[i]) / (Σi a[i]) >= c
	//		⇔ (Σi a[i] p[i]) >= c (Σi a[i])
	//		⇔ (Σi a[i] p[i]) - (Σi c a[i]) >= 0
	//		⇔ Σi a[i] (p[i] - c) >= 0
	// と同値変形できる．
	// 
	// したがって，左辺の値を最大化し，0 以上となるかを考えれば良い．
	// a[i] (p[i] - c) の値を降順に並べて始め k 個の和を取れば左辺を最大化できる．

	int n = sz(a);

	// 濃度 c が達成できるか
	function<bool(double)> okQ = [&](double c) {
		// d[i] : i 番目の液体の溶質の量と濃度 c だったときとの差
		vd d(n);

		rep(i, n) {
			d[i] = a[i] * (p[i] - c);
		}

		// 溶質の量が濃度 c に比べて多い順に並べる．
		sort(all(d), greater<double>());

		// 貪欲に k 個の液体を選ぶ．
		double sum = accumulate(d.begin(), d.begin() + k, 0.);

		// 溶質が足りているなら濃度 c を達成可能．
		return sum >= 0;
	};

	// 達成可能か不可能かの境目となる濃度を得る．
	double res = binary_search(0., 100., okQ);

	return res;
}


//【互いに異なるカードの選択】O(n log n)
/*
* n 種のカードが各 c[i] 枚ある．互いに異なる k 枚のカードを最大何回抽出できるかを返す．
*
* 利用：【めぐる式二分探索】
*/
ll all_different_select(vl c, ll k) {
	int n = sz(c);

	// 枚数昇順にソートする．
	sort(all(c));

	// 累積枚数
	vl acc(n + 1);
	rep(i, n) acc[i + 1] = acc[i] + c[i];

	// x 回抽出できるか
	function<bool(ll)> okQ = [&](ll x) {
		// x 枚以上あるカードについては，そのうち x 枚しか使えないので，
		// そのようなカードの種類数 * x で枚数が求まる．
		// それ以外については全て使えるので下からの累積和で枚数が求まる．
		int i = distance(c.begin(), lower_bound(all(c), x));
		ll sel = acc[i] + (n - i) * x;

		// 使えるカードの枚数 sel が使いたいカードの枚数 k * x 以上なら抽出可能．
		return sel / k >= x;
	};

	ll res = meguru_search(0LL, accumulate(all(c), 0LL) / k + 1, okQ);

	return res;
}


